/*
 * Motor.c
 *
 *  Created on: 2026年6月11日
 *      Author: zr186
 */

#include "../Inc/Motor.h"

#include "../../DriveLayer/Inc/UART2.h"
#include "../../Core/Inc/adc.h"
#include "../../DriveLayer/Inc/Hall_Sensor.h"
#include "../../DriveLayer/Inc/DataReception.h"
#include "../Inc/DataReception.h"
#include "../../MiddleLayer/Inc/clark_transformation.h"
#include "../../MiddleLayer/Inc/park_transformation.h"
#include "../../MiddleLayer/Inc/svpwm.h"


Motor_Handle motor_M0, motor_M1;
IncrementalPID_Handle Current_Loop_Iq_M0;
IncrementalPID_Handle Current_Loop_Id_M0;
IncrementalPID_Handle Current_Loop_Iq_M1;
IncrementalPID_Handle Current_Loop_Id_M1;
IncrementalPID_Handle Speed_Loop_M0;
IncrementalPID_Handle Speed_Loop_M1;
uint32_t Current_ADC[2] = {0};

static void Clark_forward_transformation(Motor_Handle *Pmotor);
static void Park_forward_transformation(Motor_Handle *Pmotor);
static void Electrical_Angle_Get(Motor_Handle *Pmotor);
static void Park_inverse_transformation(Motor_Handle *Pmotor);
static void Current_Loop(Motor_Handle *Pmotor);
static void Speed_LooP(Motor_Handle *Pmotor);


void Motor_Init(void){
	EG2134_Init();
	motor_M0.motorDrive = &EG2134_M0;
	motor_M0.hall = &HALL_M0;
	motor_M0.Iq_LOOP = &Current_Loop_Iq_M0;
	motor_M0.Id_LOOP = &Current_Loop_Id_M0;
	motor_M0.Speed_Loop = &Speed_Loop_M0;
	IncrementalPID_ParameterSet(motor_M0.Iq_LOOP, 1.1, 0.05, 0);
	motor_M0.Iq_LOOP->MAX_Output =300;
	motor_M0.Iq_LOOP->MIN_Output = -300;
	IncrementalPID_ParameterSet(motor_M0.Id_LOOP, 1.1, 0.05, 0);
	motor_M0.Id_LOOP->MAX_Output = 300;
	motor_M0.Id_LOOP->MIN_Output = -300;
	IncrementalPID_ParameterSet(motor_M0.Speed_Loop, 2.5, 0.055, 0.05);
	motor_M0.Speed_Loop->MAX_Output = 260;
	motor_M0.Speed_Loop->MIN_Output = -260;
	motor_M0.Current_PhaseA_BAIS = 2048;
	motor_M0.Current_PhaseB_BAIS = 2018;
	motor_M0.Current_PhaseC_BAIS = 2048;
	motor_M0.Current_PhaseA_GAIN = 1.0f;
	motor_M0.Current_PhaseB_GAIN = 0.915385f;
	motor_M0.Current_PhaseC_GAIN = 1.0f;

	motor_M1.motorDrive = &EG2134_M1;
	motor_M1.hall = &HALL_M1;
	motor_M1.Iq_LOOP = &Current_Loop_Iq_M1;
	motor_M1.Id_LOOP = &Current_Loop_Id_M1;
	motor_M1.Speed_Loop = &Speed_Loop_M1;
	IncrementalPID_ParameterSet(motor_M1.Iq_LOOP, 1.1, 0.05, 0);
	motor_M1.Iq_LOOP->MAX_Output = 300;
	motor_M1.Iq_LOOP->MIN_Output = -300;
	IncrementalPID_ParameterSet(motor_M1.Id_LOOP, 1.1, 0.05, 0);
	motor_M1.Id_LOOP->MAX_Output = 300;
	motor_M1.Id_LOOP->MIN_Output = -300;
	IncrementalPID_ParameterSet(motor_M1.Speed_Loop, 2.5, 0.055, 0.05);
	motor_M1.Speed_Loop->MAX_Output = 260;
	motor_M1.Speed_Loop->MIN_Output = -260;
	motor_M1.Current_PhaseA_BAIS = 2048;
	motor_M1.Current_PhaseB_BAIS = 2024;
	motor_M1.Current_PhaseC_BAIS = 2048;
	motor_M1.Current_PhaseA_GAIN = 1.0f;
	motor_M1.Current_PhaseB_GAIN = 0.97143f;
	motor_M1.Current_PhaseC_GAIN = 1.0303f;

	Hall_Init();
	DataReception_Init();
	HAL_Delay(1000);
	HAL_ADCEx_MultiModeStart_DMA(&hadc1, Current_ADC, 2);
	HAL_ADC_Start(&hadc2); // ADC2仅开启转换，不开启DMA
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc == &hadc1) {
    	motor_M0.Current_PhaseB = (int)(motor_M0.Current_PhaseB_GAIN * (float)((int16_t)(Current_ADC[0] & 0xFFFF) - motor_M0.Current_PhaseB_BAIS));   // ADC1 CH10
    	motor_M0.Current_PhaseC = (int)(motor_M0.Current_PhaseC_GAIN * (float)((int16_t)(Current_ADC[0] >> 16) - motor_M0.Current_PhaseC_BAIS));       // ADC2 CH11
    	motor_M0.Current_PhaseA = (int)(-1 * motor_M0.Current_PhaseB - motor_M0.Current_PhaseC);
    	motor_M1.Current_PhaseC = (int)(motor_M1.Current_PhaseC_GAIN * (float)((int16_t)(Current_ADC[1] & 0xFFFF) - motor_M1.Current_PhaseC_BAIS));   // ADC1 CH12
    	motor_M1.Current_PhaseB = (int)(motor_M1.Current_PhaseB_GAIN * (float)((int16_t)(Current_ADC[1] >> 16) - motor_M1.Current_PhaseB_BAIS));       // ADC2 CH13
    	motor_M1.Current_PhaseA = (int)(-1 * motor_M1.Current_PhaseC - motor_M1.Current_PhaseB);
    }
	Current_Loop(&motor_M0);
	Current_Loop(&motor_M1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim6){
		HALL_TIM_Supervised_Update(motor_M0.hall);
		HALL_TIM_Supervised_Update(motor_M1.hall);
		DataReception_TIM_Supervised();
	}
	if(htim == &htim1){
	    if(READ_BIT(TIM1->CR1, TIM_CR1_DIR)){   // DIR=1 → 正在向下计数 → 刚溢出
			__HAL_ADC_CLEAR_FLAG(&hadc1, (ADC_FLAG_EOC | ADC_FLAG_OVR));
	        ADC1->CR2 |= ADC_CR2_SWSTART;

	    }
	}
	if(htim == &htim7){
		//TIM7用于提供更高精度的时钟，是systick的20倍，即20kHz。
		TIM7_PeriodElapsedCallback();
	}

	if(htim == &htim10){
		Speed_LooP(&motor_M0);
		Speed_LooP(&motor_M1);
	}
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	HALL_EXTI_Callback(motor_M0.hall, GPIO_Pin);
	HALL_EXTI_Callback(motor_M1.hall, GPIO_Pin);
	DataReception_EXTI_CallBack(&DataReception, GPIO_Pin);
}

