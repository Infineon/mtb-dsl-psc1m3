/**
 * @file cy_spi.c
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
 * @brief SPI driver for PCS1M microcontroller family
 *
 */
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include "cy_scu.h"
#include "cy_spi.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define CY_SPI_CH_OVERSAMPLING (2UL)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* Initializes the selected SPI channel with the config structure. */
void Cy_SPI_CH_InitEx(Cy_USIC_CH_t *const channel, const Cy_SPI_CH_CONFIG_t *const config, bool init_brg)
{
  Cy_USIC_CH_Enable(channel);

  if ((config->bus_mode == CY_SPI_CH_BUS_MODE_MASTER) && init_brg)
  {
    /* Configure baud rate */
    if (config->normal_divider_mode)
    {
      /* Normal divider mode */
      (void)Cy_USIC_CH_SetBaudrateEx(channel, config->baudrate, CY_SPI_CH_OVERSAMPLING);
    }
    else
    {
      /* Fractional divider mode */
      (void)Cy_USIC_CH_SetBaudrate(channel, config->baudrate, CY_SPI_CH_OVERSAMPLING);
    }
  }

  /* Configuration of USIC Shift Control */
  /* Transmission Mode (TRM) = 1  */
  /* Passive Data Level (PDL) = 1 */
  channel->SCTR = USIC_CH_SCTR_PDL_Msk |
                  (0x1UL << USIC_CH_SCTR_TRM_Pos) |
                  (0x3fUL << USIC_CH_SCTR_FLE_Pos) |
                  (0x7UL << USIC_CH_SCTR_WLE_Pos);

  /* Configuration of USIC Transmit Control/Status Register */
  /* TBUF Data Enable (TDEN) = 1 */
  /* TBUF Data Single Shot Mode (TDSSM) = 1 */
  channel->TCSR = (uint32_t)(USIC_CH_TCSR_HPCMD_Msk |
                             (0x01UL  << USIC_CH_TCSR_TDEN_Pos) |
                             USIC_CH_TCSR_TDSSM_Msk);

  if (config->bus_mode == CY_SPI_CH_BUS_MODE_MASTER)
  {
    /* Configuration of Protocol Control Register */
    channel->PCR_SSCMode = (uint32_t)(USIC_CH_PCR_SSCMode_MSLSEN_Msk |
                                      USIC_CH_PCR_SSCMode_SELCTR_Msk |
                                      (uint32_t)config->selo_inversion |
                                      USIC_CH_PCR_SSCMode_FEM_Msk);
  }

  /* Clear protocol status */
  channel->PSCR = 0xFFFFFFFFUL;

  /* Set parity settings */
  channel->CCR = (uint32_t)config->parity_mode;
}

Cy_SPI_CH_STATUS_t Cy_SPI_CH_SetBaudrate(Cy_USIC_CH_t *const channel, const uint32_t rate)
{
  Cy_SPI_CH_STATUS_t status;

  status = CY_SPI_CH_STATUS_ERROR;

  if (rate <= (Cy_SCU_CLOCK_GetPeripheralClockFrequency() >> 1U))
  {
    if (Cy_USIC_CH_SetBaudrate(channel, rate, CY_SPI_CH_OVERSAMPLING) == CY_USIC_CH_STATUS_OK)
    {
      status = CY_SPI_CH_STATUS_OK;
    }
  }
  return status;
}

Cy_SPI_CH_STATUS_t Cy_SPI_CH_SetBaudrateEx(Cy_USIC_CH_t *const channel, const uint32_t rate, bool normal_divider_mode)
{
  Cy_USIC_CH_STATUS_t status;

  if (rate <= (Cy_SCU_CLOCK_GetPeripheralClockFrequency() >> 1U))
  {
    if (normal_divider_mode)
    {
      /* Normal divider mode */
      status = Cy_USIC_CH_SetBaudrateEx(channel, rate, CY_SPI_CH_OVERSAMPLING);
    }
    else
    {
      /* Fractional divider mode */
      status = Cy_USIC_CH_SetBaudrate(channel, rate, CY_SPI_CH_OVERSAMPLING);
    }
  }
  else
  {
    status = CY_USIC_CH_STATUS_ERROR;
  }

  return (Cy_SPI_CH_STATUS_t)status;
}

/* Enable the selected slave signal by setting (SELO) bits in PCR register. */
void Cy_SPI_CH_EnableSlaveSelect(Cy_USIC_CH_t *const channel, const Cy_SPI_CH_SLAVE_SELECT_t slave)
{
  /* Configuration of Protocol Control Register */
  channel->PCR_SSCMode &= (uint32_t)~USIC_CH_PCR_SSCMode_SELO_Msk;
  channel->PCR_SSCMode |= (uint32_t)slave;
}

/* Disable the slave signals by clearing (SELO) bits in PCR register. */
void Cy_SPI_CH_DisableSlaveSelect(Cy_USIC_CH_t *const channel)
{
  Cy_SPI_CH_ClearStatusFlag(channel, (uint32_t)CY_SPI_CH_STATUS_FLAG_MSLS);

  /* Configuration of Protocol Control Register */
  channel->PCR_SSCMode &= (uint32_t)~USIC_CH_PCR_SSCMode_SELO_Msk;
}

