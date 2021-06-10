/*
 * recording_use_case.c
 *
 *  Created on: Dec 16, 2020
 *      Author: andrey
 */

#include <string.h>
#include "domain/use_cases/recording_use_case.h"
#include "app_touchgfx.h"
#include "stdio.h"
#include "utils/file_utils.h"

#define BUFFER_IS_READY			1
#define BUFFER_IS_NOT_READY		0

char* filename = NULL;

typedef struct {
	//uint16_t dataBuffer[ECG_BUFFER_NUMBER][ECG_BUFFER_SIZE];
	QueueHandle_t dataQueue;
	StaticQueue_t dataStaticQueue;
	uint8_t dataQueueStorageArea[ECG_BUFFER_NUMBER * ECG_BUFFER_SIZE * 2];
	uint16_t bufferIndex;
	uint8_t bufferNumberIndex;
	uint8_t isBufferReady;
	uint16_t prebildCallsWhenBufferReady;

} BiosignalDataStruct;

/*
USE_SPECIAL_RAM_REGION
BiosignalDataStruct ecgData = {.dataBuffer = {{0}}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct earEcgData = {.dataBuffer = {{0}}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct fingerPPGRedData = {.dataBuffer = {{0}}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct fingerPPGIRData = {.dataBuffer = {{0}}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct leftEarPPGGreenData = {.dataBuffer = {{0}}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct leftEarPPGRedData = {.dataBuffer = {{0}}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct leftEarPPGIRData = {.dataBuffer = {{0}}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct rightEarPPGGreenData = {.dataBuffer = {{0}}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct rightEarPPGRedData = {.dataBuffer = {{0}}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct rightEarPPGIRData = {.dataBuffer = {{0}}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};
*/

