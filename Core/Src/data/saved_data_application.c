/*
 * saved_data_application.c
 *
 *  Created on: Nov 23, 2020
 *      Author: andrey
 */

#include "data/saved_data_application.h"
#include "data/saved_data.h"

void setTimeSet(uint8_t isSet) {
	saveUByte(RTC_BKP_DR0, 0, isSet);
}

uint8_t isTimeSet() {
	readUByte(RTC_BKP_DR0, 0);
}

void saveDisplayBrigthness(uint8_t brigthnessPercent) {
	saveUByte(RTC_BKP_DR0, 8, brigthnessPercent);
}

uint8_t readDisplayBrigthness() {
	readUByte(RTC_BKP_DR0, 8);
}

