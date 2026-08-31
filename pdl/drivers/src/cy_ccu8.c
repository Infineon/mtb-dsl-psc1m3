/**
 * @file cy_ccu8.c
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
#include "cy_ccu8.h"

#if defined(CCU80)
#include "cy_scu.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define CY_CCU8_NUM_SLICES_PER_MODULE          (4U)
#define CY_CCU8_SLICE_DITHER_PERIOD_MASK       (1U)
#define CY_CCU8_SLICE_DITHER_DUTYCYCLE_MASK    (2U)
#define CY_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK   (3U)
#define CY_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK  (1U)
#define CY_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK (3U)
#if defined(CY_CCU8V3)
#define CY_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK  CCU8_CC8_INS1_EV0IS_Msk
#else
#define CY_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK  CCU8_CC8_INS_EV0IS_Msk
#endif
#define CY_CCU8_GIDLC_CLOCK_MASK               (15U)
#define CY_CCU8_GCSS_SLICE0_MASK               (1U)
#define CY_CCU8_GCSS_SLICE1_MASK               (16U)
#define CY_CCU8_GCSS_SLICE2_MASK               (256U)
#define CY_CCU8_GCSS_SLICE3_MASK               (4096U)
#define CY_CCU8_SLICE_DEAD_TIME_CONFIG_MASK    (63U)

#define CY_CCU8_SLICE_CHECK_DTC_DIV(div) \
    ((div == CY_CCU8_SLICE_DTC_DIV_1) || \
     (div == CY_CCU8_SLICE_DTC_DIV_2) || \
     (div == CY_CCU8_SLICE_DTC_DIV_4) || \
     (div == CY_CCU8_SLICE_DTC_DIV_8))

#define CY_CCU8_SLICE_CHECK_CLOCK(clock) \
    ((clock == CY_CCU8_CLOCK_SCU)        || \
     (clock == CY_CCU8_CLOCK_EXTERNAL_A) || \
     (clock == CY_CCU8_CLOCK_EXTERNAL_B) || \
     (clock == CY_CCU8_CLOCK_EXTERNAL_C))

#define CY_CCU8_SLICE_CHECK_OUTPUT(out) \
    ((out == CY_CCU8_SLICE_OUTPUT_0) || \
     (out == CY_CCU8_SLICE_OUTPUT_1) || \
     (out == CY_CCU8_SLICE_OUTPUT_2) || \
     (out == CY_CCU8_SLICE_OUTPUT_3))

#define CY_CCU8_SLICE_CHECK_END_MODE(end_mode) \
    ((end_mode == CY_CCU8_SLICE_END_MODE_TIMER_STOP) || \
     (end_mode == CY_CCU8_SLICE_END_MODE_TIMER_CLEAR) || \
     (end_mode == CY_CCU8_SLICE_END_MODE_TIMER_STOP_CLEAR))

#define CY_CCU8_SLICE_CHECK_EVENT_ID(event_id) \
    ((event_id == CY_CCU8_SLICE_EVENT_NONE)|| \
     (event_id == CY_CCU8_SLICE_EVENT_0)   || \
     (event_id == CY_CCU8_SLICE_EVENT_1)   || \
     (event_id == CY_CCU8_SLICE_EVENT_2))

#define CY_CCU8_SLICE_CHECK_EDGE_SENSITIVITY(edge) \
    ((edge == CY_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_NONE)        || \
     (edge == CY_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_RISING_EDGE) || \
     (edge == CY_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_FALLING_EDGE)|| \
     (edge == CY_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_DUAL_EDGE))

#define CY_CCU8_SLICE_CHECK_EVENT_FILTER(cycles) \
    ((cycles == CY_CCU8_SLICE_EVENT_FILTER_DISABLED) || \
     (cycles == CY_CCU8_SLICE_EVENT_FILTER_3_CYCLES) || \
     (cycles == CY_CCU8_SLICE_EVENT_FILTER_5_CYCLES) || \
     (cycles == CY_CCU8_SLICE_EVENT_FILTER_7_CYCLES))

#define CY_CCU8_SLICE_CHECK_CAP_TIMER_CLEAR_MODE(mode) \
    ((mode == CY_CCU8_SLICE_TIMER_CLEAR_MODE_NEVER)   || \
     (mode == CY_CCU8_SLICE_TIMER_CLEAR_MODE_CAP_HIGH)|| \
     (mode == CY_CCU8_SLICE_TIMER_CLEAR_MODE_CAP_LOW) || \
     (mode == CY_CCU8_SLICE_TIMER_CLEAR_MODE_ALWAYS))

#define CY_CCU8_SLICE_CHECK_MCS_ACTION(mcs_action) \
    ((mcs_action == CY_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR)      || \
     (mcs_action == CY_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP) || \
     (mcs_action == CY_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP_DIT))

#define CY_CCU8_SLICE_CHECK_SR_ID(id) \
    ((id == CY_CCU8_SLICE_SR_ID_0) || \
     (id == CY_CCU8_SLICE_SR_ID_1) || \
     (id == CY_CCU8_SLICE_SR_ID_2) || \
     (id == CY_CCU8_SLICE_SR_ID_3))

#define CY_CCU8_SLICE_CHECK_MODULATION_CHANNEL(channel) \
    ((channel == CY_CCU8_SLICE_MODULATION_CHANNEL_NONE) || \
     (channel == CY_CCU8_SLICE_MODULATION_CHANNEL_1)    || \
     (channel == CY_CCU8_SLICE_MODULATION_CHANNEL_2)    || \
     (channel == CY_CCU8_SLICE_MODULATION_CHANNEL_1_AND_2))

#define CY_CCU8_SLICE_CHECK_SLICE_STATUS(channel) \
    ((channel == CY_CCU8_SLICE_STATUS_CHANNEL_1)       || \
     (channel == CY_CCU8_SLICE_STATUS_CHANNEL_2)       || \
     (channel == CY_CCU8_SLICE_STATUS_CHANNEL_1_AND_2) || \
     (channel == CY_CCU8_SLICE_STATUS_CHANNEL_1_OR_2))

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/
#if defined(CY_IP_PERIPHERAL_RESET)
__STATIC_INLINE void Cy_CCU8_lAssertReset(const Cy_CCU8_MODULE_t *const module)
{
  if (module == CCU80)
  {
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU80);
  }
#if defined(CCU81)
  else if (module == CCU81)
  {
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU81);
  }
#endif
  else
  {
    CY_ASSERT(0);
  }
}

__STATIC_INLINE void Cy_CCU8_lDeassertReset(const Cy_CCU8_MODULE_t *const module)
{
  if (module == CCU80)
  {
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU80);
  }
#if defined(CCU81)
  else if (module == CCU81)
  {
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_CCU81);
  }
#endif
  else
  {
    CY_ASSERT(0);
  }
}
#endif

#if defined(CY_IP_CLOCK_GATING)
__STATIC_INLINE void Cy_CCU8_lGateClock(Cy_CCU8_MODULE_t *const module)
{
  if (module == CCU80)
  {
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU80);
  }
#if defined(CCU81)
  else if (module == CCU81)
  {
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU81);
  }
#endif
  else
  {
    CY_ASSERT(0);
  }
}

__STATIC_INLINE void Cy_CCU8_lUngateClock(Cy_CCU8_MODULE_t *const module)
{
  if (module == CCU80)
  {
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU80);
  }
#if defined(CCU81)
  else if (module == CCU81)
  {
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_CCU81);
  }
#endif
  else
  {
    CY_ASSERT(0);
  }
}
#endif

#define Cy_CCU8_SLICE_IsInputvalid(input) ((input) < 48U)
/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* API to set the CCU8 module as active and enable the clock  */
void Cy_CCU8_EnableModule(Cy_CCU8_MODULE_t *const module)
{
  CY_ASSERT(Cy_CCU8_IsValidModule(module));

#if defined(CY_IP_CLOCK_GATING)
  Cy_CCU8_lUngateClock(module);
#endif

#if defined(CY_IP_PERIPHERAL_RESET)
  Cy_CCU8_lDeassertReset(module);
#endif
}