USE_SPECIAL_RAM_REGION
BiosignalDataStruct ecgData = {.dataQueue = NULL, .dataStaticQueue = NULL, .dataQueueStorageArea = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct earEcgData = {.dataQueue = NULL, .dataStaticQueue = NULL, .dataQueueStorageArea = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct fingerPPGRedData = {.dataQueue = NULL, .dataStaticQueue = NULL, .dataQueueStorageArea = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct fingerPPGIRData = {.dataQueue = NULL, .dataStaticQueue = NULL, .dataQueueStorageArea = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct leftEarPPGGreenData = {.dataQueue = NULL, .dataStaticQueue = NULL, .dataQueueStorageArea = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct leftEarPPGRedData = {.dataQueue = NULL, .dataStaticQueue = NULL, .dataQueueStorageArea = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct leftEarPPGIRData = {.dataQueue = NULL, .dataStaticQueue = NULL, .dataQueueStorageArea = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct rightEarPPGGreenData = {.dataQueue = NULL, .dataStaticQueue = NULL, .dataQueueStorageArea = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct rightEarPPGRedData = {.dataQueue = NULL, .dataStaticQueue = NULL, .dataQueueStorageArea = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};

USE_SPECIAL_RAM_REGION
BiosignalDataStruct rightEarPPGIRData = {.dataQueue = NULL, .dataStaticQueue = NULL, .dataQueueStorageArea = {0}, .bufferIndex = 0, .bufferNumberIndex = 0, .isBufferReady = BUFFER_IS_READY};


//USE_SPECIAL_RAM_REGION
//char writingBuffer[ECG_BUFFER_NUMBER][WRITING_BUFFER_SIZE];
char writingBuffer[WRITING_BUFFER_SIZE];

uint8_t	currentRecordingState = RECORDING_STATE_DEFAULT;
uint8_t	currentConnectingState = CONNECTING_STATE_DEFAULT;

static SemaphoreHandle_t* doubleBufferBinarySemaphore = NULL;

//static uint8_t writingDataBufferNumberIndex = 0;


static void initDataStructs() {
	ecgData.bufferIndex = 0;
	ecgData.bufferNumberIndex = 0;
	ecgData.isBufferReady = BUFFER_IS_NOT_READY;
	ecgData.prebildCallsWhenBufferReady = 0;

	earEcgData.bufferIndex = 0;
	earEcgData.bufferNumberIndex = 0;
	earEcgData.isBufferReady = BUFFER_IS_NOT_READY;
	earEcgData.prebildCallsWhenBufferReady = 0;

	fingerPPGRedData.bufferIndex = 0;
	fingerPPGRedData.bufferNumberIndex = 0;
	fingerPPGRedData.isBufferReady = BUFFER_IS_NOT_READY;
	fingerPPGRedData.prebildCallsWhenBufferReady = 0;

	fingerPPGIRData.bufferIndex = 0;
	fingerPPGIRData.bufferNumberIndex = 0;
	fingerPPGIRData.isBufferReady = BUFFER_IS_NOT_READY;
	fingerPPGIRData.prebildCallsWhenBufferReady = 0;

	leftEarPPGGreenData.bufferIndex = 0;
	leftEarPPGGreenData.bufferNumberIndex = 0;
	leftEarPPGGreenData.isBufferReady = BUFFER_IS_NOT_READY;
	leftEarPPGGreenData.prebildCallsWhenBufferReady = 0;

	leftEarPPGRedData.bufferIndex = 0;
	leftEarPPGRedData.bufferNumberIndex = 0;
	leftEarPPGRedData.isBufferReady = BUFFER_IS_NOT_READY;
	leftEarPPGRedData.prebildCallsWhenBufferReady = 0;

	leftEarPPGIRData.bufferIndex = 0;
	leftEarPPGIRData.bufferNumberIndex = 0;
	leftEarPPGIRData.isBufferReady = BUFFER_IS_NOT_READY;
	leftEarPPGIRData.prebildCallsWhenBufferReady = 0;

	rightEarPPGGreenData.bufferIndex = 0;
	rightEarPPGGreenData.bufferNumberIndex = 0;
	rightEarPPGGreenData.isBufferReady = BUFFER_IS_NOT_READY;
	rightEarPPGGreenData.prebildCallsWhenBufferReady = 0;

	rightEarPPGRedData.bufferIndex = 0;
	rightEarPPGRedData.bufferNumberIndex = 0;
	rightEarPPGRedData.isBufferReady = BUFFER_IS_NOT_READY;
	rightEarPPGRedData.prebildCallsWhenBufferReady = 0;

	rightEarPPGIRData.bufferIndex = 0;
	rightEarPPGIRData.bufferNumberIndex = 0;
	rightEarPPGIRData.isBufferReady = BUFFER_IS_NOT_READY;
	rightEarPPGIRData.prebildCallsWhenBufferReady = 0;
}

void initRecordingUseCase(char* filenamePointer) {
	filename = filenamePointer;
	initDataStructs();

	/*
	// clean volatile buffers
	for(uint32_t i = 0; i < ECG_BUFFER_NUMBER; i++) {
		for(uint32_t j = 0; j < ECG_BUFFER_SIZE; j++) {
			ecgData.dataBuffer[i][j] = 0;
			earEcgData.dataBuffer[i][j] = 0;

			fingerPPGRedData.dataBuffer[i][j] = 0;
			fingerPPGIRData.dataBuffer[i][j] = 0;

			leftEarPPGGreenData.dataBuffer[i][j] = 0;
			leftEarPPGRedData.dataBuffer[i][j] = 0;
			leftEarPPGIRData.dataBuffer[i][j] = 0;

			rightEarPPGGreenData.dataBuffer[i][j] = 0;
			rightEarPPGRedData.dataBuffer[i][j] = 0;
			rightEarPPGIRData.dataBuffer[i][j] = 0;
		}
	}
	*/

	for(uint32_t i = 0; i < ECG_BUFFER_NUMBER * ECG_BUFFER_SIZE * 2; i++) {
		ecgData.dataQueueStorageArea[i] = 0;
		earEcgData.dataQueueStorageArea[i] = 0;

		fingerPPGRedData.dataQueueStorageArea[i] = 0;
		fingerPPGIRData.dataQueueStorageArea[i] = 0;

		leftEarPPGGreenData.dataQueueStorageArea[i] = 0;
		leftEarPPGRedData.dataQueueStorageArea[i] = 0;
		leftEarPPGIRData.dataQueueStorageArea[i] = 0;

		rightEarPPGGreenData.dataQueueStorageArea[i] = 0;
		rightEarPPGRedData.dataQueueStorageArea[i] = 0;
		rightEarPPGIRData.dataQueueStorageArea[i] = 0;
	}

	memset(writingBuffer, 0, WRITING_BUFFER_SIZE);

	for(int i = 0; i < ECG_BUFFER_SIZE; i++) {
		uint16_t newIndex = i * NUMBER_OF_BYTES_PER_SAMPLE;
		writingBuffer[newIndex + 20] = END_OF_SAMPLE;
	}

	ecgData.dataQueue = xQueueCreateStatic(ECG_BUFFER_SIZE * ECG_BUFFER_NUMBER, sizeof(uint16_t), ecgData.dataQueueStorageArea, &(ecgData.dataStaticQueue));
	earEcgData.dataQueue = xQueueCreateStatic(ECG_BUFFER_SIZE * ECG_BUFFER_NUMBER, sizeof(uint16_t), earEcgData.dataQueueStorageArea, &(earEcgData.dataStaticQueue));

	fingerPPGRedData.dataQueue = xQueueCreateStatic(ECG_BUFFER_SIZE * ECG_BUFFER_NUMBER, sizeof(uint16_t), fingerPPGRedData.dataQueueStorageArea, &(fingerPPGRedData.dataStaticQueue));
	fingerPPGIRData.dataQueue = xQueueCreateStatic(ECG_BUFFER_SIZE * ECG_BUFFER_NUMBER, sizeof(uint16_t), fingerPPGIRData.dataQueueStorageArea, &(fingerPPGIRData.dataStaticQueue));

	leftEarPPGGreenData.dataQueue = xQueueCreateStatic(ECG_BUFFER_SIZE * ECG_BUFFER_NUMBER, sizeof(uint16_t), leftEarPPGGreenData.dataQueueStorageArea, &(leftEarPPGGreenData.dataStaticQueue));
	leftEarPPGRedData.dataQueue = xQueueCreateStatic(ECG_BUFFER_SIZE * ECG_BUFFER_NUMBER, sizeof(uint16_t), leftEarPPGRedData.dataQueueStorageArea, &(leftEarPPGRedData.dataStaticQueue));
	leftEarPPGIRData.dataQueue = xQueueCreateStatic(ECG_BUFFER_SIZE * ECG_BUFFER_NUMBER, sizeof(uint16_t), leftEarPPGIRData.dataQueueStorageArea, &(leftEarPPGIRData.dataStaticQueue));

	rightEarPPGGreenData.dataQueue = xQueueCreateStatic(ECG_BUFFER_SIZE * ECG_BUFFER_NUMBER, sizeof(uint16_t), rightEarPPGGreenData.dataQueueStorageArea, &(rightEarPPGGreenData.dataStaticQueue));
	rightEarPPGRedData.dataQueue = xQueueCreateStatic(ECG_BUFFER_SIZE * ECG_BUFFER_NUMBER, sizeof(uint16_t), rightEarPPGRedData.dataQueueStorageArea, &(rightEarPPGRedData.dataStaticQueue));
	rightEarPPGIRData.dataQueue = xQueueCreateStatic(ECG_BUFFER_SIZE * ECG_BUFFER_NUMBER, sizeof(uint16_t), rightEarPPGIRData.dataQueueStorageArea, &(rightEarPPGIRData.dataStaticQueue));
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
		if(xQueueSendFromISR(ecgData.dataQueue, (void *)&sample, (TickType_t)0) == pdTRUE) {
			// OK
		} else {
			// Error
			printf("sECG queue is full!\n");
		}
	}
}

void storeSampleECGEar(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		if(xQueueSendFromISR(earEcgData.dataQueue, (void *)&sample, (TickType_t)0) == pdTRUE) {
			// OK
		} else {
			// Error
			printf("eECG queue is full!\n");
		}
	}
}

void storeSamplePPGFingerRed(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		if(xQueueSendFromISR(fingerPPGRedData.dataQueue, (void *)&sample, (TickType_t)0) == pdTRUE) {
			// OK
		} else {
			// Error
		}
	}
}

void storeSamplePPGFingerIR(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		if(xQueueSendFromISR(fingerPPGIRData.dataQueue, (void *)&sample, (TickType_t)0) == pdTRUE) {
			// OK
		} else {
			// Error
		}
	}
}

void storeSamplePPGEarGreenLeft(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		if(xQueueSendFromISR(leftEarPPGGreenData.dataQueue, (void *)&sample, (TickType_t)0) == pdTRUE) {
			// OK
		} else {
			// Error
		}
	}
}

void storeSamplePPGEarRedLeft(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		if(xQueueSendFromISR(leftEarPPGRedData.dataQueue, (void *)&sample, (TickType_t)0) == pdTRUE) {
			// OK
		} else {
			// Error
		}
	}
}

void storeSamplePPGEarIRLeft(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		if(xQueueSendFromISR(leftEarPPGIRData.dataQueue, (void *)&sample, (TickType_t)0) == pdTRUE) {
			// OK
		} else {
			// Error
		}
	}

}

void storeSamplePPGEarGreenRight(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		if(xQueueSendFromISR(rightEarPPGGreenData.dataQueue, (void *)&sample, (TickType_t)0) == pdTRUE) {
			// OK
		} else {
			// Error
		}
	}
}

