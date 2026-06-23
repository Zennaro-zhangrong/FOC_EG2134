/*
 * Hall_Sensor.c
 *
 *  Created on: 2026年6月22日
 *      Author: zr186
 */

#include "../Inc/Hall_Sensor.h"
#include "main.h"
#include "../../Core/Inc/tim.h"
HALL_Handle HALL_M0, HALL_M1;
static uint8_t HALL_SectorEncoding(uint8_t HALL_Encoding);

void Hall_Init(void){

	static int16_t HALL_M0_Average_Buff[12] = {0};
	HALL_M0.Average_Length = 12;
	HALL_M0.Average_Period = HALL_M0_Average_Buff;
	HALL_M0.ENC_A_GPIOx = M0_ENC_A_GPIO_Port;
	HALL_M0.ENC_A_PIN = M0_ENC_A_Pin;
	HALL_M0.ENC_B_GPIOx = M0_ENC_B_GPIO_Port;
	HALL_M0.ENC_B_PIN = M0_ENC_B_Pin;
	HALL_M0.ENC_Z_GPIOx = M0_ENC_Z_GPIO_Port;
	HALL_M0.ENC_Z_PIN = M0_ENC_Z_Pin;
	HALL_M0.LastTime = HAL_GetTick();
	HALL_M0.Encoding = 0x00;
	Hall_GetEncoding(&HALL_M0);

	static int16_t HALL_M1_Average_Buff[12] = {0};
	HALL_M1.Average_Length = 12;
	HALL_M1.Average_Period = HALL_M1_Average_Buff;
	HALL_M1.ENC_A_GPIOx = M1_ENC_A_GPIO_Port;
	HALL_M1.ENC_A_PIN = M1_ENC_A_Pin;
	HALL_M1.ENC_B_GPIOx = M1_ENC_B_GPIO_Port;
	HALL_M1.ENC_B_PIN = M1_ENC_B_Pin;
	HALL_M1.ENC_Z_GPIOx = M1_ENC_Z_GPIO_Port;
	HALL_M1.ENC_Z_PIN = M1_ENC_Z_Pin;
	HALL_M1.LastTime = HAL_GetTick();
	HALL_M1.Encoding = 0x00;
	Hall_GetEncoding(&HALL_M1);
}


void Hall_GetEncoding(HALL_Handle *phall){
	phall->LastEncoding = phall->Encoding;
	if(HAL_GPIO_ReadPin(phall->ENC_A_GPIOx, phall->ENC_A_PIN) == GPIO_PIN_RESET){
		phall->Encoding &= 0xFE;
	}else{
		phall->Encoding |= 0x01;
	}
	if(HAL_GPIO_ReadPin(phall->ENC_B_GPIOx, phall->ENC_B_PIN) == GPIO_PIN_RESET){
		phall->Encoding &= 0xFD;
	}else{
		phall->Encoding |= 0x02;
	}
	if(HAL_GPIO_ReadPin(phall->ENC_Z_GPIOx, phall->ENC_Z_PIN) == GPIO_PIN_RESET){
		phall->Encoding &= 0xFB;
	}else{
		phall->Encoding |= 0x04;
	}
}


void Hall_GetPeriod(HALL_Handle *phall){
	uint8_t encoding,lastencoding;
	if(phall->LastTime + 20 >= TIM7_GetTick()) return;
	uint32_t currentTime = TIM7_GetTick();
	lastencoding = HALL_SectorEncoding(phall->LastEncoding);
	encoding = HALL_SectorEncoding(phall->Encoding);
	if(encoding == lastencoding + 1){
		phall->Average_Period[phall->Average_Length - 1] = (int16_t)(currentTime - phall->LastTime);//(int16_t)(10000 / (currentTime - phall->LastTime));
	}else if(encoding == (lastencoding - 1) % 6){
		phall->Average_Period[phall->Average_Length - 1] = (int16_t)(currentTime - phall->LastTime);
		phall->Average_Period[phall->Average_Length - 1] *= -1;
	}
	for(int i = 0; i < phall->Average_Length - 1; i++){
		phall->Average_Period[i] = phall->Average_Period[i + 1];
		phall->Pulse_Period += phall->Average_Period[i];
	}
	phall->Pulse_Period += phall->Average_Period[phall->Average_Length - 1];
	phall->Pulse_Period = (int16_t)(phall->Pulse_Period / 6);
	phall->LastTime = currentTime;
}



static uint8_t HALL_SectorEncoding(uint8_t HALL_Encoding){
	switch(HALL_Encoding) {
		case 0x05:	return 1;break;
		case 0x01:	return 2;break;
		case 0x03:	return 3;break;
		case 0x02:	return 4;break;
		case 0x06:	return 5;break;
		case 0x04:	return 6;break;
		default:	return 0;

	}
}
