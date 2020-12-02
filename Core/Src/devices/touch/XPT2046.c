//
// Created by Andrey on 2020-03-14.
//
#include <stdio.h>
#include "devices/touch/XPT2046.h"

static void readValue(XPT2046Device_t* device) {
    if (device->settings->spi != NULL) {
    	device->settings->disablePenInterruptFunction();
    	if(device->isInitialCommandSent == 0 || device->settings->readPenStateFunction()) {
    		device->isReadStarted = 1;
			if (device->settings->spi->chipEnable != NULL) {
				device->settings->spi->chipEnable();
			}

			device->txBuffer[0] = (0x80
						   | XPT2046_CONTROL_BYTE_MODE_SINGLE_ENDED
						   | XPT2046_CONTROL_BYTE_MODE_12_BIT
						   | XPT2046_CONTROL_BYTE_ADC_SINGLE_CHANNEL_VBAT
						   | XPT2046_CONTROL_BYTE_POWER_INTERNAL_REFERENCE_ON
						   | XPT2046_CONTROL_BYTE_POWER_FULL_POWER);
			device->txBuffer[2] = device->txBuffer[0];
			device->txBuffer[4] = device->txBuffer[0];
			device->txBuffer[6] = device->txBuffer[0];


			device->txBuffer[8] = (0x80
						   | XPT2046_CONTROL_BYTE_MODE_SINGLE_ENDED
						   | XPT2046_CONTROL_BYTE_MODE_12_BIT
						   | XPT2046_CONTROL_BYTE_ADC_SINGLE_CHANNEL_AUX
						   | XPT2046_CONTROL_BYTE_POWER_INTERNAL_REFERENCE_ON
						   | XPT2046_CONTROL_BYTE_POWER_FULL_POWER);
			device->txBuffer[10] = device->txBuffer[8];
			device->txBuffer[12] = device->txBuffer[8];
			device->txBuffer[14] = device->txBuffer[8];


			device->txBuffer[16] = (0x80
						   | XPT2046_CONTROL_BYTE_MODE_SINGLE_ENDED
						   | XPT2046_CONTROL_BYTE_MODE_12_BIT
						   | XPT2046_CONTROL_BYTE_ADC_SINGLE_CHANNEL_TEMP
						   | XPT2046_CONTROL_BYTE_POWER_INTERNAL_REFERENCE_ON
						   | XPT2046_CONTROL_BYTE_POWER_FULL_POWER);
			device->txBuffer[18] = device->txBuffer[16];
			device->txBuffer[20] = device->txBuffer[16];
			device->txBuffer[22] = device->txBuffer[16];


			device->txBuffer[24] = (0x80
						   | XPT2046_CONTROL_BYTE_MODE_DIFFERENTIAL_REFERENCE
						   | XPT2046_CONTROL_BYTE_MODE_12_BIT
						   | XPT2046_CONTROL_BYTE_ADC_DIFF_Y_POSITION
						   //| XPT2046_CONTROL_BYTE_POWER_INTERNAL_REFERENCE_ON);
						   | XPT2046_CONTROL_BYTE_POWER_FULL_POWER);
			device->txBuffer[26] = device->txBuffer[24];
			device->txBuffer[28] = device->txBuffer[24];
			device->txBuffer[30] = device->txBuffer[24];


			device->txBuffer[32] = (0x80
						   | XPT2046_CONTROL_BYTE_MODE_DIFFERENTIAL_REFERENCE
						   | XPT2046_CONTROL_BYTE_MODE_12_BIT
						   | XPT2046_CONTROL_BYTE_ADC_DIFF_X_POSITION
						   //| XPT2046_CONTROL_BYTE_POWER_INTERNAL_REFERENCE_ON
						   | XPT2046_CONTROL_BYTE_POWER_FULL_POWER);
			device->txBuffer[34] = device->txBuffer[32];
			device->txBuffer[36] = device->txBuffer[32];
			device->txBuffer[38] = device->txBuffer[32];


			device->txBuffer[40] = (0x80
						   | XPT2046_CONTROL_BYTE_MODE_DIFFERENTIAL_REFERENCE
						   | XPT2046_CONTROL_BYTE_MODE_12_BIT
						   | XPT2046_CONTROL_BYTE_ADC_DIFF_Z1_POSITION
						   //| XPT2046_CONTROL_BYTE_POWER_INTERNAL_REFERENCE_ON
						   | XPT2046_CONTROL_BYTE_POWER_FULL_POWER);
			device->txBuffer[42] = device->txBuffer[40];
			device->txBuffer[44] = device->txBuffer[40];
			device->txBuffer[46] = device->txBuffer[40];


			device->txBuffer[48] = (0x80
						   | XPT2046_CONTROL_BYTE_MODE_DIFFERENTIAL_REFERENCE
						   | XPT2046_CONTROL_BYTE_MODE_12_BIT
						   | XPT2046_CONTROL_BYTE_ADC_DIFF_Z2_POSITION);
			device->txBuffer[50] = device->txBuffer[48];
			device->txBuffer[52] = device->txBuffer[48];
			device->txBuffer[54] = device->txBuffer[48];

			if (device->settings->spi->operations.writeData != NULL) {
				device->settings->spi->operations.writeData(device, device->txBuffer, XPT2046_DATA_TRANSFER_BUFFER_SIZE);
			}
    	}
    }
}

