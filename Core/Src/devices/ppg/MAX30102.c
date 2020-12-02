/*
 * MAX30102.c
 *
 *  Created on: Oct 1, 2020
 *      Author: andrey
 */
#include "devices/ppg/MAX30102.h"
#include "stdio.h"


static void receiveData(void* device, uint8_t *buffer, uint8_t size);
static void readRegister(MAX30102Device_t* device, uint8_t address);
static void writeRegister(MAX30102Device_t* device, uint8_t address, uint8_t data);
static void powerReady(MAX30102Device_t* device);
static void configurate(MAX30102Device_t* device);
static void readConfigurationBack(MAX30102Device_t* device);
static void enqueueRegisterDataRequest(MAX30102Device_t* device, uint8_t address, uint8_t data, uint8_t isReadRequest);
static void dequeueRegisterDataRequest(MAX30102Device_t* device, uint8_t* address, uint8_t* data);
static void enqueueRxData(MAX30102Device_t* device, uint8_t data);
static uint8_t dequeueRxData(MAX30102Device_t* device);
static void reset(MAX30102Device_t* device);
static void readInterruptStatus1(MAX30102Device_t* device);
static void readInterruptStatus2(MAX30102Device_t* device);
static void readRevisionID(MAX30102Device_t* device);
static void readPartID(MAX30102Device_t* device);
static void readFIFOReadPointer(MAX30102Device_t* device);
static void readFIFOWritePointer(MAX30102Device_t* device);
static void readFIFODataSample(MAX30102Device_t* device);

static void changeState(MAX30102Device_t* device, uint8_t *buffer, uint8_t size) {
	//printf("changeState start, device->currentState %d\n", device->currentState);
	switch(device->currentState) {
		case MAX30102_STATE_IDLE:
			// Do nothing
			break;
		case MAX30102_STATE_START:
			device->currentState = MAX30102_STATE_RESET;
			reset(device);
			break;
		case MAX30102_STATE_READING_PART_ID:
			if(size == 1 && buffer[0] == MAX30102_PART_ID) {
				device->partId = buffer[0];
				device->currentState = MAX30102_STATE_READING_REVISION_ID;
				readRevisionID(device);
			} else {
				device->error = MAX30102_ERROR_INVALID_PART_ID;
				device->currentState = MAX30102_STATE_STOPPED;
			}
			break;
		case MAX30102_STATE_READING_REVISION_ID:
			if(size == 1 && buffer[0] == MAX30102_PART_ID) {
				device->revID = buffer[0];
			}

			device->currentState = MAX30102_STATE_WRITE_CONFIGURATIONS;
			configurate(device);
			//device->currentState = MAX30102_STATE_WRITE_CONFIGURATIONS;
			//configurate(device);
			//device->currentState = MAX30102_STATE_RESET;
			//reset(device);
			break;
		case MAX30102_STATE_WRITE_CONFIGURATIONS:
			device->currentState = MAX30102_STATE_READ_CONFIGURATIONS;
			readConfigurationBack(device);
			break;
		case MAX30102_STATE_READ_CONFIGURATIONS:
			device->currentState = MAX30102_STATE_IDLE;
			device->isConfigurated = 1;
			break;
		case MAX30102_STATE_RESET:
			device->currentState = MAX30102_STATE_READING_PART_ID;
			readPartID(device);
			//device->currentState = MAX30102_STATE_WRITE_CONFIGURATIONS;
			//configurate(device);
			//device->currentState = MAX30102_STATE_POWER_READY;
			//powerReady(device);
			break;
		case MAX30102_STATE_POWER_READY:
			if(size == 1 && (buffer[0] & MAX30102_REGISTER_INTERRUPT_STATUS_1_PWR_RDY) == MAX30102_REGISTER_INTERRUPT_STATUS_1_PWR_RDY) {
				device->currentState = MAX30102_STATE_WRITE_CONFIGURATIONS;
				configurate(device);
			} else {
				powerReady(device);
			}
			break;
		case MAX30102_STATE_READ_PPG:
			device->currentState = MAX30102_STATE_READ_INTERRUPT_STATUS1;
			readInterruptStatus1(device);
			break;
		case MAX30102_STATE_READ_FIFO_READ_POINTER:
			if(size == 1) {
				device->fifoReadPointer = buffer[0];
			}

			device->currentState = MAX30102_STATE_READ_FIFO_WRITE_POINTER;
			readFIFOWritePointer(device);
			break;
		case MAX30102_STATE_READ_FIFO_WRITE_POINTER:
			if(size == 1) {
				device->fifoWritePointer = buffer[0];
				device->fifoAvailableDataCount = device->fifoWritePointer - device->fifoReadPointer;
				if(device->fifoAvailableDataCount > 0) {
					device->currentState = MAX30102_STATE_READ_FIFO_DATA;
					readFIFODataSample(device);
				} else {
					// Nothing available in the FIFO
					device->currentState = MAX30102_STATE_IDLE;
				}
			} else {

			}
			break;
		case MAX30102_STATE_READ_FIFO_DATA:
			if(size == 6) {
				uint32_t redData = buffer[0] << 16;
				redData |= buffer[1] << 8;
				redData |= buffer[2];
				uint32_t irData = buffer[3] << 16;
				irData |= buffer[4] << 8;
				irData |= buffer[5];
				device->currentDataSample.redSample = redData;
				device->currentDataSample.irSample = irData;
				device->settings->ppgDataCallback(&(device->currentDataSample));
				//if(device->fifoAvailableDataCount > 1) {
				device->currentState = MAX30102_STATE_READ_INTERRUPT_STATUS1;
				readInterruptStatus1(device);
				//printf("R: %d, IR: %d\n", redData, irData);
				//} else {
				//	device->currentState = MAX30102_STATE_IDLE;
				//}

			} else {
				// Error
				printf("Read PPG error!\n");
			}


			break;
		case MAX30102_STATE_READ_INTERRUPT_STATUS1:

			if(size == 1) {
				uint8_t interruptStatus1 = buffer[0];
			}

			device->currentState = MAX30102_STATE_READ_FIFO_READ_POINTER;
			readFIFOReadPointer(device);
			break;
		case MAX30102_STATE_STOPPED:
			if(device->error != MAX30102_ERROR_NONE) {
				printf("MAX30102, error: %d !!!\n", device->error);
			}
			break;
	}
	//printf("changeState end, device->currentState %d\n", device->currentState);
}

