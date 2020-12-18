/*
 * file_utils.c
 *
 *  Created on: Nov 23, 2020
 *      Author: andrey
 */

#include "utils/file_utils.h"
#include "time/time.h"
#include <stdio.h>

/*
 * This function always generates 12 bytes length filename.
 */
void generateFilename(char* name) {
	RTCDate currentTime = getCurrentRTCDate();
	sprintf(name, "%02u%02u%02u%02u%02u%02u.BCSV",
			currentTime.years,
			currentTime.months,
			currentTime.days,
			currentTime.hours,
			currentTime.minutes,
			currentTime.seconds);
}

void generateTestFilename(char* name) {
	RTCDate currentTime = getCurrentRTCDate();
	sprintf(name, "%02u%02u%02u%02u%02u%02u.TEST",
			currentTime.years,
			currentTime.months,
			currentTime.days,
			currentTime.hours,
			currentTime.minutes,
			currentTime.seconds);
}

