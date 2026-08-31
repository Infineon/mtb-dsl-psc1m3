/**
 * @file cy_bccu.c
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

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_bccu.h"

#if defined(BCCU0)
#include "cy_scu.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define CY_BCCU_NO_OF_CHANNELS    (9U)
#define CY_BCCU_CHANNEL_MASK      ((0x1 << CY_BCCU_NO_OF_CHANNELS)-1)
#define CY_BCCU_NO_OF_DIM_ENGINE  (3U)
#define CY_BCCU_DIM_ENGINE_MASK   (((0x1 << CY_BCCU_NO_OF_DIM_ENGINE)-1))

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * GLOBAL DATA
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * LOCAL/UTILITY ROUTINES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/*
 * API to initialise the global resources of a BCCU module
 */
void Cy_BCCU_GlobalInit(Cy_BCCU_t *const bccu, const Cy_BCCU_GLOBAL_CONFIG_t *const config)
{
  Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_BCCU0);

  bccu->GLOBCON = config->globcon;

  bccu->GLOBCLK = config->globclk;
  bccu->GLOBDIM = config->global_dimlevel;

}

/*
 * API to configure the global trigger mode & delay of a BCCU module
 */
void Cy_BCCU_ConfigGlobalTrigger(Cy_BCCU_t *const bccu, Cy_BCCU_TRIGMODE_t mode, Cy_BCCU_TRIGDELAY_t delay)
{
  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_TM_Msk | BCCU_GLOBCON_TRDEL_Msk);
  bccu->GLOBCON |= ((uint32_t)mode | ((uint32_t)delay << BCCU_GLOBCON_TRDEL_Pos));
}

/*
 * API to configure the trap input selection of a BCCU module
 */
void Cy_BCCU_SelectTrapInput (Cy_BCCU_t *const bccu, Cy_BCCU_CH_TRAP_IN_t input)
{
  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_TRAPIS_Msk);
  bccu->GLOBCON |= ((uint32_t)input << BCCU_GLOBCON_TRAPIS_Pos);
}

/*
 * API to configure the trap edge selection of a BCCU module
 */
void Cy_BCCU_SetTrapEdge (Cy_BCCU_t *const bccu, Cy_BCCU_CH_TRAP_EDGE_t edge)
{
  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_TRAPED_Msk);
  bccu->GLOBCON |= ((uint32_t)edge << BCCU_GLOBCON_TRAPED_Pos);
}

/*
 * API to configure the suspend mode of a BCCU module
 */
void Cy_BCCU_ConfigSuspendMode (Cy_BCCU_t *const bccu, Cy_BCCU_SUSPEND_MODE_t mode)
{
  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_SUSCFG_Msk);
  bccu->GLOBCON |= ((uint32_t)mode << BCCU_GLOBCON_SUSCFG_Pos);
}

/*
 * API to configure number of consecutive zeroes allowed at modulator output (flicker watch-dog number)
 */
void Cy_BCCU_SetFlickerWDThreshold (Cy_BCCU_t *const bccu, uint32_t threshold_no)
{
  CY_ASSERT((threshold_no <= BCCU_GLOBCON_WDMBN_Msk));

  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_WDMBN_Msk);
  bccu->GLOBCON |= (uint32_t)(threshold_no << BCCU_GLOBCON_WDMBN_Pos);
}

/*
 * API to configure the fast clock prescaler factor of a BCCU module
 */
void Cy_BCCU_SetFastClockPrescaler (Cy_BCCU_t *const bccu, uint32_t div)
{
  CY_ASSERT((div <= BCCU_GLOBCLK_FCLK_PS_Msk));

  bccu->GLOBCLK &= ~(uint32_t)(BCCU_GLOBCLK_FCLK_PS_Msk);
  bccu->GLOBCLK |= div;

}

/*
 * API to configure the dimmer clock prescaler factor of a BCCU module
 */
void Cy_BCCU_SetDimClockPrescaler (Cy_BCCU_t *const bccu, uint32_t div)
{
  CY_ASSERT((div <= BCCU_GLOBCLK_DCLK_PS_Msk));

  bccu->GLOBCLK &= ~(uint32_t)(BCCU_GLOBCLK_DCLK_PS_Msk);
  bccu->GLOBCLK |= (uint32_t)(div << BCCU_GLOBCLK_DCLK_PS_Pos);

}

