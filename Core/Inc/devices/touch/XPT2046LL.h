//
// Created by Andrey on 2020-03-28.
//

#ifndef CORE429I_BOARD_XPT2046LL_H
#define CORE429I_BOARD_XPT2046LL_H

#include "stm32h7xx_hal.h"
#include "cmsis_os.h"
#include <devices/io_device_types.h>
#include "devices/touch/XPT2046.h"
#include "main.h"

#define XPT2046_PEN_INTERRUPT_PORT    TOUCH_PIRQ_GPIO_Port
#define XPT2046_PEN_INTERRUPT_PIN     TOUCH_PIRQ_Pin

#define XPT2046_CHIP_ENABLE_PORT    TOUCH_CS_GPIO_Port
#define XPT2046_CHIP_ENABLE_PIN     TOUCH_CS_Pin

#define TOUCH_PIRQ_GPIO_Port        GPIOI
#define TOUCH_PIRQ_Pin              GPIO_PIN_1

extern SPI_HandleTypeDef hspi1;
extern osThreadId touchIRQTaskHandle;;

void xpt2046LowLevelInit(XPT2046TouchEventCallback_t callback);
void xpt2046LowLevelDeInit();
void xpt2046LowLevelChipEnable();
void xpt2046LowLevelChipDisable();
void xpt2046LowLevelWriteData(void* device, uint8_t* txBuffer, uint8_t size);
void xpt2046LowLevelSpiTxRxHandler(SPI_HandleTypeDef *hspi);
void xpt2046LowLevelEXTIHandler();
void xpt2046LowLevelTick();
void xpt2046LowLevelPenInterruptBottomHalfHandler();

#endif //CORE429I_BOARD_XPT2046LL_H

