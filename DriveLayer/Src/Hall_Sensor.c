/*
 * Hall_Sensor.c
 *
 *  Created on: 2026年6月22日
 *      Author: 张荣
 */

#include "../Inc/Hall_Sensor.h"
#include "main.h"
#include "../../Core/Inc/tim.h"



HALL_Handle HALL_M0, HALL_M1;
static uint8_t HALL_SectorEncoding(uint8_t HALL_Encoding);



void Hall_Init(void){
	//关于角速度滤波，定义数据缓冲区和滤波长度
	static float HALL_M0_Average_Buff[6] = {0};
	HALL_M0.Average_Length = 6;
	HALL_M0.Average_Frequency = HALL_M0_Average_Buff;

	//霍尔引脚定义
	HALL_M0.ENC_A_GPIOx = M0_ENC_A_GPIO_Port;
	HALL_M0.ENC_A_PIN = M0_ENC_A_Pin;
	HALL_M0.ENC_B_GPIOx = M0_ENC_B_GPIO_Port;
	HALL_M0.ENC_B_PIN = M0_ENC_B_Pin;
	HALL_M0.ENC_Z_GPIOx = M0_ENC_Z_GPIO_Port;
	HALL_M0.ENC_Z_PIN = M0_ENC_Z_Pin;

	//数据填充
	HALL_M0.LastTime = HAL_GetTick();
	HALL_M0.Encoding = 0x00;

	//初始测速周期，30000时相当于角速度趋近于0
	HALL_M0.Pulse_Period = 30000;

	//获取当前霍尔的角度编码，用来预填充
	Hall_GetEncoding(&HALL_M0);

	static float HALL_M1_Average_Buff[3] = {0};
	HALL_M1.Average_Length = 3;
	HALL_M1.Average_Frequency = HALL_M1_Average_Buff;
	HALL_M1.ENC_A_GPIOx = M1_ENC_A_GPIO_Port;
	HALL_M1.ENC_A_PIN = M1_ENC_A_Pin;
	HALL_M1.ENC_B_GPIOx = M1_ENC_B_GPIO_Port;
	HALL_M1.ENC_B_PIN = M1_ENC_B_Pin;
	HALL_M1.ENC_Z_GPIOx = M1_ENC_Z_GPIO_Port;
	HALL_M1.ENC_Z_PIN = M1_ENC_Z_Pin;
	HALL_M1.LastTime = HAL_GetTick();
	HALL_M1.Encoding = 0x00;
	HALL_M1.Pulse_Period = 30000;
	Hall_GetEncoding(&HALL_M1);
}




void Hall_GetEncoding(HALL_Handle *phall){
	phall->LastEncoding = phall->Encoding;
	uint8_t encoding = 0;
	if(HAL_GPIO_ReadPin(phall->ENC_A_GPIOx, phall->ENC_A_PIN) == GPIO_PIN_RESET){
		encoding &= 0xFE;
	}else{
		encoding |= 0x01;
	}
	if(HAL_GPIO_ReadPin(phall->ENC_B_GPIOx, phall->ENC_B_PIN) == GPIO_PIN_RESET){
		encoding &= 0xFD;
	}else{
		encoding |= 0x02;
	}
	if(HAL_GPIO_ReadPin(phall->ENC_Z_GPIOx, phall->ENC_Z_PIN) == GPIO_PIN_RESET){
		encoding &= 0xFB;
	}else{
		encoding |= 0x04;
	}
	phall->Encoding = HALL_SectorEncoding(encoding);
}




