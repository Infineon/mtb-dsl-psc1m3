/**
 * @file cy_ccu4.c
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
#include "cy_ccu4.h"

#if defined(CCU40)
#include "cy_scu.h"
/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define CY_CCU4_NUM_SLICES_PER_MODULE          (4U)
#define CY_CCU4_SLICE_DITHER_PERIOD_MASK       (1U)
#define CY_CCU4_SLICE_DITHER_DUTYCYCLE_MASK    (2U)
#define CY_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK   (3U)
#define CY_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK  (1U)
#define CY_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK (3U)
#if defined(CY_CCU4V3)
#define CY_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK  CCU4_CC4_INS1_EV0IS_Msk
#else
#define CY_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK  CCU4_CC4_INS_EV0IS_Msk
#endif
#define CY_CCU4_GIDLC_CLOCK_MASK               (15U)
#define CY_CCU4_GCSS_SLICE0_MASK               (1U)
#define CY_CCU4_GCSS_SLICE1_MASK               (16U)
#define CY_CCU4_GCSS_SLICE2_MASK               (256U)
#define CY_CCU4_GCSS_SLICE3_MASK               (4096U)

/** Macro to check if the clock selected enum passed is valid */
#define CY_CCU4_SLICE_CHECK_CLOCK(clock) \
    ((clock == CY_CCU4_CLOCK_SCU)        || \
     (clock == CY_CCU4_CLOCK_EXTERNAL_A) || \
     (clock == CY_CCU4_CLOCK_EXTERNAL_B) || \
     (clock == CY_CCU4_CLOCK_EXTERNAL_C))

/** Macro used to check if the event ID is valid*/
#define CY_CCU4_SLICE_CHECK_EVENT_ID(event_id) \
    ((event_id == CY_CCU4_SLICE_EVENT_NONE)|| \
     (event_id == CY_CCU4_SLICE_EVENT_0)   || \
     (event_id == CY_CCU4_SLICE_EVENT_1)   || \
     (event_id == CY_CCU4_SLICE_EVENT_2))

/** Macro used to check if the edge sensitivity is valid*/
#define CY_CCU4_SLICE_CHECK_EDGE_SENSITIVITY(edge) \
    ((edge == CY_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_NONE)        || \
     (edge == CY_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_RISING_EDGE) || \
     (edge == CY_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_FALLING_EDGE)|| \
     (edge == CY_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_DUAL_EDGE))

/** Macro used to check if the filter clock cycles are valid */
#define CY_CCU4_SLICE_CHECK_EVENT_FILTER(cycles) \
    ((cycles == CY_CCU4_SLICE_EVENT_FILTER_DISABLED) || \
     (cycles == CY_CCU4_SLICE_EVENT_FILTER_3_CYCLES) || \
     (cycles == CY_CCU4_SLICE_EVENT_FILTER_5_CYCLES) || \
     (cycles == CY_CCU4_SLICE_EVENT_FILTER_7_CYCLES))

/** Macro used to check if the Multi-channel input related action is valid*/
#define CY_CCU4_SLICE_CHECK_MCS_ACTION(mcs_action) \
    ((mcs_action == CY_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR)      || \
     (mcs_action == CY_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP) || \
     (mcs_action == CY_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP_DIT))

/** Macro used to check if the SR line is valid*/
#define CY_CCU4_SLICE_CHECK_SR_ID(id) \
    ((id == CY_CCU4_SLICE_SR_ID_0) || \
     (id == CY_CCU4_SLICE_SR_ID_1) || \
     (id == CY_CCU4_SLICE_SR_ID_2) || \
     (id == CY_CCU4_SLICE_SR_ID_3))

/** Macro to check if the end mode enum passed is valid */
#define CY_CCU4_CHECK_END_MODE(end_mode) \
    ((end_mode == CY_CCU4_SLICE_END_MODE_TIMER_STOP) || \
     (end_mode == CY_CCU4_SLICE_END_MODE_TIMER_CLEAR) || \
     (end_mode == CY_CCU4_SLICE_END_MODE_TIMER_STOP_CLEAR))

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/
#if defined(CY_IP_PERIPHERAL_RESET)
__STATIC_INLINE void Cy_CCU4_lAssertReset(const Cy_CCU4_MODULE_t *const module)
{
  if (module == CCU40)
  {
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU40);
  }
