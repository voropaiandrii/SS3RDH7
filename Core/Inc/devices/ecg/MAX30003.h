/*
 * MAX30003.h
 *
 *  Created on: Apr 26, 2020
 *      Author: andrey
 */

#ifndef INC_DEVICES_ECG_MAX30003_H_
#define INC_DEVICES_ECG_MAX30003_H_

#include "stdint.h"
#include "devices/io_device_types.h"

#define MAX30003_ONE_TRANSACTION_LENGTH										4
#define MAX30003_TOTAL_BUFFER_LENGTH										32
#define MAX30003_CONFIGURATION_BUFFER_LENGTH								36

#define MAX30003_COMMAND_WRITE												0x00
#define MAX30003_COMMAND_READ												0x01

/*
 * Registers addresses section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
No Operation (NO_OP) registers are read-write registers that have no internal effect on the device. If these
registers are read back, DOUT remains zero for the entire SPI transaction. Any attempt to write to these registers is
ignored without impact to internal operation.
 */
#define MAX30003_REGISTER_ADDRESS_NO_OP_1									0x00
#define MAX30003_REGISTER_ADDRESS_NO_OP_2									0x7F
/*
STATUS is a read-only register that provides a comprehensive overview of the current status of the device. The
first two bytes indicate the state of all interrupt terms (regardless of whether interrupts are enabled in registers
EN_INT (0x02) or EN_INT2 (0x03)). All interrupt terms are active high. The last byte includes detailed status
information for conditions associated with the interrupt terms.
 */
#define MAX30003_REGISTER_ADDRESS_STATUS									0x01
/*
EN_INT and EN_INT2 are read/write registers that govern the operation of the INTB output and INT2B output,
respectively. The first two bytes indicate which interrupt input terms are included in the interrupt output OR
term (ex. a one in an EN_INT register indicates that the corresponding input term is included in the INTB interrupt
output OR term). See the STATUS register for detailed descriptions of the interrupt terms. The power-on reset
state of all EN_INT terms is 0 (ignored by INT). EN_INT and EN_INT2 can also be used to mask persistent
interrupt conditions in order to perform other interrupt-driven operations until the persistent conditions are resolved.
INTB_TYPE[1:0] allows the user to select between a CMOS or an open-drain NMOS mode INTB output. If
using open-drain mode, an option for an internal 125kΩ pullup resistor is also offered.
All INTB and INT2B types are active-low (INTB low indicates the device requires servicing by the µC);
however, the open-drain mode allows the INTB line to be shared with other devices in a wired-or configuration.
In general, it is suggested that INT2B be used to support specialized/dedicated interrupts of use in specific
applications, such as the self-clearing versions of SAMP or RRINT.
 */
#define MAX30003_REGISTER_ADDRESS_EN_INT									0x02
#define MAX30003_REGISTER_ADDRESS_EN_INT_2									0x03

/*
MNGR_INT is a read/write register that manages the operation of the configurable interrupt bits in response to
ECG FIFO conditions (see the STATUS register and ECG FIFO descriptions for more details). Finally, this register
contains the configuration bits supporting the sample synchronization pulse (SAMP) and RTOR heart rate
detection interrupt (RRINT).
 */
#define MAX30003_REGISTER_ADDRESS_MNGR_INT  								0x04
/*
MNGR_DYN is a read/write register that manages the settings of any general/dynamic modes within the device.
The ECG Fast Recovery modes and thresholds are managed here. Unlike many CNFG registers, changes to
dynamic modes do not impact FIFO operations or require a SYNCH operation (though the affected circuits may
require time to settle, resulting in invalid/corrupted FIFO output voltage information during the settling interval).
 */
#define MAX30003_REGISTER_ADDRESS_MNGR_DYN  								0x05
/*
SW_RST (Software Reset) is a write-only register/command that resets the MAX30003 to its original default
conditions at the end of the SPI SW_RST transaction (i.e. the 32nd SCLK rising edge). Execution occurs only
if DIN[23:0] = 0x000000. The effect of a SW_RST is identical to power-cycling the device.
 */
#define MAX30003_REGISTER_ADDRESS_SW_RST 									0x08
/*
SYNCH (Synchronize) is a write-only register/command that begins new ECG operations and recording, beginning
on the internal MSTR clock edge following the end of the SPI SYNCH transaction (i.e. the 32nd SCLK rising
edge). Execution occurs only if DIN[23:0] = 0x000000. In addition to resetting and synchronizing the operations
of any active ECG and RtoR circuitry, SYNCH will also reset and clear the FIFO memories and the DSP filters
(to midscale), allowing the user to effectively set the “Time Zero” for the FIFO records. No configuration settings
are impacted. For best results, users should wait until the PLL has achieved lock before synchronizing if the
CNFG_GEN settings have been altered. Once the device is initially powered up, it will need to be
fully configured prior to launching recording operations. Likewise, anytime a change to CNFG_GEN or CNFG_
ECG registers are made there may be discontinuities in the ECG record and possibly changes to the size of the
time steps recorded in the FIFOs. The SYNCH command provides a means to restart operations cleanly following
any such disturbances. During multi-channel operations, if a FIFO overflow event
occurs and a portion of the record is lost, it is recommended to use the SYNCH command to recover and restart the
recording (avoiding issues with missing data in one or more channel records). Note that the two channel records
cannot be directly synchronized within the device, due to significant differences in group delays, depending on filter
selections—alignment of the records will have to be done externally.
 */
#define MAX30003_REGISTER_ADDRESS_SYNCH 									0x09
/*
FIFO_RST (FIFO Reset) is a write-only register/command that begins a new ECG recording by resetting the FIFO
memories and resuming the record with the next available ECG data. Execution occurs only if DIN[23:0]=0x000000.
Unlike the SYNCH command, the operations of any active ECG and R-to-R circuitry are not impacted by FIFO_RST,
so no settling/recovery transients apply. FIFO_RST can also be used to quickly recover from a FIFO overflow
state (recommended for single-channel use, see above).
 */
#define MAX30003_REGISTER_ADDRESS_FIFO_RST 									0x0A
/*
INFO is a read-only register that provides information about the MAX30003. The first nibble contains an alternating
bit pattern to aide in interface verification. The second nibble contains the revision ID. The third nibble includes
part ID information. The final 3 nibbles contain a serial number for Maxim internal use—note that individual
units are not given unique serial numbers, and these bits should not be used as serial numbers for end products,
though they may be useful during initial development efforts.
Note: due to internal initialization procedures, this command will not read-back valid data if it is the first
command executed following either a power-cycle event, or a SW_RST event
 */
#define MAX30003_REGISTER_ADDRESS_INFO 										0x0F
/*
CNFG_GEN is a read/write register which governs general settings, most significantly the master clock rate
for all internal timing operations. Anytime a change to CNFG_GEN is made, there may be discontinuities in
the ECG record and possibly changes to the size of the time steps recorded in the FIFOs. The SYNCH command
can be used to restore internal synchronization resulting from configuration changes.
Note when EN_ECG is logiclow, the device is in one of two ultra-low power modes (determined by EN_ULP_LON).
 */
#define MAX30003_REGISTER_ADDRESS_CNFG_GEN 									0x10
/*
 * CNFG_CAL is a read/write register that configures the operation, settings, and function of the Internal Calibration Voltage
Sources (VCALP and VCALN). The output of the voltage sources can be routed to the ECG inputs through the channel
input MUXes to facilitate end-to-end testing operations. Note if a VCAL source is applied to a connected device, it is
recommended that the appropriate channel MUX switches be placed in the OPEN position.
 */
#define MAX30003_REGISTER_ADDRESS_CNFG_CAL 									0x12
/*
CNFG_EMUX is a read/write register which configures the operation, settings, and functionality of the Input Multiplexer
associated with the ECG channel.
 */
#define MAX30003_REGISTER_ADDRESS_CNFG_EMUX 								0x14
/*
CNFG_ECG is a read/write register which configures the operation, settings, and functionality of the ECG channel.
Anytime a change to CNFG_ECG is made, there may be discontinuities in the ECG record and possibly changes to the
size of the time steps recorded in the ECG FIFO. The SYNCH command can be used to restore internal synchronization
resulting from configuration changes
 */
