/*
 * SVPWM.h
 *
 *  Created on: 2026年6月18日
 *      Author: zr186
 */

#ifndef INC_SVPWM_H_
#define INC_SVPWM_H_

#include "../../DriveLayer/Inc/PWM_EG2134.h"


typedef struct {
	uint8_t sector;
	int PhaseAlpha;
	int PhaseBeta;
	int Duty_A;
	int Duty_B;
	int Duty_C;
	uint16_t MaxDuty;
	int DeadTime;
} SVPWM_Handle;


extern SVPWM_Handle SVPWM;

void SVPWM_Generate(void);
void SVPWM_Test(EG2134_Handle *pEG2134, unsigned short Amplitude, float Frequency);


#endif /* INC_SVPWM_H_ */
