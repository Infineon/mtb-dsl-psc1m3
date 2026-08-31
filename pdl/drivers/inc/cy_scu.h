/**
 * @file cy_scu.h
 *
 * @cond
 *****************************************************************************
 * \copyright
 * (c) 2016-2026, Infineon Technologies AG or an affiliate of
 * Infineon Technologies AG.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************
 * @endcond
 *
 */
#ifndef CY_SCU_H
#define CY_SCU_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_common.h"


/**
 * @addtogroup SCU
 * @brief System Control Unit(SCU) driver for PSC1M microcontroller family.
 *
 * System control unit is the SoC power, reset and a clock manager with additional responsibility of
 * providing system stability protection and other auxiliary functions.<br>
 * SCU provides the following features,
 * -# Power control
 * -# Reset control
 * -# Clock control
 * -# Miscellaneous control(boot mode, system interrupts etc.)<br><br>
 *
 * The SCU driver is divided in to clock control logic, reset control logic, system interrupt control logic
 * and miscellaneous control logic.<br>
 *
 * Clock driver features:
 * -# Allows clock configuration using the structure Cy_SCU_CLOCK_CONFIG_t and API Cy_SCU_CLOCK_Init()
 * -# Allows selection of peripheral clock frequency, Cy_SCU_CLOCK_SetFastPeripheralClockSource()
 * -# Provides API to get the peripheral clock frequency, Cy_SCU_CLOCK_GetFastPeripheralClockFrequency()
 *
 * Reset driver features:
 * -# Allows to trigger device reset Cy_SCU_RESET_AssertMasterReset()
 * -# Allows to configure multiple sources for reset, Cy_SCU_RESET_EnableResetRequest()<br>
 *
 * Interrupt driver features:
 * -# Provides APIs for enabling/ disabling interrupt event generation Cy_SCU_INTERRUPT_EnableEvent(),
 Cy_SCU_INTERRUPT_DisableEvent()
 * -# Provides API for registering callback function for events Cy_SCU_INTERRUPT_SetEventHandler()<br>
 *
 * Miscellaneous features:
 * -# Allows to trigger multiple capture compare unit(CCU) channels to be started together Cy_SCU_SetCcuTriggerHigh()
 * -# Enables configuration of die temperature sensor Cy_SCU_StartTempMeasurement(), Cy_SCU_SetRawTempLimits()
 * -# Allows configuring supply monitor unit using the structure Cy_SCU_SUPPLYMONITOR_t and API Cy_SCU_SupplyMonitorInit()
 * -# Allows handling of protected bits Cy_SCU_LockProtectedBits(), Cy_SCU_UnlockProtectedBits()<br>
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

/*
 * List of events
 */
#define CY_SCU_INTERRUPT_EVENT_WDT_WARN         SCU_INTERRUPT_SRMSK_PRWARN_Msk /**< WDT pre-warning event. */
#define CY_SCU_INTERRUPT_EVENT_RTC_PERIODIC     SCU_INTERRUPT_SRCLR_PI_Msk     /**< RTC periodic event. */
#define CY_SCU_INTERRUPT_EVENT_RTC_ALARM        SCU_INTERRUPT_SRCLR_AI_Msk     /**< RTC alarm event. */
#define CY_SCU_INTERRUPT_EVENT_VDDPI            SCU_INTERRUPT_SRMSK_VDDPI_Msk  /**< VDDP pre-warning event. */
#if defined(COMPARATOR)
#define CY_SCU_INTERRUPT_EVENT_ACMP0            SCU_INTERRUPT_SRMSK_ACMP0I_Msk /**< Analog comparator-0 output event. */
#define CY_SCU_INTERRUPT_EVENT_ACMP1            SCU_INTERRUPT_SRMSK_ACMP1I_Msk /**< Analog comparator-1 output event. */
#define CY_SCU_INTERRUPT_EVENT_ACMP2            SCU_INTERRUPT_SRMSK_ACMP2I_Msk /**< Analog comparator-2 output event. */
#define CY_SCU_INTERRUPT_EVENT_ACMP3            (((int64_t)SCU_INTERRUPT_SRMSK1_ACMP3I_Msk) << 32U) /**< Analog comparator-3 output event. */
#endif
#define CY_SCU_INTERRUPT_EVENT_VDROP            SCU_INTERRUPT_SRMSK_VDROPI_Msk  /**< VDROP event. */
#define CY_SCU_INTERRUPT_EVENT_ORC0             SCU_INTERRUPT_SRMSK_ORC0I_Msk   /**< Out of range comparator-0 event. */
#define CY_SCU_INTERRUPT_EVENT_ORC1             SCU_INTERRUPT_SRMSK_ORC1I_Msk   /**< Out of range comparator-1 event. */
#define CY_SCU_INTERRUPT_EVENT_ORC2             SCU_INTERRUPT_SRMSK_ORC2I_Msk   /**< Out of range comparator-2 event. */
#define CY_SCU_INTERRUPT_EVENT_ORC3             SCU_INTERRUPT_SRMSK_ORC3I_Msk   /**< Out of range comparator-3 event. */
#define CY_SCU_INTERRUPT_EVENT_ORC4             SCU_INTERRUPT_SRMSK_ORC4I_Msk   /**< Out of range comparator-4 event. */
#define CY_SCU_INTERRUPT_EVENT_ORC5             SCU_INTERRUPT_SRMSK_ORC5I_Msk   /**< Out of range comparator-5 event. */
#define CY_SCU_INTERRUPT_EVENT_ORC6             SCU_INTERRUPT_SRMSK_ORC6I_Msk   /**< Out of range comparator-6 event. */
#define CY_SCU_INTERRUPT_EVENT_ORC7             SCU_INTERRUPT_SRMSK_ORC7I_Msk   /**< Out of range comparator-7 event. */
#define CY_SCU_INTERRUPT_EVENT_LOCI             SCU_INTERRUPT_SRMSK_LOCI_Msk    /**< Loss of clock event. */
#define CY_SCU_INTERRUPT_EVENT_PESRAM           SCU_INTERRUPT_SRMSK_PESRAMI_Msk /**< PSRAM Parity error event. */
#define CY_SCU_INTERRUPT_EVENT_PEUSIC0          SCU_INTERRUPT_SRMSK_PEU0I_Msk   /**< USIC0 Parity error event. */
#if defined(USIC1)
#define CY_SCU_INTERRUPT_EVENT_PEUSIC1          (((int64_t)SCU_INTERRUPT_SRMSK1_PEU1I_Msk) << 32U)   /**< USIC1 Parity error event. */
#endif
#define CY_SCU_INTERRUPT_EVENT_FLASH_ERROR      SCU_INTERRUPT_SRMSK_FLECC2I_Msk /**< Flash ECC double bit error event. */
#define CY_SCU_INTERRUPT_EVENT_FLASH_COMPLETED  SCU_INTERRUPT_SRCLR_FLCMPLTI_Msk /**< Flash operation completion event. */
#define CY_SCU_INTERRUPT_EVENT_VCLIP            SCU_INTERRUPT_SRMSK_VCLIPI_Msk  /**< VCLIP event. */
#define CY_SCU_INTERRUPT_EVENT_STDBYCLKFAIL     SCU_INTERRUPT_SRMSK_SBYCLKFI_Msk  /**< Standby clock failure event. */
#define CY_SCU_INTERRUPT_EVENT_RTCCTR_UPDATED   SCU_INTERRUPT_SRMSK_RTC_CTR_Msk  /**< RTCCTR register update event. */
#define CY_SCU_INTERRUPT_EVENT_RTCATIM0_UPDATED SCU_INTERRUPT_SRMSK_RTC_ATIM0_Msk  /**< RTCATIM0 register update event. */
#define CY_SCU_INTERRUPT_EVENT_RTCATIM1_UPDATED SCU_INTERRUPT_SRMSK_RTC_ATIM1_Msk  /**< RTCATIM1 register update event. */
#define CY_SCU_INTERRUPT_EVENT_RTCTIM0_UPDATED  SCU_INTERRUPT_SRMSK_RTC_TIM0_Msk  /**< RTCTIM0 register update event. */
#define CY_SCU_INTERRUPT_EVENT_RTCTIM1_UPDATED  SCU_INTERRUPT_SRMSK_RTC_TIM1_Msk  /**< RTCTIM1 register update event. */
#define CY_SCU_INTERRUPT_EVENT_TSE_DONE         SCU_INTERRUPT_SRMSK_TSE_DONE_Msk  /**< Temperature measurement Completion event. */
#define CY_SCU_INTERRUPT_EVENT_TSE_HIGH         SCU_INTERRUPT_SRMSK_TSE_HIGH_Msk  /**< Temperature too high event. */
#define CY_SCU_INTERRUPT_EVENT_TSE_LOW          SCU_INTERRUPT_SRMSK_TSE_LOW_Msk /**< Temperature too low event. */
#if defined(CAN) || defined(CY_DOXYGEN)
#define CY_SCU_INTERRUPT_EVENT_PEMCAN           (((int64_t)SCU_INTERRUPT_SRMSK1_PEMCI_Msk) << 32U)   /**< MultiCAN SRAM Parity Error Event. */
#endif
#define CY_SCU_INTERRUPT_EVENT_LOSS_EXT_CLOCK   (((int64_t)SCU_INTERRUPT_SRMSK1_LOECI_Msk) << 32U)  /**< Loss of external OSC_HP clock event. */
#define CY_SCU_INTERRUPT_EVENT_DCO1_OUT_SYNC    (((int64_t)SCU_INTERRUPT_SRMSK1_DCO1OFSI_Msk) << 32U)   /**< DCO1 Out of SYNC Event. */

/*
 *  These are the flags which may be passed to Cy_SCU_SetBMI().
 */
#if defined(CAN) || defined(CY_DOXYGEN)
#define CY_SCU_BMI_HWCFG_CAN_BSL      (0x0000U) /**< CAN Bootstrap Loader Start-up Mode */
#define CY_SCU_BMI_HWCFG_CAN_BSLTO    (0x0010U) /**< CAN Bootstrap Loader Start-up Mode with time-out */
#define CY_SCU_BMI_HWCFG_SBSL_CANOPEN (0x0020U) /**< Secure Bootstrap Loader Start-up Mode over CANopen */
#endif

#define CY_SCU_BMI_HWCFG_ASC_BSL      (0x0040U) /**< ASC Bootstrap Loader Start-up Mode */
#define CY_SCU_BMI_HWCFG_UPM          (0x0041U) /**< User productive Start-up Mode */
#define CY_SCU_BMI_HWCFG_UMD          (0x0043U) /**< User Start-up Mode with debug enabled */
#define CY_SCU_BMI_HWCFG_UMHAR        (0x0047U) /**< User Start-up Mode with debug enabled and halt after reset (HAR) */
#define CY_SCU_BMI_HWCFG_SSC_BSL      (0x0048U) /**< SSC Bootstrap Loader Start-up Mode */
#define CY_SCU_BMI_HWCFG_ASC_BSLTO    (0x0050U) /**< ASC BSL Start-up Mode with time-out */
#define CY_SCU_BMI_HWCFG_SSC_BSLTO    (0x0058U) /**< SSC BSL Start-up Mode with time-out */
#define CY_SCU_BMI_HWCFG_SBSL         (0x007AU) /**< Secure Bootstrap Loader Start-up Mode over ASC */

#define CY_SCU_BMI_HWCFG_PINDIS       (0x0080U) /**< Boot Configuration Type Selection, Boot from BMI is selected */

#define CY_SCU_BMI_DAPTYP_SWD         (0U << 8) /**< Serial wire debug (SWD) interface is selected */
#define CY_SCU_BMI_DAPTYP_SPD         (1U << 8) /**< Single pin debug (SPD) interface is selected */

#define CY_SCU_BMI_DAPDIS_CHANNEL_0   (0U << 9) /**< SWD/SPD_0 pin is selected */
#define CY_SCU_BMI_DAPDIS_CHANNEL_1   (1U << 9) /**< SWD/SPD_1 pin is selected */

#if defined(CAN) || defined(CY_DOXYGEN)
#define CY_SCU_BMI_CANCLK_DCO1        (0U << 11) /**< Synchronous CAN clock via internal oscillator (DCO1) with enabled trimming via external reference is selected */
#define CY_SCU_BMI_CANCLK_OSCHP       (1U << 11) /**< Synchronous CAN clock via external oscillator (OSC_HP) is selected */
#endif

