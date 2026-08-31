/**
 * @file cy_common.h
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

#ifndef CY_COMMON_H
#define CY_COMMON_H

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "cy_device_headers.h"
#include "cy_utils.h"

/**
 * @addtogroup COMMON
 * @brief Common APIs to all peripherals for PSC1M microcontroller family

 * The Common driver contains a set of different system functions. These functions
 * can be called in the application routine.
 * @{
 */

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
#define CY_PSC1M3_LIB_MAJOR_VERSION	(1U)
#define CY_PSC1M3_LIB_MINOR_VERSION	(0U)
#define CY_PSC1M3_LIB_PATCH_VERSION	(0U)

#define CY_PSC1M3_LIB_VERSION         ((CY_PSC1M3_LIB_MAJOR_VERSION << 16U) + (CY_PSC1M3_LIB_MINOR_VERSION << 8U) + CY_PSC1M3_LIB_PATCH_VERSION)

#define CY_VERSION_NUMBER(major, minor, patch)         ((major << 16U) + (minor << 8U) + patch)

/* Define WEAK attribute */
#if !defined(__WEAK)
#if defined ( __CC_ARM )
#define __WEAK __attribute__ ((weak))
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define __WEAK __attribute__ ((weak))
#elif defined ( __ICCARM__ )
#define __WEAK __weak
#elif defined ( __GNUC__ )
#define __WEAK __attribute__ ((weak))
#elif defined ( __TASKING__ )
#define __WEAK __attribute__ ((weak))
#endif
#endif

#if !defined(__RAM_FUNC)
#if defined ( __CC_ARM )
// http://www.keil.com/support/docs/3723.htm
#define __RAM_FUNC __attribute__((section("RAMCODESECTION")))
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define __RAM_FUNC __attribute__((section("RAMCODESECTION")))
#elif defined ( __ICCARM__ )
// https://www.iar.com/support/tech-notes/linker/controlling-placement-of-the-section-where-__ramfunc-functions-reside-ewarm-5.x--6.x/
#define __RAM_FUNC __ramfunc
#elif defined ( __GNUC__ )
#define __RAM_FUNC __attribute__((section(".ram_code"), long_call))
#elif defined ( __TASKING__ )
#define __RAM_FUNC __attribute__((section(".ram_code")))
#endif
#endif
#ifdef CY_DEBUG_ENABLE
#include <stdio.h>
#define CY_DEBUG(...) { printf(__VA_ARGS__); }
#else
#define CY_DEBUG(...)
#endif

#define CY_UNUSED_ARG(x) (void)x

#define CY_STRUCT_INIT(m) memset(&m, 0, sizeof(m))

/***************************************************************************************************
 * Macro Name: CY_DIV_ROUND
 ***********************************************************************************************//**
 *
 *  Calculates a / b with rounding to the nearest integer,
 *  a and b must have the same sign.
 *
 **************************************************************************************************/
#define CY_DIV_ROUND(a, b) (((a) + ((b) / 2U)) / (b))

/***************************************************************************************************
 * Macro Name: CY_DIV_ROUNDUP
 ***********************************************************************************************//**
 *
 *  Calculates a / b with rounding up if remainder != 0,
 *  both a and b must be positive.
 *
 **************************************************************************************************/
#define CY_DIV_ROUNDUP(a, b) ((((a) - 1U) / (b)) + 1U)

/** @cond */
/**********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/
/*
 *
 */
typedef struct CY_DRIVER_VERSION
{
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
} Cy_DRIVER_VERSION_t;
/** @endcond */

/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Function Name: Cy_DelayCycles
****************************************************************************//**
*
* The function delays by the specified number of CPU clock cycles.
*
* \note There is memory reading timing limitation in PSC1M device family
*       (see section Memory Read in the device TRM)
*       this may significantly stretch this function execution,
*       especially at high CPU speeds.
*
* @param cycles The number of CPU clock cycles to delay.
*               Valid range: 0x0 .. 0xFFFFFFFD.
*
*******************************************************************************/
void Cy_DelayCycles(uint32_t cycles);

/*******************************************************************************
* Function Name: Cy_Delay
****************************************************************************//**
*
* The function delays by the specified number of milliseconds.
* The number of cycles to delay is calculated based on the
* \ref SystemCoreClock.
*
* @param milliseconds  The number of milliseconds to delay.
*
* \note The function calls \ref Cy_DelayCycles() API to generate a delay.
*       If the desired delay is bigger than 0xFFFFFFFD CPU cycles,
*       then an additional loop runs to prevent
*       an overflow in parameter passed to \ref Cy_DelayCycles() API.
*
*******************************************************************************/
void Cy_Delay(uint32_t milliseconds);

/*******************************************************************************
* Function Name: Cy_DelayUs
****************************************************************************//**
*
* The function delays by the specified number of microseconds.
* The number of cycles to delay is calculated based on the
* \ref SystemCoreClock.
*
* @param microseconds  The number of microseconds to delay.
*
* \note If the CPU frequency in MHz is a small number,
*       the actual delay can be significantly bigger than the desired value.
*       The actual delay cannot be shorter than the desired one.
*
*******************************************************************************/
void Cy_DelayUs(uint16_t microseconds);

/*******************************************************************************
* Function Name: Cy_EnterCriticalSection
****************************************************************************//**
*
*  Cy_EnterCriticalSection disables interrupts and returns a value
*  indicating whether the interrupts were previously enabled.
*
*  \return Returns the current interrupt status. Returns 0 if the interrupts
*          were previously enabled or 1 if the interrupts were previously
*          disabled.
*
*  \note Implementation of Cy_EnterCriticalSection manipulates the IRQ
*        enable bit with interrupts still enabled.
*
*******************************************************************************/
uint32_t Cy_EnterCriticalSection(void);

/*******************************************************************************
* Function Name: Cy_ExitCriticalSection
****************************************************************************//**
*
*  Re-enables the interrupts if they were enabled before
*  Cy_EnterCriticalSection() was called. The argument should be the value
*  returned from \ref Cy_EnterCriticalSection().
*
*  \param savedIntrStatus  Puts the saved interrupts status returned by
*         the \ref Cy_EnterCriticalSection().
*
*******************************************************************************/
void Cy_ExitCriticalSection(uint32_t savedIntrStatus);

#ifdef __cplusplus
}
#endif

/**
 * @} (end addtogroup COMMON)
 */

#endif /* CY_COMMON_H */
