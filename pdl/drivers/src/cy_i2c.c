/**
 * @file cy_i2c.c
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
#include "cy_i2c.h"

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
#define Cy_I2C_7BIT_ADDR_Pos   (8U)		/**< 7-bit address position */
#define TRANSMISSION_MODE       (3U)		/**< The shift control signal is considered active
                                                 without referring to the actual signal level. Data
                                                 frame transfer is possible after each edge of the signal.*/
#define WORDLENGTH              (7U)        /**< Word length */
#define SET_TDV                 (1U)		/**< Transmission data valid */
#define CY_I2C_10BIT_ADDR_MASK (0x7C00U)   /**< Address mask for 10-bit mode */

/*********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

typedef enum CY_I2C_CH_TDF
{
  CY_I2C_CH_TDF_MASTER_SEND =         0U,
  CY_I2C_CH_TDF_SLAVE_SEND =          (uint32_t)1U << 8U,
  CY_I2C_CH_TDF_MASTER_RECEIVE_ACK =  (uint32_t)2U << 8U,
  CY_I2C_CH_TDF_MASTER_RECEIVE_NACK = (uint32_t)3U << 8U,
  CY_I2C_CH_TDF_MASTER_START =        (uint32_t)4U << 8U,
  CY_I2C_CH_TDF_MASTER_RESTART =      (uint32_t)5U << 8U,
  CY_I2C_CH_TDF_MASTER_STOP =         (uint32_t)6U << 8U
} Cy_I2C_CH_TDF_t;

typedef enum CY_I2C_CH_MAX_SPEED
{
  CY_I2C_CH_MAX_SPEED_STANDARD = 100000U,
  CY_I2C_CH_MAX_SPEED_FAST = 400000U
} Cy_I2C_CH_MAX_SPEED_t;

typedef enum CY_I2C_CH_CLOCK_OVERSAMPLING
{
  CY_I2C_CH_CLOCK_OVERSAMPLING_STANDARD = 10U,
  CY_I2C_CH_CLOCK_OVERSAMPLING_FAST     = 25U
} Cy_I2C_CH_CLOCK_OVERSAMPLINGS_t;

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/
/* Initializes the USIC channel by setting the data format, slave address, baudrate, transfer buffer */
void Cy_I2C_CH_InitEx(Cy_USIC_CH_t *const channel, const Cy_I2C_CH_CONFIG_t *const config, bool init_brg)
{
  Cy_USIC_CH_Enable(channel);

  /* Data format configuration */
  channel->SCTR = ((uint32_t)TRANSMISSION_MODE << (uint32_t)USIC_CH_SCTR_TRM_Pos) | /* Transmision mode */
                  ((uint32_t)WORDLENGTH << (uint32_t)USIC_CH_SCTR_WLE_Pos) | /* 8 data bits */
                  USIC_CH_SCTR_FLE_Msk |           /* unlimited data flow */
                  USIC_CH_SCTR_SDIR_Msk |          /* MSB shifted first */
                  USIC_CH_SCTR_PDL_Msk;            /* Passive Data Level */

  Cy_I2C_CH_SetSlaveAddress(channel, config->address);

  if (init_brg)
  {
    (void)Cy_I2C_CH_SetBaudrateEx(channel, config->baudrate, config->normal_divider_mode);
  }


  /* Enable transfer buffer */
  channel->TCSR = ((uint32_t)SET_TDV << (uint32_t)USIC_CH_TCSR_TDEN_Pos) | USIC_CH_TCSR_TDSSM_Msk;

  /* Clear status flags */
  channel->PSCR = 0xFFFFFFFFU;

  /* Disable parity generation */
  channel->CCR = 0x0U;
}

/* Sets the slave address */
void Cy_I2C_CH_SetSlaveAddress(Cy_USIC_CH_t *const channel, const uint16_t address)
{
  if ((address & CY_I2C_10BIT_ADDR_MASK) == CY_I2C_10BIT_ADDR_GROUP)
  {
    channel->PCR_IICMode = (channel->PCR_IICMode & (uint32_t)~USIC_CH_PCR_IICMode_SLAD_Msk) |
                           (address & 0x00ffU) | ((address << 1) & 0xfe00U);
  }
  else
  {
    channel->PCR_IICMode = (channel->PCR_IICMode & (uint32_t)~USIC_CH_PCR_IICMode_SLAD_Msk) |
                           ((uint32_t)address) << Cy_I2C_7BIT_ADDR_Pos;
  }
}
/* Read the slave address */
uint16_t Cy_I2C_CH_GetSlaveAddress(const Cy_USIC_CH_t *const channel)
{
  uint32_t address = channel->PCR_IICMode & (uint32_t)USIC_CH_PCR_IICMode_SLAD_Msk;

  if ((address & 0xffU) == 0U)
  {
    address = address >> Cy_I2C_7BIT_ADDR_Pos;
  }
  else
  {
    address = (address & 0xffU) | ((address >> 1) & 0x0300U);
  }

  return (uint16_t)address;
}

