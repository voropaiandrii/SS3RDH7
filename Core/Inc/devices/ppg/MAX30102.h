/*
 * MAX30102.h
 *
 *  Created on: Oct 1, 2020
 *      Author: andrey
 */
#ifndef INC_DEVICES_PPG_MAX30102_H_
#define INC_DEVICES_PPG_MAX30102_H_

#include "stdint.h"
#include "devices/io_device_types.h"
#include "FreeRTOS.h"
#include "queue.h"

#define MAX30102_TXRX_BUFFER_LENGTH									22U
#define MAX30102_PART_ID											0x15

/*
 * Registers addresses section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#define MAX30102_REGISTER_ADDRESS_INTERRUPT_STATUS_1 				0x00
#define MAX30102_REGISTER_ADDRESS_INTERRUPT_STATUS_2 				0x01

#define MAX30102_REGISTER_ADDRESS_INTERRUPT_ENABLE_1 				0x02
#define MAX30102_REGISTER_ADDRESS_INTERRUPT_ENABLE_2 				0x03

#define MAX30102_REGISTER_ADDRESS_FIFO_WRITE_POINTER 				0x04
#define MAX30102_REGISTER_ADDRESS_OVERFLOW_COUNTER 					0x05
#define MAX30102_REGISTER_ADDRESS_FIFO_READ_POINTER 				0x06
#define MAX30102_REGISTER_ADDRESS_FIFO_DATA 						0x07

#define MAX30102_REGISTER_ADDRESS_FIFO_CONFIG						0x08
#define MAX30102_REGISTER_ADDRESS_MODE_CONFIG						0x09

#define MAX30102_REGISTER_ADDRESS_SPO2_CONFIG						0x0A
#define MAX30102_REGISTER_ADDRESS_LED1_PULSE						0x0C
#define MAX30102_REGISTER_ADDRESS_LED2_PULSE						0x0D
#define MAX30102_REGISTER_ADDRESS_PROXIMITY_LED_PULSE				0x10

#define MAX30102_REGISTER_ADDRESS_MULTI_LED_CONTROL1				0x11
#define MAX30102_REGISTER_ADDRESS_MULTI_LED_CONTROL2				0x12

#define MAX30102_REGISTER_ADDRESS_DIE_TEMP_INTEGER					0x1F
#define MAX30102_REGISTER_ADDRESS_DIE_TEMP_FRACTION					0x20
#define MAX30102_REGISTER_ADDRESS_DIE_TEMP_CONFIG					0x21

#define MAX30102_REGISTER_ADDRESS_PROXIMITY_INT_THESHOLD			0x30
#define MAX30102_REGISTER_ADDRESS_REVISION_ID						0xFE
#define MAX30102_REGISTER_ADDRESS_PART_ID			 				0xFF
/*
 * Registers addresses section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


/*
 * Interrupt Status 1 register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_INTERRUPT_STATUS_1_A_FULL 				0x80
/*
 * A_FULL: FIFO Almost Full Flag
 * In SpO2 and HR modes, this interrupt triggers when the FIFO write pointer has a certain number of free spaces remaining.
 * The trigger number can be set by the FIFO_A_FULL[3:0] register.
 * The interrupt is cleared by reading the Interrupt Status 1 register (0x00).
 */

#define MAX30102_REGISTER_INTERRUPT_STATUS_1_PPG_RDY 				0x40
/*
 * PPG_RDY: New FIFO Data Ready
 * In SpO2 and HR modes, this interrupt triggers when there is a new sample in the data FIFO.
 * The interrupt is cleared by reading the Interrupt Status 1 register (0x00), or by reading the FIFO_DATA register.
 */

#define MAX30102_REGISTER_INTERRUPT_STATUS_1_ALC_OVF 				0x20
/*
 * ALC_OVF: Ambient Light Cancellation Overflow
 * This interrupt triggers when the ambient light cancellation function of the SpO2/HR photodiode has reached its maximum limit,
 * and therefore, ambient light is affecting the output of the ADC. The interrupt is cleared by reading the Interrupt Status 1 register (0x00).
 */

