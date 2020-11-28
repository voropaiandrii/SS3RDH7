/*
 * saved_data.c
 *
 *  Created on: Nov 23, 2020
 *      Author: andrey
 */

#include "data/saved_data.h"
#include "stm32h7xx_hal_rtc_ex.h"

extern RTC_HandleTypeDef hrtc;

uint8_t saveUByte(uint8_t registerAddress, uint8_t offset, uint8_t data) {
	//__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
	uint32_t currentRegisterValue = HAL_RTCEx_BKUPRead(&hrtc, registerAddress);
	currentRegisterValue &= ~(0xFF << offset);
	currentRegisterValue |= data << offset;
	HAL_RTCEx_BKUPWrite(&hrtc, registerAddress, currentRegisterValue);
	//HAL_PWR_DiBkUpAccess();
	//__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
	return SAVED_DATA_RESULT_OK;
}

uint8_t readUByte(uint8_t registerAddress, uint8_t offset) {
	uint8_t result;
	result = (uint8_t)(HAL_RTCEx_BKUPRead(&hrtc, registerAddress) >> offset);
	return result;
}

uint16_t saveUShort(uint8_t registerAddress, uint8_t offset, uint8_t data) {
	return 0;
}

uint16_t readUShort(uint8_t registerAddress, uint8_t offset) {
	return 0;
}

uint32_t saveUInt(uint8_t registerAddress, uint8_t offset, uint8_t data) {
	return 0;
}

uint32_t readUInt(uint8_t registerAddress, uint8_t offset) {
	return 0;
}

uint32_t saveULong(uint8_t registerAddress, uint8_t offset, uint8_t data) {
	return 0;
}

uint32_t readULong(uint8_t registerAddress, uint8_t offset) {
	return 0;
}
