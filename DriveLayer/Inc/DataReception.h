/*
 * DataReception.h
 *
 *  Created on: 2026年7月2日
 *      Author: zr186
 */

#ifndef INC_DATARECEPTION_H_
#define INC_DATARECEPTION_H_

#include "stm32f4xx_hal.h"



typedef struct{
	//相关接收引脚信息
	GPIO_TypeDef* CS_GPIOx;
	uint16_t CS_PIN;
	GPIO_TypeDef* MOSI_GPIOx;
	uint16_t MOSI_PIN;
	GPIO_TypeDef* MISO_GPIOx;
	uint16_t MISO_PIN;
	GPIO_TypeDef* SCK_GPIOx;
	uint16_t SCK_PIN;


	uint32_t State_Data;//接收到的电机状态数据
	uint32_t Joystick_Data;//接收到的摇杆数据数据
	uint8_t Index;//当前接收位数索引
	int16_t Forward_value;
	int16_t Turn_value;
	uint32_t lastTime;
}DataReception_Handle;

extern DataReception_Handle DataReception;
void DataReception_Init(void);
void DataReception_EXTI_CallBack(DataReception_Handle *pdatareception, uint16_t GPIO_Pin);
void DataReception_TIM_Supervised(void);
#endif /* INC_DATARECEPTION_H_ */
