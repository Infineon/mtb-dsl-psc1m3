/**
 * @file cy_ccu4.h
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
 */

#ifndef CY_CCU4_H
#define CY_CCU4_H

/**
 * @addtogroup CCU4
 * @brief Capture Compare Unit 4 (CCU4) low level driver for PSC1M family of microcontrollers<br>
 *
 * The CCU4 peripheral is a major component for systems that need general purpose timers for signal
 * monitoring/conditioning and Pulse Width Modulation (PWM) signal generation. Power electronic control systems like
 * switched mode power supplies or interruptible power supplies, can easily be implemented with the functions inside the
 * CCU4 peripheral.\n
 * Each CCU4 module is comprised of four identical 16 bit Capture/Compare Timer slices, CC4y (where y = [0..4]). Each
 * timer slice can work in compare mode or in capture mode.
 *
 * APIs provided in this file cover the following functional blocks of CCU4:\n
 * -- Timer configuration, Capture configuration, Function/Event configuration, Interrupt configuration\n
 * \par Note:
 * 1. SLICE (APIs prefixed with e.g. CY_CCU4_SLICE_)
 * 2. Module (APIs are not having any prefix e.g. CY_CCU4_)
 *
 * \par Timer(Compare mode) configuration:
 * This section of the LLD provides the configuration structure Cy_CCU4_SLICE_COMPARE_CONFIG_t and the initialization
 * function Cy_CCU4_SLICE_CompareInit().
 *
 * It can be used to:
 * -# Start and Stop the timer. (Cy_CCU4_SLICE_StartTimer(), Cy_CCU4_SLICE_StopTimer())
 * -# Update the period, compare, Dither, Prescaler  and Passive values. (Cy_CCU4_SLICE_SetTimerPeriodMatch(),
 *    Cy_CCU4_SLICE_SetTimerCompareMatch(), Cy_CCU4_SLICE_SetPrescaler(), Cy_CCU4_SLICE_SetDitherCompareValue(),
 *    Cy_CCU4_SLICE_SetPassiveLevel())
 * -# Enable the slices to support multichannel mode. (Cy_CCU4_SLICE_EnableMultiChannelMode())
 *
 * \par Capture configuration:
 * This section of the LLD provides the configuration structure Cy_CCU4_SLICE_CAPTURE_CONFIG_t and the initialization
 * function Cy_CCU4_SLICE_CaptureInit().
 *
 * It can be used to:
 * -# Configure the capture functionality. (Cy_CCU4_SLICE_Capture0Config(), Cy_CCU4_SLICE_Capture1Config())
 * -# Read the captured values along with the status, which indicate the value is latest or not.
 *    (Cy_CCU4_SLICE_GetCaptureRegisterValue())
 *
 * \par Function/Event configuration:
 * This section of the LLD provides the configuration structure Cy_CCU4_SLICE_EVENT_CONFIG_t.\n
 *
 * It can be used to:
 * -# Enable and Disable the events. (Cy_CCU4_SLICE_EnableEvent(), Cy_CCU4_SLICE_DisableEvent())
 * -# Configure to start and stop the timer on external events.(Cy_CCU4_SLICE_StartConfig(), Cy_CCU4_SLICE_StopConfig())
 * -# Modulation, external load and Gating of the timer output.(Cy_CCU4_SLICE_ModulationConfig(),
 *    Cy_CCU4_SLICE_LoadConfig(), Cy_CCU4_SLICE_GateConfig())
 * -# Control the count direction of the timer based on the external event. (Cy_CCU4_SLICE_DirectionConfig())
 * -# Count the external events.(Cy_CCU4_SLICE_CountConfig())
 * -# External Trap. Which can be used as protective feature.(Cy_CCU4_SLICE_EnableTrap(), Cy_CCU4_SLICE_DisableTrap(),
 *    Cy_CCU4_SLICE_TrapConfig())
 *
 * \par Interrupt configuration:
 * This section of the LLD provides the function to configure the interrupt node to each event (Cy_CCU4_SLICE_SetInterruptNode())
 *
 * Also, the CCU4 block can be configured from the ModusToolbox&trade; Device Configurator: \ref section_personality_ccu4
 * @{
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_common.h"
#if defined(CCU40) || defined(CY_DOXYGEN)

#include "cy_ccu4_map.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
/* Macro to check if the interrupt enum passed is valid */
#define CY_CCU4_SLICE_CHECK_INTERRUPT(interrupt) \
    ((interrupt == CY_CCU4_SLICE_IRQ_ID_PERIOD_MATCH)           || \
     (interrupt == CY_CCU4_SLICE_IRQ_ID_ONE_MATCH)              || \
     (interrupt == CY_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP)  || \
     (interrupt == CY_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_DOWN)|| \
     (interrupt == CY_CCU4_SLICE_IRQ_ID_EVENT0)                 || \
     (interrupt == CY_CCU4_SLICE_IRQ_ID_EVENT1)                 || \
     (interrupt == CY_CCU4_SLICE_IRQ_ID_EVENT2)                 || \
     (interrupt == CY_CCU4_SLICE_IRQ_ID_TRAP))

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/**
 * Typedef for CCU4 Global data structure
 */
typedef CCU4_GLOBAL_TypeDef Cy_CCU4_MODULE_t;

/**
 * Typedef for CCU4 Slice data structure
 */
typedef CCU4_CC4_TypeDef Cy_CCU4_SLICE_t;

/**
 *  Return Value of an API
 */
typedef enum CY_CCU4_STATUS
{
  CY_CCU4_STATUS_OK      = 0U, /**< API fulfils request */
  CY_CCU4_STATUS_ERROR,        /**< API cannot fulfil the request */
  CY_CCU4_STATUS_RUNNING,      /**< The timer slice is currently running */
  CY_CCU4_STATUS_IDLE          /**< The timer slice is currently idle */
} Cy_CCU4_STATUS_t;

/**
 *  CCU4 module clock
 */
typedef enum CY_CCU4_CLOCK
{
  CY_CCU4_CLOCK_SCU        = 0U, /**< Select the fCCU as the clock */
  CY_CCU4_CLOCK_EXTERNAL_A,      /**< External clock-A */
  CY_CCU4_CLOCK_EXTERNAL_B,      /**< External clock-B */
  CY_CCU4_CLOCK_EXTERNAL_C       /**< External clock-C */
} Cy_CCU4_CLOCK_t;

/**
 * CCU4 set the shadow transfer type for multichannel mode
 */
typedef enum CY_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER
{
  CY_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_SW_SLICE0       = (uint32_t)0x4000000, /**< Shadow transfer through software
                                                                                     only for slice 0*/
  CY_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_SW_MCSS_SLICE0  = (uint32_t)0x4000400, /**< Shadow transfer through software
                                                                                     and hardware for slice 0 */
  CY_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_SW_SLICE1       = (uint32_t)0x8000000, /**< Shadow transfer through software
                                                                                     only for slice 1*/
  CY_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_SW_MCSS_SLICE1  = (uint32_t)0x8000800, /**< Shadow transfer through software
                                                                                     and hardware for slice 1 */
  CY_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_SW_SLICE2       = (uint32_t)0x10000000, /**< Shadow transfer through software
                                                                                      only for slice 2 */
  CY_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_SW_MCSS_SLICE2  = (uint32_t)0x10001000, /**< Shadow transfer through software
                                                                                      and hardware for slice 2 */
  CY_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_SW_SLICE3       = (uint32_t)0x20000000, /**< Shadow transfer through software
                                                                                      only for slice 3*/
  CY_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_SW_MCSS_SLICE3  = (uint32_t)0x20002000  /**< Shadow transfer through software
                                                                                      and hardware for slice 3 */
} Cy_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_t;

/**
 *  Operational modes of CCU4 slice
 */
typedef enum CY_CCU4_SLICE_MODE
{
  CY_CCU4_SLICE_MODE_COMPARE  = 0U, /**< slice(CC4y) operates in Compare Mode */
  CY_CCU4_SLICE_MODE_CAPTURE        /**< slice(CC4y) operates in Capture Mode */
} Cy_CCU4_SLICE_MODE_t;

/**
 *  Timer counting modes for the slice
 */
typedef enum CY_CCU4_SLICE_TIMER_COUNT_MODE
{
  CY_CCU4_SLICE_TIMER_COUNT_MODE_EA  = 0U, /**< Edge Aligned Mode */
  CY_CCU4_SLICE_TIMER_COUNT_MODE_CA        /**< Center Aligned Mode */
} Cy_CCU4_SLICE_TIMER_COUNT_MODE_t;

/**
 *  Timer repetition mode for the slice
 */
typedef enum CY_CCU4_SLICE_TIMER_REPEAT_MODE
{
  CY_CCU4_SLICE_TIMER_REPEAT_MODE_REPEAT = 0U,  /**< Repetitive mode: continuous mode of operation */
  CY_CCU4_SLICE_TIMER_REPEAT_MODE_SINGLE        /**< Single shot mode: Once a Period match/One match
                                                      occurs timer goes to idle state */
} Cy_CCU4_SLICE_TIMER_REPEAT_MODE_t;

/**
 * Timer counting direction for the CCU4 slice
 */
typedef enum CY_CCU4_SLICE_TIMER_COUNT_DIR
{
  CY_CCU4_SLICE_TIMER_COUNT_DIR_UP   = 0U, /**< Counting up */
  CY_CCU4_SLICE_TIMER_COUNT_DIR_DOWN       /**< Counting down */
} Cy_CCU4_SLICE_TIMER_COUNT_DIR_t;

/**
 *  Capture mode register sets
 */
typedef enum CY_CCU4_SLICE_CAP_REG_SET
{
  CY_CCU4_SLICE_CAP_REG_SET_LOW  = 0U, /**< Capture register-0, Capture register-1 used */
  CY_CCU4_SLICE_CAP_REG_SET_HIGH       /**< Capture register-2, Capture register-3 used */
} Cy_CCU4_SLICE_CAP_REG_SET_t;

/**
 *  Prescaler mode
 */
typedef enum CY_CCU4_SLICE_PRESCALER_MODE
{
  CY_CCU4_SLICE_PRESCALER_MODE_NORMAL = 0U, /**< Fixed division of module clock */
  CY_CCU4_SLICE_PRESCALER_MODE_FLOAT        /**< Floating divider. */
} Cy_CCU4_SLICE_PRESCALER_MODE_t;

/**
 *  Timer output passive level
 */
typedef enum CY_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL
{
  CY_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW  = 0U, /**< Passive level = Low */
  CY_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH       /**< Passive level = High */
} Cy_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_t;

/**
 * Timer clock Divider
 */
typedef enum CY_CCU4_SLICE_PRESCALER
{
  CY_CCU4_SLICE_PRESCALER_1  = 0U, /**< Slice Clock = fccu4 */
  CY_CCU4_SLICE_PRESCALER_2,       /**< Slice Clock = fccu4/2 */
  CY_CCU4_SLICE_PRESCALER_4,       /**< Slice Clock = fccu4/4 */
  CY_CCU4_SLICE_PRESCALER_8,       /**< Slice Clock = fccu4/8 */
  CY_CCU4_SLICE_PRESCALER_16,      /**< Slice Clock = fccu4/16 */
  CY_CCU4_SLICE_PRESCALER_32,      /**< Slice Clock = fccu4/32 */
  CY_CCU4_SLICE_PRESCALER_64,      /**< Slice Clock = fccu4/64 */
  CY_CCU4_SLICE_PRESCALER_128,     /**< Slice Clock = fccu4/128 */
  CY_CCU4_SLICE_PRESCALER_256,     /**< Slice Clock = fccu4/256 */
  CY_CCU4_SLICE_PRESCALER_512,     /**< Slice Clock = fccu4/512 */
  CY_CCU4_SLICE_PRESCALER_1024,    /**< Slice Clock = fccu4/1024 */
  CY_CCU4_SLICE_PRESCALER_2048,    /**< Slice Clock = fccu4/2048 */
  CY_CCU4_SLICE_PRESCALER_4096,    /**< Slice Clock = fccu4/4096 */
  CY_CCU4_SLICE_PRESCALER_8192,    /**< Slice Clock = fccu4/8192 */
  CY_CCU4_SLICE_PRESCALER_16384,   /**< Slice Clock = fccu4/16384 */
  CY_CCU4_SLICE_PRESCALER_32768    /**< Slice Clock = fccu4/32768 */
} Cy_CCU4_SLICE_PRESCALER_t;

