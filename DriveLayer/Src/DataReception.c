/*
 * DataReception.c
 *
 *  Created on: 2026年7月2日
 *      Author: zr186
 */
#include "main.h"
#include "../Inc/Datareception.h"
#include "../Inc/Motor.h"
#include "../../MiddleLayer/Inc/PID.h"
#include <string.h>

DataReception_Handle DataReception;
static void Motor_State_Updata(void);
static void DataReception_AngleVelocity_Get(void);

void DataReception_Init(void){
	DataReception.CS_GPIOx = SPI_CS_GPIO_Port;
	DataReception.CS_PIN = SPI_CS_Pin;
	DataReception.MISO_GPIOx = SPI_MISO_GPIO_Port;
	DataReception.MISO_PIN = SPI_MISO_Pin;
	DataReception.MOSI_GPIOx = SPI_MOSI_GPIO_Port;
	DataReception.MOSI_PIN = SPI_MOSI_Pin;
	DataReception.SCK_GPIOx = SPI_CLK_GPIO_Port;
	DataReception.SCK_PIN = SPI_CLK_Pin;

	DataReception.State_Data = 0;
	DataReception.Joystick_Data = 0;
	DataReception.Index = 0;
}


void DataReception_EXTI_CallBack(DataReception_Handle *pdatareception, uint16_t GPIO_Pin){
	if(GPIO_Pin != pdatareception->SCK_PIN)	return;
	if(HAL_GPIO_ReadPin(pdatareception->CS_GPIOx, pdatareception->CS_PIN) == GPIO_PIN_SET){
   		pdatareception->Index = 0;
		pdatareception->Forward_value = (int16_t)((pdatareception->Joystick_Data & 0xFFFF0000) >> 16);
		pdatareception->Turn_value = (int16_t)(pdatareception->Joystick_Data & 0xFFFF);
   		Motor_State_Updata();
		pdatareception->State_Data = 0;
		pdatareception->Joystick_Data = 0;
		return;
	}
	GPIO_PinState S0_State = HAL_GPIO_ReadPin(pdatareception->MISO_GPIOx, pdatareception->MISO_PIN);
	GPIO_PinState S1_State = HAL_GPIO_ReadPin(pdatareception->MOSI_GPIOx, pdatareception->MOSI_PIN);
	if(S0_State == GPIO_PIN_SET)	pdatareception->State_Data |= 0x80000000 >> pdatareception->Index;
	if(S1_State == GPIO_PIN_SET)	pdatareception->Joystick_Data |= 0x80000000 >> pdatareception->Index;
	pdatareception->Index++;
}

static void Motor_State_Updata(void){
	switch(DataReception.State_Data & 0x0007){
	case 0x0001:
		motor_M0.State = Motor_speedlose;
		motor_M0.Speed_Loop->Expactation_value = -1 * (DataReception.Forward_value / 3 + DataReception.Turn_value / 28);
		motor_M1.State = Motor_speedlose;
		motor_M1.Speed_Loop->Expactation_value = (DataReception.Forward_value / 3 - DataReception.Turn_value / 28);

		break;
	case 0x0002:
		motor_M0.State = Motor_currentlose;
		motor_M0.Iq_LOOP->Expactation_value = -1 * (DataReception.Forward_value + DataReception.Turn_value);
		if(motor_M0.Iq_LOOP->Expactation_value > motor_M0.Speed_Loop->MAX_Output) motor_M0.Iq_LOOP->Expactation_value = motor_M0.Speed_Loop->MAX_Output;
		if(motor_M0.Iq_LOOP->Expactation_value < motor_M0.Speed_Loop->MIN_Output) motor_M0.Iq_LOOP->Expactation_value = motor_M0.Speed_Loop->MIN_Output;
		motor_M1.State = Motor_currentlose;
		motor_M1.Iq_LOOP->Expactation_value = (DataReception.Forward_value - DataReception.Turn_value);
		if(motor_M1.Iq_LOOP->Expactation_value > motor_M1.Speed_Loop->MAX_Output) motor_M1.Iq_LOOP->Expactation_value = motor_M1.Speed_Loop->MAX_Output;
		if(motor_M1.Iq_LOOP->Expactation_value < motor_M1.Speed_Loop->MIN_Output) motor_M1.Iq_LOOP->Expactation_value = motor_M1.Speed_Loop->MIN_Output;
		break;
	case 0x0004:
		motor_M0.State = Motor_bark;
		motor_M1.State = Motor_bark;
		break;
	default:
		break;
	}
}


static void DataReception_AngleVelocity_Get(void){

}

void DataReception_TIM_Supervised(void){
	if(motor_M0.State == Motor_bark) return;
	if(HAL_GetTick() > DataReception.lastTime + 500){
		motor_M0.State = Motor_bark;
		motor_M1.State = Motor_bark;
	}
}
