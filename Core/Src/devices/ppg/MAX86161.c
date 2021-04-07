/*
 * MAX86161.c
 *
 *  Created on: Oct 16, 2020
 *      Author: andrey
 */

#include "devices/ppg/MAX86161.h"



static void receiveData(void* device, uint8_t *buffer, uint8_t size);
static void readRegister(MAX86161Device_t* device, uint8_t address);
static void writeRegister(MAX86161Device_t* device, uint8_t address, uint8_t data);
static void powerReady(MAX86161Device_t* device);
static void configurate(MAX86161Device_t* device);
static void readConfigurationBack(MAX86161Device_t* device);
static void enqueueRegisterDataRequest(MAX86161Device_t* device, uint8_t address, uint8_t data, uint8_t isReadRequest);
static void dequeueRegisterDataRequest(MAX86161Device_t* device, uint8_t* address, uint8_t* data);
static void enqueueRxData(MAX86161Device_t* device, uint8_t data);
static uint8_t dequeueRxData(MAX86161Device_t* device);
static void reset(MAX86161Device_t* device);
static void shutdown(MAX86161Device_t* device);
static void powerup(MAX86161Device_t* device);
static void readInterruptStatus1(MAX86161Device_t* device);
static void readInterruptStatus2(MAX86161Device_t* device);
static void readRevisionID(MAX86161Device_t* device);
static void readPartID(MAX86161Device_t* device);
static void readFIFOReadPointer(MAX86161Device_t* device);
static void readFIFOWritePointer(MAX86161Device_t* device);
static void readFIFODataSample(MAX86161Device_t* device);
static void readFIFOCounter(MAX86161Device_t* device);