/* Sets the baudrate and oversampling based on standard speed or fast speed */
Cy_I2C_CH_STATUS_t Cy_I2C_CH_SetBaudrate(Cy_USIC_CH_t *const channel, uint32_t rate)
{
  Cy_I2C_CH_STATUS_t status;

  status = CY_I2C_CH_STATUS_ERROR;

  if (rate <= (uint32_t)CY_I2C_CH_MAX_SPEED_STANDARD)
  {
    channel->PCR_IICMode &= (uint32_t)~USIC_CH_PCR_IICMode_STIM_Msk;
    if (Cy_USIC_CH_SetBaudrate(channel, rate, (uint32_t)CY_I2C_CH_CLOCK_OVERSAMPLING_STANDARD) == CY_USIC_CH_STATUS_OK)
    {
      status = CY_I2C_CH_STATUS_OK;
    }
  }
  else if (rate <= (uint32_t)CY_I2C_CH_MAX_SPEED_FAST)
  {
    channel->PCR_IICMode |= (uint32_t)USIC_CH_PCR_IICMode_STIM_Msk;
    if (Cy_USIC_CH_SetBaudrate(channel, rate, (uint32_t)CY_I2C_CH_CLOCK_OVERSAMPLING_FAST) == CY_USIC_CH_STATUS_OK)
    {
      status = CY_I2C_CH_STATUS_OK;
    }
  }
  else
  {
    status = CY_I2C_CH_STATUS_ERROR;
  }

  return status;
}

/* Sets the baudrate and oversampling based on standard speed or fast speed */
Cy_I2C_CH_STATUS_t Cy_I2C_CH_SetBaudrateEx(Cy_USIC_CH_t *const channel, uint32_t rate, bool normal_divider_mode)
{
  Cy_USIC_CH_STATUS_t status;

  if (rate <= (uint32_t)CY_I2C_CH_MAX_SPEED_STANDARD)
  {
    channel->PCR_IICMode &= (uint32_t)~USIC_CH_PCR_IICMode_STIM_Msk;
    if (normal_divider_mode)
    {
      status = Cy_USIC_CH_SetBaudrateEx(channel, rate, (uint32_t)CY_I2C_CH_CLOCK_OVERSAMPLING_STANDARD);
    }
    else
    {
      /* Fractional divider mode */
      status = Cy_USIC_CH_SetBaudrate(channel, rate, (uint32_t)CY_I2C_CH_CLOCK_OVERSAMPLING_STANDARD);
    }
  }
  else if (rate <= (uint32_t)CY_I2C_CH_MAX_SPEED_FAST)
  {
    channel->PCR_IICMode |= (uint32_t)USIC_CH_PCR_IICMode_STIM_Msk;
    if (normal_divider_mode)
    {
      status = Cy_USIC_CH_SetBaudrateEx(channel, rate, (uint32_t)CY_I2C_CH_CLOCK_OVERSAMPLING_FAST);
    }
    else
    {
      /* Fractional divider mode */
      status = Cy_USIC_CH_SetBaudrate(channel, rate, (uint32_t)CY_I2C_CH_CLOCK_OVERSAMPLING_FAST);
    }
  }
  else
  {
    status = CY_USIC_CH_STATUS_ERROR;
  }

  return (Cy_I2C_CH_STATUS_t)status;
}

