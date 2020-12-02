/*
 * MAX30003.c
 *
 *  Created on: Apr 26, 2020
 *      Author: andrey
 */

#include "devices/ecg/MAX30003.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "stdio.h"

static ECGData_t ecgEvent;
static QueueHandle_t txSerialQueue;
static QueueHandle_t rxSerialQueue;

static void reset(MAX30003Device_t* device);
static void configurate(MAX30003Device_t* device);
static void getConfigurationBack(MAX30003Device_t* device);
static void sync(MAX30003Device_t* device);
static void resetFifo(MAX30003Device_t* device);
static void readStatusRegister(MAX30003Device_t* device);
static void readRevisionIdentifier(MAX30003Device_t* device);
static void readECGData(MAX30003Device_t* device);
static void sendCommand(MAX30003Device_t* device, uint32_t* buffer, uint8_t size);

static void changeState(MAX30003Device_t* device, uint8_t *buffer, uint8_t size) {
	//printf("changeState, device->currentState %d\n", device->currentState);
	switch(device->currentState) {
		case MAX30003_STATE_IDLE:
			device->currentState = MAX30003_STATE_RESET;
			reset(device);
			break;
		case MAX30003_STATE_RESET:
			device->currentState = MAX30003_STATE_READING_REVISION;
			readRevisionIdentifier(device);
			break;
		case MAX30003_STATE_READING_REVISION:
			if(size >= 2) {
				if((buffer[1] & 0x50) == 0x50 && (buffer[2] & 0x30) == 0x30) {
					device->revisionID = buffer[1] & 0x0F;
					device->currentState = MAX30003_STATE_WRITE_CONFIGURATION;
					printf("device->revisionID %d\n", device->revisionID);
					configurate(device);
				} else {
					device->currentState = MAX30003_STATE_ERROR;
					device->error = MAX30003_ERROR_COMMUNICATION_FAILED;
					printf("Can't read device revision!\n");
				}
			}
			break;
		case MAX30003_STATE_WRITE_CONFIGURATION:
			device->currentState = MAX30003_STATE_READ_CONFIGURATION;
			getConfigurationBack(device);
			break;
		case MAX30003_STATE_READ_CONFIGURATION:
			device->isInitialCommandSent = 1;
			device->currentState = MAX30003_STATE_READING_STATUS;
			readStatusRegister(device);
			break;
		case MAX30003_STATE_SYNC:
			device->currentState = MAX30003_STATE_WAITING_FOR_TICK;
			//readECGData(device);
			break;
		case MAX30003_STATE_READING_STATUS:
			if(size == 4) {
				if(buffer[2] == 0x01) {
					device->currentState = MAX30003_STATE_READING_STATUS;
					readStatusRegister(device);
				} else {
					device->currentState = MAX30003_STATE_SYNC;
					sync(device);
				}
			}
			break;
		case MAX30003_STATE_WAITING_FOR_ECG_INTERRUPT:
			device->currentState = MAX30003_STATE_READING_ECG;
			readECGData(device);
			break;
		case MAX30003_STATE_WAITING_FOR_TICK:
			device->currentState = MAX30003_STATE_READING_ECG;
			readECGData(device);
			break;
		case MAX30003_STATE_READING_ECG:
			//if(size == 12) {
			if(size == 4) {
				//uint32_t statusRegister = (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
				//uint32_t rtorRegister = (buffer[4] << 16) | (buffer[5] << 8) | buffer[6];
				//uint32_t ecgFifoRegister = (buffer[9] << 16) | (buffer[10] << 8) | buffer[11];

				uint32_t ecgFifoRegister = (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
				uint8_t etag = ecgFifoRegister & MAX30003_REGISTER_ECG_FIFO_ETAG_MASK;
				uint8_t ptag = ecgFifoRegister & MAX30003_REGISTER_ECG_FIFO_PTAG_MASK;

				uint32_t voltage = ecgFifoRegister >> 8;
				if(voltage & 0x8000) {
					voltage |= 0xFFFF0000;
				}
				//uint16_t interval = rtorRegister >> 10;

				/*
				printf("statusRegister: 0x%08x\n", statusRegister);
				printf("rtorRegister: 0x%08x\n", rtorRegister);
				printf("ecgFifoRegister: 0x%08x\n", ecgFifoRegister);
				printf("etag: %d\n", etag);
				printf("ptag: %d\n", ptag);
				printf("voltage: %d\n", voltage);
				printf("interval: %d\n", interval);
				*/


				if(etag == MAX30003_REGISTER_ECG_FIFO_ETAG_VALID) {
					ecgEvent.sample = voltage;
					device->settings->ecgDataCallback(&ecgEvent);

					device->currentState = MAX30003_STATE_READING_ECG;
					readECGData(device);

				} else if(etag == MAX30003_REGISTER_ECG_FIFO_ETAG_LAST_VALID) {

					ecgEvent.sample = voltage;
					device->settings->ecgDataCallback(&ecgEvent);

					device->currentState = MAX30003_STATE_WAITING_FOR_TICK;
					//readECGData(device);

				} else if(etag == MAX30003_REGISTER_ECG_FIFO_ETAG_FAST_MODE || etag == MAX30003_REGISTER_ECG_FIFO_ETAG_LAST_FAST_MODE) {
					device->currentState = MAX30003_STATE_READING_ECG;
					readECGData(device);
				} else if(etag == MAX30003_REGISTER_ECG_FIFO_ETAG_FIFO_OVERFLOW) {
					device->currentState = MAX30003_STATE_SYNC;
					sync(device);
				} else {
					//device->currentState = MAX30003_STATE_WAITING_FOR_ECG_INTERRUPT;
					device->currentState = MAX30003_STATE_WAITING_FOR_TICK;
					//readECGData(device);
				}

			}
			break;
		case MAX30003_STATE_RESET_FIFO:
			device->currentState = MAX30003_STATE_WAITING_FOR_ECG_INTERRUPT;
			//readECGData(device);
			break;
		case MAX30003_STATE_ERROR:
			// TODO: Handle an error
			break;
		case MAX30003_STATE_CHANGE_MODE:

			break;
		case MAX30003_STATE_STOPPED:
			// Do nothing
			break;
		default:
			// TODO: Handle an error
			break;
	}
}

static void configurate(MAX30003Device_t* device) {
	if(device->isReadStarted == 0) {
		// 0x20
		uint32_t firstRegisterTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_CNFG_GEN << 1)) << 24;
		firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_ENABLE_ECG;								// Enable ECG channel
		if(device->mode == MAX30003_MODE_ECG) {
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGN_CONNECTED;					// Enable resistive bias on negative input
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGP_CONNECTED;					// Enable resistive bias on positive input
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_EN_RBIAS_RESISTIVE_BIAS_ENABLED;		// Enable resistive bias
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_10nA;						// Current magnitude = 10nA
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_RBIASV_RBIAS_50M;						// 200 MOhm bias
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_EN_DCLOFF_DC_DETECTION_DISABLED;		// Enable DC lead-off detection
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_FMSTR_32000HZ;						// Set Master Clock Frequency to 32000HZ to support 500 Hz sampling rate
		} else if(device->mode == MAX30003_MODE_CALIBRATION) {
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGN_DISCONNECTED;					// Enable resistive bias on negative input
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGP_DISCONNECTED;					// Enable resistive bias on positive input
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_EN_RBIAS_RESISTIVE_BIAS_ENABLED;		// Enable resistive bias
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_10nA;						// Current magnitude = 10nA
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_EN_DCLOFF_DC_DETECTION_DISABLED;		// Enable DC lead-off detection
			firstRegisterTemp |= MAX30003_REGISTER_CNFG_GEN_FMSTR_32000HZ;
		}

		// 0x2A
		uint32_t registerTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_CNFG_ECG << 1)) << 24;
		if(device->mode == MAX30003_MODE_ECG) {
			registerTemp |= MAX30003_REGISTER_CNFG_ECG_DLPF_40HZ;								// Digital LPF cutoff = 40Hz
			registerTemp |= MAX30003_REGISTER_CNFG_ECG_DHPF_0_50HZ;								// Digital HPF cutoff = 0.5Hz
		} else if(device->mode == MAX30003_MODE_CALIBRATION) {
			registerTemp |= MAX30003_REGISTER_CNFG_ECG_DLPF_BYPASS;								// Digital LPF turn off
			registerTemp |= MAX30003_REGISTER_CNFG_ECG_DHPF_BYPASS;								// Digital HPF turn off
		}
		registerTemp |= MAX30003_REGISTER_CNFG_ECG_GAIN_80;									// ECG gain = 160V/V
		registerTemp |= MAX30003_REGISTER_CNFG_ECG_RATE_HIGH;								// Sample rate = 500 sps
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x3A
		registerTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_CNFG_RTOR1 << 1)) << 24;
		if(device->mode == MAX30003_MODE_ECG) {
			registerTemp |= MAX30003_REGISTER_CNFG_RTOR1_WNDW_12;								// WNDW = 96ms
			registerTemp |= MAX30003_REGISTER_CNFG_RTOR1_GAIN_16384;						// Auto-scale gain
			registerTemp |= MAX30003_REGISTER_CNFG_RTOR1_PAVG_16;								// 16-average
			registerTemp |= MAX30003_REGISTER_CNFG_RTOR1_PTSF_4;								// PTSF = 4/16
			registerTemp |= MAX30003_REGISTER_CNFG_RTOR1_DETECTION_DISABLED;						// Enable R-to-R detection
		} else if(device->mode == MAX30003_MODE_CALIBRATION) {
			registerTemp |= MAX30003_REGISTER_CNFG_RTOR1_WNDW_12;								// WNDW = 96ms
			registerTemp |= MAX30003_REGISTER_CNFG_RTOR1_GAIN_AUTO_SCALE;						// Auto-scale gain
			registerTemp |= MAX30003_REGISTER_CNFG_RTOR1_PAVG_16;								// 16-average
			registerTemp |= MAX30003_REGISTER_CNFG_RTOR1_PTSF_4;								// PTSF = 4/16
			registerTemp |= MAX30003_REGISTER_CNFG_RTOR1_DETECTION_DISABLED;						// Enable R-to-R detection
		}
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x08
		registerTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_MNGR_INT << 1)) << 24;
		//registerTemp |= MAX30003_REGISTER_MNGR_INT_CLR_SAMP;
		//registerTemp |= MAX30003_REGISTER_MNGR_INT_SAMP_IT_EVERY_16ND_SAMPLE;				// Sync interrupt each 16 samples
		//registerTemp |= MAX30003_REGISTER_MNGR_INT_CLR_RRINT_CLEAR_RRINT_ON_RTOR;		    // Clear R-to-R on RTOR reg. read back
		registerTemp |= (0x07 << MAX30003_REGISTER_MNGR_INT_EFIT_POS);						// Interrupt if 8 samples is unread
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x04
		registerTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_EN_INT << 1)) << 24;
		registerTemp |= MAX30003_REGISTER_EN_INT_EINT;										// Enable EINT interrupt
		//registerTemp |= MAX30003_REGISTER_EN_INT_EN_SAMP;								 	// Enable R-to-R interrupt
		registerTemp |= MAX30003_REGISTER_EN_INT_INTB_TYPE_OPEN_DRAIN_NMOS_DRIVER_PULLUP;	// Open-drain NMOS with internal pullup
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x06
		registerTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_EN_INT_2 << 1)) << 24;
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x0A
		registerTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_MNGR_DYN << 1)) << 24;
		registerTemp |= MAX30003_REGISTER_MNGR_DYN_FAST_NORMAL_MODE;						// Fast recovery mode disabled
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x28
		registerTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_CNFG_EMUX << 1)) << 24;

		if(device->mode == MAX30003_MODE_ECG) {
			registerTemp |= MAX30003_REGISTER_CNFG_EMUX_OPENN_CONNECTED;						// Connect ECGN to AFE channel
			registerTemp |= MAX30003_REGISTER_CNFG_EMUX_OPENP_CONNECTED;						// Connect ECGP to AFE channel
		} else if(device->mode == MAX30003_MODE_CALIBRATION) {
			registerTemp |= MAX30003_REGISTER_CNFG_EMUX_OPENN_ISOLATED;						// Connect ECGN to AFE channel
			registerTemp |= MAX30003_REGISTER_CNFG_EMUX_OPENP_ISOLATED;						// Connect ECGP to AFE channel
			registerTemp |= MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_CONNECTE_TO_VCALN;						// Connect ECGN to AFE channel
			registerTemp |= MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_CONNECTE_TO_VCALP;						// Connect ECGP to AFE channel
		}
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		if(device->mode == MAX30003_MODE_CALIBRATION) {
			// 0x24
			registerTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_CNFG_CAL << 1)) << 24;
			registerTemp |= MAX30003_REGISTER_CNFG_CAL_EN_VCAL_ENABLE_CALIBRATION;
			registerTemp |= MAX30003_REGISTER_CNFG_CAL_VMODE_BIPOLAR;
			registerTemp |= MAX30003_REGISTER_CNFG_CAL_VMAG_250UV;							// 0.25mV
			registerTemp |= MAX30003_REGISTER_CNFG_CAL_FCAL_524288;							// Approximately 0.25Hz
			registerTemp |= MAX30003_REGISTER_CNFG_CAL_FIFTY_IGNORE_CAL_THIGH;				//  FMSTR [1:0] = 0b00, CAL_RES = 30.52us x 1023
			xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);
		}

		sendCommand(device, &firstRegisterTemp, 1);
	}
}

