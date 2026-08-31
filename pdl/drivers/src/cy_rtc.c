/**
 * @file cy_rtc.c
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

/**
 *
 * @brief RTC driver for PSC1M microcontroller family.
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "cy_scu.h"
#include "cy_rtc.h"

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

#define CY_RTC_MAXSECONDS  (60U)   /**< RTC time : Maximum seconds */
#define CY_RTC_MAXMINUTES  (60U)   /**< RTC time : Maximum minutes */
#define CY_RTC_MAXHOURS    (24U)   /**< RTC time : Maximum hours */
#define CY_RTC_MAXDAYS     (31U)   /**< RTC time : Maximum days */
#define CY_RTC_MAXDAYSOFWEEK  (7U) /**< RTC time : Maximum days of week */
#define CY_RTC_MAXMONTH  (12U)     /**< RTC time : Maximum month */
#define CY_RTC_MAXYEAR  (0xFFFFU)  /**< RTC time : Maximum year */
#define CY_RTC_MAXPRESCALER  (0xFFFFU)  /**< RTC time : Maximum prescaler */
#define CY_RTC_YEAR_OFFSET (1900U)      /**< RTC year offset : Year offset */

#define CY_RTC_INIT_SEQUENCE  (0U)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/

/*
 * Enables RTC peripheral to start counting time
 */
