/**
 * @file cy_wdt.h
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

#ifndef CY_WDT_H
#define CY_WDT_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include "cy_common.h"
#include "cy_scu.h"


/**
 * @addtogroup WDT
 * @brief Watchdog driver for the PSC1M microcontroller family.
 *
 * The watchdog unit (WDT) improves the system integrity, by triggering the system reset request to bring the system
 * back from the unresponsive state to normal operation.
 *
 * This LLD provides the Configuration structure Cy_WDT_CONFIG_t and initialization function Cy_WDT_Init().\n
 * It can be used to:
 * -# Start or Stop the watchdog timer. (Cy_WDT_Start() and Cy_WDT_Stop())
 * -# Service the watchdog timer. (Cy_WDT_Service())
 * -# Configure the service window upper bound and lower bound timing values. (Cy_WDT_SetWindowBounds())
 * -# Enable the generation of the pre-warning event for the first overflow of the timer. (Cy_WDT_SetMode())
 * -# Clear the pre-warning alarm event. It is mandatory to clear the flag during pre-warning alarm ISR, to stop
      generating reset request for the second overflow of the timer. (Cy_WDT_ClearAlarm())
 * -# Suspend the watchdog timer during Debug HALT mode. (Cy_WDT_SetDebugMode())
 * -# Configure service indication pulse width.(Cy_WDT_SetServicePulseWidth())
 *
 * Also, the WDT block can be configured from the ModusToolbox&trade; Device Configurator: \ref section_personality_wdt
 * @{
 */

/*********************************************************************************************************************
 * MACROS
  ********************************************************************************************************************/

#define CY_WDT_MAGIC_WORD             (0xABADCAFEU) /* Magic word to be written in Service Register (SRV),
                                                          to service or feed the watchdog. */

/*********************************************************************************************************************
 * ENUMS
  ********************************************************************************************************************/

/**
 *  Defines working modes for watchdog. Use type Cy_WDT_MODE_t for this enum.
 */
typedef enum CY_WDT_MODE
{
  CY_WDT_MODE_TIMEOUT    = (uint32_t)0x0 << WDT_CTR_PRE_Pos, /**< Generates reset request as soon as the timer overflow
                                                                   occurs. */
  CY_WDT_MODE_PREWARNING = (uint32_t)0x1 << WDT_CTR_PRE_Pos  /**< Generates an alarm event for the first overflow. And
                                                                   reset request after subsequent overflow, if not
                                                                   serviced after first overflow. */
} Cy_WDT_MODE_t;

/**
 *  Defines debug behaviour of watchdog when the CPU enters HALT mode. Use type Cy_WDT_DEBUG_MODE_t for this enum.
 */
typedef enum CY_WDT_DEBUG_MODE
{
  CY_WDT_DEBUG_MODE_STOP = (uint32_t)0x0 << WDT_CTR_DSP_Pos, /**< Watchdog counter is paused during debug halt. */
  CY_WDT_DEBUG_MODE_RUN  = (uint32_t)0x1 << WDT_CTR_DSP_Pos  /**< Watchdog counter is not paused during debug halt. */
} Cy_WDT_DEBUG_MODE_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
  ********************************************************************************************************************/
/* Anonymous structure/union guard start */
#if defined(__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined(__TASKING__)
#pragma warning 586
#endif

/**
 * Structure for initializing watchdog timer. Use type Cy_WDT_CONFIG_t for this structure.
 */