#if defined(CCU41)
  else if (module == CCU41)
  {
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU41);
  }
#endif
#if defined(CCU42)
  else if (module == CCU42)
  {
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU42);
  }
#endif
#if defined(CCU43)
  else if (module == CCU43)
  {
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU43);
  }
#endif
  else
  {
    CY_ASSERT(0);
  }
}

__STATIC_INLINE void Cy_CCU4_lDeassertReset(const Cy_CCU4_MODULE_t *const module)
{
  if (module == CCU40)
  {
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU40);
  }
#if defined(CCU41)
  else if (module == CCU41)
  {
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU41);
  }
#endif
#if defined(CCU42)
  else if (module == CCU42)
  {
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU42);
  }
#endif
#if defined(CCU43)
  else if (module == CCU43)
  {
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU43);
  }
#endif
  else
  {
    CY_ASSERT(0);
  }
}
#endif

#if defined(CY_IP_CLOCK_GATING)
__STATIC_INLINE void Cy_CCU4_lGateClock(const Cy_CCU4_MODULE_t *const module)
{

  if (module == CCU40)
  {
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU40);
  }
#if defined(CCU41)
  else if (module == CCU41)
  {
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU41);
  }
#endif
#if defined(CCU42)
  else if (module == CCU42)
  {
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU42);
  }
#endif
#if defined(CCU43)
  else if (module == CCU43)
  {
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU43);
  }
#endif
  else
  {
    CY_ASSERT(0);
  }
}

__STATIC_INLINE void Cy_CCU4_lUngateClock(const Cy_CCU4_MODULE_t *const module)
{
  if (module == CCU40)
  {
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU40);
  }
#if defined(CCU41)
  else if (module == CCU41)
  {
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU41);
  }
#endif
#if defined(CCU42)
  else if (module == CCU42)
  {
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU42);
  }
#endif
#if defined(CCU43)
  else if (module == CCU43)
  {
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU43);
  }
#endif
  else
  {
    CY_ASSERT(0);
  }
}
#endif

#define Cy_CCU4_SLICE_IsInputvalid(input) ((input) < 48U)
/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

void Cy_CCU4_EnableModule(Cy_CCU4_MODULE_t *const module)
{
  CY_ASSERT(Cy_CCU4_IsValidModule(module));

#if defined(CY_IP_CLOCK_GATING)
  Cy_CCU4_lUngateClock(module);
#endif

#if defined(CY_IP_PERIPHERAL_RESET)
  Cy_CCU4_lDeassertReset(module);
#endif
}

void Cy_CCU4_DisableModule(Cy_CCU4_MODULE_t *const module)
{
  CY_ASSERT(Cy_CCU4_IsValidModule(module));

#if defined(CY_IP_PERIPHERAL_RESET)
  Cy_CCU4_lAssertReset(module);
#endif

#if defined(CY_IP_CLOCK_GATING)
  Cy_CCU4_lGateClock(module);
#endif
}

/* API to initialize CCU4 global resources  */
void Cy_CCU4_Init(Cy_CCU4_MODULE_t *const module, const Cy_CCU4_SLICE_MCMS_ACTION_t mcs_action)
{
  uint32_t gctrl;

  CY_ASSERT(Cy_CCU4_IsValidModule(module));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_MCS_ACTION(mcs_action));

  /* Enable CCU4 module */
  Cy_CCU4_EnableModule(module);
  /* Start the prescaler */
  Cy_CCU4_StartPrescaler(module);

  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t) CCU4_GCTRL_MSDE_Msk);
  gctrl |= ((uint32_t) mcs_action) << CCU4_GCTRL_MSDE_Pos;

  module->GCTRL = gctrl;
}

/* API to select CCU4 module clock */
void Cy_CCU4_SetModuleClock(Cy_CCU4_MODULE_t *const module, const Cy_CCU4_CLOCK_t clock)
{
  uint32_t gctrl;

  CY_ASSERT(Cy_CCU4_IsValidModule(module));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_CLOCK(clock));

  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t) CCU4_GCTRL_PCIS_Msk);
  gctrl |= ((uint32_t) clock) << CCU4_GCTRL_PCIS_Pos;

  module->GCTRL = gctrl;
}