void max30102Init(MAX30102Device_t* device) {
	device->settings->i2c->operations.receiveData = receiveData;
	device->isConfigurated = 0;
	device->isReadStarted = 0;
	device->currentState = MAX30102_STATE_IDLE;
	device->error = MAX30102_ERROR_NONE;
	device->i2cState = MAX30102_I2C_STATE_START;

	device->txSerialQueue = xQueueCreate(11, sizeof(uint16_t));
	device->rxSerialQueue = xQueueCreate(11, sizeof(uint8_t));

	device->settings->disableInterruptFunction();
}

void max30102DeInit(MAX30102Device_t* device) {
	for(uint16_t i = 0; i < MAX30102_TXRX_BUFFER_LENGTH; i++) {
		device->rxBuffer[i] = 0;
	}
	for(uint16_t i = 0; i < MAX30102_TXRX_BUFFER_LENGTH; i++) {
		device->txBuffer[i] = 0;
	}
}

void max30102Start(MAX30102Device_t* device) {
	device->settings->disableInterruptFunction();
	device->currentState = MAX30102_STATE_START;
	changeState(device, 0, 0);
	device->settings->enableInterruptFunction();
}

void max30102Stop(MAX30102Device_t* device) {
	device->settings->disableInterruptFunction();
	device->currentState = MAX30102_STATE_STOPPED;
	device->i2cState = MAX30102_I2C_STATE_START;
}

static void receiveData(void* device, uint8_t *buffer, uint8_t size) {

	MAX30102Device_t* devicePointer = (MAX30102Device_t*)device;
	devicePointer->settings->disableInterruptFunction();
	if(xQueueIsQueueEmptyFromISR(devicePointer->txSerialQueue) == pdTRUE) {
		devicePointer->isReadStarted = 0;
		uint8_t data = 0;
		uint8_t rxBufferIndex = 0;

		// Copy all enqueued bytes to the RX buffer
		while(xQueueIsQueueEmptyFromISR(devicePointer->rxSerialQueue) != pdTRUE) {
			xQueueReceiveFromISR(devicePointer->rxSerialQueue, &data, pdFALSE);
			devicePointer->rxBuffer[rxBufferIndex] = data >> 24;
			rxBufferIndex++;
		}

		// Copy the last received bytes
		if(size > 0) {
			for(uint8_t i = 0; i < size; i++) {
				devicePointer->rxBuffer[rxBufferIndex] = buffer[i];
				rxBufferIndex++;
			}
		}

		changeState(devicePointer, devicePointer->rxBuffer, rxBufferIndex);

	} else {

		if(size > 0) {
			for(uint8_t i = 0; i < size; i++) {
				enqueueRxData(device, buffer[i]);
			}
		}

		uint8_t firstRegisterAddress = 0;
		uint8_t firstRegisterData = 0;
		dequeueRegisterDataRequest(devicePointer, &firstRegisterAddress, &firstRegisterData);
		if(firstRegisterAddress & 0x80) {
			// Is a read register request
			firstRegisterAddress = firstRegisterAddress & 0x7F;
			readRegister(devicePointer, firstRegisterAddress);
		} else {
			// Is a write register request
			writeRegister(devicePointer, firstRegisterAddress, firstRegisterData);
		}

	}
	devicePointer->settings->enableInterruptFunction();
}