/**
 *  External Function list
 */
typedef enum CY_CCU4_SLICE_FUNCTION
{
  CY_CCU4_SLICE_FUNCTION_START           = 0U, /**< Start function */
  CY_CCU4_SLICE_FUNCTION_STOP,                 /**< Stop function */
  CY_CCU4_SLICE_FUNCTION_CAPTURE_EVENT0, /**< Capture Event-0 function, CCycapt0 signal is used for event
                                                     generation */
  CY_CCU4_SLICE_FUNCTION_CAPTURE_EVENT1, /**< Capture Event-1 function, CCycapt1 signal is used for event
                                                     generation */
  CY_CCU4_SLICE_FUNCTION_GATING,               /**< Gating function */
  CY_CCU4_SLICE_FUNCTION_DIRECTION,            /**< Direction function */
  CY_CCU4_SLICE_FUNCTION_LOAD,                 /**< Load function */
  CY_CCU4_SLICE_FUNCTION_COUNT,                /**< Counting function */
  CY_CCU4_SLICE_FUNCTION_OVERRIDE,             /**< Override function */
  CY_CCU4_SLICE_FUNCTION_MODULATION,           /**< Modulation function */
  CY_CCU4_SLICE_FUNCTION_TRAP                  /**< Trap function */
} Cy_CCU4_SLICE_FUNCTION_t;

/**
 *  External Event list
 */
typedef enum CY_CCU4_SLICE_EVENT
{
  CY_CCU4_SLICE_EVENT_NONE  = 0U, /**< None */
  CY_CCU4_SLICE_EVENT_0,          /**< Event-0 */
  CY_CCU4_SLICE_EVENT_1,          /**< Event-1 */
  CY_CCU4_SLICE_EVENT_2           /**< Event-2 */
} Cy_CCU4_SLICE_EVENT_t;

/**
 *  External Event trigger criteria  - Edge sensitivity
 */
typedef enum CY_CCU4_SLICE_EVENT_EDGE_SENSITIVITY
{
  CY_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_NONE         = 0U, /**< None */
  CY_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_RISING_EDGE,       /**< Rising Edge of the input signal generates event trigger*/
  CY_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_FALLING_EDGE, /**< Falling Edge of the input signal generates event
                                                                trigger */
  CY_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_DUAL_EDGE          /**< Both Rising and Falling edges cause an event trigger*/
} Cy_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_t;

/**
 *  External Event trigger criteria  - Level sensitivity
 */
typedef enum CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY
{
  CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH = 0U, /**< Level sensitive functions react to a high signal level*/
  CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW  = 1U, /**< Level sensitive functions react to a low signal level*/
  /* Below enum items can be utilised specific to the functionality */
  CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_COUNT_UP_ON_LOW = 0U, /**< Timer counts up, during Low state of the control signal */
  CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_COUNT_UP_ON_HIGH = 1U /**< Timer counts up, during High state of the control signal */
} Cy_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_t;

/**
 *  Low pass filter Configuration. The External Event input should be stable for a selected number of clock cycles.
 */
typedef enum CY_CCU4_SLICE_EVENT_FILTER
{
  CY_CCU4_SLICE_EVENT_FILTER_DISABLED  = 0U, /**< No Low Pass Filter */
  CY_CCU4_SLICE_EVENT_FILTER_3_CYCLES,       /**< 3 clock cycles */
  CY_CCU4_SLICE_EVENT_FILTER_5_CYCLES,       /**< 5 clock cycles */
  CY_CCU4_SLICE_EVENT_FILTER_7_CYCLES        /**< 7 clock cycles */
} Cy_CCU4_SLICE_EVENT_FILTER_t;

/**
 *  External Event Input list. This list depicts the possible input connections to the CCU4 slice.
 *  Interconnects are specific to each device.
 */
typedef uint8_t Cy_CCU4_SLICE_INPUT_t;

/**
 * Actions that can be performed upon detection of an external Timer STOP event
 */
typedef enum CY_CCU4_SLICE_END_MODE
{
  CY_CCU4_SLICE_END_MODE_TIMER_STOP       = 0U, /**< Stops the timer, without clearing TIMER register */
  CY_CCU4_SLICE_END_MODE_TIMER_CLEAR,           /**< Without stopping timer, clears the TIMER register */
  CY_CCU4_SLICE_END_MODE_TIMER_STOP_CLEAR       /**< Stops the timer and clears the TIMER register */
} Cy_CCU4_SLICE_END_MODE_t;

/**
 *  Actions that can be performed upon detection of an external Timer START event
 */
typedef enum CY_CCU4_SLICE_START_MODE
{
  CY_CCU4_SLICE_START_MODE_TIMER_START       = 0U, /**< Start the timer from the current count of TIMER register */
  CY_CCU4_SLICE_START_MODE_TIMER_START_CLEAR       /**< Clears the TIMER register and start the timer */
} Cy_CCU4_SLICE_START_MODE_t;

/**
 *  Modulation of timer output signals
 */
typedef enum CY_CCU4_SLICE_MODULATION_MODE
{
  CY_CCU4_SLICE_MODULATION_MODE_CLEAR_ST_OUT  = 0U, /**< Clear ST and OUT signals */
  CY_CCU4_SLICE_MODULATION_MODE_CLEAR_OUT           /**< Clear only the OUT signal */
} Cy_CCU4_SLICE_MODULATION_MODE_t;

/**
 *  Trap exit mode
 */
typedef enum CY_CCU4_SLICE_TRAP_EXIT_MODE
{
  CY_CCU4_SLICE_TRAP_EXIT_MODE_AUTOMATIC = 0U, /**< Clear trap state as soon as the trap signal is de-asserted */
  CY_CCU4_SLICE_TRAP_EXIT_MODE_SW              /**< Clear only when acknowledged by software */
} Cy_CCU4_SLICE_TRAP_EXIT_MODE_t;

/**
 *  Timer clear on capture
 */
typedef enum CY_CCU4_SLICE_TIMER_CLEAR_MODE
{
  CY_CCU4_SLICE_TIMER_CLEAR_MODE_NEVER    = 0U, /**< Never clear the timer on any capture event */
  CY_CCU4_SLICE_TIMER_CLEAR_MODE_CAP_HIGH,      /**< Clear only when timer value has been captured in C3V and C2V */
  CY_CCU4_SLICE_TIMER_CLEAR_MODE_CAP_LOW,       /**< Clear only when timer value has been captured in C1V and C0V */
  CY_CCU4_SLICE_TIMER_CLEAR_MODE_ALWAYS         /**< Always clear the timer upon detection of any capture event */
} Cy_CCU4_SLICE_TIMER_CLEAR_MODE_t;

/**
 *  Multi Channel Shadow transfer request configuration options
 */
typedef enum CY_CCU4_SLICE_MCMS_ACTION
{
  CY_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR          = 0U, /**< Transfer Compare and Period Shadow register values to
                                                                the actual registers upon MCS xfer request */
  CY_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP     = 1U, /**< Transfer Compare, Period and Prescaler Compare Shadow
                                                                register values to the actual registers upon MCS xfer
                                                                request */
  CY_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP_DIT = 3U  /**< Transfer Compare, Period ,Prescaler Compare and Dither
                                                                Compare register values to the actual registers upon
                                                                MCS xfer request */
} Cy_CCU4_SLICE_MCMS_ACTION_t;

/**
 * Available Interrupt Event Ids
 */
typedef enum CY_CCU4_SLICE_IRQ_ID
{
  CY_CCU4_SLICE_IRQ_ID_PERIOD_MATCH       = 0U,  /**< Period match counting up */
  CY_CCU4_SLICE_IRQ_ID_ONE_MATCH          = 1U,  /**< Period match -> One match counting down */
  CY_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP   = 2U,  /**< Compare match counting up */
  CY_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_DOWN = 3U,  /**< Compare match counting down */
  CY_CCU4_SLICE_IRQ_ID_EVENT0             = 8U,  /**< Event-0 occurrence */
  CY_CCU4_SLICE_IRQ_ID_EVENT1             = 9U,  /**< Event-1 occurrence */
  CY_CCU4_SLICE_IRQ_ID_EVENT2             = 10U, /**< Event-2 occurrence */
  CY_CCU4_SLICE_IRQ_ID_TRAP               = 11U  /**< Trap occurrence */
} Cy_CCU4_SLICE_IRQ_ID_t;

/**
 * Available Interrupt Event Ids, which is added to support multi event APIs
 */
typedef enum CY_CCU4_SLICE_MULTI_IRQ_ID
{
  CY_CCU4_SLICE_MULTI_IRQ_ID_PERIOD_MATCH       = 0x1U,   /**< Period match counting up */
  CY_CCU4_SLICE_MULTI_IRQ_ID_ONE_MATCH          = 0x2U,   /**< Period match -> One match counting down */
  CY_CCU4_SLICE_MULTI_IRQ_ID_COMPARE_MATCH_UP   = 0x4U,   /**< Compare match counting up */
  CY_CCU4_SLICE_MULTI_IRQ_ID_COMPARE_MATCH_DOWN = 0x8U,   /**< Compare match counting down */
  CY_CCU4_SLICE_MULTI_IRQ_ID_EVENT0             = 0x100U, /**< Event-0 occurrence */
  CY_CCU4_SLICE_MULTI_IRQ_ID_EVENT1             = 0x200U, /**< Event-1 occurrence */
  CY_CCU4_SLICE_MULTI_IRQ_ID_EVENT2             = 0x400U, /**< Event-2 occurrence */
} Cy_CCU4_SLICE_MULTI_IRQ_ID_t;

/**
 * Service Request Lines for CCU4. Event are mapped to these SR lines and these are used to generate the interrupt.
 */
typedef enum CY_CCU4_SLICE_SR_ID
{
  CY_CCU4_SLICE_SR_ID_0    = 0U, /**< Service Request Line-0 selected  */
  CY_CCU4_SLICE_SR_ID_1,         /**< Service Request Line-1 selected  */
  CY_CCU4_SLICE_SR_ID_2,         /**< Service Request Line-2 selected  */
  CY_CCU4_SLICE_SR_ID_3          /**< Service Request Line-3 selected  */
} Cy_CCU4_SLICE_SR_ID_t;

/**
 *  Slice shadow transfer options.
 */
typedef enum CY_CCU4_SHADOW_TRANSFER
{
  CY_CCU4_SHADOW_TRANSFER_SLICE_0            = CCU4_GCSS_S0SE_Msk, /**< Transfer Period, Compare and Passive Level
                                                                          shadow register values to actual registers for
                                                                          SLICE-0 */
  CY_CCU4_SHADOW_TRANSFER_DITHER_SLICE_0     = CCU4_GCSS_S0DSE_Msk, /**< Transfer Dither compare shadow register value
                                                                          to actual register for SLICE-0 */
  CY_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0  = CCU4_GCSS_S0PSE_Msk, /**< Transfer Prescaler shadow register value to
                                                                          actual register for SLICE-0 */
  CY_CCU4_SHADOW_TRANSFER_SLICE_1            = CCU4_GCSS_S1SE_Msk, /**< Transfer Period, Compare and Passive Level
                                                                          shadow register values to actual registers for
                                                                          SLICE-1 */
  CY_CCU4_SHADOW_TRANSFER_DITHER_SLICE_1     = CCU4_GCSS_S1DSE_Msk, /**< Transfer Dither compare shadow register value
                                                                          to actual registers for SLICE-1 */
  CY_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_1  = CCU4_GCSS_S1PSE_Msk, /**< Transfer Prescaler shadow register value to
                                                                          actual register for SLICE-1 */
  CY_CCU4_SHADOW_TRANSFER_SLICE_2            = CCU4_GCSS_S2SE_Msk, /**< Transfer Period, Compare and Passive Level
                                                                          shadow register values to actual registers for
                                                                          SLICE-2 */
  CY_CCU4_SHADOW_TRANSFER_DITHER_SLICE_2     = CCU4_GCSS_S2DSE_Msk, /**< Transfer Dither compare shadow register value
                                                                          to actual register for SLICE-2 */
  CY_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_2  = CCU4_GCSS_S2PSE_Msk, /**< Transfer Prescaler shadow register value to
                                                                          actual register for SLICE-2 */
  CY_CCU4_SHADOW_TRANSFER_SLICE_3            = CCU4_GCSS_S3SE_Msk, /**< Transfer Period, Compare and Passive Level
                                                                          shadow register values to actual registers for
                                                                          SLICE-3 */
  CY_CCU4_SHADOW_TRANSFER_DITHER_SLICE_3     = CCU4_GCSS_S3DSE_Msk, /**< Transfer Dither compare shadow register value
                                                                          to actual register for SLICE-3 */
  CY_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_3  = CCU4_GCSS_S3PSE_Msk  /**< Transfer Prescaler shadow register value to
                                                                          actual register for SLICE-3 */
} Cy_CCU4_SHADOW_TRANSFER_t;

