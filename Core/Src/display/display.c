/*
 * display.c
 *
 *  Created on: Apr 19, 2020
 *      Author: andrey
 */

#include "display/display.h"
#include "main.h"

void setBrightness(int brightnessPercent) {
	TIM12->CCR1 = (brightnessPercent * 10);
}



