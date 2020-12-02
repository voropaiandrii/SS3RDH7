/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
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

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include "devices/touch/XPT2046.h"
#include <stdio.h>

TouchEvent_t event;
extern "C" void xpt2046LowLevelInit(XPT2046TouchEventCallback_t);

static bool isTouchEventDetected = false;

void xpt2046TouchEventCallback(TouchEvent_t* e) {
	if(!isTouchEventDetected) {
		event.xPosition = e->xPosition;
		event.yPosition = e->yPosition;
		event.batteryVoltage = e->batteryVoltage;
		event.auxValue = e->auxValue;
		event.temperature = e->temperature;
		event.z1Position = e->z1Position;
		event.z2Position = e->z2Position;
	}
    isTouchEventDetected = true;
}

void STM32TouchController::init()
{
    /**
     * Initialize touch controller and driver
     *
     */
	xpt2046LowLevelInit(xpt2046TouchEventCallback);
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    /**
     * By default sampleTouch returns false,
     * return true if a touch has been detected, otherwise false.
     *
     * Coordinates are passed to the caller by reference by x and y.
     *
     * This function is called by the TouchGFX framework.
     * By default sampleTouch is called every tick, this can be adjusted by HAL::setTouchSampleRate(int8_t);
     *
     */
	if(isTouchEventDetected) {
		x = event.xPosition;
		y = event.yPosition;
		isTouchEventDetected = false;
		//printf("Touched, x: %d, y: %d\n", x, y);
		return true;
	} else {
		return false;
	}
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
