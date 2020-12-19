/*
 * testing_use_case.c
 *
 *  Created on: Dec 17, 2020
 *      Author: andrey
 */

#include "domain/use_cases/testing_use_case.h"
#include "domain/use_cases/recording_use_case.h"
#include "app_touchgfx.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "fatfs.h"
#include "utils/file_utils.h"

extern SemaphoreHandle_t testBinarySemaphore;
extern osThreadId_t testTaskHandle;

#define TESTING_BUFFER_SIZE 		4096

uint8_t testingState = TESTING_STATE_DEFAULT;
char printtBuffer[100];
char currentTestFilename[FILE_UTILS_GENERATED_NAME_LENGTH_BYTES];

//USE_SPECIAL_RAM_REGION
char testWritingBuffer[TESTING_BUFFER_SIZE];
//USE_SPECIAL_RAM_REGION
char testReadingBuffer[TESTING_BUFFER_SIZE];

void startTestingUseCase() {
	testingState = TESTING_STATE_STARTED;
	printt("\nStart testing\n");
	xSemaphoreGiveFromISR(testBinarySemaphore, pdTRUE);
	portYIELD_FROM_ISR(pdTRUE);
}

void pauseTestingUseCase() {
	testingState = TESTING_STATE_PAUSED;
	printt("Pause testing\n");
	//vTaskSuspend(testTaskHandle);
}

void stopTestingUseCase() {
	testingState = TESTING_STATE_STOPPED;
	printt("Stop testing\n");
	//vTaskDelete(testTaskHandle);
}

void sdcardTestingUseCase(uint16_t blockSize) {

	if(blockSize > TESTING_BUFFER_SIZE) {
		blockSize = TESTING_BUFFER_SIZE;
	}

	FIL MyFile;
	uint32_t totalFileBytes = 0;
	uint32_t writtenBytes = 0;
	uint32_t readBytes = 0;
	uint32_t fileError = 0;
	FRESULT fresult = FR_OK;

	generateTestFilename(currentTestFilename);

	uint32_t overflowCounter = 0;
	for(uint32_t i = 0; i < blockSize; i++) {

		testWritingBuffer[i] = i - (overflowCounter * 256);
		if(overflowCounter == 255) {
			overflowCounter++;
		}
	}

	printt("Test file name: %s, blockSize: %u\n", currentTestFilename, blockSize);

	if(BSP_PlatformIsDetected() == SD_PRESENT) {
		if(retSD == 0) {
			fresult = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
			if(fresult == FR_OK) {
				printt("Mount completed\n");
				fresult = f_open(&MyFile, (char*)currentTestFilename, FA_OPEN_APPEND | FA_WRITE | FA_READ);
				if(fresult == FR_OK) {
					/*
					totalFileBytes = f_size(&MyFile);
					fresult = f_lseek(&MyFile, totalFileBytes);
					if(fresult == FR_OK) {

					} else {
						printt("Failed to seek\n");
					}
					*/

					fresult = f_write(&MyFile, testWritingBuffer, blockSize, (void *)&writtenBytes);
					if(fresult == FR_OK) {
						printt("Written bytes: %lu, buffer length: %lu\n", writtenBytes, blockSize);
						fresult = f_sync(&MyFile);
						if(fresult == FR_OK) {


							//totalFileBytes = f_size(&MyFile);
							fresult = f_lseek(&MyFile, 0);
							if(fresult == FR_OK) {

							} else {
								printt("Failed to seek\n");
							}


							fresult = f_read(&MyFile, testReadingBuffer, blockSize, (void *)&readBytes);
							if(fresult == FR_OK) {
								printt("Read bytes: %lu, buffer length: %lu\n", readBytes, blockSize);
								for(uint32_t i = 0; i < blockSize; i++) {
									if(testWritingBuffer[i] != testReadingBuffer[i]) {
										fileError++;
									}
								}
								printt("File errors: %lu\n", fileError);
							} else {
								printt("Failed to read the file\n");
							}
						} else {
							printt("Failed to sync a file\n");
						}
						f_close(&MyFile);

					} else {
						printt("Can't write a file, fresult: %d\n", fresult);
					}
					f_close(&MyFile);
				} else {
					printt("Can't open a file, fresult: %d\n", fresult);
				}
				fresult = f_mount(NULL, (TCHAR const*)SDPath, 1);
				if(fresult == FR_OK) {
					printt("Unmount completed\n");
				} else {
					printt("Failed to unmount\n");
				}
			} else {
				printt("Can't mount a SD card, fresult: %d\n", fresult);
			}
		} else {
			printt("Can't link a driver!\n");
		}

	} else {
		printt("SDcard is not present!\n");
	}

}

uint8_t getTestingState() {
	return testingState;
}

void printt( const char * format, ... ) {
	va_list argptr;
	va_start(argptr, format);
	vsprintf(printtBuffer, format, argptr);
	va_end(argptr);
	printf(printtBuffer);
	printForTesting(printtBuffer);
}

