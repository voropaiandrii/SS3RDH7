/*
 * saved_data.h
 *
 *  Created on: Nov 23, 2020
 *      Author: andrey
 */

#ifndef INC_DATA_SAVED_DATA_H_
#define INC_DATA_SAVED_DATA_H_

#include "stm32h7xx_hal.h"

#define SAVED_DATA_RESULT_OK						0
#define SAVED_DATA_RESULT_FAILED					1

uint8_t saveUByte(uint8_t registerAddress, uint8_t offset, uint8_t data);
uint8_t readUByte(uint8_t registerAddress, uint8_t offset);

uint16_t saveUShort(uint8_t registerAddress, uint8_t offset, uint8_t data);
uint16_t readUShort(uint8_t registerAddress, uint8_t offset);

uint32_t saveUInt(uint8_t registerAddress, uint8_t offset, uint8_t data);
uint32_t readUInt(uint8_t registerAddress, uint8_t offset);

uint32_t saveULong(uint8_t registerAddress, uint8_t offset, uint8_t data);
uint32_t readULong(uint8_t registerAddress, uint8_t offset);

#endif /* INC_DATA_SAVED_DATA_H_ */