/* Puts the data into FIFO if FIFO mode is enabled or else into standard buffers, by setting the proper mode. */
void Cy_SPI_CH_Transmit(Cy_USIC_CH_t *const channel, const uint16_t data, const Cy_SPI_CH_MODE_t mode)
{

  channel->CCR = (channel->CCR & (uint32_t)(~USIC_CH_CCR_HPCEN_Msk)) |
                 (((uint32_t) mode << USIC_CH_CCR_HPCEN_Pos) & (uint32_t)USIC_CH_CCR_HPCEN_Msk);


  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
    {
    }

    Cy_SPI_CH_ClearStatusFlag(channel, (uint32_t)CY_SPI_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[mode] = data;
  }
  else
  {
    channel->IN[mode] = data;
  }
}

/* Reads the data from the buffers based on the FIFO mode selection. */
uint16_t Cy_SPI_CH_GetReceivedData(Cy_USIC_CH_t *const channel)
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

/* Configures the inter word delay by setting PCR.PCTQ1, PCR.DCTQ1 bit fields. */
void Cy_SPI_CH_SetInterwordDelay(Cy_USIC_CH_t *const channel, uint32_t tinterword_delay_us)
{
  uint32_t peripheral_clock;
  uint32_t pdiv;
  uint32_t step;
  uint32_t fFD;
  uint32_t fpdiv;
  uint32_t divider_factor1 = 0U;
  uint32_t divider_factor2 = 32U;
  uint32_t divider_factor1_int = 0U;
  uint32_t divider_factor1_int_min = 4U;
  uint32_t divider_factor1_frac_min = 100U;
  uint32_t divider_factor1_frac = 0U;
  uint32_t divider_factor2_temp = 0U;
  peripheral_clock = Cy_SCU_CLOCK_GetPeripheralClockFrequency();
  pdiv = (uint32_t)(channel->BRG & USIC_CH_BRG_PDIV_Msk) >> USIC_CH_BRG_PDIV_Pos;
  step = (uint32_t)(channel->FDR & USIC_CH_FDR_STEP_Msk) >> USIC_CH_FDR_STEP_Pos;
  fFD = (uint32_t)((peripheral_clock >> 10U) * step);
  fpdiv = fFD / (1U + pdiv);

  if (tinterword_delay_us < (128000000 / fpdiv))
  {
    for (divider_factor2_temp = 32U; divider_factor2_temp > 0U; --divider_factor2_temp)
    {

      divider_factor1 = (tinterword_delay_us * fpdiv) / (divider_factor2_temp * 10000);
      divider_factor1_frac = divider_factor1 % 100U;

      if (divider_factor1_frac > 50)
      {
        divider_factor1_int =  (divider_factor1 / 100U) + 1;
        divider_factor1_frac = (divider_factor1_int * 100) - divider_factor1;
      }
      else
      {
        divider_factor1_int =  (divider_factor1 / 100U);
      }

      if ((divider_factor1_int < 5U) && (divider_factor1_int > 0) && (divider_factor1_frac < divider_factor1_frac_min))
      {
        divider_factor1_frac_min = divider_factor1_frac;
        divider_factor1_int_min = divider_factor1_int;
        divider_factor2 = divider_factor2_temp;
      }
    }
  }

  channel->PCR_SSCMode = (uint32_t)((channel->PCR_SSCMode) & (~(USIC_CH_PCR_SSCMode_DCTQ1_Msk |
                                    USIC_CH_PCR_SSCMode_PCTQ1_Msk |
                                    USIC_CH_PCR_SSCMode_CTQSEL1_Msk))) |
                         (((divider_factor1_int_min - 1) << USIC_CH_PCR_SSCMode_PCTQ1_Pos) & (uint32_t)USIC_CH_PCR_SSCMode_PCTQ1_Msk) |
                         (((divider_factor2 - 1 ) << USIC_CH_PCR_SSCMode_DCTQ1_Pos) & (uint32_t)USIC_CH_PCR_SSCMode_DCTQ1_Msk);
}

Cy_SPI_CH_STATUS_t Cy_SPI_CH_Stop(Cy_USIC_CH_t *const channel)
{
  Cy_SPI_CH_STATUS_t status = CY_SPI_CH_STATUS_OK;

  if (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
  {
    status = CY_SPI_CH_STATUS_BUSY;
  }
  else
  {

    /* USIC channel in IDLE mode */
    Cy_USIC_CH_SetMode(channel, CY_USIC_CH_OPERATING_MODE_IDLE);
  }

  return status;
}

void Cy_SPI_CH_EnableEvent(Cy_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR |= (event & 0x1fc00U);
  channel->PCR_SSCMode |= ((event << 13U) & 0xe000U);
}

void Cy_SPI_CH_DisableEvent(Cy_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR &= (uint32_t)~(event & 0x1fc00U);
  channel->PCR_SSCMode &= (uint32_t)~((event << 13U) & 0xe000U);
}
