/*
 * MAX86161LL.c
 *
 *  Created on: Oct 16, 2020
 *      Author: andrey
 */

#include "devices/ppg/MAX86161LL.h"
#include "i2c.h"

static I2C_t i2cMAX86161Left;
static I2C_t i2cMAX86161Right;
MAX86161Device_t max86161LeftDevice;
MAX86161Device_t max86161RightDevice;
static MAX86161Settings_t max86161LeftSettings;
static MAX86161Settings_t max86161RightSettings;

static void changeI2CState(MAX86161Device_t* max86161device, I2C_HandleTypeDef *hi2c, uint8_t size) {
	HAL_StatusTypeDef operationResult = HAL_OK;
	switch(max86161device->i2cState) {
		case MAX86161_I2C_STATE_START:
			max86161device->i2cState = MAX86161_I2C_STATE_SEND_ADDRESS;
			if(max86161device->i2cAction == MAX86161_I2C_ACTION_READ_DATA) {
				operationResult = HAL_I2C_Master_Seq_Transmit_IT(hi2c, MAX86161_I2C_ADDRESS, max86161device->txBuffer, 1, I2C_FIRST_AND_LAST_FRAME);
			} else {
				operationResult = HAL_I2C_Master_Seq_Transmit_IT(hi2c, MAX86161_I2C_ADDRESS, max86161device->txBuffer, 1, I2C_FIRST_AND_NEXT_FRAME);
			}
			break;
		case MAX86161_I2C_STATE_SEND_ADDRESS:
			if(max86161device->i2cAction == MAX86161_I2C_ACTION_READ_DATA) {
				max86161device->i2cState = MAX86161_I2C_STATE_RECEIVE_DATA;
				operationResult = HAL_I2C_Master_Seq_Receive_IT(hi2c, MAX86161_I2C_ADDRESS, max86161device->rxBuffer, max86161device->i2cDataSize, I2C_LAST_FRAME);
			} else if(max86161device->i2cAction == MAX86161_I2C_ACTION_WRITE_DATA) {
				max86161device->i2cState = MAX86161_I2C_STATE_SEND_DATA;
				operationResult = HAL_I2C_Master_Seq_Transmit_IT(hi2c, MAX86161_I2C_ADDRESS, (max86161device->txBuffer + 1), 1, I2C_LAST_FRAME);
			}
			break;
		case MAX86161_I2C_STATE_SEND_DATA:
			max86161device->i2cState = MAX86161_I2C_STATE_COMPLETED;
			if(max86161device->settings->i2c->operations.receiveData != NULL) {
				max86161device->settings->i2c->operations.receiveData((void*)max86161device, max86161device->rxBuffer, 0);
			}
			break;
		case MAX86161_I2C_STATE_RECEIVE_DATA:
			max86161device->i2cState = MAX86161_I2C_STATE_COMPLETED;
			if(max86161device->settings->i2c->operations.receiveData != NULL) {
				max86161device->settings->i2c->operations.receiveData((void*)max86161device, max86161device->rxBuffer, max86161device->i2cDataSize);
			}
			break;
	}
	if(operationResult == HAL_ERROR) {
		max86161device->currentState = MAX86161_STATE_STOPPED;
		max86161device->error = MAX86161_ERROR_COMMUNICATION_FAILED;
	}
}

static void max86161RightEnableInterrupt() {
	__HAL_GPIO_EXTI_CLEAR_IT(MAX86161_RIGHT_INTERRUPT_PIN);
	HAL_NVIC_EnableIRQ(MAXM86161_RIGHT_INT_EXTI_IRQn);
}