/*
 * API to configure the modulator output (bit-time) clock prescaler factor of a BCCU module
 */
void Cy_BCCU_SelectBitClock (Cy_BCCU_t *const bccu, Cy_BCCU_BCLK_MODE_t div)
{
  bccu->GLOBCLK &= ~(uint32_t)(BCCU_GLOBCLK_BCS_Msk);
  bccu->GLOBCLK |= ((uint32_t)div << BCCU_GLOBCLK_BCS_Pos);
}

/*
 * API to enable the channels at the same time
 */
void Cy_BCCU_ConcurrentEnableChannels (Cy_BCCU_t *const bccu, uint32_t mask)
{
  CY_ASSERT((mask <= CY_BCCU_CHANNEL_MASK));

  bccu->CHEN |= mask;
}

/*
 * API to disable the channels at the same time
 */
void Cy_BCCU_ConcurrentDisableChannels (Cy_BCCU_t *const bccu, uint32_t mask)
{
  CY_ASSERT((mask <= CY_BCCU_CHANNEL_MASK));
  bccu->CHEN &= ~(uint32_t)(mask);
}

/*
 * API to set the channel's output passive levels at the same time
 */
void Cy_BCCU_ConcurrentSetOutputPassiveLevel(Cy_BCCU_t *const bccu, uint32_t chan_mask, Cy_BCCU_CH_ACTIVE_LEVEL_t level)
{
  CY_ASSERT((chan_mask <= CY_BCCU_CHANNEL_MASK));

  bccu->CHOCON &= ~(uint32_t)(chan_mask);
  bccu->CHOCON |= (chan_mask * (uint32_t)level);
}

/*
 * API to enable the various types of traps at the same time
 */
void Cy_BCCU_ConcurrentEnableTrap (Cy_BCCU_t *const bccu, uint32_t mask)
{
  CY_ASSERT((mask <= CY_BCCU_CHANNEL_MASK));

  bccu->CHOCON |= (uint32_t)(mask << BCCU_CHOCON_CH0TPE_Pos);
}

/*
 * API to disable the various types of traps at the same time
 */
void Cy_BCCU_ConcurrentDisableTrap (Cy_BCCU_t *const bccu, uint32_t mask)
{
  CY_ASSERT((mask <= CY_BCCU_CHANNEL_MASK));

  bccu->CHOCON &= ~(uint32_t)(mask << BCCU_CHOCON_CH0TPE_Pos);
}

/*
 * API to configure trigger mode and trigger delay at the same time, and also configure the channel enable
 */
void Cy_BCCU_ConcurrentConfigTrigger (Cy_BCCU_t *const bccu, Cy_BCCU_TRIG_CONFIG_t *trig)
{
  uint32_t reg;

  CY_ASSERT((trig->mask_chans <= CY_BCCU_CHANNEL_MASK));

  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_TM_Msk | BCCU_GLOBCON_TRDEL_Msk);
  bccu->GLOBCON |= ((uint32_t)trig->mode | ((uint32_t)trig->delay << BCCU_GLOBCON_TRDEL_Pos));
  reg = 0U;
  reg |= trig->mask_chans;
  reg |= ((uint32_t)trig->mask_trig_lines << BCCU_CHTRIG_TOS0_Pos);
  bccu->CHTRIG = reg;
}

/*
 * API to start the linear walk of the channels to change towards target intensity at the same time
 */
void Cy_BCCU_ConcurrentStartLinearWalk (Cy_BCCU_t *const bccu, uint32_t mask)
{
  CY_ASSERT((mask <= CY_BCCU_CHANNEL_MASK));

  bccu->CHSTRCON |= (uint32_t)(mask);
}

/*
 * API to abort the linear walk of the channels at the same time
 */
void Cy_BCCU_ConcurrentAbortLinearWalk (Cy_BCCU_t *const bccu, uint32_t mask)
{
  CY_ASSERT((mask <= CY_BCCU_CHANNEL_MASK));

  bccu->CHSTRCON |= (uint32_t)(mask << BCCU_CHSTRCON_CH0A_Pos);
}

