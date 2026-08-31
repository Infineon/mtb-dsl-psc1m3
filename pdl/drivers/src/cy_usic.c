/**
 * @file cy_usic.c
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

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include "cy_usic.h"
#include "cy_scu.h"

#include <stdlib.h>     /* abs */

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define USIC_CH_INPR_Msk       (0x7UL)

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

void Cy_USIC_CH_Enable(Cy_USIC_CH_t *const channel)
{
  CY_ASSERT(Cy_USIC_IsChannelValid(channel));

  if ((channel == CY_USIC0_CH0) || (channel == CY_USIC0_CH1))
  {
    Cy_USIC_Enable(CY_USIC0);
  }
#if defined(USIC1)
  else if ((channel == CY_USIC1_CH0) || (channel == CY_USIC1_CH1))
  {
    Cy_USIC_Enable(CY_USIC1);
  }
#endif
#if defined(USIC2)
  else if ((channel == CY_USIC2_CH0) || (channel == CY_USIC2_CH1))
  {
    Cy_USIC_Enable(CY_USIC2);
  }
#endif
  else
  {
    CY_ASSERT(0U/*Always*/);
  }

  /* USIC channel switched on*/
  channel->KSCFG = (USIC_CH_KSCFG_MODEN_Msk | USIC_CH_KSCFG_BPMODEN_Msk);
  while ((channel->KSCFG & USIC_CH_KSCFG_MODEN_Msk) == 0U)
  {
    /* Wait till the channel is enabled */
  }

  /* Set USIC channel in IDLE mode */
  channel->CCR &= (uint32_t)~USIC_CH_CCR_MODE_Msk;
}

void Cy_USIC_CH_Disable(Cy_USIC_CH_t *const channel)
{
  channel->KSCFG = (uint32_t)((channel->KSCFG & (~USIC_CH_KSCFG_MODEN_Msk)) | USIC_CH_KSCFG_BPMODEN_Msk);
}

Cy_USIC_CH_STATUS_t Cy_USIC_CH_SetBaudrate(Cy_USIC_CH_t *const channel, uint32_t rate, uint32_t oversampling)
{
  Cy_USIC_CH_STATUS_t status;

  uint32_t peripheral_clock;

  uint32_t clock_divider;
  uint32_t clock_divider_min;

  uint32_t pdiv;
  uint32_t pdiv_int;
  uint32_t pdiv_int_min;

  uint32_t pdiv_frac;
  uint32_t pdiv_frac_min;

  /* The rate and peripheral clock are divided by 100 to be able to use only 32bit arithmetic */
  if ((rate >= 100U) && (oversampling != 0U))
  {
    peripheral_clock = Cy_SCU_CLOCK_GetPeripheralClockFrequency() / 100U;
    rate = rate / 100U;

    clock_divider_min = 1024U;
    pdiv_int_min = 1U;
    pdiv_frac_min = 0x3ffU;

    for (clock_divider = 1024U; clock_divider > 0U; --clock_divider)
    {
      pdiv = ((peripheral_clock * clock_divider) / (rate * oversampling));
      pdiv_int = pdiv >> 10U;
      pdiv_frac = pdiv & 0x3ffU;

      if ((pdiv_int <= 1024U) && (pdiv_frac < pdiv_frac_min))
      {
        pdiv_frac_min = pdiv_frac;
        pdiv_int_min = pdiv_int;
        clock_divider_min = clock_divider;
      }
    }

    channel->FDR = CY_USIC_CH_BRG_CLOCK_DIVIDER_MODE_FRACTIONAL |
                   ((clock_divider_min - 1) << USIC_CH_FDR_STEP_Pos);

    channel->BRG = (channel->BRG & ~(USIC_CH_BRG_DCTQ_Msk |
                                     USIC_CH_BRG_PDIV_Msk |
                                     USIC_CH_BRG_PCTQ_Msk |
                                     USIC_CH_BRG_PPPEN_Msk)) |
                   ((oversampling - 1U) << USIC_CH_BRG_DCTQ_Pos) |
                   ((pdiv_int_min - 1U) << USIC_CH_BRG_PDIV_Pos);

    status = CY_USIC_CH_STATUS_OK;
  }
  else
  {
    status = CY_USIC_CH_STATUS_ERROR;
  }

  return status;
}