#if defined(CY_CCU4V3) || defined(CY_DOXYGEN)
/**
 *  Slice shadow transfer mode options.
 */
typedef enum CY_CCU4_SLICE_SHADOW_TRANSFER_MODE
{
  CY_CCU4_SLICE_SHADOW_TRANSFER_MODE_IN_PERIOD_MATCH_AND_ONE_MATCH = 0U,  /**< Shadow transfer is done in Period Match and
                                                                                One match. */
  CY_CCU4_SLICE_SHADOW_TRANSFER_MODE_ONLY_IN_PERIOD_MATCH = 1U,  /**< Shadow transfer is done only in Period Match. */
  CY_CCU4_SLICE_SHADOW_TRANSFER_MODE_ONLY_IN_ONE_MATCH = 2U  /**< Shadow transfer is done only in One Match. */
} Cy_CCU4_SLICE_SHADOW_TRANSFER_MODE_t;


/**
 *  Immediate write into configuration register
 */
typedef enum CY_CCU4_SLICE_WRITE_INTO
{
  CY_CCU4_SLICE_WRITE_INTO_PERIOD_CONFIGURATION = CCU4_CC4_STC_IRPC_Msk,                  /**< Immediate or Coherent
                                                                                                Write into Period
                                                                                                Configuration */
  CY_CCU4_SLICE_WRITE_INTO_COMPARE_CONFIGURATION = CCU4_CC4_STC_IRCC_Msk,                 /**< Immediate or Coherent
                                                                                                Write into Compare
                                                                                                Configuration */
  CY_CCU4_SLICE_WRITE_INTO_PASSIVE_LEVEL_CONFIGURATION = CCU4_CC4_STC_IRLC_Msk,           /**< Immediate or Coherent
                                                                                                Write into Passive Level
	                                                                                            Configuration */
  CY_CCU4_SLICE_WRITE_INTO_DITHER_VALUE_CONFIGURATION = CCU4_CC4_STC_IRDC_Msk,            /**< Immediate or Coherent
                                                                                                Write into Dither Value
	                                                                                            Configuration */
  CY_CCU4_SLICE_WRITE_INTO_FLOATING_PRESCALER_VALUE_CONFIGURATION = CCU4_CC4_STC_IRFC_Msk /**< Immediate or Coherent
                                                                                                Write into Floating Prescaler
                                                                                                Value Configuration */
} Cy_CCU4_SLICE_WRITE_INTO_t;


/**
 *  Automatic Shadow Transfer request when writing into shadow register
 */
typedef enum CY_CCU4_SLICE_AUTOMAIC_SHADOW_TRANSFER_WRITE_INTO
{
  CY_CCU4_SLICE_AUTOMAIC_SHADOW_TRANSFER_WRITE_INTO_PERIOD_SHADOW = CCU4_CC4_STC_ASPC_Msk,            /**< Automatic Shadow
	                                                                                                        Transfer request when
                                                                                                            writing into Period
                                                                                                            Shadow Register */
  CY_CCU4_SLICE_AUTOMAIC_SHADOW_TRANSFER_WRITE_INTO_COMPARE_SHADOW = CCU4_CC4_STC_ASCC_Msk,           /**< Automatic Shadow
	                                                                                                        transfer request
	                                                                                                        when writing into
	                                                                                                        Compare Shadow Register */
  CY_CCU4_SLICE_AUTOMAIC_SHADOW_TRANSFER_WRITE_INTO_PASSIVE_LEVEL = CCU4_CC4_STC_ASLC_Msk,            /**< Automatic Shadow transfer
	                                                                                                        request when writing
                                                                                                            into Passive Level Register*/
  CY_CCU4_SLICE_AUTOMAIC_SHADOW_TRANSFER_WRITE_INTO_DITHER_SHADOW = CCU4_CC4_STC_ASDC_Msk,            /**< Automatic Shadow transfer
                                                                                                            request when writing
                                                                                                            into Dither Shadow Register */
  CY_CCU4_SLICE_AUTOMAIC_SHADOW_TRANSFER_WRITE_INTO_FLOATING_PRESCALER_SHADOW = CCU4_CC4_STC_ASFC_Msk /**< Automatic Shadow transfer
	                                                                                                        request when writing
                                                                                                            into Floating Prescaler Shadow
                                                                                                            register */

} Cy_CCU4_SLICE_AUTOMAIC_SHADOW_TRANSFER_WRITE_INTO_t;
#endif
/**
 *  Used to create Mask needed for Multi-channel Shadow transfer enable/disable
 */
typedef enum CY_CCU4_SLICE_MASK
{
  CY_CCU4_SLICE_MASK_SLICE_0  = 1U,  /**< SLICE-0 */
  CY_CCU4_SLICE_MASK_SLICE_1  = 2U,  /**< SLICE-1 */
  CY_CCU4_SLICE_MASK_SLICE_2  = 4U,  /**< SLICE-2 */
  CY_CCU4_SLICE_MASK_SLICE_3  = 8U   /**< SLICE-3 */
} Cy_CCU4_SLICE_MASK_t;

/**
 *  Selects suspend mode
 */
typedef enum CY_CCU4_SUSPEND_MODE
{
  CY_CCU4_SUSPEND_MODE_IGNORE = 0 << CCU4_GCTRL_SUSCFG_Pos, /**< Suspend request ignored. The module never enters in suspend */
  CY_CCU4_SUSPEND_MODE_STOPS_INMEDIATELLY = 1 << CCU4_GCTRL_SUSCFG_Pos, /**< Stops all the running slices immediately. Safe stop is not applied. */
  CY_CCU4_SUSPEND_MODE_SAFE_STOP = 2 << CCU4_GCTRL_SUSCFG_Pos, /**< Stops the block immediately and clamps all the outputs to PASSIVE state. Safe stop is applied. */
  CY_CCU4_SUSPEND_MODE_WAIT_ROLL_OVER = 3 << CCU4_GCTRL_SUSCFG_Pos /**< Waits for the roll over of each slice to stop and clamp the slices outputs. Safe stop is applied. */
} Cy_CCU4_SUSPEND_MODE_t;


/**
 *  CCU4 slice status bit
 */
typedef enum CY_CCU4_SLICE_STATUS_BIT
{
  CY_CCU4_SLICE_STATUS_BIT_PASSIVE = false, /**< Passive */
  CY_CCU4_SLICE_STATUS_BIT_ACTIVE  = true   /**< Active  */
} Cy_CCU4_SLICE_STATUS_BIT_t;


/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/**
 *  Configuration data structure of an External Event(Event-0/1/2).
 *  Needed to configure the various aspects of an External Event.
 *  This structure will not connect the external event with an external function.
 */
typedef struct CY_CCU4_SLICE_EVENT_CONFIG
{
  Cy_CCU4_SLICE_INPUT_t                   mapped_input; /**< Required input signal for the Event */
  Cy_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_t  edge;         /**< Select the event edge of the input signal.
                                                              This is needed for an edge sensitive External function.*/
  Cy_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_t level;        /**< Select the event level of the input signal.
                                                              This is needed for an level sensitive External function.*/
  Cy_CCU4_SLICE_EVENT_FILTER_t            duration;     /**< Low Pass filter duration in terms of fCCU clock cycles */
} Cy_CCU4_SLICE_EVENT_CONFIG_t;

/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined(__TASKING__)
#pragma warning 586
#endif

/**
 *  Configuration data structure for CCU4 slice. Specifically configures the CCU4 slice to compare mode operation.
 *  This excludes event and function configuration.
 */
typedef struct CY_CCU4_SLICE_COMPARE_CONFIG
{
  union
  {
    struct
    {
      uint32_t timer_mode : 1;         /**< Edge aligned or Centre Aligned.
                                             Accepts enum ::Cy_CCU4_SLICE_TIMER_COUNT_MODE_t */
      uint32_t monoshot : 1;           /**< Single shot or Continuous mode .
                                             Accepts enum :: Cy_CCU4_SLICE_TIMER_REPEAT_MODE_t*/
      uint32_t shadow_xfer_clear : 1;  /**< Should PR and CR shadow xfer happen when timer is cleared? */
      uint32_t : 10;
      uint32_t dither_timer_period: 1; /**< Can the period of the timer dither? */
      uint32_t dither_duty_cycle : 1;  /**< Can the compare match of the timer dither? */
      uint32_t : 1;
      uint32_t prescaler_mode: 1;      /**< Normal or floating prescaler mode.
                                             Accepts enum :: Cy_CCU4_SLICE_PRESCALER_MODE_t*/
      uint32_t : 8;
      uint32_t mcm_enable : 1;         /**< Multi-Channel mode enable? */
      uint32_t : 6;
    };
    uint32_t tc;
  };
  uint32_t prescaler_initval : 4;      /**< Initial prescaler divider value
                                            Accepts enum :: Cy_CCU4_SLICE_PRESCALER_t */
  uint32_t float_limit : 4;            /**< The max value which the prescaler divider can increment to */
  uint32_t dither_limit : 4;           /**< The value that determines the spreading of dithering */
  uint32_t passive_level : 1;          /**< Configuration of ST and OUT passive levels.
                                             Accepts enum :: Cy_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_t*/
  uint32_t timer_concatenation : 1;    /**< Enables the concatenation of the timer if true.*/
} Cy_CCU4_SLICE_COMPARE_CONFIG_t;

/**
 *  Configuration data structure for CCU4 slice. Specifically configures the CCU4 slice to capture mode operation.
 *  This excludes event and function configuration.
 */