/* API to configure the multichannel shadow transfer request via SW and via the CCU4x.MCSS input. */
void Cy_CCU4_SetMultiChannelShadowTransferMode(Cy_CCU4_MODULE_t *const module, const uint32_t slice_mode_msk)
{
  uint32_t gctrl;

  CY_ASSERT(Cy_CCU4_IsValidModule(module));

  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t)slice_mode_msk >> 16U);
  gctrl |= ((uint32_t)slice_mode_msk & 0xFFFFU);
  module->GCTRL = gctrl;
}

/* API to configure CC4 Slice as Timer */
void Cy_CCU4_SLICE_CompareInit(Cy_CCU4_SLICE_t *const slice,
                                const Cy_CCU4_SLICE_COMPARE_CONFIG_t *const compare_init)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT((Cy_CCU4_SLICE_COMPARE_CONFIG_t *) NULL != compare_init);

  /* Program the timer mode */
  slice->TC = compare_init->tc;
  /* Enable the timer concatenation */
  slice->CMC = ((uint32_t) compare_init->timer_concatenation << CCU4_CC4_CMC_TCE_Pos);
  /* Program initial prescaler divider value */
  slice->PSC = (uint32_t) compare_init->prescaler_initval;
  /* Program the dither compare value */
  slice->DITS = (uint32_t) compare_init->dither_limit;
  /* Program timer output passive level */
  slice->PSL = (uint32_t) compare_init->passive_level;
  /* Program floating prescaler compare value */
  slice->FPCS = (uint32_t) compare_init->float_limit;
}

/* API to configure CC4 Slice for Capture */
void Cy_CCU4_SLICE_CaptureInit(Cy_CCU4_SLICE_t *const slice,
                                const Cy_CCU4_SLICE_CAPTURE_CONFIG_t *const capture_init)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT((Cy_CCU4_SLICE_CAPTURE_CONFIG_t *) NULL != capture_init);

  /* Program the capture mode */
  slice->TC = capture_init->tc;
  /* Enable the timer concatenation */
  slice->CMC = ((uint32_t)capture_init->timer_concatenation << CCU4_CC4_CMC_TCE_Pos);
  /* Program initial prescaler divider value */
  slice->PSC = (uint32_t) capture_init->prescaler_initval;
  /* Program initial floating prescaler compare value */
  slice->FPCS = (uint32_t) capture_init->float_limit;
}


/* API to configure the Start trigger function of a slice */
void Cy_CCU4_SLICE_StartConfig(Cy_CCU4_SLICE_t *const slice,
                                const Cy_CCU4_SLICE_EVENT_t event,
                                const Cy_CCU4_SLICE_START_MODE_t start_mode)
{
  uint32_t cmc;
  uint32_t tc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));
  CY_ASSERT(((start_mode == CY_CCU4_SLICE_START_MODE_TIMER_START_CLEAR) || \
              (start_mode == CY_CCU4_SLICE_START_MODE_TIMER_START)));
  /* First, Bind the event with the stop function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_STRTS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_STRTS_Pos;

  slice->CMC = cmc;

  tc  = slice->TC;
  /* Next, Configure the start mode */
  if (start_mode == CY_CCU4_SLICE_START_MODE_TIMER_START_CLEAR)
  {
    tc |= (uint32_t)CCU4_CC4_TC_STRM_Msk;
  }
  else
  {
    tc &= ~((uint32_t)CCU4_CC4_TC_STRM_Msk);
  }

  slice->TC = tc;
}

/* API to configure the Stop trigger function of a slice */
void Cy_CCU4_SLICE_StopConfig(Cy_CCU4_SLICE_t *const slice,
                               const Cy_CCU4_SLICE_EVENT_t event,
                               const Cy_CCU4_SLICE_END_MODE_t end_mode)
{
  uint32_t cmc;
  uint32_t tc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));
  CY_ASSERT(CY_CCU4_CHECK_END_MODE(end_mode));

  /* First, Bind the event with the stop function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_ENDS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_ENDS_Pos;

  slice->CMC = cmc;

  /* Next, Configure the stop mode */
  tc = slice->TC;
  tc &= ~((uint32_t) CCU4_CC4_TC_ENDM_Msk);
  tc |= ((uint32_t) end_mode) << CCU4_CC4_TC_ENDM_Pos;

  slice->TC = tc;
}