/* API to set the CCU8 module as idle and disable the clock  */
void Cy_CCU8_DisableModule(Cy_CCU8_MODULE_t *const module)
{
  CY_ASSERT(Cy_CCU8_IsValidModule(module));

#if defined(CY_IP_PERIPHERAL_RESET)
  Cy_CCU8_lAssertReset(module);
#endif

#if defined(CY_IP_CLOCK_GATING)
  Cy_CCU8_lGateClock(module);
#endif
}

/* API to initialize CCU8 global resources  */
void Cy_CCU8_Init(Cy_CCU8_MODULE_t *const module, const Cy_CCU8_SLICE_MCMS_ACTION_t mcs_action)
{
  uint32_t gctrl;

  CY_ASSERT(Cy_CCU8_IsValidModule(module));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_MCS_ACTION(mcs_action));

  /* Enable CCU8 module */
  Cy_CCU8_EnableModule(module);
  /* Start the prescaler */
  Cy_CCU8_StartPrescaler(module);

  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t) CCU8_GCTRL_MSDE_Msk);
  gctrl |= (uint32_t)mcs_action << CCU8_GCTRL_MSDE_Pos;

  module->GCTRL = gctrl;
}

/* API to select CCU8 module clock */
void Cy_CCU8_SetModuleClock(Cy_CCU8_MODULE_t *const module, const Cy_CCU8_CLOCK_t clock)
{
  uint32_t gctrl;

  CY_ASSERT(Cy_CCU8_IsValidModule(module));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_CLOCK(clock));

  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t) CCU8_GCTRL_PCIS_Msk);
  gctrl |= ((uint32_t) clock) << CCU8_GCTRL_PCIS_Pos;

  module->GCTRL = gctrl;
}