static void changeState(MAX86161Device_t* device, uint8_t *buffer, uint8_t size) {
	//printf("changeState, device->currentState %d\n", device->currentState);
	switch(device->currentState) {
		case MAX86161_STATE_IDLE:
			// Do nothing
			break;
		case MAX86161_STATE_START:
			device->currentState = MAX86161_STATE_RESET;
			reset(device);
			break;
		case MAX86161_STATE_RESET:
			device->currentState = MAX86161_STATE_SHUTDOWN;
			shutdown(device);
			break;
		case MAX86161_STATE_SHUTDOWN:
			device->currentState = MAX86161_STATE_CLEAR_INTERRUPT1;
			readInterruptStatus1(device);
			break;
		case MAX86161_STATE_CLEAR_INTERRUPT1:
			device->currentState = MAX86161_STATE_CLEAR_INTERRUPT2;
			readInterruptStatus2(device);
			break;
		case MAX86161_STATE_CLEAR_INTERRUPT2:
			device->currentState = MAX86161_STATE_READING_PART_ID;
			readPartID(device);
			break;
		case MAX86161_STATE_READING_PART_ID:
			if(size == 1 && buffer[0] == MAX86161_PART_ID) {
				device->partId = buffer[0];
				device->currentState = MAX86161_STATE_READING_REVISION_ID;
				readRevisionID(device);
			} else {
				device->error = MAX86161_ERROR_INVALID_PART_ID;
				device->currentState = MAX86161_STATE_STOPPED;
			}
			break;
		case MAX86161_STATE_READING_REVISION_ID:
			if(size == 1 && buffer[0] == MAX86161_PART_ID) {
				device->revID = buffer[0];
			}

			device->currentState = MAX86161_STATE_WRITE_CONFIGURATIONS;
			configurate(device);
			break;

		case MAX86161_STATE_WRITE_CONFIGURATIONS:
			device->currentState = MAX86161_STATE_READ_CONFIGURATIONS;
			readConfigurationBack(device);
			break;
		case MAX86161_STATE_READ_CONFIGURATIONS:
			device->currentState = MAX86161_STATE_POWER_UP;
			powerup(device);
			break;
		case MAX86161_STATE_POWER_UP:
			device->isConfigurated = 1;
			break;
		case MAX86161_STATE_READ_PPG:
			device->currentState = MAX86161_STATE_READ_PPG_STATUS;
			readInterruptStatus1(device);
			break;
		case MAX86161_STATE_READ_PPG_STATUS:
			if(size == 1) {
				if(buffer[0] & MAX86161_REGISTER_INTERRUPT_STATUS_1_A_FULL) {
					//device->currentState = MAX86161_STATE_READ_FIFO_READ_POINTER;
					//readFIFOReadPointer(device);
					device->currentState = MAX86161_STATE_READ_FIFO_READ_COUNTER;
					readFIFOCounter(device);
				} else {
					device->currentState = MAX86161_STATE_IDLE;
				}
			}
			break;
		case MAX86161_STATE_READ_FIFO_READ_POINTER:
			device->currentState = MAX86161_STATE_READ_FIFO_WRITE_POINTER;
			if(size == 1) {
				device->fifoReadPointer = buffer[0];
			}
			readFIFOWritePointer(device);
			break;
		case MAX86161_STATE_READ_FIFO_WRITE_POINTER:
			if(size == 1) {
				device->fifoWritePointer = buffer[0];
				device->fifoAvailableDataCount = device->fifoWritePointer - device->fifoReadPointer;
				if(device->fifoAvailableDataCount > 0) {
					device->currentState = MAX86161_STATE_READ_FIFO_DATA;
					readFIFODataSample(device);
				} else {
					// Nothing available in the FIFO
					device->currentState = MAX86161_STATE_IDLE;
				}
			} else {

			}
			break;
		case MAX86161_STATE_READ_FIFO_READ_COUNTER:
			if(size == 1) {
				device->fifoAvailableDataCount = buffer[0];// - (buffer[0] % MAXM86161_BYTES_NUMBER_OF_CHANNELS);
				if(device->fifoAvailableDataCount > 0) {
					device->currentState = MAX86161_STATE_READ_FIFO_DATA;
					readFIFODataSample(device);
				} else {
					// Nothing available in the FIFO
					device->currentState = MAX86161_STATE_IDLE;
				}
			} else {

			}
			break;
		case MAX86161_STATE_READ_FIFO_DATA:
			if(size == MAXM86161_BYTES_PER_CHANNEL * device->fifoAvailableDataCount) {
				for(uint8_t i = 0; i < device->fifoAvailableDataCount / MAXM86161_BYTES_NUMBER_OF_CHANNELS; i++) {
					device->currentDataSample.greenSample = (buffer[i * MAXM86161_BYTES_PER_SAMPLE] & 0x0007) << 16;
					device->currentDataSample.greenSample |= buffer[i * MAXM86161_BYTES_PER_SAMPLE + 1] << 8;
					device->currentDataSample.greenSample |= buffer[i * MAXM86161_BYTES_PER_SAMPLE + 2];
					device->currentDataSample.irSample  = (buffer[i * MAXM86161_BYTES_PER_SAMPLE + 3] & 0x0007) << 16;
					device->currentDataSample.irSample  |= buffer[i * MAXM86161_BYTES_PER_SAMPLE + 4] << 8;
					device->currentDataSample.irSample  |= buffer[i * MAXM86161_BYTES_PER_SAMPLE + 5];
					device->currentDataSample.redSample = (buffer[i * MAXM86161_BYTES_PER_SAMPLE + 6] & 0x0007) << 16;
					device->currentDataSample.redSample |= buffer[i * MAXM86161_BYTES_PER_SAMPLE + 7] << 8;
					device->currentDataSample.redSample |= buffer[i * MAXM86161_BYTES_PER_SAMPLE + 8];
					device->settings->ppgDataCallback(&(device->currentDataSample));
				}
				device->currentState = MAX86161_STATE_READ_INTERRUPT_STATUS1;
				readInterruptStatus1(device);
			} else {
				// Error
				device->currentState = MAX86161_STATE_IDLE;
			}
			break;
		case MAX86161_STATE_STOPPED:
			break;
	}
}