#define MAX30003_REGISTER_ADDRESS_CNFG_ECG 									0x15
/*
CNFG_RTOR is a two-part read/write register that configures the operation, settings, and function of the RTOR heart
rate detection block. The first register contains algorithmic voltage gain and threshold parameters, the second contains
algorithmic timing parameters
 */
#define MAX30003_REGISTER_ADDRESS_CNFG_RTOR1 								0x1D
#define MAX30003_REGISTER_ADDRESS_CNFG_RTOR2 								0x1E
/*
The ECG FIFO memory is a standard circular FIFO consisting of 32 words, each with 24 bits of information.
The ECG FIFO is independently managed by internal read and write pointers. The read pointer is updated in
response to the 32nd SCLK rising edge in a normal mode read back transaction and on the (32+n x 24)th SCLK
rising edge(s) in a burst mode transaction. Once a FIFO sample is marked as read, it cannot be accessed again.
The write pointer is governed internally. To aide data management and reduce µC overhead, the device
provides a user-programmable ECG FIFO Interrupt Threshold (EFIT) governing the ECG Interrupt term (EINT).
This threshold can be programmed with values from 1 to 32, representing the number of unread ECG FIFO entries
required before the EINT term will be asserted, alerting the µC that there is a significant amount of data in the
ECG FIFO ready for read back (see MNGR_INT (0x04) for details).
If the write pointer ever traverses the entire FIFO array and catches up to the read pointer (due to failure of the
µC to read/maintain FIFO data), a FIFO overflow will occur and data will be corrupted. The EOVF STATUS
and tag bits will indicate this condition and the FIFO should be cleared before continuing measurements using
either a SYNCH or FIFO_RST command—note overflow events will result in the loss of samples and thus timing
information, so these conditions should not occur in welldesigned applications.
 */
#define MAX30003_REGISTER_ADDRESS_ECG_FIFO_BURST 							0x20
#define MAX30003_REGISTER_ADDRESS_ECG_FIFO									0x21
/*
The RTOR Interval (RTOR) memory register is a single read-only register consisting of 14 bits of timing interval
information, left justified (and 8 unused bits, set to zero). The RTOR register stores the time interval between the
last two R events, as identified by the RTOR detection circuitry, which operates on the ECG output data. Each
LSB in the RTOR register is approximately equal to 8ms (CNFG_GEN for exact figures). The resulting 14 bit
storage interval can thus be approximately 130 seconds in length, again depending on device settings.
Each time the RTOR detector identifies a new R event, the RTOR register is updated, and the RRINT interrupt
term is asserted (see STATUS register for details). Users wishing to log heart rate based on RTOR register
data should set CLR_RRINT equals 01 in the MNGR_INT register. This will clear the RRINT interrupt term after the
RTOR register has been read back, preparing the device for identification of the next RTOR interval.
Users wishing to log heart rate based on the time elapsed between RRINT assertions using the µC to keep track of
the time base (and ignoring the RTOR register data) have two choices for interrupt management. If CLR_RRINT
equals 00 in the MNGR_INT register, the RRINT interrupt term will clear after each STATUS register read back,
preparing the device for identification of the next RTOR interval. If CLR_RRINT equals 10 in the MNGR_INT
register, the RRINT interrupt term will self-clear after each one full ECG data cycle has passed, preparing the device
for identification of the next RTOR interval (this mode is recommended only if using the INT2B as a dedicated
heart rate indicator).
If the RTOR detector reaches an overflow state after several minutes without detection of an R event, the counter will
simply roll over, and the lack of the RRINT activity on the dedicated INT2B line will inform the µC that no RTOR
activity was detected.
 */
#define MAX30003_REGISTER_ADDRESS_RTOR										0x25

/*
 * Registers addresses section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


/*
 * STATUS register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
 * ECG FIFO Interrupt. Indicates that ECG records meeting/exceeding the ECG FIFO Interrupt
Threshold (EFIT) are available for readback. Remains active until ECG FIFO is read back to the
extent required to clear the EFIT condition.
 */
#define MAX30003_REGISTER_STATUS_EINT_POS									23U
#define MAX30003_REGISTER_STATUS_EINT										(1U << MAX30003_REGISTER_STATUS_EINT_POS)
/*
ECG FIFO Overflow. Indicates that the ECG FIFO has overflown and the data record has been corrupted.
Remains active until a FIFO Reset (recommended) or SYNCH operation is issued.
 */
#define MAX30003_REGISTER_STATUS_EOVF_POS									22U
#define MAX30003_REGISTER_STATUS_EOVF										(1U << MAX30003_REGISTER_STATUS_EOVF_POS)
/*
ECG Fast Recovery Mode. Issued when the ECG Fast Recovery Mode is engaged (either manually or automatically).
Status and Interrupt Clear behavior is defined by CLR_FAST, see MNGR_INT for details.
 */
#define MAX30003_REGISTER_STATUS_FSTINT_POS									21U
#define MAX30003_REGISTER_STATUS_FSTINT										(1U << MAX30003_REGISTER_STATUS_FSTINT_POS)
/*
DC Lead-Off Detection Interrupt. Indicates that the MAX30003 has determined it is in an ECG
leads off condition (as selected in CNFG_GEN) for more than 115ms.
Remains active as long as the leads-off condition persists, then held until cleared by STATUS read back (32nd SCLK).
 */
#define MAX30003_REGISTER_STATUS_DCLOFFINT_POS								20U
#define MAX30003_REGISTER_STATUS_DCLOFFINT									(1U << MAX30003_REGISTER_STATUS_DCLOFFINT_POS)
/*
Ultra-Low Power (ULP) Leads-On Detection Interrupt. Indicates that the MAX30003 has determined
it is in a leads-on condition (as selected in CNFG_GEN).
LONINT is asserted whenever EN_ULP_LON[1:0] in register CNFG_GEN is set to either 0b01 or
0b10 to indicate that the ULP leads on detection mode has been enabled. The STATUS register
has to be read back once after ULP leads on detection mode has been activated to clear LONINT
and enable leads on detection.
LONINT remains active while the leads-on condition persists, then held until cleared by STATUS read
back (32nd SCLK).
 */
#define MAX30003_REGISTER_STATUS_LONINT_POS									11U
#define MAX30003_REGISTER_STATUS_LONINT										(1U << MAX30003_REGISTER_STATUS_LONINT_POS)
/*
ECG R to R Detector R Event Interrupt. Issued when the R to R detector has identified a new R event.
Clear behavior is defined by CLR_RRINT[1:0]; see MNGR_INT for details.
 */
#define MAX30003_REGISTER_STATUS_RRINT_POS									10U
#define MAX30003_REGISTER_STATUS_RRINT										(1U << MAX30003_REGISTER_STATUS_RRINT_POS)
/*
Sample Synchronization Pulse. Issued on the ECG base-rate sampling instant, for use in assisting
µC monitoring and synchronizing other peripheral operations and data, generally recommended for use
as a dedicated interrupt.
Frequency is selected by SAMP_IT[1:0], see MNGR_INT for details.
Clear behavior is defined by CLR_SAMP, see MNGR_INT for details.
 */
#define MAX30003_REGISTER_STATUS_SAMP_POS									9U
#define MAX30003_REGISTER_STATUS_SAMP										(1U << MAX30003_REGISTER_STATUS_SAMP_POS)
/*
PLL Unlocked Interrupt. Indicates that the PLL has not yet achieved or has lost its phase lock.
PLLINT will only be asserted when the PLL is powered up and active (ECG and/or BIOZ Channel
enabled).
Remains asserted while the PLL unlocked condition persists, then held until cleared by STATUS read
back (32nd SCLK).
 */
