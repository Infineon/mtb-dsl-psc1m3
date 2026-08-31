/**
 * @file cy_common.c
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

#include "cy_common.h"

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/
#define CY_DELAY_MAX_CYCLES (0xFFFFFFFFUL - 2UL) /* To avoid an overflow during rounding inside the Cy_DelayCycles */

void Cy_Delay(uint32_t milliseconds)
{
    uint32_t clocksMs = SystemCoreClock / 1000UL; /* SystemCoreClock in kiloHertzs */
    uint32_t maxDelay = CY_DELAY_MAX_CYCLES / clocksMs; /* maximum Cy_DelayCycles delay in milliseconds */

    /* This loop prevents an overflow in value passed to Cy_DelayCycles() API. */
    while(milliseconds > maxDelay)
    {
        Cy_DelayCycles(CY_DELAY_MAX_CYCLES);
        milliseconds -= maxDelay;
    }

    Cy_DelayCycles(milliseconds * clocksMs);
}

void Cy_DelayUs(uint16_t microseconds)
{
    Cy_DelayCycles(microseconds * CY_DIV_ROUNDUP(SystemCoreClock, 1000000UL)); /* microseconds * SystemCoreClock in MHz units */
}