void max86161Init(MAX86161Device_t* device) {
	device->settings->i2c->operations.receiveData = receiveData;
	device->isConfigurated = 0;
	device->isReadStarted = 0;
	device->currentState = MAX86161_STATE_IDLE;
	device->error = MAX86161_ERROR_NONE;
	device->i2cState = MAX86161_I2C_STATE_START;

	device->txSerialQueue = xQueueCreate(15, sizeof(uint16_t));
	device->rxSerialQueue = xQueueCreate(15, sizeof(uint8_t));
}

void max86161DeInit(MAX86161Device_t* device) {
	for(uint16_t i = 0; i < MAX86161_TXRX_BUFFER_LENGTH; i++) {
		device->rxBuffer[i] = 0;
	}
	for(uint16_t i = 0; i < MAX86161_TXRX_BUFFER_LENGTH; i++) {
		device->txBuffer[i] = 0;
	}
}

void max86161Start(MAX86161Device_t* device) {
	device->currentDataSample.greenSample = 0;
	device->currentDataSample.redSample = 0;
	device->currentDataSample.irSample = 0;
	device->currentState = MAX86161_STATE_START;
	changeState(device, 0, 0);
}

void max86161Stop(MAX86161Device_t* device) {
	device->currentState = MAX86161_STATE_STOPPED;
	device->i2cState = MAX86161_I2C_STATE_START;
}

void max86161Tick(MAX86161Device_t* device) {
	if(device->isConfigurated && device->isReadStarted == 0) {
		if(device->settings->readInterruptStateFunction()) {
			device->currentState = MAX86161_STATE_READ_PPG;
			changeState(device, device->rxBuffer, 0);
		} else {
			device->currentState = MAX86161_STATE_IDLE;
		}
	}
}

void max86161InterruptTopHalfHandler(MAX86161Device_t* device) {

}

void max86161InterruptBottomHalfHandler(MAX86161Device_t* device) {

}

static void receiveData(void* device, uint8_t *buffer, uint8_t size) {
	MAX86161Device_t* devicePointer = (MAX86161Device_t*)device;

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
}
static void readRegister(MAX86161Device_t* device, uint8_t address) {
	device->isReadStarted = 1;
	device->i2cAction = MAX86161_I2C_ACTION_READ_DATA;
	device->i2cState = MAX86161_I2C_STATE_START;
	device->i2cDataSize = 1;
	device->txBuffer[0] = address;
	device->settings->i2c->operations.writeData((void*)device, (device->txBuffer), 1);
}

static void writeRegister(MAX86161Device_t* device, uint8_t address, uint8_t data) {
	device->isReadStarted = 1;
	device->i2cAction = MAX86161_I2C_ACTION_WRITE_DATA;
	device->i2cState = MAX86161_I2C_STATE_START;
	device->i2cDataSize = 1;
	device->txBuffer[0] = address;
	device->txBuffer[1] = data;
	device->settings->i2c->operations.writeData((void*)device, (device->txBuffer), 0);
}

static void powerReady(MAX86161Device_t* device) {

}

