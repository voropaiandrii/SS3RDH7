/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define RTOS_DEBUG_BUFFER_SIZE 					2000

#define GRAPH_DOWNSAMPLING_VALUE				19
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI1_CS2_Pin GPIO_PIN_13
#define SPI1_CS2_GPIO_Port GPIOC
#define MAX30003_CS_Pin GPIO_PIN_6
#define MAX30003_CS_GPIO_Port GPIOF
#define TOUCH_CS_Pin GPIO_PIN_4
#define TOUCH_CS_GPIO_Port GPIOA
#define TOUCH_BUSY_Pin GPIO_PIN_9
#define TOUCH_BUSY_GPIO_Port GPIOH
#define LCD_3V3_EN_Pin GPIO_PIN_12
#define LCD_3V3_EN_GPIO_Port GPIOH
#define SDCARD_PLG_Pin GPIO_PIN_12
#define SDCARD_PLG_GPIO_Port GPIOB
#define LCD_PWR_EN_Pin GPIO_PIN_13
#define LCD_PWR_EN_GPIO_Port GPIOB
#define LCD_BL_EN_Pin GPIO_PIN_15
#define LCD_BL_EN_GPIO_Port GPIOB
#define TOUCH_PWR_EN_Pin GPIO_PIN_11
#define TOUCH_PWR_EN_GPIO_Port GPIOD
#define MAX30003_INT2B_Pin GPIO_PIN_2
#define MAX30003_INT2B_GPIO_Port GPIOG
#define MAX30003_INT2B_EXTI_IRQn EXTI2_IRQn
#define MAX30003_INTB_Pin GPIO_PIN_3
#define MAX30003_INTB_GPIO_Port GPIOG
#define MAX30003_INTB_EXTI_IRQn EXTI3_IRQn
#define MAX30003_CLK_Pin GPIO_PIN_8
#define MAX30003_CLK_GPIO_Port GPIOA
#define MAXM86161_RIGHT_EN_Pin GPIO_PIN_13
#define MAXM86161_RIGHT_EN_GPIO_Port GPIOH
#define MAXM86161_LEFT_EN_Pin GPIO_PIN_15
#define MAXM86161_LEFT_EN_GPIO_Port GPIOH
#define TOUCH_PIRQ_Pin GPIO_PIN_1
#define TOUCH_PIRQ_GPIO_Port GPIOI
#define TOUCH_PIRQ_EXTI_IRQn EXTI1_IRQn
#define MAXM86161_RIGHT_INT_Pin GPIO_PIN_4
#define MAXM86161_RIGHT_INT_GPIO_Port GPIOD
#define MAXM86161_RIGHT_INT_EXTI_IRQn EXTI4_IRQn
#define MAX30003_PWR_EN_Pin GPIO_PIN_5
#define MAX30003_PWR_EN_GPIO_Port GPIOD
#define OLIMEX_PWR_EN_Pin GPIO_PIN_7
#define OLIMEX_PWR_EN_GPIO_Port GPIOD
#define LCD_STBY_Pin GPIO_PIN_10
#define LCD_STBY_GPIO_Port GPIOG
#define LCD_RESET_Pin GPIO_PIN_13
#define LCD_RESET_GPIO_Port GPIOG
#define MAXM86161_LEFT_INT_Pin GPIO_PIN_5
#define MAXM86161_LEFT_INT_GPIO_Port GPIOB
#define MAXM86161_LEFT_INT_EXTI_IRQn EXTI9_5_IRQn
#define MAX30102_INT_Pin GPIO_PIN_6
#define MAX30102_INT_GPIO_Port GPIOI
#define MAX30102_INT_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
