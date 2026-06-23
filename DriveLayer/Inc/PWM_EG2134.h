/*
 * PWM_EG2134.h
 *
 *  Created on: 2026年6月9日
 *      Author: zr186
 */

#ifndef INC_PWM_EG2134_H_
#define INC_PWM_EG2134_H_

#include "../../Core/Inc/tim.h"
typedef struct{
	TIM_HandleTypeDef *htim;
	uint32_t PWM_CH_A;
	uint32_t PWM_CH_B;
	uint32_t PWM_CH_C;
	uint16_t Duty_cycle_A;
	uint16_t Duty_cycle_B;
	uint16_t Duty_cycle_C;

}EG2134_Handle;



extern EG2134_Handle EG2134_M0, EG2134_M1;
/*
 * @功能：初始化栅极驱动器
 * */
void EG2134_Init(void);


/*
 * @功能：同步句柄中的占空比值，使用前要先修改句柄中的转空比值
 * @参数：栅极驱动器EG2134句柄
 * */
void EG2134_PWM_Compare_Update(EG2134_Handle *pEG2134);


/*
 * @功能：强制输出三相正弦波
 * @参数：栅极驱动器句柄
 * @参数：正弦波幅值
 * @参数：正弦波频率
 * @注意：此函数应当放在while(1)中使用，避免被耗时应用抢占，目的仅仅是测试定时器是否配置正确
 * */
void EG2134_Test(EG2134_Handle *pEG2134, uint16_t Amplitude, float Frequency);
#endif /* INC_PWM_EG2134_H_ */