void storeSamplePPGEarRedRight(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		if(xQueueSendFromISR(rightEarPPGRedData.dataQueue, (void *)&sample, (TickType_t)0) == pdTRUE) {
			// OK
		} else {
			// Error
		}
	}
}

void storeSamplePPGEarIRRight(uint16_t sample) {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		if(xQueueSendFromISR(rightEarPPGIRData.dataQueue, (void *)&sample, (TickType_t)0) == pdTRUE) {
			// OK
		} else {
			// Error
		}
	}
	//notifyDoubleBufferEvent();
}

void prebuildWritingBuffer() {
	if(currentRecordingState == RECORDING_STATE_STARTED) {
		uint16_t itemValue;
		// Standard ECG
		if(ecgData.isBufferReady != BUFFER_IS_READY) {
			while(xQueueReceive(ecgData.dataQueue, &itemValue, 5) == pdTRUE) {
				writingBuffer[ecgData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE] = itemValue >> 8;
				writingBuffer[ecgData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 1] = itemValue;
				if(ecgData.bufferIndex < ECG_BUFFER_SIZE - 1) {
					ecgData.bufferIndex++;
				} else {
					ecgData.isBufferReady = BUFFER_IS_READY;
					break;
				}
			}
		} else {
			ecgData.prebildCallsWhenBufferReady++;
		}

		// Ear ECG
		if(earEcgData.isBufferReady != BUFFER_IS_READY) {
			while(xQueueReceive(earEcgData.dataQueue, &itemValue, 5) == pdTRUE) {
				writingBuffer[earEcgData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 2] = itemValue >> 8;
				writingBuffer[earEcgData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 3] = itemValue;
				if(earEcgData.bufferIndex < ECG_BUFFER_SIZE - 1) {
					earEcgData.bufferIndex++;
				} else {
					earEcgData.isBufferReady = BUFFER_IS_READY;
					break;
				}
			}
		} else {
			earEcgData.prebildCallsWhenBufferReady++;
		}
		// Finger PPG
		if(fingerPPGRedData.isBufferReady != BUFFER_IS_READY) {
			while(xQueueReceive(fingerPPGRedData.dataQueue, &itemValue, 5) == pdTRUE) {
				writingBuffer[fingerPPGRedData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 4] = itemValue >> 8;
				writingBuffer[fingerPPGRedData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 5] = itemValue;
				if(fingerPPGRedData.bufferIndex < ECG_BUFFER_SIZE - 1) {
					fingerPPGRedData.bufferIndex++;
				} else {
					fingerPPGRedData.isBufferReady = BUFFER_IS_READY;
					break;
				}
			}
		} else {
			fingerPPGRedData.prebildCallsWhenBufferReady++;
		}

		if(fingerPPGIRData.isBufferReady != BUFFER_IS_READY) {
			while(xQueueReceive(fingerPPGIRData.dataQueue, &itemValue, 5) == pdTRUE) {
				writingBuffer[fingerPPGIRData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 6] = itemValue >> 8;
				writingBuffer[fingerPPGIRData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 7] = itemValue;
				if(fingerPPGIRData.bufferIndex < ECG_BUFFER_SIZE - 1) {
					fingerPPGIRData.bufferIndex++;
				} else {
					fingerPPGIRData.isBufferReady = BUFFER_IS_READY;
					break;
				}
			}
		} else {
			fingerPPGIRData.prebildCallsWhenBufferReady++;
		}

		// Left ear PPG
		if(leftEarPPGGreenData.isBufferReady != BUFFER_IS_READY) {
			while(xQueueReceive(leftEarPPGGreenData.dataQueue, &itemValue, 5) == pdTRUE) {
				writingBuffer[leftEarPPGGreenData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 8] = itemValue >> 8;
				writingBuffer[leftEarPPGGreenData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 9] = itemValue;
				if(leftEarPPGGreenData.bufferIndex < ECG_BUFFER_SIZE - 1) {
					leftEarPPGGreenData.bufferIndex++;
				} else {
					leftEarPPGGreenData.isBufferReady = BUFFER_IS_READY;
					break;
				}
			}
		} else {
			leftEarPPGGreenData.prebildCallsWhenBufferReady++;
		}

		if(leftEarPPGRedData.isBufferReady != BUFFER_IS_READY) {
			while(xQueueReceive(leftEarPPGRedData.dataQueue, &itemValue, 5) == pdTRUE) {
				writingBuffer[leftEarPPGRedData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 10] = itemValue >> 8;
				writingBuffer[leftEarPPGRedData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 11] = itemValue;
				if(leftEarPPGRedData.bufferIndex < ECG_BUFFER_SIZE - 1) {
					leftEarPPGRedData.bufferIndex++;
				} else {
					leftEarPPGRedData.isBufferReady = BUFFER_IS_READY;
					break;
				}
			}
		} else {
			leftEarPPGRedData.prebildCallsWhenBufferReady++;
		}

		if(leftEarPPGIRData.isBufferReady != BUFFER_IS_READY) {
			while(xQueueReceive(leftEarPPGIRData.dataQueue, &itemValue, 5) == pdTRUE) {
				writingBuffer[leftEarPPGIRData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 12] = itemValue >> 8;
				writingBuffer[leftEarPPGIRData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 13] = itemValue;
				if(leftEarPPGIRData.bufferIndex < ECG_BUFFER_SIZE - 1) {
					leftEarPPGIRData.bufferIndex++;
				} else {
					leftEarPPGIRData.isBufferReady = BUFFER_IS_READY;
					break;
				}
			}
		} else {
			leftEarPPGIRData.prebildCallsWhenBufferReady++;
		}
		// Right ear PPG
		if(rightEarPPGGreenData.isBufferReady != BUFFER_IS_READY) {
			while(xQueueReceive(rightEarPPGGreenData.dataQueue, &itemValue, 5) == pdTRUE) {
				writingBuffer[rightEarPPGGreenData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 14] = itemValue >> 8;
				writingBuffer[rightEarPPGGreenData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 15] = itemValue;
				if(rightEarPPGGreenData.bufferIndex < ECG_BUFFER_SIZE - 1) {
					rightEarPPGGreenData.bufferIndex++;
				} else {
					rightEarPPGGreenData.isBufferReady = BUFFER_IS_READY;
					break;
				}
			}
		} else {
			rightEarPPGGreenData.prebildCallsWhenBufferReady++;
		}

		if(rightEarPPGRedData.isBufferReady != BUFFER_IS_READY) {
			while(xQueueReceive(rightEarPPGRedData.dataQueue, &itemValue, 5) == pdTRUE) {
				writingBuffer[rightEarPPGRedData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 16] = itemValue >> 8;
				writingBuffer[rightEarPPGRedData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 17] = itemValue;
				if(rightEarPPGRedData.bufferIndex < ECG_BUFFER_SIZE - 1) {
					rightEarPPGRedData.bufferIndex++;
				} else {
					rightEarPPGRedData.isBufferReady = BUFFER_IS_READY;
					break;
				}
			}
		} else {
			rightEarPPGRedData.prebildCallsWhenBufferReady++;
		}

		if(rightEarPPGIRData.isBufferReady != BUFFER_IS_READY) {
			while(xQueueReceive(rightEarPPGIRData.dataQueue, &itemValue, 5) == pdTRUE) {
				writingBuffer[rightEarPPGIRData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 18] = itemValue >> 8;
				writingBuffer[rightEarPPGIRData.bufferIndex * NUMBER_OF_BYTES_PER_SAMPLE + 19] = itemValue;
				if(rightEarPPGIRData.bufferIndex < ECG_BUFFER_SIZE - 1) {
					rightEarPPGIRData.bufferIndex++;
				} else {
					rightEarPPGIRData.isBufferReady = BUFFER_IS_READY;
					break;
				}
			}
		} else {
			rightEarPPGIRData.prebildCallsWhenBufferReady++;
		}

		/*
		if(ecgData.isBufferReady == BUFFER_IS_READY &&
				earEcgData.isBufferReady == BUFFER_IS_READY &&
				fingerPPGRedData.isBufferReady == BUFFER_IS_READY &&
				fingerPPGIRData.isBufferReady == BUFFER_IS_READY &&
				leftEarPPGGreenData.isBufferReady == BUFFER_IS_READY &&
				leftEarPPGIRData.isBufferReady == BUFFER_IS_READY &&
				leftEarPPGRedData.isBufferReady == BUFFER_IS_READY &&
				rightEarPPGGreenData.isBufferReady == BUFFER_IS_READY &&
				rightEarPPGRedData.isBufferReady == BUFFER_IS_READY &&
				rightEarPPGIRData.isBufferReady == BUFFER_IS_READY) {
			notifyDoubleBufferEvent();
		} else if(ecgData.isBufferReady == BUFFER_IS_READY &&
				earEcgData.isBufferReady == BUFFER_IS_READY &&
				ecgData.prebildCallsWhenBufferReady > 1 &&
				earEcgData.prebildCallsWhenBufferReady > 1){
			notifyDoubleBufferEvent();
		}
		*/

		/*
		if(ecgData.isBufferReady == BUFFER_IS_READY &&
						earEcgData.isBufferReady == BUFFER_IS_READY) {
			notifyDoubleBufferEvent();
		}
		*/


		/*
		if(ecgData.isBufferReady == BUFFER_IS_READY && ecgData.prebildCallsWhenBufferReady > 1) {
			notifyDoubleBufferEvent();
		}
		*/
		if(earEcgData.isBufferReady == BUFFER_IS_READY && ecgData.prebildCallsWhenBufferReady > 10) {
			notifyDoubleBufferEvent();
		}
	}
}

uint32_t combineWritingBuffer(char** bufferPointer) {
	*bufferPointer = writingBuffer;
	/*
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
	*/
	initDataStructs();
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
	generateFilename(filename);
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