/*
 * @功能：克拉克正变换
 * @参数：要变换的对象电机句柄
 * @返回：无
 * */
static void Clark_forward_transformation(Motor_Handle *Pmotor){
	Clark.PhaseA = Pmotor->Current_PhaseA;
	Clark.PhaseB = Pmotor->Current_PhaseB;
	Clark.PhaseC = Pmotor->Current_PhaseC;
	Clark_transformation(Clark_Forward);
	Pmotor->Current_PhaseAlpha = Clark.PhaseAlpha;
	Pmotor->Current_PhaseBeta = Clark.PhaseBeta;
}

/*
 * @功能：帕克正变换
 * @参数：要变换的对象电机句柄
 * @返回：无
 * */
static void Park_forward_transformation(Motor_Handle *Pmotor){
	Park.PhaseAlpha = Pmotor->Current_PhaseAlpha;
	Park.PhaseBeta = Pmotor->Current_PhaseBeta;
	Park.PhaseTheta = Pmotor->HALL_Angle;
	Park_transformation(Park_Forward);
	Pmotor->Current_PhaseD = Park.PhaseD;
	Pmotor->Current_PhaseQ = Park.PhaseQ;
}



/*
 * @功能：帕克逆变换
 * @参数：要变换的对象电机句柄
 * @返回：无
 * */
static void Park_inverse_transformation(Motor_Handle *Pmotor){
	Park.PhaseD = (int)Pmotor->Id_LOOP->Output;
	Park.PhaseQ = (int)Pmotor->Iq_LOOP->Output;
	Park.PhaseTheta = Pmotor->HALL_Angle;
	Park_transformation(Park_Inverse);
	Pmotor->Voltage_PhaseAlpha = Park.PhaseAlpha;
	Pmotor->Voltage_PhaseBeta = Park.PhaseBeta;
}

