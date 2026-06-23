/*
 * park_transform.h
 *
 *  Created on: 2026年6月16日
 *      Author: zr186
 */

#ifndef INC_PARK_TRANSFORMATION_H_
#define INC_PARK_TRANSFORMATION_H_



typedef struct {
	int PhaseAlpha;
	int PhaseBeta;
	int PhaseD;
	int PhaseQ;
	int PhaseTheta;
} Park_Handle;


typedef enum{
	Park_Forward,
	Park_Inverse
}Park_Mode;


extern Park_Handle Park;

void Park_transformation(Park_Mode diration);




#endif /* INC_PARK_TRANSFORMATION_H_ */