static void getConfigurationBack(MAX30003Device_t* device) {
	if(device->isReadStarted == 0) {
		// 0x21
		uint32_t firstRegisterTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_CNFG_GEN << 1)) << 24;

		// 0x2B
		uint32_t registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_CNFG_ECG << 1)) << 24;
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x3B
		registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_CNFG_RTOR1 << 1)) << 24;
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x09
		registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_MNGR_INT << 1)) << 24;
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x05
		registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_EN_INT << 1)) << 24;
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x07
		//registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_EN_INT_2 << 1)) << 24;
		//xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x0B
		registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_MNGR_DYN << 1)) << 24;
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x29
		registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_CNFG_EMUX << 1)) << 24;
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		if(device->mode == MAX30003_MODE_CALIBRATION) {
			// 0x25
			registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_CNFG_CAL << 1)) << 24;
			xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);
		}
		sendCommand(device, &firstRegisterTemp, 1);
	}
}

static void sync(MAX30003Device_t* device) {
	if(device->isReadStarted == 0) {
		// 0x12
		uint32_t firstRegisterTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_SYNCH << 1)) << 24;
		sendCommand(device, &firstRegisterTemp, 1);
	}
}

static void resetFifo(MAX30003Device_t* device) {
	if(device->isReadStarted == 0) {
		// 0x14
		uint32_t firstRegisterTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_FIFO_RST << 1)) << 24;
		sendCommand(device, &firstRegisterTemp, 1);
	}
}