static void readRegister(MAX30102Device_t* device, uint8_t address) {
	device->isReadStarted = 1;
	device->i2cAction = MAX30102_I2C_ACTION_READ_DATA;
	device->i2cState = MAX30102_I2C_STATE_START;
	device->i2cDataSize = 1;
	device->txBuffer[0] = address;
	device->settings->i2c->operations.writeData((void*)device, (device->txBuffer), 1);
}

static void writeRegister(MAX30102Device_t* device, uint8_t address, uint8_t data) {
	device->isReadStarted = 1;
	device->i2cAction = MAX30102_I2C_ACTION_WRITE_DATA;
	device->i2cState = MAX30102_I2C_STATE_START;
	device->i2cDataSize = 1;
	device->txBuffer[0] = address;
	device->txBuffer[1] = data;
	device->settings->i2c->operations.writeData((void*)device, (device->txBuffer), 0);
}

static void powerReady(MAX30102Device_t* device) {
	readRegister(device, MAX30102_REGISTER_ADDRESS_INTERRUPT_STATUS_1);
}

static void configurate(MAX30102Device_t* device) {
	uint8_t firstRegisterAddress = 0;
	uint8_t firstRegisterData = 0;

	firstRegisterAddress = MAX30102_REGISTER_ADDRESS_FIFO_CONFIG;
	firstRegisterData = MAX30102_REGISTER_FIFO_CONFIG_SMP_AVE_4 |
			MAX30102_REGISTER_FIFO_CONFIG_ROLLOVER_EN |
			MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_17;

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_SPO2_CONFIG,
			MAX30102_REGISTER_SPO2_CONFIG_SPO2_ADC_RGE_16384 |
			MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_400 |
			MAX30102_REGISTER_SPO2_CONFIG_LED_PW_18bit,
			0);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_MODE_CONFIG,
			MAX30102_REGISTER_MODE_CONFIG_MODE_MULTI_LED,
			0);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_LED1_PULSE,
			MAX30102_REGISTER_ADDRESS_LED_CURRENT_10000uA,
			0);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_LED2_PULSE,
			MAX30102_REGISTER_ADDRESS_LED_CURRENT_10000uA,
			0);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_PROXIMITY_LED_PULSE,
			MAX30102_REGISTER_ADDRESS_LED_CURRENT_10000uA,
			0);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_MULTI_LED_CONTROL1,
			(MAX30102_REGISTER_MULTI_LED_CONTROL_SLOT_LED1_PA_RED << MAX30102_REGISTER_MULTI_LED_CONTROL1_SLOT1_POS) |
			(MAX30102_REGISTER_MULTI_LED_CONTROL_SLOT_LED2_PA_IR << MAX30102_REGISTER_MULTI_LED_CONTROL1_SLOT2_POS),
			0);


	//enqueueRegisterDataRequest(device,
	//		MAX30102_REGISTER_ADDRESS_MULTI_LED_CONTROL2,
	//		(MAX30102_REGISTER_MULTI_LED_CONTROL_SLOT_LED1_PA_RED << MAX30102_REGISTER_MULTI_LED_CONTROL2_SLOT3_POS) |
	//		(MAX30102_REGISTER_MULTI_LED_CONTROL_SLOT_LED2_PA_IR << MAX30102_REGISTER_MULTI_LED_CONTROL2_SLOT4_POS),
	//		0);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_INTERRUPT_ENABLE_1,
			MAX30102_REGISTER_INTERRUPT_ENABLE_1_A_FULL_EN,
			0);

	writeRegister(device,
			firstRegisterAddress,
			firstRegisterData);
}