/*
 * @功能：霍尔角度插值，计算电机的电角度，前提是霍尔扇区和电机转速一定要正确
 * @参数：要获取电角度的电机对象句柄
 * @返回：无
 * */
static void Electrical_Angle_Get(Motor_Handle *Pmotor){
	uint32_t currentTime = TIM7_GetTick();
	int32_t Offset_Theta = (int32_t)((currentTime - Pmotor->hall->LastTime) * 60) / (int32_t)(Pmotor->hall->Pulse_Period);
	uint8_t encoding = Pmotor->hall->Encoding ;//扇区解码
	Pmotor->HALL_Angle = (int16_t)((encoding % 6) * 60 + (Offset_Theta % 60));
}


static void Current_Loop(Motor_Handle *Pmotor){
	if(Pmotor->State == Motor_bark){
		Pmotor->motorDrive->Duty_cycle_A = 0;
		Pmotor->motorDrive->Duty_cycle_B = 0;
		Pmotor->motorDrive->Duty_cycle_C = 0;
		Pmotor->Iq_LOOP->Out_Last = 0;
		Pmotor->Iq_LOOP->Output = 0;
    	Pmotor->Id_LOOP->Out_Last = 0;
		Pmotor->Id_LOOP->Output = 0;
		EG2134_PWM_Compare_Update(Pmotor->motorDrive);
		return;
	}else if(Pmotor->State == Motor_speedlose){

	}else if(Pmotor->State == Motor_currentlose){

	}
    if(Pmotor->Iq_LOOP->Expactation_value > -10 && Pmotor->Iq_LOOP->Expactation_value < 10){
    	Pmotor->Iq_LOOP->Out_Last = 0;
    	Pmotor->Iq_LOOP->Output = 0;
    	Pmotor->Id_LOOP->Out_Last = 0;
        Pmotor->Id_LOOP->Output = 0;
    }
    Clark_forward_transformation(Pmotor);
    Electrical_Angle_Get(Pmotor);
    Park_forward_transformation(Pmotor);
    IncrementalPID_Output(Pmotor->Iq_LOOP, Pmotor->Iq_LOOP->Expactation_value, (float)Pmotor->Current_PhaseQ);
    IncrementalPID_Output(Pmotor->Id_LOOP, Pmotor->Id_LOOP->Expactation_value, (float)Pmotor->Current_PhaseD);
    Park_inverse_transformation(Pmotor);
    SVPWM.PhaseAlpha = Pmotor->Voltage_PhaseAlpha;
    SVPWM.PhaseBeta = Pmotor->Voltage_PhaseBeta;
	SVPWM.MaxDuty  = 1679;
	SVPWM.DeadTime = 30;
    SVPWM_Generate();
    Pmotor->FOC_Angle = SVPWM.Theta;
    Pmotor->motorDrive->Duty_cycle_A = SVPWM.Duty_A;
    Pmotor->motorDrive->Duty_cycle_B = SVPWM.Duty_B;
    Pmotor->motorDrive->Duty_cycle_C = SVPWM.Duty_C;
	EG2134_PWM_Compare_Update(Pmotor->motorDrive);
}


static void Speed_LooP(Motor_Handle *Pmotor){
	if(Pmotor->State == Motor_bark){
		Pmotor->Speed_Loop->Output = 0;
		Pmotor->Speed_Loop->Out_Last = 0;
		return;
	}else if(Pmotor->State == Motor_currentlose){
		return;
	}else if(Pmotor->State == Motor_speedlose){
		if(Pmotor->Speed_Loop->Expactation_value > -1 && Pmotor->Speed_Loop->Expactation_value < 1){
			Pmotor->Speed_Loop->Output = 0;
			Pmotor->Speed_Loop->Out_Last = 0;
			Pmotor->Iq_LOOP->Expactation_value = Pmotor->Speed_Loop->Output;
			return;
		}
		float Actual_Speed = Pmotor->hall->Pulse_Frequency;//每秒转多少圈
		IncrementalPID_Output(Pmotor->Speed_Loop, Pmotor->Speed_Loop->Expactation_value, Actual_Speed);
		Pmotor->Id_LOOP->Expactation_value = 0;
		float Iq_correction = Pmotor->Speed_Loop->Expactation_value * 0.8f;
		if(Iq_correction > 0) Iq_correction += 50;
		else Iq_correction -= 50;
		Pmotor->Speed_Loop->Output += Iq_correction;
		Pmotor->Iq_LOOP->Expactation_value = Pmotor->Speed_Loop->Output;
		return;
	}

}