static void configurate(MAX86161Device_t* device) {
	uint8_t firstRegisterAddress = 0;
	uint8_t firstRegisterData = 0;

	firstRegisterAddress = MAX86161_REGISTER_ADDRESS_SYSTEM_CONTROL;
	firstRegisterData = MAX86161_REGISTER_SYSTEM_CONTROL_SINGLE_PPG;

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_PPG_CONFIGURATION_1,
			MAX86161_REGISTER_PPG_CONFIGURATION_1_PPG_TINT_14800_NS |
			MAX86161_REGISTER_PPG_CONFIGURATION_1_PPG1_ADC_RGE_32768,
			0);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_PPG_CONFIGURATION_2,
			MAX86161_REGISTER_PPG_CONFIGURATION_2_PPG_SR_1024_HZ |
			MAX86161_REGISTER_PPG_CONFIGURATION_2_SMP_AVE_4,
			0);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_PPG_CONFIGURATION_3,
			MAX86161_REGISTER_PPG_CONFIGURATION_3_LED_SETLNG_4_US,
			0);

	enqueueRegisterDataRequest(device,
				MAX86161_REGISTER_ADDRESS_PPG_SYNC_CONTROL,
				MAX86161_REGISTER_PPG_SYNC_CONTROL_GPIO_CTRL_IN_TRIGGER,
				0);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_PHOTO_DIODE_BIAS,
			MAX86161_REGISTER_PHOTO_DIODE_BIAS_PDBIAS1_65_PF |
			MAX86161_REGISTER_PHOTO_DIODE_BIAS_PDBIAS2_65_PF,
			0);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED_RANGE_1,
			MAX86161_REGISTER_LED_RANGE1_LED1_RGE_62 |
			MAX86161_REGISTER_LED_RANGE1_LED1_RGE_62 |
			MAX86161_REGISTER_LED_RANGE1_LED1_RGE_62,
			0);

	uint8_t ledCurrent = MAX86161_REGISTER_LED_PA(MAX86161_REGISTER_LED_RANGE1_LED1_RGE_62, 60);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED1_PULSE_AMPLITUDE,
			ledCurrent,
			0);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED2_PULSE_AMPLITUDE,
			ledCurrent,
			0);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED3_PULSE_AMPLITUDE,
			ledCurrent,
			0);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_FIFO_CONFIGURATION_1,
			MAX86161_REGISTER_FIFO_FIFO_CONFIGURATION_1_FIFO_A_FULL_60,
			0);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_INTERRUPT_ENABLE_1,
			MAX86161_REGISTER_INTERRUPT_ENABLE_1_A_FULL_EN,
			0);

	/*
	 * LED1 - Green LED
	 * LED3 - Red LED
	 * LED2 - IR LED
	 */

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED_SEQ_CONTROL_1,
			MAX86161_REGISTER_LED_SEQUENCE_REGISTER_1_LEDC1_LED1 |
			MAX86161_REGISTER_LED_SEQUENCE_REGISTER_1_LEDC2_LED3,
			0);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED_SEQ_CONTROL_2,
			MAX86161_REGISTER_LED_SEQUENCE_REGISTER_2_LEDC3_LED2 |
			MAX86161_REGISTER_LED_SEQUENCE_REGISTER_2_LEDC4_NONE ,
			0);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED_SEQ_CONTROL_3,
			MAX86161_REGISTER_LED_SEQUENCE_REGISTER_3_LEDC5_NONE |
			MAX86161_REGISTER_LED_SEQUENCE_REGISTER_3_LEDC6_NONE,
			0);

	writeRegister(device,
			firstRegisterAddress,
			firstRegisterData);
}

static void readConfigurationBack(MAX86161Device_t* device) {
	uint8_t firstRegisterAddress = 0;

	firstRegisterAddress = MAX86161_REGISTER_ADDRESS_SYSTEM_CONTROL;

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_PPG_CONFIGURATION_1,
				0,
				1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_PPG_CONFIGURATION_2,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_PPG_CONFIGURATION_3,
			0,
			1);

	enqueueRegisterDataRequest(device,
				MAX86161_REGISTER_ADDRESS_PPG_SYNC_CONTROL,
				0,
				1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_PHOTO_DIODE_BIAS,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED_RANGE_1,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED1_PULSE_AMPLITUDE,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED2_PULSE_AMPLITUDE,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED3_PULSE_AMPLITUDE,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_FIFO_CONFIGURATION_1,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_INTERRUPT_ENABLE_1,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED_SEQ_CONTROL_1,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED_SEQ_CONTROL_2,
			0,
			1);

	enqueueRegisterDataRequest(device,
			MAX86161_REGISTER_ADDRESS_LED_SEQ_CONTROL_3,
			0,
			1);

	readRegister(device, firstRegisterAddress);
}

