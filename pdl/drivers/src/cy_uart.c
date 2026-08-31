/**
 * @file cy_uart.c
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
 *********************************************************************************************************************/

#include "cy_scu.h"
#include "cy_uart.h"

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

#define CY_UART_CH_OVERSAMPLING (16UL)
#define CY_UART_CH_OVERSAMPLING_MIN_VAL (4UL)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/

void Cy_UART_CH_InitEx(Cy_USIC_CH_t *channel, const Cy_UART_CH_CONFIG_t *const config, bool init_brg)
{
  uint32_t oversampling = CY_UART_CH_OVERSAMPLING;

  /* USIC channel switched on*/
  Cy_USIC_CH_Enable(channel);

  if (config->oversampling != 0U)
  {
    oversampling = (uint32_t)config->oversampling;
  }

  if (init_brg)
  {
    /* Configure baud rate */
    if (config->normal_divider_mode)
    {
      /* Normal divider mode */
      (void)Cy_USIC_CH_SetBaudrateEx(channel, config->baudrate, oversampling);
    }
    else
    {
      /* Fractional divider mode */
      (void)Cy_USIC_CH_SetBaudrate(channel, config->baudrate, oversampling);
    }
  }

  /* Configure frame format
   * Configure the number of stop bits
   * Pulse length is set to 0 to have standard UART signaling,
   * i.e. the 0 level is signaled during the complete bit time
   * Sampling point set equal to the half of the oversampling period
   * Enable Sample Majority Decision
   * Enable Transfer Status BUSY
   */
  channel->PCR_ASCMode = (uint32_t)(((config->stop_bits - 1UL) << USIC_CH_PCR_ASCMode_STPB_Pos) |
                                    (((oversampling >> 1UL) + 1UL) << USIC_CH_PCR_ASCMode_SP_Pos) |
                                    USIC_CH_PCR_ASCMode_SMD_Msk |
                                    USIC_CH_PCR_ASCMode_RSTEN_Msk | USIC_CH_PCR_ASCMode_TSTEN_Msk);

  /* Set passive data level, high
     Set word length. Data bits - 1
     If frame length is > 0, frame_lemgth-1; else, FLE = WLE (Data bits - 1)
     Transmission Mode: The shift control signal is considered active if it
     is at 1-level. This is the setting to be programmed to allow data transfers */
  channel->SCTR = (uint32_t)((((uint32_t)config->data_bits - 1UL) << USIC_CH_SCTR_WLE_Pos) |
                             ((0x1UL << USIC_CH_SCTR_TRM_Pos) | USIC_CH_SCTR_PDL_Msk));

  if (config->frame_length != 0U)
  {
    channel->SCTR |= (uint32_t)(((uint32_t)config->frame_length - 1UL) << USIC_CH_SCTR_FLE_Pos);
  }
  else
  {
    channel->SCTR |= (uint32_t)(((uint32_t)config->data_bits - 1UL) << USIC_CH_SCTR_FLE_Pos);
  }

  /* Enable transfer buffer */
  channel->TCSR = (0x1UL << USIC_CH_TCSR_TDEN_Pos) |
                  USIC_CH_TCSR_TDSSM_Msk;

  /* Clear protocol status */
  channel->PSCR = 0xFFFFFFFFUL;

  /* Set parity settings */
  channel->CCR = (uint32_t)config->parity_mode;
}

Cy_UART_CH_STATUS_t Cy_UART_CH_SetBaudrate(Cy_USIC_CH_t *const channel, uint32_t rate, uint32_t oversampling)
{
  Cy_UART_CH_STATUS_t status;

  status = CY_UART_CH_STATUS_ERROR;

  if ((rate <= (Cy_SCU_CLOCK_GetPeripheralClockFrequency() >> 2U)) && (oversampling >= CY_UART_CH_OVERSAMPLING_MIN_VAL))
  {
    if (Cy_USIC_CH_SetBaudrate(channel, rate, oversampling) == CY_USIC_CH_STATUS_OK)
    {
      status = CY_UART_CH_STATUS_OK;
    }
  }
  return status;
}

Cy_UART_CH_STATUS_t Cy_UART_CH_SetBaudrateEx(Cy_USIC_CH_t *const channel, uint32_t rate, uint32_t oversampling, bool normal_divider_mode)
{
  Cy_USIC_CH_STATUS_t status;

  if ((rate <= (Cy_SCU_CLOCK_GetPeripheralClockFrequency() >> 2U)) && (oversampling >= CY_UART_CH_OVERSAMPLING_MIN_VAL))
  {
    if (normal_divider_mode)
    {
      /* Normal divider mode */
      status = Cy_USIC_CH_SetBaudrateEx(channel, rate, oversampling);
    }
    else
    {
      /* Fractional divider mode */
      status = Cy_USIC_CH_SetBaudrate(channel, rate, oversampling);
    }
  }
  else
  {
    status = CY_USIC_CH_STATUS_ERROR;
  }

  return (Cy_UART_CH_STATUS_t)status;
}

void Cy_UART_CH_Transmit(Cy_USIC_CH_t *const channel, const uint16_t data)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0UL)
  {
    /* Wait till the Transmit Buffer is free for transmission */
    while (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
    {
    }

    /* Clear the Transmit Buffer indication flag */
    Cy_UART_CH_ClearStatusFlag(channel, (uint32_t)CY_UART_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    /*Transmit data */
    channel->TBUF[0U] = data;
  }
  else
  {
    channel->IN[0U] = data;
  }
}

uint16_t Cy_UART_CH_GetReceivedData(Cy_USIC_CH_t *const channel)
{
  uint16_t retval;

  /* Check FIFO size */
  if ((channel->RBCTR & USIC_CH_RBCTR_SIZE_Msk) == 0U)
  {
    retval = (uint16_t)channel->RBUF;
  }
  else
  {
    retval = (uint16_t)channel->OUTR;
  }

  return retval;
}

Cy_UART_CH_STATUS_t Cy_UART_CH_Stop(Cy_USIC_CH_t *const channel)
{
  Cy_UART_CH_STATUS_t status = CY_UART_CH_STATUS_OK;

  if (((Cy_USIC_CH_GetTransmitBufferStatus(channel) & (uint32_t) CY_USIC_CH_TBUF_STATUS_BUSY) != 0U) ||
      ((Cy_UART_CH_GetStatusFlag(channel) & CY_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY) != 0))
  {
    status = CY_UART_CH_STATUS_BUSY;
  }
  else
  {
    /* USIC channel in IDLE mode */
    Cy_USIC_CH_SetMode(channel, CY_USIC_CH_OPERATING_MODE_IDLE);
  }
  return status;
}

void Cy_UART_CH_EnableEvent(Cy_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR |= (event & 0x1fc00U);
  channel->PCR_ASCMode |= (event & 0xf8U);
}

void Cy_UART_CH_DisableEvent(Cy_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR &= (uint32_t)~(event & 0x1fc00U);
  channel->PCR_ASCMode &= (uint32_t)~(event & 0xf8U);
}
