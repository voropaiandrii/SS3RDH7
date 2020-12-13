/*
 * MAX30102LL.c
 *
 *  Created on: Oct 1, 2020
 *      Author: andrey
 */
#include "devices/ppg/MAX30102LL.h"
#include "i2c.h"
#include "stdio.h"

static I2C_t i2cMAX30102;
MAX30102Device_t max30102device;
static MAX30102Settings_t max30102Settings;

//extern I2C_HandleTypeDef hi2c1;
extern osThreadId max30102InterruptTaskHandle;

static void changeI2CState(MAX30102Device_t* max30102device, I2C_HandleTypeDef *hi2c, uint8_t size) {
	HAL_StatusTypeDef operationResult = 0;
	//printf("changeI2CState start, device->currentState %d\n", max30102device->i2cState);
	switch(max30102device->i2cState) {
		case MAX30102_I2C_STATE_START:
			max30102device->i2cState = MAX30102_I2C_STATE_SEND_ADDRESS;
			if(max30102device->i2cAction == MAX30102_I2C_ACTION_READ_DATA) {
				operationResult = HAL_I2C_Master_Seq_Transmit_IT(&hi2c1, MAX30102_I2C_ADDRESS, max30102device->txBuffer, 1, I2C_FIRST_AND_LAST_FRAME);
			} else {
				operationResult = HAL_I2C_Master_Seq_Transmit_IT(&hi2c1, MAX30102_I2C_ADDRESS, max30102device->txBuffer, 1, I2C_FIRST_AND_NEXT_FRAME);
			}
			//HAL_I2C_Master_Seq_Transmit_DMA(&hi2c1, MAX30102_I2C_ADDRESS, max30102device->txBuffer, 1, I2C_FIRST_FRAME);
			break;
		case MAX30102_I2C_STATE_SEND_ADDRESS:
			if(max30102device->i2cAction == MAX30102_I2C_ACTION_READ_DATA) {
				max30102device->i2cState = MAX30102_I2C_STATE_RECEIVE_DATA;
				operationResult = HAL_I2C_Master_Seq_Receive_IT(&hi2c1, MAX30102_I2C_ADDRESS, max30102device->rxBuffer, max30102device->i2cDataSize, I2C_LAST_FRAME);
				//HAL_I2C_Master_Seq_Receive_DMA(&hi2c1, MAX30102_I2C_ADDRESS, max30102device->rxBuffer, max30102device->i2cDataSize, I2C_LAST_FRAME);
			} else if(max30102device->i2cAction == MAX30102_I2C_ACTION_WRITE_DATA) {
				max30102device->i2cState = MAX30102_I2C_STATE_SEND_DATA;
				operationResult = HAL_I2C_Master_Seq_Transmit_IT(&hi2c1, MAX30102_I2C_ADDRESS, (max30102device->txBuffer + 1), 1, I2C_LAST_FRAME);
				//HAL_I2C_Master_Seq_Transmit_DMA(&hi2c1, MAX30102_I2C_ADDRESS, (max30102device->txBuffer + 1), 1, I2C_LAST_FRAME);
			}
			break;
		case MAX30102_I2C_STATE_SEND_DATA:
			max30102device->i2cState = MAX30102_I2C_STATE_COMPLETED;
			if(i2cMAX30102.operations.receiveData != NULL) {
				i2cMAX30102.operations.receiveData((void*)max30102device, max30102device->rxBuffer, 0);
			}
			break;
		case MAX30102_I2C_STATE_RECEIVE_DATA:
			max30102device->i2cState = MAX30102_I2C_STATE_COMPLETED;
			if(i2cMAX30102.operations.receiveData != NULL) {
				i2cMAX30102.operations.receiveData((void*)max30102device, max30102device->rxBuffer, max30102device->i2cDataSize);
			}
			break;
	}
	if(operationResult == HAL_ERROR) {
		max30102device->currentState = MAX30102_STATE_STOPPED;
		max30102device->error = MAX30102_ERROR_COMMUNICATION_FAILED;
	}
	//printf("changeI2CState end, device->currentState: %d, operationResult: %d\n", max30102device->i2cState, operationResult);
}

