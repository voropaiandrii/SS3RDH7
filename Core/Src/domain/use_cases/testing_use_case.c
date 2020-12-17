/*
 * testing_use_case.c
 *
 *  Created on: Dec 17, 2020
 *      Author: andrey
 */

#include "domain/use_cases/testing_use_case.h"
#include "app_touchgfx.h"

static uint8_t testingState = TESTING_STATE_DEFAULT;


void startTestingUseCase() {
	testingState = TESTING_STATE_STARTED;
	printForTesting("HELLO!\n");

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

