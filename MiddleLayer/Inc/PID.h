/**
 * @名称：PID.h
 * @功能：PID控制算法接口
 * @作者：张荣
 */

#ifndef INC_PID_H_
#define INC_PID_H_

/* ---- PID 控制句柄 ---- */
typedef struct{
	float Kp;
	float Ki;
	float Kd;
	float Out_Last;
	float Output;
	float MAX_Output;
	float MIN_Output;
	float Expactation_value;
	float Actual_value;
	float Error_negative_one;
	float Error_negative_two;
}IncrementalPID_Handle;


void IncrementalPID_ParameterSet(IncrementalPID_Handle *Ppid, float Kp, float Ki, float Kd);
float IncrementalPID_Output(IncrementalPID_Handle *Ppid, float Expactation_value, float Actual_value);

#endif /* INC_PID_H_ */
