/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "domain/use_cases/recording_use_case.h"

//#define USE_SOFTWARE_OVERSAMPLING

#ifdef USE_SOFTWARE_OVERSAMPLING
	#define ADC_BUFFER_LENGHT 4
#endif
extern QueueHandle_t earECGQueue;

#ifdef USE_SOFTWARE_OVERSAMPLING
uint16_t adcBuffer[ADC_BUFFER_LENGHT] = {0, 0, 0, 0};
uint8_t bufferCounter = 0;
uint16_t adcValue;
#endif
uint32_t averageValue;
uint8_t graphCounter = 0;
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV128;
  hadc1.Init.Resolution = ADC_RESOLUTION_16B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T3_TRGO;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;

#ifdef USE_SOFTWARE_OVERSAMPLING
  hadc1.Init.OversamplingMode = DISABLE;
#else
  hadc1.Init.OversamplingMode = ENABLE;
  hadc1.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_RESUMED_MODE;
  hadc1.Init.Oversampling.Ratio = 4;
  hadc1.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_2;
  hadc1.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_MULTI_TRIGGER;
#endif
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC12_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_INP10
    PC4     ------> ADC1_INP4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* ADC1 interrupt Init */
  /* USER CODE BEGIN ADC1_MspInit 1 */
    HAL_NVIC_SetPriority(ADC_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC12_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_INP10
    PC4     ------> ADC1_INP4
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_4);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
#ifdef USE_SOFTWARE_OVERSAMPLING
	adcValue = HAL_ADC_GetValue(hadc);
	if(bufferCounter < ADC_BUFFER_LENGHT - 1) {
		adcBuffer[bufferCounter] = adcValue;
		bufferCounter++;
	} else {
		adcBuffer[bufferCounter] = adcValue;
		bufferCounter = 0;
		averageValue = 0;
		for(int i = 0; i < ADC_BUFFER_LENGHT; i++) {
			averageValue += adcBuffer[i];
		}
		averageValue = averageValue / ADC_BUFFER_LENGHT;
#else
		averageValue = HAL_ADC_GetValue(hadc);
#endif
		storeSampleECGEar(averageValue);

		if(earECGQueue != 0) {
			if(graphCounter < GRAPH_DOWNSAMPLING_VALUE) {
				graphCounter++;
			} else {
				graphCounter = 0;
				xQueueSendFromISR(earECGQueue, (void *)&averageValue, (TickType_t)0);
			}
		}

#ifdef USE_SOFTWARE_OVERSAMPLING
	}
#endif

}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