#define MAX30102_REGISTER_INTERRUPT_STATUS_1_PROX_INT 				0x10
/*
 * ALC_OVF: Ambient Light Cancellation Overflow
 * This interrupt triggers when the ambient light cancellation function of the SpO2/HR photodiode has reached its maximum limit,
 * and therefore, ambient light is affecting the output of the ADC. The interrupt is cleared by reading the Interrupt Status 1 register (0x00).
 */

#define MAX30102_REGISTER_INTERRUPT_STATUS_1_PWR_RDY 				0x01
/*
 * PWR_RDY: Power Ready Flag
 * On power-up or after a brownout condition,
 * when the supply voltage VDD transitions from below the undervoltage lockout (UVLO) voltage to above the UVLO voltage,
 * a power-ready interrupt is triggered to signal that the module is powered-up and ready to collect data.
 */

/*
 * Interrupt Status 1 register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Interrupt Status 2 register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#define MAX30102_REGISTER_INTERRUPT_STATUS_2_DIE_TEMP_RDY 			0x02
/*
 * DIE_TEMP_RDY: Internal Temperature Ready Flag
 * When an internal die temperature conversion is finished,
 * this interrupt is triggered so the processor can read the temperature data registers.
 * The interrupt is cleared by reading either the Interrupt Status 2 register (0x01) or the TFRAC register (0x20).
 */

/*
 * Interrupt Status 2 register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Interrupt Enable 1 register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_INTERRUPT_ENABLE_1_A_FULL_EN				0x80
#define MAX30102_REGISTER_INTERRUPT_ENABLE_1_PPG_RDY_EN				0x40
#define MAX30102_REGISTER_INTERRUPT_ENABLE_1_ALC_OVF_EN				0x20
#define MAX30102_REGISTER_INTERRUPT_ENABLE_1_PROX_INT_EN			0x10
/*
 * Interrupt Enable 1 register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Interrupt Enable 2 register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_INTERRUPT_ENABLE_2_DIE_TEMP_ RDY_EN		0x02
/*
 * Interrupt Enable 2 register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * FIFO Write Pointer register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The FIFO Write Pointer points to the location where the MAX30102 writes the next sample.
 * This pointer advances for each sample pushed on to the FIFO.
 * It can also be changed through the I2C interface when MODE[2:0] is 010, 011, or 111.
 */
#define MAX30102_REGISTER_FIFO_WRITE_POINTER_MASK					0x14
/*
 * FIFO Write Pointer register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Overflow Counter register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * When the FIFO is full, samples are not pushed on to the FIFO, samples are lost.
 * OVF_COUNTER counts the number of samples lost. It saturates at 0xF.
 * When a complete sample is “popped” (i.e., removal of old FIFO data and shifting the samples down) from the FIFO (when the read pointer advances),
 * OVF_COUNTER is reset to zero.
 */
#define MAX30102_REGISTER_OVERFLOW_COUNTER_MASK						0x14
/*
 * Overflow Counter register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * FIFO Read Pointer register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The FIFO Read Pointer points to the location from where the processor gets the next sample from the FIFO through the I2C interface.
 * This advances each time a sample is popped from the FIFO.
 * The processor can also write to this pointer after reading the samples to allow rereading samples from the FIFO if there is a data communication error.
 */
