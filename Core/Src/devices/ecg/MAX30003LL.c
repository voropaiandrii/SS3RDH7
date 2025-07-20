/*
 * MAX30003LL.c
 *
 *  Created on: Apr 26, 2020
 *      Author: andrey
 */
#include "devices/ecg/MAX30003LL.h"

MAX30003Device_t max30003device;
static SPI_t spiMAX30003;
static MAX30003Settings_t max30003Settings;

extern SPI_HandleTypeDef hspi5;
extern osThreadId_t max30003IRQTaskHandle;

static void max30003EnableInterrupt() {
	__HAL_GPIO_EXTI_CLEAR_IT(MAX30003_INTERRUPT_PIN);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

// TODO: Move to main
static void max30003DisableInterrupt() {
	HAL_NVIC_DisableIRQ(EXTI3_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(MAX30003_INTERRUPT_PIN);
}

static uint8_t max30003ReadState() {
	return HAL_GPIO_ReadPin(MAX30003_INTERRUPT_PORT, MAX30003_INTERRUPT_PIN) == GPIO_PIN_RESET;
}


void max30003LowLevelInit(MAX30003ECGDataCallback_t callback) {
	spiMAX30003.chipEnable = max30003LowLevelChipEnable;
	spiMAX30003.chipDisable = max30003LowLevelChipDisable;
	spiMAX30003.operations.writeData = max30003LowLevelWriteData;

	max30003Settings.enableInterruptFunction = max30003EnableInterrupt;
	max30003Settings.disableInterruptFunction = max30003DisableInterrupt;
	max30003Settings.readInterruptStateFunction = max30003ReadState;
	max30003Settings.ecgDataCallback = callback;
	max30003Settings.spi = &spiMAX30003;

	max30003device.settings = &max30003Settings;
	max30003Initilize(&max30003device);
}

void max30003LowLevelDeInit() {

}

void max30003LowLevelChipEnable() {
	HAL_GPIO_WritePin(MAX30003_CHIP_ENABLE_PORT, MAX30003_CHIP_ENABLE_PIN, GPIO_PIN_RESET);
}

void max30003LowLevelChipDisable() {
	HAL_GPIO_WritePin(MAX30003_CHIP_ENABLE_PORT, MAX30003_CHIP_ENABLE_PIN, GPIO_PIN_SET);
}

void max30003LowLevelWriteData(void* deviceStructurePointer, uint8_t* txBuffer, uint8_t size) {
	HAL_SPI_TransmitReceive_IT(&hspi5, txBuffer, ((MAX30003Device_t*)deviceStructurePointer)->rxBuffer, size);
}

void max30003LowLevelSpiTxRxHandler(SPI_HandleTypeDef *hspi) {
	if(spiMAX30003.operations.receiveData != NULL) {
		spiMAX30003.operations.receiveData((void*)&max30003device, max30003device.rxBuffer, hspi->RxXferSize);
	}
}
void max30003LowLevelEXTIHandler() {
	max30003LowLevelInterruptTopHalfHandler();
	if(max30003device.isInitialCommandSent == 1) {
		osThreadResume(max30003IRQTaskHandle);
	}
}

void max30003LowLevelInterruptTopHalfHandler() {
	max30003InterruptTopHalfHandler(&max30003device);
}

void max30003LowLevelInterruptBottomHalfHandler() {
	max30003InterruptBottomHalfHandler(&max30003device);
}

void max30003LowLevelTick() {
	max30003Tick(&max30003device);
}

