/*
 * recording_use_case.c
 *
 *  Created on: Dec 16, 2020
 *      Author: andrey
 */

#include "domain/use_cases/recording_use_case.h"

USE_SPECIAL_RAM_REGION
uint16_t ecgDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t ecgDataBufferIndex = 0;
uint8_t ecgDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t earEcgDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t earEcgDataBufferIndex = 0;
uint8_t earEcgDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t fingerPPGRedDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t fingerPPGRedDataBufferIndex = 0;
uint8_t fingerPPGRedDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t fingerPPGIRDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t fingerPPGIRDataBufferIndex = 0;
uint8_t fingerPPGIRDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t leftEarPPGGreenDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t leftEarPPGreenDataBufferIndex = 0;
uint8_t leftEarPPGGreenDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t leftEarPPGRedDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t leftEarPPRedDataBufferIndex = 0;
uint8_t leftEarPPGRedDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t leftEarPPGIRDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t leftEarPPIRDataBufferIndex = 0;
uint8_t leftEarPPGIRDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t rightEarPPGGreenDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t rightEarPPGreenDataBufferIndex = 0;
uint8_t rightEarPPGGreenDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t rightEarPPGRedDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t rightEarPPRedDataBufferIndex = 0;
uint8_t rightEarPPGRedDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
int16_t rightEarPPGIRDataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
uint16_t rightEarPPIRDataBufferIndex = 0;
uint8_t rightEarPPGIRDataBufferNumberIndex = 0;

USE_SPECIAL_RAM_REGION
char writingBuffer[WRITING_BUFFER_SIZE];

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
		} else {
			ecgDataBufferNumberIndex = 0;
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
			writingDataBufferNumberIndex = 0;
		} else {
			earEcgDataBufferNumberIndex = 0;
			writingDataBufferNumberIndex++;
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
		} else {
			rightEarPPGIRDataBufferNumberIndex = 0;
		}
	}
}

uint32_t combineWritingBuffer(char** bufferPointer) {
	*bufferPointer = writingBuffer;
	for(int i = 0; i < ECG_BUFFER_SIZE; i++) {
		uint16_t newIndex = i * BYTES_PER_SAMPLE * NUMBER_OF_CHANNELS;
		writingBuffer[newIndex] = ecgDataBuffer[0][i] >> 8;
		writingBuffer[newIndex + 1] = ecgDataBuffer[0][i];
		writingBuffer[newIndex + 2] = earEcgDataBuffer[0][i] >> 8;
		writingBuffer[newIndex + 3] = earEcgDataBuffer[0][i];
		writingBuffer[newIndex + 4] = fingerPPGRedDataBuffer[0][i] >> 8;
		writingBuffer[newIndex + 5] = fingerPPGRedDataBuffer[0][i];
		writingBuffer[newIndex + 6] = fingerPPGIRDataBuffer[0][i] >> 8;
		writingBuffer[newIndex + 7] = fingerPPGIRDataBuffer[0][i];
		writingBuffer[newIndex + 8] = leftEarPPGGreenDataBuffer[0][i] >> 8;
		writingBuffer[newIndex + 9] = leftEarPPGGreenDataBuffer[0][i];
		writingBuffer[newIndex + 10] = leftEarPPGRedDataBuffer[0][i] >> 8;
		writingBuffer[newIndex + 11] = leftEarPPGRedDataBuffer[0][i];
		writingBuffer[newIndex + 12] = leftEarPPGIRDataBuffer[0][i] >> 8;
		writingBuffer[newIndex + 13] = leftEarPPGIRDataBuffer[0][i];
		writingBuffer[newIndex + 14] = rightEarPPGGreenDataBuffer[0][i] >> 8;
		writingBuffer[newIndex + 15] = rightEarPPGGreenDataBuffer[0][i];
		writingBuffer[newIndex + 16] = rightEarPPGRedDataBuffer[0][i] >> 8;
		writingBuffer[newIndex + 17] = rightEarPPGRedDataBuffer[0][i];
		writingBuffer[newIndex + 18] = rightEarPPGIRDataBuffer[0][i] >> 8;
		writingBuffer[newIndex + 19] = rightEarPPGIRDataBuffer[0][i];
		writingBuffer[newIndex + 20] = '\n';
	}
	return WRITING_BUFFER_SIZE;
}