/* API to configure CC8 Slice in Compare mode */
void Cy_CCU8_SLICE_CompareInit(Cy_CCU8_SLICE_t *const slice,
                                const Cy_CCU8_SLICE_COMPARE_CONFIG_t *const compare_init)
{
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT((Cy_CCU8_SLICE_COMPARE_CONFIG_t *) NULL != compare_init);
  /* Stops the timer */
  Cy_CCU8_SLICE_StopTimer(slice);
  /* Program the timer mode */
  slice->TC = compare_init->tc;
  /* Enable the timer concatenation */
  slice->CMC =  (uint32_t)compare_init->timer_concatenation << CCU8_CC8_CMC_TCE_Pos;
  /* Program initial prescaler divider value */
  slice->PSC = (uint32_t) compare_init->prescaler_initval;
  /* Program floating prescaler compare value */
  slice->FPCS = (uint32_t) compare_init->float_limit;
  /* Program the dither compare value */
  slice->DITS = (uint32_t) compare_init->dither_limit;
  /* Program timer output passive level */
  slice->PSL = (uint32_t) compare_init->psl;
  /* Asymmetric PWM and Slice output routing configuration */
#if defined(CY_CCU8V3)
  slice->CHC = (uint32_t) compare_init->chc;
#else
  slice->CHC = (uint32_t)((uint32_t)compare_init->chc ^ CY_CCU8_SLICE_CHC_CONFIG_MASK);
#endif
}

/* API to configure CC8 Slice in Capture mode */
void Cy_CCU8_SLICE_CaptureInit(Cy_CCU8_SLICE_t *const slice,
                                const Cy_CCU8_SLICE_CAPTURE_CONFIG_t *const capture_init)
{
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT((Cy_CCU8_SLICE_CAPTURE_CONFIG_t *) NULL != capture_init);
  /* Stops the timer */
  Cy_CCU8_SLICE_StopTimer(slice);
  /* Capture mode configuration */
  slice->TC = capture_init->tc;
  /* Enable the timer concatenation */
  slice->CMC = (uint32_t)capture_init->timer_concatenation << CCU8_CC8_CMC_TCE_Pos;
  /* Program floating prescaler compare value */
  slice->FPCS = (uint32_t) capture_init->float_limit;
  /* Program initial prescaler divider value */
  slice->PSC = (uint32_t) capture_init->prescaler_initval;
}

/* API to configure the each output of the slice with either STx or inverted STx. */
void Cy_CCU8_SLICE_SetOutPath(Cy_CCU8_SLICE_t *const slice, const uint32_t out_path_msk)
{
  uint32_t chc;
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  chc = slice->CHC;
  chc &= ~((uint32_t)((uint32_t)(out_path_msk & 0xCCCC0U) >> 2U));
  chc |= ((uint32_t)out_path_msk & 0x33330U);
  slice->CHC = chc;
}

/* API to configure the multichannel shadow transfer request via SW and via the CCU8x.MCSS input. */
void Cy_CCU8_SetMultiChannelShadowTransferMode(Cy_CCU8_MODULE_t *const module, const uint32_t slice_mode_msk)
{
  uint32_t gctrl;

  CY_ASSERT(Cy_CCU8_IsValidModule(module));

  gctrl = module->GCTRL;
  gctrl &= ~((uint32_t)slice_mode_msk >> 16U);
  gctrl |= ((uint32_t)slice_mode_msk & 0xFFFFU);
  module->GCTRL = gctrl;
}


/* API to configure the Start trigger function of a slice*/
void Cy_CCU8_SLICE_StartConfig(Cy_CCU8_SLICE_t *const slice,
                                const Cy_CCU8_SLICE_EVENT_t event,
                                const Cy_CCU8_SLICE_START_MODE_t start_mode)
{
  uint32_t cmc;
  uint32_t tc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));
  CY_ASSERT(((start_mode == CY_CCU8_SLICE_START_MODE_TIMER_START) || \
              (start_mode == CY_CCU8_SLICE_START_MODE_TIMER_START_CLEAR)));
  cmc = slice->CMC;

  cmc &= ~((uint32_t) CCU8_CC8_CMC_STRTS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_STRTS_Pos;

  slice->CMC = cmc;

  tc  = slice->TC;

  if (start_mode == CY_CCU8_SLICE_START_MODE_TIMER_START_CLEAR)
  {
    tc |= (uint32_t) CCU8_CC8_TC_STRM_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_STRM_Msk);
  }

  slice->TC = tc;
}

/* API to configure the Stop trigger function of a slice */
void Cy_CCU8_SLICE_StopConfig(Cy_CCU8_SLICE_t *const slice,
                               const Cy_CCU8_SLICE_EVENT_t event,
                               const Cy_CCU8_SLICE_END_MODE_t end_mode)
{
  uint32_t cmc;
  uint32_t tc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_END_MODE(end_mode));

  cmc = slice->CMC;
  /* First, Bind the event with the stop function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_ENDS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_ENDS_Pos;

  slice->CMC = cmc;

  /* Configure the stop mode */
  tc = slice->TC;
  tc &= ~((uint32_t) CCU8_CC8_TC_ENDM_Msk);
  tc |= ((uint32_t) end_mode) << CCU8_CC8_TC_ENDM_Pos;

  slice->TC = tc;
}

/* API to configure the Load trigger function of a slice*/
void Cy_CCU8_SLICE_LoadConfig(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));

  cmc = slice->CMC;

  /* First, Bind the event with the load function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_LDS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_LDS_Pos;

  slice->CMC = cmc;
}

/* API to configure, which compare register value has to be loaded during external load event */
void Cy_CCU8_SLICE_LoadSelector(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_COMPARE_CHANNEL_t ch_num)
{
  uint32_t tc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_COMP_CHANNEL(ch_num));

  tc = slice->TC;

  /* First, Bind the event with the load function */
  tc &= ~((uint32_t) CCU8_CC8_TC_TLS_Msk);
  tc |= (uint32_t)ch_num << CCU8_CC8_TC_TLS_Pos;

  slice->TC = tc;
}