typedef struct CY_WDT_CONFIG
{
  uint32_t window_upper_bound; /**< Upper bound for service window (WUB). Reset request is generated up on overflow of
                                    timer. ALways upper bound value has to be more than lower bound value. If it is set
                                    lower than WLB, triggers a system reset after timer crossed upper bound value.\n
                                    Range: [0H to FFFFFFFFH] */
  uint32_t window_lower_bound; /**< Lower bound for servicing window (WLB). Setting the lower bound to 0H disables the
                                    window mechanism.\n
                                    Range: [0H to FFFFFFFFH] */
  union
  {
    struct
    {
      uint32_t : 1;
      uint32_t prewarn_mode : 1;        /**< Pre-warning mode (PRE). This accepts boolean values as input. */
      uint32_t : 2;
      uint32_t run_in_debug_mode : 1;   /**< Watchdog timer behaviour during debug (DSP). This accepts boolean values as input. */
      uint32_t : 3;
      uint32_t service_pulse_width : 8; /**< Service Indication Pulse Width (SPW). Generated Pulse width is of (SPW+1),
                                             in fwdt cycles.\n
                                             Range: [0H to FFH] */
      uint32_t : 16;
    };
    uint32_t wdt_ctr; /* Value of operation mode control register (CTR). It’s bit fields are represented by above
                           union members. */
  };
} Cy_WDT_CONFIG_t;
/* Anonymous structure/union guard end */
#if defined(__CC_ARM)
#pragma pop
#elif defined(__TASKING__)
#pragma warning restore
#endif
/*********************************************************************************************************************
 * API PROTOTYPES
  ********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables watchdog clock.\n
 * \par
 * This API is invoked by Cy_WDT_Init() and therefore no need to call it explicitly during watchdog initialization
 * sequence. Invoke this API to enable watchdog once again if the watchdog is disabled by invoking Cy_WDT_Disable().
 *
 * \par<b>Note:</b><br>
 * 1. Not required to configure the watchdog again after invoking Cy_WDT_Disable(). Since the registers retains with
 *    the configured values.
 * \par<b>Related APIs:</b><BR>
 * Cy_WDT_Init(), Cy_WDT_Disable()
 */
void Cy_WDT_Enable(void);

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the clock to the watchdog timer.\n
 *
 * \par<b>Note:</b><br>
 * 1. After invoking Cy_WDT_Disable(), all register values are displayed with 0F in debugger. Once enabled by
      calling Cy_WDT_Enable(), previous configured register values are displayed. No need to invoke Cy_WDT_Init()
      again.
 * \par<b>Related APIs:</b><BR>
 * Cy_WDT_Enable()
 */
void Cy_WDT_Disable(void);

/**
 * @param config pointer to a constant watchdog configuration data structure. Refer data structure Cy_WDT_CONFIG_t
 *               for detail.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Initializes and configures watchdog with configuration data pointed by \a config.\n
 * \par
 * It invokes Cy_WDT_Enable() to enable clock and release reset. Then configures the lower and upper window bounds,
 * working mode (timeout/pre-warning), debug behaviour and service request indication pulse width.
 *
 * \par<b>Note:</b><br>
 * 1. With out invoking this Cy_WDT_Init() or Cy_WDT_Enable(), invocation of other APIs like Cy_WDT_SetWindowBounds(),
 *    Cy_WDT_SetMode(), Cy_WDT_SetServicePulseWidth(), Cy_WDT_SetDebugMode(), Cy_WDT_Start(), Cy_WDT_GetCounter(),
 *    Cy_WDT_Service(), Cy_WDT_ClearAlarm() has no affect.
 */
void Cy_WDT_Init(const Cy_WDT_CONFIG_t *const config);

/**
 * @param lower_bound specifies watchdog window lower bound in terms of watchdog clock (fWDT) cycles.
 *                    Range: [0H to FFFFFFFFH].
 * @param upper_bound specifies watchdog window upper bound in terms of watchdog clock (fWDT) cycles.
 *                    Range: [0H to FFFFFFFFH].
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets watchdog window lower and upper bounds by updating WLB and WUB registers.\n
 * \par
 * Window lower and upper bounds are set during initialization in Cy_WDT_Init(). Invoke this API to alter the values as
 * needed later in the program. This upper bound and lower bound can be calculated by using the below formula\n
 *       upper_bound or lower_bound = desired_boundary_time(sec) * fwdt(hz)
 *
 * \par<b>Note:</b>
 * 1. Always ensure that upper_bound is greater than the lower_bound value. If not, whenever timer crosses the
 *    upper_bound value it triggers the reset(wdt_rst_req) of the controller.
 */
__STATIC_INLINE void Cy_WDT_SetWindowBounds(uint32_t lower_bound, uint32_t upper_bound)
{
  WDT->WLB = lower_bound;
  WDT->WUB = upper_bound;
}

/**
 * @param mode is one of the working modes of the watchdog timer, i.e timeout or pre-warning. Refer @ref Cy_WDT_MODE_t
 *             for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets watchdog working mode (timeout or pre-warning) by updating PRE bit of CTR register.\n
 * \par
 * The working mode is set during initialization in Cy_WDT_Init(). Invoke this API to alter the mode as needed later in
 * the program.
 */
__STATIC_INLINE void Cy_WDT_SetMode(Cy_WDT_MODE_t mode)
{
  WDT->CTR = (WDT->CTR & (uint32_t)~WDT_CTR_PRE_Msk) | (uint32_t)mode;
}

