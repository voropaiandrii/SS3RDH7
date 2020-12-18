/*
 * testing_use_case.c
 *
 *  Created on: Dec 17, 2020
 *      Author: andrey
 */

#include "domain/use_cases/testing_use_case.h"
#include "app_touchgfx.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

static uint8_t testingState = TESTING_STATE_DEFAULT;
static char printtBuffer[100];

void startTestingUseCase() {
	testingState = TESTING_STATE_STARTED;

	printt("Hello, my name is Andrey, I'm %d y.o. I live in Ukraine\n", 27);
}

void pauseTestingUseCase() {
	testingState = TESTING_STATE_PAUSED;
}

void stopTestingUseCase() {
	testingState = TESTING_STATE_STOPPED;
}

uint8_t getTestingState() {
	return testingState;
}

void printt( const char * format, ... ) {
	va_list argptr;
	va_start(argptr, format);
	vsprintf(printtBuffer, format, argptr);
	va_end(argptr);
	printForTesting(printtBuffer);
}

