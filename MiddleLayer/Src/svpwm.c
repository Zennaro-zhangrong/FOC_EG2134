/*
 * SVPWM.c
 *
 *  Created on: 2026年6月18日
 *      Author: zr186
 */

#include "../Inc/svpwm.h"
#include "../../DriveLayer/Inc/Motor.h"
#include "stm32f4xx_hal.h"
#include "math.h"
#include "../Inc/park_transformation.h"


#define SQRT3_DIV2   0.86602540378f
#define ONE_HALF     0.5f

SVPWM_Handle SVPWM;

static void SVPWM_calculate(uint16_t amplitude, int32_t angle);
static void SVPWM_Polar_Coordinate(uint16_t amplitude, int32_t angle);
static void SVPWM_Rectangular_Coordinate(void);


void SVPWM_Generate(void)
{
	SVPWM_Rectangular_Coordinate();
}


/**
 * @brief SVPWM 七段式空间矢量调制
 *
 * 输入: PhaseAlpha, PhaseBeta (来自逆Park变换的αβ轴电压)
 * 输出: Duty_A, Duty_B, Duty_C (三相占空比, 范围 0 ~ MaxDuty)
 *
 * MaxDuty = PWM周期计数值 (1680)
 * DeadTime = 死区时间 (25个定时器周期), 由调用方在外部补偿
 */
static void SVPWM_Polar_Coordinate(uint16_t amplitude, int32_t angle)
{
	/* 第1步: 扇区判定(1~6) :扇区定义：[0,60]->1,[60,120]->2,...,[300,360]->6*/
	uint8_t sector = 0;
	while(angle <= 0)		{	angle += 36000000;	}
	while(angle > 36000000){	angle -= 36000000;	}
	while(angle >= 360000)	{	angle -= 360000;	}
	while(angle >= 3600)	{	angle -= 3600;		}
	while(angle >= 60){
		angle -= 60;
		sector++;
	}
	SVPWM.sector = (sector % 6) + 1;
	SVPWM.Theta = (SVPWM.sector - 1) * 60 + angle;
	/* 第2步: 矢量分解，求Vfirst，Vsecond的模长 */
	float T_Vfirst = amplitude
			* sin((M_PI / 3.0f) - (float)angle * M_PI / 180.0f)
			/ sin(M_PI / 3.0f);

	float T_Vsecond = amplitude
			* sin((float)angle * M_PI / 180.0f)
			/ sin(M_PI / 3.0f);

	/* 第3步: 查表求第一矢量Vfirst和第二矢量Vsecond*/
	uint8_t Vfirst, Vsecond;
	switch(SVPWM.sector) {
		case 1: Vfirst = 0x01; Vsecond = 0x03; break;
		case 2: Vfirst = 0x03; Vsecond = 0x02; break;
		case 3: Vfirst = 0x02; Vsecond = 0x06; break;
		case 4: Vfirst = 0x06; Vsecond = 0x04; break;
		case 5: Vfirst = 0x04; Vsecond = 0x05; break;
		case 6: Vfirst = 0x05; Vsecond = 0x01; break;
		default:
			SVPWM.Duty_A = 0;
			SVPWM.Duty_B = 0;
			SVPWM.Duty_C = 0;
			return;
	}//第0位表示A相，第1位表示B相，第2位表示C相

	/* 第4步: 计算 T1(第一有效矢量时间), T2(第二有效矢量时间) */
	float t1 = 0, t2 = 0;
	t1 = (float)T_Vfirst;
	t2 = (float)T_Vsecond;

	/* 过调制限幅 */
	if (t1 + t2 > SVPWM.MaxDuty) {
		float scale = SVPWM.MaxDuty / (t1 + t2);
		t1 *= scale;
		t2 *= scale;
	}

	/* 第5步: 计算三相占空比 (七段式, 零矢量对称分布) */
	uint16_t T0_2 = (SVPWM.MaxDuty - t1 - t2) * 0.5f;
	uint16_t Ta = T0_2 + (Vfirst & 0x01) * t1 + (Vsecond & 0x01) * t2;
	uint16_t Tb = T0_2 + (Vfirst & 0x02) * t1 * 0.5 + (Vsecond & 0x02) * t2 * 0.5;
	uint16_t Tc = T0_2 + (Vfirst & 0x04) * t1 * 0.25 + (Vsecond & 0x04) * t2 * 0.25;

	/* 第6步: 实际三相输出 */
	SVPWM.Duty_A = Ta;
	SVPWM.Duty_B = Tb;
	SVPWM.Duty_C = Tc;
}




void SVPWM_Test(EG2134_Handle *pEG2134, unsigned short Amplitude, float Frequency){
	/* ---- 0. 初始化 SVPWM 参数 ---- */
	SVPWM.MaxDuty  = 1679;
	SVPWM.DeadTime = 30;

	int32_t theta_deg = fmodf(15 * Frequency * 360.0f * HAL_GetTick() * 0.001f, 360.0f);
	while(theta_deg > 360000){
		theta_deg -= 360000;
	}
	while(theta_deg <= 0){
		theta_deg += 360000;
	}
	SVPWM_Polar_Coordinate(Amplitude, (int32_t)theta_deg);

	/* ---- 4. 更新 PWM ---- */
	pEG2134->Duty_cycle_A = (uint16_t)SVPWM.Duty_A;
	pEG2134->Duty_cycle_B = (uint16_t)SVPWM.Duty_B;
	pEG2134->Duty_cycle_C = (uint16_t)SVPWM.Duty_C;
	EG2134_PWM_Compare_Update(pEG2134);
}


static void SVPWM_Rectangular_Coordinate(void){
	int alpha = SVPWM.PhaseAlpha;
	int beta = SVPWM.PhaseBeta;
	double radian = atan2((double)beta, (double)alpha);
	int32_t angle = (radian + 3.1415926f * 2.0f) * 180.0f / 3.1415926f;
	uint16_t Amplitude = (uint16_t)sqrt((double)alpha * alpha + (double)beta * beta);
	SVPWM_Polar_Coordinate(Amplitude, angle);
}
