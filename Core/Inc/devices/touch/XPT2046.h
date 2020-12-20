//
// Created by Andrey on 2020-03-14.
//

#ifndef CORE429I_BOARD_XPT2046_H
#define CORE429I_BOARD_XPT2046_H

#include <stdint.h>
#include "devices/io_device_types.h"

#define XPT2046_SCREEN_ORIENTATION_MIRROR_X_AXIS	0
#define XPT2046_SCREEN_ORIENTATION_MIRROR_Y_AXIS	1

#define XPT2046_SCREEN_WIDTH				800
#define XPT2046_SCREEN_HEIGHT				480
#define XPT2046_Y_POSITION_VALUE_OFFSET_MIN	225
#define XPT2046_Y_POSITION_VALUE_OFFSET_MAX	3875

#define XPT2046_X_POSITION_VALUE_OFFSET_MIN	135
#define XPT2046_X_POSITION_VALUE_OFFSET_MAX	4045//4000

#define XPT2046_X_SCALE_FACTOR				0.2046f
#define XPT2046_Y_SCALE_FACTOR				0.1315f

// VBAT value will be 0.25 of real VBAT voltage
#define XPT2046_ADC_CHANNEL_VBAT 0
#define XPT2046_ADC_CHANNEL_AUX 1

#define XPT2046_CONTROL_BYTE_START								0x80

// A2, A1, A0 - Input Configuration (DIN), Differential Reference Mode (SER/DFR low)
//                                                          A2  A1  A0  VBAT    AUXIN   TEMP    YN  XP  YP  Y-POSITION  X-POSITION  Z1-POSITION Z2-POSITION X-DRIVERS   Y-DRIVERS
#define XPT2046_CONTROL_BYTE_ADC_CHANNEL_0 (0 << 4) //      0   0   0                   +IN                                                                 OFF         OFF
#define XPT2046_CONTROL_BYTE_ADC_CHANNEL_1 (1 << 4) //      0   0   1                               +IN     M                                               OFF         ON
#define XPT2046_CONTROL_BYTE_ADC_CHANNEL_2 (2 << 4) //      0   1   0   +IN                                                                                 OFF         OFF
#define XPT2046_CONTROL_BYTE_ADC_CHANNEL_3 (3 << 4) //      0   1   1                               +IN                             M                       XN,ON       YP,ON
#define XPT2046_CONTROL_BYTE_ADC_CHANNEL_4 (4 << 4) //      1   0   0                           +IN                                             M           XN,ON       YP,ON
#define XPT2046_CONTROL_BYTE_ADC_CHANNEL_5 (5 << 4) //      1   0   1                                   +IN             M                                   ON          OFF
#define XPT2046_CONTROL_BYTE_ADC_CHANNEL_6 (6 << 4) //      1   1   0           +IN                                                                         OFF         OFF
#define XPT2046_CONTROL_BYTE_ADC_CHANNEL_7 (7 << 4) //      1   1   1                   +IN                                                                 OFF         OFF

#define XPT2046_CONTROL_BYTE_ADC_SINGLE_CHANNEL_VBAT           XPT2046_CONTROL_BYTE_ADC_CHANNEL_2
#define XPT2046_CONTROL_BYTE_ADC_SINGLE_CHANNEL_AUX            XPT2046_CONTROL_BYTE_ADC_CHANNEL_6
#define XPT2046_CONTROL_BYTE_ADC_SINGLE_CHANNEL_TEMP           XPT2046_CONTROL_BYTE_ADC_CHANNEL_7
/*
 * Measure Y position, XP connected to ADC IN, Y drivers is on
 */
#define XPT2046_CONTROL_BYTE_ADC_SINGLE_CHANNEL_Y_POSITION     XPT2046_CONTROL_BYTE_ADC_CHANNEL_1
/*
 * Measure X position, YP connected to ADC IN, X drivers is on
 */
#define XPT2046_CONTROL_BYTE_ADC_SINGLE_CHANNEL_X_POSITION     XPT2046_CONTROL_BYTE_ADC_CHANNEL_5


#define XPT2046_CONTROL_BYTE_ADC_DIFF_X_POSITION        XPT2046_CONTROL_BYTE_ADC_CHANNEL_5
#define XPT2046_CONTROL_BYTE_ADC_DIFF_Y_POSITION        XPT2046_CONTROL_BYTE_ADC_CHANNEL_1
#define XPT2046_CONTROL_BYTE_ADC_DIFF_Z1_POSITION       XPT2046_CONTROL_BYTE_ADC_CHANNEL_3
#define XPT2046_CONTROL_BYTE_ADC_DIFF_Z2_POSITION       XPT2046_CONTROL_BYTE_ADC_CHANNEL_4