/* API to configure the slice modulation function */
void Cy_CCU8_SLICE_ModulationConfig(Cy_CCU8_SLICE_t *const slice,
                                     const Cy_CCU8_SLICE_EVENT_t event,
                                     const Cy_CCU8_SLICE_MODULATION_MODE_t mod_mode,
                                     const Cy_CCU8_SLICE_MODULATION_CHANNEL_t channel,
                                     const bool synch_with_pwm)
{
  uint32_t cmc;
  uint32_t tc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_MODULATION_CHANNEL(channel));
  CY_ASSERT(((mod_mode == CY_CCU8_SLICE_MODULATION_MODE_CLEAR_ST_OUT) || \
              (mod_mode == CY_CCU8_SLICE_MODULATION_MODE_CLEAR_OUT)));

  cmc = slice->CMC;

  /* First, Bind the event with the modulation function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_MOS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_MOS_Pos;

  slice->CMC = cmc;

  tc  = slice->TC;

  /* Next, Modulation mode */
  if (mod_mode == CY_CCU8_SLICE_MODULATION_MODE_CLEAR_OUT)
  {
    tc |= (uint32_t) CCU8_CC8_TC_EMT_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_EMT_Msk);
  }

  /* Synchronization of modulation effect with PWM cycle */
  if (synch_with_pwm == true)
  {
    tc |= (uint32_t) CCU8_CC8_TC_EMS_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_EMS_Msk);
  }

  /* Configure on which channel external modulation to be applied */
  tc &= ~((uint32_t) CCU8_CC8_TC_EME_Msk);
  tc |= (uint32_t)channel << CCU8_CC8_TC_EME_Pos;

  slice->TC = tc;
}

/* API to configure the slice count function */
void Cy_CCU8_SLICE_CountConfig(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));

  cmc = slice->CMC;

  /* First, Bind the event with the count function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_CNTS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_CNTS_Pos;

  slice->CMC = cmc;
}

/* API to configure slice gate function */
void Cy_CCU8_SLICE_GateConfig(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));

  cmc = slice->CMC;

  /* First, Bind the event with the gate function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_GATES_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_GATES_Pos;

  slice->CMC = cmc;
}

/* API to configure Capture-0 function */
void Cy_CCU8_SLICE_Capture0Config(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));

  cmc = slice->CMC;

  /* First, Bind the event with the gate function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_CAP0S_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_CAP0S_Pos;

  slice->CMC = cmc;
}

/* API to configure Capture-1 function */
void Cy_CCU8_SLICE_Capture1Config(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));


  cmc = slice->CMC;

  /* First, Bind the event with the gate function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_CAP1S_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_CAP1S_Pos;

  slice->CMC = cmc;
}

/* API to configure direction function */
void Cy_CCU8_SLICE_DirectionConfig(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_EVENT_t event)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));

  cmc = slice->CMC;

  /* First, Bind the event with the direction function */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_UDS_Msk);
  cmc |= ((uint32_t) event) << CCU8_CC8_CMC_UDS_Pos;

  slice->CMC = cmc;
}

/* API to configure slice status bit override function */
void Cy_CCU8_SLICE_StatusBitOverrideConfig(Cy_CCU8_SLICE_t *const slice)
{
  uint32_t cmc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));

  cmc = slice->CMC;

  /* Map status bit trigger override to Event 1 &
         status bit value override to Event 2 */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_OFS_Msk);
  cmc |= ((uint32_t) 1) << CCU8_CC8_CMC_OFS_Pos;

  slice->CMC = cmc;
}

/* API to configure trap function*/
void Cy_CCU8_SLICE_TrapConfig(Cy_CCU8_SLICE_t *const slice,
                               const Cy_CCU8_SLICE_TRAP_EXIT_MODE_t exit_mode,
                               const bool synch_with_pwm)
{
  uint32_t cmc;
  uint32_t tc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(((exit_mode == CY_CCU8_SLICE_TRAP_EXIT_MODE_AUTOMATIC) || \
             (exit_mode == CY_CCU8_SLICE_TRAP_EXIT_MODE_SW)));

  cmc = slice->CMC;

  /* Map trap function to Event 2 */
  cmc &= ~((uint32_t) CCU8_CC8_CMC_TS_Msk);
  cmc |= ((uint32_t) 1) << CCU8_CC8_CMC_TS_Pos;

  slice->CMC = cmc;

  tc = slice->TC;

  /* Configure synchronization option */
  if (synch_with_pwm == true)
  {
    tc |= (uint32_t) CCU8_CC8_TC_TRPSE_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_TRPSE_Msk);
  }

  /* Configure exit mode */
  if (exit_mode == CY_CCU8_SLICE_TRAP_EXIT_MODE_SW)
  {
    tc |= (uint32_t) CCU8_CC8_TC_TRPSW_Msk;
  }
  else
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_TRPSW_Msk);
  }

  slice->TC = tc;
}

