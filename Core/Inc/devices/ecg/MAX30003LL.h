/*
 * MAX30003LL.h
 *
 *  Created on: Apr 26, 2020
 *      Author: andrey
 */

#ifndef INC_DEVICES_ECG_MAX30003LL_H_
#define INC_DEVICES_ECG_MAX30003LL_H_

#include "stm32h7xx_hal.h"
#include "cmsis_os.h"
#include "devices/ecg/MAX30003.h"
#include "main.h"

#define MAX30003_CHIP_ENABLE_PORT    	MAX30003_CS_GPIO_Port
#define MAX30003_CHIP_ENABLE_PIN     	MAX30003_CS_Pin

#define MAX30003_INTERRUPT_PORT        	MAX30003_INTB_GPIO_Port
#define MAX30003_INTERRUPT_PIN         	MAX30003_INTB_Pin

void max30003LowLevelInit(MAX30003ECGDataCallback_t callback);
void max30003LowLevelDeInit();
void max30003LowLevelChipEnable();
void max30003LowLevelChipDisable();
void max30003LowLevelWriteData(void* device, uint8_t* txBuffer, uint8_t size);
void max30003LowLevelSpiTxRxHandler(SPI_HandleTypeDef *hspi);
void max30003LowLevelEXTIHandler();
void max30003LowLevelInterruptTopHalfHandler();
void max30003LowLevelInterruptBottomHalfHandler();
void max30003LowLevelTick();

#endif /* INC_DEVICES_ECG_MAX30003LL_H_ */