// MODE
#define XPT2046_CONTROL_BYTE_MODE_8_BIT (1 << 3)
#define XPT2046_CONTROL_BYTE_MODE_12_BIT (0 << 3)

// SER/DFR
#define XPT2046_CONTROL_BYTE_MODE_SINGLE_ENDED (1 << 2)
#define XPT2046_CONTROL_BYTE_MODE_DIFFERENTIAL_REFERENCE (0 << 2)

// PD1 - internal 2.5V voltage reference
// Typically, the internal reference voltage is only used in the single-ended mode for battery monitoring, temperature measurement, and for using the auxiliary input.
#define XPT2046_CONTROL_BYTE_POWER_INTERNAL_REFERENCE_ON (1 << 1)

// PD0
// full-power (PD0 = 1) and auto power-down (PD0 = 0)
#define XPT2046_CONTROL_BYTE_POWER_FULL_POWER (1 << 0)
#define XPT2046_CONTROL_BYTE_POWER_AUTO_POWER_DOWN (0 << 0)

/*
 * PD1  PD0     PENIRQ      DESCRIPTION
 * 0    0       Enabled     Power-Down Between Conversions. When each conversion is finished, the converter enters a low-power mode.
 *                          At the start of the next conversion, the device instantly powers up to full power.
 *                          There is no need for additional delays to ensure full operation, and the very first conversion is valid.
 *                          The Y− switch is on when in power-down.
 * 0    1       Disabled    Reference is off and ADC is on.
 * 1    0       Enabled     Reference is on and ADC is off.
 * 1    1       Disabled    Device is always powered. Reference is on and ADC is on.
 */

#define XPT2046_STATE_READ_Y_POSITION       		0
#define XPT2046_STATE_READ_X_POSITION       		1
#define XPT2046_STATE_READ_VBAT_POSITION    		2
#define XPT2046_STATE_READ_AUX_POSITION     		3
#define XPT2046_STATE_READ_TEMP_POSITION    		4

#define XPT2046_STATE_MASK                  		0x07

#define XPT2046_AVERAGE_MEASURE_COUNT				8
#define XPT2046_DATA_TRANSFER_CONVERSATION_COUNT	7
#define XPT2046_DATA_TRANSFER_BUFFER_SIZE			(2 * XPT2046_DATA_TRANSFER_CONVERSATION_COUNT * XPT2046_AVERAGE_MEASURE_COUNT) + 1

typedef struct {
    uint16_t xPosition;
    uint16_t yPosition;
    uint16_t z1Position;
    uint16_t z2Position;
    uint16_t batteryVoltage;
    uint16_t auxValue;
    uint16_t temperature;
    uint16_t rawX;
    uint16_t rawY;
} TouchEvent_t;


typedef void (*XPT2046EnablePenInterruptFunction)();
typedef void (*XPT2046DisablePenInterruptFunction)();
typedef uint8_t (*XPT2046ReadPenStateFunction)();
typedef void (*XPT2046TouchEventCallback_t)(TouchEvent_t* touchEvent);

typedef struct {
	SPI_t* spi;
	XPT2046ReadPenStateFunction readPenStateFunction;
	XPT2046EnablePenInterruptFunction enablePenInterruptFunction;
	XPT2046DisablePenInterruptFunction disablePenInterruptFunction;
	XPT2046TouchEventCallback_t touchEventCallback;
} XPT2046Settings_t;

typedef struct {
	XPT2046Settings_t* settings;
	TouchEvent_t currentTouchEvent;
	uint8_t isInitialCommandSent;
	uint8_t isReadStarted;
	uint8_t txBuffer[XPT2046_DATA_TRANSFER_BUFFER_SIZE];
	uint8_t rxBuffer[XPT2046_DATA_TRANSFER_BUFFER_SIZE];
	uint32_t lastTouchPenInterruptTicks;
} XPT2046Device_t;

void xpt2046Initialize(XPT2046Device_t* device);
void xpt2046DeInitialize(XPT2046Device_t* device);
void xpt2046PenInterruptTopHalfHandler(XPT2046Device_t* device);
void xpt2046PenInterruptBottomHalfHandler(XPT2046Device_t* device);
void xpt2046Tick(XPT2046Device_t* device);

#endif //CORE429I_BOARD_XPT2046_H