#define MAX30003_REGISTER_STATUS_PLLINT_POS									8U
#define MAX30003_REGISTER_STATUS_PLLINT										(1U << MAX30003_REGISTER_STATUS_PLLINT_POS)
/*
DC Lead Off Detection Detailed Status. Indicates that the MAX30003 has determined (as selected by
CNFG_GEN):
ECGP is above the high threshold (VTHH), ECGP is below the low threshold (VTHL),
ECGN is above the high threshold (VTHH), ECGN is below the low threshold (VTHL), respectively.
Remains active as long as the leads-off detection is active and the leads-off condition persists, then
held until cleared by STATUS read back (32nd SCLK). LDOFF_PH to LDOFF_NL are detailed status
bits that are asserted at the same time as DCLOFFINT.
 */
#define MAX30003_REGISTER_STATUS_LDOFF_PH_POS								3U
#define MAX30003_REGISTER_STATUS_LDOFF_PH									(1U << MAX30003_REGISTER_STATUS_LDOFF_PH_POS)

#define MAX30003_REGISTER_STATUS_LDOFF_PL_POS								2U
#define MAX30003_REGISTER_STATUS_LDOFF_PL									(1 << MAX30003_REGISTER_STATUS_LDOFF_PL_POS)

#define MAX30003_REGISTER_STATUS_LDOFF_NH_POS								1U
#define MAX30003_REGISTER_STATUS_LDOFF_NH									(1U << MAX30003_REGISTER_STATUS_LDOFF_NH_POS)

#define MAX30003_REGISTER_STATUS_LDOFF_NL									1U
/*
 * STATUS register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * EN_INT AND EN_INT2 registers section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
Interrupt Enables for interrupt terms in STATUS[23:8]
0 = Individual interrupt term is not included in the interrupt OR term
1 = Individual interrupt term is included in the interrupt OR term
Default value: 0x0000
 */
#define MAX30003_REGISTER_EN_INT_EINT_POS									23U
#define MAX30003_REGISTER_EN_INT_EINT										(1U << MAX30003_REGISTER_EN_INT_EINT_POS)

#define MAX30003_REGISTER_EN_INT_EOVF_POS									22U
#define MAX30003_REGISTER_EN_INT_EOVF										(1U << MAX30003_REGISTER_EN_INT_EOVF_POS)

#define MAX30003_REGISTER_EN_INT_FSTINT_POS									21U
#define MAX30003_REGISTER_EN_INT_FSTINT										(1U << MAX30003_REGISTER_EN_INT_FSTINT_POS)

#define MAX30003_REGISTER_EN_INT_DCLOFFINT_POS								20U
#define MAX30003_REGISTER_EN_INT_DCLOFFINT									(1U << MAX30003_REGISTER_EN_INT_DCLOFFINT_POS)

#define MAX30003_REGISTER_EN_INT_EN_LONINT_POS								11U
#define MAX30003_REGISTER_EN_INT_EN_LONINT									(1U << MAX30003_REGISTER_EN_INT_EN_LONINT_POS)

#define MAX30003_REGISTER_EN_INT_EN_RRINT_POS								10U
#define MAX30003_REGISTER_EN_INT_EN_RRINT									(1U << MAX30003_REGISTER_EN_INT_EN_RRINT_POS)

#define MAX30003_REGISTER_EN_INT_EN_SAMP_POS								9U

#define MAX30003_REGISTER_EN_INT_EN_SAMP									(1U << MAX30003_REGISTER_EN_INT_EN_SAMP_POS)

#define MAX30003_REGISTER_EN_INT_EN_PLLINT_POS								8U
#define MAX30003_REGISTER_EN_INT_EN_PLLINT									(1U << MAX30003_REGISTER_EN_INT_EN_PLLINT_POS)

/*
INTB and INT2B Port Type (EN_INT Selections)
00 = Disabled (Three-state)
01 = CMOS Driver
10 = Open-Drain NMOS Driver
11 = Open-Drain NMOS Driver with Internal 125kΩ Pullup Resistance
Default value: 0x03
 */
#define MAX30003_REGISTER_EN_INT_INTB_TYPE_MASK								0x03U
#define MAX30003_REGISTER_EN_INT_INTB_TYPE_DISABLED							0x00U
#define MAX30003_REGISTER_EN_INT_INTB_TYPE_CMOS_DRIVER						0x01U
#define MAX30003_REGISTER_EN_INT_INTB_TYPE_OPEN_DRAIN_NMOS_DRIVER			0x02U
#define MAX30003_REGISTER_EN_INT_INTB_TYPE_OPEN_DRAIN_NMOS_DRIVER_PULLUP	0x03U
/*
 * EN_INT AND EN_INT2 registers section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


/*
 * MNGR_INT register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
ECG FIFO Interrupt Threshold (issues EINT based on number of unread
FIFO records)
00000 to 11111 = 1 to 32, respectively (i.e. EFIT[4:0]+1 unread records)
Default value: 0x0F
 */
#define MAX30003_REGISTER_MNGR_INT_EFIT_POS									19U
#define MAX30003_REGISTER_MNGR_INT_EFIT_MASK								(0x1FU << MAX30003_REGISTER_MNGR_INT_EFIT_POS)
/*
FAST MODE Interrupt Clear Behavior:
0 = FSTINT remains active until the FAST mode is disengaged (manually or
automatically), then held until cleared by STATUS read back (32nd SCLK).
1 = FSTINT remains active until cleared by STATUS read back (32nd SCLK),
even if the MAX30003 remains in FAST recovery mode. Once cleared,
FSTINT will not be re-asserted until FAST mode is exited and re-entered,
either manually or automatically.
Default value: 0x00
 */
#define MAX30003_REGISTER_MNGR_INT_CLR_FAST_POS								6U
#define MAX30003_REGISTER_MNGR_INT_CLR_FAST									(1U << MAX30003_REGISTER_MNGR_INT_CLR_FAST_POS)
/*
RTOR R Detect Interrupt (RRINT) Clear Behavior:
00 = Clear RRINT on STATUS Register Read Back
01 = Clear RRINT on RTOR Register Read Back
10 = Self-Clear RRINT after one ECG data rate cycle, approximately 2ms to 8ms
11 = Reserved. Do not use.
Default value: 0x00
 */
#define MAX30003_REGISTER_MNGR_INT_CLR_RRINT_POS							4U
#define MAX30003_REGISTER_MNGR_INT_CLR_RRINT_MASK							(0x3U << MAX30003_REGISTER_MNGR_INT_CLR_RRINT_POS)
#define MAX30003_REGISTER_MNGR_INT_CLR_RRINT_CLEAR_RRINT_ON_STATUS			(0U << MAX30003_REGISTER_MNGR_INT_CLR_RRINT_POS)
#define MAX30003_REGISTER_MNGR_INT_CLR_RRINT_CLEAR_RRINT_ON_RTOR			(1U << MAX30003_REGISTER_MNGR_INT_CLR_RRINT_POS)
#define MAX30003_REGISTER_MNGR_INT_CLR_RRINT_CLEAR_RRINT_SELF_CLEAR_RRINT	(2U << MAX30003_REGISTER_MNGR_INT_CLR_RRINT_POS)
/*
Sample Synchronization Pulse (SAMP) Clear Behavior:
0 = Clear SAMP on STATUS Register Read Back (recommended for debug/
evaluation only).
1 = Self-clear SAMP after approximately one-fourth of one data rate cycle.
Default value: 0x01
 */
#define MAX30003_REGISTER_MNGR_INT_CLR_SAMP_POS								2U
#define MAX30003_REGISTER_MNGR_INT_CLR_SAMP									(1U << MAX30003_REGISTER_MNGR_INT_CLR_SAMP_POS)
/*
Sample Synchronization Pulse (SAMP) Frequency
00 = issued every sample instant
01 = issued every 2nd sample instant
10 = issued every 4th sample instant
11 = issued every 16th sample instant
Default value: 0x00
 */
