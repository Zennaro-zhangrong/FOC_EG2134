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

typedef struct{
	EG2134_Handle *motorDrive;
	HALL_Handle *hall;
	int Current_PhaseA;//单位mA
	int Current_PhaseB;
	int Current_PhaseC;
	int Current_PhaseAlpha;
	int Current_PhaseBeta;
	int Current_PhaseQ;
	int Current_PhaseD;
	int Voltage_PhaseAlpha;
	int Voltage_PhaseBeta;
	int16_t mechanical_angle;
}Motor_Handle;

extern Motor_Handle motor_M0, motor_M1;
extern uint32_t Current_ADC[2];

void Motor_Init(void);








#endif /* INC_MOTOR_H_ */
