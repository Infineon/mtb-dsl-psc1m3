/**
 * @file cy_rtc.h
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

#ifndef CY_RTC_H
#define CY_RTC_H

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include <time.h>

#include "cy_common.h"


/**
 * @addtogroup RTC
 * @brief RTC driver for PSC1M microcontroller family.
 *
 * Real-time clock (RTC) is a clock that keeps track of the current time. Precise
 * real time keeping is with a 32.768 KHz external crystal clock or a 32.768 KHz
 * high precision internal clock. It provides a periodic time based interrupt and
 * a programmable alarm interrupt on time match. It also supports wakeup from
 * hibernate.
 *
 * The RTC low level driver provides functions to configure and initialize the RTC
 * hardware peripheral.
 *
 * Also, the RTC block can be configured from the ModusToolbox&trade; Device Configurator: \ref section_personality_rtc
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * Debug mode status values
 */
typedef enum CY_RTC_DEBUG_MODE
{
  CY_RTC_RUN_IN_DEBUG_MODE  = 0U, /**< RTC is not stopped during halting mode debug */
  CY_RTC_STOP_IN_DEBUG_MODE = 1U  /**< RTC is stopped during halting mode debug */
} Cy_RTC_DEBUG_MODE_t;


/**
 *  Status return values for RTC low level driver
 */
typedef enum CY_RTC_STATUS
{
  CY_RTC_STATUS_OK    = 0U, /**< Operation successful */
  CY_RTC_STATUS_ERROR = 1U, /**< Operation unsuccessful */
  CY_RTC_STATUS_BUSY  = 2U  /**< Busy with a previous request */
} Cy_RTC_STATUS_t;

/**
 * Events which enables interrupt request generation
 */
typedef enum CY_RTC_EVENT
{
  CY_RTC_EVENT_PERIODIC_SECONDS = RTC_MSKSR_MPSE_Msk, /**< Mask value to enable an event on periodic seconds */
  CY_RTC_EVENT_PERIODIC_MINUTES = RTC_MSKSR_MPMI_Msk, /**< Mask value to enable an event on periodic minutes */
  CY_RTC_EVENT_PERIODIC_HOURS   = RTC_MSKSR_MPHO_Msk, /**< Mask value to enable an event on periodic hours */
  CY_RTC_EVENT_PERIODIC_DAYS    = RTC_MSKSR_MPDA_Msk, /**< Mask value to enable an event on periodic days */
  CY_RTC_EVENT_PERIODIC_MONTHS  = RTC_MSKSR_MPMO_Msk, /**< Mask value to enable an event on periodic months */
  CY_RTC_EVENT_PERIODIC_YEARS   = RTC_MSKSR_MPYE_Msk, /**< Mask value to enable an event on periodic years */
  CY_RTC_EVENT_ALARM            = RTC_MSKSR_MAI_Msk   /**< Mask value to enable an alarm event */
} Cy_RTC_EVENT_t;

/**
 *  Months used to program the date
 */
typedef enum CY_RTC_MONTH
{
  CY_RTC_MONTH_JANUARY   = 0U,
  CY_RTC_MONTH_FEBRUARY  = 1U,
  CY_RTC_MONTH_MARCH     = 2U,
  CY_RTC_MONTH_APRIL     = 3U,
  CY_RTC_MONTH_MAY       = 4U,
  CY_RTC_MONTH_JUNE      = 5U,
  CY_RTC_MONTH_JULY      = 6U,
  CY_RTC_MONTH_AUGUST    = 7U,
  CY_RTC_MONTH_SEPTEMBER = 8U,
  CY_RTC_MONTH_OCTOBER   = 9U,
  CY_RTC_MONTH_NOVEMBER  = 10U,
  CY_RTC_MONTH_DECEMBER  = 11U
} Cy_RTC_MONTH_t;

/**
 *  Week days used program the date
 */
typedef enum CY_RTC_WEEKDAY
{
  CY_RTC_WEEKDAY_SUNDAY    = 0U,
  CY_RTC_WEEKDAY_MONDAY    = 1U,
  CY_RTC_WEEKDAY_TUESDAY   = 2U,
  CY_RTC_WEEKDAY_WEDNESDAY = 3U,
  CY_RTC_WEEKDAY_THURSDAY  = 4U,
  CY_RTC_WEEKDAY_FRIDAY    = 5U,
  CY_RTC_WEEKDAY_SATURDAY  = 6U
} Cy_RTC_WEEKDAY_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/
/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined(__TASKING__)
#pragma warning 586
#endif


/**
 * Alarm time values of RTC  <br>
 *
 * The structure presents a convenient way to set/obtain the
 * alarm time values for seconds, minutes, hours, days, month and year of RTC.
 * The Cy_RTC_SetAlarm() and Cy_RTC_GetAlarm() can be
 * used to populate the structure with the alarm time value of
 * RTC
 */
