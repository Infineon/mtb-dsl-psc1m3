/**
 * @file cy_i2s.c
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
/**
 *
 * @brief I2S driver for PSC1M microcontroller family
 *
 */
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include "cy_scu.h"
#include "cy_i2s.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
/* To take into account the SCLK divider by 2 and the PPPEN divider (see Divider Mode Counter figure in RM) */
#define CY_I2S_CH_OVERSAMPLING (4UL)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* Initializes the selected I2S channel with the config structure. */
void Cy_I2S_CH_InitEx(Cy_USIC_CH_t *const channel, const Cy_I2S_CH_CONFIG_t *const config, bool init_brg)
{
  CY_ASSERT((config->data_delayed_sclk_periods  > 0U) &&
             (config->data_delayed_sclk_periods  < config->frame_length));
  Cy_USIC_CH_Enable(channel);

  if ((config->bus_mode == CY_I2S_CH_BUS_MODE_MASTER) && init_brg)
  {
    /* Configure baud rate */
    (void)Cy_I2S_CH_SetBaudrateEx(channel, config->baudrate, config->normal_divider_mode);
  }
  /* Configuration of USIC Shift Control */
  /* Transmission Mode (TRM) = 1  */
  channel->SCTR = (uint32_t)(0x3UL << USIC_CH_SCTR_TRM_Pos) |
                  (uint32_t)((config->frame_length - 1U) << USIC_CH_SCTR_FLE_Pos) |
                  (uint32_t)((config->data_bits - 1U) << USIC_CH_SCTR_WLE_Pos) |
                  USIC_CH_SCTR_SDIR_Msk;

  /* Configuration of USIC Transmit Control/Status Register */
  /* TBUF Data Enable (TDEN) = 1 */
  /* TBUF Data Single Shot Mode (TDSSM) = 1 */
  /* WA mode enabled(WAMD) = 1 */
  channel->TCSR = (uint32_t)((channel->TCSR & (~(USIC_CH_TCSR_WLEMD_Msk |
                              USIC_CH_TCSR_SELMD_Msk |
                              USIC_CH_TCSR_FLEMD_Msk |
                              USIC_CH_TCSR_HPCMD_Msk))) |
                             USIC_CH_TCSR_WAMD_Msk |
                             (0x01UL  << USIC_CH_TCSR_TDEN_Pos) |
                             USIC_CH_TCSR_TDSSM_Msk);

  if (config->bus_mode == CY_I2S_CH_BUS_MODE_MASTER)
  {
    /* Configuration of Protocol Control Register */
    channel->PCR_IISMode = (uint32_t)USIC_CH_PCR_IISMode_WAGEN_Msk;
  }

  /* Configuration of Protocol Control Register */
  channel->PCR_IISMode |= (uint32_t)(USIC_CH_PCR_IISMode_DTEN_Msk |
                                     (uint32_t)config->wa_inversion) |
                          ((uint32_t)((uint32_t)config->data_delayed_sclk_periods - 1U) << USIC_CH_PCR_IISMode_TDEL_Pos);

  Cy_I2S_CH_SetSystemWordLength(channel, config->frame_length);

  /* Clear protocol status */
  channel->PSCR = 0xFFFFFFFFUL;
}


Cy_I2S_CH_STATUS_t Cy_I2S_CH_SetBaudrate(Cy_USIC_CH_t *const channel, const uint32_t rate)
{
  Cy_I2S_CH_STATUS_t status;

  status = CY_I2S_CH_STATUS_ERROR;

  if (rate <= (Cy_SCU_CLOCK_GetPeripheralClockFrequency() >> 1U))
  {
    if (Cy_USIC_CH_SetBaudrate(channel, rate, CY_I2S_CH_OVERSAMPLING) == CY_USIC_CH_STATUS_OK)
    {
      channel->BRG = (uint32_t)((channel->BRG & ~(USIC_CH_BRG_CTQSEL_Msk)) |
                                (0x2UL << USIC_CH_BRG_CTQSEL_Pos)) |
                     USIC_CH_BRG_PPPEN_Msk;

      status = CY_I2S_CH_STATUS_OK;
    }

  }
  return status;
}