/**
 * @param service_pulse_width specifies Service indication pulse width in terms of fwdt.
 *                            Range: [0H – FFH].
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets service indication pulse width by updating SPW bit field of CTR register.\n
 * \par
 * The service indication pulse (with width service_pulse_width + 1 in fwdt cycles) is generated on successful servicing
 * or feeding of watchdog. The pulse width is initially set during initialization in Cy_WDT_Init(). Invoke this API to
 * alter the width as needed later in the program.
 */
__STATIC_INLINE void Cy_WDT_SetServicePulseWidth(uint8_t service_pulse_width)
{
  WDT->CTR = (WDT->CTR & (uint32_t)~WDT_CTR_SPW_Msk) | ((uint32_t)service_pulse_width << WDT_CTR_SPW_Pos);
}

/**
 * @param debug_mode running state of watchdog during debug halt mode. Refer @ref Cy_WDT_DEBUG_MODE_t for
 *                   valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets debug behaviour of watchdog by modifying DSP bit of CTR register.\n
 * \par
 * Depending upon DSP bit, the watchdog timer stops when CPU is in HALT mode. The debug behaviour is initially set as
 * CY_WDT_DEBUG_MODE_STOP during initialization in Cy_WDT_Init(). Invoke this API to change the debug behaviour as
 * needed later in the program.
 */
__STATIC_INLINE void Cy_WDT_SetDebugMode(const Cy_WDT_DEBUG_MODE_t debug_mode)
{
  WDT->CTR = (WDT->CTR & (uint32_t)~WDT_CTR_DSP_Msk) | (uint32_t)debug_mode;
}

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Start the watchdog timer by setting ENB bit of CTR register.\n
 * \par
 * Invoke this API to start the watchdog after initialization, or to resume the watchdog when
 * paused by invoking Cy_WDT_Stop().
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_WDT_Init(), Cy_WDT_Stop()
 */
__STATIC_INLINE void Cy_WDT_Start(void)
{
  WDT->CTR |= (uint32_t)WDT_CTR_ENB_Msk;
}

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Pauses watchdog timer by resetting ENB bit of CTR register.\n
 * \par
 * Invoke this API to pause the watchdog as needed in the program e.g. debugging through software control.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_WDT_Init(), Cy_WDT_Stop()
 */
__STATIC_INLINE void Cy_WDT_Stop(void)
{
  WDT->CTR &= (uint32_t)~WDT_CTR_ENB_Msk;
}

/**
 * @param None
 *
 * @return uint32_t Current count value of watchdog timer register (TIM).
 *                  Range: [0H to FFFFFFFFH]
 *
 * \par<b>Description:</b><br>
 * Reads current count of timer register (TIM).\n
 * \par
 * Invoke this API before servicing or feeding the watchdog to check whether count is between lower and upper
 * window bounds.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_WDT_Service()
 */
__STATIC_INLINE uint32_t Cy_WDT_GetCounter(void)
{
  return WDT->TIM;
}

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Services or feeds the watchdog by writing the Magic word in SRV register.\n
 * \par
 * Service watchdog when count value of watchdog timer is between lower and upper window bounds. Successful servicing
 * will reset watchdog timer (TIM register) to 0H and generate service indication pulse.
 *
 * \par<b>Note:</b><br>
 * 1. invoking this API when count value of watchdog timer is less than window lower bound results
 *    wrong servicing and immediately triggers reset request.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_WDT_GetCounter(), Cy_WDT_SetWindowBounds(), Cy_WDT_SetServicePulseWidth()
 */
__STATIC_INLINE void Cy_WDT_Service(void)
{
  WDT->SRV = CY_WDT_MAGIC_WORD;
}

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Clears pre-warning alarm by setting ALMC bit in WDTCLR register.\n
 * \par
 * In pre-warning mode, first overflow of the timer upper window bound fires the pre-warning alarm. Cy_WDT_ClearAlarm()
 * must be invoked to clear the alarm alarm. After clearing of the alarm, watchdog timer must be serviced within valid
 * time window. Otherwise watchdog timer triggers the reset request up on crossing the upper bound value in a subsequent
 * cycle.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_WDT_Service(), Cy_WDT_SetMode()
 */
__STATIC_INLINE void Cy_WDT_ClearAlarm(void)
{
  WDT->WDTCLR = WDT_WDTCLR_ALMC_Msk;
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* CY_WDT_H */