#define CY_BMI_ADDR                   (0x10000e00U) /**< Boot Mode Index (BMI) address holding information about start-up mode and debug configuration of the device. */


/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
/**
 *  Defines the status of SCU API execution, used to verify the SCU related API calls.
 */
typedef enum CY_SCU_STATUS
{
  CY_SCU_STATUS_OK   = 0UL, /**< SCU related operation successfully completed.*/
  CY_SCU_STATUS_ERROR,      /**< SCU related operation failed. When API cannot fulfill request, this value is returned. */
  CY_SCU_STATUS_BUSY,       /**< Cannot execute the SCU related operation request because
                                  another operation is in progress. \a CY_SCU_STATUS_BUSY is returned when API is busy
                                  processing another request. */
} Cy_SCU_STATUS_t;

/**
 *  Defines the cause of last reset. The cause of last reset gets automatically stored in
 *  the \a SCU_RSTSTAT register and can be checked by user software to determine the state of the system and for debug
 *  purpose. All the enum items are tabulated as per bits present in \a SCU_RSTSTAT register.
 *  Use type \a Cy_SCU_RESET_REASON_t for accessing these enum parameters.
 */
typedef enum CY_SCU_RESET_REASON
{
  CY_SCU_RESET_REASON_PORST        = (1UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos),  /**< Reset due to Power On reset. */
  CY_SCU_RESET_REASON_MASTER       = (2UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos),  /**< Reset due to Master reset. */
  CY_SCU_RESET_REASON_SW           = (4UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos),  /**< Reset due to Software initiated reset. */
  CY_SCU_RESET_REASON_LOCKUP       = (8UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos),  /**< Reset due to CPU lockup. */
  CY_SCU_RESET_REASON_FLASH        = (16UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos), /**< Reset due to flash error. */
  CY_SCU_RESET_REASON_WATCHDOG     = (32UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos), /**< Reset due to watchdog. */
  CY_SCU_RESET_REASON_CLOCK_LOSS   = (64UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos), /**< Reset due to clock loss. */
  CY_SCU_RESET_REASON_PARITY_ERROR = (128UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos) /**< Reset due to RAM parity error. */
} Cy_SCU_RESET_REASON_t;

/**
 *  Defines the reset sources that can cause device reset. These enums can be used to configure reset source in reset
 *  control \a RSTCON register which enables different reset sources to identify the reset cause. The \a SCU_RSTSTAT
 *  register can be checked by user software to determine the state of the system and for debug
 *  purpose. Use type \a Cy_SCU_SYSTEM_RESET_REQUEST_t for accessing these enum parameters.
 */
typedef enum CY_SCU_SYSTEM_RESET_REQUEST
{
  CY_SCU_RESET_REQUEST_FLASH_ECC_ERROR = SCU_RESET_RSTCON_ECCRSTEN_Msk,        /**< Reset when ECC double bit error occurs.*/
  CY_SCU_RESET_REQUEST_CLOCK_LOSS = SCU_RESET_RSTCON_LOCRSTEN_Msk,             /**< Reset when loss of clock occurs.*/
  CY_SCU_RESET_REQUEST_SRAM_PARITY_ERROR = SCU_RESET_RSTCON_SPERSTEN_Msk,      /**< Reset when SRAM parity error occurs.*/
  CY_SCU_RESET_REQUEST_USIC_SRAM_PARITY_ERROR = SCU_RESET_RSTCON_U0PERSTEN_Msk /**< Reset when USIC0 memory parity error occurs.*/
} Cy_SCU_SYSTEM_RESET_REQUEST_t;

/**
 *  Defines list of events that can generate SCU interrupt. These enums can be used to configure events
 *  in \a SRMSK register for assertion of interrupt. All the enum items are tabulated as per bits present
 *  in \a SRMSK register. Use type \a Cy_SCU_INTERRUPT_EVENT_t for accessing these enum parameters. These
 *  enums can also be used for checking the status of events from the \a SRSTAT register.
 */
typedef uint64_t Cy_SCU_INTERRUPT_EVENT_t;

/**
 *  Defines possible sources of RTC clock. These enums can be used to configure \a RTCCLKSEL bits of \a CLKCR Clock Control
 *  Register. Use type \a Cy_SCU_CLOCK_RTCCLKSRC_t for accessing these enum parameters.
 */
typedef enum CY_SCU_CLOCK_RTCCLKSRC
{
  CY_SCU_CLOCK_RTCCLKSRC_DCO2 = (0x0UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos), /**< RTC clock source is standby clock. */
  CY_SCU_CLOCK_RTCCLKSRC_ERU_IOUT0 = (0x1UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos), /**< RTC clock source is external clock from
                                                                                   ERU0.IOUT0. */
  CY_SCU_CLOCK_RTCCLKSRC_ACMP0_OUT = (0x2UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos), /**< RTC clock source is external clock from
                                                                                   ACMP0.OUT. */
  CY_SCU_CLOCK_RTCCLKSRC_ACMP1_OUT = (0x3UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos), /**< RTC clock source is external clock from
                                                                                  ACMP1.OUT. */
  CY_SCU_CLOCK_RTCCLKSRC_ACMP2_OUT = (0x4UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos), /**< RTC clock source is external clock from
                                                                                  ACMP2.OUT. */
  CY_SCU_CLOCK_RTCCLKSRC_OSCLP = (0x5UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos) /**< 32.768kHz XTAL clock via OSC_LP. */
} Cy_SCU_CLOCK_RTCCLKSRC_t;

/**
 *  Defines possible sources of peripheral clock (PCLK). These enums can be used to configure \a PCLKSEL bits of \a CLKCR
 *  Clock Control Register. Use type \a Cy_SCU_CLOCK_PCLKSRC_t for accessing these enum parameters.
 */
typedef enum CY_SCU_CLOCK_PCLKSRC
{
  CY_SCU_CLOCK_PCLKSRC_MCLK = (0UL << SCU_CLK_CLKCR_PCLKSEL_Pos), /**< MCLK as the source for PCLK. */
  CY_SCU_CLOCK_PCLKSRC_DOUBLE_MCLK = (1UL << SCU_CLK_CLKCR_PCLKSEL_Pos) /**< Source of PCLK is twice the MCLK. */
} Cy_SCU_CLOCK_PCLKSRC_t;

/**
 * DCLK clock source selection
 */
typedef enum CY_SCU_CLOCK_DCLKSRC
{
  CY_SCU_CLOCK_DCLKSRC_DCO1 = 0UL << SCU_CLK_CLKCR1_DCLKSEL_Pos, /**< Internal oscillator DCO1 (48MHz) */
  CY_SCU_CLOCK_DCLKSRC_EXT_XTAL = 1UL << SCU_CLK_CLKCR1_DCLKSEL_Pos, /**< External crystal oscillator */
} Cy_SCU_CLOCK_DCLKSRC_t;

/**
 * ADC clock source selection
 */
typedef enum CY_SCU_CLOCK_ADCCLKSRC
{
  CY_SCU_CLOCK_ADCCLKSRC_48MHZ = 0UL << SCU_CLK_CLKCR1_ADCCLKSEL_Pos, /**< Internal oscillator DCO1 (48MHz) */
  CY_SCU_CLOCK_ADCCLKSRC_32MHZ = 1UL << SCU_CLK_CLKCR1_ADCCLKSEL_Pos, /**< Internal oscillator DCO1 (48MHz) divided by 1.5 */
} Cy_SCU_CLOCK_ADCCLKSRC_t;

/**
 * OSCHP mode
 */
typedef enum CY_SCU_CLOCK_OSCHP_MODE
{
  CY_SCU_CLOCK_OSCHP_MODE_OSC = 0UL << SCU_ANALOG_ANAOSCHPCTRL_MODE_Pos, /**< Oscillator is enabled and in active power mode with shaper enabled */
  CY_SCU_CLOCK_OSCHP_MODE_DIRECT = 1UL << SCU_ANALOG_ANAOSCHPCTRL_MODE_Pos, /**< Oscillator in power down mode with shaper enabled */
  CY_SCU_CLOCK_OSCHP_MODE_DISABLED = 3UL << SCU_ANALOG_ANAOSCHPCTRL_MODE_Pos, /**< Oscillator in power down mode with shaper enabled */
} Cy_SCU_CLOCK_OSCHP_MODE_t;

/**
 * OSCLP mode
 */
typedef enum CY_SCU_CLOCK_OSCLP_MODE
{
  CY_SCU_CLOCK_OSCLP_MODE_OSC = 0UL << SCU_ANALOG_ANAOSCLPCTRL_MODE_Pos, /**< Oscillator is enabled and in active power mode with shaper enabled */
  CY_SCU_CLOCK_OSCLP_MODE_DISABLED = 3UL << SCU_ANALOG_ANAOSCLPCTRL_MODE_Pos, /**< Oscillator in power down mode with shaper enabled */
} Cy_SCU_CLOCK_OSCLP_MODE_t;

/**
 * Clock source for synchronization
 */
typedef enum CY_SCU_CLOCK_SYNC_CLKSRC
{
  CY_SCU_CLOCK_SYNC_CLKSRC_OSCLP = 0U << SCU_ANALOG_ANASYNC1_XTAL_SEL_Pos,
  CY_SCU_CLOCK_SYNC_CLKSRC_OSCHP = 1U << SCU_ANALOG_ANASYNC1_XTAL_SEL_Pos,
} Cy_SCU_CLOCK_SYNC_CLKSRC_t;

/**
 *  Defines the list of peripherals that support clock gating. After a master reset, only core, memories, SCU
 *  and PORT peripheral are not clock gated. The rest of the peripherals are by default clock gated.
 *  All the enum items are tabulated as per bits present in \a CGATSTAT0 register.
 *  Use type \a Cy_SCU_PERIPHERAL_CLOCK_t for accessing these enum parameters.
 *  @note Peripherals availability depends on device
 */
typedef enum CY_SCU_PERIPHERAL_CLOCK
{
#if defined(VADC)
  CY_SCU_PERIPHERAL_CLOCK_VADC   = SCU_CLK_CGATSTAT0_VADC_Msk, /**< VADC peripheral clock gate. */
#endif
#if defined(CCU80)
  CY_SCU_PERIPHERAL_CLOCK_CCU80  = SCU_CLK_CGATSTAT0_CCU80_Msk, /**< CCU80 peripheral clock gate. */
#endif
#if defined(CCU40)
  CY_SCU_PERIPHERAL_CLOCK_CCU40  = SCU_CLK_CGATSTAT0_CCU40_Msk, /**< CCU40 peripheral clock gate. */
#endif
#if defined(USIC0)
  CY_SCU_PERIPHERAL_CLOCK_USIC0  = SCU_CLK_CGATSTAT0_USIC0_Msk, /**< USIC0 peripheral clock gate. */
#endif
#if defined(BCCU0)
  CY_SCU_PERIPHERAL_CLOCK_BCCU0  = SCU_CLK_CGATSTAT0_BCCU0_Msk, /**< BCCU0 peripheral clock gate. */
#endif
#if defined(POSIF0)
  CY_SCU_PERIPHERAL_CLOCK_POSIF0 = SCU_CLK_CGATSTAT0_POSIF0_Msk, /**< POSIF0 peripheral clock gate. */
#endif
#if defined(MATH)
  CY_SCU_PERIPHERAL_CLOCK_MATH   = SCU_CLK_CGATSTAT0_MATH_Msk, /**< MATH peripheral clock gate. */
#endif
  CY_SCU_PERIPHERAL_CLOCK_WDT    = SCU_CLK_CGATSTAT0_WDT_Msk, /**< WDT peripheral clock gate. */
  CY_SCU_PERIPHERAL_CLOCK_RTC    = SCU_CLK_CGATSTAT0_RTC_Msk, /**< RTC peripheral clock gate. */
#if defined(CCU81)
  CY_SCU_PERIPHERAL_CLOCK_CCU81  = SCU_CLK_CGATSTAT0_CCU81_Msk, /**< CCU80 peripheral clock gate. */
#endif
#if defined(CCU41)
  CY_SCU_PERIPHERAL_CLOCK_CCU41  = SCU_CLK_CGATSTAT0_CCU41_Msk, /**< CCU80 peripheral clock gate. */
#endif
#if defined(USIC1)
  CY_SCU_PERIPHERAL_CLOCK_USIC1  = SCU_CLK_CGATSTAT0_USIC1_Msk, /**< USIC0 peripheral clock gate. */
#endif
#if defined(POSIF1)
  CY_SCU_PERIPHERAL_CLOCK_POSIF1 = SCU_CLK_CGATSTAT0_POSIF1_Msk, /**< POSIF0 peripheral clock gate. */
#endif
#if defined(CAN)
  CY_SCU_PERIPHERAL_CLOCK_MCAN = SCU_CLK_CGATSTAT0_MCAN0_Msk, /**< POSIF0 peripheral clock gate. */
#endif
} Cy_SCU_PERIPHERAL_CLOCK_t;


