/**
 * @file cy_common_iar.c
 * @brief Assembly routines for IAR Embedded Workbench IDE.
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


    SECTION .ram_code:CODE:ROOT(4)
    PUBLIC Cy_DelayCycles
    THUMB


/*******************************************************************************
* Function Name: Cy_DelayCycles
****************************************************************************//**
*
* Delays for the specified number of cycles.
*
* @note This function is placed in RAM (.ram_code) to guarantee correct timing
* regardless of flash wait states. PSC1M2/M3 at 48 MHz requires 3 flash
* wait states (NWSFLASH), which causes every taken BNE branch to flush the
* pipeline and re-fetch from flash, inflating the loop cost from 4 to ~9 cycles.
* Executing from RAM (zero wait states) restores the expected 4-cycle loop.
*
* @param uint32_t cycles: The number of cycles to delay.
*
*******************************************************************************/
/* void Cy_DelayCycles(uint32_t cycles) */

Cy_DelayCycles:
    ADDS r0, r0, #2
    LSRS r0, r0, #2
    BEQ Cy_DelayCycles_done
Cy_DelayCycles_loop:
    SUBS r0, r0, #1
    BNE Cy_DelayCycles_loop
    NOP
    NOP
Cy_DelayCycles_done:
    BX lr

/* Subsequent functions remain in flash (.text) */
    SECTION .text:CODE:ROOT(4)
    PUBLIC Cy_EnterCriticalSection
    PUBLIC Cy_ExitCriticalSection
    THUMB

/*******************************************************************************
* Function Name: Cy_EnterCriticalSection
****************************************************************************//**
*
* Cy_EnterCriticalSection disables interrupts and returns a value
* indicating whether interrupts were previously enabled.
*
* Note Implementation of Cy_EnterCriticalSection manipulates the IRQ
* enable bit with interrupts still enabled. The test and set of the interrupt
* bits are not atomic. Therefore, to avoid corrupting processor state, it must
* be the policy that all interrupt routines restore the interrupt enable bits
* as they were found on entry.
*
* \return Returns 0 if interrupts were previously enabled or 1 if interrupts
* were previously disabled.
*
*******************************************************************************/
/* uint8_t Cy_EnterCriticalSection(void) */

Cy_EnterCriticalSection:
    MRS r0, PRIMASK         ; Save and return an interrupt state.
    CPSID I                 ; Disable interrupts.
    BX lr

/*******************************************************************************
* Function Name: Cy_ExitCriticalSection
****************************************************************************//**
*
* Cy_ExitCriticalSection re-enables the interrupts if they were enabled
* before Cy_EnterCriticalSection was called. The argument should be the
* value returned from Cy_EnterCriticalSection.
*
*  \param uint8_t savedIntrStatus:
*   The saved interrupt status returned by the
*   \ref Cy_EnterCriticalSection().
*
*******************************************************************************/
/* void Cy_ExitCriticalSection(uint8_t savedIntrStatus) */

Cy_ExitCriticalSection:
    MSR PRIMASK, r0         ; Restore the interrupt state.
    BX lr

    END