/* Sends master start condition along with read/write command to IN/TBUF register based on FIFO/non-FIFO modes. */
void Cy_I2C_CH_MasterStart(Cy_USIC_CH_t *const channel, const uint16_t addr, const Cy_I2C_CH_CMD_t command)
{
  uint32_t temp;

  temp = addr | (uint32_t)CY_I2C_CH_TDF_MASTER_START;
  if (command == CY_I2C_CH_CMD_READ)
  {
    temp |= 0x1U;
  }

  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
    {
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    Cy_I2C_CH_ClearStatusFlag(channel, (uint32_t)CY_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = temp;
  }
  else
  {
    channel->IN[0U] = temp;
  }
}
/* Sends master repeated start condition along with read/write command to IN/TBUF register based on FIFO/non-FIFO modes. */
void Cy_I2C_CH_MasterRepeatedStart(Cy_USIC_CH_t *const channel, const uint16_t addr, const Cy_I2C_CH_CMD_t command)
{
  uint32_t tmp;
  tmp = addr | (uint32_t)CY_I2C_CH_TDF_MASTER_RESTART;
  if (command == CY_I2C_CH_CMD_READ)
  {
    tmp |= 0x1U;
  }

  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
    {
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    Cy_I2C_CH_ClearStatusFlag(channel, (uint32_t)CY_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = tmp;
  }
  else
  {
    channel->IN[0U] = tmp;
  }
}

/* Sends master stop command to IN/TBUF register based on FIFO/non-FIFO modes. */
void Cy_I2C_CH_MasterStop(Cy_USIC_CH_t *const channel)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
    {
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    Cy_I2C_CH_ClearStatusFlag(channel, (uint32_t)CY_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = (uint32_t)CY_I2C_CH_TDF_MASTER_STOP;
  }
  else
  {
    channel->IN[0U] = (uint32_t)CY_I2C_CH_TDF_MASTER_STOP;
  }
}

/* Sends master send command along with data to IN/TBUF register based on FIFO/non-FIFO modes. */
void Cy_I2C_CH_MasterTransmit(Cy_USIC_CH_t *const channel, const uint8_t data)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
    {
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    Cy_I2C_CH_ClearStatusFlag(channel, (uint32_t)CY_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = (uint32_t)CY_I2C_CH_TDF_MASTER_SEND | data;
  }
  else
  {
    channel->IN[0] = (uint32_t)CY_I2C_CH_TDF_MASTER_SEND | data;
  }
}

/* Sends slave send command along with data to IN/TBUF register based on FIFO/non-FIFO modes. */
void Cy_I2C_CH_SlaveTransmit(Cy_USIC_CH_t *const channel, const uint8_t data)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
    {
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    Cy_I2C_CH_ClearStatusFlag(channel, (uint32_t)CY_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = (uint32_t)CY_I2C_CH_TDF_SLAVE_SEND | data;
  }
  else
  {
    channel->IN[0] = (uint32_t)CY_I2C_CH_TDF_SLAVE_SEND | data;
  }
}

/* Sends master receive ack command to IN/TBUF register based on FIFO/non-FIFO modes. */
void Cy_I2C_CH_MasterReceiveAck(Cy_USIC_CH_t *const channel)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
    {
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    Cy_I2C_CH_ClearStatusFlag(channel, (uint32_t)CY_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = (uint32_t)CY_I2C_CH_TDF_MASTER_RECEIVE_ACK;
  }
  else
  {
    channel->IN[0] = (uint32_t)CY_I2C_CH_TDF_MASTER_RECEIVE_ACK;
  }
}

/* Sends master receive nack command to IN/TBUF register based on FIFO/non-FIFO modes. */
void Cy_I2C_CH_MasterReceiveNack(Cy_USIC_CH_t *const channel)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
    {
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    Cy_I2C_CH_ClearStatusFlag(channel, (uint32_t)CY_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = (uint32_t)CY_I2C_CH_TDF_MASTER_RECEIVE_NACK;
  }
  else
  {
    channel->IN[0] = (uint32_t)CY_I2C_CH_TDF_MASTER_RECEIVE_NACK;
  }
}

/* Reads the data from RBUF if FIFO size is 0 otherwise from OUTR. */
uint8_t Cy_I2C_CH_GetReceivedData(const Cy_USIC_CH_t *const channel)
{
  uint8_t retval;

  /* Check FIFO size */
  if ((channel->RBCTR & USIC_CH_RBCTR_SIZE_Msk) == 0U)
  {
    retval = (uint8_t)channel->RBUF;
  }
  else
  {
    retval = (uint8_t)channel->OUTR;
  }

  return retval;
}

/* Sets the operating mode of USIC to IDLE */
Cy_I2C_CH_STATUS_t Cy_I2C_CH_Stop(Cy_USIC_CH_t *const channel)
{
  Cy_I2C_CH_STATUS_t status = CY_I2C_CH_STATUS_OK;

  if (Cy_USIC_CH_GetTransmitBufferStatus(channel) == CY_USIC_CH_TBUF_STATUS_BUSY)
  {
    status = CY_I2C_CH_STATUS_BUSY;
  }
  else
  {
    /* USIC channel in IDLE mode */
    Cy_USIC_CH_SetMode(channel, CY_USIC_CH_OPERATING_MODE_IDLE);
  }
  return status;
}

void Cy_I2C_CH_EnableEvent(Cy_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR |= (event & 0x1fc00U);
  channel->PCR_IICMode |= ((event) & 0x41fc0000U);
}

void Cy_I2C_CH_DisableEvent(Cy_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR &= (uint32_t)~(event & 0x1fc00U);
  channel->PCR_IICMode &= (uint32_t)~((event) & 0x41fc0000U);
}