/**
 *  Defines options for Capture/Compare unit timer slice trigger that enables synchronous start function available on the \a SCU,
 *  \a CCUCON register. Use type \a Cy_SCU_CCU_TRIGGER_t for accessing these enum parameters.
 */
typedef enum CY_SCU_CCU_TRIGGER
{
  CY_SCU_CCU_TRIGGER_CCU40 = SCU_GENERAL_CCUCON_GSC40_Msk, /**< Trigger CCU40 peripheral. */
#if defined(CCU80)
  CY_SCU_CCU_TRIGGER_CCU80 = SCU_GENERAL_CCUCON_GSC80_Msk, /**< Trigger CCU80 peripheral. */
#endif
#if defined(CCU41)
  CY_SCU_CCU_TRIGGER_CCU41 = SCU_GENERAL_CCUCON_GSC41_Msk, /**< Trigger CCU40 peripheral. */
#endif
#if defined(CCU81)
  CY_SCU_CCU_TRIGGER_CCU81 = SCU_GENERAL_CCUCON_GSC81_Msk, /**< Trigger CCU80 peripheral. */
#endif
} Cy_SCU_CCU_TRIGGER_t;

/**
 * Selects the service request connected to the interrupt node.
 * @image html "psc1_irqmux.png"
 */
typedef enum CY_SCU_IRQCTRL
{
  CY_SCU_IRQCTRL_SCU_SR0_IRQ0              = (0U << 8U) | 0U, /**< SCU_SR0 connected to IRQ0 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR0_IRQ0             = (0U << 8U) | 1U, /**< CAN0_SR0 connected to IRQ0 */
#endif
  CY_SCU_IRQCTRL_CCU40_SR0_IRQ0            = (0U << 8U) | 2U, /**< CCU40_SR0 connected to IRQ0 */
#if defined(CAN)
  CY_SCU_IRQCTRL_SCU_SR0_OR_CAN0_SR0_IRQ0  = (0U << 8U) | 3U, /**< SCU_SR0 and CAN_SR0 are both connected to IRQ0*/
#endif

  CY_SCU_IRQCTRL_SCU_SR1_IRQ1              = (1U << 8U) | 0U, /**< SCU_SR1 connected to IRQ1 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR1_IRQ1             = (1U << 8U) | 1U, /**< CAN0_SR1 connected to IRQ1 */
#endif
#if defined(CCU80)
  CY_SCU_IRQCTRL_CCU80_SR0_IRQ1            = (1U << 8U) | 2U, /**< CCU80_SR0 connected to IRQ1 */
#endif
#if defined(CAN)
  CY_SCU_IRQCTRL_SCU_SR1_OR_CAN0_SR1_IRQ1  = (1U << 8U) | 3U, /**< SCU_SR1 and CAN0_SR1 connected to IRQ1 */
#endif

  CY_SCU_IRQCTRL_SCU_SR2_IRQ2              = (2U << 8U) | 0U, /**< SCU_SR2 connected to IRQ2 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR2_IRQ2             = (2U << 8U) | 1U, /**< CAN0_SR2 connected to IRQ2 */
#endif
#if defined(CCU80)
  CY_SCU_IRQCTRL_CCU80_SR1_IRQ2            = (2U << 8U) | 2U, /**< CCU80_SR1 connected to IRQ2 */
#endif
#if defined(CAN)
  CY_SCU_IRQCTRL_SCU_SR2_OR_CAN0_SR2_IRQ2  = (2U << 8U) | 3U, /**< SCU_SR2 and CAN0_SR2 connected to IRQ2 */
#endif

  CY_SCU_IRQCTRL_ERU0_SR0_IRQ3             = (3U << 8U) | 0U, /**< ERU0_SR0 connected to IRQ3 */
  CY_SCU_IRQCTRL_ERU1_SR0_IRQ3             = (3U << 8U) | 1U, /**< ERU1_SR0 connected to IRQ3 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR0_IRQ3             = (3U << 8U) | 2U, /**< CAN0_SR0 connected to IRQ3 */
#endif
  CY_SCU_IRQCTRL_ERU0_SR0_OR_ERU1_SR0_IRQ3 = (3U << 8U) | 3U, /**< ERU0_SR0 and ERU1_SR0 connected to IRQ3 */

  CY_SCU_IRQCTRL_ERU0_SR1_IRQ4             = (4U << 8U) | 0U, /**< ERU0_SR1 connected to IRQ4 */
  CY_SCU_IRQCTRL_ERU1_SR1_IRQ4             = (4U << 8U) | 1U, /**< ERU1_SR1 connected to IRQ4 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR1_IRQ4             = (4U << 8U) | 2U, /**< CAN0_SR1 connected to IRQ4 */
#endif
  CY_SCU_IRQCTRL_ERU0_SR1_OR_ERU1_SR1_IRQ4 = (4U << 8U) | 3U, /**< ERU0_SR1 and ERU1_SR1 connected to IRQ4 */

  CY_SCU_IRQCTRL_ERU0_SR2_IRQ5             = (5U << 8U) | 0U, /**< ERU0_SR2 connected to IRQ5 */
  CY_SCU_IRQCTRL_ERU1_SR2_IRQ5             = (5U << 8U) | 1U, /**< ERU1_SR2 connected to IRQ5 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR2_IRQ5             = (5U << 8U) | 2U, /**< CAN0_SR2 connected to IRQ5 */
#endif
  CY_SCU_IRQCTRL_ERU0_SR2_OR_ERU1_SR2_IRQ5 = (5U << 8U) | 3U, /**< ERU0_SR2 and ERU1_SR2 connected to IRQ5 */

  CY_SCU_IRQCTRL_ERU0_SR3_IRQ6             = (6U << 8U) | 0U, /**< ERU0_SR3 connected to IRQ6 */
  CY_SCU_IRQCTRL_ERU1_SR3_IRQ6             = (6U << 8U) | 1U, /**< ERU1_SR3 connected to IRQ6 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR3_IRQ6             = (6U << 8U) | 2U, /**< CAN0_SR3 connected to IRQ6 */
#endif
  CY_SCU_IRQCTRL_ERU0_SR3_OR_ERU1_SR3_IRQ6 = (6U << 8U) | 3U, /**< ERU0_SR3 and ERU1_SR3 connected to IRQ6 */

#if defined(MATH)
  CY_SCU_IRQCTRL_MATH_SR0_IRQ7                = (7U << 8U) | 0U, /**< MATH_SR0 connected to IRQ7 */
#endif
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR3_IRQ7                = (7U << 8U) | 1U, /**< CAN0_SR3 connected to IRQ7 */
#endif
  CY_SCU_IRQCTRL_CCU40_SR1_IRQ7               = (7U << 8U) | 2U, /**< CCU40_SR1 connected to IRQ7 */
#if defined(MATH) && defined(CAN)
  CY_SCU_IRQCTRL_MATH_SR0_OR_CAN0_SR3_IRQ7    = (7U << 8U) | 3U, /**< MATH_SR0 and CAN0_SR3 connected to IRQ7 */
#endif

  CY_SCU_IRQCTRL_CCU40_SR0_IRQ8               = (8U << 8U) | 1U, /**< CCU40_SR0 connected to IRQ8 */
#if defined(CCU80)
  CY_SCU_IRQCTRL_CCU80_SR0_IRQ8               = (8U << 8U) | 2U, /**< CCU80_SR0 connected to IRQ8 */
#endif

  CY_SCU_IRQCTRL_USIC0_SR0_IRQ9               = (9U << 8U) | 0U, /**< USIC0_SR0 connected to IRQ9 */
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC1_SR0_IRQ9               = (9U << 8U) | 1U, /**< USIC1_SR0 connected to IRQ9 */
#endif
  CY_SCU_IRQCTRL_ERU0_SR0_IRQ9                = (9U << 8U) | 2U, /**< ERU0_SR0 connected to IRQ9 */
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC0_SR0_OR_USIC1_SR0_IRQ9  = (9U << 8U) | 3U, /**< USIC0_SR0 and USIC1_SR0 connected to IRQ9 */
#endif

  CY_SCU_IRQCTRL_USIC0_SR1_IRQ10              = (10U << 8U) | 0U, /**< USIC0_SR1 connected to IRQ10 */
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC1_SR1_IRQ10              = (10U << 8U) | 1U, /**< USIC1_SR1 connected to IRQ10 */
#endif
  CY_SCU_IRQCTRL_ERU0_SR1_IRQ10               = (10U << 8U) | 2U, /**< ERU0_SR1 connected to IRQ10 */
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC0_SR1_OR_USIC1_SR1_IRQ10 = (10U << 8U) | 3U, /**< USIC0_SR1 and USIC1_SR1 connected to IRQ10 */
#endif

  CY_SCU_IRQCTRL_USIC0_SR2_IRQ11              = (11U << 8U) | 0U, /**< USIC0_SR2 connected to IRQ11 */
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC1_SR2_IRQ11              = (11U << 8U) | 1U, /**< USIC1_SR2 connected to IRQ11 */
#endif
  CY_SCU_IRQCTRL_ERU0_SR2_IRQ11               = (11U << 8U) | 2U, /**< ERU0_SR2 connected to IRQ11 */
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC0_SR2_OR_USIC1_SR2_IRQ11 = (11U << 8U) | 3U, /**< USIC0_SR2 and USIC1_SR2 connected to IRQ11 */
#endif

  CY_SCU_IRQCTRL_USIC0_SR3_IRQ12              = (12U << 8U) | 0U, /**< USIC0_SR3 connected to IRQ12 */
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC1_SR3_IRQ12              = (12U << 8U) | 1U, /**< USIC1_SR3 connected to IRQ12 */
#endif
  CY_SCU_IRQCTRL_ERU0_SR3_IRQ12               = (12U << 8U) | 2U, /**< ERU0_SR3 connected to IRQ12 */
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC0_SR3_OR_USIC1_SR3_IRQ12 = (12U << 8U) | 3U, /**< USIC0_SR3 and USIC1_SR3 connected to IRQ12 */
#endif

  CY_SCU_IRQCTRL_USIC0_SR4_IRQ13              = (13U << 8U) | 0U, /**< USIC0_SR4 connected to IRQ13 */
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC1_SR4_IRQ13              = (13U << 8U) | 1U, /**< USIC1_SR4 connected to IRQ13 */
#endif
#if defined(CCU80)
  CY_SCU_IRQCTRL_CCU80_SR1_IRQ13              = (13U << 8U) | 2U, /**< CCU80_SR1 connected to IRQ13 */
#endif
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC0_SR4_OR_USIC1_SR4_IRQ13 = (13U << 8U) | 3U, /**< USIC0_SR4 and USIC1_SR4 connected to IRQ13 */
#endif

  CY_SCU_IRQCTRL_USIC0_SR5_IRQ14              = (14U << 8U) | 0U, /**< USIC0_SR5 connected to IRQ14 */
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC1_SR5_IRQ14              = (14U << 8U) | 1U, /**< USIC1_SR5 connected to IRQ14 */
#endif
#if defined(POSIF0)
  CY_SCU_IRQCTRL_POSIF0_SR0_IRQ14             = (14U << 8U) | 2U, /**< POSIF0_SR0 connected to IRQ14 */
#endif
#if defined(USIC1)
  CY_SCU_IRQCTRL_USIC0_SR5_OR_USIC1_SR5_IRQ14 = (14U << 8U) | 3U, /**< USIC0_SR5 and USIC1_SR5 connected to IRQ14 */
#endif

  CY_SCU_IRQCTRL_VADC0_C0SR0_IRQ15              = (15U << 8U) | 0U, /**< VADC0_C0SR0 connected to IRQ15 */
  CY_SCU_IRQCTRL_USIC0_SR0_IRQ15                = (15U << 8U) | 1U, /**< USIC0_SR0 connected to IRQ15 */
#if defined(POSIF0)
  CY_SCU_IRQCTRL_POSIF0_SR1_IRQ15               = (15U << 8U) | 2U, /**< POSIF0_SR1 connected to IRQ15 */
#endif
  CY_SCU_IRQCTRL_VADC0_C0SR0_OR_USIC0_SR0_IRQ15 = (15U << 8U) | 3U, /**< VADC0_C0SR0 and USIC0_SR0 connected to IRQ15 */

  CY_SCU_IRQCTRL_VADC0_C0SR1_IRQ16              = (16U << 8U) | 0U, /**< VADC0_C0SR1 connected to IRQ16 */
  CY_SCU_IRQCTRL_USIC0_SR1_IRQ16                = (16U << 8U) | 1U, /**< USIC0_SR1 connected to IRQ16 */
  CY_SCU_IRQCTRL_CCU40_SR2_IRQ16                = (16U << 8U) | 2U, /**< CCU40_SR2 connected to IRQ16 */
  CY_SCU_IRQCTRL_VADC0_C0SR1_OR_USIC0_SR1_IRQ16 = (16U << 8U) | 3U, /**< VADC0_C0SR1 and USIC0_SR1 connected to IRQ16 */

  CY_SCU_IRQCTRL_VADC0_G0SR0_IRQ17              = (17U << 8U) | 0U, /**< VADC0_G0SR0 connected to IRQ17 */
  CY_SCU_IRQCTRL_USIC0_SR2_IRQ17                = (17U << 8U) | 1U, /**< USIC0_SR2 connected to IRQ17 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR0_IRQ17                 = (17U << 8U) | 2U, /**< CAN0_SR0 connected to IRQ17 */
#endif
  CY_SCU_IRQCTRL_VADC0_G0SR0_OR_USIC0_SR2_IRQ17 = (17U << 8U) | 3U, /**< VADC0_G0SR0 and USIC0_SR2 connected to IRQ17 */

  CY_SCU_IRQCTRL_VADC0_G0SR1_IRQ18              = (18U << 8U) | 0U, /**< VADC0_G0SR1 connected to IRQ18 */
  CY_SCU_IRQCTRL_USIC0_SR3_IRQ18                = (18U << 8U) | 1U, /**< USIC0_SR3 connected to IRQ18 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR1_IRQ18                 = (18U << 8U) | 2U, /**< CAN0_SR1 connected to IRQ18 */
#endif
  CY_SCU_IRQCTRL_VADC0_G0SR1_OR_USIC0_SR3_IRQ18 = (18U << 8U) | 3U, /**< VADC0_G0SR1 and USIC0_SR3 connected to IRQ18 */

  CY_SCU_IRQCTRL_VADC0_G1SR0_IRQ19              = (19U << 8U) | 0U, /**< VADC0_G1SR0 connected to IRQ19 */
  CY_SCU_IRQCTRL_USIC0_SR4_IRQ19                = (19U << 8U) | 1U, /**< USIC0_SR4 connected to IRQ19 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR2_IRQ19                 = (19U << 8U) | 2U, /**< CAN0_SR2 connected to IRQ19 */
#endif
  CY_SCU_IRQCTRL_VADC0_G1SR0_OR_USIC0_SR4_IRQ19 = (19U << 8U) | 3U, /**< VADC0_G1SR0 and USIC0_SR4 connected to IRQ19 */

  CY_SCU_IRQCTRL_VADC0_G1SR1_IRQ20              = (20U << 8U) | 0U, /**< VADC0_G1SR1 connected to IRQ20 */
  CY_SCU_IRQCTRL_USIC0_SR5_IRQ20                = (20U << 8U) | 1U, /**< USIC0_SR5 connected to IRQ20 */
#if defined(CAN)
  CY_SCU_IRQCTRL_CAN0_SR3_IRQ20                 = (20U << 8U) | 2U, /**< CAN0_SR3 connected to IRQ20 */
  CY_SCU_IRQCTRL_CAN0_SR4_IRQ20                 = (20U << 8U) | 2U, /**< @deprecated use instead CY_SCU_IRQCTRL_CAN0_SR3_IRQ20 */
#endif
  CY_SCU_IRQCTRL_VADC0_G1SR1_OR_USIC0_SR5_IRQ20 = (20U << 8U) | 3U, /**< VADC0_G1SR1 and USIC0_SR5 connected to IRQ20 */

  CY_SCU_IRQCTRL_CCU40_SR0_IRQ21                = (21U << 8U) | 0U, /**< CCU40_SR0 connected to IRQ21 */
#if defined(CCU41)
  CY_SCU_IRQCTRL_CCU41_SR0_IRQ21                = (21U << 8U) | 1U, /**< CCU41_SR0 connected to IRQ21 */
#endif
  CY_SCU_IRQCTRL_USIC0_SR0_IRQ21                = (21U << 8U) | 2U, /**< USIC0_SR0 connected to IRQ21 */
#if defined(CCU41)
  CY_SCU_IRQCTRL_CCU40_SR0_OR_CCU41_SR0_IRQ21   = (21U << 8U) | 3U, /**< CCU40_SR0 and CCU41_SR0 connected to IRQ21 */
#endif

  CY_SCU_IRQCTRL_CCU40_SR1_IRQ22                = (22U << 8U) | 0U, /**< CCU40_SR1 connected to IRQ22 */
#if defined(CCU41)
  CY_SCU_IRQCTRL_CCU41_SR1_IRQ22                = (22U << 8U) | 1U, /**< CCU41_SR1 connected to IRQ22 */
#endif
  CY_SCU_IRQCTRL_USIC0_SR1_IRQ22                = (22U << 8U) | 2U, /**< USIC0_SR1 connected to IRQ22 */
#if defined(CCU41)
  CY_SCU_IRQCTRL_CCU40_SR0_OR_CCU41_SR0_IRQ22   = (22U << 8U) | 3U, /**< CCU40_SR0 and CCU41_SR0 connected to IRQ22 */
#endif

  CY_SCU_IRQCTRL_CCU40_SR2_IRQ23                = (23U << 8U) | 0U, /**< CCU40_SR2 connected to IRQ23 */
#if defined(CCU41)
  CY_SCU_IRQCTRL_CCU41_SR2_IRQ23                = (23U << 8U) | 1U, /**< CCU41_SR2 connected to IRQ23 */
#endif
  CY_SCU_IRQCTRL_USIC0_SR2_IRQ23                = (23U << 8U) | 2U, /**< USIC0_SR2 connected to IRQ23 */
#if defined(CCU41)
  CY_SCU_IRQCTRL_CCU40_SR2_OR_CCU41_SR2_IRQ23   = (23U << 8U) | 3U, /**< CCU40_SR2 and CCU41_SR2 connected to IRQ23 */
#endif

  CY_SCU_IRQCTRL_CCU40_SR3_IRQ24                = (24U << 8U) | 0U, /**< CCU40_SR3 connected to IRQ24 */
#if defined(CCU41)
  CY_SCU_IRQCTRL_CCU41_SR3_IRQ24                = (24U << 8U) | 1U, /**< CCU41_SR3 connected to IRQ24 */
#endif
  CY_SCU_IRQCTRL_USIC0_SR3_IRQ24                = (24U << 8U) | 2U, /**< USIC0_SR3 connected to IRQ24 */
#if defined(CCU41)
  CY_SCU_IRQCTRL_CCU40_SR3_OR_CCU41_SR3_IRQ24   = (24U << 8U) | 3U, /**< CCU40_SR3 and CCU41_SR3 connected to IRQ24 */
#endif

#if defined(CCU80)
  CY_SCU_IRQCTRL_CCU80_SR0_IRQ25                = (25U << 8U) | 0U, /**< CCU80_SR0 connected to IRQ25 */
#endif
#if defined(CCU81)
  CY_SCU_IRQCTRL_CCU81_SR0_IRQ25                = (25U << 8U) | 1U, /**< CCU81_SR0 connected to IRQ25 */
#endif
  CY_SCU_IRQCTRL_USIC0_SR4_IRQ25                = (25U << 8U) | 2U, /**< USIC0_SR4 connected to IRQ25 */
#if defined(CCU80) && defined(CCU81)
  CY_SCU_IRQCTRL_CCU80_SR0_OR_CCU81_SR0_IRQ25   = (25U << 8U) | 3U, /**< CCU80_SR0 and CCU81_SR0 connected to IRQ25 */
#endif

#if defined(CCU80)
  CY_SCU_IRQCTRL_CCU80_SR1_IRQ26                = (26U << 8U) | 0U, /**< CCU80_SR1 connected to IRQ26 */
#endif
#if defined(CCU81)
  CY_SCU_IRQCTRL_CCU81_SR1_IRQ26                = (26U << 8U) | 1U, /**< CCU81_SR1 connected to IRQ26 */
#endif
  CY_SCU_IRQCTRL_USIC0_SR5_IRQ26                = (26U << 8U) | 2U, /**< USIC0_SR5 connected to IRQ26 */
#if defined(CCU80) && defined(CCU81)
  CY_SCU_IRQCTRL_CCU80_SR1_OR_CCU81_SR1_IRQ26   = (26U << 8U) | 3U, /**< CCU80_SR1 and CCU81_SR1 connected to IRQ26 */
#endif

#if defined(POSIF0)
  CY_SCU_IRQCTRL_POSIF0_SR0_IRQ27               = (27U << 8U) | 0U, /**< POSIF0_SR0 connected to IRQ27 */
#endif
#if defined(POSIF1)
  CY_SCU_IRQCTRL_POSIF1_SR0_IRQ27               = (27U << 8U) | 1U, /**< POSIF1_SR0 connected to IRQ27 */
#endif
  CY_SCU_IRQCTRL_CCU40_SR3_IRQ27                = (27U << 8U) | 2U, /**< CCU40_SR3 connected to IRQ27 */
#if defined(POSIF0) && defined(POSIF1)
  CY_SCU_IRQCTRL_POSIF0_SR0_OR_POSIF1_SR0_IRQ27 = (27U << 8U) | 3U, /**< POSIF0_SR0 and POSIF1_SR0 connected to IRQ27 */
#endif

#if defined(POSIF0)
  CY_SCU_IRQCTRL_POSIF0_SR1_IRQ28               = (28U << 8U) | 0U, /**< POSIF0_SR1 connected to IRQ28 */
#endif
#if defined(POSIF1)
  CY_SCU_IRQCTRL_POSIF1_SR1_IRQ28               = (28U << 8U) | 1U, /**< POSIF1_SR1 connected to IRQ28 */
#endif
  CY_SCU_IRQCTRL_ERU0_SR0_IRQ28                 = (28U << 8U) | 2U, /**< ERU0_SR0 connected to IRQ28 */
#if defined(POSIF0) && defined(POSIF1)
  CY_SCU_IRQCTRL_POSIF0_SR1_OR_POSIF1_SR1_IRQ28 = (28U << 8U) | 3U, /**< POSIF0_SR1 and POSIF1_SR1 connected to IRQ28 */
#endif

  CY_SCU_IRQCTRL_CCU40_SR1_IRQ29                = (29U << 8U) | 1U, /**< CCU40_SR1 connected to IRQ29 */
  CY_SCU_IRQCTRL_ERU0_SR1_IRQ29                 = (29U << 8U) | 2U, /**< ERU0_SR1 connected to IRQ29 */

  CY_SCU_IRQCTRL_CCU40_SR2_IRQ30                = (30U << 8U) | 1U, /**< CCU40_SR2 connected to IRQ30 */
  CY_SCU_IRQCTRL_ERU0_SR2_IRQ30                 = (30U << 8U) | 2U, /**< ERU0_SR2 connected to IRQ30 */

#if defined(BCCU0)
  CY_SCU_IRQCTRL_BCCU0_SR0_IRQ31                = (31U << 8U) | 0U, /**< BCCU0_SR0 connected to IRQ31 */
#endif
  CY_SCU_IRQCTRL_CCU40_SR3_IRQ31                = (31U << 8U) | 1U, /**< CCU40_SR3 connected to IRQ31 */
  CY_SCU_IRQCTRL_ERU0_SR3_IRQ31                 = (31U << 8U) | 2U, /**< ERU0_SR3 connected to IRQ31 */
#if defined(BCCU0)
  CY_SCU_IRQCTRL_BCCU0_SR0_OR_CCU40_SR3_IRQ31   = (31U << 8U) | 3U, /**< BCCU0_SR0 and CCU40_SR3 connected to IRQ31 */
#endif
} Cy_SCU_IRQCTRL_t;

