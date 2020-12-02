/*
 * time.h
 *
 *  Created on: Nov 28, 2020
 *      Author: andrey
 */

#ifndef INC_TIME_TIME_H_
#define INC_TIME_TIME_H_

#include "main.h"

typedef struct {
	uint16_t years;
	uint8_t months;
	uint8_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} RTCDate;


RTCDate getCurrentRTCDate();

#endif /* INC_TIME_TIME_H_ */
