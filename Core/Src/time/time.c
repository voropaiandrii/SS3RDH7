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
	RTC_TimeTypeDef currentTime = {0};
	RTC_DateTypeDef currentDate = {0};
	HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
	date.hours = currentTime.Hours;
	date.minutes = currentTime.Minutes;
	date.seconds = currentTime.Seconds;
	date.years = currentDate.Year + 2000;
	date.months = currentDate.Month;
	date.days = currentDate.Date;
	return date;
}

void setCurrentRTCDate(RTCDate* date) {
	RTC_TimeTypeDef currentTime = {0};
	RTC_DateTypeDef currentDate = {0};

	currentTime.Hours = date->hours;
	currentTime.Minutes = date->minutes;
	currentTime.Seconds = date->seconds;
	currentDate.Year = date->years - 2000;
	currentDate.Month = date->months;
	currentDate.Date = date->days;

	HAL_RTC_SetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
}