static void reset(MAX30003Device_t* device) {
	if(device->isReadStarted == 0) {
		// 0x10
		uint32_t firstRegisterTemp = (MAX30003_COMMAND_WRITE | (MAX30003_REGISTER_ADDRESS_SW_RST << 1)) << 24;

		// 0x01 x 4
		uint32_t registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_NO_OP_1 << 1)) << 24;
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);
		xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);
		sendCommand(device, &firstRegisterTemp, 1);
	}
}

static void readStatusRegister(MAX30003Device_t* device) {
	if(device->isReadStarted == 0) {
		// 0x01
		uint32_t firstRegisterTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_STATUS << 1)) << 24;
		sendCommand(device, &firstRegisterTemp, 1);
	}
}

static void readRevisionIdentifier(MAX30003Device_t* device) {
	if(device->isReadStarted == 0) {
		// 0x1F
		uint32_t firstRegisterTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_INFO << 1)) << 24;
		sendCommand(device, &firstRegisterTemp, 1);
	}
}

static void receiveData(void* device, uint8_t *buffer, uint8_t size) {
	MAX30003Device_t* devicePointer = (MAX30003Device_t*)device;
	devicePointer->settings->spi->chipDisable();
	devicePointer->isReadStarted = 0;
	uint32_t data = 0;
	uint8_t rxBufferIndex = 0;

	if(size == 4) {
		data |= buffer[0] << 24;
		data |= buffer[1] << 16;
		data |= buffer[2] << 8;
		data |= buffer[3];

		xQueueSendFromISR(rxSerialQueue, &data, pdFALSE);
	}

	if(xQueueIsQueueEmptyFromISR(txSerialQueue) == pdTRUE) {

		while(xQueueIsQueueEmptyFromISR(rxSerialQueue) != pdTRUE) {
			xQueueReceiveFromISR(rxSerialQueue, &data, pdFALSE);

			devicePointer->rxBuffer[rxBufferIndex] = data >> 24;
			rxBufferIndex++;

			devicePointer->rxBuffer[rxBufferIndex] = data >> 16;
			rxBufferIndex++;

			devicePointer->rxBuffer[rxBufferIndex] = data >> 8;
			rxBufferIndex++;

			devicePointer->rxBuffer[rxBufferIndex] = data;
			rxBufferIndex++;
		}

		changeState(devicePointer, devicePointer->rxBuffer, rxBufferIndex);
	} else {
		xQueueReceiveFromISR(txSerialQueue, &data, pdFALSE);
		sendCommand(devicePointer, &data, 1);
	}
}