typedef struct CY_RTC_ALARM
{
  union
  {
    uint32_t raw0;
    struct
    {
      uint32_t seconds  : 6; /**< Alarm seconds compare value (0-59: Above this causes this bitfield to be set with 0)*/
      uint32_t          : 2;
      uint32_t minutes  : 6; /**< Alarm minutes compare value (0-59: Above this causes this bitfield to be set with 0)*/
      uint32_t          : 2;
      uint32_t hours    : 5; /**< Alarm hours compare value   (0-23: Above this causes this bitfield to be set with 0)*/
      uint32_t          : 3;
      uint32_t days     : 5; /**< Alarm days compare value (0-Actual days of month: Above this causes this bitfield to be set with 0)*/
      uint32_t          : 3;
    };
  };

  union
  {
    uint32_t raw1;
    struct
    {
      uint32_t          : 8;
      uint32_t month    : 4; /**< Alarm month compare value (0-11: Above this causes this bitfield to be set with 0) */
      uint32_t          : 4;
      uint32_t year     : 16; /**< Alarm year compare value */
    };
  };
} Cy_RTC_ALARM_t;

/**
 * Time values of RTC  <br>
 *
 * The structure presents a convenient way to set/obtain the
 * time values for seconds, minutes, hours, days, month and year of RTC.
 * The Cy_RTC_SetTime() and Cy_RTC_GetTime() can be
 * used to populate the structure with the time value of
 * RTC
 */
typedef struct CY_RTC_TIME
{
  union
  {
    uint32_t raw0;
    struct
    {
      uint32_t seconds  : 6; /**< Seconds time value (0-59: Above this causes this bitfield to be set with 0) */
      uint32_t          : 2;
      uint32_t minutes  : 6; /**< Minutes time value (0-59: Above this causes this bitfield to be set with 0) */
      uint32_t          : 2;
      uint32_t hours    : 5; /**< Hours time value   (0-23: Above this causes this bitfield to be set with 0) */
      uint32_t          : 3;
      uint32_t days     : 5; /**< Days time value (0-Actual days of month: Above this causes this bitfield to be set with 0)*/
      uint32_t          : 3;
    };
  };

  union
  {
    uint32_t raw1;
    struct
    {
      uint32_t daysofweek  : 3; /**< Days of week time value (0-6: Above this causes this bitfield to be set with 0) */
      uint32_t             : 5;
      uint32_t month       : 4; /**< Month time value       (0-11: Above this causes this bitfield to be set with 0) */
      uint32_t             : 4;
      uint32_t year        : 16; /**< Year time value */
    };
  };
} Cy_RTC_TIME_t;
/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
#pragma pop
#elif defined(__TASKING__)
#pragma warning restore
#endif

/**
 * RTC initialization with time, alarm and clock divider(prescaler) configurations <br>
 *
 * The structure presents a convenient way to set/obtain the time and alarm configurations
 * for RTC. The Cy_RTC_Init() can be used to populate the structure with the time and alarm
 * values of RTC.
 */
typedef struct CY_RTC_CONFIG
{
  Cy_RTC_TIME_t    time;
  Cy_RTC_ALARM_t   alarm;
  uint16_t          prescaler;
} Cy_RTC_CONFIG_t;

/*******************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param config Constant pointer to a constant ::Cy_RTC_CONFIG_t structure containing the
 *               time, alarm time and clock divider(prescaler) configuration.
 * @return Cy_RTC_STATUS_t Always returns CY_RTC_STATUS_OK (It contains only register assignment statements)
 *
 * \par<b>Description: </b><br>
 * Initialize the RTC peripheral <br>
 *
 * The function ungates the peripheral clock for RTC, configures
 * internal clock divider, time and alarm values by writing to the CTR.DIV, TIM0, TIM1, ATIM0 and
 * ATIM1 registers.
 */
Cy_RTC_STATUS_t Cy_RTC_Init(const Cy_RTC_CONFIG_t *const config);

/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables RTC peripheral for programming its registers <br>
 *
 * Ungates the peripheral clock.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_Disable(), Cy_SCU_RESET_DeassertPeripheralReset()
 */
void Cy_RTC_Enable(void);

/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables RTC peripheral for programming its registers <br>
 *
 * Gates the peripheral clock.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_Enable(), Cy_SCU_RESET_AssertPeripheralReset()
 */
void Cy_RTC_Disable(void);

/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Checks RTC peripheral is enabled for programming its registers <br>
 *
 * Checks peripheral clock is ungated or not.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_Enable(), Cy_RTC_Disable(), Cy_SCU_RESET_DeassertPeripheralReset(),
 * Cy_SCU_RESET_AssertPeripheralReset()
 */
