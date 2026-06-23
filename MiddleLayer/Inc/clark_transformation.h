/*
 * clark_transform.h
 *
 *  Created on: 2026年6月16日
 *      Author: zr186
 */

#ifndef INC_CLARK_TRANSFORMATION_H_
#define INC_CLARK_TRANSFORMATION_H_



typedef struct {
    int PhaseA;
    int PhaseB;
    int PhaseC;
    int PhaseAlpha;
    int PhaseBeta;
} Clark_Handle;


typedef enum{
	Clark_Forward,
	Clark_Invers
}Clark_Mode;


extern Clark_Handle Clark;

void Clark_transformation(Clark_Mode diration);




#endif /* INC_CLARK_TRANSFORMATION_H_ */
