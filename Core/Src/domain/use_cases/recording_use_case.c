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


USE_SPECIAL_RAM_REGION
uint16_t ecgDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE] = {0};
uint16_t ecgDataBufferIndex = 0;
uint8_t ecgDataBufferNumberIndex = 0;
uint8_t isBufferReady = BUFFER_IS_READY;

USE_SPECIAL_RAM_REGION
int16_t earEcgDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE] = {0};
uint16_t earEcgDataBufferIndex = 0;
uint8_t earEcgDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t fingerPPGRedDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE] = {0};
uint16_t fingerPPGRedDataBufferIndex = 0;
uint8_t fingerPPGRedDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t fingerPPGIRDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE] = {0};
uint16_t fingerPPGIRDataBufferIndex = 0;
uint8_t fingerPPGIRDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t leftEarPPGGreenDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE] = {0};
uint16_t leftEarPPGreenDataBufferIndex = 0;
uint8_t leftEarPPGGreenDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t leftEarPPGRedDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE] = {0};
uint16_t leftEarPPRedDataBufferIndex = 0;
uint8_t leftEarPPGRedDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t leftEarPPGIRDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE] = {0};
uint16_t leftEarPPIRDataBufferIndex = 0;
uint8_t leftEarPPGIRDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t rightEarPPGGreenDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE] = {0};
uint16_t rightEarPPGreenDataBufferIndex = 0;
uint8_t rightEarPPGGreenDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t rightEarPPGRedDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE] = {0};
uint16_t rightEarPPRedDataBufferIndex = 0;
uint8_t rightEarPPGRedDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t rightEarPPGIRDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE] = {0};
uint16_t rightEarPPIRDataBufferIndex = 0;
uint8_t rightEarPPGIRDataBufferNumberIndex = 0;

//USE_SPECIAL_RAM_REGION
char writingBuffer[WRITING_BUFFER_SIZE];

uint8_t	currentRecordingState = RECORDING_STATE_DEFAULT;
uint8_t	currentConnectingState = CONNECTING_STATE_DEFAULT;

static SemaphoreHandle_t* doubleBufferBinarySemaphore = NULL;

static uint8_t writingDataBufferNumberIndex = 0;

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
	ecgDataBuffer[ecgDataBufferNumberIndex][ecgDataBufferIndex] = sample;
	if(ecgDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		ecgDataBufferIndex++;
	} else {
		ecgDataBufferIndex = 0;
		if(ecgDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			ecgDataBufferNumberIndex++;
			//writingDataBufferNumberIndex = 0;
		} else {
			ecgDataBufferNumberIndex = 0;
			//writingDataBufferNumberIndex++;
			//notifyDoubleBufferEvent();
		}
	}

}

void storeSampleECGEar(uint16_t sample) {
	earEcgDataBuffer[earEcgDataBufferNumberIndex][earEcgDataBufferIndex] = sample;
	if(earEcgDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		earEcgDataBufferIndex++;
	} else {
		earEcgDataBufferIndex = 0;
		if(earEcgDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			earEcgDataBufferNumberIndex++;
			writingDataBufferNumberIndex++;
		} else {
			earEcgDataBufferNumberIndex = 0;
			writingDataBufferNumberIndex = 0;

		}
		notifyDoubleBufferEvent();
	}
}

void storeSamplePPGFingerRed(uint16_t sample) {
	fingerPPGRedDataBuffer[fingerPPGRedDataBufferNumberIndex][fingerPPGRedDataBufferIndex] = sample;
	if(fingerPPGRedDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		fingerPPGRedDataBufferIndex++;
	} else {
		fingerPPGRedDataBufferIndex = 0;
		if(fingerPPGRedDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			fingerPPGRedDataBufferNumberIndex++;
		} else {
			fingerPPGRedDataBufferNumberIndex = 0;
		}
	}
}

void storeSamplePPGFingerIR(uint16_t sample) {
	fingerPPGIRDataBuffer[fingerPPGIRDataBufferNumberIndex][fingerPPGIRDataBufferIndex] = sample;
	if(fingerPPGIRDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		fingerPPGIRDataBufferIndex++;
	} else {
		fingerPPGIRDataBufferIndex = 0;
		if(fingerPPGIRDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			fingerPPGIRDataBufferNumberIndex++;
		} else {
			fingerPPGIRDataBufferNumberIndex = 0;
		}
	}
}

void storeSamplePPGEarGreenLeft(uint16_t sample) {
	leftEarPPGGreenDataBuffer[leftEarPPGGreenDataBufferNumberIndex][leftEarPPGreenDataBufferIndex] = sample;
	if(leftEarPPGreenDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		leftEarPPGreenDataBufferIndex++;
	} else {
		leftEarPPGreenDataBufferIndex = 0;
		if(leftEarPPGGreenDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			leftEarPPGGreenDataBufferNumberIndex++;
		} else {
			leftEarPPGGreenDataBufferNumberIndex = 0;
		}
	}
}

void storeSamplePPGEarRedLeft(uint16_t sample) {
	leftEarPPGRedDataBuffer[leftEarPPGRedDataBufferNumberIndex][leftEarPPRedDataBufferIndex] = sample;
	if(leftEarPPRedDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		leftEarPPRedDataBufferIndex++;
	} else {
		leftEarPPRedDataBufferIndex = 0;
		if(leftEarPPGRedDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			leftEarPPGRedDataBufferNumberIndex++;
		} else {
			leftEarPPGRedDataBufferNumberIndex = 0;
		}
	}
}

