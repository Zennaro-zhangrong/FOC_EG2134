/*
 * Hall_Sensor.h
 *
 *  Created on: 2026年6月22日
 *      Author: 张荣
 */

#ifndef INC_HALL_SENSOR_H_
#define INC_HALL_SENSOR_H_
#include "stm32f4xx_hal.h"



typedef struct{
	//三相霍尔引脚信息，均为外部双边沿中断
	GPIO_TypeDef* ENC_A_GPIOx;
	uint16_t ENC_A_PIN;
	GPIO_TypeDef* ENC_B_GPIOx;
	uint16_t ENC_B_PIN;
	GPIO_TypeDef* ENC_Z_GPIOx;
	uint16_t ENC_Z_PIN;

	volatile uint32_t LastTime;//上次本对象触发时间
	volatile int16_t Pulse_Period;//两次相邻边沿的周期长度，具有方向性
	volatile float Pulse_Frequency;//估算出的频率，似乎有问题，计算不准
	uint8_t Average_Length;//频率均值滤波长度
	float *Average_Frequency;//均值滤波数组实际地址
	volatile uint8_t Encoding;//当前霍尔电角度编码
	volatile uint8_t LastEncoding;//前一次霍尔电角度编码，用来判断转动方向
}HALL_Handle;


extern HALL_Handle HALL_M0, HALL_M1;

/*
 * @功能：初始化霍尔传感器参数和地址
 * */
void Hall_Init(void);

/*
 * @功能：获取霍尔传感器编码扇区编号
 * @参数：要获取的霍尔传感器句柄指针
 * */
void Hall_GetEncoding(HALL_Handle *phall);

/*
 * @功能：获取当前一个扇区经历的时间长度，用来计算角速度和线性插值
 * @参数：要获取的霍尔编码器的句柄地址
 * @注意：此周期的单位是 (ms/20)
 * */
void Hall_GetFrequency(HALL_Handle *phall);

void HALL_EXTI_Callback(HALL_Handle *pHALL, uint16_t GPIO_Pin);
void HALL_TIM_Supervised_Update(HALL_Handle *pHALL);
#endif /* INC_HALL_SENSOR_H_ */