/* API to configure a slice Status Bit Override event */
void Cy_CCU8_SLICE_ConfigureStatusBitOverrideEvent(Cy_CCU8_SLICE_t *const slice,
    const Cy_CCU8_SLICE_EVENT_CONFIG_t *const ev1_config,
    const Cy_CCU8_SLICE_EVENT_CONFIG_t *const ev2_config)
{
  uint32_t ins;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(Cy_CCU8_SLICE_IsInputvalid(ev1_config->mapped_input));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EDGE_SENSITIVITY(ev1_config->edge));
  CY_ASSERT(((ev1_config->level == CY_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) || \
              (ev1_config->level == CY_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_FILTER(ev1_config->duration));
  CY_ASSERT(Cy_CCU8_SLICE_IsInputvalid(ev2_config->mapped_input));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EDGE_SENSITIVITY(ev2_config->edge));
  CY_ASSERT(((ev2_config->level == CY_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) || \
              (ev2_config->level == CY_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_FILTER(ev2_config->duration));

#if defined(CY_CCU8V3)
  ins = slice->INS2;

  /* Configure the edge sensitivity for event 1 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU8_CC8_INS2_EV1EM_Pos);
  ins |= ((uint32_t) ev1_config->edge) << CCU8_CC8_INS2_EV1EM_Pos;

  /* Configure the edge sensitivity for event 2 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU8_CC8_INS2_EV2EM_Pos);
  ins |= ((uint32_t) ev2_config->edge) << CCU8_CC8_INS2_EV2EM_Pos;

  /* Configure the level sensitivity for event 1 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU8_CC8_INS2_EV1LM_Pos);
  ins |= ((uint32_t) ev1_config->level) << CCU8_CC8_INS2_EV1LM_Pos;

  /* Configure the level sensitivity for event 2 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU8_CC8_INS2_EV2LM_Pos);
  ins |= ((uint32_t) ev2_config->level) << CCU8_CC8_INS2_EV2LM_Pos;

  /* Configure the debounce filter for event 1 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU8_CC8_INS2_LPF1M_Pos);
  ins |= ((uint32_t) ev1_config->duration) << CCU8_CC8_INS2_LPF1M_Pos;

  /* Configure the debounce filter for event 2 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU8_CC8_INS2_LPF2M_Pos);
  ins |= ((uint32_t) ev2_config->duration) << CCU8_CC8_INS2_LPF2M_Pos;

  slice->INS2 = ins;

  ins = slice->INS1;
  /* Next, the input for Event1 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU8_CC8_INS1_EV1IS_Pos);
  ins |= ((uint32_t) ev1_config->mapped_input) << CCU8_CC8_INS1_EV1IS_Pos;

  /* Finally, the input for Event2 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU8_CC8_INS1_EV2IS_Pos);
  ins |= ((uint32_t) ev2_config->mapped_input) << CCU8_CC8_INS1_EV2IS_Pos;

  slice->INS1 = ins;
#else
  ins = slice->INS;

  /* Configure the edge sensitivity for event 1 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU8_CC8_INS_EV1EM_Pos);
  ins |= ((uint32_t) ev1_config->edge) << CCU8_CC8_INS_EV1EM_Pos;

  /* Configure the edge sensitivity for event 2 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << CCU8_CC8_INS_EV2EM_Pos);
  ins |= ((uint32_t) ev2_config->edge) << CCU8_CC8_INS_EV2EM_Pos;

  /* Configure the level sensitivity for event 1 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU8_CC8_INS_EV1LM_Pos);
  ins |= ((uint32_t) ev1_config->level) << CCU8_CC8_INS_EV1LM_Pos;

  /* Configure the level sensitivity for event 2 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << CCU8_CC8_INS_EV2LM_Pos);
  ins |= ((uint32_t) ev2_config->level) << CCU8_CC8_INS_EV2LM_Pos;

  /* Configure the debounce filter for event 1 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU8_CC8_INS_LPF1M_Pos);
  ins |= ((uint32_t) ev1_config->duration) << CCU8_CC8_INS_LPF1M_Pos;

  /* Configure the debounce filter for event 2 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << CCU8_CC8_INS_LPF2M_Pos);
  ins |= ((uint32_t) ev2_config->duration) << CCU8_CC8_INS_LPF2M_Pos;

  /* Next, the input for Event1 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU8_CC8_INS_EV1IS_Pos);
  ins |= ((uint32_t) ev1_config->mapped_input) << CCU8_CC8_INS_EV1IS_Pos;

  /* Finally, the input for Event2 */
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << CCU8_CC8_INS_EV2IS_Pos);
  ins |= ((uint32_t) ev2_config->mapped_input) << CCU8_CC8_INS_EV2IS_Pos;

  slice->INS = ins;
#endif
}

/* API to configure a slice trigger event */
void Cy_CCU8_SLICE_ConfigureEvent(Cy_CCU8_SLICE_t *const slice,
                                   const Cy_CCU8_SLICE_EVENT_t event,
                                   const Cy_CCU8_SLICE_EVENT_CONFIG_t *const config)
{
  uint32_t ins;
  uint8_t  pos;
  uint8_t  offset;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));
  CY_ASSERT(Cy_CCU8_SLICE_IsInputvalid(config->mapped_input));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EDGE_SENSITIVITY(config->edge));
  CY_ASSERT(((config->level == CY_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH) || \
              (config->level == CY_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW)));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_FILTER(config->duration));

  /* Calculate offset with reference to event */
  offset = ((uint8_t) event) - 1U;