typedef struct CY_CCU4_SLICE_CAPTURE_CONFIG
{
  union
  {
    struct
    {
      uint32_t : 4;
      uint32_t fifo_enable : 1;      /**< Should the capture registers be setup as a FIFO?(Extended capture mode)*/
      uint32_t timer_clear_mode : 2; /**< How should the timer register be cleared upon detection of capture event?
                                          Accepts enum ::Cy_CCU4_SLICE_TIMER_CLEAR_MODE_t*/
      uint32_t : 4;
      uint32_t same_event : 1;       /**< Should the capture event for C1V/C0V and C3V/C2V be same capture edge? */
      uint32_t ignore_full_flag : 1; /**< Should updates to capture registers follow full flag rules? */
      uint32_t : 3;
      uint32_t prescaler_mode: 1;    /**< Normal or floating prescaler Accepts enum :: Cy_CCU4_SLICE_PRESCALER_MODE_t*/
      uint32_t : 15;
    };
    uint32_t tc;
  };
  uint32_t prescaler_initval : 4;    /**< Prescaler divider value */
  uint32_t float_limit : 4;          /**< The max value which the prescaler divider can increment to */
  uint32_t timer_concatenation : 1;  /**< Enables the concatenation of the timer */
} Cy_CCU4_SLICE_CAPTURE_CONFIG_t;
/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
#pragma pop
#elif defined(__TASKING__)
#pragma warning restore
#endif
/*********************************************************************************************************************
 * API Prototypes
 ********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

__STATIC_INLINE bool Cy_CCU4_IsValidModule(const Cy_CCU4_MODULE_t *const module)
{
  bool tmp = false;

  tmp = (module == CCU40);

#if defined(CCU41)
  tmp = tmp || (module == CCU41);
#endif

#if defined(CCU42)
  tmp = tmp || (module == CCU42);
#endif

#if defined(CCU43)
  tmp = tmp || (module == CCU43);
#endif

  return tmp;
}

__STATIC_INLINE bool Cy_CCU4_IsValidSlice(const Cy_CCU4_SLICE_t *const slice)
{
  bool tmp = false;

  tmp = (slice == CCU40_CC40);
#if defined(CCU40_CC41)
  tmp = tmp || (slice == CCU40_CC41);
#endif
#if defined(CCU40_CC42)
  tmp = tmp || (slice == CCU40_CC42);
#endif
#if defined(CCU40_CC43)
  tmp = tmp || (slice == CCU40_CC43);
#endif
#if defined(CCU41)
  tmp = tmp || (slice == CCU41_CC40);
#if defined(CCU41_CC41)
  tmp = tmp || (slice == CCU41_CC41);
#endif
#if defined(CCU41_CC42)
  tmp = tmp || (slice == CCU41_CC42);
#endif
#if defined(CCU41_CC43)
  tmp = tmp || (slice == CCU41_CC43);
#endif
#endif
#if defined(CCU42)
  tmp = tmp || (slice == CCU42_CC40);
#if defined(CCU42_CC41)
  tmp = tmp || (slice == CCU42_CC41);
#endif
#if defined(CCU42_CC42)
  tmp = tmp || (slice == CCU42_CC42);
#endif
#if defined(CCU42_CC43)
  tmp = tmp || (slice == CCU42_CC43);
#endif
#endif
#if defined(CCU43)
  tmp = tmp || (slice == CCU43_CC40);
#if defined(CCU43_CC41)
  tmp = tmp || (slice == CCU43_CC41);
#endif
#if defined(CCU43_CC42)
  tmp = tmp || (slice == CCU43_CC42);
#endif
#if defined(CCU43_CC43)
  tmp = tmp || (slice == CCU43_CC43);
#endif
#endif

  return tmp;
}

/**
 * @param module Constant pointer to CCU4 module
 * @param mcs_action multi-channel shadow transfer request configuration
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Initialization of global register GCTRL.<br>\n
 *  As part of module initialization, behaviour of the module upon detection
 *  Multi-Channel Mode trigger is configured. Will also invoke the Cy_CCU4_EnableModule().
 *  The API call would bring up the required CCU4 module and also initialize the module for
 *  the required multi-channel shadow transfer.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_SLICE_CompareInit()<BR> Cy_CCU4_SLICE_CaptureInit().
 */
void Cy_CCU4_Init(Cy_CCU4_MODULE_t *const module, const Cy_CCU4_SLICE_MCMS_ACTION_t mcs_action);

/**
 * @param module Constant pointer to CCU4 module
 * @param clock Choice of input clock to the module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Selects the Module Clock by configuring GCTRL.PCIS bits.<BR>\n
 *  There are 3 potential clock sources. This API helps to select the required clock source.
 *  Call to this API is valid after the Cy_CCU4_Init().
 *
 * \par<b>Related APIs:</b><br>
 * None.<BR>
 */
void Cy_CCU4_SetModuleClock(Cy_CCU4_MODULE_t *const module, const Cy_CCU4_CLOCK_t clock);

/**
 * @param module Constant pointer to CCU4 module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the CCU4 module and brings it to active state.<BR>\n
 * Also disables the gating of the clock signal (if applicable depending on the device being selected).
 * Invoke this API before any operations are done on the CCU4 module. Invoked from  Cy_CCU4_Init().
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_SetModuleClock()<BR> Cy_CCU4_DisableModule()<BR> Cy_CCU4_StartPrescaler().
 */
void Cy_CCU4_EnableModule(Cy_CCU4_MODULE_t *const module);

/**
 * @param module Constant pointer to CCU4 module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Brings the CCU4 module to reset state and enables gating of the clock signal(if applicable depending
 * on the device being selected).<BR>\n
 * Invoke this API when a CCU4 module needs to be disabled completely.
 * Any operation on the CCU4 module will have no effect after this API is called.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_EnableModule()<BR> Cy_CCU4_DisableModule().
 */
void Cy_CCU4_DisableModule(Cy_CCU4_MODULE_t *const module);

/**
 * @param module Constant pointer to CCU4 module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Starts the prescaler and restores clocks to the timer slices, by setting GIDLC.SPRB bit.<BR>\n
 *  Once the input to the prescaler has been chosen and the prescaler divider of all slices programmed,
 *  the prescaler itself may be started. Invoke this  API after Cy_CCU4_Init()
 *  (Mandatory to fully initialize the module).Directly accessed register is GIDLC.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_Init()<BR> Cy_CCU4_EnableClock()<BR> Cy_CCU4_DisableClock()<BR> Cy_CCU4_StartPrescaler()<BR>
 * Cy_CCU4_StopPrescaler().
 */
__STATIC_INLINE void Cy_CCU4_StartPrescaler(Cy_CCU4_MODULE_t *const module)
{
  CY_ASSERT(Cy_CCU4_IsValidModule(module));
  module->GIDLC = (uint32_t) CCU4_GIDLC_SPRB_Msk;
}

/**
 * @param module Constant pointer to CCU4 module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Stops the prescaler and blocks clocks to the timer slices, by setting GIDLS.CPRB bit.<BR>\n
 * Opposite of the StartPrescaler routine.
 * Clears the run bit of the prescaler. Ensures that the module clock is not supplied to
 * the slices of the module.Registers directly accessed is GIDLS.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_EnableClock()<BR> Cy_CCU4_DisableClock()<BR> Cy_CCU4_StartPrescaler()<BR> Cy_CCU4_StopPrescaler().
 */
__STATIC_INLINE void Cy_CCU4_StopPrescaler(Cy_CCU4_MODULE_t *const module)
{
  CY_ASSERT(Cy_CCU4_IsValidModule(module));
  module->GIDLS = (uint32_t) CCU4_GIDLS_CPRB_Msk;
}

/**
 * @param module Constant pointer to CCU4 module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Returns the state of the prescaler, by reading GSTAT.PRB bit.<BR>\n
 * This will return true if the prescaler is running. If clock is being supplied to the slices of the
 * module then returns as true.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_StartPrescaler()<BR> Cy_CCU4_StopPrescaler()<BR> Cy_CCU4_EnableClock()<BR> Cy_CCU4_DisableClock().
 */
__STATIC_INLINE bool Cy_CCU4_IsPrescalerRunning(Cy_CCU4_MODULE_t *const module)
{
  CY_ASSERT(Cy_CCU4_IsValidModule(module));
  return ((bool)((module->GSTAT & (uint32_t) CCU4_GSTAT_PRB_Msk) == (uint32_t)CCU4_GSTAT_PRB_Msk));
}

/**
 * @param module Constant pointer to CCU4 module
 * @param clock_mask Slices whose clocks are to be enabled simultaneously.
 *                   Bit location 0/1/2/3 represents slice-0/1/2/3 respectively.
 *                   Range: [0x1 to 0xF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables clocks of multiple slices at a time, by configuring GIDLC.CS0I, GIDLC.CS1I, GIDLC.CS2I,
 * GIDLC.CS3I bits.\n\n
 * Takes an input clock_mask, which determines the slices that would receive the clock. Bring them out
 * of the idle state simultaneously.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_EnableClock()<BR> Cy_CCU4_DisableClock().
 */
__STATIC_INLINE void Cy_CCU4_EnableMultipleClocks(Cy_CCU4_MODULE_t *const module, const uint8_t clock_mask)
{
  CY_ASSERT(Cy_CCU4_IsValidModule(module));
  CY_ASSERT((clock_mask < 16U));

  module->GIDLC = (uint32_t) clock_mask;
}

/**
 * @param module Constant pointer to CCU4 module
 * @param slice_number Slice for which the clock should be Enabled.
 *                   Range: [0x0 to 0x3]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Enables the slice timer clock, by configuring GIDLC.CS0I, GIDLC.CS1I, GIDLC.CS2I,
 *  GIDLC.CS3I bits according to the selected \a slice_number.\n\n
 *  It is possible to enable/disable clock at slice level. This uses the \b slice_number to indicate the
 *  slice whose clock needs to be enabled.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_DisableClock()<BR> Cy_CCU4_EnableMultipleClocks()<BR> Cy_CCU4_StartPrescaler()<BR> Cy_CCU4_StopPrescaler().
 */
__STATIC_INLINE void Cy_CCU4_EnableClock(Cy_CCU4_MODULE_t *const module, const uint8_t slice_number)
{
  CY_ASSERT(Cy_CCU4_IsValidModule(module));
  CY_ASSERT((slice_number < 4U));

  module->GIDLC = ((uint32_t) 1) << slice_number;
}

/**
 * @param module Constant pointer to CCU4 module
 * @param slice_number Slice for which the clock should be disabled.
 *                   Range: [0x0 to 0x3]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the slice timer clock, by configuring GIDLS.SS0I, GIDLS.SSS1I, GIDLS.SSS2I,
 * GIDLS.SSS3I bits according to the selected \a slice_number .\n\n
 * It is possible to disable clock at slice level using the module pointer.
 * \b slice_number is used to disable the clock to a given slice of the module.
 * Directly accessed Register is GIDLS.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_EnableClock()<BR> Cy_CCU4_EnableMultipleClocks()<BR> Cy_CCU4_StartPrescaler()<BR> Cy_CCU4_StopPrescaler().
 */
__STATIC_INLINE void Cy_CCU4_DisableClock(Cy_CCU4_MODULE_t *const module, const uint8_t slice_number)
{
  CY_ASSERT(Cy_CCU4_IsValidModule(module));
  CY_ASSERT((slice_number < 4U));

  module->GIDLS = ((uint32_t) 1) << slice_number;
}

/**
 * @param module Constant pointer to CCU8 module
 * @param mode   Selects suspend mode
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the entering in suspend mode for all the CCU8 slices.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
__STATIC_INLINE void Cy_CCU4_SetSuspendMode(Cy_CCU4_MODULE_t *const module, const Cy_CCU4_SUSPEND_MODE_t mode)
{
  CY_ASSERT(Cy_CCU4_IsValidModule(module));
  module->GCTRL = (module->GCTRL & (uint32_t)~CCU4_GCTRL_SUSCFG_Msk) | mode;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param compare_init Pointer to slice configuration structure
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Initialization of a CC4 slice to compare mode, by configuring CC4yTC, CC4yCMC, CC4yPSC, CC4yDITH, CC4yPSL,
 * CC4yFPCS, CC4yCHC registers.\n\n
 * CC4 slice is configured with Timer configurations in this routine.
 * After initialization user has to explicitly enable the shadow transfer for the required values by calling
 * Cy_CCU4_EnableShadowTransfer() with appropriate mask.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
void Cy_CCU4_SLICE_CompareInit(Cy_CCU4_SLICE_t *const slice,
                                const Cy_CCU4_SLICE_COMPARE_CONFIG_t *const compare_init);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param capture_init Pointer to slice configuration structure
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Initialization of a CC4 slice to capture mode, by configuring CC4yTC, CC4yCMC, CC4yPSC,CC4yFPCS registers.\n\n
 * CC4 slice is configured with Capture configurations in  this routine.After initialization user has to explicitly
 * enable the shadow transfer for the required  values by calling Cy_CCU4_EnableShadowTransfer()
 * with appropriate mask.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_SLICE_Capture0Config()<BR> Cy_CCU4_SLICE_Capture1Config().
 */
void Cy_CCU4_SLICE_CaptureInit(Cy_CCU4_SLICE_t *const slice,
                                const Cy_CCU4_SLICE_CAPTURE_CONFIG_t *const capture_init);


/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Map an External event to the External Start Function
 * @param start_mode Behavior of slice when the start function is activated
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Configures the Start Function of the slice, by configuring CC4yCMC.ENDS and CC4yTC.ENDM bits.\n\n
 *  Start function is mapped with one of the 3 events. An external signal can control when a CC4 timer should start.
 *  Additionally, the behaviour of the slice upon activation of the start function is configured as well.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_SLICE_StopConfig()<BR> Cy_CCU4_SLICE_ConfigureEvent()<BR> Cy_CCU4_SLICE_SetInput().
 */
