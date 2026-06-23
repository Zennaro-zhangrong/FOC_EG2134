/*
 * clark_transform.c
 *
 *  Created on: 2026年6月16日
 *      Author: zr186
 */

#include "../Inc/clark_transformation.h"
#include "stm32f4xx_hal.h"



#define COS60_F     0.5f
#define COS30_F     0.8660254038f
#define CLARK_SCALE 0.6666666667f  // 2/3 Clark正变换系数

Clark_Handle Clark;

static void Clark_forward_transformation();
static void Clark_inverse_transformation();




void Clark_transformation(Clark_Mode diration){

	if(diration == Clark_Forward){
		Clark_forward_transformation();
	}else if(diration == Clark_Invers){
		Clark_inverse_transformation();
	}else{
		return;
	}
}


/**
 * @brief Clark 正变换 abc → αβ
 * @param Pclark 克拉克变换句柄指针
 */
static void Clark_forward_transformation()
{
    float Va = (float)Clark.PhaseA;
    float Vb = (float)Clark.PhaseB;
    float Vc = (float)Clark.PhaseC;

    Clark.PhaseAlpha = (int)(CLARK_SCALE * (Va - Vb * COS60_F - Vc * COS60_F));
    Clark.PhaseBeta = (int)(CLARK_SCALE * (Vb - Vc) * COS30_F);
}

/**
 * @brief Clark 逆变换 αβ → abc
 * @param Pclark 克拉克变换句柄指针
 */
static void Clark_inverse_transformation()
{
    float Valpha = (float)Clark.PhaseAlpha;
    float Vbeta  = (float)Clark.PhaseBeta;


    Clark.PhaseA = (int)Valpha;
    Clark.PhaseB = (int)(-COS60_F * Valpha + COS30_F * Vbeta);
    Clark.PhaseC = (int)(-COS60_F * Valpha - COS30_F * Vbeta);
}


