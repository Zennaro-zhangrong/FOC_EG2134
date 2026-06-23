/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M1_ENC_Z_Pin GPIO_PIN_15
#define M1_ENC_Z_GPIO_Port GPIOC
#define M1_ENC_Z_EXTI_IRQn EXTI15_10_IRQn
#define M0_SO1_Pin GPIO_PIN_0
#define M0_SO1_GPIO_Port GPIOC
#define M0_SO2_Pin GPIO_PIN_1
#define M0_SO2_GPIO_Port GPIOC
#define M1_SO2_Pin GPIO_PIN_2
#define M1_SO2_GPIO_Port GPIOC
#define M1_SO1_Pin GPIO_PIN_3
#define M1_SO1_GPIO_Port GPIOC
#define M0_ENC_Z_Pin GPIO_PIN_9
#define M0_ENC_Z_GPIO_Port GPIOC
#define M0_ENC_Z_EXTI_IRQn EXTI9_5_IRQn
#define LED_Pin GPIO_PIN_2
#define LED_GPIO_Port GPIOD
#define M0_ENC_A_Pin GPIO_PIN_4
#define M0_ENC_A_GPIO_Port GPIOB
#define M0_ENC_A_EXTI_IRQn EXTI4_IRQn
#define M0_ENC_B_Pin GPIO_PIN_5
#define M0_ENC_B_GPIO_Port GPIOB
#define M0_ENC_B_EXTI_IRQn EXTI9_5_IRQn
#define M1_ENC_A_Pin GPIO_PIN_6
#define M1_ENC_A_GPIO_Port GPIOB
#define M1_ENC_A_EXTI_IRQn EXTI9_5_IRQn
#define M1_ENC_B_Pin GPIO_PIN_7
#define M1_ENC_B_GPIO_Port GPIOB
#define M1_ENC_B_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
