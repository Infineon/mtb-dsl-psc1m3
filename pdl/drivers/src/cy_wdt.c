/**
 * @file cy_wdt.c
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

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_wdt.h"
#include "cy_scu.h"

/*********************************************************************************************************************
 * API IMPLEMENTATION
  ********************************************************************************************************************/

/* Enables watchdog clock and releases watchdog reset. */
void Cy_WDT_Enable(void)
{

#if defined(CY_IP_CLOCK_GATING)
  Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_WDT);
#endif
#if defined(CY_IP_PERIPHERAL_RESET)
  Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_WDT);
#endif
}

/* Disables watchdog clock and resets watchdog. */
void Cy_WDT_Disable(void)
{
#if defined(CY_IP_PERIPHERAL_RESET)
  Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_WDT);
#endif
#if defined(CY_IP_CLOCK_GATING)
  Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_WDT);
#endif
}
/* Initializes and configures watchdog with configuration data pointed by \a config. */
void Cy_WDT_Init(const Cy_WDT_CONFIG_t *const config)
{
  Cy_WDT_Enable();
  WDT->CTR = config->wdt_ctr;
  WDT->WLB = config->window_lower_bound;
  WDT->WUB = config->window_upper_bound;
}