/**
 * Power monitor voltage range selection.
 * Defines the voltage threshold ranges for the external voltage detector (VDEL).
 */
typedef enum CY_SCU_POWER_MONITOR_RANGE
{
  CY_SCU_POWER_MONITOR_RANGE_2_25V = 0U << SCU_ANALOG_ANAVDEL_VDEL_SELECT_Pos, /**< Voltage range 2.25V */
  CY_SCU_POWER_MONITOR_RANGE_3_00V = 1U << SCU_ANALOG_ANAVDEL_VDEL_SELECT_Pos, /**< Voltage range 3.00V */
  CY_SCU_POWER_MONITOR_RANGE_4_40V = 2U << SCU_ANALOG_ANAVDEL_VDEL_SELECT_Pos  /**< Voltage range 4.40V */
} Cy_SCU_POWER_MONITOR_RANGE_t;

/**
 * Power monitor delay timing adjustment.
 * Defines the delay for the external voltage detector (VDEL) to filter voltage fluctuations.
 */
typedef enum CY_SCU_POWER_MONITOR_DELAY
{
  CY_SCU_POWER_MONITOR_DELAY_1US = 0U << SCU_ANALOG_ANAVDEL_VDEL_TIM_ADJ_Pos,   /**< Delay 1 microsecond */
  CY_SCU_POWER_MONITOR_DELAY_500NS = 1U << SCU_ANALOG_ANAVDEL_VDEL_TIM_ADJ_Pos, /**< Delay 500 nanoseconds */
  CY_SCU_POWER_MONITOR_DELAY_250NS = 2U << SCU_ANALOG_ANAVDEL_VDEL_TIM_ADJ_Pos, /**< Delay 250 nanoseconds */
  CY_SCU_POWER_MONITOR_DELAY_NONE = 3U << SCU_ANALOG_ANAVDEL_VDEL_TIM_ADJ_Pos   /**< No delay */
} Cy_SCU_POWER_MONITOR_DELAY_t;