static void readECGData(MAX30003Device_t* device) {
	if(device->isReadStarted == 0) {
		// 0x21
		//uint32_t firstRegisterTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_STATUS << 1)) << 24;

		uint32_t firstRegisterTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_ECG_FIFO << 1)) << 24;

		// 0x4B
		//uint32_t registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_RTOR << 1)) << 24;
		//xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);

		// 0x43
		//registerTemp = (MAX30003_COMMAND_READ | (MAX30003_REGISTER_ADDRESS_ECG_FIFO << 1)) << 24;
		//xQueueSendFromISR(txSerialQueue, &registerTemp, pdFALSE);
		sendCommand(device, &firstRegisterTemp, 1);
	}
}

void max30003Initilize(MAX30003Device_t* device) {
	device->settings->spi->operations.receiveData = receiveData;
	device->isInitialCommandSent = 0;
	device->isReadStarted = 0;
	device->currentState = MAX30003_STATE_IDLE;
	device->error = MAX30003_ERROR_NONE;
	device->mode = MAX30003_MODE_ECG;
	//device->mode = MAX30003_MODE_CALIBRATION;

	txSerialQueue = xQueueCreate(11, sizeof(uint32_t));
	rxSerialQueue = xQueueCreate(11, sizeof(uint32_t));
}