#if defined(CY_CCU8V3)
  ins = slice->INS2;

  /* First, configure the edge sensitivity */
  pos = ((uint8_t) CCU8_CC8_INS2_EV0EM_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->edge) << pos;

  /* Next, the level */
  pos = ((uint8_t) CCU8_CC8_INS2_EV0LM_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << pos);
  ins |= ((uint32_t) (config->level)) << pos;

  /* Next, the debounce filter */
  pos = ((uint8_t) CCU8_CC8_INS2_LPF0M_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->duration) << pos;

  slice->INS2 = ins;

  ins = slice->INS1;

  /* Finally the input */
  pos = ((uint8_t) CCU8_CC8_INS1_EV0IS_Pos) + (uint8_t)(offset << 3U);
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->mapped_input) << pos;

  slice->INS1 = ins;

#else
  ins = slice->INS;

  /* First, configure the edge sensitivity */
  pos = ((uint8_t) CCU8_CC8_INS_EV0EM_Pos) + (uint8_t)(offset << 1U);
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_EDGE_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->edge) << pos;

  /* Next, the level */
  pos = ((uint8_t) CCU8_CC8_INS_EV0LM_Pos) + offset;
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_LEVEL_CONFIG_MASK) << pos);
  ins |= ((uint32_t) (config->level)) << pos;

  /* Next, the debounce filter */
  pos = ((uint8_t) CCU8_CC8_INS_LPF0M_Pos) + (uint8_t)(offset << 1U);
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_FILTER_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->duration) << pos;

  /* Finally the input */
  pos = ((uint8_t) CCU8_CC8_INS_EV0IS_Pos) + (uint8_t)(offset << 2U);
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) config->mapped_input) << pos;

  slice->INS = ins;
#endif
}

/* API to bind an input to a slice trigger event */
void Cy_CCU8_SLICE_SetInput(Cy_CCU8_SLICE_t *const slice,
                             const Cy_CCU8_SLICE_EVENT_t event,
                             const Cy_CCU8_SLICE_INPUT_t input)
{
  uint32_t ins;
  uint8_t  pos;
  uint8_t  offset;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_EVENT_ID(event));
  CY_ASSERT(Cy_CCU8_SLICE_IsInputvalid(input));

  /* Calculate offset with reference to event */
  offset = ((uint8_t) event) - 1U;

#if defined(CY_CCU8V3)
  pos = ((uint8_t) CCU8_CC8_INS1_EV0IS_Pos) + (uint8_t) (offset << 3U);
  ins = slice->INS1;
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) input) << pos;

  slice->INS1 = ins;
#else

  pos = ((uint8_t) CCU8_CC8_INS_EV0IS_Pos) + (uint8_t) (offset << 2U);
  ins = slice->INS;
  ins &= ~(((uint32_t) CY_CCU8_SLICE_EVENT_INPUT_CONFIG_MASK) << pos);
  ins |= ((uint32_t) input) << pos;

  slice->INS = ins;
#endif
}

/* API to program timer repeat mode - Single shot vs repeat  */
void Cy_CCU8_SLICE_SetTimerRepeatMode(Cy_CCU8_SLICE_t *const slice,
                                       const Cy_CCU8_SLICE_TIMER_REPEAT_MODE_t mode)
{
  uint32_t tc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(((mode == CY_CCU8_SLICE_TIMER_REPEAT_MODE_REPEAT) || \
              (mode == CY_CCU8_SLICE_TIMER_REPEAT_MODE_SINGLE)));

  tc = slice->TC;

  if (CY_CCU8_SLICE_TIMER_REPEAT_MODE_REPEAT == mode)
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_TSSM_Msk);
  }
  else
  {
    tc |= (uint32_t) CCU8_CC8_TC_TSSM_Msk;
  }

  slice->TC = tc;
}

/* Programs timer counting mode  */
void Cy_CCU8_SLICE_SetTimerCountingMode(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_TIMER_COUNT_MODE_t mode)
{
  uint32_t tc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(((mode == CY_CCU8_SLICE_TIMER_COUNT_MODE_EA) || \
              (mode == CY_CCU8_SLICE_TIMER_COUNT_MODE_CA)));

  tc = slice->TC;

  if (CY_CCU8_SLICE_TIMER_COUNT_MODE_EA == mode)
  {
    tc &= ~((uint32_t) CCU8_CC8_TC_TCM_Msk);
  }
  else
  {
    tc |= (uint32_t) CCU8_CC8_TC_TCM_Msk;
  }

  slice->TC = tc;
}

/* Programs period match value of the timer  */
void Cy_CCU8_SLICE_SetTimerPeriodMatch(Cy_CCU8_SLICE_t *const slice, const uint16_t period_val)
{
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  slice->PRS = (uint32_t) period_val;
}

/* Retrieves desired capture register value */
uint32_t Cy_CCU8_SLICE_GetCaptureRegisterValue(const Cy_CCU8_SLICE_t *const slice, const uint8_t reg_num)
{
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT((reg_num < 4U));
  return (slice->CV[reg_num]);
}

