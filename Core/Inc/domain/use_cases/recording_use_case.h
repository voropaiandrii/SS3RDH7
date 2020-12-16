/*
 * recording_use_case.h
 *
 *  Created on: Dec 16, 2020
 *      Author: andrey
 */

#ifndef INC_DOMAIN_USE_CASES_RECORDING_USE_CASE_H_
#define INC_DOMAIN_USE_CASES_RECORDING_USE_CASE_H_

#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"

#define PLACE_TO_RAM_D2_REGION					volatile static __attribute__ ((section(".buffers"), used))
#define USE_SPECIAL_RAM_REGION					PLACE_TO_RAM_D2_REGION
#define ECG_BUFFER_SIZE							2000
#define ECG_BUFFER_NUMBER						2
#define BYTES_PER_SAMPLE						2
#define NUMBER_OF_CHANNELS						10
#define WRITING_BUFFER_SIZE						ECG_BUFFER_SIZE * ((BYTES_PER_SAMPLE * NUMBER_OF_CHANNELS) + 1)

void setDoubleBufferSemaphore(SemaphoreHandle_t* semaphore);
void storeSampleECG(uint16_t sample);
void storeSampleECGEar(uint16_t sample);
void storeSamplePPGFingerRed(uint16_t sample);
void storeSamplePPGFingerIR(uint16_t sample);
void storeSamplePPGEarGreenLeft(uint16_t sample);
void storeSamplePPGEarRedLeft(uint16_t sample);
void storeSamplePPGEarIRLeft(uint16_t sample);
void storeSamplePPGEarGreenRight(uint16_t sample);
void storeSamplePPGEarRedRight(uint16_t sample);
void storeSamplePPGEarIRRight(uint16_t sample);
uint32_t combineWritingBuffer(char** bufferPointer);

#endif /* INC_DOMAIN_USE_CASES_RECORDING_USE_CASE_H_ */
