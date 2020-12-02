//
// Created by Andrey on 2020-03-15.
//

#ifndef CORE429I_BOARD_IO_DEVICE_TYPES_H
#define CORE429I_BOARD_IO_DEVICE_TYPES_H

#include <stdint.h>

typedef void (*ReceiveData_t)(void* device, uint8_t* buffer, uint8_t size);
typedef void (*WriteData_t)(void* device, uint8_t* buffer, uint8_t size);

typedef void (*ChipEnable_t)();
typedef void (*ChipDisable_t)();

typedef struct {
    ReceiveData_t receiveData;
    WriteData_t writeData;
} BytePort_t;

typedef struct {
    BytePort_t operations;
    ChipEnable_t chipEnable;
    ChipDisable_t chipDisable;
} SPI_t;

typedef struct {
	BytePort_t operations;
} I2C_t;




#endif //CORE429I_BOARD_IO_DEVICE_TYPES_H