void Cy_CCU4_SLICE_StartConfig(Cy_CCU4_SLICE_t *const slice,
                                const Cy_CCU4_SLICE_EVENT_t event,
                                const Cy_CCU4_SLICE_START_MODE_t start_mode);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Map an External event to the External Stop Function
 * @param end_mode Behavior of slice when the stop function is activated
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Stop function for the slice, by configuring CC4yCMC.STRTS and CC4yTC.STRM bits.\n\n
 * Stop function is mapped with one of the 3 events. An external signal can control when a CCU4 timer should stop.
 * Additionally, the behaviour of the slice upon activation of the stop function is configured as well.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_SLICE_StartConfig()<BR> Cy_CCU4_SLICE_ConfigureEvent()<BR> Cy_CCU4_SLICE_SetInput().
 */
void Cy_CCU4_SLICE_StopConfig(Cy_CCU4_SLICE_t *const slice,
                               const Cy_CCU4_SLICE_EVENT_t event,
                               const Cy_CCU4_SLICE_END_MODE_t end_mode);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Map an External event to the External load Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Configures the Load Function for the slice, by configuring CC4yCMC.LDS bit.\n\n
 *  Load function is mapped with one of the 3 events. Up on occurrence of the event,\n
 *  if CC4yTCST.CDIR set to 0,CC4yTIMER register is reloaded with the value from compare register\n
 *  if CC4yTCST.CDIR set to 1,CC4yTIMER register is reloaded with the value from period register\n
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_ConfigureEvent()<BR> Cy_CCU4_SLICE_SetInput().
 */
void Cy_CCU4_SLICE_LoadConfig(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event);


/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Map an External event to the External Modulation Function
 * @param mod_mode Desired Modulation mode
 * @param synch_with_pwm Option to synchronize modulation with PWM start
 *                       Pass \b true if the modulation needs to be synchronized with PWM signal.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Output Modulation Function of the slice, by configuring CCeyCMC.MOS, CC4yTC.EMT and
 * CC4yTC.EMS bits.\n\n
 * Modulation function is mapped with one of the 3 events. The output signal of the CCU can
 * be modulated according to a external input.  Additionally, the behaviour of the slice upon activation
 * of the modulation function is configured as well.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_ConfigureEvent()<BR> Cy_CCU4_SLICE_SetInput().
 */
void Cy_CCU4_SLICE_ModulationConfig(Cy_CCU4_SLICE_t *const slice,
                                     const Cy_CCU4_SLICE_EVENT_t event,
                                     const Cy_CCU4_SLICE_MODULATION_MODE_t mod_mode,
                                     const bool synch_with_pwm);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Map an External event to the External Count Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Count Function of the slice, by configuring CC4yCMC.CNTS bit.\n\n
 * Count function is mapped with one of the 3 events. CCU4 slice can take an external
 * signal to act as the counting event. The CCU4 slice would count the
 * edges present on the \b event selected.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_ConfigureEvent()<BR> Cy_CCU4_SLICE_SetInput().
 */