#define MAX30003_REGISTER_MNGR_INT_SAMP_IT_MASK								0x03U
#define MAX30003_REGISTER_MNGR_INT_SAMP_IT_EVERY_SAMPLE						0x00U
#define MAX30003_REGISTER_MNGR_INT_SAMP_IT_EVERY_2ND_SAMPLE					0x01U
#define MAX30003_REGISTER_MNGR_INT_SAMP_IT_EVERY_4ND_SAMPLE					0x02U
#define MAX30003_REGISTER_MNGR_INT_SAMP_IT_EVERY_16ND_SAMPLE				0x03U
/*
 * MNGR_INT register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * MNGR_DYN register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
ECG Channel Fast Recovery Mode Selection (ECG High Pass Filter Bypass):
00 = Normal Mode (Fast Recovery Mode Disabled)
01 = Manual Fast Recovery Mode Enable (remains active until disabled)
10 = Automatic Fast Recovery Mode Enable (Fast Recovery automatically
activated when/while ECG outputs are saturated, using FAST_TH).
11 = Reserved. Do not use.
Default value: 0x00
 */
#define MAX30003_REGISTER_MNGR_DYN_FAST_POS									22U
#define MAX30003_REGISTER_MNGR_DYN_FAST_MASK								(0x03U << MAX30003_REGISTER_MNGR_DYN_FAST_POS)
#define MAX30003_REGISTER_MNGR_DYN_FAST_NORMAL_MODE							(0x00U << MAX30003_REGISTER_MNGR_DYN_FAST_POS)
#define MAX30003_REGISTER_MNGR_DYN_FAST_MANUAL_FAST_RECOVERY_MODE			(0x01U << MAX30003_REGISTER_MNGR_DYN_FAST_POS)
#define MAX30003_REGISTER_MNGR_DYN_FAST_AUTOMATIC_FAST_RECOVERY_MOD			(0x02U << MAX30003_REGISTER_MNGR_DYN_FAST_POS)

/*
Automatic Fast Recovery Threshold:
If FAST[1:0] = 10 and the output of an ECG measurement exceeds the symmetric
thresholds defined by 2048*FAST_TH for more than 125ms, the Fast Recovery
mode will be automatically engaged and remain active for 500ms.
For example, the default value (FAST_TH = 0x3F) corresponds to an ECG output
upper threshold of 0x1F800, and an ECG output lower threshold of 0x20800.
Default value: 0x3F
 */
#define MAX30003_REGISTER_MNGR_DYN_FAST_TH_POS								16U
#define MAX30003_REGISTER_MNGR_DYN_FAST_TH_MASK								(0x3FU << MAX30003_REGISTER_MNGR_DYN_FAST_TH_POS)
/*
 * MNGR_DYN register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * SW_RST register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30003_REGISTER_SW_RST_VALUE										(0x000000)
/*
 * SW_RST register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * SYNCH register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30003_REGISTER_SYNCH_VALUE										(0x000000)
/*
 * SYNCH register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * FIFO_RST register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30003_REGISTER_FIFO_RST_VALUE									(0x000000)
/*
 * FIFO_RST register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * INFO register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#define MAX30003_REGISTER_INFO_REV_ID_POS									16U
#define MAX30003_REGISTER_INFO_REV_ID_MASK									(0x07U << MAX30003_REGISTER_INFO_REV_ID_POS)
/*
 * INFO register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * CNFG_GEN register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
Ultra-Low Power Lead-On Detection Enable
00 = ULP Lead-On Detection disabled
01 = ECG ULP Lead-On Detection enabled
10 = Reserved. Do not use.
11 = Reserved. Do not use.
ULP mode is only active when the ECG channel is powered down/disabled
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_GEN_EN_ULP_LON_POS							22U
#define MAX30003_REGISTER_CNFG_GEN_EN_ULP_LON_MASK							(0x03U << MAX30003_REGISTER_CNFG_GEN_EN_ULP_LON_POS)
#define MAX30003_REGISTER_CNFG_GEN_EN_ULP_LON_ULP_DETECTION_DISABLED		(0x00U << MAX30003_REGISTER_CNFG_GEN_EN_ULP_LON_POS)
#define MAX30003_REGISTER_CNFG_GEN_EN_ULP_LON_ULP_DETECTION_ENABLED			(0x01U << MAX30003_REGISTER_CNFG_GEN_EN_ULP_LON_POS)
/*
Master Clock Frequency. Selects the Master Clock Frequency (FMSTR), and Timing
Resolution (TRES), which also determines the ECG and CAL timing characteristics.
These are generated from FCLK, which is always 32.768Khz.
00 = FMSTR = 32768Hz, TRES = 15.26µs (512Hz ECG progressions)
01 = FMSTR = 32000Hz, TRES = 15.63µs (500Hz ECG progressions)
10 = FMSTR = 32000Hz, TRES = 15.63µs (200Hz ECG progressions)
11 = FMSTR = 31968.78Hz, TRES = 15.64µs (199.8049Hz ECG progressions)
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_GEN_FMSTR_POS								20U
#define MAX30003_REGISTER_CNFG_GEN_FMSTR_MASK								(0x03U << MAX30003_REGISTER_CNFG_GEN_FMSTR_POS)
#define MAX30003_REGISTER_CNFG_GEN_FMSTR_32768HZ							(0U << MAX30003_REGISTER_CNFG_GEN_FMSTR_POS)
#define MAX30003_REGISTER_CNFG_GEN_FMSTR_32000HZ							(1U << MAX30003_REGISTER_CNFG_GEN_FMSTR_POS)
#define MAX30003_REGISTER_CNFG_GEN_FMSTR_32000HZ_2							(2U << MAX30003_REGISTER_CNFG_GEN_FMSTR_POS)
#define MAX30003_REGISTER_CNFG_GEN_FMSTR_31968_78HZ							(3U << MAX30003_REGISTER_CNFG_GEN_FMSTR_POS)
/*
ECG Channel Enable
0 = ECG Channel disabled
1 = ECG Channel enabled
Note: The ECG channel must be enabled to allow R-to-R operation
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_GEN_ECG_POS									19U
#define MAX30003_REGISTER_CNFG_GEN_DISABLE_ECG								(0U << MAX30003_REGISTER_CNFG_GEN_ECG_POS)
#define MAX30003_REGISTER_CNFG_GEN_ENABLE_ECG								(1U << MAX30003_REGISTER_CNFG_GEN_ECG_POS)
/*
DC Lead-Off Detection Enable
00 = DC Lead-Off Detection disabled
01 = DCLOFF Detection applied to the ECGP/N pins
10 = Reserved. Do not use.
11 = Reserved. Do not use.
DC Method, requires active selected channel, enables DCLOFF interrupt
and status bit behavior.
Uses current sources and comparator thresholds set below
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_GEN_EN_DCLOFF_POS							12U
#define MAX30003_REGISTER_CNFG_GEN_EN_DCLOFF_MASK							(0x03U << MAX30003_REGISTER_CNFG_GEN_EN_DCLOFF_POS)
#define MAX30003_REGISTER_CNFG_GEN_EN_DCLOFF_DC_DETECTION_DISABLED			(0x00U << MAX30003_REGISTER_CNFG_GEN_EN_DCLOFF_POS)
#define MAX30003_REGISTER_CNFG_GEN_EN_DCLOFF_DC_DETECTION_ENABLED			(0x01U << MAX30003_REGISTER_CNFG_GEN_EN_DCLOFF_POS)
/*
DC Lead-Off Current Polarity (if current sources are enabled/connected)
0 = ECGP - Pullup ECGN – Pulldown
1 = ECGP - Pulldown ECGN – Pullup
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_IPOL_POS							11U
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_IPOL_ECGP_PULLUP					(0U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_IPOL_POS)
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_IPOL_ECGP_PULLDOWN				(1U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_IPOL_POS)
/*
DC Lead-Off Current Magnitude Selection
000 = 0nA (Disable and Disconnect Current Sources)
001 = 5nA
010 = 10nA
011 = 20nA
100 = 50nA
101 = 100nA
110 = Reserved. Do not use.
111 = Reserved. Do not use
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_POS							8U
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_MASK							(7U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_POS)
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_5nA							(1U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_POS)
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_10nA							(2U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_POS)
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_20nA							(3U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_POS)
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_50nA							(4U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_POS)
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_100nA						(5U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_IMAG_POS)
/*
DC Lead-Off Voltage Threshold Selection
00 = VMID ± 300mV
01 = VMID ± 400mV
10 = VMID ± 450mV
11 = VMID ± 500mV
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_POS							6U
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_MASK							(3U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_POS)
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_VMID_300mV					(0U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_POS)
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_VMID_400mV					(1U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_POS)
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_VMID_450mV					(2U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_POS)
#define MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_VMID_500mV					(3U << MAX30003_REGISTER_CNFG_GEN_DCLOFF_VTH_POS)
/*
Enable and Select Resistive Lead Bias Mode
00 = Resistive Bias disabled
01 = ECG Resistive Bias enabled if EN_ECG is also enabled
10 = Reserved. Do not use.
11 = Reserved. Do not use.
If EN_ECG is not asserted at the same time as prior to EN_RBIAS[1:0] being set to
01, then EN_RBIAS[1:0] will remain set to 00.
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_GEN_EN_RBIAS_POS								4U
#define MAX30003_REGISTER_CNFG_GEN_EN_RBIAS_MASK							(3U << MAX30003_REGISTER_CNFG_GEN_EN_RBIAS_POS)
#define MAX30003_REGISTER_CNFG_GEN_EN_RBIAS_RESISTIVE_BIAS_DISABLED			(0U << MAX30003_REGISTER_CNFG_GEN_EN_RBIAS_POS)
#define MAX30003_REGISTER_CNFG_GEN_EN_RBIAS_RESISTIVE_BIAS_ENABLED			(1U << MAX30003_REGISTER_CNFG_GEN_EN_RBIAS_POS)
/*
Resistive Bias Mode Value Selection
00 = RBIAS = 50MΩ
01 = RBIAS = 100MΩ
10 = RBIAS = 200MΩ
11 = Reserved. Do not use
Default value: 0x01
 */