/*
 * API to enable the dimming engines at the same time
 */
void Cy_BCCU_ConcurrentEnableDimmingEngine (Cy_BCCU_t *const bccu, uint32_t mask)
{
  CY_ASSERT((mask <= CY_BCCU_DIM_ENGINE_MASK));

  bccu->DEEN = (uint32_t)(mask);
}

/*
 * API to enable the dimming engines at the same time
 */
void Cy_BCCU_ConcurrentDisableDimmingEngine (Cy_BCCU_t *const bccu, uint32_t mask)
{
  CY_ASSERT((mask <= CY_BCCU_DIM_ENGINE_MASK));

  bccu->DEEN &= ~(uint32_t)(mask);
}

/*
 * API to start the dimming engines at the same time to change towards target dim level
 */
void Cy_BCCU_ConcurrentStartDimming (Cy_BCCU_t *const bccu, uint32_t mask)
{
  CY_ASSERT((mask <= CY_BCCU_DIM_ENGINE_MASK));

  bccu->DESTRCON = (uint32_t)(mask);
}

/*
 * API to abort the dimming engines at the same time
 */
void Cy_BCCU_ConcurrentAbortDimming (Cy_BCCU_t *const bccu, uint32_t mask)
{
  CY_ASSERT((mask <= CY_BCCU_DIM_ENGINE_MASK));

  bccu->DESTRCON = (uint32_t)(mask << BCCU_DESTRCON_DE0A_Pos);
}

/*
 * API to configure the dim level of a dimming engine
 */
void  Cy_BCCU_SetGlobalDimmingLevel (Cy_BCCU_t *const bccu, uint32_t level)
{
  CY_ASSERT((level <= BCCU_GLOBDIM_GLOBDIM_Msk));

  bccu->GLOBDIM = level;
}

/*
 * API to enable a specific channel
 */
void Cy_BCCU_EnableChannel (Cy_BCCU_t *const bccu, uint32_t chan_no)
{
  CY_ASSERT((chan_no <= (CY_BCCU_NO_OF_CHANNELS - 1)));

  bccu->CHEN |= (uint32_t)(BCCU_CHEN_ECH0_Msk << chan_no);
}

/*
 * API to disable a specific channel
 */
void Cy_BCCU_DisableChannel (Cy_BCCU_t *const bccu, uint32_t chan_no)
{
  CY_ASSERT((chan_no <= (CY_BCCU_NO_OF_CHANNELS - 1)));

  bccu->CHEN &= ~(uint32_t)(BCCU_CHEN_ECH0_Msk << chan_no);
}

/*
 * API to set the specific channel's passive level
 */
void Cy_BCCU_SetOutputPassiveLevel(Cy_BCCU_t *const bccu, uint32_t chan_no, Cy_BCCU_CH_ACTIVE_LEVEL_t level)
{
  CY_ASSERT((chan_no <= (CY_BCCU_NO_OF_CHANNELS - 1)));

  bccu->CHOCON |= ((uint32_t)level << chan_no);
}

/*
 * API to enable the specific channel trap
 */
void Cy_BCCU_EnableTrap (Cy_BCCU_t *const bccu, uint32_t chan_no)
{
  CY_ASSERT((chan_no <= (CY_BCCU_NO_OF_CHANNELS - 1)));

  bccu->CHOCON |= (uint32_t)(BCCU_CHOCON_CH0TPE_Msk << chan_no);
}

/*
 * API to disable the specific channel trap
 */
void Cy_BCCU_DisableTrap (Cy_BCCU_t *const bccu, uint32_t chan_no)
{
  CY_ASSERT((chan_no <= (CY_BCCU_NO_OF_CHANNELS - 1)));

  bccu->CHOCON &= ~(uint32_t)(BCCU_CHOCON_CH0TPE_Msk << chan_no);
}

/*
 * API to configure specific channel trigger enable and trigger line.
 */