/* API to configure the Load trigger function of a slice */
void Cy_CCU4_SLICE_LoadConfig(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));

  /* Bind the event with the load function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_LDS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_LDS_Pos;

  slice->CMC = cmc;
}

/* API to configure the slice modulation function */
void Cy_CCU4_SLICE_ModulationConfig(Cy_CCU4_SLICE_t *const slice,
                                     const Cy_CCU4_SLICE_EVENT_t event,
                                     const Cy_CCU4_SLICE_MODULATION_MODE_t mod_mode,
                                     const bool synch_with_pwm)
{
  uint32_t cmc;
  uint32_t tc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));
  CY_ASSERT(((mod_mode == CY_CCU4_SLICE_MODULATION_MODE_CLEAR_OUT) || \
              (mod_mode == CY_CCU4_SLICE_MODULATION_MODE_CLEAR_ST_OUT)));

  tc  = slice->TC;
  cmc = slice->CMC;

  /* First, Bind the event with the modulation function */
  cmc &= ~((uint32_t) CCU4_CC4_CMC_MOS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_MOS_Pos;
  slice->CMC = cmc;

  /* Next, Modulation mode */
  if (mod_mode == CY_CCU4_SLICE_MODULATION_MODE_CLEAR_OUT)
  {
    tc |= (uint32_t) CCU4_CC4_TC_EMT_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU4_CC4_TC_EMT_Msk);
  }

  /* Synchronization of modulation effect with PWM cycle */
  if (synch_with_pwm == (bool) true)
  {
    tc |= (uint32_t) CCU4_CC4_TC_EMS_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU4_CC4_TC_EMS_Msk);
  }

  slice->TC = tc;
}

/* API to configure the slice count function */
void Cy_CCU4_SLICE_CountConfig(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));

  /* Bind the event with the count function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_CNTS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_CNTS_Pos;

  slice->CMC = cmc;
}

/* API to configure slice gate function */
void Cy_CCU4_SLICE_GateConfig(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));

  /* Bind the event with the gate function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_GATES_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_GATES_Pos;

  slice->CMC = cmc;
}

/* API to configure Capture-0 function */
void Cy_CCU4_SLICE_Capture0Config(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));

  /* Bind the event with the gate function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_CAP0S_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_CAP0S_Pos;

  slice->CMC = cmc;
}

/* API to configure Capture-1 function */
void Cy_CCU4_SLICE_Capture1Config(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));

  /* Bind the event with the gate function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_CAP1S_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_CAP1S_Pos;

  slice->CMC = cmc;
}

/* API to configure direction function */
void Cy_CCU4_SLICE_DirectionConfig(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));

  /* Bind the event with the direction function */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_UDS_Msk);
  cmc |= ((uint32_t) event) << CCU4_CC4_CMC_UDS_Pos;

  slice->CMC = cmc;
}

/* API to configure slice status bit override function */
void Cy_CCU4_SLICE_StatusBitOverrideConfig(Cy_CCU4_SLICE_t *const slice)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));

  /* Bind the event with the override function */
  cmc = slice->CMC;
  /* Map status bit trigger override to Event 1 &
         status bit value override to Event 2 */
  cmc &= ~((uint32_t) CCU4_CC4_CMC_OFS_Msk);
  cmc |= ((uint32_t) 1) << CCU4_CC4_CMC_OFS_Pos;

  slice->CMC = cmc;
}

/* API to configure trap function */
void Cy_CCU4_SLICE_TrapConfig(Cy_CCU4_SLICE_t *const slice,
                               const Cy_CCU4_SLICE_TRAP_EXIT_MODE_t exit_mode,
                               const bool synch_with_pwm)
{
  uint32_t cmc;
  uint32_t tc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(((exit_mode == CY_CCU4_SLICE_TRAP_EXIT_MODE_AUTOMATIC) || \
             (exit_mode == CY_CCU4_SLICE_TRAP_EXIT_MODE_SW)));

  /* First, Map trap function to Event 2 */
  cmc = slice->CMC;
  cmc &= ~((uint32_t) CCU4_CC4_CMC_TS_Msk);
  cmc |= ((uint32_t) 1) << CCU4_CC4_CMC_TS_Pos;
  slice->CMC = cmc;

  /* Next, Configure synchronization option */
  tc = slice->TC;

  if (synch_with_pwm == (bool) true)
  {
    tc |= (uint32_t) CCU4_CC4_TC_TRPSE_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU4_CC4_TC_TRPSE_Msk);
  }

  /* Configure exit mode */
  if (exit_mode == CY_CCU4_SLICE_TRAP_EXIT_MODE_SW)
  {
    tc |= (uint32_t) CCU4_CC4_TC_TRPSW_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU4_CC4_TC_TRPSW_Msk);
  }

  slice->TC = tc;
}