Cy_USIC_CH_STATUS_t Cy_USIC_CH_SetBaudrateEx(Cy_USIC_CH_t *const channel, int32_t rate, int32_t oversampling)
{
  int32_t peripheral_clock = Cy_SCU_CLOCK_GetPeripheralClockFrequency();
  int32_t brg_clock = rate * oversampling;
  int32_t actual_rate_upper;
  int32_t actual_rate_lower;
  uint32_t pdiv = 1;
  uint32_t divider_step;
  Cy_USIC_CH_STATUS_t status;

  if (peripheral_clock > brg_clock)
  {
    divider_step = peripheral_clock / brg_clock; // integer division gets truncated
    while (divider_step >= 1023)
    {
      pdiv++;
      brg_clock = rate * oversampling * pdiv;
      divider_step = peripheral_clock / brg_clock; // integer division gets truncated
    }
    actual_rate_upper = peripheral_clock / (divider_step * oversampling * pdiv);
    actual_rate_lower = peripheral_clock / ((divider_step + 1) * oversampling * pdiv);

    // choose better approximation if the peripheral frequency is not a multiple of the baudrate
    if (abs(rate - actual_rate_lower) < abs(rate - actual_rate_upper))
    {
      divider_step += 1;
    }

    divider_step = 1024 - divider_step;


    channel->FDR = CY_USIC_CH_BRG_CLOCK_DIVIDER_MODE_NORMAL |
                   (divider_step << USIC_CH_FDR_STEP_Pos);

    channel->BRG = (channel->BRG & ~(USIC_CH_BRG_DCTQ_Msk |
                                     USIC_CH_BRG_PDIV_Msk |
                                     USIC_CH_BRG_PCTQ_Msk |
                                     USIC_CH_BRG_PPPEN_Msk)) |
                   ((oversampling - 1U) << USIC_CH_BRG_DCTQ_Pos) |
                   ((pdiv - 1) << USIC_CH_BRG_PDIV_Pos);

    status = CY_USIC_CH_STATUS_OK;
  }
  else
  {
    status = CY_USIC_CH_STATUS_ERROR;
  }

  return status;
}

void Cy_USIC_CH_SetBaudrateDivider(Cy_USIC_CH_t *const channel,
                                    Cy_USIC_CH_BRG_CLOCK_SOURCE_t clksel,
                                    bool pppen,
                                    uint32_t pdiv,
                                    Cy_USIC_CH_BRG_CTQSEL_t ctqsel,
                                    uint32_t pctq,
                                    uint32_t dctq)
{
    uint32_t regval = channel->BRG;
    regval &= (uint32_t)~(USIC_CH_BRG_CLKSEL_Msk | USIC_CH_BRG_PPPEN_Msk | USIC_CH_BRG_PDIV_Msk | USIC_CH_BRG_CTQSEL_Msk | USIC_CH_BRG_PCTQ_Msk | USIC_CH_BRG_DCTQ_Msk);
    regval |= clksel | (pppen ? USIC_CH_BRG_PPPEN_Msk : 0) | (pdiv << USIC_CH_BRG_PDIV_Pos) | ctqsel | (pctq << USIC_CH_BRG_PCTQ_Pos) | (dctq << USIC_CH_BRG_DCTQ_Pos);
    channel->BRG = regval;
}

