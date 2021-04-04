/*
 * recording_use_case.c
 *
 *  Created on: Dec 16, 2020
 *      Author: andrey
 */

#include "domain/use_cases/recording_use_case.h"
#include "app_touchgfx.h"

#define BUFFER_IS_READY			1
#define BUFFER_IS_NOT_READY		0

typedef struct {
	uint16_t dataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
	uint16_t bufferIndex;
	uint8_t bufferNumberIndex;
	uint8_t isBufferReady;
} BiosignalDataStruct;

USE_SPECIAL_RAM_REGION
BiosignalDataStruct ecgData = {.dataBuffer = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct earEcgData = {.dataBuffer = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct fingerPPGRedData = {.dataBuffer = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct fingerPPGIRData = {.dataBuffer = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct leftEarPPGGreenData = {.dataBuffer = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct leftEarPPGRedData = {.dataBuffer = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct leftEarPPGIRData = {.dataBuffer = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct rightEarPPGGreenData = {.dataBuffer = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct rightEarPPGRedData = {.dataBuffer = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct rightEarPPGIRData = {.dataBuffer = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

//USE_SPECIAL_RAM_REGION
//char writingBuffer[ECG_BUFFER_NUMBER][WRITING_BUFFER_SIZE];
char writingBuffer[WRITING_BUFFER_SIZE];

uint8_t	currentRecordingState = RECORDING_STATE_DEFAULT;
uint8_t	currentConnectingState = CONNECTING_STATE_DEFAULT;

static SemaphoreHandle_t* doubleBufferBinarySemaphore = NULL;

static uint8_t writingDataBufferNumberIndex = 0;

static initDataStructs() {
	ecgData.bufferIndex = 0;
	ecgData.bufferNumberIndex = 0;
	ecgData.isBufferReady = BUFFER_IS_NOT_READY;

	earEcgData.bufferIndex = 0;
	earEcgData.bufferNumberIndex = 0;
	earEcgData.isBufferReady = BUFFER_IS_NOT_READY;

	fingerPPGRedData.bufferIndex = 0;
	fingerPPGRedData.bufferNumberIndex = 0;
	fingerPPGRedData.isBufferReady = BUFFER_IS_NOT_READY;

	fingerPPGIRData.bufferIndex = 0;
	fingerPPGIRData.bufferNumberIndex = 0;
	fingerPPGIRData.isBufferReady = BUFFER_IS_NOT_READY;

	leftEarPPGGreenData.bufferIndex = 0;
	leftEarPPGGreenData.bufferNumberIndex = 0;
	leftEarPPGGreenData.isBufferReady = BUFFER_IS_NOT_READY;

	leftEarPPGRedData.bufferIndex = 0;
	leftEarPPGRedData.bufferNumberIndex = 0;
	leftEarPPGRedData.isBufferReady = BUFFER_IS_NOT_READY;

	leftEarPPGIRData.bufferIndex = 0;
	leftEarPPGIRData.bufferNumberIndex = 0;
	leftEarPPGIRData.isBufferReady = BUFFER_IS_NOT_READY;

	rightEarPPGGreenData.bufferIndex = 0;
	rightEarPPGGreenData.bufferNumberIndex = 0;
	rightEarPPGGreenData.isBufferReady = BUFFER_IS_NOT_READY;

	rightEarPPGRedData.bufferIndex = 0;
	rightEarPPGRedData.bufferNumberIndex = 0;
	rightEarPPGRedData.isBufferReady = BUFFER_IS_NOT_READY;

	rightEarPPGIRData.bufferIndex = 0;
	rightEarPPGIRData.bufferNumberIndex = 0;
	rightEarPPGIRData.isBufferReady = BUFFER_IS_NOT_READY;
}

void setDoubleBufferSemaphore(SemaphoreHandle_t* semaphore) {
	doubleBufferBinarySemaphore = semaphore;
}

static void notifyDoubleBufferEvent() {
	if(doubleBufferBinarySemaphore != NULL) {
		xSemaphoreGiveFromISR(doubleBufferBinarySemaphore, pdTRUE);
		portYIELD_FROM_ISR(pdTRUE);
	}
}

void storeSampleECG(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		ecgData.dataBuffer[ecgData.bufferNumberIndex][ecgData.bufferIndex] = sample;
		if(ecgData.bufferIndex < ECG_BUFFER_SIZE - 1) {
			ecgData.bufferIndex++;
		} else {
			ecgData.bufferIndex = 0;
			if(ecgData.bufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
				ecgData.bufferNumberIndex++;
				//writingDataBufferNumberIndex = 0;
			} else {
				ecgData.bufferNumberIndex = 0;
				//writingDataBufferNumberIndex++;
				//notifyDoubleBufferEvent();
			}
			ecgData.isBufferReady = BUFFER_IS_READY;
		}
	}
}

void storeSampleECGEar(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		earEcgData.dataBuffer[earEcgData.bufferNumberIndex][earEcgData.bufferIndex] = sample;
		if(earEcgData.bufferIndex < ECG_BUFFER_SIZE - 1) {
			earEcgData.bufferIndex++;
		} else {
			earEcgData.bufferIndex = 0;
			if(earEcgData.bufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
				earEcgData.bufferNumberIndex++;
				writingDataBufferNumberIndex++;
			} else {
				earEcgData.bufferNumberIndex = 0;
				writingDataBufferNumberIndex = 0;

			}
			earEcgData.isBufferReady = BUFFER_IS_READY;
			notifyDoubleBufferEvent();
		}
	}
}

void storeSamplePPGFingerRed(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		fingerPPGRedData.dataBuffer[fingerPPGRedData.bufferNumberIndex][fingerPPGRedData.bufferIndex] = sample;
		if(fingerPPGRedData.bufferIndex < ECG_BUFFER_SIZE - 1) {
			fingerPPGRedData.bufferIndex++;
		} else {
			fingerPPGRedData.bufferIndex = 0;
			if(fingerPPGRedData.bufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
				fingerPPGRedData.bufferNumberIndex++;
			} else {
				fingerPPGRedData.bufferNumberIndex = 0;
			}
			fingerPPGRedData.isBufferReady = BUFFER_IS_READY;
		}
	}
}

void storeSamplePPGFingerIR(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		fingerPPGIRData.dataBuffer[fingerPPGIRData.bufferNumberIndex][fingerPPGIRData.bufferIndex] = sample;
		if(fingerPPGIRData.bufferIndex < ECG_BUFFER_SIZE - 1) {
			fingerPPGIRData.bufferIndex++;
		} else {
			fingerPPGIRData.bufferIndex = 0;
			if(fingerPPGIRData.bufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
				fingerPPGIRData.bufferNumberIndex++;
			} else {
				fingerPPGIRData.bufferNumberIndex = 0;
			}
			fingerPPGIRData.isBufferReady = BUFFER_IS_READY;
		}
		}
}

void storeSamplePPGEarGreenLeft(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		leftEarPPGGreenData.dataBuffer[leftEarPPGGreenData.bufferNumberIndex][leftEarPPGGreenData.bufferIndex] = sample;
		if(leftEarPPGGreenData.bufferIndex < ECG_BUFFER_SIZE - 1) {
			leftEarPPGGreenData.bufferIndex++;
		} else {
			leftEarPPGGreenData.bufferIndex = 0;
			if(leftEarPPGGreenData.bufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
				leftEarPPGGreenData.bufferNumberIndex++;
			} else {
				leftEarPPGGreenData.bufferNumberIndex = 0;
			}
			leftEarPPGGreenData.isBufferReady = BUFFER_IS_READY;
		}
	}
}

void storeSamplePPGEarRedLeft(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		leftEarPPGRedData.dataBuffer[leftEarPPGRedData.bufferNumberIndex][leftEarPPGRedData.bufferIndex] = sample;
		if(leftEarPPGRedData.bufferIndex < ECG_BUFFER_SIZE - 1) {
			leftEarPPGRedData.bufferIndex++;
		} else {
			leftEarPPGRedData.bufferIndex = 0;
			if(leftEarPPGRedData.bufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
				leftEarPPGRedData.bufferNumberIndex++;
			} else {
				leftEarPPGRedData.bufferNumberIndex = 0;
			}
			leftEarPPGRedData.isBufferReady = BUFFER_IS_READY;
		}
	}
}

void storeSamplePPGEarIRLeft(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		leftEarPPGIRData.dataBuffer[leftEarPPGIRData.bufferNumberIndex][leftEarPPGIRData.bufferIndex] = sample;
		if(leftEarPPGIRData.bufferIndex < ECG_BUFFER_SIZE - 1) {
			leftEarPPGIRData.bufferIndex++;
		} else {
			leftEarPPGIRData.bufferIndex = 0;
			if(leftEarPPGIRData.bufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
				leftEarPPGIRData.bufferNumberIndex++;
			} else {
				leftEarPPGIRData.bufferNumberIndex = 0;
			}
			leftEarPPGIRData.isBufferReady = BUFFER_IS_READY;
		}
	}

}

void storeSamplePPGEarGreenRight(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		rightEarPPGGreenData.dataBuffer[rightEarPPGGreenData.bufferNumberIndex][rightEarPPGGreenData.bufferIndex] = sample;
		if(rightEarPPGGreenData.bufferIndex < ECG_BUFFER_SIZE - 1) {
			rightEarPPGGreenData.bufferIndex++;
		} else {
			rightEarPPGGreenData.bufferIndex = 0;
			if(rightEarPPGGreenData.bufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
				rightEarPPGGreenData.bufferNumberIndex++;
			} else {
				rightEarPPGGreenData.bufferNumberIndex = 0;
			}
			rightEarPPGGreenData.isBufferReady = BUFFER_IS_READY;
		}
	}
}

void storeSamplePPGEarRedRight(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		rightEarPPGRedData.dataBuffer[rightEarPPGRedData.bufferNumberIndex][rightEarPPGRedData.bufferIndex] = sample;
		if(rightEarPPGRedData.bufferIndex < ECG_BUFFER_SIZE - 1) {
			rightEarPPGRedData.bufferIndex++;
		} else {
			rightEarPPGRedData.bufferIndex = 0;
			if(rightEarPPGRedData.bufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
				rightEarPPGRedData.bufferNumberIndex++;
			} else {
				rightEarPPGRedData.bufferNumberIndex = 0;
			}
			rightEarPPGRedData.isBufferReady = BUFFER_IS_READY;
		}
	}
}

void storeSamplePPGEarIRRight(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		rightEarPPGIRData.dataBuffer[rightEarPPGIRData.bufferNumberIndex][rightEarPPGIRData.bufferIndex] = sample;
		if(rightEarPPGIRData.bufferIndex < ECG_BUFFER_SIZE - 1) {
			rightEarPPGIRData.bufferIndex++;
		} else {
			rightEarPPGIRData.bufferIndex = 0;
			if(rightEarPPGIRData.bufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
				rightEarPPGIRData.bufferNumberIndex++;
				//writingDataBufferNumberIndex = 0;
			} else {
				rightEarPPGIRData.bufferNumberIndex = 0;
				//writingDataBufferNumberIndex++;
			}
			rightEarPPGIRData.isBufferReady = BUFFER_IS_READY;
		}
	}
	//notifyDoubleBufferEvent();
}

