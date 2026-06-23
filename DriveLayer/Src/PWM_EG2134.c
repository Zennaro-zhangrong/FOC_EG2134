/*
 * PWM_EG2134.c
 *
 *  Created on: 2026年6月9日
 *      Author: zr186
 */

#include "../Inc/PWM_EG2134.h"
#include "math.h"

static void PWM_HAL_Init(EG2134_Handle *pEG2134);


EG2134_Handle EG2134_M0, EG2134_M1;


void EG2134_Init(void)
{
	EG2134_M0.htim = &htim1;
	EG2134_M0.PWM_CH_A = TIM_CHANNEL_1;
	EG2134_M0.PWM_CH_B = TIM_CHANNEL_2;
	EG2134_M0.PWM_CH_C = TIM_CHANNEL_3;
	EG2134_M0.Duty_cycle_A = 0;
	EG2134_M0.Duty_cycle_B = 0;
	EG2134_M0.Duty_cycle_C = 0;

	EG2134_M1.htim = &htim8;
	EG2134_M1.PWM_CH_A = TIM_CHANNEL_1;
	EG2134_M1.PWM_CH_B = TIM_CHANNEL_2;
	EG2134_M1.PWM_CH_C = TIM_CHANNEL_3;
	EG2134_M1.Duty_cycle_A = 0;
	EG2134_M1.Duty_cycle_B = 0;
	EG2134_M1.Duty_cycle_C = 0;

	PWM_HAL_Init(&EG2134_M0);
	PWM_HAL_Init(&EG2134_M1);
}


void EG2134_PWM_Compare_Update(EG2134_Handle *pEG2134){
	__HAL_TIM_SET_COMPARE(pEG2134->htim, pEG2134->PWM_CH_A, pEG2134->Duty_cycle_A);
	__HAL_TIM_SET_COMPARE(pEG2134->htim, pEG2134->PWM_CH_B, pEG2134->Duty_cycle_B);
	__HAL_TIM_SET_COMPARE(pEG2134->htim, pEG2134->PWM_CH_C, pEG2134->Duty_cycle_C);
}



void EG2134_Test(EG2134_Handle *pEG2134, uint16_t Amplitude, float Frequency){
	static float FrequencyMin = -10;
	static float FrequencyMax = 10;
	if(Frequency > FrequencyMax) Frequency = FrequencyMax;
	else if(Frequency < FrequencyMin) Frequency = FrequencyMin;
	static uint32_t lasttime = 0;
	uint32_t currenttime = HAL_GetTick();
	if(currenttime > lasttime + 1){
		pEG2134->Duty_cycle_A = (uint16_t)(Amplitude * sinf(0.024 * 3.1415 * Frequency * currenttime - 0.666 * 3.1415) + 840);
		pEG2134->Duty_cycle_B = (uint16_t)(Amplitude * sinf(0.024 * 3.1415 * Frequency * currenttime) + 840);
		pEG2134->Duty_cycle_C = (uint16_t)(Amplitude * sinf(0.024 * 3.1415 * Frequency * currenttime + 0.666 * 3.1415) + 840);
		EG2134_PWM_Compare_Update(pEG2134);
		lasttime = currenttime;
	}

}


/*
 * @功能：初始化硬件PWM通道，前提是tim配置好pwm三通道互补输出
 * @参数：EG2134句柄地址
 * @注意：使用前一定要先初始化句柄，否则空地址或者无效数据会使此函数初始化失败*/
static void PWM_HAL_Init(EG2134_Handle *pEG2134){
	// 主输出通道
	HAL_TIM_PWM_Start(pEG2134->htim, pEG2134->PWM_CH_A);
	HAL_TIM_PWM_Start(pEG2134->htim, pEG2134->PWM_CH_B);
	HAL_TIM_PWM_Start(pEG2134->htim, pEG2134->PWM_CH_C);
	// 互补输出通道
	HAL_TIMEx_PWMN_Start(pEG2134->htim, pEG2134->PWM_CH_A);
	HAL_TIMEx_PWMN_Start(pEG2134->htim, pEG2134->PWM_CH_B);
	HAL_TIMEx_PWMN_Start(pEG2134->htim, pEG2134->PWM_CH_C);
}
