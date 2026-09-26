/*
 * angle_get.c
 *
 *  Created on: 2026年6月16日
 *      Author: zr186
 *
 */

#include "../Inc/angle_progress.h"

#include "stm32f4xx_hal.h"
#include "math.h"





/**
 * @功能 饱和函数sat
 * @参数 x：输入误差
 * @参数 limit：饱和阈值参数
 * @返回 输出 [-limit, limit]
 */
static float SMO_SatFunc(float x, float limit)
{
    if(x > limit) return limit;
    else if(x < -limit) return -limit;
    else return x / limit;
}



/**
 * @功能：弧度限幅到 [-PI, PI]
 * @输入：任意弧度
 * @输出：限幅后的对应弧度
 */
static float SMO_RadianLimit(float rad)
{
    while(rad > M_PI)
    	rad -= 2.0f * M_PI;
    while(rad < -M_PI)
    	rad += 2.0f * M_PI;
    return rad;
}