static void receiveData(void* device, uint8_t *buffer, uint8_t size) {

	XPT2046Device_t* devicePointer = (XPT2046Device_t*)device;

	if(devicePointer->isInitialCommandSent == 1) {
		if (devicePointer->isReadStarted) {
			if (size == XPT2046_DATA_TRANSFER_BUFFER_SIZE) {

				uint32_t vBatSum = ((buffer[1] & 0x7F) << 5 | buffer[2] >> 3) +
				((buffer[3] & 0x7F) << 5 | buffer[4] >> 3) +
						((buffer[5] & 0x7F) << 5 | buffer[6] >> 3) +
						((buffer[7] & 0x7F) << 5 | buffer[8] >> 3);

				uint32_t auxSum = ((buffer[9] & 0x7F) << 5 | buffer[10] >> 3) +
				((buffer[11] & 0x7F) << 5 | buffer[12] >> 3) +
						((buffer[13] & 0x7F) << 5 | buffer[14] >> 3) +
						((buffer[15] & 0x7F) << 5 | buffer[16] >> 3);

				uint32_t tempSum = ((buffer[17] & 0x7F) << 5 | buffer[18] >> 3) +
				((buffer[19] & 0x7F) << 5 | buffer[20] >> 3) +
						((buffer[21] & 0x7F) << 5 | buffer[22] >> 3) +
						((buffer[23] & 0x7F) << 5 | buffer[24] >> 3);

				uint32_t yPositionSum = ((buffer[25] & 0x7F) << 5 | buffer[26] >> 3) +
				((buffer[27] & 0x7F) << 5 | buffer[28] >> 3) +
						((buffer[29] & 0x7F) << 5 | buffer[30] >> 3) +
						((buffer[31] & 0x7F) << 5 | buffer[32] >> 3);

				uint32_t xPositionSum = ((buffer[33] & 0x7F) << 5 | buffer[34] >> 3) +
				((buffer[35] & 0x7F) << 5 | buffer[36] >> 3) +
						((buffer[37] & 0x7F) << 5 | buffer[38] >> 3) +
						((buffer[39] & 0x7F) << 5 | buffer[40] >> 3);

				uint32_t z1PositionSum = ((buffer[41] & 0x7F) << 5 | buffer[42] >> 3) +
				((buffer[43] & 0x7F) << 5 | buffer[44] >> 3) +
						((buffer[45] & 0x7F) << 5 | buffer[46] >> 3) +
						((buffer[47] & 0x7F) << 5 | buffer[48] >> 3);

				uint32_t z2PositionSum = ((buffer[49] & 0x7F) << 5 | buffer[50] >> 3) +
				((buffer[51] & 0x7F) << 5 | buffer[52] >> 3) +
						((buffer[53] & 0x7F) << 5 | buffer[54] >> 3) +
						((buffer[55] & 0x7F) << 5 | buffer[56] >> 3);

				uint16_t rawXPosition = xPositionSum/XPT2046_AVERAGE_MEASURE_COUNT;
				uint16_t rawYPosition = yPositionSum/XPT2046_AVERAGE_MEASURE_COUNT;

				uint32_t calculatedXPosition = 0;
				uint32_t calculatedYPosition = 0;

				if(rawXPosition > XPT2046_X_POSITION_VALUE_OFFSET_MAX) {
					calculatedXPosition = XPT2046_SCREEN_WIDTH;
				} else if(rawXPosition > XPT2046_X_POSITION_VALUE_OFFSET_MIN) {
					calculatedXPosition = (rawXPosition - XPT2046_X_POSITION_VALUE_OFFSET_MIN) *
							XPT2046_SCREEN_WIDTH / (XPT2046_X_POSITION_VALUE_OFFSET_MAX - XPT2046_X_POSITION_VALUE_OFFSET_MIN);
				}

				if(rawYPosition > XPT2046_Y_POSITION_VALUE_OFFSET_MAX) {
					calculatedYPosition = XPT2046_SCREEN_WIDTH;
				} else if(rawYPosition > XPT2046_Y_POSITION_VALUE_OFFSET_MIN) {
					calculatedYPosition = (rawYPosition - XPT2046_Y_POSITION_VALUE_OFFSET_MIN) *
											XPT2046_SCREEN_HEIGHT / (XPT2046_Y_POSITION_VALUE_OFFSET_MAX - XPT2046_Y_POSITION_VALUE_OFFSET_MIN);
				}

#if XPT2046_SCREEN_ORIENTATION_MIRROR_X_AXIS == 1
			calculatedXPosition = XPT2046_SCREEN_WIDTH - calculatedXPosition;
#else

#endif

#if XPT2046_SCREEN_ORIENTATION_MIRROR_Y_AXIS == 1
			calculatedYPosition = XPT2046_SCREEN_HEIGHT - calculatedYPosition;
#else

#endif
			devicePointer->currentTouchEvent.batteryVoltage = vBatSum/XPT2046_AVERAGE_MEASURE_COUNT;
			devicePointer->currentTouchEvent.auxValue = auxSum/XPT2046_AVERAGE_MEASURE_COUNT;
			devicePointer->currentTouchEvent.temperature = tempSum/XPT2046_AVERAGE_MEASURE_COUNT;
			devicePointer->currentTouchEvent.yPosition = (uint16_t)calculatedYPosition;
			devicePointer->currentTouchEvent.xPosition = (uint16_t)calculatedXPosition;
			devicePointer->currentTouchEvent.z1Position = z1PositionSum/XPT2046_AVERAGE_MEASURE_COUNT;
			devicePointer->currentTouchEvent.z2Position = z2PositionSum/XPT2046_AVERAGE_MEASURE_COUNT;
				if(devicePointer->settings->readPenStateFunction()) {
					devicePointer->settings->touchEventCallback(&(devicePointer->currentTouchEvent));
				}
			}
		}
	} else {
		// Skip first initialization command
		devicePointer->isInitialCommandSent = 1;
	}


	if (devicePointer->settings->spi->chipDisable != NULL) {
		devicePointer->settings->spi->chipDisable();
	}

	devicePointer->settings->enablePenInterruptFunction();
	devicePointer->isReadStarted = 0;
}


void xpt2046Initialize(XPT2046Device_t* device) {
    device->settings->spi->operations.receiveData = receiveData;
    device->isInitialCommandSent = 0;
    device->isReadStarted = 0;
    device->lastTouchPenInterruptTicks = 0;

    for(uint8_t i = 0; i < XPT2046_DATA_TRANSFER_BUFFER_SIZE; i++) {
    	device->txBuffer[i] = 0;
    }

    readValue(device);
}

void xpt2046DeInitialize(XPT2046Device_t* device) {

}

void xpt2046PenInterruptTopHalfHandler(XPT2046Device_t* device) {

}

void xpt2046PenInterruptBottomHalfHandler(XPT2046Device_t* device) {
    if (!device->isReadStarted) {
        readValue(device);
    }
}

void xpt2046Tick(XPT2046Device_t* device) {
    if (device->settings->spi != NULL) {
    	if (!device->isReadStarted) {
			if(device->settings->readPenStateFunction()) {
				readValue(device);
			}
    	}
    }
}
