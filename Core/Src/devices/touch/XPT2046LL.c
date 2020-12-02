//
// Created by Andrey on 2020-03-28.
//
#include "devices/touch/XPT2046.h"
#include "devices/touch/XPT2046LL.h"

static XPT2046Device_t xpt2046Device;
static SPI_t spiTouch;
static XPT2046Settings_t xpt2046Settings;

// TODO: Move to main
static void enablePenInterrupt() {
	//EXTI->IMR &= ~EXTI_IMR_IM1;
	__HAL_GPIO_EXTI_CLEAR_IT(XPT2046_PEN_INTERRUPT_PIN);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	//HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET);
}

// TODO: Move to main
static void disablePenInterrupt() {
	//EXTI->IMR |= EXTI_IMR_IM1;
	HAL_NVIC_DisableIRQ(EXTI1_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(XPT2046_PEN_INTERRUPT_PIN);
	//HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET);
}

// TODO: Move to main
static uint8_t readPenState() {
	return HAL_GPIO_ReadPin(XPT2046_PEN_INTERRUPT_PORT, XPT2046_PEN_INTERRUPT_PIN) == GPIO_PIN_RESET;
}

void xpt2046LowLevelInit(XPT2046TouchEventCallback_t callback) {
	spiTouch.operations.writeData = xpt2046LowLevelWriteData;
    spiTouch.chipEnable = xpt2046LowLevelChipEnable;
    spiTouch.chipDisable = xpt2046LowLevelChipDisable;

	xpt2046Settings.spi = &spiTouch;
	xpt2046Settings.touchEventCallback = callback;
	xpt2046Settings.enablePenInterruptFunction = enablePenInterrupt;
	xpt2046Settings.disablePenInterruptFunction = disablePenInterrupt;
	xpt2046Settings.readPenStateFunction = readPenState;

	xpt2046Device.settings = &xpt2046Settings;

    xpt2046Initialize(&xpt2046Device);
}

void xpt2046LowLevelDeInit() {

}

// TODO: Move to main
void xpt2046LowLevelChipEnable() {
    HAL_GPIO_WritePin(XPT2046_CHIP_ENABLE_PORT, XPT2046_CHIP_ENABLE_PIN, GPIO_PIN_RESET);
}

// TODO: Move to main
void xpt2046LowLevelChipDisable() {
    HAL_GPIO_WritePin(XPT2046_CHIP_ENABLE_PORT, XPT2046_CHIP_ENABLE_PIN, GPIO_PIN_SET);
}

// TODO: Move to main
void xpt2046LowLevelWriteData(void* deviceStructurePointer, uint8_t* txBuffer, uint8_t size) {
    HAL_SPI_TransmitReceive_IT(&hspi1, txBuffer, ((XPT2046Device_t*)deviceStructurePointer)->rxBuffer, size);
}

// TODO: Move to main
void xpt2046LowLevelSpiTxRxHandler(SPI_HandleTypeDef *hspi) {
	if(spiTouch.operations.receiveData != NULL) {
		spiTouch.operations.receiveData((void*)&xpt2046Device, xpt2046Device.rxBuffer, XPT2046_DATA_TRANSFER_BUFFER_SIZE);
	}
}
void xpt2046LowLevelEXTIHandler() {
	if(xpt2046Device.isInitialCommandSent) {
		xpt2046PenInterruptTopHalfHandler(&xpt2046Device);
		osThreadResume(touchIRQTaskHandle);
	}
}

void xpt2046LowLevelTick() {
	xpt2046Tick(&xpt2046Device);
}


void xpt2046LowLevelPenInterruptBottomHalfHandler() {
	xpt2046PenInterruptBottomHalfHandler(&xpt2046Device);
}