/* API to configure a slice Status Bit Override event */
void Cy_CCU4_SLICE_ConfigureStatusBitOverrideEvent(Cy_CCU4_SLICE_t *const slice,
    const Cy_CCU4_SLICE_EVENT_CONFIG_t *const ev1_config,
    const Cy_CCU4_SLICE_EVENT_CONFIG_t *const ev2_config)
{
  uint32_t ins;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(Cy_CCU4_SLICE_IsInputvalid(ev1_config->mapped_input));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EDGE_SENSITIVITY(ev1_config->edge));
  CY_ASSERT(((ev1_config->level == CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) || \
              (ev1_config->level == CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_FILTER(ev1_config->duration));
  CY_ASSERT(Cy_CCU4_SLICE_IsInputvalid(ev2_config->mapped_input));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EDGE_SENSITIVITY(ev2_config->edge));
  CY_ASSERT(((ev2_config->level == CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) || \
              (ev2_config->level == CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_FILTER(ev2_config->duration));
#if defined(CY_CCU4V3)
  ins = slice->INS2;

  /* Configure the edge sensitivity for event 1 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU4_CC4_INS2_EV1EM_Pos);
  ins |= ((uint32_t) ev1_config->edge) << CCU4_CC4_INS2_EV1EM_Pos;

  /* Configure the edge sensitivity for event 2 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU4_CC4_INS2_EV2EM_Pos);
  ins |= ((uint32_t) ev2_config->edge) << CCU4_CC4_INS2_EV2EM_Pos;

  /* Configure the level sensitivity for event 1 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU4_CC4_INS2_EV1LM_Pos);
  ins |= ((uint32_t) ev1_config->level) << CCU4_CC4_INS2_EV1LM_Pos;

  /* Configure the level sensitivity for event 2 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU4_CC4_INS2_EV2LM_Pos);
  ins |= ((uint32_t) ev2_config->level) << CCU4_CC4_INS2_EV2LM_Pos;

  /* Configure the debounce filter for event 1 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU4_CC4_INS2_LPF1M_Pos);
  ins |= ((uint32_t) ev1_config->duration) << CCU4_CC4_INS2_LPF1M_Pos;

  /* Configure the debounce filter for event 2 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU4_CC4_INS2_LPF2M_Pos);
  ins |= ((uint32_t) ev2_config->duration) << CCU4_CC4_INS2_LPF2M_Pos;

  slice->INS2 = ins;

  ins = slice->INS1;

  /* Next, the input for Event1 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU4_CC4_INS1_EV1IS_Pos);
  ins |= ((uint32_t) ev1_config->mapped_input) << CCU4_CC4_INS1_EV1IS_Pos;

  /* Finally, the input for Event2 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU4_CC4_INS1_EV2IS_Pos);
  ins |= ((uint32_t) ev2_config->mapped_input) << CCU4_CC4_INS1_EV2IS_Pos;

  slice->INS1 = ins;
#else
  ins = slice->INS;

  /* Configure the edge sensitivity for event 1 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU4_CC4_INS_EV1EM_Pos);
  ins |= ((uint32_t) ev1_config->edge) << CCU4_CC4_INS_EV1EM_Pos;

  /* Configure the edge sensitivity for event 2 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU4_CC4_INS_EV2EM_Pos);
  ins |= ((uint32_t) ev2_config->edge) << CCU4_CC4_INS_EV2EM_Pos;

  /* Configure the level sensitivity for event 1 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU4_CC4_INS_EV1LM_Pos);
  ins |= ((uint32_t) ev1_config->level) << CCU4_CC4_INS_EV1LM_Pos;

  /* Configure the level sensitivity for event 2 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU4_CC4_INS_EV2LM_Pos);
  ins |= ((uint32_t) ev2_config->level) << CCU4_CC4_INS_EV2LM_Pos;

  /* Configure the debounce filter for event 1 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU4_CC4_INS_LPF1M_Pos);
  ins |= ((uint32_t) ev1_config->duration) << CCU4_CC4_INS_LPF1M_Pos;

  /* Configure the debounce filter for event 2 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU4_CC4_INS_LPF2M_Pos);
  ins |= ((uint32_t) ev2_config->duration) << CCU4_CC4_INS_LPF2M_Pos;

  /* Next, the input for Event1 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU4_CC4_INS_EV1IS_Pos);
  ins |= ((uint32_t) ev1_config->mapped_input) << CCU4_CC4_INS_EV1IS_Pos;

  /* Finally, the input for Event2 */
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU4_CC4_INS_EV2IS_Pos);
  ins |= ((uint32_t) ev2_config->mapped_input) << CCU4_CC4_INS_EV2IS_Pos;

  slice->INS = ins;
#endif
}

/* API to configure a slice trigger event */
void Cy_CCU4_SLICE_ConfigureEvent(Cy_CCU4_SLICE_t *const slice,
                                   const Cy_CCU4_SLICE_EVENT_t event,
                                   const Cy_CCU4_SLICE_EVENT_CONFIG_t *const config)
{
  uint32_t ins;
  uint8_t  pos;
  uint8_t  offset;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));
  CY_ASSERT(Cy_CCU4_SLICE_IsInputvalid(config->mapped_input));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EDGE_SENSITIVITY(config->edge));
  CY_ASSERT(((config->level == CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) || \
              (config->level == CY_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_FILTER(config->duration));
  /* Calculate offset with reference to event */
  offset = ((uint8_t) event) - 1U;

#if defined(CY_CCU4V3)
  ins = slice->INS2;

  /* First, configure the edge sensitivity */
  pos = ((uint8_t) CCU4_CC4_INS2_EV0EM_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->edge) << pos;

  /* Next, the level */
  pos = ((uint8_t) CCU4_CC4_INS2_EV0LM_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->level) << pos;

  /* Next, the debounce filter */
  pos = ((uint8_t) CCU4_CC4_INS2_LPF0M_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->duration) << pos;

  slice->INS2 = ins;

  ins = slice->INS1;

  /* Finally the input */
  pos = ((uint8_t) CCU4_CC4_INS1_EV0IS_Pos) + (uint8_t)(offset << 3U);
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->mapped_input) << pos;

  slice->INS1 = ins;

#else
  ins = slice->INS;

  /* First, configure the edge sensitivity */
  pos = ((uint8_t) CCU4_CC4_INS_EV0EM_Pos) + (uint8_t)(offset << 1U);
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_EDGE_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->edge) << pos;

  /* Next, the level */
  pos = ((uint8_t) CCU4_CC4_INS_EV0LM_Pos) + offset;
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_LEVEL_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->level) << pos;

  /* Next, the debounce filter */
  pos = ((uint8_t) CCU4_CC4_INS_LPF0M_Pos) + (uint8_t)(offset << 1U);
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_FILTER_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->duration) << pos;

  /* Finally the input */
  pos = ((uint8_t) CCU4_CC4_INS_EV0IS_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->mapped_input) << pos;

  slice->INS = ins;
