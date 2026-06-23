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
#include "../../MiddleLayer/Inc/clark_transformation.h"
#include "../../MiddleLayer/Inc/svpwm.h"

Motor_Handle motor_M0, motor_M1;
uint32_t Current_ADC[2] = {0};

static void Clark_forward_transformation(Motor_Handle *Pmotor);


void Motor_Init(void){
	EG2134_Init();

	motor_M0.motorDrive = &EG2134_M0;
	motor_M0.hall = &HALL_M0;


	motor_M1.motorDrive = &EG2134_M1;
	motor_M1.hall = &HALL_M1;

	HAL_ADC_Start(&hadc2); // ADC2仅开启转换，不开启DMA
	HAL_ADCEx_MultiModeStart_DMA(&hadc1, Current_ADC, 2);

	Hall_Init();
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc == &hadc1) {
    	motor_M0.Current_PhaseB = (int)((int16_t)(Current_ADC[0] & 0xFFFF) - 2048);   // ADC1 CH10
    	motor_M0.Current_PhaseC = (int)((int16_t)(Current_ADC[0] >> 16) - 2048);       // ADC2 CH11
    	motor_M0.Current_PhaseA = (int)(-1 * motor_M0.Current_PhaseB - motor_M0.Current_PhaseC);
    	motor_M1.Current_PhaseC = (int)((int16_t)(Current_ADC[1] & 0xFFFF) - 2048);   // ADC1 CH12
    	motor_M1.Current_PhaseB = (int)((int16_t)(Current_ADC[1] >> 16) - 2048);       // ADC2 CH13
    	motor_M1.Current_PhaseA = (int)(-1 * motor_M1.Current_PhaseC - motor_M1.Current_PhaseB);
    }
    //Clark_forward_transformation(&motor_M0);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim6){
		SVPWM_Test(motor_M0.motorDrive, 150, 1);
		SVPWM_Test(motor_M1.motorDrive, 200, 1.5);
	}
	if(htim == &htim1){
	    if(READ_BIT(TIM1->CR1, TIM_CR1_DIR)){   // DIR=1 → 正在向下计数 → 刚溢出
			__HAL_ADC_CLEAR_FLAG(&hadc1, (ADC_FLAG_EOC | ADC_FLAG_OVR));
	        ADC1->CR2 |= ADC_CR2_SWSTART;
	    }
	}
	if(htim == &htim7){
		TIM7_PeriodElapsedCallback();
	}
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == motor_M0.hall->ENC_A_PIN || GPIO_Pin == motor_M0.hall->ENC_B_PIN || GPIO_Pin == motor_M0.hall->ENC_Z_PIN){
		Hall_GetEncoding(motor_M0.hall);
		Hall_GetPeriod(motor_M0.hall);
	}
	if(GPIO_Pin == motor_M1.hall->ENC_A_PIN || GPIO_Pin == motor_M1.hall->ENC_B_PIN || GPIO_Pin == motor_M1.hall->ENC_Z_PIN){
		Hall_GetEncoding(motor_M1.hall);
		Hall_GetPeriod(motor_M1.hall);
	}
}


static void Clark_forward_transformation(Motor_Handle *Pmotor){
	Clark.PhaseA = Pmotor->Current_PhaseA;
	Clark.PhaseB = Pmotor->Current_PhaseB;
	Clark.PhaseC = Pmotor->Current_PhaseC;
	Clark_transformation(Clark_Forward);
	Pmotor->Current_PhaseAlpha = Clark.PhaseAlpha;
	Pmotor->Current_PhaseBeta = Clark.PhaseBeta;
}