void Cy_CCU4_SLICE_CountConfig(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event  Map an External event to the External Gating Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Gating Function of the slice, by configuring CC4yCMC.GATES bit.\n\n
 * Gating function is mapped with one of the 3 events.  A CCU4 slice can use an input signal that would
 * operate as counter gating. If the configured Active level is detected the counter will gate all the pulses.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_ConfigureEvent()<BR> Cy_CCU4_SLICE_SetInput().
 */
void Cy_CCU4_SLICE_GateConfig(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Map an External event to the Capture-0 Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Capture-0 Function of the slice, by configuring CC4yCMC.CAP0S bit.\n\n
 * Capture function is mapped with one of the 3 events.  A CCU4 slice can be configured into capture-0 mode
 * with the selected \b event. In this mode the CCU4 will capture the timer value into CC4yC0V and CC4yC1V.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_ConfigureEvent()<BR> Cy_CCU4_SLICE_SetInput().
 */
void Cy_CCU4_SLICE_Capture0Config(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Map an External event to the Capture-1 Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Capture-1 Function of the slice, by configuring CC4yCMC.CAP1S bit.\n\n
 * Capture function is mapped with one of the 3 events. A CCU4 slice can be configured into capture-1
 * mode with the selected \b event. In this mode the CCU4 will capture the timer value into CC4yC2V and CC4yC3V.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_ConfigureEvent()<BR> Cy_CCU4_SLICE_SetInput().
 */
void Cy_CCU4_SLICE_Capture1Config(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    bool would return true if the extended capture read back mode is enabled<BR>
 *
 * \par<b>Description:</b><br>
 * Checks if Extended capture mode read is enabled for particular slice or not, by reading CC4yTC.ECM bit.\n\n
 * In this mode the there is only one associated read address for all the capture registers.
 * Individual capture registers can still be accessed in this mode.
 *
 * \par<b>Related APIs:</b><br>
 *      Cy_CCU4_SLICE_GetCapturedValueFromFifo().
 */
__STATIC_INLINE bool Cy_CCU4_SLICE_IsExtendedCapReadEnabled(const Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  return ((bool)((slice->TC & (uint32_t) CCU4_CC4_TC_ECM_Msk) == (uint32_t)CCU4_CC4_TC_ECM_Msk));
}

#if defined(CY_CCU4V2) || defined(CY_CCU4V3) || defined(CY_DOXYGEN)
/**
 * @param slice Constant pointer to CC4 Slice
 * @param set The capture register set from which the captured value is to be retrieved
 * @return <BR>
 *    uint32_t Returns the value captured in the \b slice_number
 *             Range: [0x0 to 0xFFFF]
 *
 * \par<b>Description:</b><br>
 * Read captured value from FIFO(CC4yECRD0 and CC4yECRD1).\n\n
 * This is applicable only in the Capture mode of operation. The signal whose timing characteristics are to be measured
 * must be mapped to an event which in turn must be mapped to the capture function. Based on the capture criteria, the
 * instant timer values are captured into capture registers. Timing characteristics of the input signal may then be
 * derived/inferred from the captured values.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_IsExtendedCapReadEnabled().
 */
uint32_t Cy_CCU4_SLICE_GetCapturedValueFromFifo(const Cy_CCU4_SLICE_t *const slice,
    const Cy_CCU4_SLICE_CAP_REG_SET_t set);
#endif

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Map an External event to the External Count Direction Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Count Direction of the slice, by configuring CC4yCMC.UDS bit.\n\n
 * Count direction function is mapped with one of the 3 events. A slice can be configured to change the
 * CC4yTIMER count direction depending on an external signal.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_ConfigureEvent()<BR> Cy_CCU4_SLICE_SetInput().
 */
void Cy_CCU4_SLICE_DirectionConfig(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the status bit override Function of the slice, by configuring CC4yCMC.OFS bit.\n\n
 * Status bit override function is mapped with one of the 3 events. A slice can be configured to change the
 * output of the timer's CC4yST signal depending on an external signal.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_ConfigureStatusBitOverrideEvent().
 */
void Cy_CCU4_SLICE_StatusBitOverrideConfig(Cy_CCU4_SLICE_t *const slice);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param exit_mode How should a previously logged trap state be exited?
 * @param synch_with_pwm Should exit of trap state be synchronized with PWM cycle start?
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Trap Function of the slice, by configuring CC4yCMC.TS, CC4yTC.TRPSE, and CC4yTC.TRPSW bits.\n\n
 * Trap function is mapped with Event-2. Criteria for exiting the trap state is configured.
 * This trap function allows PWM outputs to react on the state of an input pin.
 * Thus PWM output can be forced to inactive state upon detection of a trap.
 * It is also possible to synchronize the trap function with the PWM signal using the \b synch_with_pwm.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_ConfigureEvent()<BR> Cy_CCU4_SLICE_SetInput().
 */
void Cy_CCU4_SLICE_TrapConfig(Cy_CCU4_SLICE_t *const slice,
                               const Cy_CCU4_SLICE_TRAP_EXIT_MODE_t exit_mode,
                               bool synch_with_pwm);


/**
 * @param slice Constant pointer to CC4 Slice
 * @param ev1_config Pointer to event 1 configuration data
 * @param ev2_config Pointer to event 2 configuration data
 * @return <BR>
 *    None<BR>
 *
 *
 * \par<b>Description:</b><br>
 * Map Status bit override function with an Event1 & Event 2 of the slice and configure CC4yINS register.\n\n
 * Details such as the input mapped to the event, event detection criteria and Low Pass filter options are programmed
 * by this routine for the events 1 & 2.  Event-1 input would be the trigger to override the value.
 * Event-2 input would be the override value.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_StatusBitOverrideConfig().
 */
void Cy_CCU4_SLICE_ConfigureStatusBitOverrideEvent(Cy_CCU4_SLICE_t *const slice,
    const Cy_CCU4_SLICE_EVENT_CONFIG_t *const ev1_config,
    const Cy_CCU4_SLICE_EVENT_CONFIG_t *const ev2_config);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event The External Event which needs to be configured.
 * @param config Pointer to event configuration data.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures an External Event of the slice, by updating CC4yINS register .\n\n
 * Details such as the input mapped to the event, event detection criteria and low pass filter
 * options are programmed by this routine. The Event \b config  will configure the input selection,
 * the edge selection, the level selection and the Low pass filter for the  event.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_StartConfig()<BR> Cy_CCU4_SLICE_StopConfig()<BR> Cy_CCU4_SLICE_LoadConfig()<BR>
 *  Cy_CCU4_SLICE_ModulationConfig()<BR> Cy_CCU4_SLICE_CountConfig()<BR> Cy_CCU4_SLICE_GateConfig()<BR>
 *  Cy_CCU4_SLICE_Capture0Config()<BR> Cy_CCU4_SLICE_Capture1Config()<BR> Cy_CCU4_SLICE_DirectionConfig()<BR>
 *  Cy_CCU4_SLICE_StatusBitOverrideConfig()<BR> Cy_CCU4_SLICE_TrapConfig().
 */
void Cy_CCU4_SLICE_ConfigureEvent(Cy_CCU4_SLICE_t *const slice,
                                   const Cy_CCU4_SLICE_EVENT_t event,
                                   const Cy_CCU4_SLICE_EVENT_CONFIG_t *const config);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event The External Event which needs to be configured.
 * @param input One of the 16 inputs meant to be mapped to the desired event
 * @return <BR>
 *    None<BR>
 *
 *
 * \par<b>Description:</b><br>
 * Selects an input for an external event, by configuring CC4yINS register.\n\n
 * It is possible to select one of the possible 16 input signals for a given Event.
 * This configures the CC4yINS.EVxIS for the selected event.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_StartConfig()<BR> Cy_CCU4_SLICE_StopConfig()<BR> Cy_CCU4_SLICE_LoadConfig()<BR>
 *  Cy_CCU4_SLICE_ModulationConfig()<BR> Cy_CCU4_SLICE_CountConfig()<BR> Cy_CCU4_SLICE_GateConfig()<BR>
 *  Cy_CCU4_SLICE_Capture0Config()<BR> Cy_CCU4_SLICE_Capture1Config()<BR> Cy_CCU4_SLICE_DirectionConfig()<BR>
 *  Cy_CCU4_SLICE_StatusBitOverrideConfig()<BR> Cy_CCU4_SLICE_TrapConfig().
 */
void Cy_CCU4_SLICE_SetInput(Cy_CCU4_SLICE_t *const slice,
                             const Cy_CCU4_SLICE_EVENT_t event,
                             const Cy_CCU4_SLICE_INPUT_t input);

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the trap feature, by setting CC4yTC.TRAPE0, CC4yTC.TRAPE1, CC4yTC.TRAPE2 and CC4yTC.TRAPE3 bit based on the
 * \a out_mask.\n\n
 * A particularly useful feature where the PWM output can be forced inactive upon detection of a trap. The trap signal
 * can be the output of a sensing element which has just detected an abnormal electrical condition.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_TrapConfig()<BR> Cy_CCU4_SLICE_DisableTrap()<BR> Cy_CCU4_SLICE_ConfigureEvent()<BR>
 *  Cy_CCU4_SLICE_SetInput().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_EnableTrap(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TC |= (uint32_t) CCU4_CC4_TC_TRAPE_Msk;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the trap feature, by clearing CC4yTC.TRAPE0, CC4yTC.TRAPE1, CC4yTC.TRAPE2 and CC4yTC.TRAPE3 bit based on the
 * \a out_mask.\n\n.\n\n
 * This API will revert the changes done by Cy_CCU4_SLICE_EnableTrap().
 * This Ensures that the TRAP function has no effect on the output of the CCU4 slice.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_EnableTrap().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_DisableTrap(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TC &= ~((uint32_t) CCU4_CC4_TC_TRAPE_Msk);
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    bool returns \b true if the Timer is running else it returns \b false.
 *
 * \par<b>Description:</b><br>
 * Returns the state of the timer (Either Running or stopped(idle)), by reading CC4yTCST.TRB bit.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_StartTimer()<BR> Cy_CCU4_SLICE_StopTimer().
 */
__STATIC_INLINE bool Cy_CCU4_SLICE_IsTimerRunning(const Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  return (bool)(((slice->TCST) & CCU4_CC4_TCST_TRB_Msk) == CCU4_CC4_TCST_TRB_Msk);
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *   ::Cy_CCU4_SLICE_TIMER_COUNT_DIR_t returns the direction in which the timer is counting.
 *
 * \par<b>Description:</b><br>
 * Returns the timer counting direction, by reading CC4yTCST.CDIR bit.\n\n
 * This API will return the direction in which the timer is currently
 * incrementing(CY_CCU4_SLICE_TIMER_COUNT_DIR_UP) or decrementing (CY_CCU4_SLICE_TIMER_COUNT_DIR_DOWN).
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE Cy_CCU4_SLICE_TIMER_COUNT_DIR_t Cy_CCU4_SLICE_GetCountingDir(const Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  return ((Cy_CCU4_SLICE_TIMER_COUNT_DIR_t)(((slice->TCST) & CCU4_CC4_TCST_CDIR_Msk) >> CCU4_CC4_TCST_CDIR_Pos));
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Starts the timer counting operation, by setting CC4yTCSET.TRBS bit.\n\n
 * It is necessary to have configured the CC4 slice before starting its timer.
 * Before the Timer is started ensure that the clock is provided to the slice.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_StopTimer().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_StartTimer(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TCSET = CCU4_CC4_TCSET_TRBS_Msk;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Stops the Timer.<BR>\n
 * Timer counting operation can be stopped by invoking this API, by setting CC4yTCCLR.TRBC bit.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_StartTimer().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_StopTimer(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TCCLR = (uint32_t) CCU4_CC4_TCCLR_TRBC_Msk;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Resets the timer count to zero, by setting CC4yTCCLR.TCC bit.\n\n
 * A timer which has been stopped can still retain the last counted value.
 * After invoking this API the timer value will be cleared.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_StartTimer().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_ClearTimer(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TCCLR = (uint32_t) CCU4_CC4_TCCLR_TCC_Msk;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Stops and resets the timer count to zero, by setting CC4yTCCLR.TCC and CC4yTCCLR.TRBC bit.\n\n
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_StartTimer().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_StopClearTimer(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TCCLR = CCU4_CC4_TCCLR_TRBC_Msk | CCU4_CC4_TCCLR_TCC_Msk;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    ::Cy_CCU4_SLICE_MODE_t returns CY_CCU4_SLICE_MODE_COMPARE if the slice is operating in compare mode
 *                          returns CY_CCU4_SLICE_MODE_CAPTURE if the slice is operating in capture mode
 *
 * \par<b>Description:</b><br>
 * Retrieves the current mode of operation in the slice (either Capture mode or Compare mode), by reading
 * CC4yTC.CMOD bit.\n\n
 * Ensure that before invoking this API the CCU4 slice should be configured otherwise the output of this API is
 * invalid.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE Cy_CCU4_SLICE_MODE_t Cy_CCU4_SLICE_GetSliceMode(const Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  return ((Cy_CCU4_SLICE_MODE_t)(((slice->TC) & CCU4_CC4_TC_CMOD_Msk) >> CCU4_CC4_TC_CMOD_Pos));
}
/**
 * @param slice Constant pointer to CC4 Slice
 * @param mode Desired repetition mode (Either single shot or Continuous)
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Timer to either Single shot mode or continuous mode, by configuring CC4yTC.TSSM bit.\n\n
 * The timer will count up to the terminal count as specified in the period register and stops immediately if the repeat
 * mode has been set to single shot. In the continuous mode of operation, the timer starts counting all over again after
 * reaching the terminal count.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_GetTimerRepeatMode().
 */
void Cy_CCU4_SLICE_SetTimerRepeatMode(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_TIMER_REPEAT_MODE_t mode);

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <br>
 *  ::Cy_CCU4_SLICE_TIMER_REPEAT_MODE_t returns CY_CCU4_SLICE_TIMER_REPEAT_MODE_REPEAT if continuous mode is selected
 *                                     returns CY_CCU4_SLICE_TIMER_REPEAT_MODE_SINGLE if single shot mode is selected
 *
 * \par<b>Description:</b><br>
 * Retrieves the Timer repeat mode, either Single shot mode or continuous mode, by reading CC4yTC.TSSM bit.\n\n
 * The timer will count up to the terminal count as specified in the period register and stops immediately if the repeat
 * mode has been set to single shot mode. In the continuous mode of operation, the timer starts counting
 * all over again after reaching the terminal count.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetTimerRepeatMode().
 */
__STATIC_INLINE Cy_CCU4_SLICE_TIMER_REPEAT_MODE_t Cy_CCU4_SLICE_GetTimerRepeatMode(
  const Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  return ((Cy_CCU4_SLICE_TIMER_REPEAT_MODE_t)(((slice->TC) & CCU4_CC4_TC_TSSM_Msk) >> CCU4_CC4_TC_TSSM_Pos));
}
/**
 * @param slice Constant pointer to CC4 Slice
 * @param mode Desired counting mode (Either Edge Aligned or Center Aligned)
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the timer counting mode either Edge Aligned or Center Aligned, by configuring CC4yTC.TCM bit.\n\n
 * In the edge aligned mode, the timer counts from 0 to the terminal count. Once the timer count has reached a preset
 * compare value, the timer status output asserts itself. It will now deassert only after the timer count reaches the
 * terminal count.\n In the center aligned mode, the timer first counts from 0 to the terminal count and then back to 0.
 * During this upward and downward counting, the timer status output stays asserted as long as the timer value is
 * greater than the compare value.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_GetTimerCountingMode().
 */
void Cy_CCU4_SLICE_SetTimerCountingMode(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_TIMER_COUNT_MODE_t mode);

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <br>
 *  ::Cy_CCU4_SLICE_TIMER_COUNT_MODE_t returns CY_CCU4_SLICE_TIMER_COUNT_MODE_EA if edge aligned mode is selected
 *                                     returns CY_CCU4_SLICE_TIMER_COUNT_MODE_CA if center aligned mode is selected
 *
 * \par<b>Description:</b><br>
 * Retrieves timer counting mode either Edge aligned or Center Aligned, by reading CC4yTC.TCM bit.\n\n
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetTimerCountingMode().
 */
__STATIC_INLINE Cy_CCU4_SLICE_TIMER_COUNT_MODE_t Cy_CCU4_SLICE_GetTimerCountingMode(
  const Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  return ((Cy_CCU4_SLICE_TIMER_COUNT_MODE_t)(((slice->TC) & CCU4_CC4_TC_TCM_Msk) >> CCU4_CC4_TC_TCM_Pos));
}
/**
 * @param slice Constant pointer to CC4 Slice
 * @param period_val Timer period value
 *        Range: [0x0 to 0xFFFF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Programs the timer period, by writing CC4yPRS register.\n\n
 * The frequency of counting/ PWM frequency is determined by this value. The period value is written to a shadow
 * register. Explicitly enable the shadow transfer for the the period value by calling
 * Cy_CCU4_EnableShadowTransfer() with appropriate mask. If shadow transfer is enabled and the timer is running,
 * a period match transfers the value from the shadow register to the actual period register.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_GetTimerPeriodMatch().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_SetTimerPeriodMatch(Cy_CCU4_SLICE_t *const slice, const uint16_t period_val)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->PRS = (uint32_t) period_val;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *   uint16_t returns the current timer period value
 *            Range: [0x0 to 0xFFFF]
 *
 * \par<b>Description:</b><br>
 * Retrieves the timer period value currently effective, by reading CC4yPR register.\n\n
 * If the timer is active then the value being returned is currently being used for the PWM period.
 *
 * \par<b>Note:</b><br>
 * The Cy_CCU4_SLICE_SetTimerPeriodMatch() would set the new period value to a shadow register.
 * This would only transfer the new values into the actual period register if the shadow transfer request
 * is enabled and if a period match occurs. Hence a consecutive call to Cy_CCU4_SLICE_GetTimerPeriodMatch()
 * would not reflect the new values until the shadow transfer completes.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetTimerPeriodMatch().
 */
__STATIC_INLINE uint16_t Cy_CCU4_SLICE_GetTimerPeriodMatch(const Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  return ((uint16_t)slice->PR);
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param compare_val Timer compare value
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Programs the timer compare value, by writing CC4yCRS register.<BR>\n
 *  The PWM duty cycle is determined by this value.
 * The compare value is written to a shadow register. Explicitly enable the shadow transfer for
 * the the period/compare value by calling Cy_CCU4_EnableShadowTransfer() with
 * appropriate mask.If shadow transfer is enabled and the timer is running,
 * a period match transfers the value from the shadow register to the actual compare register.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_GetTimerPeriodMatch().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_SetTimerCompareMatch(Cy_CCU4_SLICE_t *const slice, const uint16_t compare_val)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->CRS = (uint32_t) compare_val;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *   uint16_t returns the current timer compare value
 *            Range: [0x0 to 0xFFFF]
 *
 * \par<b>Description:</b><br>
 * Retrieves the timer compare value currently effective, by reading CC4yCRS register.\n\n
 * If the timer is active then the value being returned is currently being for the PWM duty cycle( timer compare value).
 *
 * \par<b>Note:</b><br>
 * The Cy_CCU4_SLICE_SetTimerCompareMatch() would set the new compare value to a shadow register.
 * This would only transfer the new values into the actual compare register if the shadow transfer request
 * is enabled and if a period match occurs. Hence a consecutive call to Cy_CCU4_SLICE_GetTimerCompareMatch()
 * would not reflect the new values until the shadow transfer completes.
 * Directly accessed Register is CC4yCR.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetTimerCompareMatch().
 */
__STATIC_INLINE uint16_t Cy_CCU4_SLICE_GetTimerCompareMatch(const Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  return ((uint16_t)slice->CR);
}

/**
 * @param module Constant pointer to CCU4 module
 * @param shadow_transfer_msk Shadow transfer request mask for various transfers.
 *                             Use ::Cy_CCU4_SHADOW_TRANSFER_t enum items to create a mask of choice,
 *                              using a bit wise OR operation.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Requests of shadow transfer for Period, Compare, Passive level, dither and prescaler, by configuring
 * the GCSS register.\n\n
 * The transfer from the shadow registers to the actual registers is done in the immediate next occurrence of the
 * shadow transfer trigger after the API is called.
 *
 * Any call to Cy_CCU4_SLICE_SetTimerPeriodMatch()<BR> Cy_CCU4_SLICE_SetTimerCompareMatch()<BR>
 * Cy_CCU4_SLICE_SetPrescaler()<BR> Cy_CCU4_SLICE_CompareInit()<BR> Cy_CCU4_SLICE_CaptureInit().
 * must be succeeded by this API.
 * Directly accessed Register is GCSS.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE void Cy_CCU4_EnableShadowTransfer(Cy_CCU4_MODULE_t *const module, const uint32_t shadow_transfer_msk)
{
  CY_ASSERT(Cy_CCU4_IsValidModule(module));
  module->GCSS = (uint32_t)shadow_transfer_msk;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *   uint16_t returns the current timer value
 *            Range: [0x0 to 0xFFFF]
 *
 * \par<b>Description:</b><br>
 * Retrieves the latest timer value, from CC4yTIMER register.\n\n
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetTimerValue().
 */
__STATIC_INLINE uint16_t Cy_CCU4_SLICE_GetTimerValue(const Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  return ((uint16_t)slice->TIMER);
}
/**
 * @param slice Constant pointer to CC4 Slice
 * @param timer_val The new timer value that has to be loaded into the TIMER register.
 *                  Range: [0x0 to 0xFFFF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Loads a new timer value, by setting CC4yTIMER register.\n\n
 *
 * \par<b>Note:</b><br>
 *  Request to load is ignored if the timer is running.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_GetTimerValue().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_SetTimerValue(Cy_CCU4_SLICE_t *const slice, const uint16_t timer_val)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TIMER = (uint32_t) timer_val;
}
/**
 * @param slice Constant pointer to CC4 Slice
 * @param period_dither Boolean instruction on dithering of period match
 * @param duty_dither Boolean instruction on dithering of compare match
 * @param spread Dither compare value
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables dithering of PWM frequency and duty cycle, by configuring CC4yTC.DITHE and CC4yDITS bits.\n\n
 * Some control loops are slow in updating PWM frequency and duty cycle. In such a case, a Bresenham style dithering
 * can help reduce long term errors. Dithering can be applied to period and duty individually,
 * this can be selected using the parameter \b period_dither and \b duty_dither.
 * The \b spread would provide the dither compare value. If the dither counter value is less than this \b spread then
 * the period/compare values would be dithered according to the dither mode selected. This API would invoke
 * Cy_CCU4_SLICE_SetDitherCompareValue().
 *
 * \par<b>Note:</b><br>
 * After this API call, Cy_CCU4_EnableShadowTransfer() has to be called with appropriate mask
 * to transfer the dither value.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_DisableDithering().
 */
void Cy_CCU4_SLICE_EnableDithering(Cy_CCU4_SLICE_t *const slice,
                                    const bool period_dither,
                                    const bool duty_dither,
                                    const uint8_t spread);

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables dithering of PWM frequency and duty cycle, by clearing CC4yTC.DITHE bits.\n\n
 * This disables the Dither mode that was set in Cy_CCU4_SLICE_EnableDithering().
 * This API will not clear the dither compare value.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_EnableDithering().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_DisableDithering(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TC &= ~((uint32_t) CCU4_CC4_TC_DITHE_Msk);
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the floating prescaler, by setting CC4yTC.FPE bit.\n\n
 * The prescaler divider starts with an initial value and increments upon every period match. It keeps incrementing
 * until a ceiling (prescaler compare value) is hit and thereafter rolls back to the original prescaler divider value.\n
 * It is necessary to have programmed an initial divider value and a compare value before the feature is enabled.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetFloatingPrescalerCompareValue()<BR> Cy_CCU4_SLICE_DisableFloatingPrescaler()<BR>
 *  Cy_CCU4_SLICE_SetPrescaler().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_EnableFloatingPrescaler(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TC |= (uint32_t) CCU4_CC4_TC_FPE_Msk;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the floating prescaler, by clearing CC4yTC.FPE bit.\n\n
 * This would return the prescaler to the normal mode.
 * The prescaler that would be applied is the value present in CC4yPSC.
 *
  * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_EnableFloatingPrescaler().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_DisableFloatingPrescaler(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TC &= ~((uint32_t) CCU4_CC4_TC_FPE_Msk);
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param comp_val Dither compare value
 *                 Range: [0x0 to 0xF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Sets the dither spread/compare value, by setting CC4yDITS.DCVS bits.\n\n
 * This value is the cornerstone of PWM dithering feature. Dithering is applied/done when the value in the
 * dithering counter is less than this compare/spread value. For all dithering counter values greater than
 * the spread value, there is no dithering. After setting the value Cy_CCU4_EnableShadowTransfer() has to be
 * called with appropriate mask.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_EnableDithering().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_SetDitherCompareValue(Cy_CCU4_SLICE_t *const slice, const uint8_t comp_val)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->DITS = comp_val;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param div_val Prescaler divider value. Accepts enum :: Cy_CCU4_SLICE_PRESCALER_t
 *                 Range: [0x0 to 0xF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Programs the slice specific prescaler divider, by configuring the CC4yPSC and CC4yFPC registers.\n\n
 * The prescaler divider may only be programmed after the prescaler run bit has been cleared
 * by calling Cy_CCU4_StopPrescaler().
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetFloatingPrescalerCompareValue().
 */
void Cy_CCU4_SLICE_SetPrescaler(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_PRESCALER_t div_val);

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    Cy_CCU4_SLICE_PRESCALER_t Prescaler divider value.<BR>
 *
 * \par<b>Description:</b><br>
 * Returns current prescaler value.\n\n
 * Used to detrmine the clock frequency of the CCU4 slice Cy_SCU_CLOCK_GetCcuClockFrequency() / Cy_CCU4_SLICE_GetPrescaler()
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetPrescaler().
 */
__STATIC_INLINE Cy_CCU4_SLICE_PRESCALER_t Cy_CCU4_SLICE_GetPrescaler(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));

  return (Cy_CCU4_SLICE_PRESCALER_t)slice->PSC;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param cmp_val Prescaler divider compare value
 *                 Range: [0x0 to 0xF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Programs the slice specific prescaler divider compare value, by configuring CC4yFPCS register.\n\n
 * The compare value is applicable only in floating mode of operation. The prescaler divider starts with an initial
 * value and increments to the compare value steadily upon every period match. Once prescaler divider
 * equals the prescaler divider compare value, the value in the former resets back to the PVAL (from FPC). After setting
 * the value, Cy_CCU4_EnableShadowTransfer() has to be called with appropriate mask.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetPrescaler().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_SetFloatingPrescalerCompareValue(Cy_CCU4_SLICE_t *const slice,
    const uint8_t cmp_val)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  /*  write to the shadow register */
  slice->FPCS = (uint32_t) cmp_val;
}
/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the multichannel mode, by setting CC4yTC.MCME bit.<BR>\n
 * The output state of the Timer slices can be controlled in parallel by a single input signal.
 * A particularly useful feature in motor control applications where the PWM output of multiple slices of a module can
 * be gated and ungated by multi-channel gating inputs connected to the slices. A peripheral like POSIF connected to the
 * motor knows exactly which of the power drive switches are to be turned on and off at any instant. It can thus through
 * a gating bus (known as multi-channel inputs) control which of the slices output stays gated/ungated.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_DisableMultiChannelMode()<BR> Cy_CCU4_SetMultiChannelShadowTransferMode().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_EnableMultiChannelMode(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TC |= (uint32_t) CCU4_CC4_TC_MCME_Msk;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the multichannel mode, by clearing CC4yTC.MCME bit.<BR>\n
 * This would return the slices to the normal operation mode.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_EnableMultiChannelMode().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_DisableMultiChannelMode(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->TC &= ~((uint32_t) CCU4_CC4_TC_MCME_Msk);
}

/**
 * @param module Constant pointer to CCU4 module
 * @param slice_mode_msk Slices for which the configuration has to be applied.
 *                       Use ::Cy_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_t enum items to create a mask of choice,
 *                             using a bit wise OR operation.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the Multi-channel shadow transfer request trigger signal either by software or hardware by configuring
 * GCTRL.MSE0, GCTRL.MSE1, GCTRL.MSE2, and GCTRL.MSE3 based on the mask.\n\n
 * The shadow transfer would take place either if it was requested by software or by the CCU4x.MCSS input.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
*/
void Cy_CCU4_SetMultiChannelShadowTransferMode(Cy_CCU4_MODULE_t *const module, const uint32_t slice_mode_msk);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param reg_num The capture register from which the captured value is to be retrieved
 *                Range: [0,3]
 * @return <BR>
 *    uint32_t  Returns the Capture register value.
 *              Range: [0 to 0x1FFFFF]
 *
 * \par<b>Description:</b><br>
 * Retrieves timer value which has been captured in the Capture registers, by reading CC4yCV[\b reg_num] register.\n\n
 * The signal whose timing characteristics are to be measured must be mapped to an event which in turn must be mapped
 * to the capture function. Based on the capture criteria, the timer values are captured into capture registers. Timing
 * characteristics of the input signal may then be derived/inferred from the captured values. The full flag will help
 * to find out if there is a new captured value present.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_GetLastCapturedTimerValue().
 */
uint32_t Cy_CCU4_SLICE_GetCaptureRegisterValue(const Cy_CCU4_SLICE_t *const slice, const uint8_t reg_num);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param set  The capture register set, which must be evaluated
 * @param val_ptr Out Parameter of the API.Stores the captured timer value into this out parameter.
 * @return <BR>
 *     ::Cy_CCU4_STATUS_t Returns CY_CCU4_STATUS_OK if there was new value present in the capture registers.
 *                   returns CY_CCU4_STATUS_ERROR if there was no new value present in the capture registers.
 *
 * \par<b>Description:</b><br>
 * Retrieves the latest captured timer value, by reading CC4yCV registers.\n\n
 * Retrieve the timer value last stored by the slice. When separate capture events are used,
 * users must specify the capture set to evaluate. If single capture event mode is used, all 4 capture registers are
 * evaluated.\n
 * The lowest register is evaluated first followed by the next higher ordered register and this continues until all
 * capture registers have been evaluated.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_GetCaptureRegisterValue().
 */
Cy_CCU4_STATUS_t Cy_CCU4_SLICE_GetLastCapturedTimerValue(const Cy_CCU4_SLICE_t *const slice,
    const Cy_CCU4_SLICE_CAP_REG_SET_t set,
    uint32_t *val_ptr);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Event whose assertion can potentially lead to an interrupt
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the generation of an interrupt pulse for the event, by configuring CC4yINTE register.\n\n
 * For an event to lead to an interrupt, it must first be enabled and bound to a service request line. The corresponding
 * NVIC node must be enabled as well. This API merely enables the event. Binding with SR is performed by another API.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetInterruptNode()<BR> Cy_CCU4_SLICE_EnableMultipleEvents()<BR> Cy_CCU4_SLICE_DisableEvent()<BR>
 *  Cy_CCU4_SLICE_DisableMultipleEvents().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_EnableEvent(Cy_CCU4_SLICE_t *const slice,
    const Cy_CCU4_SLICE_IRQ_ID_t event)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_INTERRUPT(event));
  slice->INTE |= ((uint32_t) 1) << ((uint32_t) event);
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param intr_mask Event mask such that multiple events can be enabled.
 *                  Use ::Cy_CCU4_SLICE_MULTI_IRQ_ID_t enum items to create a mask of choice,
 *                  using a bit wise OR operation.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the generation of an interrupt pulse for the required events, by configuring CC4yINTE register.\n\n
 * For an event to lead to an interrupt, it must first be enabled and bound to a service request line. The corresponding
 * NVIC node must be enabled as well. This API merely enables the events. Binding with SR is performed by another API.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetInterruptNode()<BR> Cy_CCU4_SLICE_EnableEvent()<BR> Cy_CCU4_SLICE_DisableEvent()<BR>
 *  Cy_CCU4_SLICE_DisableMultipleEvents().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_EnableMultipleEvents(Cy_CCU4_SLICE_t *const slice, const uint16_t intr_mask)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->INTE |= (uint32_t)intr_mask;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Event whose assertion can potentially lead to an interrupt
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the generation of an interrupt pulse for the event, by clearing CC4yINTE register.\n\n
 * Prevents the event from being asserted
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetInterruptNode()<BR> Cy_CCU4_SLICE_EnableEvent()<BR> Cy_CCU4_SLICE_EnableMultipleEvents()<BR>
 *  Cy_CCU4_SLICE_DisableMultipleEvents().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_DisableEvent(Cy_CCU4_SLICE_t *const slice,
    const Cy_CCU4_SLICE_IRQ_ID_t event)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_INTERRUPT(event));
  slice->INTE &= ~(((uint32_t) 1) << ((uint32_t) event));
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param mask Event mask such that multiple events can be enabled.
 *             Use ::Cy_CCU4_SLICE_MULTI_IRQ_ID_t enum items to create a mask of choice,
 *             using a bit wise OR operation.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Disables the generation of an interrupt pulse for the required events, by clearing CC4yINTE register.\n\n
 *  Prevents selected events of the slice from being asserted.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetInterruptNode()<BR> Cy_CCU4_SLICE_EnableEvent()<BR> Cy_CCU4_SLICE_EnableMultipleEvents()<BR>
 *  Cy_CCU4_SLICE_DisableEvent().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_DisableMultipleEvents(Cy_CCU4_SLICE_t *const slice, const uint16_t mask)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->INTE &= ~((uint32_t) mask);
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Event whose assertion can potentially lead to an interrupt
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Manually asserts the requested event, by setting CC4ySWS register.\n\n
 * For an event to lead to an interrupt, it must first be enabled and bound to a service request line. The corresponding
 * NVIC node must be enabled as well. This API manually asserts the requested event.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_SetInterruptNode()<BR> Cy_CCU4_SLICE_EnableEvent()<BR> Cy_CCU4_SLICE_EnableMultipleEvents().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_SetEvent(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_IRQ_ID_t event)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_INTERRUPT(event));
  slice->SWS = ((uint32_t) 1) << ((uint32_t) event);
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Asserted event which must be acknowledged.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Acknowledges an asserted event, by setting CC4ySWR with respective event flag.\n\n
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_EnableEvent()<BR> Cy_CCU4_SLICE_SetEvent()<BR> Cy_CCU4_SLICE_GetEvent().
 */
__STATIC_INLINE void Cy_CCU4_SLICE_ClearEvent(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_IRQ_ID_t event)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_INTERRUPT(event));
  slice->SWR = ((uint32_t) 1) << ((uint32_t) event);
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Event to be evaluated for assertion
 * @return <br>
 *         bool  Returns true if event is set else false is returned.
 *
 * \par<b>Description:</b><br>
 * Evaluates if a given event is asserted or not, by reading CC4yINTS register.\n\n
 * Return true if the event is asserted. For a event to be asserted it has to be
 * first enabled. Only if that event is enabled the call to this API is valid.
 * If the Event is enabled and has not yet occurred then a false is returned.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_EnableEvent()<BR> Cy_CCU4_SLICE_SetEvent().
 */
__STATIC_INLINE bool Cy_CCU4_SLICE_GetEvent(const Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_IRQ_ID_t event)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_INTERRUPT(event));

  return (((uint32_t)(slice->INTS & ((uint32_t)1 << (uint32_t)event))) != 0U);
}
/**
 * @param slice Constant pointer to CC4 Slice
 * @param event Event which must be bound to a service request line
 * @param sr The Service request line which is bound to the \b event
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Binds requested event to a service request line, by configuring CC4ySRS register with respective event.\n\n
 * For an event to lead to an interrupt, it must first be enabled and bound to a service request line. The corresponding
 * NVIC node must be enabled as well. This API binds the requested event with the requested service request line(\b sr).
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_EnableEvent()<BR> Cy_CCU4_SLICE_SetEvent().
 */
void Cy_CCU4_SLICE_SetInterruptNode(Cy_CCU4_SLICE_t *const slice,
                                     const Cy_CCU4_SLICE_IRQ_ID_t event,
                                     const Cy_CCU4_SLICE_SR_ID_t sr);

/**
 * @param slice Constant pointer to CC4 Slice
 * @param level Slice output passive level
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the passive level for the slice output, by setting CC4yPSL register.\n\n
 * Defines the passive level for the timer slice output pin. Selects either level high is passive
 * or level low is passive. This is the level of the output before the compare match is value changes it.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_EnableEvent()<BR> Cy_CCU4_SLICE_SetEvent().
 */
void Cy_CCU4_SLICE_SetPassiveLevel(Cy_CCU4_SLICE_t *const slice,
                                    const Cy_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_t level);

#if defined(CY_CCU4V3) || defined(DOXYGEN)
/**
 * @param slice Constant pointer to CC4 Slice
 *
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Cascades the shadow transfer operation throughout the CCU4 timer slices, by setting CSE bit in STC register.\n\n
 *
 * The shadow transfer enable bits needs to be set in all timer slices, that are being used in the cascaded architecture,
 * at the same time. The shadow transfer enable bits, also need to be set for all slices even if the shadow values of
 * some slices were not updated. It is possible to to cascade with the adjacent slices only. CC40 slice is a
 * master to start the operation.
 *
 * \par<b>Note:</b><br>
 * Cy_CCU4_EnableShadowTransfer() must be called to enable the shadow transfer of the all the slices, which needs to be
 * cascaded.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_EnableShadowTransfer(), Cy_CCU4_SLICE_DisableCascadedShadowTransfer()<BR>.
 */
__STATIC_INLINE void Cy_CCU4_SLICE_EnableCascadedShadowTransfer(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->STC |= (uint32_t) CCU4_CC4_STC_CSE_Msk;
}

/**
 * @param slice Constant pointer to CC4 Slice
 *
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the cascaded the shadow transfer operation, by clearing CSE bit in STC register.\n\n
 *
 * If in any slice the cascaded mode disabled, other slices from there onwards does not update the values in cascaded mode.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_CCU4_SLICE_EnableCascadedShadowTransfer()<BR>.
 */
__STATIC_INLINE void Cy_CCU4_SLICE_DisableCascadedShadowTransfer(Cy_CCU4_SLICE_t *const slice)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->STC &= ~(uint32_t) CCU4_CC4_STC_CSE_Msk;
}

/**
 * @param slice Constant pointer to CC4 Slice
 * @param shadow_transfer_mode mode to be configured
 *        Use :: Cy_CCU4_SLICE_SHADOW_TRANSFER_MODE_t enum items for mode
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures when the shadow transfer has to occur, by setting STM bit in STC register.\n\n
 *
 * After requesting for shadow transfer mode using Cy_CCU4_EnableShadowTransfer(), actual transfer occurs based on the
 * selection done using this API (i.e. on period and One match, on Period match only, on One match only).
 *
 * \par<b>Note:</b><br>
 * This is effective when the timer is configured in centre aligned mode.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_CCU4_EnableShadowTransfer()<BR>
 *
*/
__STATIC_INLINE void Cy_CCU4_SLICE_SetShadowTransferMode(Cy_CCU4_SLICE_t *const slice,
    const Cy_CCU4_SLICE_SHADOW_TRANSFER_MODE_t shadow_transfer_mode)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->STC = ((slice->STC) & (uint32_t)~CCU4_CC4_STC_STM_Msk) |
               ((shadow_transfer_mode << CCU4_CC4_STC_STM_Pos) & (uint32_t)CCU4_CC4_STC_STM_Msk);
}

/**
* @param slice Constant pointer to CC4 Slice
* @param coherent_write specifies for what fields this mode has to be applied
*        Use :: Cy_CCU4_SLICE_WRITE_INTO_t enum items to create a mask of choice, using a bit wise OR operation.
* @return <BR>
*    None<BR>
*
* \par<b>Description:</b><br>
* Configures the specified fields shadow value to be updated in synchronous with PWM after shadow transfer request, by
* clearing IRPC, IRCC1, IRCC2, IRLC, IRDC, IRFC bits in STC register.\n\n
*
* When coherent shadow is enabled, after calling Cy_CCU4_EnableShadowTransfer(), the value which are written in the
* respective shadow registers get updated according the configuration done using Cy_CCU4_SLICE_SetShadowTransferMode()
* API. \par<b>Note:</b><br>
*
* \par<b>Related APIs:</b><br>
* Cy_CCU4_EnableShadowTransfer(), Cy_CCU4_SLICE_SetShadowTransferMode()<BR>
*
*/
__STATIC_INLINE void Cy_CCU4_SLICE_WriteCoherentlyWithPWMCycle(Cy_CCU4_SLICE_t *const slice,
    const uint32_t coherent_write)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->STC &= ~(uint32_t)coherent_write;
}

/**
* @param slice Constant pointer to CC4 Slice
* @param immediate_write specifies for what fields this mode has to be applied
*        Use :: Cy_CCU4_SLICE_WRITE_INTO_t enum items to create a mask of choice, using a bit wise OR operation.
* @return <BR>
*    None<BR>
*
* \par<b>Description:</b><br>
* Configures the specified fields shadow value to be updated immediately after shadow transfer request, by setting
* IRPC, IRCC1, IRCC2, IRLC, IRDC, IRFC bits in STC register.\n\n
*
* When immediate shadow is enabled, by calling Cy_CCU4_EnableShadowTransfer() the value which are written in the
* shadow registers get updated to the actual registers immediately. \par<b>Note:</b><br>
*
* \par<b>Related APIs:</b><br>
* Cy_CCU4_EnableShadowTransfer()<BR>
*
*/
__STATIC_INLINE void Cy_CCU4_SLICE_WriteImmediateAfterShadowTransfer(Cy_CCU4_SLICE_t *const slice,
    const uint32_t immediate_write)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->STC |= immediate_write;
}

/**
* @param slice Constant pointer to CC4 Slice
* @param automatic_shadow_transfer specify upon which register update, automatic shadow transfer request is generated
*        Use :: Cy_CCU4_SLICE_AUTOMAIC_SHADOW_TRANSFER_WRITE_INTO_t enum items to create a mask of choice, using a
*               bit wise OR operation.
* @return <BR>
*    None<BR>
*
* \par<b>Description:</b><br>
* Configure on which shadow register update, automatic shadow transfer request generation has to be enabled. By setting
* ASPC, ASCC1, ASCC2, ASLC, ASDC, ASFC bits in STC register.\n\n
*
* By updating the configured shadow register, the shadow transfer request is generated to update all the shadow registers.
* \par<b>Note:</b><br>
* Automatic shadow transfer feature does not work when system PCLK is faster than MCLK.
*
* \par<b>Related APIs:</b><br>
*  Cy_CCU4_SLICE_DisableAutomaticShadowTransferRequest().
*
*/
__STATIC_INLINE void Cy_CCU4_SLICE_EnableAutomaticShadowTransferRequest(Cy_CCU4_SLICE_t *const slice,
    const uint32_t automatic_shadow_transfer)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->STC |= automatic_shadow_transfer;
}

/**
* @param slice Constant pointer to CC4 Slice
* @param automatic_shadow_transfer specify upon which register update, automatic shadow transfer request should not be
*                                  generated
*        Use :: Cy_CCU4_SLICE_AUTOMAIC_SHADOW_TRANSFER_WRITE_INTO_t enum items to create a mask of choice, using a
*               bit wise OR operation.
* @return <BR>
*    None<BR>
*
* \par<b>Description:</b><br>
* Configure on which shadow register update, automatic shadow transfer request generation has to be disabled. By
* clearing ASPC, ASCC1, ASCC2, ASLC, ASDC, ASFC bits in STC register.\n\n
*
* This disables the generation of automatic shadow transfer request for the specified register update.
* \par<b>Note:</b><br>
*
* \par<b>Related APIs:</b><br>
*  Cy_CCU4_SLICE_EnableAutomaticShadowTransferRequest().
*
*/
__STATIC_INLINE void Cy_CCU4_SLICE_DisableAutomaticShadowTransferRequest(Cy_CCU4_SLICE_t *const slice,
    const uint32_t automatic_shadow_transfer)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  slice->STC &= ~(uint32_t)automatic_shadow_transfer;
}
#endif

/**
 * @param module Constant pointer to CCU4 module
 * @param slice_number Slice for which the status bit should be obtained.
 *                   Range: [0x0 to 0x3]
 * @return status bit of slice passive (false) or active (true)
 *
 * \par<b>Description:</b><br>
 * Returns CCU4 slice status bit (passive or active)
 *
 */
__STATIC_INLINE Cy_CCU4_SLICE_STATUS_BIT_t Cy_CCU4_GetSliceStatusBit(Cy_CCU4_MODULE_t *const module, uint8_t slice_number)
{
  return (Cy_CCU4_SLICE_STATUS_BIT_t)((module->GCST & (CCU4_GCST_CC40ST_Msk << slice_number)) ? 1 : 0);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */


#endif /* defined(CCU40) */

#endif /* CCU4_H */
