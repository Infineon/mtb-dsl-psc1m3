; @file cy_common_mdk.s
; @brief Assembly routines for ARMCC.
; @cond
;-------------------------------------------------------------------------------
; \copyright
; (c) 2016-2026, Infineon Technologies AG or an affiliate of
; Infineon Technologies AG.
; SPDX-License-Identifier: Apache-2.0
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
;-------------------------------------------------------------------------------
; @endcond

    AREA |.ram_code|,CODE,ALIGN=3
    THUMB

;-------------------------------------------------------------------------------
; Function Name: Cy_DelayCycles
;-------------------------------------------------------------------------------
;
; Summary:
;  Delays for the specified number of cycles.
;
;  Note: This function is placed in RAM (.ram_code) to guarantee correct timing
;  regardless of flash wait states. PSC1M2/M3 at 48 MHz requires 3 flash
;  wait states (NWSFLASH), which causes every taken BNE branch to flush the
;  pipeline and re-fetch from flash, inflating the loop cost from 4 to ~9 cycles.
;  Executing from RAM (zero wait states) restores the expected 4-cycle loop.
;
; Parameters:
;  uint32_t cycles: The number of cycles to delay.
;
;-------------------------------------------------------------------------------
; void Cy_DelayCycles(uint32_t cycles)
    ALIGN 8
Cy_DelayCycles FUNCTION
    EXPORT Cy_DelayCycles
                             ; cycles bytes
    ADDS r0, r0, #2          ;    1    2    Round to the nearest multiple of 4.
    LSRS r0, r0, #2          ;    1    2    Divide by 4 and set flags.
    BEQ Cy_DelayCycles_done ;    2    2    Skip if 0.
Cy_DelayCycles_loop
    SUBS r0, r0, #1          ;    1    2    Decrement the counter.
    BNE Cy_DelayCycles_loop ;    3(1) 2    3 CPU cycles if branch is taken, and 1 cycle if branch is not taken
    NOP                      ;    1    2    Loop alignment padding.
    NOP                      ;    1    2    Loop alignment padding.
Cy_DelayCycles_done
    BX lr                    ;    3    2
    ENDFUNC


; Subsequent functions remain in flash (.text)
    AREA |.text|,CODE,ALIGN=3
    THUMB
    EXTERN Reset


;-------------------------------------------------------------------------------
; Function Name: Cy_EnterCriticalSection
;-------------------------------------------------------------------------------
;
; Summary:
;  Cy_EnterCriticalSection disables interrupts and returns a value
;  indicating whether interrupts were previously enabled.
;
;  Note Implementation of Cy_EnterCriticalSection manipulates the IRQ
;  enable bit with interrupts still enabled. The test and set of the interrupt
;  bits are not atomic. Therefore, to avoid a corrupting processor state, it must
;  be the policy that all interrupt routines restore the interrupt enable bits as
;  they were found on entry.
;
; Return:
;  uint8_t
;   Returns 0 if interrupts were previously enabled or 1 if interrupts
;   were previously disabled.
;
;-------------------------------------------------------------------------------
; uint8_t Cy_EnterCriticalSection(void)
Cy_EnterCriticalSection FUNCTION
    EXPORT Cy_EnterCriticalSection
    MRS r0, PRIMASK         ; Save and return an interrupt state.
    CPSID I                 ; Disable the interrupts.
    BX lr
    ENDFUNC


;-------------------------------------------------------------------------------
; Function Name: Cy_ExitCriticalSection
;-------------------------------------------------------------------------------
;
; Summary:
;  Cy_ExitCriticalSection re-enables interrupts if they were enabled
;  before Cy_EnterCriticalSection was called. The argument should be the
;  value returned from Cy_EnterCriticalSection.
;
; Parameters:
;  uint8_t savedIntrStatus:
;   The saved interrupt status returned by the Cy_EnterCriticalSection
;   function.
;
;-------------------------------------------------------------------------------
; void Cy_ExitCriticalSection(uint8_t savedIntrStatus)
Cy_ExitCriticalSection FUNCTION
    EXPORT Cy_ExitCriticalSection
    MSR PRIMASK, r0         ; Restore the interrupt state.
    BX lr
    ENDFUNC

    END

; [] END OF FILE
