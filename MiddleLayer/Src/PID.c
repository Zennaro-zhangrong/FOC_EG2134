/**
 * @名称：PID.c
 * @功能：位置式PID控制算法，含条件积分抗饱和、微分先行的微分项、微分低通滤波
 * @调用频率假设：固定周期调用，dt隐含在I/D参数中
 * @作者：张荣
 */

#include "../Inc/PID.h"
#include "stm32f4xx_hal.h"
#include "../../DriveLayer/Inc/UART2.h"



void IncrementalPID_ParameterSet(IncrementalPID_Handle *Ppid, float Kp, float Ki, float Kd){
	Ppid->Kp = Kp;
	Ppid->Ki = Ki;
	Ppid->Kd = Kd;
}

float IncrementalPID_Output(IncrementalPID_Handle *Ppid, float Expactation_value, float Actual_value){
	Ppid->Expactation_value = Expactation_value;
	Ppid->Actual_value = Actual_value;
	float Error_value = Ppid->Expactation_value - Ppid->Actual_value;
	float delta_out =
			Ppid->Kp * (Error_value - Ppid->Error_negative_one)
			+ Ppid->Ki * Error_value
			+ Ppid->Kd * (Error_value - 2 * Ppid->Error_negative_one + Ppid->Error_negative_two);
	Ppid->Error_negative_two = Ppid->Error_negative_one;
	Ppid->Error_negative_one = Error_value;
	Ppid->Output = Ppid->Out_Last + delta_out;
	if(Ppid->Output > Ppid->MAX_Output) Ppid->Output = Ppid->MAX_Output;
	if(Ppid->Output < Ppid->MIN_Output)	Ppid->Output = Ppid->MIN_Output;
	Ppid->Out_Last = Ppid->Output;
	return Ppid->Output;
}