uint32_t Cy_USIC_CH_GetBaudrate(Cy_USIC_CH_t *const channel)
{
  uint32_t divider;
  if ((channel->BRG & USIC_CH_BRG_CTQSEL_Msk) == USIC_CH_BRG_CTQSEL_Msk)
  {
    // CTQSEL = 3
    divider = 2;
  }
  else
  {
    // CTQSEL = 0, 1, or 2
    divider = (channel->BRG & USIC_CH_BRG_PPPEN_Msk) ? 2 : 1;

    if ((((channel->BRG & USIC_CH_BRG_CTQSEL_Msk) >> USIC_CH_BRG_CTQSEL_Pos) & 0x1) == 0)
    {
      // CTQSEL = 0 or 2
      divider *= ((channel->BRG & USIC_CH_BRG_PDIV_Msk) >> USIC_CH_BRG_PDIV_Pos) + 1;
      if ((((channel->BRG & USIC_CH_BRG_CTQSEL_Msk) >> USIC_CH_BRG_CTQSEL_Pos) & 0x2) != 0)
      {
        // CTQSEL = 2
        divider *= 2;
      }
    }
  }

  divider *= ((channel->BRG & USIC_CH_BRG_PCTQ_Msk) >> USIC_CH_BRG_PCTQ_Pos) + 1;
  divider *= ((channel->BRG & USIC_CH_BRG_DCTQ_Msk) >> USIC_CH_BRG_DCTQ_Pos) + 1;

  uint32_t fperi = Cy_SCU_CLOCK_GetPeripheralClockFrequency();
  float baudrate;
  if ((channel->FDR & USIC_CH_FDR_DM_Msk) == CY_USIC_CH_BRG_CLOCK_DIVIDER_MODE_FRACTIONAL)
  {
    baudrate = fperi * (((channel->FDR & USIC_CH_FDR_STEP_Msk) >> USIC_CH_FDR_STEP_Pos) / 1024.0F);
  }
  else
  {
    /* Normal divider mode */
    baudrate = fperi * (1.0F / (1024 - ((channel->FDR & USIC_CH_FDR_STEP_Msk) >> USIC_CH_FDR_STEP_Pos)));
  }

  baudrate /= divider;

  return (uint32_t)baudrate;
}

uint32_t Cy_USIC_CH_GetSCLKFrequency(Cy_USIC_CH_t *const channel)
{
  uint32_t divider;
  divider = (channel->BRG & USIC_CH_BRG_PPPEN_Msk) ? 2 : 1;
  divider *= ((channel->BRG & USIC_CH_BRG_PDIV_Msk) >> USIC_CH_BRG_PDIV_Pos) + 1;
  divider *= 2;

  uint32_t fperi = Cy_SCU_CLOCK_GetPeripheralClockFrequency();
  float baudrate;
  if ((channel->FDR & USIC_CH_FDR_DM_Msk) == CY_USIC_CH_BRG_CLOCK_DIVIDER_MODE_FRACTIONAL)
  {
    /* Fractional divider mode */
    baudrate = fperi * (((channel->FDR & USIC_CH_FDR_STEP_Msk) >> USIC_CH_FDR_STEP_Pos) / 1024.0F);
  }
  else
  {
    /* Normal divider mode */
    baudrate = fperi * (1.0F / (1024 - ((channel->FDR & USIC_CH_FDR_STEP_Msk) >> USIC_CH_FDR_STEP_Pos)));
  }

  baudrate /= divider;

  return (uint32_t)baudrate;
}

uint32_t Cy_USIC_CH_GetMCLKFrequency(Cy_USIC_CH_t *const channel)
{
  uint32_t fperi = Cy_SCU_CLOCK_GetPeripheralClockFrequency();

  float baudrate;
  if ((channel->FDR & USIC_CH_FDR_DM_Msk) == CY_USIC_CH_BRG_CLOCK_DIVIDER_MODE_FRACTIONAL)
  {
    /* Fractional divider mode */
    baudrate = fperi * (1.0F / (1024 - ((channel->FDR & USIC_CH_FDR_STEP_Msk) >> USIC_CH_FDR_STEP_Pos)));
  }
  else
  {
    /* Normal divider mode */
    baudrate = fperi / (((channel->FDR & USIC_CH_FDR_STEP_Msk) >> USIC_CH_FDR_STEP_Pos) / 1024.0F);
  }

  baudrate /= 2;

  return (uint32_t)baudrate;
}