#define MAX30102_REGISTER_FIFO_READ_POINTER_MASK					0x14
/*
 * FIFO Read Pointer register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * FIFO Data Register Pointer section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
 * FIFO Data Register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * FIFO Configuration section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_FIFO_CONFIG_SMP_AVE_1						0x00
#define MAX30102_REGISTER_FIFO_CONFIG_SMP_AVE_2						0x20
#define MAX30102_REGISTER_FIFO_CONFIG_SMP_AVE_4						0x40
#define MAX30102_REGISTER_FIFO_CONFIG_SMP_AVE_8						0x60
#define MAX30102_REGISTER_FIFO_CONFIG_SMP_AVE_16					0x80
#define MAX30102_REGISTER_FIFO_CONFIG_SMP_AVE_32					0xE0
/*
 * Bits 7:5: Sample Averaging (SMP_AVE)
 * To reduce the amount of data throughput, adjacent samples (in each individual channel) can be averaged and decimated on the chip by setting this register.
 */

#define MAX30102_REGISTER_FIFO_CONFIG_ROLLOVER_EN					0x10
/*
 * Bit 4: FIFO Rolls on Full (FIFO_ROLLOVER_EN)
 * This bit controls the behavior of the FIFO when the FIFO becomes completely filled with data.
 * If FIFO_ROLLOVER_EN is set (1), the FIFO address rolls over to zero and the FIFO continues to fill with new data.
 * If the bit is not set (0), then the FIFO is not updated until FIFO_DATA is read or the WRITE/READ pointer positions are changed.
 */

#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_MASK				0x0F
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS				0U
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_32	0U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_31	1U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_30	2U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_29	3U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_28	4U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_27	5U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_26	6U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_25	7U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_24	8U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_23	9U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_22	10U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_21	11U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_20 12U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_19	13U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_18	14U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
#define MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_SAMPLES_UNREAD_17	15U << MAX30102_REGISTER_FIFO_CONFIG_FIFO_A_FULL_POS
/*
 * FIFO Almost Full Value (FIFO_A_FULL)
 * This register sets the number of data samples (3 bytes/sample) remaining in the FIFO when the interrupt is issued.
 * For example, if this field is set to 0x0, the interrupt is issued when there is 0 data samples remaining in the FIFO (all 32 FIFO words have unread data).
 * Furthermore, if this field is set to 0xF, the interrupt is issued when 15 data samples are remaining in the FIFO (17 FIFO data samples have unread data).
 */
/*
 * FIFO Configuration section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Mode Configuration section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_MODE_CONFIG_SHDN							0x80
/*
 * Bit 7: Shutdown Control (SHDN)
 * The part can be put into a power-save mode by setting this bit to one.
 * While in power-save mode, all registers retain their values, and write/read operations function as normal.
 * All interrupts are cleared to zero in this mode.
 */
#define MAX30102_REGISTER_MODE_CONFIG_RESET							0x40
/*
 * Bit 6: Reset Control (RESET)
 * When the RESET bit is set to one, all configuration, threshold, and data registers are reset to their power-on-state through a power-on reset.
 * The RESET bit is cleared automatically back to zero after the reset sequence is completed.
 * Note: Setting the RESET bit does not trigger a PWR_RDY interrupt event
 */
#define MAX30102_REGISTER_MODE_CONFIG_MODE_HEART_RATE				0x02
#define MAX30102_REGISTER_MODE_CONFIG_MODE_SPO2						0x03
#define MAX30102_REGISTER_MODE_CONFIG_MODE_MULTI_LED				0x07
/*
 * Bits 2:0: Mode Control
 * These bits set the operating state of the MAX30102.
 * Changing modes does not change any other setting, nor does it erase any previously stored data inside the data registers.
 */

/*
 * Mode Configuration section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * SpO2 Configuration section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_ADC_RGE_POS				5U

#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_ADC_RGE_2048				0U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_ADC_RGE_POS
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_ADC_RGE_4096				1U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_ADC_RGE_POS
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_ADC_RGE_8192				2U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_ADC_RGE_POS
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_ADC_RGE_16384			3U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_ADC_RGE_POS
/*
 * Bits 6:5: SpO2 ADC Range Control
 * This register sets the SpO2 sensor ADC’s full-scale range
 */