void Cy_BCCU_EnableChannelTrigger (Cy_BCCU_t *const bccu, uint32_t chan_no, Cy_BCCU_CH_TRIGOUT_t trig_line)
{
  uint32_t reg;
  CY_ASSERT((chan_no <= (CY_BCCU_NO_OF_CHANNELS - 1)));

  bccu->CHTRIG &= ~(uint32_t)(BCCU_CHTRIG_TOS0_Msk << chan_no);
  reg = (uint32_t)(BCCU_CHTRIG_ET0_Msk << chan_no);
  reg |= ((uint32_t)trig_line << (BCCU_CHTRIG_TOS0_Pos + chan_no));
  bccu->CHTRIG |= reg;
}

/*
 * API to disable specific channel
 */
void Cy_BCCU_DisableChannelTrigger (Cy_BCCU_t *const bccu, uint32_t chan_no)
{
  CY_ASSERT((chan_no <= (CY_BCCU_NO_OF_CHANNELS - 1)));

  bccu->CHTRIG &= ~(uint32_t)(BCCU_CHTRIG_ET0_Msk << chan_no);
}

/*
 * API to initialise the channel of a BCCU module
 */
void Cy_BCCU_CH_Init (Cy_BCCU_CH_t *const channel, const Cy_BCCU_CH_CONFIG_t *const config)
{
  channel->CHCONFIG = config->chconfig;

  channel->PKCMP = config->pkcmp;

  channel->PKCNTR = config->pkcntr;
}

/*
 * API to configure channel trigger edge and force trigger edge
 */
void Cy_BCCU_CH_ConfigTrigger (Cy_BCCU_CH_t *const channel, Cy_BCCU_CH_TRIG_EDGE_t edge, uint32_t force_trig_en)
{
  uint32_t reg;
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_TRED_Msk | BCCU_CH_CHCONFIG_ENFT_Msk);

  reg = ((uint32_t)edge << BCCU_CH_CHCONFIG_TRED_Pos);
  reg |= (uint32_t)(force_trig_en << BCCU_CH_CHCONFIG_ENFT_Pos);
  channel->CHCONFIG |= reg;
}

/*
 * API to configure the linear walker clock prescaler factor of a BCCU channel
 */
void Cy_BCCU_CH_SetLinearWalkPrescaler (Cy_BCCU_CH_t *const channel, uint32_t clk_div)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_LINPRES_Msk);
  channel->CHCONFIG |= (uint32_t)(clk_div << BCCU_CH_CHCONFIG_LINPRES_Pos);
}

/*
 * API to set channel target intensity
 */
void Cy_BCCU_CH_SetTargetIntensity (Cy_BCCU_CH_t *const channel, uint32_t ch_int)
{
  channel->INTS = ch_int;
}

/*
 * API to retrieve the channel actual intensity
 */
uint32_t Cy_BCCU_CH_ReadIntensity (Cy_BCCU_CH_t *const channel)
{
  return (uint32_t)(channel->INT &BCCU_CH_INT_CHINT_Msk);
}

/*
 * API to enable packer. Also configures packer threshold, off-time and on-time compare levels
 */
void Cy_BCCU_CH_EnablePacker (Cy_BCCU_CH_t *const channel, uint32_t thresh, uint32_t off_comp, uint32_t on_comp)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_PEN_Msk | BCCU_CH_CHCONFIG_PKTH_Msk);
  channel->CHCONFIG |= thresh;
  channel->PKCMP = (off_comp | (uint32_t)(on_comp << BCCU_CH_PKCMP_ONCMP_Pos));
  channel->CHCONFIG |= (uint32_t)BCCU_CH_CHCONFIG_PEN_Msk;
}

/*
 * API to configure packer threshold
 */
void Cy_BCCU_CH_SetPackerThreshold (Cy_BCCU_CH_t *const channel, uint32_t val)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_PKTH_Msk);
  channel->CHCONFIG |= val;
}

/*
 * API to configure packer off-time compare level
 */
void Cy_BCCU_CH_SetPackerOffCompare (Cy_BCCU_CH_t *const channel, uint32_t level)
{
  channel->PKCMP &= ~(uint32_t)(BCCU_CH_PKCMP_OFFCMP_Msk);
  channel->PKCMP |= level;
}

