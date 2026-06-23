/*
 * Hall_Sensor.h
 *
 *  Created on: 2026年6月22日
 *      Author: zr186
 */

#ifndef INC_HALL_SENSOR_H_
#define INC_HALL_SENSOR_H_
#include "stm32f4xx_hal.h"



typedef struct{
	GPIO_TypeDef* ENC_A_GPIOx;
	uint16_t ENC_A_PIN;
	GPIO_TypeDef* ENC_B_GPIOx;
	uint16_t ENC_B_PIN;
	GPIO_TypeDef* ENC_Z_GPIOx;
	uint16_t ENC_Z_PIN;
	volatile uint32_t LastTime;
	volatile int16_t Pulse_Period;
	uint8_t Average_Length;
	int16_t *Average_Period;
	volatile uint8_t Encoding;
	volatile uint8_t LastEncoding;
}HALL_Handle;


extern HALL_Handle HALL_M0, HALL_M1;

void Hall_Init(void);
void Hall_GetEncoding(HALL_Handle *phall);
void Hall_GetPeriod(HALL_Handle *phall);

#endif /* INC_HALL_SENSOR_H_ */