void max30003DeInitilize(MAX30003Device_t* device) {
	for(uint16_t i = 0; i < MAX30003_TOTAL_BUFFER_LENGTH; i++) {
		device->rxBuffer[i] = 0;
	}
	for(uint16_t i = 0; i < MAX30003_TOTAL_BUFFER_LENGTH; i++) {
		device->txBuffer[i] = 0;
	}
}

void max30003InterruptTopHalfHandler(MAX30003Device_t* device) {

}

void max30003InterruptBottomHalfHandler(MAX30003Device_t* device) {
	//if(device->isInitialCommandSent && device->currentState == MAX30003_STATE_WAITING_FOR_ECG_INTERRUPT) {
	//	changeState(device, 0, 0);
	//}
}

void max30003Tick(MAX30003Device_t* device) {
	if(device->isInitialCommandSent == 1 && device->currentState == MAX30003_STATE_WAITING_FOR_TICK) {
		changeState(device, 0, 0);
	}
}

static void sendCommand(MAX30003Device_t* device, uint32_t* buffer, uint8_t size) {

	device->settings->spi->chipEnable();
	device->isReadStarted = 1;

	for(uint8_t i = 0; i < size; i++) {
		device->txBuffer[i * 4] = buffer[i] >> 24;
		device->txBuffer[i * 4 + 1] = buffer[i] >> 16;
		device->txBuffer[i * 4 + 2] = buffer[i] >> 8;
		device->txBuffer[i * 4 + 3] = buffer[i];
	}

	device->settings->spi->operations.writeData((void*)device, (device->txBuffer), size * 4);
}

void max30003Start(MAX30003Device_t* device) {
	device->currentState = MAX30003_STATE_IDLE;
	changeState(device, 0, 0);
}

void max30003Stop(MAX30003Device_t* device) {
	device->currentState = MAX30003_STATE_STOPPED;
}

void max30003SetMode(MAX30003Device_t* device, uint8_t m) {
	device->mode = m;
	device->currentState = MAX30003_STATE_READING_REVISION;
}

uint8_t max30003GetMode(MAX30003Device_t* device) {
	return device->mode;
}