#endif
}

/* API to bind an input to a slice trigger event */
void Cy_CCU4_SLICE_SetInput(Cy_CCU4_SLICE_t *const slice,
                             const Cy_CCU4_SLICE_EVENT_t event,
                             const Cy_CCU4_SLICE_INPUT_t input)
{
  uint32_t ins;
  uint8_t  pos;
  uint8_t  offset;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_EVENT_ID(event));
  CY_ASSERT(Cy_CCU4_SLICE_IsInputvalid(input));

  /* Calculate offset with reference to event */
  offset = ((uint8_t) event) - 1U;

#if defined(CY_CCU4V3)
  pos = ((uint8_t) CCU4_CC4_INS1_EV0IS_Pos) + (uint8_t) (offset << 3U);

  ins = slice->INS1;
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) input) << pos;

  slice->INS1 = ins;
#else
  pos = ((uint8_t) CCU4_CC4_INS_EV0IS_Pos) + (uint8_t) (offset << 2U);

  ins = slice->INS;
  ins &= ~(((uint32_t) CY_CCU4_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) input) << pos;

  slice->INS = ins;
#endif
}

/* API to program timer repeat mode - Single shot vs repeat */
void Cy_CCU4_SLICE_SetTimerRepeatMode(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_TIMER_REPEAT_MODE_t mode)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(((mode == CY_CCU4_SLICE_TIMER_REPEAT_MODE_REPEAT) || \
              (mode == CY_CCU4_SLICE_TIMER_REPEAT_MODE_SINGLE)));

  if (CY_CCU4_SLICE_TIMER_REPEAT_MODE_REPEAT == mode)
  {
    slice->TC &= ~((uint32_t) CCU4_CC4_TC_TSSM_Msk);
  }
  else
  {
    slice->TC |= (uint32_t) CCU4_CC4_TC_TSSM_Msk;
  }
}

