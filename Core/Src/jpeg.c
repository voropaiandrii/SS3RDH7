/**
  ******************************************************************************
  * @file    jpeg.c
  * @brief   This file provides code for the configuration
  *          of the JPEG instances.
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

/* Includes ------------------------------------------------------------------*/
#include "jpeg.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

JPEG_HandleTypeDef hjpeg;

/* JPEG init function */
void MX_JPEG_Init(void)
{

  hjpeg.Instance = JPEG;
  if (HAL_JPEG_Init(&hjpeg) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_JPEG_MspInit(JPEG_HandleTypeDef* jpegHandle)
{

  if(jpegHandle->Instance==JPEG)
  {
  /* USER CODE BEGIN JPEG_MspInit 0 */

  /* USER CODE END JPEG_MspInit 0 */
    /* JPEG clock enable */
    __HAL_RCC_JPEG_CLK_ENABLE();

    /* JPEG interrupt Init */
    HAL_NVIC_SetPriority(JPEG_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(JPEG_IRQn);
  /* USER CODE BEGIN JPEG_MspInit 1 */

  /* USER CODE END JPEG_MspInit 1 */
  }
}

void HAL_JPEG_MspDeInit(JPEG_HandleTypeDef* jpegHandle)
{

  if(jpegHandle->Instance==JPEG)
  {
  /* USER CODE BEGIN JPEG_MspDeInit 0 */

  /* USER CODE END JPEG_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_JPEG_CLK_DISABLE();

    /* JPEG interrupt Deinit */
    HAL_NVIC_DisableIRQ(JPEG_IRQn);
  /* USER CODE BEGIN JPEG_MspDeInit 1 */

  /* USER CODE END JPEG_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