#define MAX30003_REGISTER_CNFG_GEN_RBIASV_POS								2U
#define MAX30003_REGISTER_CNFG_GEN_RBIASV_MASK								(3U << MAX30003_REGISTER_CNFG_GEN_RBIASV_POS)
#define MAX30003_REGISTER_CNFG_GEN_RBIASV_RBIAS_50M							(0U << MAX30003_REGISTER_CNFG_GEN_RBIASV_POS)
#define MAX30003_REGISTER_CNFG_GEN_RBIASV_RBIAS_100M						(1U << MAX30003_REGISTER_CNFG_GEN_RBIASV_POS)
#define MAX30003_REGISTER_CNFG_GEN_RBIASV_RBIAS_200M						(2U << MAX30003_REGISTER_CNFG_GEN_RBIASV_POS)
/*
Enables Resistive Bias on Positive Input
0 = ECGP is not resistively connected to VMID
1 = ECGP is connected to VMID through a resistor (selected by RBIASV).
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGP_POS							1U
#define MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGP_DISCONNECTED					(0U << MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGP_POS)
#define MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGP_CONNECTED					(1U << MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGP_POS)
/*
Enables Resistive Bias on Negative Input
0 = ECGN is not resistively connected to VMID
1 = ECGN is connected to VMID through a resistor (selected by RBIASV).
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGN_POS							0U
#define MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGN_DISCONNECTED					(0U << MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGN_POS)
#define MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGN_CONNECTED					(1U << MAX30003_REGISTER_CNFG_GEN_RBIASP_ECGN_POS)
/*
 * CNFG_GEN register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * CNFG_CAL register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
Calibration Source (VCALP and VCALN) Enable
0 = Calibration sources and modes disabled
1 = Calibration sources and modes enabled
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_CAL_EN_VCAL_POS								22U
#define MAX30003_REGISTER_CNFG_CAL_EN_VCAL_ENABLE_CALIBRATION				(1U << MAX30003_REGISTER_CNFG_CAL_EN_VCAL_POS)
#define MAX30003_REGISTER_CNFG_CAL_EN_VCAL_DISABLE_CALIBRATION				(0U << MAX30003_REGISTER_CNFG_CAL_EN_VCAL_POS)
/*
Calibration Source Mode Selection
0 = Unipolar, sources swing between VMID ± VMAG and VMID
1 = Bipolar, sources swing between VMID + VMAG and VMID - VMAG
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_CAL_VMODE_POS								21U
#define MAX30003_REGISTER_CNFG_CAL_VMODE_UNIPOLAR							(0U << MAX30003_REGISTER_CNFG_CAL_VMODE_POS)
#define MAX30003_REGISTER_CNFG_CAL_VMODE_BIPOLAR							(1U << MAX30003_REGISTER_CNFG_CAL_VMODE_POS)
/*
Calibration Source Magnitude Selection (VMAG)
0 = 0.25mV
1 = 0.50mV
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_CAL_VMAG_POS									20U
#define MAX30003_REGISTER_CNFG_CAL_VMAG_250UV								(0U << MAX30003_REGISTER_CNFG_CAL_VMAG_POS)
#define MAX30003_REGISTER_CNFG_CAL_VMAG_500UV								(1U << MAX30003_REGISTER_CNFG_CAL_VMAG_POS)
/*
Calibration Source Frequency Selection (FCAL)
000 = FMSTR/128 (Approximately 256Hz)
001 = FMSTR /512 (Approximately 64Hz)
010 = FMSTR /2048 (Approximately 16Hz)
011 = FMSTR /8192 (Approximately 4Hz)
100 = FMSTR /2^15
 (Approximately 1Hz)
101 = FMSTR /2^17
 (Approximately 1/4Hz)
110 = FMSTR /2^19
 (Approximately 1/16Hz)
111 = FMSTR /2^21
 (Approximately 1/64Hz)
Actual frequencies are determined by FMSTR selection (see CNFG_GEN for
details), approximate frequencies are based on a 32768Hz clock (FMSTR[2:0] = 000).
TCAL = 1/FCAL.
Default value: 0x04
 */
#define MAX30003_REGISTER_CNFG_CAL_FCAL_POS									11U
#define MAX30003_REGISTER_CNFG_CAL_FCAL_128									(0U << MAX30003_REGISTER_CNFG_CAL_FCAL_POS)
#define MAX30003_REGISTER_CNFG_CAL_FCAL_512									(1U << MAX30003_REGISTER_CNFG_CAL_FCAL_POS)
#define MAX30003_REGISTER_CNFG_CAL_FCAL_2048								(2U << MAX30003_REGISTER_CNFG_CAL_FCAL_POS)
#define MAX30003_REGISTER_CNFG_CAL_FCAL_8192								(3U << MAX30003_REGISTER_CNFG_CAL_FCAL_POS)
#define MAX30003_REGISTER_CNFG_CAL_FCAL_32768								(4U << MAX30003_REGISTER_CNFG_CAL_FCAL_POS)
#define MAX30003_REGISTER_CNFG_CAL_FCAL_131072								(5U << MAX30003_REGISTER_CNFG_CAL_FCAL_POS)
#define MAX30003_REGISTER_CNFG_CAL_FCAL_524288								(6U << MAX30003_REGISTER_CNFG_CAL_FCAL_POS)
#define MAX30003_REGISTER_CNFG_CAL_FCAL_2097152								(7U << MAX30003_REGISTER_CNFG_CAL_FCAL_POS)
/*
Calibration Source Duty Cycle Mode Selection
0 = Use CAL_THIGH to select time high for VCALP and VCALN
1 = THIGH = 50% (CAL_THIGH[10:0] are ignored)
Default value: 0x01
 */
#define MAX30003_REGISTER_CNFG_CAL_FIFTY_POS								10U
#define MAX30003_REGISTER_CNFG_CAL_FIFTY_USE_CAL_THIGH						(0U << MAX30003_REGISTER_CNFG_CAL_FIFTY_POS)
#define MAX30003_REGISTER_CNFG_CAL_FIFTY_IGNORE_CAL_THIGH					(1U << MAX30003_REGISTER_CNFG_CAL_FIFTY_POS)
/*
Calibration Source Time High Selection
If FIFTY = 1, tHIGH = 50% (and THIGH[10:0] are ignored),
otherwise THIGH = THIGH[10:0] x CAL_RES
CAL_RES is determined by FMSTR selection (see CNFG_GEN for details);
for example, if FMSTR[2:0] = 000,CAL_RES = 30.52µs.
Default value: 0x000
 */