/*********************************************************************************************************************
 * DATA TYPES
 ********************************************************************************************************************/

/**
 * Function pointer type used for registering callback functions on SCU event occurrence.
 */
typedef void (*Cy_SCU_INTERRUPT_EVENT_HANDLER_t)(void);

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

 /**
 *  Defines a data structure for initializing the data of the supply voltage monitoring block.
 *  Supply voltage monitoring block consists of 2 detectors namely External voltage detector (VDEL) and External brownout
 *  detector (BDE) in the EVR that are used to monitor the VDDP. \a VDEL detector compares the supply voltage against a
 *  pre-warning threshold voltage.
 *  Use type \a Cy_SCU_SUPPLYMONITOR_t for accessing these structure parameters.
 */
typedef struct CY_SCU_SUPPLYMONITOR
{
  uint32_t ext_supply_threshold;   /**<  External supply range (VDEL Range Select).\n
                                         \b Range:
                                         <ul>
                                         <li>00B sets threshold value to 2.25V
                                         <li>01B sets threshold value to 3.0V
                                         <li>10B sets threshold value to 4.4V
                                         </ul>*/
  uint32_t ext_supply_monitor_speed; /**<  Speed of the voltage monitor(VDEL Timing Setting).\n
                                           \b Range:
                                           <ul>
                                           <li>00B sets monitor speed typ 1us - slowest response time
                                           <li>01B sets monitor speed typ 500n
                                           <li>10B sets monitor speed typ 250n
                                           <li>11B sets monitor speed with no delay - fastest response time.
                                           </ul>*/
  bool     enable_prewarning_int;    /**< Configure pre-warning interrupt generation.\n
                                          \b Range:
                                          <ul>
                                          <li>\a true to enable the interrupt.
                                          <li>\a false to disable the interrupt.
                                          </ul>*/
  bool     enable_vdrop_int;         /**< Configure VDROP interrupt generation.\n
                                          \b Range:
                                          <ul>
                                          <li>\a true to enable the interrupt.
                                          <li>\a false to disable the interrupt.
                                          </ul>*/
  bool     enable_vclip_int;         /**< Configure VCLIP interrupt.\n
                                          \b Range:
                                          <ul>
                                          <li>\a true to enable the interrupt.
                                          <li>\a false to disable the interrupt.
                                          </ul>*/
  bool     enable_at_init;           /**< Whether the monitor has to be enabled (VDEL unit Enable) after initialization.\n
                                          \b Range:
                                          <ul>
                                          <li>\a true to enable after initialization.
                                          <li>\a false to enable after initialization.
                                          </ul>*/
} Cy_SCU_SUPPLYMONITOR_t;

/**
 *  Defines a data structure for initializing the data of the clock functional block.
 *  Clock functional block configures clock dividers, peripheral and RTC clock source by configuring corresponding
 *  bits in \a CLKCR clock control register.
 *  Use type \a Cy_SCU_CLOCK_CONFIG_t for accessing these structure parameters.
 */
typedef struct CY_SCU_CLOCK_CONFIG
{
  uint16_t fdiv; /**<  Fractional clock divider (FDIV). \b Range: 0 to 1023. */
  uint8_t idiv; /**<  Integer clock divider (IDIV). \b Range: 0 to 255. */
  Cy_SCU_CLOCK_DCLKSRC_t dclk_src; /**< DCLK clock source selection. */
  Cy_SCU_CLOCK_OSCHP_MODE_t oschp_mode; /**< OSCHP mode. */
  Cy_SCU_CLOCK_OSCLP_MODE_t osclp_mode; /**< OSCLP mode. */
  Cy_SCU_CLOCK_PCLKSRC_t pclk_src; /**<  Source of PCLK Clock */
  Cy_SCU_CLOCK_RTCCLKSRC_t rtc_src; /**<  Source of RTC Clock */
} Cy_SCU_CLOCK_CONFIG_t;

/*********************************************************************************************************************
 * API Prototypes
 ********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @param trigger    CCU slices to be triggered synchronously via software. The value is a bitmask of CCU slice bits
 *                    in the register CCUCON. <br>
 *                    \b Range: Use type @ref Cy_SCU_CCU_TRIGGER_t for bitmask of individual CCU slices. Multiple slices can be
 *                    combined using \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Generates active edge(low to high) trigger for multiple CCU units at the same time.\n\n
 * Before executing this API, all the required CCU timers should configure external start.
 * The edge of the start signal should be selected as active edge.
 * The input signal for the CCU slice should be selected as SCU input.
 * The above mentioned configurations can be made using the CCU LLD API Cy_CCU4_SLICE_StartConfig().
 * CCU timer slice should be started using Cy_CCU4_SLICE_StartTimer() before triggering
 * the timer using this API.<BR>
 * \par<b>Related APIs:</b><BR>
 * Cy_CCU4_SLICE_StartConfig(), Cy_CCU4_SLICE_SetInput(), Cy_SCU_SetCcuTriggerLow()\n\n\n
 */
__STATIC_INLINE void Cy_SCU_SetCcuTriggerHigh(const uint32_t trigger)
{
  SCU_GENERAL->CCUCON |= (uint32_t)trigger;
}

/**
 *
 * @param trigger  CCU slices to be triggered synchronously via software. The value is a bitmask of CCU slice bits
 *                    in the register CCUCON. <br>
 *                    \b Range: Use type @ref Cy_SCU_CCU_TRIGGER_t for bitmask of individual CCU slices. Multiple slices can be
 *                    combined using \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Generates passive edge(high to low) trigger for multiple CCU units at the same time.\n\n
 * Before executing this API, all the required CCU timers should configure external start.
 * The edge of the start signal should be selected as passive edge.
 * The input signal for the CCU slice should be selected as SCU input.
 * The above mentioned configurations can be made using the CCU LLD API Cy_CCU4_SLICE_StartConfig().
 * CCU timer slice should be started using Cy_CCU4_SLICE_StartTimer() before triggering
 * the timer using this API.<BR>
 * \par<b>Related APIs:</b><BR>
 * Cy_CCU4_SLICE_StartConfig(), Cy_CCU4_SLICE_SetInput(), Cy_SCU_SetCcuTriggerHigh()\n\n\n
 */
__STATIC_INLINE void Cy_SCU_SetCcuTriggerLow(const uint32_t trigger)
{
  SCU_GENERAL->CCUCON &= (uint32_t)~trigger;
}

/**
 *
 * @param config Pointer to structure holding the clock prescaler values and divider values for
 *                configuring clock generators and clock tree.\n
 *                \b Range: Configure the members of structure @ref Cy_SCU_CLOCK_CONFIG_t for various
 *                parameters of clock setup.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Initializes clock generators and clock tree.\n\n
 * Peripheral clock and system clock are configured based on the input configuration \a config.
 * The system clock frequency is tuned by configuring the FDIV and IDIV values of CLKCR register.
 * The values of FDIV and IDIV can be provided as part of input configuration.
 * The PCLK divider determines the ratio of peripheral clock to the system clock.
 * The source of RTC clock is set based on the input configuration.
 * \a SystemCoreClock variable will be updated with the value of
 * system clock frequency. Access to protected bit fields are handled internally.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_CLOCK_GetPeripheralClockFrequency(), Cy_SCU_CLOCK_GetCpuClockFrequency() \n\n\n
 */
void Cy_SCU_CLOCK_Init(const Cy_SCU_CLOCK_CONFIG_t *const config);

/**
 *
 * @param event  Bit mask of the event to enable. \b Range: Use type @ref Cy_SCU_INTERRUPT_EVENT_t
 *                for providing the input value. Multiple events can be combined using the \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the generation of interrupt for the input events.\n\n
 * The events are enabled by setting the respective bit fields in the SRMSK register. \n
 * Note: User should separately enable the NVIC node responsible for handling the SCU interrupt.
 * The interrupt will be generated when the respective event occurs.
 * \par<b>Related APIs:</b><BR>
 * NVIC_EnableIRQ(), Cy_SCU_INTERRUPT_DisableEvent()\n\n\n
 */
void Cy_SCU_INTERRUPT_EnableEvent(const Cy_SCU_INTERRUPT_EVENT_t event);


/**
 *
 * @param event  Bit mask of the event to disable. \b Range: Use type @ref Cy_SCU_INTERRUPT_EVENT_t
 *                for providing the input value. Multiple events can be combined using the \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables generation of interrupt on occurrence of the input event.\n\n
 * The events are disabled by resetting the respective bit fields in the SRMSK register. \n
 * \par<b>Related APIs:</b><BR>
 * NVIC_DisableIRQ(), Cy_SCU_INTERRUPT_EnableEvent()\n\n\n
 */
void Cy_SCU_INTERRUPT_DisableEvent(const Cy_SCU_INTERRUPT_EVENT_t event);

/**
 *
 * @param event Bit mask of the event to be triggered. \b Range: Use type @ref Cy_SCU_INTERRUPT_EVENT_t
 *                for providing the input value. Multiple events can be combined using the \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Triggers the event as if the hardware raised it.\n\n
 * Event will be triggered by setting the respective bitfield in the SRSET register.\n
 * Note: User should enable the NVIC node that handles the respective event for interrupt generation.
 * \par<b>Related APIs:</b><BR>
 * NVIC_EnableIRQ(), Cy_SCU_INTERRUPT_GetEventStatus(), Cy_SCU_INTERRUPT_ClearEventStatus() \n\n\n
 */
void Cy_SCU_INTERRUPT_TriggerEvent(const Cy_SCU_INTERRUPT_EVENT_t  event);

/**
 * @return uint32_t  Status of the SCU events.
 *
 * \par<b>Description</b><br>
 * Provides the status of all SCU events.\n\n
 * The status is read from the SRRAW register. To check the status of a particular
 * event, the returned value should be masked with the bit mask of the event. The bitmask
 * of events can be obtained using the type @ref Cy_SCU_INTERRUPT_EVENT_t. Multiple events'
 * status can be checked by combining the bit masks using \a OR operation.
 * After detecting the event, the event status should be cleared using software to detect the event again.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_INTERRUPT_ClearEventStatus(), Cy_SCU_INTERRUPT_TriggerEvent(), Cy_SCU_INTERRUPT_SetEventHandler() \n\n\n
 */