void Cy_USIC_CH_ConfigExternalInputSignalToBRG(Cy_USIC_CH_t *const channel,
    const uint16_t pdiv,
    const uint32_t oversampling,
    const Cy_USIC_CH_INPUT_COMBINATION_MODE_t combination_mode)
{
  CY_ASSERT(((1U < pdiv) || (pdiv < 1024U)));
  CY_ASSERT(((1U < oversampling) || (oversampling < 32U)));

  /* Setting the external input frequency source through DX1 */
  Cy_USIC_CH_SetBRGInputClockSource(channel, CY_USIC_CH_BRG_CLOCK_SOURCE_DX1T);

  /* Setting the trigger combination mode */
  Cy_USIC_CH_SetInputTriggerCombinationMode(channel, CY_USIC_CH_INPUT_DX1, combination_mode);

  /* Configuring the dividers and oversampling */
  channel->BRG = (channel->BRG & ~(USIC_CH_BRG_DCTQ_Msk |
                                   USIC_CH_BRG_PDIV_Msk |
                                   USIC_CH_BRG_PCTQ_Msk |
                                   USIC_CH_BRG_PPPEN_Msk)) |
                 (((oversampling) - 1U) << USIC_CH_BRG_DCTQ_Pos) |
                 (((pdiv) - 1U) << USIC_CH_BRG_PDIV_Pos);
}

void Cy_USIC_CH_TXFIFO_Configure(Cy_USIC_CH_t *const channel,
                                  const uint32_t data_pointer,
                                  const Cy_USIC_CH_FIFO_SIZE_t size,
                                  const uint32_t limit)
{
  /* Disable FIFO */
  channel->TBCTR &= (uint32_t)~USIC_CH_TBCTR_SIZE_Msk;

  /* LOF = 0, A standard transmit buffer event occurs when the filling level equals the limit value and gets
   * lower due to transmission of a data word
   * STBTEN = 0, the trigger of the standard transmit buffer event is based on the transition of the fill level
   *  from equal to below the limit, not the fact being below
   */
  channel->TBCTR = (uint32_t)(channel->TBCTR & (uint32_t)~(USIC_CH_TBCTR_LIMIT_Msk |
                              USIC_CH_TBCTR_DPTR_Msk |
                              USIC_CH_TBCTR_SIZE_Msk)) |
                   (uint32_t)((limit << USIC_CH_TBCTR_LIMIT_Pos) |
                              (data_pointer << USIC_CH_TBCTR_DPTR_Pos) |
                              ((uint32_t)size << USIC_CH_TBCTR_SIZE_Pos));
}


void Cy_USIC_CH_RXFIFO_Configure(Cy_USIC_CH_t *const channel,
                                  const uint32_t data_pointer,
                                  const Cy_USIC_CH_FIFO_SIZE_t size,
                                  const uint32_t limit)
{
  /* Disable FIFO */
  channel->RBCTR &= (uint32_t)~USIC_CH_RBCTR_SIZE_Msk;

  /* LOF = 1, A standard receive buffer event occurs when the filling level equals the limit value and gets bigger
   *  due to the reception of a new data word
   */
  channel->RBCTR = (uint32_t)((channel->RBCTR & (uint32_t)~(USIC_CH_RBCTR_LIMIT_Msk |
                               USIC_CH_RBCTR_DPTR_Msk |
                               USIC_CH_RBCTR_LOF_Msk)) |
                              ((limit << USIC_CH_RBCTR_LIMIT_Pos) |
                               (data_pointer << USIC_CH_RBCTR_DPTR_Pos) |
                               ((uint32_t)size << USIC_CH_RBCTR_SIZE_Pos) |
                               (uint32_t)USIC_CH_RBCTR_LOF_Msk));
}

void Cy_USIC_CH_TXFIFO_SetSizeTriggerLimit(Cy_USIC_CH_t *const channel,
    const Cy_USIC_CH_FIFO_SIZE_t size,
    const uint32_t limit)
{
  CY_UNUSED_ARG(size);

  /* STBTEN = 0, the trigger of the standard transmit buffer event is based on the transition of the fill level
   *  from equal to below the limit, not the fact being below
   */
  channel->TBCTR = (uint32_t)((uint32_t)(channel->TBCTR & (uint32_t)~USIC_CH_TBCTR_LIMIT_Msk) |
                              (limit << USIC_CH_TBCTR_LIMIT_Pos));
}

void Cy_USIC_CH_RXFIFO_SetSizeTriggerLimit(Cy_USIC_CH_t *const channel,
    const Cy_USIC_CH_FIFO_SIZE_t size,
    const uint32_t limit)
{
  CY_UNUSED_ARG(size);

  channel->RBCTR = (uint32_t)((uint32_t)(channel->RBCTR & (uint32_t)~USIC_CH_RBCTR_LIMIT_Msk) |
                              (limit << USIC_CH_RBCTR_LIMIT_Pos));
}