#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_POS					2U

#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_50					0U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_POS
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_100					1U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_POS
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_200					2U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_POS
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_400					3U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_POS
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_800					4U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_POS
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_1000					5U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_POS
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_1600					6U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_POS
#define MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_3200					7U << MAX30102_REGISTER_SPO2_CONFIG_SPO2_SR_POS
/*
 * Bits 4:2: SpO2 Sample Rate Control
 * These bits define the effective sampling rate with one sample consisting of one IR pulse/conversion and one Red pulse/ conversion.
 * The sample rate and pulse width are related in that the sample rate sets an upper bound on the pulse width time.
 * If the user selects a sample rate that is too high for the selected LED_PW setting,
 * the highest possible sample rate is programmed instead into the register.
 */

#define MAX30102_REGISTER_SPO2_CONFIG_LED_PW_POS					0U

#define MAX30102_REGISTER_SPO2_CONFIG_LED_PW_15bit					0U << MAX30102_REGISTER_SPO2_CONFIG_LED_PW_POS
#define MAX30102_REGISTER_SPO2_CONFIG_LED_PW_16bit					1U << MAX30102_REGISTER_SPO2_CONFIG_LED_PW_POS
#define MAX30102_REGISTER_SPO2_CONFIG_LED_PW_17bit					2U << MAX30102_REGISTER_SPO2_CONFIG_LED_PW_POS
#define MAX30102_REGISTER_SPO2_CONFIG_LED_PW_18bit					3U << MAX30102_REGISTER_SPO2_CONFIG_LED_PW_POS
/*
 * SpO2 Configuration section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * LED Pulse Amplitude section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_0uA					0x00
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_200uA					0x01
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_400uA					0x02
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_600uA					0x03
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_800uA					0x04
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_1000uA				0x05
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_1200uA				0x06
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_1400uA				0x07
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_1600uA				0x08
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_1900uA				0x09
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_2000uA				0x0A
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_2200uA				0x0B
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_2400uA				0x0C
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_2600uA				0x0D
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_2800uA				0x0E

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_3100uA				0x0F
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_3300uA				0x10
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_3500uA				0x11
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_3700uA				0x12
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_3900uA				0x13
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_4100uA				0x14
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_4300uA				0x15
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_4500uA				0x16
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_4700uA				0x17
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_4900uA				0x18
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_5100uA				0x19
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_5300uA				0x1A
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_5500uA				0x1B
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_5700uA				0x1C
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_5900uA				0x1D
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_6100uA				0x1E
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_6400uA				0x1F

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_6600uA				0x20
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_6800uA				0x21
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_7000uA				0x22
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_7200uA				0x23
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_7400uA				0x24
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_7600uA				0x25
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_7800uA				0x26
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_8000uA				0x27
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_8200uA				0x28
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_8400uA				0x29
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_8600uA				0x2A
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_8800uA				0x2B
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_9000uA				0x2C
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_9200uA				0x2D
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_9400uA				0x2E
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_9500uA				0x2F

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_9600uA				0x30
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_9800uA				0x31
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_10000uA				0x32
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_10200uA				0x33
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_10400uA				0x34
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_10600uA				0x35
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_10800uA				0x36
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_11000uA				0x37
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_11200uA				0x38
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_11400uA				0x39
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_11600uA				0x3A
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_11800uA				0x3B
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_12000uA				0x3C
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_12200uA				0x3D
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_12400uA				0x3E
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_12500uA				0x3F

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_12700uA				0x40
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_12900uA				0x41
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_13100uA				0x42
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_13300uA				0x43
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_13500uA				0x44
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_13700uA				0x45
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_13900uA				0x46
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_14100uA				0x47
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_14300uA				0x48
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_14500uA				0x49
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_14700uA				0x4A
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_14900uA				0x4B
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_15100uA				0x4C
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_15300uA				0x4D
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_15500uA				0x4E
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_15700uA				0x4F

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_15900uA				0x50
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_16100uA				0x51
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_16300uA				0x52
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_16500uA				0x53
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_16700uA				0x54
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_16900uA				0x55
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_17100uA				0x56
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_17300uA				0x57
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_17500uA				0x58
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_17700uA				0x59
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_17900uA				0x5A
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_18100uA				0x5B
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_18300uA				0x5C
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_18500uA				0x5D
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_18700uA				0x5E
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_18900uA				0x5F

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_19100uA				0x60
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_19300uA				0x61
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_19500uA				0x62
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_19700uA				0x63
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_19900uA				0x64
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_20100uA				0x65
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_20300uA				0x66
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_20500uA				0x67
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_20700uA				0x68
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_20900uA				0x69
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_21100uA				0x6A
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_21300uA				0x6B
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_21500uA				0x6C
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_21700uA				0x6D
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_21900uA				0x6E
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_22100uA				0x6F

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_22300uA				0x70
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_22500uA				0x71
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_22700uA				0x72
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_22900uA				0x73
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_23100uA				0x74
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_23300uA				0x75
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_23500uA				0x76
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_23700uA				0x77
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_23900uA				0x78
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_24100uA				0x79
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_24300uA				0x7A
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_24500uA				0x7B
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_24700uA				0x7C
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_24900uA				0x7D
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_25100uA				0x7E
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_25400uA				0x7F

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_25600uA				0x80
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_25800uA				0x81
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_26000uA				0x82
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_26200uA				0x83
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_26400uA				0x84
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_26600uA				0x85
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_26800uA				0x86
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_27000uA				0x87
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_27200uA				0x88
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_27400uA				0x89
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_27600uA				0x8A
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_27800uA				0x8B
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_28000uA				0x8C
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_28200uA				0x8D
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_28400uA				0x8E
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_28600uA				0x8F

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_28800uA				0x90
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_29000uA				0x91
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_29200uA				0x92
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_29400uA				0x93
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_29600uA				0x94
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_29800uA				0x95
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_30000uA				0x96
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_30200uA				0x97
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_30400uA				0x98
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_30600uA				0x99
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_30800uA				0x9A
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_31000uA				0x9B
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_31200uA				0x9C
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_31400uA				0x9D
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_31600uA				0x9E
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_31800uA				0x9F

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_32000uA				0xA0
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_32200uA				0xA1
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_32400uA				0xA2
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_32600uA				0xA3
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_32800uA				0xA4
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_33000uA				0xA5
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_33200uA				0xA6
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_33400uA				0xA7
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_33600uA				0xA8
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_33800uA				0xA9
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_34000uA				0xAA
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_34200uA				0xAB
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_34400uA				0xAC
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_34600uA				0xAD
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_34800uA				0xAE
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_35000uA				0xAF

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_35200uA				0xB0
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_35400uA				0xB1
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_35600uA				0xB2
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_35800uA				0xB3
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_36000uA				0xB4
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_36200uA				0xB5
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_36400uA				0xB6
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_36600uA				0xB7
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_36800uA				0xB8
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_37000uA				0xB9
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_37200uA				0xBA
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_37400uA				0xBB
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_37600uA				0xBC
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_37800uA				0xBD
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_38000uA				0xBE
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_38200uA				0xBF

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_38400uA				0xC0
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_38600uA				0xC1
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_38800uA				0xC2
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_39000uA				0xC3
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_39200uA				0xC4
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_39400uA				0xC5
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_39600uA				0xC6
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_39800uA				0xC7
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_40000uA				0xC8
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_40200uA				0xC9
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_40400uA				0xCA
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_40600uA				0xCB
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_40800uA				0xCC
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_41000uA				0xCD
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_41200uA				0xCE
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_41400uA				0xCF

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_41600uA				0xD0
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_41800uA				0xD1
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_42000uA				0xD2
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_42200uA				0xD3
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_42400uA				0xD4
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_42600uA				0xD5
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_42800uA				0xD6
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_43000uA				0xD7
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_43200uA				0xD8
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_43400uA				0xD9
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_43600uA				0xDA
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_43800uA				0xDB
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_44000uA				0xDC
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_44200uA				0xDD
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_44400uA				0xDE
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_44600uA				0xDF

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_44800uA				0xE0
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_45000uA				0xE1
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_45200uA				0xE2
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_45400uA				0xE3
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_45600uA				0xE4
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_45800uA				0xE5
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_46000uA				0xE6
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_46200uA				0xE7
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_46400uA				0xE8
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_46600uA				0xE9
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_46800uA				0xEA
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_47000uA				0xEB
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_47200uA				0xEC
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_47400uA				0xED
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_47600uA				0xEE
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_47800uA				0xEF

#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_48000uA				0xF0
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_48200uA				0xF1
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_48400uA				0xF2
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_48600uA				0xF3
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_48800uA				0xF4
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_49000uA				0xF5
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_49200uA				0xF6
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_49400uA				0xF7
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_49600uA				0xF8
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_49800uA				0xF9
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_50000uA				0xFA
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_50001uA				0xFB
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_50002uA				0xFC
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_50003uA				0xFD
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_50004uA				0xFE
#define MAX30102_REGISTER_ADDRESS_LED_CURRENT_50005uA				0xFF

/*
 * LED Pulse Amplitude section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Proximity Mode LED Pulse Amplitude section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
 * Proximity Mode LED Pulse Amplitude section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Multi-LED Mode Control Register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#define MAX30102_REGISTER_MULTI_LED_CONTROL1_SLOT1_POS				0U
#define MAX30102_REGISTER_MULTI_LED_CONTROL1_SLOT1_MASK

#define MAX30102_REGISTER_MULTI_LED_CONTROL1_SLOT2_POS				4U
#define MAX30102_REGISTER_MULTI_LED_CONTROL1_SLOT2_MASK

#define MAX30102_REGISTER_MULTI_LED_CONTROL2_SLOT3_POS				0U
#define MAX30102_REGISTER_MULTI_LED_CONTROL2_SLOT3_MASK

#define MAX30102_REGISTER_MULTI_LED_CONTROL2_SLOT4_POS				4U
#define MAX30102_REGISTER_MULTI_LED_CONTROL2_SLOT4_MASK

#define MAX30102_REGISTER_MULTI_LED_CONTROL_SLOT_DISABLED			0U
#define MAX30102_REGISTER_MULTI_LED_CONTROL_SLOT_LED1_PA_RED		1U
#define MAX30102_REGISTER_MULTI_LED_CONTROL_SLOT_LED2_PA_IR			2U
#define MAX30102_REGISTER_MULTI_LED_CONTROL_SLOT_LED1_PILOT_PA_RED	5U
#define MAX30102_REGISTER_MULTI_LED_CONTROL_SLOT_LED2_PILOT_PA_IR	6U


/*
 * Multi-LED Mode Control Register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Die Temp Integer section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_DIE_TEMP_INTEGER_MASK						0xFF

/*
 * Die Temp Integer  section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Die Temp Fraction section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_DIE_TEMP_FRACTION_MASK					0x0F

/*
 * Die Temp Fraction  section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Die Temperature Config section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_DIE_TEMP_CONFIG_TEMP_EN					0x01
/*
 * Temperature Enable (TEMP_EN)
 * This is a self-clearing bit which, when set, initiates a single temperature reading from the temperature sensor.
 * This bit clears automatically back to zero at the conclusion of the temperature reading when the bit is set to one in IR or SpO2 mode.
 */