Cy_SCU_INTERRUPT_EVENT_t Cy_SCU_INTERRUPT_GetEventStatus(void);

/**
 *
 * @param event  Bit mask of the events to clear. \b Range: Use type @ref Cy_SCU_INTERRUPT_EVENT_t
 *                for providing the input value. Multiple events can be combined using the \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Clears the event status bit in SRRAW register.\n\n
 * The events are cleared by writing value 1 to their bit positions in the SRCLR register.
 * The API can be used when polling method is used. After detecting the event, the event status
 * should be cleared using software to detect the event again.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_INTERRUPT_GetEventStatus(), Cy_SCU_INTERRUPT_TriggerEvent() \n\n\n
 */
void Cy_SCU_INTERRUPT_ClearEventStatus(const Cy_SCU_INTERRUPT_EVENT_t event);

/**
 *
 * @return uint32_t  Status representing the reason for device reset.
 *
 * \par<b>Description</b><br>
 * Provides the value representing the reason for device reset.\n\n
 * The return value is an encoded word, which can indicate multiple reasons for the last reset. Each bit position of the
 * returned word is representative of a last reset cause. The returned value should be appropriately masked to check
 * the cause of reset.
 * The cause of the last reset gets automatically stored in
 * the \a SCU_RSTSTAT register. The reset status shall be reset after each
 * startup in order to ensure consistent source indication after the next reset.
 * \b Range: The type @ref Cy_SCU_RESET_REASON_t can be used to get the bit masks of the reset cause.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_RESET_ClearDeviceResetReason() \n\n\n
 */
__STATIC_INLINE uint32_t Cy_SCU_RESET_GetDeviceResetReason(void)
{
  return ((SCU_RESET->RSTSTAT) & SCU_RESET_RSTSTAT_RSTSTAT_Msk);
}
/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Clears the reset reason bits in the reset status register. \n\n
 * Clearing of the reset status information in the \a SCU_RSTSTAT register via register bit \a RSTCLR.RSCLR is strongly
 * recommended to ensure a clear indication of the cause of next reset.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_RESET_GetDeviceResetReason() \n\n\n
 */
__STATIC_INLINE void Cy_SCU_RESET_ClearDeviceResetReason(void)
{
  /* Clear RSTSTAT.RSTSTAT bitfield */
  SCU_RESET->RSTCLR = (uint32_t)SCU_RESET_RSTCLR_RSCLR_Msk;
}

/**
 * @return uint32_t  Value of CPU clock frequency.
 *
 * \par<b>Description</b><br>
 * Provides the vlaue of CPU clock frequency.\n\n
 * The value is stored in a global variable \a \b SystemCoreClock.
 * It is updated when the clock configuration is done using the SCU LLD APIs.
 * The value represents the frequency of clock used for CPU operation.
 * \b Range: Value is of type uint32_t, and gives the value of frequency in Hertz.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_CLOCK_GetPeripheralClockFrequency(), Cy_SCU_CLOCK_GatePeripheralClock() \n\n\n
 */
__STATIC_INLINE uint32_t Cy_SCU_CLOCK_GetCpuClockFrequency(void)
{
  return SystemCoreClock;
}

/**
 * @return uint32_t  Value of peripheral clock frequency in Hertz.
 *
 * \par<b>Description</b><br>
 * Provides the vlaue of clock frequency at which the peripherals are working.\n\n
 * The value is derived from the CPU frequency. \b Range: Value is of type uint32_t. It is represented in Hertz.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_CLOCK_GetCpuClockFrequency(),Cy_SCU_CLOCK_GatePeripheralClock() \n\n\n
 */
uint32_t Cy_SCU_CLOCK_GetPeripheralClockFrequency(void);

/**
 *
 * @param  peripheral The peripheral for which the clock has to be gated. \b Range: Use type @ref Cy_SCU_PERIPHERAL_CLOCK_t
 *                    to identify the peripheral clock to be gated.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Blocks the supply of clock to the selected peripheral.\n\n
 * Clock gating helps in reducing the power consumption. User can selectively gate the clocks of unused peripherals.
 * fPCLK is the source of clock to various peripherals. Some peripherals support clock gate. Such a gate blocks
 * the clock supply for the selected peripheral.
 * Software can request for individual gating of such peripheral clocks by enabling the \a SCU_CGATSET0
 * register bit field. Every bit in \a SCU_CGATSET0 register is protected by the bit protection scheme. Access to protected
 * bit fields are handled internally.
 *
 * Note: Clock gating shall not be activated unless the module is in reset state. So use \a
 * Cy_SCU_CLOCK_IsPeripheralClockGated() API before enabling the gating of any peripheral.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_CLOCK_IsPeripheralClockGated(), Cy_SCU_CLOCK_UngatePeripheralClock() \n\n\n
 */
void Cy_SCU_CLOCK_GatePeripheralClock(const Cy_SCU_PERIPHERAL_CLOCK_t peripheral);

/**
 *
 * @param  peripheral  The peripheral for which the clock has to be ungated. \b Range: Use type @ref Cy_SCU_PERIPHERAL_CLOCK_t
 *                    to identify the peripheral.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the supply of clock to the selected peripheral.\n\n
 * By default when the device powers on, the peripheral clock will be gated for the
 * peripherals that support clock gating.
 * The peripheral clock should be enabled before using it for any functionality.
 * fPCLK is the source of clock to various peripherals. Some peripherals support clock gate.
 * Software can request for individual ungating of such peripheral clocks by setting respective bits
 * in the \a SCU_CGATCLR0 register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_CLOCK_IsPeripheralClockGated(), Cy_SCU_CLOCK_GatePeripheralClock() \n\n\n
 */
void Cy_SCU_CLOCK_UngatePeripheralClock(const Cy_SCU_PERIPHERAL_CLOCK_t peripheral);

/**
 *
 * @param  peripheral  The peripheral for which the check for clock gating has to be done.
 *                    \b Range: Use type @ref Cy_SCU_PERIPHERAL_CLOCK_t to identify the peripheral.
 *
 * @return bool Status of the peripheral clock gating. \b Range: true if the peripheral clock is gated.
 *              false if the peripheral clock ungated(gate de-asserted).
 *
 * \par<b>Description</b><br>
 * Gives the status of peripheral clock gating.\n\n
 * Checks the status of peripheral clock gating using the register CGATSTAT0.
 * It is recommended to use this API before
 * enabling the gating of any peripherals through \a Cy_SCU_CLOCK_GatePeripheralClock() API.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_CLOCK_UngatePeripheralClock(), Cy_SCU_CLOCK_GatePeripheralClock() \n\n\n
 */
bool Cy_SCU_CLOCK_IsPeripheralClockGated(const Cy_SCU_PERIPHERAL_CLOCK_t peripheral);


/**
 * @return  uint32_t  Status of the register mirror update.\n
 *                    \b Range: Use the bit mask of the SCU_GENERAL_MIRRSTS register for the mirror update event of
 *                    interest. e.g.: SCU_GENERAL_MIRRSTS_RTC_CTR_Msk. Multiple update events can be combined
 *                    using \a OR operation.
 *
 * \par<b>Description</b><br>
 * Provides the status of hibernate domain register update, when the respective mirror registers are changed. \n\n
 * The hibernate domain is connected to the core domain via SPI serial communication. MIRRSTS is a status register
 * representing the communication of changed value of a mirror register to its corresponding register in the
 * hibernate domain. The bit fields of the register indicate
 * that a corresponding register of the hibernate domain is ready to accept a write or that the communication interface
 * is busy with executing the previous operation.\n
 * Note: There is no hibernate domain in PSC1M devices. This register is retained for legacy purpose.
 */
__STATIC_INLINE uint32_t Cy_SCU_GetMirrorStatus(void)
{
  return (SCU_GENERAL->MIRRSTS);
}

/**
 * @param event The event for which the interrupt handler is to be configured. \n
 *        \b Range: Use type @ref Cy_SCU_INTERRUPT_EVENT_t for identifying the event.
 * @param handler Name of the function to be executed when the event if detected. \n
 *          \b Range: The function accepts no arguments and returns no value.
 * @return Cy_SCU_STATUS_t Status of configuring the event handler function for the selected event.\n
 *          \b Range: \a CY_SCU_STATUS_OK if the event handler is successfully configured.\n
 *            \a CY_SCU_STATUS_ERROR if the input event is invalid.\n
 * \par<b>Description</b><br>
 * Assigns the event handler function to be executed on occurrence of the selected event.\n\n
 * If the input event is valid, the handler function will be assigned to a table to be executed
 * when the interrupt is generated and the event status is set in the event status register. By using this API,
 * polling for a particular event can be avoided. This way the CPU utilization will be optimized. Multiple SCU events
 * can generate a common interrupt. When the interrupt is generated, a common interrupt service routine is executed.
 * It checks for status flags of events which can generate the interrupt. The handler function will be executed if the
 * event flag is set.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_INTERRUPT_TriggerEvent(), Cy_SCU_INTERRUPT_GetEventStatus() \n\n\n
 */
Cy_SCU_STATUS_t Cy_SCU_INTERRUPT_SetEventHandler(const Cy_SCU_INTERRUPT_EVENT_t event, const Cy_SCU_INTERRUPT_EVENT_HANDLER_t handler);

/**
 * @param sr_num Service request number identifying the SCU interrupt generated.\n
 *                \b Range: 0 to 2.
 * @return None
 * \par<b>Description</b><br>
 * A common function to execute callback functions for multiple events.\n\n
 * It checks for the status of events which can generate the interrupt with the selected service request.
 * If the event is set, the corresponding callback function will be executed. It also clears the event status bit.\n
 * \b Note: This is an internal function. It should not be called by the user application.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_INTERRUPT_SetEventHandler() \n\n\n
 */
void Cy_SCU_IRQHandler(uint32_t sr_num);

/**
 *
 * @param freq_khz   Required MCLK frequency value in kHz.\n
 *                   \b Range: 188 to 48000 when DCO1 is clock source for clock control unit.
 *                              79 to 48000 when OSC_HP is clock source for clock control unit.\n
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * This API configures main clock (MCLK) frequency to requested frequency value.\n\n
 * The API configures main clock by setting \a IDIV and \a FDIV bit's of the \a CLKCR register for
 * PSC1M Device and with additional \a FDIV bit (FDIV[9:8]) of the \a CLKCR1 register settings
 * for PSC1M device.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_CLOCK_ScaleMCLKFrequency()\n\n\n
 */
void Cy_SCU_CLOCK_SetMCLKFrequency(uint32_t freq_khz);

/**
 *
 * @param idiv  Divider value.\n
 *              \b Range: 0 to 255.\n
 * @param fdiv  Fractional Divider value.\n
 *              \b Range: 0 to 1023.\n
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * This API configures main clock (MCLK) frequency by updating user provided divider values.\n\n
 * The API configures main clock by setting \a IDIV and \a FDIV bit's of the \a CLKCR register for
 * PSC1M Device and with additional \a FDIV bit (FDIV[9:8]) of the \a CLKCR1 register settings
 * for PSC1M device.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_CLOCK_SetMCLKFrequency()\n\n\n
 */
void Cy_SCU_CLOCK_ScaleMCLKFrequency(uint32_t idiv, uint32_t fdiv);

/**
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Locks access to protected bit fields of the SCU.\n\n
 * The bit protection scheme prevents changing selected register bits by unauthorized code.
 * Bit protection scheme is enabled by writing 000000C3H to \a PASSWD register. By writing
 * this value, the API is setting the \a MODE bit field to bit protection enabled state.\n
 * List of Protected Register Bit Fields are mentioned below. \n
 * <table>
 *   <tr><td> \a Register    </td> <td>\a Bit fields                  </td></tr>
 *   <tr><td> SCU_CLKCR      </td> <td> FDIV, IDIV, PCLKSEL, RTCLKSEL </td></tr>
 *   <tr><td> SCU_CGATSET0   </td> <td> All bits                      </td></tr>
 *   <tr><td> SCU_CGATCLR0   </td> <td> All bits                      </td></tr>
 *   <tr><td> SCU_ANAOFFSET  </td> <td> ADJL_OFFSET                   </td></tr>
 *   <tr><td> VADC0_ACCPROT0 </td> <td> All bits                      </td></tr>
 *   <tr><td> VADC0_ACCPROT1 </td> <td> All bits                      </td></tr>
 * </table>
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_UnlockProtectedBits() \n\n\n
 */