bool Cy_RTC_IsEnabled(void);

/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables RTC peripheral to start counting time <br>
 *
 * \par
 * The function starts the RTC for counting time by setting
 * CTR.ENB bit. Before starting the RTC, it should not be in
 * running mode and also hibernate domain should be enabled.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_Enable(), Cy_RTC_Stop(), Cy_SCU_RESET_DeassertPeripheralReset()
 */
void Cy_RTC_Start(void);

/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables RTC peripheral to start counting time <br>
 *
 * \par
 * The function stops the RTC for counting time by resetting
 * CTR.ENB. Before stopping the RTC, hibernate domain should be enabled.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_Enable(), Cy_RTC_Start(), Cy_SCU_RESET_AssertPeripheralReset()
 */
void Cy_RTC_Stop(void);

/**
 * @param prescaler Prescaler value to be set
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the RTC module prescaler value <br>
 *
 * \par
 * The function sets the CTR.DIV bitfield to configure the prescalar value.
 * The default value for the prescalar with the 32.768kHz crystal (or the internal clock)
 * is 7FFFH for a time interval of 1 sec. Before setting the prescaler value RTC should be
 * in stop mode and hibernate domain should be enabled.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_Stop(), Cy_RTC_Enable(), Cy_RTC_GetPrescaler()
 */
void Cy_RTC_SetPrescaler(uint16_t prescaler);

/**
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC module prescaler value <br>
 *
 * \par
 * The function reads the CTR.DIV bitfield to get the prescalar value. The default value
 * for the prescalar with the 32.768kHz crystal (or the internal clock) is 7FFFH for a
 * time interval of 1 sec.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_SetPrescaler()
 */
__STATIC_INLINE uint32_t Cy_RTC_GetPrescaler(void)
{
  return (uint32_t)(((uint32_t)RTC->CTR & (uint32_t)RTC_CTR_DIV_Msk) >> (uint32_t)RTC_CTR_DIV_Pos);
}

/**
 * @param timeval Contstant pointer to a constant ::Cy_RTC_TIME_t structure containing the
 *                time parameters seconds, minutes, hours, days, daysofweek, month and year.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the RTC module time values <br>
 *
 * \par
 * The function sets the TIM0, TIM1 registers with time values.
 * The values can only be written when RTC is disabled.
 * See the structure ::Cy_RTC_TIME_t for the valid range of time value parameters. <br>
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_GetTime(), Cy_RTC_Stop()
 */
void Cy_RTC_SetTime(const Cy_RTC_TIME_t *const timeval);

/**
 * @param time Pointer to a constant ::Cy_RTC_TIME_t structure containing the time parameters
 *             seconds, minutes, hours, days, daysofweek, month and year.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC module time value <br>
 *
 * \par
 * The function gets the time values from TIM0, TIM1 registers.
 * See the structure ::Cy_RTC_TIME_t for the valid range of time value parameters. <br>
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_SetTime()
 */
void Cy_RTC_GetTime(Cy_RTC_TIME_t *const time);

/**
 * @param stdtime Pointer to a ::tm structure containing the time parameters seconds,
 *                minutes, hours, days, daysofweek, month, year(since 1900) and days in a
 *                year in standard format.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the RTC module time value in standard format <br>
 *
 * \par
 * The function sets the time values from TIM0, TIM1 registers.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_SetTime(), Cy_RTC_GetTime()
 */
void Cy_RTC_SetTimeStdFormat(const struct tm *const stdtime);

/**
 * @param stdtime Pointer to a constant ::tm structure containing the time parameters seconds,
 *                minutes, hours, days, daysofweek, month, year(since 1900) and days in a
 *                year in standard format.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC module time value in standard format <br>
 *
 * \par
 * The function gets the time values from TIM0, TIM1 registers.
 * See the structure ::Cy_RTC_TIME_t for the valid range of time value parameters. <br>
 * For days the valid range is (1 - Actual days of month), year (since 1900) and
 * daysinyear (0 -365).
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_SetTime(), Cy_RTC_GetTime()
 */
void Cy_RTC_GetTimeStdFormat(struct tm *const stdtime);

/**
 * @param alarm Constant pointer to a constant ::Cy_RTC_ALARM_t structure containing the
 *                alarm time parameters alarm seconds, alarm minutes, alarm hours, alarm days,
 *                alarm daysofweek, alarm month and alarm year.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the RTC module alarm time value <br>
 *
 * \par
 * The function sets the ATIM0, ATIM1 registers with alarm time values.
 * See the structure ::Cy_RTC_ALARM_t for the valid range of alarm time value parameters. <br>
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_GetAlarm()
 */
void Cy_RTC_SetAlarm(const Cy_RTC_ALARM_t *const alarm);