static void readConfigurationBack(MAX30102Device_t* device) {
	uint8_t firstRegisterAddress = 0;

	firstRegisterAddress = MAX30102_REGISTER_ADDRESS_FIFO_CONFIG;

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_SPO2_CONFIG,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_MODE_CONFIG,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_LED1_PULSE,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_LED2_PULSE,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_PROXIMITY_LED_PULSE,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_MULTI_LED_CONTROL1,
			0,
			1);

	//enqueueRegisterDataRequest(device,
	//		MAX30102_REGISTER_ADDRESS_MULTI_LED_CONTROL2,
	//		0,
	//		1);

	enqueueRegisterDataRequest(device,
			MAX30102_REGISTER_ADDRESS_INTERRUPT_ENABLE_1,
			0,
			1);

	readRegister(device, firstRegisterAddress);
}

/*
 * Place register value in the TX queue, only write operations
 */
static void enqueueRegisterDataRequest(MAX30102Device_t* device, uint8_t address, uint8_t data, uint8_t isReadRequest) {
	uint16_t value = ((address | (isReadRequest << 7)) << 8) | data;
	xQueueSendFromISR(device->txSerialQueue, &value, pdFALSE);
}

/*
 * Get register value in the TX queue, only write operations
 */
static void dequeueRegisterDataRequest(MAX30102Device_t* device, uint8_t* address, uint8_t* data) {
	uint16_t value = 0;
	xQueueReceiveFromISR(device->txSerialQueue, &value, pdFALSE);
	*address = value >> 8;
	*data = value & 0x00FF;
}

/*
 * Place register value in the TX queue, only write operations
 */
static void enqueueRxData(MAX30102Device_t* device, uint8_t data) {
	xQueueSendFromISR(device->rxSerialQueue, &data, pdFALSE);
}

/*
 * Get register value in the TX queue, only write operations
 */
static uint8_t dequeueRxData(MAX30102Device_t* device) {
	uint8_t result = 0;
	xQueueReceiveFromISR(device->rxSerialQueue, &result, pdFALSE);
	return result;
}

static void reset(MAX30102Device_t* device) {
	writeRegister(device, MAX30102_REGISTER_ADDRESS_MODE_CONFIG, MAX30102_REGISTER_MODE_CONFIG_RESET);
}

static void readInterruptStatus1(MAX30102Device_t* device) {
	readRegister(device, MAX30102_REGISTER_ADDRESS_INTERRUPT_STATUS_1);
}

static void readInterruptStatus2(MAX30102Device_t* device) {
	readRegister(device, MAX30102_REGISTER_ADDRESS_INTERRUPT_STATUS_2);
}

static void readRevisionID(MAX30102Device_t* device) {
	readRegister(device, MAX30102_REGISTER_ADDRESS_REVISION_ID);
}

static void readPartID(MAX30102Device_t* device) {
	readRegister(device, MAX30102_REGISTER_ADDRESS_PART_ID);
}

static void readPPG(MAX30102Device_t* device) {

}

void max30102InterruptTopHalfHandler(MAX30102Device_t* device) {

}

void max30102InterruptBottomHalfHandler(MAX30102Device_t* device) {
	//if(device->isConfigurated) {
		//device->currentState = MAX30102_STATE_READ_PPG;
		if(device->isConfigurated && device->isReadStarted == 0) {
			if(device->settings->readInterruptStateFunction()) {
				device->currentState = MAX30102_STATE_READ_PPG;
				changeState(device, device->rxBuffer, 0);
			} else {
				device->currentState = MAX30102_STATE_IDLE;
			}
		}
	//}
}

static void readFIFOReadPointer(MAX30102Device_t* device) {
	readRegister(device, MAX30102_REGISTER_ADDRESS_FIFO_READ_POINTER);
}

static void readFIFOWritePointer(MAX30102Device_t* device) {
	readRegister(device, MAX30102_REGISTER_ADDRESS_FIFO_WRITE_POINTER);
}

static void readFIFODataSample(MAX30102Device_t* device) {
	device->isReadStarted = 1;
	device->i2cAction = MAX30102_I2C_ACTION_READ_DATA;
	device->i2cState = MAX30102_I2C_STATE_START;
	device->i2cDataSize = 6;
	device->txBuffer[0] = MAX30102_REGISTER_ADDRESS_FIFO_DATA;
	device->settings->i2c->operations.writeData((void*)device, (device->txBuffer), 1);
}

void max30102Tick(MAX30102Device_t* device) {
	//device->settings->disableInterruptFunction();

	//device->settings->enableInterruptFunction();
}


