/*
 * MAX86161LL.h
 *
 *  Created on: Oct 16, 2020
 *      Author: andrey
 */

#ifndef INC_DEVICES_PPG_MAX86161LL_H_
#define INC_DEVICES_PPG_MAX86161LL_H_
#include "stm32h7xx_hal.h"
#include "cmsis_os.h"
#include "devices/ppg/MAX86161.h"
#include "main.h"

#define MAX86161_LEFT_INTERRUPT_PORT									MAXM86161_LEFT_INT_GPIO_Port
#define MAX86161_LEFT_INTERRUPT_PIN										MAXM86161_LEFT_INT_Pin
#define MAX86161_RIGHT_INTERRUPT_PORT									MAXM86161_RIGHT_INT_GPIO_Port
#define MAX86161_RIGHT_INTERRUPT_PIN									MAXM86161_RIGHT_INT_Pin

#define MAX86161_LEFT_EN_PORT											MAXM86161_LEFT_EN_GPIO_Port
#define MAX86161_LEFT_EN_PIN											MAXM86161_LEFT_EN_Pin
#define MAX86161_RIGHT_EN_PORT											MAXM86161_RIGHT_EN_GPIO_Port
#define MAX86161_RIGHT_EN_PIN											MAXM86161_RIGHT_EN_Pin

#define MAX86161_I2C_ADDRESS											0xC4
//#define MAX86161_I2C_READ_ADDRESS										0xC5

void max86161RightLLInit(MAX86161PPGDataCallback_t callback);
void max86161LeftLLInit(MAX86161PPGDataCallback_t callback);
void max86161RightLLDeInit();
void max86161LeftLLDeInit();
void max86161RightLLChipEnable();
void max86161RightLLChipDisable();
void max86161LeftLLChipEnable();
void max86161LeftLLChipDisable();
void max86161RightLLWriteData(void* device, uint8_t* txBuffer, uint8_t size);
void max86161LeftLLWriteData(void* device, uint8_t* txBuffer, uint8_t size);
void max86161LLI2C1LeftTxHandler(I2C_HandleTypeDef *hi2c);
void max86161LLI2C1LeftRxHandler(I2C_HandleTypeDef *hi2c);
void max86161LLI2C1LeftErrorHandler(I2C_HandleTypeDef *hi2c);
void max86161LLI2C2RightTxHandler(I2C_HandleTypeDef *hi2c);
void max86161LLI2C2RightRxHandler(I2C_HandleTypeDef *hi2c);
void max86161LLI2C2RightErrorHandler(I2C_HandleTypeDef *hi2c);
void max86161RightLLEXTIHandler();
void max86161LeftLLEXTIHandler();
void max86161RightLLInterruptTopHalfHandler();
void max86161RightLLInterruptBottomHalfHandler();
void max86161RightLLTick();
void max86161LeftLLInterruptTopHalfHandler();
void max86161LeftLLInterruptBottomHalfHandler();
void max86161LeftLLTick();
uint8_t isMax86161Operation(I2C_HandleTypeDef *hi2c);

#endif /* INC_DEVICES_PPG_MAX86161LL_H_ */