void storeSamplePPGEarIRLeft(uint16_t sample) {
	leftEarPPGIRDataBuffer[leftEarPPGIRDataBufferNumberIndex][leftEarPPIRDataBufferIndex] = sample;
	if(leftEarPPIRDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		leftEarPPIRDataBufferIndex++;
	} else {
		leftEarPPIRDataBufferIndex = 0;
		if(leftEarPPGIRDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			leftEarPPGIRDataBufferNumberIndex++;
		} else {
			leftEarPPGIRDataBufferNumberIndex = 0;
		}
	}

}

void storeSamplePPGEarGreenRight(uint16_t sample) {
	rightEarPPGGreenDataBuffer[rightEarPPGGreenDataBufferNumberIndex][rightEarPPGreenDataBufferIndex] = sample;
	if(rightEarPPGreenDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		rightEarPPGreenDataBufferIndex++;
	} else {
		rightEarPPGreenDataBufferIndex = 0;
		if(rightEarPPGGreenDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			rightEarPPGGreenDataBufferNumberIndex++;
		} else {
			rightEarPPGGreenDataBufferNumberIndex = 0;
		}
	}
}

void storeSamplePPGEarRedRight(uint16_t sample) {
	rightEarPPGRedDataBuffer[rightEarPPGRedDataBufferNumberIndex][rightEarPPRedDataBufferIndex] = sample;
	if(rightEarPPRedDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		rightEarPPRedDataBufferIndex++;
	} else {
		rightEarPPRedDataBufferIndex = 0;
		if(rightEarPPGRedDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			rightEarPPGRedDataBufferNumberIndex++;
		} else {
			rightEarPPGRedDataBufferNumberIndex = 0;
		}
	}
}

void storeSamplePPGEarIRRight(uint16_t sample) {
	rightEarPPGIRDataBuffer[rightEarPPGIRDataBufferNumberIndex][rightEarPPIRDataBufferIndex] = sample;
	if(rightEarPPIRDataBufferIndex < ECG_BUFFER_SIZE - 1) {
		rightEarPPIRDataBufferIndex++;
	} else {
		rightEarPPIRDataBufferIndex = 0;
		if(rightEarPPGIRDataBufferNumberIndex < ECG_BUFFER_NUMBER - 1) {
			rightEarPPGIRDataBufferNumberIndex++;
			//writingDataBufferNumberIndex = 0;
		} else {
			rightEarPPGIRDataBufferNumberIndex = 0;
			//writingDataBufferNumberIndex++;
		}
	}
	//notifyDoubleBufferEvent();
}

uint32_t combineWritingBuffer(char** bufferPointer) {
	*bufferPointer = writingBuffer;
	for(int i = 0; i < ECG_BUFFER_SIZE; i++) {
		uint16_t newIndex = i * NUMBER_OF_BYTES_PER_SAMPLE;
		writingBuffer[newIndex] = ecgDataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 1] = ecgDataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 2] = earEcgDataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 3] = earEcgDataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 4] = fingerPPGRedDataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 5] = fingerPPGRedDataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 6] = fingerPPGIRDataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 7] = fingerPPGIRDataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 8] = leftEarPPGGreenDataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 9] = leftEarPPGGreenDataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 10] = leftEarPPGRedDataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 11] = leftEarPPGRedDataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 12] = leftEarPPGIRDataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 13] = leftEarPPGIRDataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 14] = rightEarPPGGreenDataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 15] = rightEarPPGGreenDataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 16] = rightEarPPGRedDataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 17] = rightEarPPGRedDataBuffer[writingDataBufferNumberIndex][i];
		writingBuffer[newIndex + 18] = rightEarPPGIRDataBuffer[writingDataBufferNumberIndex][i] >> 8;
		writingBuffer[newIndex + 19] = rightEarPPGIRDataBuffer[writingDataBufferNumberIndex][i];
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
	ecgDataBufferIndex = 0;
	earEcgDataBufferNumberIndex = 0;

	earEcgDataBufferIndex = 0;
	earEcgDataBufferNumberIndex = 0;

	fingerPPGRedDataBufferIndex = 0;
	fingerPPGRedDataBufferNumberIndex = 0;

	fingerPPGIRDataBufferIndex = 0;
	fingerPPGIRDataBufferNumberIndex = 0;

	leftEarPPGreenDataBufferIndex = 0;
	leftEarPPGreenDataBufferNumberIndex = 0;

	leftEarPPRedDataBufferIndex = 0;
	leftEarPPRedDataBufferNumberIndex = 0;

	leftEarPPIRDataBufferIndex = 0;
	leftEarPPIRDataBufferNumberIndex = 0;

	rightEarPPGreenDataBufferIndex = 0;
	rightEarPPGreenDataBufferNumberIndex = 0;

	rightEarPPRedDataBufferIndex = 0;
	rightEarPPRedDataBufferNumberIndex = 0;

	rightEarPPIRDataBufferIndex = 0;
	rightEarPPIRDataBufferNumberIndex = 0;
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