/* Programs timer counting mode */
void Cy_CCU4_SLICE_SetTimerCountingMode(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_TIMER_COUNT_MODE_t mode)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(((mode == CY_CCU4_SLICE_TIMER_COUNT_MODE_EA) || \
             (mode == CY_CCU4_SLICE_TIMER_COUNT_MODE_CA)));

  if (CY_CCU4_SLICE_TIMER_COUNT_MODE_EA == mode)
  {
    slice->TC &= ~((uint32_t) CCU4_CC4_TC_TCM_Msk);
  }
  else
  {
    slice->TC |= (uint32_t) CCU4_CC4_TC_TCM_Msk;
  }
}

/* Retrieves desired capture register value */
uint32_t Cy_CCU4_SLICE_GetCaptureRegisterValue(const Cy_CCU4_SLICE_t *const slice, const uint8_t reg_num)
{
  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT((reg_num < 4U));
  return (slice->CV[reg_num]);
}

/* @brief Retrieves the latest captured timer value */
Cy_CCU4_STATUS_t Cy_CCU4_SLICE_GetLastCapturedTimerValue(const Cy_CCU4_SLICE_t *const slice,
    const Cy_CCU4_SLICE_CAP_REG_SET_t set,
    uint32_t *val_ptr)
{
  Cy_CCU4_STATUS_t retval;
  uint8_t i;
  uint8_t start;
  uint8_t end;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(((set == CY_CCU4_SLICE_CAP_REG_SET_LOW) || \
             (set == CY_CCU4_SLICE_CAP_REG_SET_HIGH)));

  retval = CY_CCU4_STATUS_ERROR;

  /* First check if extended capture mode is enabled */
  if ((slice->TC) & CCU4_CC4_TC_ECM_Msk)
  {
    /* Extended capture mode has been enabled. So start with the lowest capture register and work your way up */
    start = 0U;
    end = CY_CCU4_NUM_SLICES_PER_MODULE;
  }
  else
  {
    /* Extended capture mode is not enabled */
    if (set == CY_CCU4_SLICE_CAP_REG_SET_HIGH)
    {
      start = ((uint8_t) CY_CCU4_NUM_SLICES_PER_MODULE) >> 1U;
      end   = (uint8_t) CY_CCU4_NUM_SLICES_PER_MODULE;
    }
    else
    {
      start = 0U;
      end = ((uint8_t) CY_CCU4_NUM_SLICES_PER_MODULE) >> 1U;
    }
  }

  for (i = start; i < end; i++)
  {
    if ( (slice->CV[i]) & CCU4_CC4_CV_FFL_Msk )
    {
      *val_ptr = slice->CV[i];
      retval = CY_CCU4_STATUS_OK;
      break;
    }
  }

  return retval;
}

/* Retrieves timer capture value from a FIFO made of capture registers */
uint32_t Cy_CCU4_SLICE_GetCapturedValueFromFifo(const Cy_CCU4_SLICE_t *const slice,
    const Cy_CCU4_SLICE_CAP_REG_SET_t set)
{
  uint32_t cap;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(((set == CY_CCU4_SLICE_CAP_REG_SET_LOW) || \
              (set == CY_CCU4_SLICE_CAP_REG_SET_HIGH)));

  if (CY_CCU4_SLICE_CAP_REG_SET_LOW == set)
  {
    cap = slice->ECRD0;
  }
  else
  {
    cap = slice->ECRD1;
  }

  return cap;
}