#define MAX30003_REGISTER_CNFG_CAL_THIGH_POS								 0U
#define MAX30003_REGISTER_CNFG_CAL_THIGH_MASK								 0x3FF


/*
 * CNFG_CAL register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * CNFG_EMUX register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
ECG Input Polarity Selection
0 = Non-inverted
1 = Inverted
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_EMUX_POL_POS									23U
#define MAX30003_REGISTER_CNFG_EMUX_POL_NON_INVERTED						(0U << MAX30003_REGISTER_CNFG_EMUX_POL_POS)
#define MAX30003_REGISTER_CNFG_EMUX_POL_INVERTED							(1U << MAX30003_REGISTER_CNFG_EMUX_POL_POS)

/*
Open the ECGP Input Switch (most often used for testing and calibration studies)
0 = ECGP is internally connected to the ECG AFE Channel
1 = ECGP is internally isolated from the ECG AFE Channel
Default value: 0x01
 */
#define MAX30003_REGISTER_CNFG_EMUX_OPENP_POS								21U
#define MAX30003_REGISTER_CNFG_EMUX_OPENP_CONNECTED							(0U << MAX30003_REGISTER_CNFG_EMUX_OPENP_POS)
#define MAX30003_REGISTER_CNFG_EMUX_OPENP_ISOLATED							(1U << MAX30003_REGISTER_CNFG_EMUX_OPENP_POS)
/*
 * Open the ECGN Input Switch (most often used for testing and calibration studies)
0 = ECGN is internally connected to the ECG AFE Channel
1 = ECGN is internally isolated from the ECG AFE Channel
Default value: 0x01
 */
#define MAX30003_REGISTER_CNFG_EMUX_OPENN_POS								20U
#define MAX30003_REGISTER_CNFG_EMUX_OPENN_CONNECTED							(0U << MAX30003_REGISTER_CNFG_EMUX_OPENN_POS)
#define MAX30003_REGISTER_CNFG_EMUX_OPENN_ISOLATED							(1U << MAX30003_REGISTER_CNFG_EMUX_OPENN_POS)
/*
ECGP Calibration Selection
00 = No calibration signal applied
01 = Input is connected to VMID
10 = Input is connected to VCALP (only available if CAL_EN_VCAL = 1)
11 = Input is connected to VCALN (only available if CAL_EN_VCAL = 1)
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_POS							18U
#define MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_MASK							(3U << MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_POS)
#define MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_NO_CALIBRATION					(0U << MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_POS)
#define MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_CONNECTE_TO_VMID				(1U << MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_POS)
#define MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_CONNECTE_TO_VCALP				(2U << MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_POS)
#define MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_CONNECTE_TO_VCALN				(3U << MAX30003_REGISTER_CNFG_EMUX_CALP_SEL_POS)
/*
ECGN Calibration Selection
00 = No calibration signal applied
01 = Input is connected to VMID
10 = Input is connected to VCALP (only available if CAL_EN_VCAL = 1)
11 = Input is connected to VCALN (only available if CAL_EN_VCAL = 1)
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_POS							16U
#define MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_MASK							(3U << MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_POS)
#define MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_NO_CALIBRATION					(0U << MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_POS)
#define MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_CONNECTE_TO_VMID				(1U << MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_POS)
#define MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_CONNECTE_TO_VCALP				(2U << MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_POS)
#define MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_CONNECTE_TO_VCALN				(3U << MAX30003_REGISTER_CNFG_EMUX_CALN_SEL_POS)
/*
 * CNFG_EMUX register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * CNFG_ECG register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
ECG Data Rate (also dependent on FMSTR selection, see CNFG_GEN Table 29):
FMSTR = 00: fMSTR = 32768Hz, tRES = 15.26µs (512Hz ECG progressions)
00 = 512sps
01 = 256sps
10 = 128sps
11 = Reserved. Do not use.
FMSTR = 01: fMSTR = 32000Hz, tRES = 15.63µs (500Hz ECG progressions)
00 = 500sps
01 = 250sps
10 = 125sps
11 = Reserved. Do not use.
FMSTR = 10: fMSTR = 32000Hz, tRES = 15.63µs (200Hz ECG progressions)
00 = Reserved. Do not use.
01 = Reserved. Do not use.
10 = 200sps
11 = Reserved. Do not use.
FMSTR = 11: fMSTR = 31968Hz, tRES = 15.64µs (199.8Hz ECG progressions)
00 = Reserved. Do not use.
01 = Reserved. Do not use.
10 = 199.8sps
11 = Reserved. Do not use.
Default value: 0x02
 */
#define MAX30003_REGISTER_CNFG_ECG_RATE_POS									22U
#define MAX30003_REGISTER_CNFG_ECG_RATE_MASK								(3U << MAX30003_REGISTER_CNFG_ECG_RATE_POS)
#define MAX30003_REGISTER_CNFG_ECG_RATE_HIGH								(0U << MAX30003_REGISTER_CNFG_ECG_RATE_POS)
#define MAX30003_REGISTER_CNFG_ECG_RATE_MED									(1U << MAX30003_REGISTER_CNFG_ECG_RATE_POS)
#define MAX30003_REGISTER_CNFG_ECG_RATE_LOW									(2U << MAX30003_REGISTER_CNFG_ECG_RATE_POS)

/*
ECG Channel Gain Setting
00 = 20V/V
01 = 40V/V
10 = 80V/V
11 = 160V/V
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_ECG_GAIN_POS									16U
#define MAX30003_REGISTER_CNFG_ECG_GAIN_MASK								(3U << MAX30003_REGISTER_CNFG_ECG_GAIN_POS)
#define MAX30003_REGISTER_CNFG_ECG_GAIN_20									(0U << MAX30003_REGISTER_CNFG_ECG_GAIN_POS)
#define MAX30003_REGISTER_CNFG_ECG_GAIN_40									(1U << MAX30003_REGISTER_CNFG_ECG_GAIN_POS)
#define MAX30003_REGISTER_CNFG_ECG_GAIN_80									(2U << MAX30003_REGISTER_CNFG_ECG_GAIN_POS)
#define MAX30003_REGISTER_CNFG_ECG_GAIN_160									(3U << MAX30003_REGISTER_CNFG_ECG_GAIN_POS)
/*
ECG Channel Digital High-Pass Filter Cutoff Frequency
0 = Bypass (DC)
1 = 0.50Hz
Default value: 0x01
 */
#define MAX30003_REGISTER_CNFG_ECG_DHPF_POS									14U
#define MAX30003_REGISTER_CNFG_ECG_DHPF_BYPASS								(0U << MAX30003_REGISTER_CNFG_ECG_DHPF_POS)
#define MAX30003_REGISTER_CNFG_ECG_DHPF_0_50HZ								(1U << MAX30003_REGISTER_CNFG_ECG_DHPF_POS)
/*
ECG Channel Digital Low-Pass Filter Cutoff Frequency
00 = Bypass (Decimation only, no FIR filter applied)
01 = approximately 40Hz
10 = approximately 100Hz (Available for 512, 256, 500, and 250sps ECG Rate
selections only)
11 = approximately 150Hz (Available for 512 and 500sps ECG Rate selections only)
Note: See Table 29. If an unsupported DLPF setting is specified, the 40Hz setting
(DLPF[1:0] = 01) will be used internally; the CNFG_ECG register will continue to hold
the value as written, but return the effective internal value when read back.
Default value: 0x01
 */