void Hall_GetFrequency(HALL_Handle *phall){

	uint8_t encoding,lastencoding;
	uint32_t currentTime = TIM7_GetTick();
	float accumulated_value = 0;

	for(int i = 0; i < phall->Average_Length - 1; i++){
		phall->Average_Frequency[i] = phall->Average_Frequency[i + 1];
		accumulated_value += phall->Average_Frequency[i];
	}


	lastencoding = phall->LastEncoding;//扇区解码
	encoding = phall->Encoding;//扇区解码
	phall->Pulse_Period = (uint16_t)(currentTime - phall->LastTime);
	phall->Average_Frequency[phall->Average_Length - 1] = 2000.0f / (float)(currentTime - phall->LastTime);//每100ms的霍尔脉冲频率

	if(encoding == (lastencoding + 1) % 6){//正转
		accumulated_value += phall->Average_Frequency[phall->Average_Length - 1];
	}else if(encoding % 6 == lastencoding - 1){
		phall->Pulse_Period *= -1;
		phall->Average_Frequency[phall->Average_Length - 1] *= -1;
		accumulated_value += phall->Average_Frequency[phall->Average_Length - 1];
	}

	phall->Pulse_Frequency = accumulated_value / phall->Average_Length;
	phall->LastTime = currentTime;
}



/*
 * @功能：将霍尔编码器的码型变换为foc扇区编号
 * @参数：要变换的霍尔编码器的句柄
 * @返回：扇区值，范围[1 ~ 6]
 * */
static uint8_t HALL_SectorEncoding(uint8_t HALL_Encoding){
	switch(HALL_Encoding) {
		case 0x05:	return 3; break;
		case 0x01:	return 4; break;
		case 0x03:	return 5; break;
		case 0x02:	return 6; break;
		case 0x06:	return 1; break;
		case 0x04:	return 2; break;
		default:	return 0;

	}
}

void HALL_EXTI_Callback(HALL_Handle *pHALL, uint16_t GPIO_Pin){
	if(GPIO_Pin == pHALL->ENC_A_PIN || GPIO_Pin == pHALL->ENC_B_PIN || GPIO_Pin == pHALL->ENC_Z_PIN){
		if(pHALL->LastTime + 8 >= TIM7_GetTick()) return;
		Hall_GetEncoding(pHALL);
		Hall_GetFrequency(pHALL);
	}
}


void HALL_TIM_Supervised_Update(HALL_Handle *pHALL){
	uint32_t currentTime = TIM7_GetTick();
	if(pHALL->Pulse_Frequency == 0) return;



	int16_t CurrentPeriod = (uint16_t)(currentTime - pHALL->LastTime);
	int16_t ActualPeriod = pHALL->Pulse_Period * 2;
	if(ActualPeriod < 0)	ActualPeriod *= -1;
	if(CurrentPeriod > ActualPeriod){
		pHALL->Pulse_Frequency = 0;
		if(pHALL->Pulse_Period > 0)	pHALL->Pulse_Period = 8000;
		else pHALL->Pulse_Period = -8000;
		float accumulated_value = 0;
		for(int i = 0; i < pHALL->Average_Length - 1; i++){
			pHALL->Average_Frequency[i] = 0;
		}
	}

}


/*static void Hall_GetPeriod(HALL_Handle *phall){

	uint8_t encoding,lastencoding;
	uint32_t currentTime = TIM7_GetTick();
	int32_t accumulated_value = 0;

	for(int i = 0; i < phall->Average_Length - 1; i++){
		phall->Average_Period[i] = phall->Average_Period[i + 1];
		accumulated_value += phall->Average_Period[i];
	}


	lastencoding = phall->LastEncoding;//扇区解码
	encoding = phall->Encoding;//扇区解码

	phall->Average_Frequency[phall->Average_Length - 1] = (int16_t)(currentTime - phall->LastTime);//(int16_t)(10000 / (currentTime - phall->LastTime));
	accumulated_value += phall->Average_Frequency[phall->Average_Length - 1];
	if(encoding == (lastencoding + 1) % 6){//正转
		phall->Pulse_Period = (int16_t)(accumulated_value / phall->Average_Length);
	}else if(encoding % 6 == lastencoding - 1){
		phall->Pulse_Period = -1 * (int16_t)(accumulated_value / phall->Average_Length);
	}
	phall->LastTime = currentTime;
}*/
