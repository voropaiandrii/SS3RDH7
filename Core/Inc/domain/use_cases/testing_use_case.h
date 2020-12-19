/*
 * testing_use_case.h
 *
 *  Created on: Dec 17, 2020
 *      Author: andrey
 */

#ifndef INC_DOMAIN_USE_CASES_TESTING_USE_CASE_H_
#define INC_DOMAIN_USE_CASES_TESTING_USE_CASE_H_

#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"

#define TESTING_STATE_STARTED				1
#define TESTING_STATE_PAUSED				2
#define TESTING_STATE_STOPPED				3
#define TESTING_STATE_COMPLETED				4

#define TESTING_STATE_DEFAULT				TESTING_STATE_STOPPED

void startTestingUseCase();
void pauseTestingUseCase();
void stopTestingUseCase();
void sdcardTestingUseCase(uint16_t blockSize);
uint8_t getTestingState();
void printt( const char * format, ... );

#endif /* INC_DOMAIN_USE_CASES_TESTING_USE_CASE_H_ */