void Cy_USIC_CH_SetInterruptNodePointer(Cy_USIC_CH_t *const channel,
    const Cy_USIC_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
    const uint32_t service_request)
{
  channel->INPR = (uint32_t)((channel->INPR & (~(uint32_t)(USIC_CH_INPR_Msk << (uint32_t)interrupt_node))) |
                             (service_request << (uint32_t)interrupt_node));
}

void Cy_USIC_CH_TXFIFO_SetInterruptNodePointer(Cy_USIC_CH_t *const channel,
    const Cy_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_t interrupt_node,
    const uint32_t service_request)
{
  channel->TBCTR = (uint32_t)((channel->TBCTR & (~(uint32_t)(USIC_CH_INPR_Msk << (uint32_t)interrupt_node))) |
                              (service_request << (uint32_t)interrupt_node));
}

void Cy_USIC_CH_RXFIFO_SetInterruptNodePointer(Cy_USIC_CH_t *const channel,
    const Cy_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_t interrupt_node,
    const uint32_t service_request)
{
  channel->RBCTR = (uint32_t)((channel->RBCTR & (~(uint32_t)(USIC_CH_INPR_Msk << (uint32_t)interrupt_node))) |
                              (service_request << (uint32_t)interrupt_node));
}

void Cy_USIC_Enable(Cy_USIC_t *const usic)
{
  if (usic == USIC0)
  {
#if defined(CY_IP_CLOCK_GATING)
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_USIC0);
    while (Cy_SCU_CLOCK_IsPeripheralClockGated(CY_SCU_PERIPHERAL_CLOCK_USIC0));
#endif
#if defined(CY_IP_PERIPHERAL_RESET)
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_USIC0);
    while (Cy_SCU_RESET_IsPeripheralResetAsserted(CY_SCU_PERIPHERAL_RESET_USIC0));
#endif
  }
#if defined(USIC1)
  else if (usic == USIC1)
  {
#if defined(CY_IP_CLOCK_GATING)
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_USIC1);
    while (Cy_SCU_CLOCK_IsPeripheralClockGated(CY_SCU_PERIPHERAL_CLOCK_USIC1));
#endif
#if defined(CY_IP_PERIPHERAL_RESET)
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_USIC1);
    while (Cy_SCU_RESET_IsPeripheralResetAsserted(CY_SCU_PERIPHERAL_RESET_USIC1));
#endif
  }
#endif
#if defined(USIC2)
  else if (usic == USIC2)
  {
#if defined(CY_IP_CLOCK_GATING)
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_USIC2);
    while (Cy_SCU_CLOCK_IsPeripheralClockGated(CY_SCU_PERIPHERAL_CLOCK_USIC2));
#endif
#if defined(CY_IP_PERIPHERAL_RESET)
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_USIC2);
    while (Cy_SCU_RESET_IsPeripheralResetAsserted(CY_SCU_PERIPHERAL_RESET_USIC2));
#endif
  }
#endif
  else
  {
    CY_ASSERT(0/*Always*/);
  }
}

void Cy_USIC_Disable(Cy_USIC_t *const usic)
{
  if (usic == (Cy_USIC_t *)USIC0)
  {
#if defined(CY_IP_PERIPHERAL_RESET)
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_USIC0);
#endif
#if defined(CY_IP_CLOCK_GATING)
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_USIC0);
#endif
  }
#if defined(USIC1)
  else if (usic == (Cy_USIC_t *)USIC1)
  {
#if defined(CY_IP_PERIPHERAL_RESET)
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_USIC1);
#endif
#if defined(CY_IP_CLOCK_GATING)
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_USIC1);
#endif
  }
#endif
#if defined(USIC2)
  else if (usic == (Cy_USIC_t *)USIC2)
  {
#if defined(CY_IP_PERIPHERAL_RESET)
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_USIC2);
#endif
#if defined(CY_IP_CLOCK_GATING)
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_USIC2);
#endif
  }
#endif
  else
  {
    CY_ASSERT(0/*Always*/);
  }

}
