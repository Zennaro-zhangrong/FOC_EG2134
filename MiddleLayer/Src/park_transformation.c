/*
 * park_transformation.c
 *
 *  Created on: 2026年6月16日
 *      Author: zr186
 */

#include "../Inc/park_transformation.h"

#include "stm32f4xx_hal.h"
#include "math.h"



#define PARK_THETA_SCALE 0.01745329252f  // π/180, int角度值 → 弧度

Park_Handle Park;

static void Park_forward_transformation();
static void Park_inverse_transformation();




void Park_transformation(Park_Mode diration){

	if(diration == Park_Forward){
		Park_forward_transformation();
	}else if(diration == Park_Inverse){
		Park_inverse_transformation();
	}else{
		return;
	}
}


/**
 * @brief Park 正变换 αβ → dq
 * @param Ppark 帕克变换句柄指针
 */
static void Park_forward_transformation()
{
	float Valpha  = (float)Park.PhaseAlpha;
	float Vbeta   = (float)Park.PhaseBeta;
	float Theta   = (float)Park.PhaseTheta * PARK_THETA_SCALE;

	Park.PhaseD = (int)( Valpha * cosf(Theta) + Vbeta * sinf(Theta));
	Park.PhaseQ = (int)(-Valpha * sinf(Theta) + Vbeta * cosf(Theta));
}

/**
 * @brief Park 逆变换 dq → αβ
 * @param Ppark 帕克变换句柄指针
 */
static void Park_inverse_transformation()
{
	float Vd    = (float)Park.PhaseD;
	float Vq    = (float)Park.PhaseQ;
	float Theta = (float)Park.PhaseTheta * PARK_THETA_SCALE;

	Park.PhaseAlpha = (int)(Vd * cosf(Theta) - Vq * sinf(Theta));
	Park.PhaseBeta  = (int)(Vd * sinf(Theta) + Vq * cosf(Theta));
}
