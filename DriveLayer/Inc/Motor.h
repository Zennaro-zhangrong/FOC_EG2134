/*
 * Motor.h
 *
 *  Created on: 2026年6月11日
 *      Author: zr186
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "../../DriveLayer/Inc/PWM_EG2134.h"
#include "../../DriveLayer/Inc/Hall_Sensor.h"
#include "../../MiddleLayer/Inc/clark_transformation.h"
#include "../../MiddleLayer/Inc/PID.h"

typedef enum{
	Motor_speedlose,
	Motor_currentlose,
	Motor_bark
}Motor_State;

typedef struct{
	EG2134_Handle *motorDrive;	//栅极驱动器对象指针
	HALL_Handle *hall;	//霍尔编码器对象指针
	IncrementalPID_Handle *Iq_LOOP;	//电流闭环对象指针
	IncrementalPID_Handle *Id_LOOP;	//电流闭环对象指针
	IncrementalPID_Handle *Speed_Loop;	//速度闭环对象指针
	int Current_PhaseA;	//A相电流实时数据
	int Current_PhaseB;	//B相电流实时数据
	int Current_PhaseC;	//C相电流实时数据
	int16_t Current_PhaseA_BAIS;	//A相电流直流偏置
	int16_t Current_PhaseB_BAIS;	//B相电流直流偏置
	int16_t Current_PhaseC_BAIS;	//C相电流直流偏置
	float Current_PhaseA_GAIN;	//A相电流幅值增益
	float Current_PhaseB_GAIN;	//B相电流幅值增益
	float Current_PhaseC_GAIN;	//C相电流幅值增益
	int Current_PhaseAlpha;	//clark正变换alpha电流
	int Current_PhaseBeta;	//clark正变换beta电流
	int Current_PhaseQ;	//park正变换Q轴电流
	int Current_PhaseD;	//park正变换D轴电流
	int Voltage_PhaseAlpha;	//park逆变换alpha电压
	int Voltage_PhaseBeta;	//park逆变换beta电压
	int16_t HALL_Angle;	//实际电机电角度
	uint16_t FOC_Angle;	//SVPWM磁矢量角度
	Motor_State State;//电机运行状态
}Motor_Handle;




extern Motor_Handle motor_M0, motor_M1;
extern uint32_t Current_ADC[2];

void Motor_Init(void);








#endif /* INC_MOTOR_H_ */