/*
 * Place register value in the TX queue, only write operations
 */
static void enqueueRegisterDataRequest(MAX86161Device_t* device, uint8_t address, uint8_t data, uint8_t isReadRequest) {
	uint16_t value = ((address | (isReadRequest << 7)) << 8) | data;
	xQueueSendFromISR(device->txSerialQueue, &value, pdFALSE);
}

/*
 * Get register value in the TX queue, only write operations
 */
static void dequeueRegisterDataRequest(MAX86161Device_t* device, uint8_t* address, uint8_t* data) {
	uint16_t value = 0;
	xQueueReceiveFromISR(device->txSerialQueue, &value, pdFALSE);
	*address = value >> 8;
	*data = value & 0x00FF;
}

/*
 * Place register value in the TX queue, only write operations
 */
static void enqueueRxData(MAX86161Device_t* device, uint8_t data) {
	xQueueSendFromISR(device->rxSerialQueue, &data, pdFALSE);
}

/*
 * Get register value in the TX queue, only write operations
 */
static uint8_t dequeueRxData(MAX86161Device_t* device) {
	uint8_t result = 0;
	xQueueReceiveFromISR(device->rxSerialQueue, &result, pdFALSE);
	return result;
}

static void reset(MAX86161Device_t* device) {
	writeRegister(device, MAX86161_REGISTER_ADDRESS_SYSTEM_CONTROL, MAX86161_REGISTER_SYSTEM_CONTROL_RESET);
}

static void shutdown(MAX86161Device_t* device) {
	writeRegister(device, MAX86161_REGISTER_ADDRESS_SYSTEM_CONTROL, MAX86161_REGISTER_SYSTEM_CONTROL_SHDN);
}

static void powerup(MAX86161Device_t* device) {
	writeRegister(device, MAX86161_REGISTER_ADDRESS_SYSTEM_CONTROL, 0);
}

static void readInterruptStatus1(MAX86161Device_t* device) {
	readRegister(device, MAX86161_REGISTER_ADDRESS_INTERRUPT_STATUS_1);
}

static void readInterruptStatus2(MAX86161Device_t* device) {
	readRegister(device, MAX86161_REGISTER_ADDRESS_INTERRUPT_STATUS_2);
}

static void readRevisionID(MAX86161Device_t* device) {
	readRegister(device, MAX86161_REGISTER_ADDRESS_REVISION_ID);
}

static void readPartID(MAX86161Device_t* device) {
	readRegister(device, MAX86161_REGISTER_ADDRESS_PART_ID);
}

static void readFIFOReadPointer(MAX86161Device_t* device) {
	readRegister(device, MAX86161_REGISTER_ADDRESS_FIFO_READ_POINTER);
}

static void readFIFOWritePointer(MAX86161Device_t* device) {
	readRegister(device, MAX86161_REGISTER_ADDRESS_FIFO_WRITE_POINTER);
}

static void readFIFODataSample(MAX86161Device_t* device) {
	device->isReadStarted = 1;
	device->i2cAction = MAX86161_I2C_ACTION_READ_DATA;
	device->i2cState = MAX86161_I2C_STATE_START;
	device->i2cDataSize = MAXM86161_BYTES_PER_CHANNEL * device->fifoAvailableDataCount;
	device->txBuffer[0] = MAX86161_REGISTER_ADDRESS_FIFO_DATA_REGISTER;
	device->settings->i2c->operations.writeData((void*)device, (device->txBuffer), 1);
}

static void readFIFOCounter(MAX86161Device_t* device) {
	readRegister(device, MAX86161_REGISTER_ADDRESS_FIFO_DATA_COUNTER);
}