void Cy_SCU_LockProtectedBits(void);

/**
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Unlocks access to protected bit fields of the SCU.\n\n
 * The bit protection scheme prevents changing selected register bits by unauthorized code.
 * Bit protection scheme can be temporarily(for 32 MCLK cycles) disabled by writing 000000C0H to \a PASSWD register.
 * By writing this value, the API is setting the \a MODE bit field to bit protection disabled state. The API
 * waits for the protection to be disabled after changing the \a MODE.\n
 * User can change the values of the protected bit fields within 32 MCLK cycles. After 32 MCLK cycles the lock will
 * be enabled automatically.
 * List of Protected Register Bit Fields are mentioned below. \n
 * <table>
 *   <tr><td> \a Register    </td> <td>\a Bit fields                  </td></tr>
 *   <tr><td> SCU_CLKCR      </td> <td> FDIV, IDIV, PCLKSEL, RTCLKSEL </td></tr>
 *   <tr><td> SCU_CGATSET0   </td> <td> All bits                      </td></tr>
 *   <tr><td> SCU_CGATCLR0   </td> <td> All bits                      </td></tr>
 *   <tr><td> SCU_ANAOFFSET  </td> <td> ADJL_OFFSET                   </td></tr>
 *   <tr><td> VADC0_ACCPROT0 </td> <td> All bits                      </td></tr>
 *   <tr><td> VADC0_ACCPROT1 </td> <td> All bits                      </td></tr>
 * </table>
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_LockProtectedBits() \n\n\n
 */
void Cy_SCU_UnlockProtectedBits(void);

/**
 *
 * @param obj   Pointer to data structure consisting voltage monitoring block configuration.\n
 *               \b Range: Use type @ref Cy_SCU_SUPPLYMONITOR_t for detailed description of structure members.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Initializes power supply monitoring unit.\n\n
 * Supply voltage monitoring block consist of 2 detectors namely External voltage detector (VDEL) and External brownout
 * detector (BDE) in the EVR that are used to monitor the VDDP. \a VDEL detector compares the supply voltage against a
 * pre-warning threshold voltage \a ext_supply_threshold. The threshold level is programmable via register \a ANAVDEL.VDEL_SELECT. An interrupt
 * if enabled via \a enable_prewarning_int, will be triggered if a level below this threshold is detected and the flag, VDDPI,
 * in SRRAW register bit is set. Similarly interrupts can be enabled for the events of VCLIP and prewarning, using the structure members,
 * \a enable_vclip_int and \a enable_prewarning_int.  The handlers for these interrupts have to be explicitly defined using
 * the API Cy_SCU_INTERRUPT_SetEventHandler().
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_INTERRUPT_SetEventHandler()\n\n\n
 */
void Cy_SCU_SupplyMonitorInit(const Cy_SCU_SUPPLYMONITOR_t *obj);

/**
 * @param limit Kelvin degree temperature lower compare limit in range [233,388]
 * @return Cy_SCU_STATUS_t status of limit installation
 *
 * \par<b>Description</b><br>
 * Set lower temperature compare limit.
 * A low temperature interrupt (SCU_IRQ1) is triggered if Tchip < limit and the event and interrupt are enabled.
 * Alternatively Cy_SCU_LowTemperature() can be used to check the status.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_LowTemperature()\n\n\n
 *
 **/
Cy_SCU_STATUS_t Cy_SCU_SetTempLowLimit(uint32_t limit);

/**
 * @param limit Kelvin degree temperature higher compare limit in range [233,388]
 * @return Cy_SCU_STATUS_t status of limit installation
 *
 * \par<b>Description</b><br>
 * Set higher temperature compare limit.
 * A high temperature interrupt (SCU_IRQ1) is triggered if Tchip > limit and the event and interrupt are enabled.
 * Alternatively Cy_SCU_HighTemperature() can be used to check the status.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_HighTemperature()\n\n\n
 *
 **/
Cy_SCU_STATUS_t Cy_SCU_SetTempHighLimit(uint32_t limit);

/**
 *
 * @param lower_temp  Lower threshold value for the die temperature.\n
 *          \b Range: 0 to 65535(16 bit unsigned value).
 * @param upper_temp  Upper threshold value for the die temperature.\n
 *          \b Range: 0 to 65535(16 bit unsigned value).
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Configures upper and lower thresholds of die temperature as raw digital values into temperature sensor.\n\n
 * The API configures \a ANATSEIH and \a ANATSEIL registers for upper and lower die temperature threshold limits
 * respectively.\n
 * It is recommended to use following steps:\n
 * - Call \a Cy_SCU_StopTempMeasurement to stop temperature measurement if it was started previously.\n
 * - Call \a Cy_SCU_SetRawTempLimits with desired lower and upper temperature threshold limit values.\n
 * - Finally call \a Cy_SCU_StartTempMeasurement to start temperature measurement.\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_StopTempMeasurement(), Cy_SCU_StartTempMeasurement() \n\n\n
 */
void Cy_SCU_SetRawTempLimits(const uint32_t lower_temp, const uint32_t upper_temp);

// /* API to program temperature limits in centigrade into temperature sensor unit */ // need to implement in future
// void Cy_SCU_SetTempLimits(const uint32_t lower_temp, const uint32_t upper_temp);
/**
 *
 * @return Cy_SCU_STATUS_t   Status of starting the temperature measurement.\n
 *                  \b Range: Use type @ref Cy_SCU_STATUS_t to identify the result.\n
 *                  CY_SCU_STATUS_OK- Temperature measurement started successfully.\n
 *                  Always returns the above status.
 *
 * \par<b>Description</b><br>
 * Starts die temperature measurement using internal temperature sensor.\n\n
 * The API, enables die temperature measurement and waits for about 10000 cycles until
 * temperature measurement result is available on \a SCU_ANALOG->ANATSEMON bit fields.\n
 * It is recommended to use following steps:\n
 * - Call \a Cy_SCU_StopTempMeasurement to stop temperature measurement if it was started previously.\n
 * - Call \a Cy_SCU_SetRawTempLimits with desired lower and upper temperature threshold limit values if it is needed.\n
 * - Call \a Cy_SCU_StartTempMeasurement to start temperature measurement.\n
 * - Read die temperature value using \a Cy_SCU_GetTemperature API.\n
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_StopTempMeasurement(), Cy_SCU_SetRawTempLimits(), Cy_SCU_GetTemperature() \n\n\n
 */
void Cy_SCU_StartTempMeasurement(void);

/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Stops the die temperature measurement.\n\n
 * Die temperature measurement is stopped by disabling the sensor using \a TSE_EN bit of
 * \a ANATSECTRL register.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_StartTempMeasurement(), Cy_SCU_SetRawTempLimits(), Cy_SCU_GetTemperature() \n\n\n
 */
void Cy_SCU_StopTempMeasurement(void);

/**
 *
 * @return bool  Result of checking whether the die temperature is more than the upper threshold.\n
 *            \b Range: \a false if temperature is below the upper threshold. \a true if temperature
 *            has exceeded the upper threshold configured in \a ANATSEIH register.
 *
 * \par<b>Description</b><br>
 * Check if the temperature has exceeded the upper threshold value.\n\n
 * The API checks for \a TSE_HIGH bit (TSE Compare High Temperature Event Status bit) of \a SRRAW register.
 * The bit will be set when the \a TSE_MON value in \a ANATSEMON register exceeds the value of
 * \a TSE_IH value in \a ANATSEIH register.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_StartTempMeasurement(), Cy_SCU_SetRawTempLimits(), Cy_SCU_GetTemperature(), Cy_SCU_LowTemperature() \n\n\n
 */
bool Cy_SCU_HighTemperature(void);

/**
 *
 * @return bool  Result of checking whether the die temperature is less than the lower threshold.\n
 *            \b Range: \a false if temperature is higher than the lower threshold. \a true if temperature
 *            has dropped below the lower threshold configured in \a ANATSEIL register.
 *
 * \par<b>Description</b><br>
 * Check if the temperature has dropped below the lower threshold value.\n\n
 * The API checks for \a TSE_LOW bit (TSE Compare Low Temperature Event Status bit) of \a SRRAW register.
 * The bit will be set when the \a TSE_MON value in \a ANATSEMON register drops below the value of
 * \a TSE_IL value in \a ANATSEIL register.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_StartTempMeasurement(), Cy_SCU_SetRawTempLimits(), Cy_SCU_GetTemperature(), Cy_SCU_HighTemperature() \n\n\n
 */
bool Cy_SCU_LowTemperature(void);

/**
 * @return uint32_t  Raw die temperature value. \b Range: 16 bit value.
 *
 * \par<b>Description</b><br>
 * Provides the raw die temperature value.\n\n
 * The API reads temperature measurement result from \a SCU_ANALOG->ANATSEMON bit fields.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_StartTempMeasurement() \n\n\n
 */
uint32_t Cy_SCU_GetTemperature(void);

/**
 * @return uint32_t  Calculate die temperature value. \b Range: 16 bit value.
 *
 * \par<b>Description</b><br>
 * Calculates the die temperature value using ROM function.\n\n
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_StartTempMeasurement() \n\n\n
 */
uint32_t Cy_SCU_CalcTemperature(void);

/**
 *
 * @return true DTS Measurement Done
 * @return false DTS Measurement not Done
 *
 * \par<b>Description</b><br>
 * This functions checks the status of the DTS Measurement completion.\n\n
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_StartTempMeasurement() \n\n\n
 */
__STATIC_INLINE bool Cy_SCU_IsTempMeasurementDone(void)
{
  return ((SCU_INTERRUPT->SRRAW & SCU_INTERRUPT_SRRAW_TSE_DONE_Msk) != 0U);
}

/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Trigger device master reset.\n\n
 * The API triggers master reset by setting the \a MRSTEN bit of \a RSTCON register.
 * It also internally triggers system reset. Almost all the logics of the device are affected by this reset.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_RESET_EnableResetRequest() \n\n\n
 */
__STATIC_INLINE void Cy_SCU_RESET_AssertMasterReset(void)
{
  SCU_RESET->RSTCON |= SCU_RESET_RSTCON_MRSTEN_Msk;
}
/**
 *
 * @param request  Reset source to trigger the device reset.\n
 *          \b Range: Use type @ref Cy_SCU_SYSTEM_RESET_REQUEST_t to identify the reset source.\n
 *          CY_SCU_RESET_REQUEST_FLASH_ECC_ERROR- Reset when flash memory double bit error is detected.\n
 *          CY_SCU_RESET_REQUEST_CLOCK_LOSS- Reset when loss of clock is detected.\n
 *          CY_SCU_RESET_REQUEST_SRAM_PARITY_ERROR- Reset when SRAM parity error is detected.\n
 *          CY_SCU_RESET_REQUEST_USIC_SRAM_PARITY_ERROR- Reset when USIC0 SRAM parity error is detected.\n
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Configures trigger for system reset from the selected source.\n\n
 * The API configures the reset source specific bit in the \a RSTCON register.
 * Multiple reset sources can be combined using \a OR operation. By enabling
 * the reset using this API will not trigger the reset. The reset will happen when
 * the configured source event is detected.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_RESET_AssertMasterReset() \n\n\n
 */
__STATIC_INLINE void Cy_SCU_RESET_EnableResetRequest(uint32_t request)
{
  SCU_RESET->RSTCON |= request;
}

/**
 *
 * @return uint32_t  Fast peripheral clock frequency in Hertz.
 *
 * \par<b>Description</b><br>
 * Provides the clock frequency of peripherals on the peripheral bus that are using a shared functional clock.\n\n
 * The value is derived using the bitfield \a PCLKSEL from \a CLKCR register. Peripheral clock
 * can have 2 times the frequency of system clock if the \a PCLKSEL is set.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_CLOCK_SetFastPeripheralClockSource() \n\n\n
 */
uint32_t Cy_SCU_CLOCK_GetFastPeripheralClockFrequency(void);

/**
 *
 * @param source  Fast peripheral clock source.\n
 *          \b Range: Use type @ref Cy_SCU_CLOCK_PCLKSRC_t to identify the clock source.\n
 *          CY_SCU_CLOCK_PCLKSRC_MCLK- Use MCLK as the peripheral clock.\n
 *          CY_SCU_CLOCK_PCLKSRC_DOUBLE_MCLK- peripheral clock will be 2 times the MCLK frequency.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Configures the source of peripheral clock. \n\n
 * The peripheral clock can be either same as MCLK or twice the frequency of MCLK.
 * \par<b>Related APIs:</b><BR>
 * Cy_SCU_CLOCK_GetFastPeripheralClockFrequency() \n\n\n
 */