/*
 * API to configure packer on-time compare level.
 */
void Cy_BCCU_CH_SetPackerOnCompare (Cy_BCCU_CH_t *const channel, uint32_t level)
{
  channel->PKCMP &= ~(uint32_t)(BCCU_CH_PKCMP_ONCMP_Msk);
  channel->PKCMP |= (level << BCCU_CH_PKCMP_ONCMP_Pos);
}

/*
 * API to disable a packer.
 */
void Cy_BCCU_CH_DisablePacker (Cy_BCCU_CH_t *const channel)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_PEN_Msk);
}

/*
 * API to set packer off-time counter value
 */
void Cy_BCCU_CH_SetPackerOffCounter (Cy_BCCU_CH_t *const channel, uint32_t cnt_val)
{
  channel->PKCNTR &= ~(uint32_t)(BCCU_CH_PKCNTR_OFFCNTVAL_Msk);
  channel->PKCNTR |= cnt_val;
}

/*
 * API to set packer on-time counter value
 */
void Cy_BCCU_CH_SetPackerOnCounter (Cy_BCCU_CH_t *const channel, uint32_t cnt_val)
{
  channel->PKCNTR &= ~(uint32_t)(BCCU_CH_PKCNTR_ONCNTVAL_Msk);
  channel->PKCNTR |= (uint32_t)(cnt_val << BCCU_CH_PKCNTR_ONCNTVAL_Pos);
}

/*
 * API to select the dimming engine of a channel
 */
void Cy_BCCU_CH_SelectDimEngine (Cy_BCCU_CH_t *const channel, Cy_BCCU_CH_DIMMING_SOURCE_t sel)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_DSEL_Msk);
  channel->CHCONFIG |= ((uint32_t)sel << BCCU_CH_CHCONFIG_DSEL_Pos);
}

/*
 * API to bypass the dimming engine. And the brightness of channel is depending only on
 * intensity of the channel.
 */
void Cy_BCCU_CH_EnableDimmingBypass (Cy_BCCU_CH_t *const channel)
{
  channel->CHCONFIG |= (uint32_t)(BCCU_CH_CHCONFIG_DBP_Msk);
}

/*
 * API to disable the bypass of dimming engine. And the brightness of channel is depending
 * on intensity of channel and dimming level of dimming engine.
 */
void Cy_BCCU_CH_DisableDimmingBypass (Cy_BCCU_CH_t *const channel)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_DBP_Msk);
}

/*
 * API to initialise a specific dimming engine of a BCCU module
 */
void Cy_BCCU_DIM_Init (Cy_BCCU_DIM_t *const dim_engine, const Cy_BCCU_DIM_CONFIG_t *const config)
{
  dim_engine->DTT = config->dtt;
}

/*
 * API to set dimming engine target dim level
 */
void Cy_BCCU_DIM_SetTargetDimmingLevel (Cy_BCCU_DIM_t *const dim_engine, uint32_t level)
{
  dim_engine->DLS = level;
}

/*
 * API to configure the dimming clock prescaler factor of a dimming engine
 */
void Cy_BCCU_DIM_SetDimDivider (Cy_BCCU_DIM_t *const dim_engine, uint32_t div)
{
  dim_engine->DTT &= ~(uint32_t)(BCCU_DE_DTT_DIMDIV_Msk);
  dim_engine->DTT |= div;
}

/*
 * API to configure the dimming curve
 */
void Cy_BCCU_DIM_ConfigDimCurve (Cy_BCCU_DIM_t *const dim_engine, uint32_t dither_en, Cy_BCCU_DIM_CURVE_t sel)
{
  uint32_t reg;
  dim_engine->DTT &= ~(uint32_t)(BCCU_DE_DTT_DTEN_Msk | BCCU_DE_DTT_CSEL_Msk);
  reg = (uint32_t)(dither_en << BCCU_DE_DTT_DTEN_Pos);
  reg |= ((uint32_t)sel << BCCU_DE_DTT_CSEL_Pos);
  dim_engine->DTT |= reg;
}

#endif /* BCCU0 */