void Cy_RTC_Start(void)
{
  while ((Cy_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_CTR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->CTR |= (uint32_t)RTC_CTR_ENB_Msk;
}

/*
 * Disables RTC peripheral to start counting time
 */
void Cy_RTC_Stop(void)
{
  while ((Cy_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_CTR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->CTR &= ~(uint32_t)RTC_CTR_ENB_Msk;
}

/*
 * Sets the RTC module prescaler value
 */
void Cy_RTC_SetPrescaler(uint16_t prescaler)
{
  CY_ASSERT((prescaler < CY_RTC_MAXPRESCALER));

  while ((Cy_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_CTR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->CTR = (RTC->CTR & ~(uint32_t)RTC_CTR_DIV_Msk) |
             ((uint32_t)prescaler << (uint32_t)RTC_CTR_DIV_Pos);
}

/*
 * Sets the RTC_TIM0, RTC_TIM1 registers with time values
 */
void Cy_RTC_SetTime(const Cy_RTC_TIME_t *const time)
{
  CY_ASSERT(((uint32_t)time->seconds < CY_RTC_MAXSECONDS));
  CY_ASSERT(((uint32_t)time->minutes < CY_RTC_MAXMINUTES));
  CY_ASSERT(((uint32_t)time->hours < CY_RTC_MAXHOURS));
  CY_ASSERT(((uint32_t)time->daysofweek < CY_RTC_MAXDAYSOFWEEK));
  CY_ASSERT(((uint32_t)time->month < CY_RTC_MAXMONTH));
  CY_ASSERT(((uint32_t)time->year < CY_RTC_MAXYEAR));

#if (CY_RTC_INIT_SEQUENCE == 1U)
  while ((Cy_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_TIM0_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->TIM0 = time->raw0;

  while ((Cy_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_TIM1_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->TIM1 = time->raw1;
#endif
#if (CY_RTC_INIT_SEQUENCE == 0U)
  while ((Cy_SCU_GetMirrorStatus() & (SCU_GENERAL_MIRRSTS_RTC_TIM0_Msk | SCU_GENERAL_MIRRSTS_RTC_TIM1_Msk)) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->TIM0 = time->raw0;
  RTC->TIM1 = time->raw1;	;
#endif
}

/*
 * Gets the RTC module time value
 */
void Cy_RTC_GetTime(Cy_RTC_TIME_t *const time)
{
  time->raw0 = RTC->TIM0;
  time->raw1 = RTC->TIM1;
}

/*
 * Sets the RTC module time values in standard format
 */
void Cy_RTC_SetTimeStdFormat(const struct tm *const stdtime)
{

  Cy_RTC_TIME_t time;

  time.seconds = stdtime->tm_sec;
  time.minutes = stdtime->tm_min;
  time.hours = stdtime->tm_hour;
  time.days = stdtime->tm_mday - 1;
  time.month = stdtime->tm_mon;
  time.year = stdtime->tm_year + CY_RTC_YEAR_OFFSET;
  time.daysofweek = stdtime->tm_wday;

  Cy_RTC_SetTime(&time);
}

/*
 * Gets the RTC module time values in standard format
 */
void Cy_RTC_GetTimeStdFormat(struct tm *const stdtime)
{
  Cy_RTC_TIME_t time;
  time.raw0 = RTC->TIM0;
  time.raw1 = RTC->TIM1;

  stdtime->tm_sec = (int8_t)time.seconds;
  stdtime->tm_min = (int8_t)time.minutes;
  stdtime->tm_hour = (int8_t)time.hours;
  stdtime->tm_mday = ((int8_t)time.days + (int8_t)1);
  stdtime->tm_mon = (int8_t)time.month;
  stdtime->tm_year = (int32_t)time.year - (int32_t)CY_RTC_YEAR_OFFSET;
  stdtime->tm_wday = (int8_t)time.daysofweek;
}

/*
 * Sets the RTC module alarm time value
 */
void Cy_RTC_SetAlarm(const Cy_RTC_ALARM_t *const alarm)
{
  CY_ASSERT(((uint32_t)alarm->seconds < CY_RTC_MAXSECONDS));
  CY_ASSERT(((uint32_t)alarm->minutes < CY_RTC_MAXMINUTES));
  CY_ASSERT(((uint32_t)alarm->hours < CY_RTC_MAXHOURS));
  CY_ASSERT(((uint32_t)alarm->month < CY_RTC_MAXMONTH));
  CY_ASSERT(((uint32_t)alarm->year < CY_RTC_MAXYEAR));

#if (CY_RTC_INIT_SEQUENCE == 1U)
  while ((Cy_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_ATIM0_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->ATIM0 = alarm->raw0;

  while ((Cy_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_ATIM1_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->ATIM1 = alarm->raw1;
#endif
#if (CY_RTC_INIT_SEQUENCE == 0U)
  while ((Cy_SCU_GetMirrorStatus() & (SCU_GENERAL_MIRRSTS_RTC_ATIM0_Msk | SCU_GENERAL_MIRRSTS_RTC_ATIM1_Msk)) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->ATIM0 = alarm->raw0;
  RTC->ATIM1 = alarm->raw1;
#endif
}

/*
 * Gets the RTC module alarm time value
 */
void Cy_RTC_GetAlarm(Cy_RTC_ALARM_t *const alarm)
{
  alarm->raw0 = RTC->ATIM0;
  alarm->raw1 = RTC->ATIM1;
}


/*
 * Sets the RTC module alarm time value in standard format
 */
void Cy_RTC_SetAlarmStdFormat(const struct tm *const stdtime)
{
  Cy_RTC_ALARM_t alarm;


  alarm.seconds = stdtime->tm_sec;
  alarm.minutes = stdtime->tm_min;
  alarm.hours = stdtime->tm_hour;
  alarm.days = stdtime->tm_mday - 1;
  alarm.month = stdtime->tm_mon;
  alarm.year = stdtime->tm_year + CY_RTC_YEAR_OFFSET;

  Cy_RTC_SetAlarm(&alarm);
}

/*
 * Gets the RTC module alarm time value in standard format
 */
void Cy_RTC_GetAlarmStdFormat(struct tm *const stdtime)
{
  Cy_RTC_ALARM_t alarm;

  alarm.raw0 = RTC->ATIM0;
  alarm.raw1 = RTC->ATIM1;

  stdtime->tm_sec = (int8_t)alarm.seconds;
  stdtime->tm_min = (int8_t)alarm.minutes;
  stdtime->tm_hour = (int8_t)alarm.hours;
  stdtime->tm_mday = ((int8_t)alarm.days + (int8_t)1);
  stdtime->tm_mon = (int8_t)alarm.month;
  stdtime->tm_year = (int32_t)alarm.year - (int32_t)CY_RTC_YEAR_OFFSET;
}

/*
 * Gets the RTC periodic and alarm event(s) status
 */
uint32_t Cy_RTC_GetEventStatus(void)
{
  return RTC->STSSR;
}

/*
 * Initialize the RTC peripheral
 */
Cy_RTC_STATUS_t Cy_RTC_Init(const Cy_RTC_CONFIG_t *const config)
{
  if (Cy_RTC_IsRunning() == false)
  {
    if (Cy_RTC_IsEnabled() == false)
    {
      Cy_RTC_Enable();
    }

    Cy_RTC_SetPrescaler(config->prescaler);

    while ((Cy_SCU_GetMirrorStatus() & (SCU_GENERAL_MIRRSTS_RTC_TIM0_Msk | SCU_GENERAL_MIRRSTS_RTC_TIM1_Msk)) != 0U)
    {
      /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
    }
    RTC->TIM0 = config->time.raw0;
    RTC->TIM1 = config->time.raw1;

    while ((Cy_SCU_GetMirrorStatus() & (SCU_GENERAL_MIRRSTS_RTC_ATIM0_Msk | SCU_GENERAL_MIRRSTS_RTC_ATIM1_Msk)) != 0U)
    {
      /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
    }
    RTC->ATIM0 = config->alarm.raw0;
    RTC->ATIM1 = config->alarm.raw1;
  }
  return CY_RTC_STATUS_OK;
}

/*
 * Ungates a clock node for RTC
 */
void Cy_RTC_Enable(void)
{
  Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_RTC);
}

/*
 * Gates a clock node for RTC
 */
void Cy_RTC_Disable(void)
{
  Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_RTC);
}

/*
 * Suspends RTC function during CPU HALT mode
 */
void Cy_RTC_SetDebugMode(const Cy_RTC_DEBUG_MODE_t debug_mode)
{
  uint32_t regval;
  regval = (RTC->CTR & (uint32_t)~RTC_CTR_SUS_Msk);
  RTC->CTR = (regval | (uint32_t)debug_mode);
}

/*
 * Enable RTC periodic and alarm event(s)
 */
void Cy_RTC_EnableEvent(const uint32_t event)
{
  RTC->MSKSR |= event;
}

/*
 * Disable RTC periodic and alarm event(s)
 */
void Cy_RTC_DisableEvent(const uint32_t event)
{
  RTC->MSKSR &= ~event;
}

/*
 * Clear RTC periodic and alarm event(s)
 */
void Cy_RTC_ClearEvent(const uint32_t event)
{
  RTC->CLRSR = event;
}

/*
 * Checks RTC peripheral is enabled for programming to its registers
 */
bool Cy_RTC_IsEnabled(void)
{
  return !Cy_SCU_CLOCK_IsPeripheralClockGated(CY_SCU_PERIPHERAL_CLOCK_RTC);
}