// TODO: Move to main
static void max86161RightDisableInterrupt() {
	HAL_NVIC_DisableIRQ(MAXM86161_RIGHT_INT_EXTI_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(MAX86161_RIGHT_INTERRUPT_PIN);
}

static uint8_t max86161RightReadState() {
	return HAL_GPIO_ReadPin(MAX86161_RIGHT_INTERRUPT_PORT, MAX86161_RIGHT_INTERRUPT_PIN) == GPIO_PIN_RESET;
}

static void max86161LeftEnableInterrupt() {
	__HAL_GPIO_EXTI_CLEAR_IT(MAX86161_LEFT_INTERRUPT_PIN);
	HAL_NVIC_EnableIRQ(MAXM86161_LEFT_INT_EXTI_IRQn);
}

// TODO: Move to main
static void max86161LeftDisableInterrupt() {
	HAL_NVIC_DisableIRQ(MAXM86161_LEFT_INT_EXTI_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(MAX86161_LEFT_INTERRUPT_PIN);
}

static uint8_t max86161LeftReadState() {
	return HAL_GPIO_ReadPin(MAX86161_LEFT_INTERRUPT_PORT, MAX86161_LEFT_INTERRUPT_PIN) == GPIO_PIN_RESET;
}


void max86161RightLLInit(MAX86161PPGDataCallback_t callback) {
	max86161RightDisableInterrupt();
	max86161RightLLChipEnable();
	i2cMAX86161Right.operations.writeData = max86161RightLLWriteData;
	max86161RightSettings.enableInterruptFunction = max86161RightEnableInterrupt;
	max86161RightSettings.disableInterruptFunction = max86161RightDisableInterrupt;
	max86161RightSettings.readInterruptStateFunction = max86161RightReadState;
	max86161RightSettings.ppgDataCallback = callback;
	max86161RightSettings.i2c = &i2cMAX86161Right;
	max86161RightDevice.settings = &max86161RightSettings;
	max86161Init(&max86161RightDevice);
}

void max86161LeftLLInit(MAX86161PPGDataCallback_t callback) {
	max86161LeftDisableInterrupt();
	max86161LeftLLChipEnable();
	i2cMAX86161Left.operations.writeData = max86161LeftLLWriteData;
	max86161LeftSettings.enableInterruptFunction = max86161LeftEnableInterrupt;
	max86161LeftSettings.disableInterruptFunction = max86161LeftDisableInterrupt;
	max86161LeftSettings.readInterruptStateFunction = max86161LeftReadState;
	max86161LeftSettings.ppgDataCallback = callback;
	max86161LeftSettings.i2c = &i2cMAX86161Left;
	max86161LeftDevice.settings = &max86161LeftSettings;
	max86161Init(&max86161LeftDevice);
}

void max86161RightLLDeInit() {

}

void max86161LeftLLDeInit() {

}

void max86161RightLLChipEnable() {
	HAL_GPIO_WritePin(MAX86161_RIGHT_EN_PORT, MAX86161_RIGHT_EN_PIN, GPIO_PIN_SET);
}

void max86161RightLLChipDisable() {
	HAL_GPIO_WritePin(MAX86161_RIGHT_EN_PORT, MAX86161_RIGHT_EN_PIN, GPIO_PIN_RESET);
}

void max86161LeftLLChipEnable() {
	HAL_GPIO_WritePin(MAX86161_LEFT_EN_PORT, MAX86161_LEFT_EN_PIN, GPIO_PIN_SET);
}

void max86161LeftLLChipDisable() {
	HAL_GPIO_WritePin(MAX86161_LEFT_EN_PORT, MAX86161_LEFT_EN_PIN, GPIO_PIN_RESET);
}

void max86161RightLLWriteData(void* device, uint8_t* txBuffer, uint8_t size) {
	changeI2CState((MAX86161Device_t*)device, &hi2c2, size);
}


void max86161LeftLLWriteData(void* device, uint8_t* txBuffer, uint8_t size) {
	changeI2CState((MAX86161Device_t*)device, &hi2c1, size);
}


void max86161LLI2C1LeftTxHandler(I2C_HandleTypeDef *hi2c) {
	changeI2CState(&max86161LeftDevice, hi2c, hi2c->XferSize);
}

void max86161LLI2C1LeftRxHandler(I2C_HandleTypeDef *hi2c) {
	changeI2CState(&max86161LeftDevice, hi2c, hi2c->XferSize);
}

void max86161LLI2C1LeftErrorHandler(I2C_HandleTypeDef *hi2c) {
	//changeI2CState(&max86161RightDevice, hi2c, hi2c->XferSize);
}

void max86161LLI2C2RightTxHandler(I2C_HandleTypeDef *hi2c) {
	changeI2CState(&max86161RightDevice, hi2c, hi2c->XferSize);
}

void max86161LLI2C2RightRxHandler(I2C_HandleTypeDef *hi2c) {
	changeI2CState(&max86161RightDevice, hi2c, hi2c->XferSize);
}

void max86161LLI2C2RightErrorHandler(I2C_HandleTypeDef *hi2c) {

}


void max86161RightLLEXTIHandler() {

}

void max86161LeftLLEXTIHandler() {

}

void max86161RightLLInterruptTopHalfHandler() {
	max86161InterruptTopHalfHandler(&max86161RightDevice);
}

void max86161RightLLInterruptBottomHalfHandler() {
	max86161InterruptBottomHalfHandler(&max86161RightDevice);
}

void max86161LeftLLInterruptTopHalfHandler() {
	max86161InterruptTopHalfHandler(&max86161LeftDevice);
}

void max86161LeftLLInterruptBottomHalfHandler() {
	max86161InterruptBottomHalfHandler(&max86161LeftDevice);
}

void max86161RightLLTick() {
	max86161Tick(&max86161RightDevice);
}

void max86161LeftLLTick() {
	max86161Tick(&max86161LeftDevice);
}

uint8_t isMax86161Operation(I2C_HandleTypeDef *hi2c) {
	return ((uint16_t)(hi2c->Instance->CR2 & I2C_CR2_SADD) == MAX86161_I2C_ADDRESS);
}