#define MAX30003_REGISTER_CNFG_ECG_DLPF_POS									12U
#define MAX30003_REGISTER_CNFG_ECG_DLPF_MASK								(3U << MAX30003_REGISTER_CNFG_ECG_DLPF_POS)
#define MAX30003_REGISTER_CNFG_ECG_DLPF_BYPASS								(0U << MAX30003_REGISTER_CNFG_ECG_DLPF_POS)
#define MAX30003_REGISTER_CNFG_ECG_DLPF_40HZ								(1U << MAX30003_REGISTER_CNFG_ECG_DLPF_POS)
#define MAX30003_REGISTER_CNFG_ECG_DLPF_100HZ								(2U << MAX30003_REGISTER_CNFG_ECG_DLPF_POS)
#define MAX30003_REGISTER_CNFG_ECG_DLPF_150HZ								(3U << MAX30003_REGISTER_CNFG_ECG_DLPF_POS)
/*
 * CNFG_ECG register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/*
 * CNFG_RTOR1 register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
This is the width of the averaging window, which adjusts the algorithm sensitivity to
the width of the QRS complex.
R to R Window Averaging (Window Width = RTOR_WNDW[3:0]*8ms)
0000 = 6
0001 = 8
0010 = 10
0011 = 12 (default = 96ms)
0100 = 14
0101 = 16
0110 = 18
0111 = 20
1000 = 22
1001 = 24
1010 = 26
1011 = 28
1100 = Reserved. Do not use.
1101 = Reserved. Do not use.
1110 = Reserved. Do not use.
1111 = Reserved. Do not use.
Default value: 0x03 (96ms)
 */
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS								20U
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_MASK								(0x0FU << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_6									(0x00U << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_8									(0x01U << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_10								(0x02U << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_12								(0x03U << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_14								(0x04U << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_16								(0x05U << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_18								(0x06U << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_20								(0x07U << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_22								(0x08U << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_24								(0x09U << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_26								(0x0AU << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_WNDW_28								(0x0BU << MAX30003_REGISTER_CNFG_RTOR1_WNDW_POS)
/*
R to R Gain (where Gain = 2^GAIN[3:0], plus an auto-scale option). This is used to
maximize the dynamic range of the algorithm.
0000 = 1 1000 = 256
0001 = 2 1001 = 512
0010 = 4 1010 = 1024
0011 = 8 1011 = 2048
0100 = 16 1100 = 4096
0101 = 32 1101 = 8192
0110 = 64 1110 = 16384
0111 = 128 1111 = Auto-Scale (default)
In Auto-Scale mode, the initial gain is set to 64.
Default value: 0x0F
 */
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS								16U
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_MASK								(0x0FU << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_1									(0x00U << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_2									(0x01U << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_4									(0x02U << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_8									(0x03U << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_16								(0x04U << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_32								(0x05U << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_64								(0x06U << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_128								(0x07U << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_256								(0x08U << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_512								(0x09U << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_1024								(0x0AU << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_2048								(0x0BU << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_4096								(0x0CU << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_8192								(0x0DU << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_16384								(0x0EU << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_GAIN_AUTO_SCALE						(0x0FU << MAX30003_REGISTER_CNFG_RTOR1_GAIN_POS)
/*
ECG RTOR Detection Enable
0 = RTOR Detection disabled
1 = RTOR Detection enabled if EN_ECG is also enabled
Default value: 0x00
 */
#define MAX30003_REGISTER_CNFG_RTOR1_DETECTION_POS							15U
#define MAX30003_REGISTER_CNFG_RTOR1_DETECTION_DISABLED						(0U << MAX30003_REGISTER_CNFG_RTOR1_DETECTION_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_DETECTION_ENABLED						(1U << MAX30003_REGISTER_CNFG_RTOR1_DETECTION_POS)
/*
R to R Peak Averaging Weight Factor
This sis the weighting factor for the current RTOR peak observation vs. past peak
observations when determining peak thresholds. Lower numbers weight current peaks
more heavily.
00 = 2
01 = 4
10 = 8 (default)
11 = 16
Peak_Average(n) =
[Peak(n) + (RTOR_PAVG-1) x Peak_Average(n-1)] / RTOR_PAVG
Default value: 0x02
 */
#define MAX30003_REGISTER_CNFG_RTOR1_PAVG_POS								12U
#define MAX30003_REGISTER_CNFG_RTOR1_PAVG_MASK								(3U << MAX30003_REGISTER_CNFG_RTOR1_PAVG_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PAVG_2									(0U << MAX30003_REGISTER_CNFG_RTOR1_PAVG_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PAVG_4									(1U << MAX30003_REGISTER_CNFG_RTOR1_PAVG_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PAVG_8									(2U << MAX30003_REGISTER_CNFG_RTOR1_PAVG_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PAVG_16								(3U << MAX30003_REGISTER_CNFG_RTOR1_PAVG_POS)
/*
R to R Peak Threshold Scaling Factor
This is the fraction of the Peak Average value used in the Threshold computation.
Values of 1/16 to 16/16 are selected directly by (RTOR_PTSF[3:0]+1)/16, default is 4/16.
Default value: 0x03
 */
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS								8U
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_MASK								(0x0FU << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_1									(0x00U << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_2									(0x01U << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_3									(0x02U << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_4									(0x03U << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_5									(0x04U << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_6									(0x05U << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_7									(0x06U << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_8									(0x07U << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_9									(0x08U << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_10								(0x09U << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_11								(0x0AU << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_12								(0x0BU << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_13								(0x0CU << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_14								(0x0DU << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_15								(0x0EU << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_PTSF_16								(0x0FU << MAX30003_REGISTER_CNFG_RTOR1_PTSF_POS)

/*
 * CNFG_RTOR1 register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
R to R Interval Averaging Weight Factor
This is the weighting factor for the current RtoR interval observation vs. the past interval
observations when determining dynamic holdoff criteria. Lower numbers weight current
intervals more heavily.
00 = 2
01 = 4
10 = 8 (default)
11 = 16
Interval_Average(n) = [Interval(n) + (RAVG-1) x
Interval_Average(n-1)] / RAVG.
Default value: 0x02
 */
#define MAX30003_REGISTER_CNFG_RTOR1_RAVG_POS								12U
#define MAX30003_REGISTER_CNFG_RTOR1_RAVG_MASK								(3U << MAX30003_REGISTER_CNFG_RTOR1_RAVG_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RAVG_2									(0U << MAX30003_REGISTER_CNFG_RTOR1_RAVG_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RAVG_4									(1U << MAX30003_REGISTER_CNFG_RTOR1_RAVG_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RAVG_8									(2U << MAX30003_REGISTER_CNFG_RTOR1_RAVG_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RAVG_16								(3U << MAX30003_REGISTER_CNFG_RTOR1_RAVG_POS)
/*
R to R Interval Hold Off Scaling Factor
This is the fraction of the RtoR average interval used for the dynamic portion of the holdoff
criteria (tHOLD_OFFDYN).
Values of 0/8 to 7/8 are selected directly by RTOR_RHSF[3:0]/8, default is 4/8.
If 000 (0/8) is selected, then no dynamic factor is used and the holdoff criteria is
determined by HOFF[5:0] only (see above).
Default value: 0x04
 */
#define MAX30003_REGISTER_CNFG_RTOR1_RHSF_POS								8U
#define MAX30003_REGISTER_CNFG_RTOR1_RHSF_MASK								(7U << MAX30003_REGISTER_CNFG_RTOR1_RHSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RHSF_0									(0U << MAX30003_REGISTER_CNFG_RTOR1_RHSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RHSF_1									(1U << MAX30003_REGISTER_CNFG_RTOR1_RHSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RHSF_2									(2U << MAX30003_REGISTER_CNFG_RTOR1_RHSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RHSF_3									(3U << MAX30003_REGISTER_CNFG_RTOR1_RHSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RHSF_4									(4U << MAX30003_REGISTER_CNFG_RTOR1_RHSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RHSF_5									(5U << MAX30003_REGISTER_CNFG_RTOR1_RHSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RHSF_6									(6U << MAX30003_REGISTER_CNFG_RTOR1_RHSF_POS)
#define MAX30003_REGISTER_CNFG_RTOR1_RHSF_7									(7U << MAX30003_REGISTER_CNFG_RTOR1_RHSF_POS)
/*
 * CNFG_RTOR2 register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
R to R Minimum Hold Off
This sets the absolute minimum interval used for the static portion of the Hold Off criteria.
Values of 0 to 63 are supported, default is 32
tHOLD_OFF_MIN = HOFF[5:0] * tRTOR, where tRTOR is ~8ms, as determined by
FMSTR[1:0] in the CNFG_GEN register.
(representing approximately ¼ second).
The R to R Hold Off qualification interval is
tHold_Off = MAX(tHold_Off_Min, tHold_Off_Dyn) (see below).
Default value: 0x20
 */
#define MAX30003_REGISTER_CNFG_RTOR2_HOFF_POS								16U
#define MAX30003_REGISTER_CNFG_RTOR2_HOFF_MASK								(0x3FU << MAX30003_REGISTER_CNFG_RTOR2_HOFF_POS)
/*
 * CNFG_RTOR2 register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * ECG_FIFO_BURST register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * ECG_FIFO_BURST register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * ECG_FIFO register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
Three bits in the sample record are used as a ECG data
tag (ETAG[2:0] = DO[5:3]). This section outlines the
meaning of the various data tags used in the ECG FIFO
and recommended handling within the continuous ECG
record.
VALID: ETAG = 000 indicates the ECG data for this
sample represents both a valid voltage and time step in
the ECG record.
FAST: ETAG = 001 indicates the ECG data for this
sample was taken in the FAST settling mode and that the
voltage information in the sample should be treated as
transient and invalid. Note that while the voltage data is
invalid, samples of this type do represent valid time steps
in the ECG record.
VALID EOF: ETAG = 010 indicates the ECG data for this
sample represents both a valid voltage and time step in
the ECG record, and that this is the last sample currently
available in the ECG FIFO (End-of-File, EOF). The µC
should wait until further samples are available before
requesting more data from the ECG FIFO.
FAST EOF: ETAG = 011 indicates the ECG data for this
sample was taken in the FAST settling mode and that the
voltage information in the sample should be treated as
transient and invalid. Note that while the voltage data is
invalid, samples of this type do represent valid time steps
in the ECG record. In addition, this is the last sample
currently available in the ECG FIFO (End-of-File, EOF).
The µC should wait until further samples are available
before requesting more data from the ECG FIFO.
EMPTY: ETAG = 110 is appended to any requested read
back data from an empty ECG FIFO. The presence
of this tag alerts the user that this FIFO data does not
represent a valid sample or time step. Note that if
handled properly by the µC, an occurrence of an empty
tag will not compromise the integrity of a continuous ECG
record – this tag only indicates that the read back request
was either premature or unnecessary.
OVERFLOW: ETAG = 111 indicates that the ECG FIFO
has overflowed and that there are interruptions or missing
data in the sample records. The ECG Overflow (EOVF) bit
is also included in the STATUS register. A FIFO_RESET
is required to resolve this situation, effectively clearing
the FIFO so that valid sampling going forward is assured.
Depending on the application, it may also be necessary to
resynchronize the MAX30003 internal channel operations
to move forward with valid recordings, the SYNCH
command can perform this function while also resetting
the FIFO memories
 */
#define MAX30003_REGISTER_ECG_FIFO_ETAG_POS									3U
#define MAX30003_REGISTER_ECG_FIFO_ETAG_MASK								(7U << MAX30003_REGISTER_ECG_FIFO_ETAG_POS)
#define MAX30003_REGISTER_ECG_FIFO_ETAG_VALID								(0U << MAX30003_REGISTER_ECG_FIFO_ETAG_POS)
#define MAX30003_REGISTER_ECG_FIFO_ETAG_FAST_MODE							(1U << MAX30003_REGISTER_ECG_FIFO_ETAG_POS)
#define MAX30003_REGISTER_ECG_FIFO_ETAG_LAST_VALID							(2U << MAX30003_REGISTER_ECG_FIFO_ETAG_POS)
#define MAX30003_REGISTER_ECG_FIFO_ETAG_LAST_FAST_MODE						(3U << MAX30003_REGISTER_ECG_FIFO_ETAG_POS)
#define MAX30003_REGISTER_ECG_FIFO_ETAG_FIFO_EMPTY							(6U << MAX30003_REGISTER_ECG_FIFO_ETAG_POS)
#define MAX30003_REGISTER_ECG_FIFO_ETAG_FIFO_OVERFLOW						(7U << MAX30003_REGISTER_ECG_FIFO_ETAG_POS)

#define MAX30003_REGISTER_ECG_FIFO_PTAG_POS									0U
#define MAX30003_REGISTER_ECG_FIFO_PTAG_MASK								(7U << MAX30003_REGISTER_ECG_FIFO_PTAG_POS)

/*
 * ECG_FIFO register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * RTOR register section begin
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/*
 * RTOR register section end
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#define MAX30003_STATE_IDLE													0U
#define MAX30003_STATE_WRITE_CONFIGURATION									1U
#define MAX30003_STATE_READ_CONFIGURATION									2U
/*
 * Note: due to internal initialization procedures, this
command will not read-back valid data if it is the first
command executed following either a power-cycle event,
or a SW_RST event.
 */
#define MAX30003_STATE_READING_REVISION										3U
#define MAX30003_STATE_READING_STATUS										4U
#define MAX30003_STATE_READING_ECG											5U
#define MAX30003_STATE_ERROR												6U
#define MAX30003_STATE_RESET												7U
#define MAX30003_STATE_WAITING_FOR_ECG_INTERRUPT							8U
#define MAX30003_STATE_WAITING_FOR_PLL_INTERRUPT							13U
#define MAX30003_STATE_WAITING_FOR_TICK										14U
#define MAX30003_STATE_SYNC													9U
#define MAX30003_STATE_RESET_FIFO											10U
#define MAX30003_STATE_CHANGE_MODE											11U
#define MAX30003_STATE_STOPPED												12U

#define MAX30003_ERROR_NONE													0U
#define MAX30003_ERROR_COMMUNICATION_FAILED									1U

#define MAX30003_MODE_ECG													0U
#define MAX30003_MODE_CALIBRATION											1U

typedef struct {
	int32_t sample;
} ECGData_t;

typedef void (*MAX30003EnableInterruptFunction)();
typedef void (*MAX30003DisableInterruptFunction)();
typedef uint8_t (*MAX30003ReadInterruptStateFunction)();
typedef void (*MAX30003ECGDataCallback_t)(ECGData_t* ecgEvent);

typedef struct {
	SPI_t* spi;
	MAX30003ReadInterruptStateFunction readInterruptStateFunction;
	MAX30003EnableInterruptFunction enableInterruptFunction;
	MAX30003DisableInterruptFunction disableInterruptFunction;
	MAX30003ECGDataCallback_t ecgDataCallback;
} MAX30003Settings_t;


typedef struct {
	uint8_t currentState;
	uint8_t isReadStarted;
	uint8_t isInitialCommandSent;
	uint8_t txBuffer[MAX30003_CONFIGURATION_BUFFER_LENGTH];
	uint8_t rxBuffer[MAX30003_CONFIGURATION_BUFFER_LENGTH];
	MAX30003Settings_t* settings;
	uint8_t revisionID;
	uint8_t error;
	uint8_t mode;
	uint8_t isFastReading;
} MAX30003Device_t;

void max30003Initilize(MAX30003Device_t* device);
void max30003Start(MAX30003Device_t* device);
void max30003Stop(MAX30003Device_t* device);
void max30003SetMode(MAX30003Device_t* device, uint8_t mode);
uint8_t max30003GetMode(MAX30003Device_t* device);
void max30003DeInitilize(MAX30003Device_t* device);
void max30003InterruptTopHalfHandler(MAX30003Device_t* device);
void max30003InterruptBottomHalfHandler(MAX30003Device_t* device);
void max30003Tick(MAX30003Device_t* device);

#endif /* INC_DEVICES_ECG_MAX30003_H_ */