/* @brief Retrieves the latest captured timer value */
Cy_CCU8_STATUS_t Cy_CCU8_SLICE_GetLastCapturedTimerValue(const Cy_CCU8_SLICE_t *const slice,
    const Cy_CCU8_SLICE_CAP_REG_SET_t set,
    uint32_t *val_ptr)
{

  Cy_CCU8_STATUS_t retval;
  uint8_t i;
  uint8_t start;
  uint8_t end;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(((set == CY_CCU8_SLICE_CAP_REG_SET_LOW) || \
              (set == CY_CCU8_SLICE_CAP_REG_SET_HIGH)));

  retval = CY_CCU8_STATUS_ERROR;

  /* First check if extended capture mode is enabled */
  if ((slice->TC) & CCU8_CC8_TC_ECM_Msk)
  {
    /* Extended capture mode has been enabled. So start with the lowest capture register and work your way up */
    start = 0U;
    end = CY_CCU8_NUM_SLICES_PER_MODULE;
  }
  else
  {
    /* Extended capture mode is not enabled */
    if (set == CY_CCU8_SLICE_CAP_REG_SET_HIGH)
    {
      start = ((uint8_t) CY_CCU8_NUM_SLICES_PER_MODULE) >> 1U;
      end   = (uint8_t) CY_CCU8_NUM_SLICES_PER_MODULE;
    }
    else
    {
      start = 0U;
      end = ((uint8_t) CY_CCU8_NUM_SLICES_PER_MODULE) >> 1U;
    }
  }

  for (i = start; i < end; i++)
  {
    if ( (slice->CV[i]) & CCU8_CC8_CV_FFL_Msk )
    {
      *val_ptr = slice->CV[i];
      retval = CY_CCU8_STATUS_OK;
      break;
    }
  }

  return retval;
}

/* Retrieves timer capture value from a FIFO made of capture registers */
uint32_t Cy_CCU8_SLICE_GetCapturedValueFromFifo(const Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_CAP_REG_SET_t set)
{
  uint32_t cap;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(((set == CY_CCU8_SLICE_CAP_REG_SET_LOW) || \
              (set == CY_CCU8_SLICE_CAP_REG_SET_HIGH)));

  if (CY_CCU8_SLICE_CAP_REG_SET_LOW == set)
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
void Cy_CCU8_SLICE_EnableDithering(Cy_CCU8_SLICE_t *const slice,
                                    const bool period_dither,
                                    const bool duty_dither,
                                    const uint8_t spread)
{
  uint32_t tc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));

  tc = slice->TC;
  tc &= ~((uint32_t) CCU8_CC8_TC_DITHE_Msk);

  if (true == period_dither)
  {
    tc |= (((uint32_t) CY_CCU8_SLICE_DITHER_PERIOD_MASK) << CCU8_CC8_TC_DITHE_Pos);
  }
  if (true == duty_dither)
  {
    tc |= (((uint32_t) CY_CCU8_SLICE_DITHER_DUTYCYCLE_MASK) << CCU8_CC8_TC_DITHE_Pos);
  }

  slice->TC = tc;

  Cy_CCU8_SLICE_SetDitherCompareValue((Cy_CCU8_SLICE_t *)slice, (uint8_t)spread);
}

/* Programs Pre-scaler divider  */
void Cy_CCU8_SLICE_SetPrescaler(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_PRESCALER_t div_val)
{
  uint32_t fpc;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));

  /* If the prescaler is not running, update directly the divider*/
  fpc = slice->FPC;
  fpc &= ~((uint32_t) CCU8_CC8_FPC_PVAL_Msk);
  fpc |= ((uint32_t) div_val) << CCU8_CC8_FPC_PVAL_Pos;
  slice->FPC = fpc;

  /*
   * In any case, update the initial value of the divider which is to be loaded once the prescaler increments to the
   * compare value.
   */
  slice->PSC = (uint32_t) div_val;
}

/* Programs timer compare match value for channel 1 or 2 */
void Cy_CCU8_SLICE_SetTimerCompareMatch(Cy_CCU8_SLICE_t *const slice,
    const Cy_CCU8_SLICE_COMPARE_CHANNEL_t channel,
    const uint16_t compare_val)
{
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_COMP_CHANNEL(channel));

  if (CY_CCU8_SLICE_COMPARE_CHANNEL_1 == channel)
  {
    slice->CR1S = (uint32_t) compare_val;
  }
  else
  {
    slice->CR2S = (uint32_t) compare_val;
  }
}

/* Returns the timer compare match value for channel 1 or 2 */
uint16_t Cy_CCU8_SLICE_GetTimerCompareMatch(const Cy_CCU8_SLICE_t *const slice,
    const Cy_CCU8_SLICE_COMPARE_CHANNEL_t channel)
{
  uint16_t compare_value;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_COMP_CHANNEL(channel));

  if (CY_CCU8_SLICE_COMPARE_CHANNEL_1 == channel)
  {
    compare_value = (uint16_t) slice->CR1;
  }
  else
  {
    compare_value = (uint16_t) slice->CR2;
  }

  return (compare_value);
}