/*
 * Die Temperature Config  section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Proximity Interrupt Threshold section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_REGISTER_PROXIMITY_INT_THESHOLD_MASK				0xFF
/*
 * Proximity Interrupt Threshold section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Revision ID section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
 * Revision ID section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Part ID section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
 * Part ID section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30102_STATE_IDLE													0U
#define MAX30102_STATE_START												1U
#define MAX30102_STATE_READING_REVISION_ID									2U
#define MAX30102_STATE_READING_PART_ID										3U
#define MAX30102_STATE_RESET												4U
#define MAX30102_STATE_POWER_READY											5U
#define MAX30102_STATE_WRITE_CONFIGURATIONS									6U
#define MAX30102_STATE_READ_CONFIGURATIONS									7U
#define MAX30102_STATE_READ_PPG												8U
#define MAX30102_STATE_READ_FIFO_READ_POINTER								9U
#define MAX30102_STATE_READ_FIFO_WRITE_POINTER								10U
#define MAX30102_STATE_READ_FIFO_DATA										11U
#define MAX30102_STATE_READ_INTERRUPT_STATUS1								12U
#define MAX30102_STATE_STOPPED												13U


#define MAX30102_ERROR_NONE													0U
#define MAX30102_ERROR_INVALID_PART_ID										1U
#define MAX30102_ERROR_COMMUNICATION_FAILED									2U

#define MAX30102_MODE_SPO2													0U
#define MAX30102_MODE_HR													1U
#define MAX30102_MODE_MULTI													2U

#define MAX30102_I2C_STATE_COMPLETED										0U
#define MAX30102_I2C_STATE_START											1U
#define MAX30102_I2C_STATE_SEND_ADDRESS										2U
#define MAX30102_I2C_STATE_SEND_DATA										3U
#define MAX30102_I2C_STATE_RECEIVE_DATA										4U

#define MAX30102_I2C_ACTION_WRITE_DATA										0U
#define MAX30102_I2C_ACTION_READ_DATA										1U

typedef struct {
	int32_t redSample;
	int32_t irSample;
} MAX30102PPGData_t;

typedef uint8_t (*MAX30102ReadInterruptStateFunction)();
typedef void (*MAX30102EnableInterruptFunction)();
typedef void (*MAX30102DisableInterruptFunction)();
typedef void (*MAX30102PPGDataCallback_t)(MAX30102PPGData_t* ppgEvent);

typedef struct {
	I2C_t* i2c;
	MAX30102ReadInterruptStateFunction readInterruptStateFunction;
	MAX30102EnableInterruptFunction enableInterruptFunction;
	MAX30102DisableInterruptFunction disableInterruptFunction;
	MAX30102PPGDataCallback_t ppgDataCallback;
} MAX30102Settings_t;

typedef struct {
	MAX30102Settings_t* settings;
	uint8_t currentState;
	uint8_t isReadStarted;
	uint8_t isConfigurated;
	uint8_t txBuffer[MAX30102_TXRX_BUFFER_LENGTH];
	uint8_t rxBuffer[MAX30102_TXRX_BUFFER_LENGTH];
	uint8_t error;
	uint8_t revID;
	uint8_t partId;
	uint8_t i2cState;
	uint8_t i2cAction;
	uint8_t i2cDataSize;
	uint8_t fifoWritePointer;
	uint8_t fifoReadPointer;
	uint8_t fifoAvailableDataCount;
	QueueHandle_t txSerialQueue;
	QueueHandle_t rxSerialQueue;
	MAX30102PPGData_t currentDataSample;
} MAX30102Device_t;

void max30102Init(MAX30102Device_t* device);
void max30102DeInit(MAX30102Device_t* device);
void max30102Start(MAX30102Device_t* device);
void max30102Stop(MAX30102Device_t* device);
void max30102Tick(MAX30102Device_t* device);

void max30102InterruptTopHalfHandler(MAX30102Device_t* device);
void max30102InterruptBottomHalfHandler(MAX30102Device_t* device);


#endif /* INC_DEVICES_PPG_MAX30102_H_ */