/* Enables PWM dithering feature */
void Cy_CCU4_SLICE_EnableDithering(Cy_CCU4_SLICE_t *const slice,
                                    const bool period_dither,
                                    const bool duty_dither,
                                    const uint8_t spread)
{
  uint32_t tc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));

  tc = slice->TC;
  tc &= ~((uint32_t) CCU4_CC4_TC_DITHE_Msk);

  if ((bool) true == period_dither)
  {
    tc |= (((uint32_t) CY_CCU4_SLICE_DITHER_PERIOD_MASK) << CCU4_CC4_TC_DITHE_Pos);
  }
  if ((bool) true == duty_dither)
  {
    tc |= (((uint32_t) CY_CCU4_SLICE_DITHER_DUTYCYCLE_MASK) << CCU4_CC4_TC_DITHE_Pos);
  }

  slice->TC = tc;

  Cy_CCU4_SLICE_SetDitherCompareValue((Cy_CCU4_SLICE_t *)slice, (uint8_t)spread);
}

/* Programs Pre-scalar divider  */
void Cy_CCU4_SLICE_SetPrescaler(Cy_CCU4_SLICE_t *const slice, const Cy_CCU4_SLICE_PRESCALER_t div_val)
{
  uint32_t fpc;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));

  fpc = slice->FPC;
  fpc &= ~((uint32_t) CCU4_CC4_FPC_PVAL_Msk);
  fpc |= ((uint32_t) div_val) << CCU4_CC4_FPC_PVAL_Pos;
  slice->FPC = fpc;
  /*
   * In any case, update the initial value of the divider which is to be loaded once the prescaler increments to the
   * compare value.
   */
  slice->PSC = (uint32_t) div_val;
}

/* Binds a capcom event to an NVIC node  */
void Cy_CCU4_SLICE_SetInterruptNode(Cy_CCU4_SLICE_t *const slice,
                                     const Cy_CCU4_SLICE_IRQ_ID_t event,
                                     const Cy_CCU4_SLICE_SR_ID_t sr)
{
  uint32_t srs;
  int32_t pos;
  uint32_t mask;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_SR_ID(sr));
  CY_ASSERT(CY_CCU4_SLICE_CHECK_INTERRUPT(event));

  switch (event)
  {
    case CY_CCU4_SLICE_IRQ_ID_PERIOD_MATCH:
    case CY_CCU4_SLICE_IRQ_ID_ONE_MATCH:
      mask = ((uint32_t) CCU4_CC4_SRS_POSR_Msk);
      pos  = CCU4_CC4_SRS_POSR_Pos;
      break;

    case CY_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP:
    case CY_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_DOWN:
      mask = ((uint32_t) CCU4_CC4_SRS_CMSR_Msk);
      pos  = CCU4_CC4_SRS_CMSR_Pos;
      break;

    case CY_CCU4_SLICE_IRQ_ID_EVENT0:
      mask = ((uint32_t) CCU4_CC4_SRS_E0SR_Msk);
      pos  = CCU4_CC4_SRS_E0SR_Pos;
      break;

    case CY_CCU4_SLICE_IRQ_ID_EVENT1:
      mask = ((uint32_t) CCU4_CC4_SRS_E1SR_Msk);
      pos  = CCU4_CC4_SRS_E1SR_Pos;
      break;

    case CY_CCU4_SLICE_IRQ_ID_EVENT2:
    case CY_CCU4_SLICE_IRQ_ID_TRAP:
      mask = ((uint32_t) CCU4_CC4_SRS_E2SR_Msk);
      pos  = CCU4_CC4_SRS_E2SR_Pos;
      break;

	default:
	  mask = 0;
	  pos = 0;
	  break;
  }

  if (mask != 0)
  {
    srs = slice->SRS;
    srs &= ~mask;
    srs |= (uint32_t)sr << pos;
    slice->SRS = srs;
  }
}

/* Asserts passive level for the slice output */
void Cy_CCU4_SLICE_SetPassiveLevel(Cy_CCU4_SLICE_t *const slice,
                                    const Cy_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_t level)
{
  uint32_t psl;

  CY_ASSERT(Cy_CCU4_IsValidSlice(slice));
  CY_ASSERT(((level == CY_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW) || \
             (level == CY_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH)));

  psl = slice->PSL;
  psl &= ~((uint32_t) CCU4_CC4_PSL_PSL_Msk);
  psl |= (uint32_t) level;

  /* Program CC4 slice output passive level */
  slice->PSL = psl;
}

#endif /* CCU40 */