Cy_I2S_CH_STATUS_t Cy_I2S_CH_SetBaudrateEx(Cy_USIC_CH_t *const channel, const uint32_t rate, bool normal_divider_mode)
{
  Cy_USIC_CH_STATUS_t status;

  if (rate <= (Cy_SCU_CLOCK_GetPeripheralClockFrequency() >> 1U))
  {
    if (normal_divider_mode)
    {
      status = Cy_USIC_CH_SetBaudrateEx(channel, rate, CY_I2S_CH_OVERSAMPLING);
    }
    else
    {
      status = Cy_USIC_CH_SetBaudrate(channel, rate, CY_I2S_CH_OVERSAMPLING);
    }

    if (status == CY_USIC_CH_STATUS_OK)
    {
      channel->BRG = (uint32_t)((channel->BRG & ~(USIC_CH_BRG_CTQSEL_Msk)) |
                                (0x2UL << USIC_CH_BRG_CTQSEL_Pos)) |
                     USIC_CH_BRG_PPPEN_Msk;
    }
  }
  else
  {
    status = CY_USIC_CH_STATUS_ERROR;
  }

  return (Cy_I2S_CH_STATUS_t)status;
}

void Cy_I2S_CH_SetSystemWordLength(Cy_USIC_CH_t *const channel, uint32_t sclk_cycles_system_word_length)
{
  uint32_t sclk_cycles_system_word_length_temp;
  uint8_t dctq_temp;
  uint8_t pctq_temp;
  uint8_t dctq = 1U;
  uint8_t pctq = 1U;
  uint8_t best_error = 64U;
  uint8_t error;
  CY_ASSERT((sclk_cycles_system_word_length  > 0U) && (sclk_cycles_system_word_length  < 65U));


  for (dctq_temp = 1U; dctq_temp < 33U ; dctq_temp++)
  {
    for (pctq_temp = 1U; pctq_temp < 5U ; pctq_temp++)
    {
      sclk_cycles_system_word_length_temp = ((uint32_t)dctq_temp) * ((uint32_t)pctq_temp);
      if (sclk_cycles_system_word_length_temp == sclk_cycles_system_word_length)
      {
        dctq = dctq_temp;
        pctq = pctq_temp;
        break;
      }
      if (sclk_cycles_system_word_length_temp > sclk_cycles_system_word_length)
      {
        error = (uint8_t)(sclk_cycles_system_word_length_temp  - sclk_cycles_system_word_length);
      }
      else
      {
        error = (uint8_t)(sclk_cycles_system_word_length - sclk_cycles_system_word_length_temp);
      }

      if (error < best_error)
      {
        best_error = error;
        dctq =  dctq_temp;
        pctq = pctq_temp;
      }
    }
  }
  channel->BRG = (uint32_t)((channel->BRG & ~((uint32_t)(USIC_CH_BRG_DCTQ_Msk |
                             USIC_CH_BRG_PCTQ_Msk))) |
                            (uint32_t)((uint32_t)((uint32_t)((uint32_t)dctq - 1U) << USIC_CH_BRG_DCTQ_Pos) |
                                       (uint32_t)((uint32_t)((uint32_t)pctq - 1U) << USIC_CH_BRG_PCTQ_Pos)));
}

/* Puts the data into FIFO if FIFO mode is enabled or else into standard buffers, by setting the proper mode. */
void Cy_I2S_CH_Transmit(Cy_USIC_CH_t *const channel, const uint16_t data, const Cy_I2S_CH_CHANNEL_t channel_number)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
    {
    }

    Cy_I2S_CH_ClearStatusFlag(channel, (uint32_t)CY_I2S_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[(uint32_t)channel_number << 4] = data;
  }
  else
  {
    channel->IN[(uint32_t)channel_number << 4] = data;
  }
}

/* Reads the data from the buffers based on the FIFO mode selection. */
uint16_t Cy_I2S_CH_GetReceivedData(Cy_USIC_CH_t *const channel)
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

Cy_I2S_CH_STATUS_t Cy_I2S_CH_Stop(Cy_USIC_CH_t *const channel)
{
  Cy_I2S_CH_STATUS_t status = CY_I2S_CH_STATUS_OK;

  if (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
  {
    status = CY_I2S_CH_STATUS_BUSY;
  }
  else
  {
    /* USIC channel in IDLE mode */
    Cy_USIC_CH_SetMode(channel, CY_USIC_CH_OPERATING_MODE_IDLE);
  }

  return status;
}

void Cy_I2S_CH_EnableEvent(Cy_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR |= (event & 0x1fc00U);
  channel->PCR_IISMode |= ((event >> 2U) & 0x8070U);
}

void Cy_I2S_CH_DisableEvent(Cy_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR &= (uint32_t)~(event & 0x1fc00U);
  channel->PCR_IISMode &= (uint32_t)~((event >> 2U) & 0x8070U);
}
