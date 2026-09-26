/*
 * angle_get.h
 *
 *  Created on: 2026年6月16日
 *      Author: zr186
 */

#ifndef INC_ANGLE_PROGRESS_H_
#define INC_ANGLE_PROGRESS_H_

/*
 * ANGLE_GET_MOED = 0 为无感方案
 * ANGLE_GET_MOED = 1 为有感方案
 * */
#define ANGLE_GET_MOED			0//无感方案



#if !ANGLE_GET_MOED//无感方案


typedef struct
{
    float Rs;       // 定子电阻 (Ω)
    float Ls;       // 定子电感 (H)
    float Psi_f;    // 永磁磁链 (Wb)
    float PolePair; // 极对数
    float K_smo;    // 滑膜增益，越大收敛越快，抖振越强
    float Ts;       // FOC控制周期 s (如0.0001f = 100us)
    float tau_filter;// 反电动势低通滤波时间常数
    float theta_comp;// 滤波相位补偿角 rad
} SMO_MotorParam_t;


typedef struct
{
    // 输入：αβ轴电压、实际采样电流
    float u_alpha;
    float u_beta;
    float i_alpha;
    float i_beta;

    // 观测器内部状态：估计电流
    float i_alpha_hat;
    float i_beta_hat;

    // 电流误差
    float err_i_alpha;
    float err_i_beta;

    // 滑膜开关控制量 zα zβ
    float z_alpha;
    float z_beta;

    // 滤波后等效反电动势
    float e_alpha_filter;
    float e_beta_filter;

    // 观测输出：转子电角度、电角速度
    float theta_e_raw;  // 未补偿角度
    float theta_e_hat;  // 补偿后最终观测角度 rad
    float omega_e_hat;  // 观测电角速度 rad/s

    // 角度微分缓存（转速平滑）
    float theta_last;

} SMO_State_t;
#endif



#if	ANGLE_GET_MOED //有感方案









#endif


#endif /* INC_ANGLE_PROGRESS_H_ */