/* Binds a capcom event to an NVIC node  */
void Cy_CCU8_SLICE_SetInterruptNode(Cy_CCU8_SLICE_t *const slice,
                                     const Cy_CCU8_SLICE_IRQ_ID_t event,
                                     const Cy_CCU8_SLICE_SR_ID_t sr)
{
  uint32_t srs;
  uint32_t mask;
  int32_t pos;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_SR_ID(sr));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_INTERRUPT(event));

  switch (event)
  {
    case CY_CCU8_SLICE_IRQ_ID_PERIOD_MATCH:
    case CY_CCU8_SLICE_IRQ_ID_ONE_MATCH:
      mask = ((uint32_t) CCU8_CC8_SRS_POSR_Msk);
      pos  = CCU8_CC8_SRS_POSR_Pos;
      break;

    case CY_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_1:
    case CY_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_DOWN_CH_1:
      mask = ((uint32_t) CCU8_CC8_SRS_CM1SR_Msk);
      pos  = CCU8_CC8_SRS_CM1SR_Pos;
      break;

    case CY_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_2:
    case CY_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_DOWN_CH_2:
      mask = ((uint32_t) CCU8_CC8_SRS_CM2SR_Msk);
      pos  = CCU8_CC8_SRS_CM2SR_Pos;
      break;

    case CY_CCU8_SLICE_IRQ_ID_EVENT0:
      mask = ((uint32_t) CCU8_CC8_SRS_E0SR_Msk);
      pos  = CCU8_CC8_SRS_E0SR_Pos;
      break;

    case CY_CCU8_SLICE_IRQ_ID_EVENT1:
      mask = ((uint32_t) CCU8_CC8_SRS_E1SR_Msk);
      pos  = CCU8_CC8_SRS_E1SR_Pos;
      break;

    case CY_CCU8_SLICE_IRQ_ID_EVENT2:
    case CY_CCU8_SLICE_IRQ_ID_TRAP:
      mask = ((uint32_t) CCU8_CC8_SRS_E2SR_Msk);
      pos  = CCU8_CC8_SRS_E2SR_Pos;
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
void Cy_CCU8_SLICE_SetPassiveLevel(Cy_CCU8_SLICE_t *const slice,
                                    const Cy_CCU8_SLICE_OUTPUT_t out,
                                    const Cy_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_t level)
{
  uint32_t psl;

  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_OUTPUT(out));
  CY_ASSERT(((level == CY_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_LOW) || \
              (level == CY_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH)));

  psl = slice->PSL;

  psl &= ~((uint32_t) out);
  psl |= (uint32_t) level << ((uint32_t)out >> 1U);

  /* Program CC8 slice output passive level */
  slice->PSL = psl;
}

/* Initializes Dead time configuration for the slice outputs */
void Cy_CCU8_SLICE_DeadTimeInit(Cy_CCU8_SLICE_t *const slice,
                                 const Cy_CCU8_SLICE_DEAD_TIME_CONFIG_t *const config)
{
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));

  /* Program dead time value for channel 1 */
  slice->DC1R = config->dc1r;
  /* Program dead time value for channel 2 */
  slice->DC2R = config->dc2r;
  /* Program dead time control configuration */
  slice->DTC = config->dtc;
}

/* Activates or deactivates dead time for compare channel and ST path */
void Cy_CCU8_SLICE_ConfigureDeadTime(Cy_CCU8_SLICE_t *const slice, const uint8_t mask)
{
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT((mask <= CY_CCU8_SLICE_DEAD_TIME_CONFIG_MASK));

  slice->DTC &= ~((uint32_t) CY_CCU8_SLICE_DEAD_TIME_CONFIG_MASK);
  slice->DTC |= (uint32_t) mask;
}

/* Configures rising edge delay and falling edge delay for dead time */
void Cy_CCU8_SLICE_SetDeadTimeValue(Cy_CCU8_SLICE_t *const slice,
                                     const Cy_CCU8_SLICE_COMPARE_CHANNEL_t channel,
                                     const uint8_t rise_value,
                                     const uint8_t fall_value)
{
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_COMP_CHANNEL(channel));

  if (CY_CCU8_SLICE_COMPARE_CHANNEL_1 == channel)
  {
    slice->DC1R = (((uint32_t) fall_value) << CCU8_CC8_DC1R_DT1F_Pos) | ((uint32_t) rise_value);
  }
  else
  {
    slice->DC2R = (((uint32_t) fall_value) << CCU8_CC8_DC2R_DT2F_Pos) | ((uint32_t) rise_value);
  }
}

/* Configures clock division factor for dead time */
void Cy_CCU8_SLICE_SetDeadTimePrescaler(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_DTC_DIV_t div_val)
{
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_DTC_DIV(div_val));

  slice->DTC &= ~((uint32_t) CCU8_CC8_DTC_DTCC_Msk);
  slice->DTC |= ((uint32_t) div_val) << CCU8_CC8_DTC_DTCC_Pos;
}

/* Configures status ST1, ST2 mapping to STy */
void Cy_CCU8_SLICE_ConfigureStatusBitOutput(Cy_CCU8_SLICE_t *const slice, const Cy_CCU8_SLICE_STATUS_t channel)
{
  CY_ASSERT(Cy_CCU8_IsValidSlice(slice));
  CY_ASSERT(CY_CCU8_SLICE_CHECK_SLICE_STATUS(channel));

  slice->TC &= ~((uint32_t) CCU8_CC8_TC_STOS_Msk);
  slice->TC |= ((uint32_t) channel) << CCU8_CC8_TC_STOS_Pos;
}

#endif /* CCU80 */
