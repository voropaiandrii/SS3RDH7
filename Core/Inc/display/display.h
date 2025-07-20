/*
 * display.h
 *
 *  Created on: Apr 18, 2020
 *      Author: andrey
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "main.h"

#define LCD_DIGITAL_POWER_ENABLE	LCD_3V3_EN_GPIO_Port->BSRR = (uint32_t)LCD_3V3_EN_Pin << 16U; 	// Set bit
#define LCD_DIGITAL_POWER_DISABLE	LCD_3V3_EN_GPIO_Port->BSRR = (uint32_t)LCD_3V3_EN_Pin;			// Reset bit

#define LCD_ANALOG_POWER_ENABLE		GPIOB->BSRR = (uint32_t)LCD_PWR_EN_Pin << 16U; 	// Set bit
#define LCD_ANALOG_POWER_DISABLE	GPIOB->BSRR = (uint32_t)LCD_PWR_EN_Pin;			// Reset bit

#define LCD_SET_STANDBY_MODE		GPIOG->BSRR = (uint32_t)LCD_STBY_Pin;
#define LCD_SET_NORMAL_MODE			GPIOG->BSRR = (uint32_t)LCD_STBY_Pin << 16U;

#define LCD_SET_RESET_STATE			GPIOG->BSRR = (uint32_t)LCD_RESET_Pin;
#define LCD_SET_NORMAL_STATE		GPIOG->BSRR = (uint32_t)LCD_RESET_Pin << 16U;

#define LCD_BACKLIGHT_ENABLE		GPIOB->BSRR = (uint32_t)LCD_BL_EN_Pin << 16U;
#define LCD_BACKLIGHT_DISABLE		GPIOB->BSRR = (uint32_t)LCD_BL_EN_Pin;

void setBrightness(int brightnessPercent);

#endif /* INC_DISPLAY_H_ */
