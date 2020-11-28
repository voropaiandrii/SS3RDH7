/*
 * time.c
 *
 *  Created on: Nov 28, 2020
 *      Author: andrey
 */

#include "time/time.h"
#include "rtc.h"

RTCDate getCurrentRTCDate() {
	RTCDate date = {0};
	RTC_TimeTypeDef currentTime;
	RTC_DateTypeDef currentDate;
	HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
	date.hours = currentTime.Hours;
	date.minutes = currentTime.Minutes;
	date.seconds = currentTime.Seconds;
	date.years = currentDate.Year;
	date.months = currentDate.Month;
	date.days = currentDate.Date;
	return date;
}