void Cy_SCU_CLOCK_SetFastPeripheralClockSource(const Cy_SCU_CLOCK_PCLKSRC_t source);

/**
 *
 * @param temperature  measured temperature using the on-chip temperature sensor
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * DCO1 clock frequency can be calibrated during runtime to achieve a better accuracy.
 * This function start the DCO1 calibration based on temperature.
 *
 */
void Cy_SCU_CLOCK_CalibrateOscillatorOnTemperature(int32_t temperature);

/**
 *
 * @param adcclk_src Clock source selected as external reference. @ref Cy_SCU_CLOCK_ADCCLKSRC_t
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * ADC converter clock (fCONV) selection
 *
 */
void Cy_SCU_CLOCK_SetAdcClockSrc(Cy_SCU_CLOCK_ADCCLKSRC_t adcclk_src);

/**
 *
 * @param sync_clk Clock source selected as external reference. @ref Cy_SCU_CLOCK_SYNC_CLKSRC_t
 * @param prescaler integer(\f$\frac{syn_preload \times f_{OSC}[MHz]}{48}\f$)
 * @param syn_preload max. value 0x1FFF integer(\f$\frac{48 \times prescaler}{f_{OSC}[MHz]}\f$)
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * DCO1 clock frequency can be calibrated during runtime to achieve a better accuracy.
 * This function starts the automatic DCO1 calibration based on the selected clock source.
 *
 */
void Cy_SCU_CLOCK_EnableDCO1ExtRefCalibration(Cy_SCU_CLOCK_SYNC_CLKSRC_t sync_clk, uint32_t prescaler, uint32_t syn_preload);

/**
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * This function stops the automatic DCO1 calibration based on the selected clock source.
 *
 */
void Cy_SCU_CLOCK_DisableDCO1ExtRefCalibration(void);

/**
 *
 * @return true DCO1 is synchronized to the selected XTAL frequency
 * @return false Actual DCO1 frequency is out of target
 *
 * \par<b>Description</b><br>
 * This functions checks the status of the synchronisation
 *
 */
bool Cy_SCU_CLOCK_IsDCO1ExtRefCalibrationReady(void);

/**
 * @param mode Oscillator mode. @ref Cy_SCU_CLOCK_OSCHP_MODE_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Configure functional mode of the OSCHP.
 */
void Cy_SCU_CLOCK_SetHighPerformanceOscillatorMode(Cy_SCU_CLOCK_OSCHP_MODE_t mode);

/**
 * @param mode Oscillator mode. @ref Cy_SCU_CLOCK_OSCLP_MODE_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Configure functional mode of the OSCLP.
 */
void Cy_SCU_CLOCK_SetLowPerformanceOscillatorMode(Cy_SCU_CLOCK_OSCLP_MODE_t mode);

/**
 * This function selects service request source for a NVIC interrupt node.
 * The PSC1M series has up to 54 peripheral service requests.
 * The Cortex M0 however has 32 interrupts available for peripherals.
 * This function allows you to select which 32 peripheral requests of the 54 the CPU should react on.
 *
 * @param irq_number Interrupt number, 0 to 31
 * @param source Peripheral service request. See @ref Cy_SCU_IRQCTRL_t
 *
 */
void Cy_SCU_SetInterruptControl(uint8_t irq_number, Cy_SCU_IRQCTRL_t source);

/**
 * This function enables the Prefetch Unit (PFU).
 * The purpose of the Prefetch unit is to reduce the Flash latency gap at higher system
 * frequencies to increase the instruction per cycle performance.
 *
 */
__STATIC_INLINE void Cy_SCU_EnablePrefetchUnit(void)
{
  SCU_GENERAL->PFUCR &= ~SCU_GENERAL_PFUCR_PFUBYP_Msk;
}

/**
 * This function disables the Prefetch Unit (PFU).
 * The purpose of the Prefetch unit is to reduce the Flash latency gap at higher system
 * frequencies to increase the instruction per cycle performance.
 *
 */
__STATIC_INLINE void Cy_SCU_DisablePrefetchUnit(void)
{
  SCU_GENERAL->PFUCR |= SCU_GENERAL_PFUCR_PFUBYP_Msk;
}

/**
 * @param range VDEL Range Select ::Cy_SCU_POWER_MONITOR_RANGE_t
 * @param delay VDEL Timing Setting ::Cy_SCU_POWER_MONITOR_DELAY_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables VDEL detector. VDEL detector compares the supply voltage against a pre-warning threshold voltage
 *
 * @note Brown Out Trap need to be enabled previously
 */
__STATIC_INLINE void Cy_SCU_POWER_EnableMonitor(Cy_SCU_POWER_MONITOR_RANGE_t range, Cy_SCU_POWER_MONITOR_DELAY_t delay)
{
  SCU_ANALOG->ANAVDEL = SCU_ANALOG_ANAVDEL_VDEL_EN_Msk |
                        (uint32_t)range |
                        (uint32_t)delay;
}

/**
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables VDEL detector
 */
__STATIC_INLINE void Cy_SCU_POWER_DisableMonitor(void)
{
  SCU_ANALOG->ANAVDEL &= ~SCU_ANALOG_ANAVDEL_VDEL_EN_Msk;
}

/**
 *
 * @param flags may be any of: <br>
 *             \ref CY_SCU_BMI_HWCFG_CAN_BSL (only available for PSC1M series with CAN Module)<br>
 *             \ref CY_SCU_BMI_HWCFG_CAN_BSLTO (only available for PSC1M series with CAN Module)<br>
 *             \ref CY_SCU_BMI_HWCFG_SBSL_CANOPEN (only available for PSC1M series with CAN Module)<br>
 *             \ref CY_SCU_BMI_HWCFG_ASC_BSL<br>
 *             \ref CY_SCU_BMI_HWCFG_UPM<br>
 *             \ref CY_SCU_BMI_HWCFG_UMD<br>
 *             \ref CY_SCU_BMI_HWCFG_UMHAR<br>
 *             \ref CY_SCU_BMI_HWCFG_SSC_BSL<br>
 *             \ref CY_SCU_BMI_HWCFG_ASC_BSLTO<br>
 *             \ref CY_SCU_BMI_HWCFG_SSC_BSLTO<br>
 *             \ref CY_SCU_BMI_HWCFG_SBSL<br><br>
 *        optionally OR'd together with any of: <br>
 *             \ref CY_SCU_BMI_HWCFG_PINDIS
 *        optionally OR'd together with any of: <br>
 *             \ref CY_SCU_BMI_DAPTYP_SWD<br>
 *             \ref CY_SCU_BMI_DAPTYP_SPD<br><br>
 *        optionally OR'd together with any of: <br>
 *             \ref CY_SCU_BMI_DAPDIS_CHANNEL_0<br>
 *             \ref CY_SCU_BMI_DAPDIS_CHANNEL_1<br><br>
 * @image html "psc1m3_debugif.png"
 *        optionally OR'd together with any of (only available for PSC1M series with CAN Module): <br>
 *             \ref CY_SCU_BMI_CANCLK_DCO1<br>
 *             \ref CY_SCU_BMI_CANCLK_OSCHP<br><br>
 *
 * @param timeout Only relevant if a start up mode is selected that uses timeout. The time-out duration is BSLTO*2664000 MCLK cycles, the supported time-out range is 0.3-5s (333...4995ms)
 * @return false only upon error, if OK the procedure triggers a reset and does not return to calling routine
 *
 * \par<b>Description</b><br>
 * This procedure initiates installation of a new BMI value. In particular, it can be used as
 * well as to restore the state upon delivery for a device already in User Productive mode.
 *
 * @code
 *   // Switch to ASC Bootstrap Loader
 *   Cy_SCU_SetBMI(CY_SCU_BMI_HWCFG_ASC_BSL, 0);
 *
 *   // Switch to Debug user mode SWD1 (pins P1.3 and P1.2)
 *   Cy_SCU_SetBMI(CY_SCU_BMI_HWCFG_UMD | CY_SCU_BMI_DAPTYP_SWD | CY_SCU_BMI_DAPDIS_CHANNEL_1, 0);
 * @endcode
 */
uint32_t Cy_SCU_SetBMI(uint32_t flags, uint8_t timeout);

/**
 *
 * @return uint32_t Current BMI value.
 *
 * \par<b>Description</b><br>
 * This procedure initiates installation of a new BMI value. In particular, it can be used as
 * well as to restore the state upon delivery for a device already in User Productive mode.
 *
 * @code
 *   // Switch to ASC Bootstrap Loader
 *   bmi_value = Cy_SCU_GetBMI();
 *
 *   if ((bmi_value & 0x000000ffU) != CY_SCU_BMI_HWCFG_ASC)
 *   {
 *     Cy_SCU_SetBMI(CY_SCU_BMI_HWCFG_ASC, 0);
 *   }
 *
 * @endcode
 */
__STATIC_INLINE uint32_t Cy_SCU_GetBMI(void)
{
  return *(volatile uint32_t *)CY_BMI_ADDR;
}

/*
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * This function enables flash power down when entering power save mode (SLEEP or DEEPSLEEP modes).
 * Upon wake-up, CPU is able to fetch code from flash.
 *
 * @usage
 * @code
 *
 * // The clock of the peripherals that are not needed during sleep state can be gated before entering sleep state
 * Cy_SCU_CLOCK_GatePeripheralClock(SCU_CLK_CGATSTAT0_MATH_Msk);
 *
 * // Enable FLASH power down during SLEEP and DEEPSLEEP mode
 * Cy_SCU_CLOCK_EnableFlashPowerDown();
 *
 * // Make sure that SLEEPDEEP bit is set
 * SCB->SCR |= SCB_SCR_DEEPSLEEP_Msk;
 *
 * // Return to SLEEP mode after handling the wakeup event
 * SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
 *
 * // Put system in DEEPSLEEP state
 * __WFI();
 *
 * @endcode
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_FLASH_EnterSleepMode()
 */
__STATIC_INLINE void Cy_SCU_CLOCK_EnableFlashPowerDown(void)
{
  SCU_CLK->PWRSVCR = SCU_CLK_PWRSVCR_FPD_Msk;
}

/*
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * This function disables flash power down when entering power save mode (SLEEP or DEEPSLEEP modes).
 * \par<b>Related APIs:</b><BR>
 * Cy_FLASH_EnterSleepMode()
 */
__STATIC_INLINE void Cy_SCU_CLOCK_DisableFlashPowerDown(void)
{
  SCU_CLK->PWRSVCR = 0;
}

/**
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * This function enables the watchdog on the DCO1 frequency
 *
 */
__STATIC_INLINE void Cy_SCU_CLOCK_EnableDCO1OscillatorWatchdog(void)
{
  SCU_CLK->OSCCSR |= SCU_CLK_OSCCSR_OWDEN_Msk;
}

/**
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * This function disables the watchdog on the DCO1 frequency
 *
 */
__STATIC_INLINE void Cy_SCU_CLOCK_DisableDCO1OscillatorWatchdog(void)
{
  SCU_CLK->OSCCSR &= ~SCU_CLK_OSCCSR_OWDEN_Msk;
}

/**
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * This function clears the status of the watchdog on the DCO1 frequency
 *
 */
__STATIC_INLINE void Cy_SCU_CLOCK_ClearDCO1OscillatorWatchdogStatus(void)
{
  SCU_CLK->OSCCSR |= SCU_CLK_OSCCSR_OWDRES_Msk;
}

/*
 *
 * @return true The OSC frequency is usable
 * @return false The OSC frequency is not usable. Frequency is too high or too low
 *
 * \par<b>Description</b><br>
 * This function checks if the DCO1 frequency is in the limits of the watchdog
 *
 */
__STATIC_INLINE bool Cy_SCU_CLOCK_IsDCO1ClockFrequencyUsable(void)
{
  return ((SCU_CLK->OSCCSR & (SCU_CLK_OSCCSR_OSC2L_Msk | SCU_CLK_OSCCSR_OSC2H_Msk)) == 0U);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */


#endif /* SCU_H */