uint32_t combineWritingBuffer(char** bufferPointer) {
	*bufferPointer = writingBuffer;
	for(int i = 0; i < ECG_BUFFER_SIZE; i++) {
		uint16_t newIndex = i * NUMBER_OF_BYTES_PER_SAMPLE;
		writingBuffer[newIndex] = ecgData.dataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 1] = ecgData.dataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 2] = earEcgData.dataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 3] = earEcgData.dataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 4] = fingerPPGRedData.dataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 5] = fingerPPGRedData.dataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 6] = fingerPPGIRData.dataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 7] = fingerPPGIRData.dataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 8] = leftEarPPGGreenData.dataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 9] = leftEarPPGGreenData.dataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 10] = leftEarPPGRedData.dataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 11] = leftEarPPGRedData.dataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 12] = leftEarPPGIRData.dataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 13] = leftEarPPGIRData.dataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 14] = rightEarPPGGreenData.dataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 15] = rightEarPPGGreenData.dataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 16] = rightEarPPGRedData.dataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 17] = rightEarPPGRedData.dataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 18] = rightEarPPGIRData.dataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 19] = rightEarPPGIRData.dataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 20] = END_OF_SAMPLE;
	}
	return WRITING_BUFFER_SIZE;
}

static void changeRecordingState(uint8_t newState, const char* error) {
	currentRecordingState = newState;
	notify_main_state_changed(error);
}

static void changeConnectingState(uint8_t newState) {
	currentConnectingState = newState;
	notify_main_state_changed(NULL);
}

uint8_t getRecordingStateUseCase() {
	return currentRecordingState;
}

void startRecordingUseCase() {
	initDataStructs();
	changeRecordingState(RECORDING_STATE_STARTED, NULL);
}

void stopRecordingUseCase() {
	changeRecordingState(RECORDING_STATE_STOPPED, NULL);
}

uint8_t getDevicesConnectedStateUseCase() {
	return currentConnectingState;
}

void connectAllDevicesUseCase() {
	changeConnectingState(CONNECTING_STATE_CONNECTED);
}

void disconnectAllDevicesUseCase() {
	stopRecordingUseCase();
	changeConnectingState(CONNECTING_STATE_DISCONNECTED);
}

void errorRecordingUseCase(const char* error) {
	changeRecordingState(RECORDING_STATE_ERROR, error);
}