/**
 * @param alarm Pointer to a constant ::Cy_RTC_ALARM_t structure containing the
 *             time parameters alarm seconds, alarm minutes, alarm hours, alarm days,
 *             alarm daysofweek, alarm month and alarm year.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC module alarm time value <br>
 *
 * \par
 * The function gets the alarm time values from ATIM0, ATIM1 registers.
 * See the structure ::Cy_RTC_ALARM_t for the valid range of alarm time value parameters. <br>
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_SetAlarm()
 */
void Cy_RTC_GetAlarm(Cy_RTC_ALARM_t *const alarm);

/**
 * @param stdtime Pointer to a ::tm structure containing the time parameters alarm seconds,
 *                alarm minutes, alarm hours, alarm days, alarm daysofweek, alarm month,
 *                alarm year(since 1900) and alarm days in a year in standard format.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the RTC module alarm time value in standard format <br>
 *
 * \par
 * The function sets the alarm time values from ATIM0, ATIM1 registers.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_SetAlarm(), Cy_RTC_GetAlarm()
 */
void Cy_RTC_SetAlarmStdFormat(const struct tm *const stdtime);

/**
 * @param stdtime Pointer to a constant ::tm structure containing the time parameters alarm seconds,
 *                alarm minutes, alarm hours, alarm days, alarm daysofweek, alarm month,
 *                alarm year(since 1900) and alarm days in a year in standard format.
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC module alarm time value in standard format <br>
 *
 * \par
 * The function gets the alarm time values from ATIM0, ATIM1 registers.
 * See the structure ::Cy_RTC_ALARM_t for the valid range of alarm time value parameters. <br>
 * For days the valid range is (1 - Actual days of month), year (since 1900) and
 * daysinyear (0 -365).
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_SetAlarm(), Cy_RTC_GetAlarm()
 */
void Cy_RTC_GetAlarmStdFormat(struct tm *const stdtime);

/**
 * @param event A valid RTC event (::Cy_RTC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable RTC periodic and alarm event(s) <br>
 *
 * \par
 * The function sets the bitfields of MSKSR register to enable interrupt generation
 * for requested RTC event(s).
 * Setting the masking value for the event(s) containing in the ::Cy_RTC_EVENT_t leads
 * to a generation of the interrupt.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_DisableEvent()
 */
void Cy_RTC_EnableEvent(const uint32_t event);

/**
 * @param event A valid RTC event (::Cy_RTC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable RTC periodic and alarm event(s) <br>
 *
 * \par
 * The function resets the bitfields of MSKSR register to disable interrupt generation
 * for requested RTC event(s).
 * Resetting the masking value for the the event(s) containing in the ::Cy_RTC_EVENT_t blocks
 * the generation of the interrupt.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_EnableEvent()
 */
void Cy_RTC_DisableEvent(const uint32_t event);

/**
 * @param event A valid RTC event (::Cy_RTC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Clears periodic and alarm event(s) status <br>
 *
 * \par
 * The function sets the bitfields of CLRSR register to clear status bits in RAWSTAT and STSSR registers.
 * Setting the value for the the RTC event(s) containing in the ::Cy_RTC_EVENT_t clears the
 * corresponding status bits in RAWSTAT and STSSR registers.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_GetEventStatus()
 */
void Cy_RTC_ClearEvent(const uint32_t event);

/**
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC periodic and alarm event(s) status <br>
 *
 * \par
 * The function reads the bitfields of STSSR register
 * to get the status of RTC events.
 * Reading the value of the register STSSR gives the status of the event(s) containing in the ::Cy_RTC_EVENT_t.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_ClearEvent()
 */
uint32_t Cy_RTC_GetEventStatus(void);

/**
 * @return bool true if RTC is running
 *              false if RTC is not running
 *
 * \par<b>Description: </b><br>
 * Checks the running status of the RTC <br>
 *
 * \par
 * The function reads the bitfield ENB of CTR register
 * to get the running status of RTC.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_RTC_Start(), Cy_RTC_Stop()
 */
__STATIC_INLINE bool Cy_RTC_IsRunning(void)
{
  return (bool)(RTC->CTR & RTC_CTR_ENB_Msk);
}

/**
 * @param debug_mode Debug mode value containing in (::Cy_RTC_DEBUG_MODE_t) to be set
 * @return None
 *
 * \par<b>Description: </b><br>
 * Configures the RTC into running or stopping mode during halting mode debug <br>
 *
 * \par
 * The function sets the CTR.SUS bitfield to configure the RTC into running
 * or stopping mode during halting mode debug.
 */
void Cy_RTC_SetDebugMode(const Cy_RTC_DEBUG_MODE_t debug_mode);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */


#endif /* CY_RTC_H */
