/*
 * MAX30102LL.h
 *
 *  Created on: Oct 1, 2020
 *      Author: andrey
 */

#ifndef INC_DEVICES_PPG_MAX30102LL_H_
#define INC_DEVICES_PPG_MAX30102LL_H_

#include "stm32h7xx_hal.h"
#include "cmsis_os.h"
#include "devices/ppg/MAX30102.h"
#include "main.h"

#define MAX30102_INTERRUPT_PORT        	MAX30102_INT_GPIO_Port
#define MAX30102_INTERRUPT_PIN         	MAX30102_INT_Pin

#define MAX30102_I2C_ADDRESS								0xAE

void max30102LLInit(MAX30102PPGDataCallback_t callback);
void max30102LLDeInit();
void max30102LLChipEnable();
void max30102LLChipDisable();
void max30102LLWriteData(void* device, uint8_t* txBuffer, uint8_t size);
void max30102LLI2CTxHandler(I2C_HandleTypeDef *hi2c);
void max30102LLI2CRxHandler(I2C_HandleTypeDef *hi2c);
void max30102LLI2CErrorHandler(I2C_HandleTypeDef *hi2c);
void max30102LLEXTIHandler();
void max30102LLInterruptTopHalfHandler();
void max30102LLInterruptBottomHalfHandler();
void max30102LLTick();
uint8_t isMax30102Operation(I2C_HandleTypeDef *hi2c);



#endif /* INC_DEVICES_PPG_MAX30102LL_H_ */