static void max30102EnableInterrupt() {
	__HAL_GPIO_EXTI_CLEAR_IT(MAX30102_INTERRUPT_PIN);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

// TODO: Move to main
static void max30102DisableInterrupt() {
	HAL_NVIC_DisableIRQ(EXTI3_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(MAX30102_INTERRUPT_PIN);
}

static uint8_t max30102ReadState() {
	return HAL_GPIO_ReadPin(MAX30102_INTERRUPT_PORT, MAX30102_INTERRUPT_PIN) == GPIO_PIN_RESET;
}


void max30102LLInit(MAX30102PPGDataCallback_t callback) {

	i2cMAX30102.operations.writeData = max30102LLWriteData;

	max30102Settings.enableInterruptFunction = max30102EnableInterrupt;
	max30102Settings.disableInterruptFunction = max30102DisableInterrupt;
	max30102Settings.readInterruptStateFunction = max30102ReadState;
	max30102Settings.ppgDataCallback = callback;
	max30102Settings.i2c = &i2cMAX30102;

	max30102device.settings = &max30102Settings;
	max30102Init(&max30102device);
}

void max30102LLDeInit() {

}

void max30102LLWriteData(void* device, uint8_t* txBuffer, uint8_t size) {
	//HAL_I2C_Master_Transmit_IT(&hi2c1, MAX30102_I2C_ADDRESS, ((MAX30102Device_t*)device)->txBuffer, size);
	changeI2CState((MAX30102Device_t*)device, &hi2c1, size);
}

void max30102LLI2CTxHandler(I2C_HandleTypeDef *hi2c) {
	changeI2CState(&max30102device, hi2c, hi2c->XferSize);
}

void max30102LLI2CRxHandler(I2C_HandleTypeDef *hi2c) {
	changeI2CState(&max30102device, hi2c, hi2c->XferSize);
}

void max30102LLI2CErrorHandler(I2C_HandleTypeDef *hi2c) {
	uint32_t errorCode = HAL_I2C_GetError(hi2c);
	switch(errorCode) {
		case HAL_I2C_ERROR_NONE:
			//CLEAR_BIT(hi2c->Instance->SR1, I2C_SR1_AF);
			break;
		case HAL_I2C_ERROR_BERR:
			CLEAR_BIT(hi2c->Instance->ISR, I2C_ISR_BERR);
			break;
		case HAL_I2C_ERROR_ARLO:
			CLEAR_BIT(hi2c->Instance->ISR, I2C_ISR_ARLO);
			break;
		case HAL_I2C_ERROR_AF:
			CLEAR_BIT(hi2c->Instance->ISR, I2C_ISR_ALERT);
			break;
		case HAL_I2C_ERROR_OVR:
			CLEAR_BIT(hi2c->Instance->ISR, I2C_ISR_OVR);
			break;
		case HAL_I2C_ERROR_DMA:
			//CLEAR_BIT(hi2c->Instance->SR1, I2C_SR1_DMA);
			break;
		case HAL_I2C_ERROR_TIMEOUT:
			//CLEAR_BIT(hi2c->Instance->SR1, I2C_SR1_TIMEOUT);
			break;
		case HAL_I2C_ERROR_SIZE:
			//CLEAR_BIT(hi2c->Instance->SR1, I2C_SR1_AF);
			break;
		case HAL_I2C_ERROR_DMA_PARAM:
			//CLEAR_BIT(hi2c->Instance->SR1, I2C_SR1_AF);
			break;
		//case HAL_I2C_WRONG_START:
			//CLEAR_BIT(hi2c->Instance->SR1, I2C_SR1_AF);
		//	break;
	}
	__HAL_RCC_I2C1_CLK_DISABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
	//max30102device.currentState = MAX30102_STATE_READ_PPG;
	//max30102device.settings->i2c->operations.receiveData((void*)&max30102device, max30102device.rxBuffer, 0);
	printf("MAX30102 I2C error code: %lu\n", errorCode);
}

void max30102LLEXTIHandler() {

}

void max30102LLInterruptTopHalfHandler() {
	max30102InterruptTopHalfHandler(&max30102device);
}

void max30102LLInterruptBottomHalfHandler() {
	max30102InterruptBottomHalfHandler(&max30102device);
}

void max30102LLTick() {
	max30102Tick(&max30102device);
}

uint8_t isMax30102Operation(I2C_HandleTypeDef *hi2c) {
	uint16_t slaveAddress = (uint16_t)(hi2c->Instance->CR2 & I2C_CR2_SADD);
	return  ( slaveAddress == MAX30102_I2C_ADDRESS);
}
