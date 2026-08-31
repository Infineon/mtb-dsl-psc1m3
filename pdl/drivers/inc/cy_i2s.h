/**
 * @file cy_i2s.h
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

#ifndef CY_I2S_H_
#define CY_I2S_H_

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include "cy_usic.h"

/**
 * @addtogroup I2S
 * @brief (IIS) driver for the PSC1M microcontroller family.
 *
 * USIC IIS Features: <br>
 * @{
 */

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#if defined(USIC0)
#define CY_I2S0_CH0 CY_USIC0_CH0                   /**< USIC0 channel 0 base address */
#define CY_I2S0_CH1 CY_USIC0_CH1                   /**< USIC0 channel 1 base address */
#endif

#if defined(USIC1)
#define CY_I2S1_CH0 CY_USIC1_CH0                   /**< USIC1 channel 0 base address */
#define CY_I2S1_CH1 CY_USIC1_CH1                   /**< USIC1 channel 1 base address */
#endif

#if defined(USIC2)
#define CY_I2S2_CH0 CY_USIC2_CH0                   /**< USIC2 channel 0 base address */
#define CY_I2S2_CH1 CY_USIC2_CH1                   /**< USIC2 channel 1 base address */
#endif
/*******************************************************************************
 * ENUMS
 *******************************************************************************/

/**
 * @brief I2S Status
 */
typedef enum CY_I2S_CH_STATUS
{
  CY_I2S_CH_STATUS_OK,      /**< Status OK */
  CY_I2S_CH_STATUS_ERROR,   /**< Status ERROR */
  CY_I2S_CH_STATUS_BUSY     /**< Status BUSY */
} Cy_I2S_CH_STATUS_t;

/**
 * @brief I2S status flag
 */
typedef enum CY_I2S_CH_STATUS_FLAG
{
  CY_I2S_CH_STATUS_FLAG_WORD_ADDRESS = USIC_CH_PSR_IISMode_WA_Msk,                     /**< Word Address status */
  CY_I2S_CH_STATUS_FLAG_DX2S = USIC_CH_PSR_IISMode_DX2S_Msk,                           /**< Status of WA input(DX2) signal*/
  CY_I2S_CH_STATUS_FLAG_DX2T_EVENT_DETECTED = USIC_CH_PSR_IISMode_DX2TEV_Msk,          /**< Status for WA input signal transition */
  CY_I2S_CH_STATUS_FLAG_WA_FALLING_EDGE_EVENT = USIC_CH_PSR_IISMode_WAFE_Msk,          /**< Falling edge of the WA output
                                                                                             signal has been generated */
  CY_I2S_CH_STATUS_FLAG_WA_RISING_EDGE_EVENT = USIC_CH_PSR_IISMode_WARE_Msk,           /**< Rising edge of the WA output
                                                                                             signal has been generated */
  CY_I2S_CH_STATUS_FLAG_WA_GENERATION_END = USIC_CH_PSR_IISMode_END_Msk,               /**< The WA generation has ended */
  CY_I2S_CH_STATUS_FLAG_RECEIVER_START_INDICATION = USIC_CH_PSR_IISMode_RSIF_Msk,      /**< Receive start indication status */
  CY_I2S_CH_STATUS_FLAG_DATA_LOST_INDICATION = USIC_CH_PSR_IISMode_DLIF_Msk,           /**< Data lost indication status */
  CY_I2S_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION = USIC_CH_PSR_IISMode_TSIF_Msk,      /**< Transmit shift indication status */
  CY_I2S_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION = USIC_CH_PSR_IISMode_TBIF_Msk,     /**< Transmit buffer indication status */
  CY_I2S_CH_STATUS_FLAG_RECEIVE_INDICATION = USIC_CH_PSR_IISMode_RIF_Msk,              /**< Receive indication status */
  CY_I2S_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION = USIC_CH_PSR_IISMode_AIF_Msk,  /**< Alternate receive indication status */
  CY_I2S_CH_STATUS_FLAG_BAUD_RATE_GENERATOR_INDICATION = USIC_CH_PSR_IISMode_BRGIF_Msk /**< Baud rate generator indication status */
} Cy_I2S_CH_STATUS_FLAG_t;

/**
 *  @brief I2S Baudrate Generator shift clock output
*/
typedef enum CY_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT
{
  CY_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK = CY_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK, /**< Baudrate Generator shift clock output: SCLK*/
  CY_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT_DX1  = CY_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_DX1   /**< Clock obtained as input from master: DX1*/
} Cy_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT_t;

/**
 *  @brief I2S channel interrupt node pointers
 */
typedef enum CY_I2S_CH_INTERRUPT_NODE_POINTER
{
  CY_I2S_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT    = CY_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT,     /**< Node pointer for transmit shift interrupt */
  CY_I2S_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER   = CY_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER,    /**< Node pointer for transmit buffer interrupt */
  CY_I2S_CH_INTERRUPT_NODE_POINTER_RECEIVE           = CY_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE,            /**< Node pointer for receive interrupt */
  CY_I2S_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE = CY_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,  /**< Node pointer for alternate receive interrupt */
  CY_I2S_CH_INTERRUPT_NODE_POINTER_PROTOCOL          = CY_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL            /**< Node pointer for protocol related interrupts */
} Cy_I2S_CH_INTERRUPT_NODE_POINTER_t;

/**
 * @brief I2S events
 */
typedef enum CY_I2S_CH_EVENT
{
  CY_I2S_CH_EVENT_RECEIVE_START       = USIC_CH_CCR_RSIEN_Msk,  /**< Receive start event */
  CY_I2S_CH_EVENT_DATA_LOST           = USIC_CH_CCR_DLIEN_Msk,  /**< Data lost event */
  CY_I2S_CH_EVENT_TRANSMIT_SHIFT      = USIC_CH_CCR_TSIEN_Msk,  /**< Transmit shift event */
  CY_I2S_CH_EVENT_TRANSMIT_BUFFER     = USIC_CH_CCR_TBIEN_Msk,  /**< Transmit buffer event */
  CY_I2S_CH_EVENT_STANDARD_RECEIVE    = USIC_CH_CCR_RIEN_Msk,   /**< Receive event */
  CY_I2S_CH_EVENT_ALTERNATIVE_RECEIVE = USIC_CH_CCR_AIEN_Msk,   /**< Alternate receive event */
  CY_I2S_CH_EVENT_BAUD_RATE_GENERATOR = USIC_CH_CCR_BRGIEN_Msk, /**< Baudrate generator event */

  CY_I2S_CH_EVENT_WA_FALLING_EDGE     = USIC_CH_PCR_IISMode_WAFEIEN_Msk << 2U,  /**< WA falling edge event */
  CY_I2S_CH_EVENT_WA_RISING_EDGE      = USIC_CH_PCR_IISMode_WAREIEN_Msk << 2U,  /**< WA rising edge event */
  CY_I2S_CH_EVENT_WA_GENERATION_END   = USIC_CH_PCR_IISMode_ENDIEN_Msk << 2U,   /**< END event */
  CY_I2S_CH_EVENT_DX2TIEN_ACTIVATED   = USIC_CH_PCR_IISMode_DX2TIEN_Msk << 2U   /**< WA input signal transition event*/
} Cy_I2S_CH_EVENT_t;

/**
 * @brief Defines the Polarity of the WA in the SELO output lines in relation to the internal WA signal.
 */
typedef enum CY_I2S_CH_WA_POLARITY
{
  CY_I2S_CH_WA_POLARITY_DIRECT = 0x0UL,                                    /**< The SELO outputs have the same polarity
                                                                                  as the WA signal (active high) */
  CY_I2S_CH_WA_POLARITY_INVERTED = 0x1UL << USIC_CH_PCR_IISMode_SELINV_Pos /**< The SELO outputs have the inverted
                                                                                 polarity to the WA signal (active low)*/
} Cy_I2S_CH_WA_POLARITY_t;

/**
 * @brief Defines the Polarity of the WA in the SELO output lines in relation to the internal WA signal.
 */
typedef enum CY_I2S_CH_CHANNEL
{
  CY_I2S_CH_CHANNEL_1_LEFT = 0U,  /**< Channel 1 (left) */
  CY_I2S_CH_CHANNEL_2_RIGHT = 1U  /**< Channel 2 (right) */
} Cy_I2S_CH_CHANNEL_t;

/**
 * @brief I2S input stage selection
 */
typedef enum CY_I2S_CH_INPUT
{
  CY_I2S_CH_INPUT_DIN0 = 0UL,         /**< Data input stage 0 */
  CY_I2S_CH_INPUT_SLAVE_SCLKIN = 1UL, /**< Clock input stage */
  CY_I2S_CH_INPUT_SLAVE_WA = 2UL,     /**< WA input stage */
  CY_I2S_CH_INPUT_DIN1 = 3UL,         /**< Data input stage 1 */
  CY_I2S_CH_INPUT_DIN2 = 4UL,         /**< Data input stage 2 */
  CY_I2S_CH_INPUT_DIN3 = 5UL          /**< Data input stage 3 */
} Cy_I2S_CH_INPUT_t;

/**
 * @brief Defines the I2S bus mode
 */
typedef enum CY_I2S_CH_BUS_MODE
{
  CY_I2S_CH_BUS_MODE_MASTER, /**< I2S Master */
  CY_I2S_CH_BUS_MODE_SLAVE   /**< I2S Slave */
} Cy_I2S_CH_BUS_MODE_t;

/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/
/**
 * @brief I2S_CH configuration structure
 */
typedef struct CY_I2S_CH_CONFIG
{
  uint32_t baudrate;					             /**< Module baud rate for communication */
  bool normal_divider_mode;                /**< Selects normal divider mode for baudrate generator instead of default fractional divider decreasing jitter at cost of frequency selection */
  uint8_t data_bits;                       /**< Data word length. A data frame can consists of several data words. \n
                                                Value configured as USIC channel word length. \n
                                                  \b Range: minimum= 1, maximum= 16*/
  uint8_t frame_length;                    /**< Number of data bits transferred after a change of signal WA (data frame). \n
                                                Configured as USIC channel frame length. \n
                                                  \b Range: minimum= 1, maximum= 63*/
  uint8_t data_delayed_sclk_periods;       /**< Data delay defined in sclk periods */
  Cy_I2S_CH_WA_POLARITY_t wa_inversion;   /**< Enable inversion of Slave select signal relative to the internal WA */
  Cy_I2S_CH_BUS_MODE_t	bus_mode;          /**< Bus mode MASTER/SLAVE */
} Cy_I2S_CH_CONFIG_t;

/*******************************************************************************
 * API PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 *          \b Range: @ref CY_I2S0_CH0, CY_I2S0_CH1,CY_I2S1_CH0, CY_I2S1_CH1,CY_I2S2_CH0, CY_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @param config Constant pointer to I2S configuration structure of type @ref Cy_I2S_CH_CONFIG_t.
 * @param init_brg Selects if the baudrate generator should be configured automatically.
 * @return Cy_I2S_CH_STATUS_t Status of initializing the USIC channel for I2S protocol.\n
 *          \b Range: @ref CY_I2S_CH_STATUS_OK if initialization is successful.\n
 *                    @ref CY_I2S_CH_STATUS_ERROR if configuration of baudrate failed.
 *
 * \par<b>Description</b><br>
 * Initializes the USIC channel for I2S protocol.\n\n
 * During the initialization, USIC channel is enabled and baudrate is configured.
 * After each change of the WA signal, a complete data frame is intended to be transferred (frame length <= system word length).
 * The number of data bits transferred after a change of signal WA is defined by config->frame_length.
 * A data frame can consist of several data words with a data word length defined by config->data_bits.
 * The changes of signal WA define the system word length as the number of SCLK cycles between two changes of WA.
 * The system word length is set by default to the frame length defined by config->frame_length.
 *
 * Cy_I2S_CH_Start() should be invoked after the initialization to enable the channel.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_Start(), Cy_I2S_CH_Stop(), Cy_I2S_CH_Transmit(), Cy_I2S_CH_SetSystemWordLength()\n\n\n
 */
void Cy_I2S_CH_InitEx(Cy_USIC_CH_t *const channel, const Cy_I2S_CH_CONFIG_t *const config, bool init_brg);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 *          \b Range: @ref CY_I2S0_CH0, CY_I2S0_CH1,CY_I2S1_CH0, CY_I2S1_CH1,CY_I2S2_CH0, CY_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @param config Constant pointer to I2S configuration structure of type @ref Cy_I2S_CH_CONFIG_t.
 * @return Cy_I2S_CH_STATUS_t Status of initializing the USIC channel for I2S protocol.\n
 *          \b Range: @ref CY_I2S_CH_STATUS_OK if initialization is successful.\n
 *                    @ref CY_I2S_CH_STATUS_ERROR if configuration of baudrate failed.
 *
 * \par<b>Description</b><br>
 * Initializes the USIC channel for I2S protocol.\n\n
 * During the initialization, USIC channel is enabled and baudrate is configured.
 * After each change of the WA signal, a complete data frame is intended to be transferred (frame length <= system word length).
 * The number of data bits transferred after a change of signal WA is defined by config->frame_length.
 * A data frame can consist of several data words with a data word length defined by config->data_bits.
 * The changes of signal WA define the system word length as the number of SCLK cycles between two changes of WA.
 * The system word length is set by default to the frame length defined by config->frame_length.
 *
 * Cy_I2S_CH_Start() should be invoked after the initialization to enable the channel.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_Start(), Cy_I2S_CH_Stop(), Cy_I2S_CH_Transmit(), Cy_I2S_CH_SetSystemWordLength()\n\n\n
 */
__STATIC_INLINE void Cy_I2S_CH_Init(Cy_USIC_CH_t *const channel, const Cy_I2S_CH_CONFIG_t *const config)
{
  Cy_I2S_CH_InitEx(channel, config, true);
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the selected USIC channel to operate in I2S mode, by setting CCR.MODE bits.\n\n
 * It should be executed after Cy_I2S_CH_Init() during initialization. By invoking Cy_I2S_CH_Stop(), the MODE is set
 * to IDLE state. Call Cy_I2S_CH_Start() to set the I2S mode again, as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_Init(), Cy_I2S_CH_Stop()
 */
__STATIC_INLINE void Cy_I2S_CH_Start(Cy_USIC_CH_t *const channel)
{
  /* USIC channel in I2S mode */
  Cy_USIC_CH_SetMode(channel, CY_USIC_CH_OPERATING_MODE_I2S);
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return Cy_I2S_CH_STATUS_t Status of the I2S driver after the request for stopping is processed. \n
 *        CY_I2S_CH_STATUS_OK- If the USIC channel is successfully put to IDLE mode. \n
 *        CY_I2S_CH_STATUS_BUSY- If the USIC channel is busy transmitting data.
 *
 * \par<b>Description:</b><br>
 * Set the selected I2S channel to IDLE mode, by clearing CCR.MODE bits.\n\n
 * After calling Cy_I2S_CH_Stop, channel is IDLE mode. So no communication is supported. Cy_I2S_CH_Start() has to be
 * invoked to start the communication again.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_Start()
 */
Cy_I2S_CH_STATUS_t Cy_I2S_CH_Stop(Cy_USIC_CH_t *const channel);

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param rate Bus speed in bits per second
 *
 * @return Cy_I2S_CH_STATUS_t Status of the I2S driver after the request for setting baudrate is processed. \n
 *        CY_I2S_CH_STATUS_OK- If the baudrate is successfully changed. \n
 *        CY_I2S_CH_STATUS_ERROR- If the new baudrate value is out of range.
 *
 * \par<b>Description:</b><br>
 * Sets the bus speed in bits per second
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_Init(), Cy_I2S_CH_Stop()
 */
Cy_I2S_CH_STATUS_t Cy_I2S_CH_SetBaudrate(Cy_USIC_CH_t *const channel, const uint32_t rate);

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param rate Bus speed in bits per second
 * @param normal_divider_mode Selects normal divider mode for baudrate generator instead of default fractional divider decreasing jitter at cost of frequency selection
 *
 * @return Cy_I2S_CH_STATUS_t Status of the I2S driver after the request for setting baudrate is processed. \n
 *        CY_I2S_CH_STATUS_OK- If the baudrate is successfully changed. \n
 *        CY_I2S_CH_STATUS_ERROR- If the new baudrate value is out of range.
 *
 * \par<b>Description:</b><br>
 * Sets the bus speed in bits per second
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_Init(), Cy_I2S_CH_Stop()
 */
Cy_I2S_CH_STATUS_t Cy_I2S_CH_SetBaudrateEx(Cy_USIC_CH_t *const channel, const uint32_t rate, bool normal_divider_mode);

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param sclk_cycles_system_word_length system word length in terms of sclk clock cycles.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the system word length by setting BRG.DCTQ bit field.\n\n
 * This value has to be always higher than 1U and lower than the data with (SCTR.FLE)
 *
 */
void Cy_I2S_CH_SetSystemWordLength(Cy_USIC_CH_t *const channel, uint32_t sclk_cycles_system_word_length);

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param data Data to be transmitted
 * @param channel_number Communication output channel of the I2S, based on this channel selection TCI(Transmit control information)is updated.\n
 *             Refer @ref Cy_I2S_CH_CHANNEL_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Puts the data into FIFO, if FIFO mode is enabled or else into standard buffer, by setting the proper mode.\n\n
 * TCI(Transmit Control Information) allows dynamic control of output channel during data transfers. To support this auto
 * update, TCSR.WAMD(Automatic WA mode) will be enabled during the initialization using Cy_I2S_CH_Init() for all modes.
 *
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_Receive()
 */
void Cy_I2S_CH_Transmit(Cy_USIC_CH_t *const channel, const uint16_t data, const Cy_I2S_CH_CHANNEL_t channel_number);

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param channel_number Communication output channel of the I2S, based on this mode TCI(Transmit control information)is updated.\n
 *             Refer @ref Cy_I2S_CH_CHANNEL_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Transmits a dummy data(FFFFH) to provide clock for slave and receives the data from the slave.\n\n
 * Cy_I2S_CH_Receive() receives the data and places it into buffer based on the FIFO selection. After reception of data
 * Cy_I2S_CH_GetReceivedData() can be invoked to read the data from the buffers.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_GetReceivedData()
 */
__STATIC_INLINE void Cy_I2S_CH_Receive(Cy_USIC_CH_t *const channel, const Cy_I2S_CH_CHANNEL_t channel_number)
{
  /* Transmit dummy data */
  Cy_I2S_CH_Transmit(channel, (uint16_t)0xffffU, channel_number);
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return uint16_t Data read from the receive buffer.
 *
 * \par<b>Description:</b><br>
 * Reads data from the receive buffer based on the FIFO selection.\n\n
 * Invocation of Cy_I2S_CH_Receive() receives the data and place it into receive buffer. After receiving the data
 * Cy_I2S_CH_GetReceivedData() can be used to read the data from the buffer.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_Receive()
 */
uint16_t Cy_I2S_CH_GetReceivedData(Cy_USIC_CH_t *const channel);

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the order of data transfer from LSB to MSB, by clearing SCTR.SDIR bit.\n\n
 * This is typically based on the slave settings. Invoke Cy_I2S_CH_SetBitOrderLsbFirst() to set direction as needed in
 * the program.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_SetBitOrderMsbFirst()
 */
__STATIC_INLINE void Cy_I2S_CH_SetBitOrderLsbFirst(Cy_USIC_CH_t *const channel)
{
  channel->SCTR &= (uint32_t)~USIC_CH_SCTR_SDIR_Msk;
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the order of data transfer from MSB to LSB, by setting SCTR.SDIR bit.\n\n
 * This is typically based on the slave settings. This is not set during Cy_I2S_CH_Init().
 * Invoke Cy_I2S_CH_SetBitOrderMsbFirst() to set direction as needed in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_SetBitOrderLsbFirst()
 */
__STATIC_INLINE void Cy_I2S_CH_SetBitOrderMsbFirst(Cy_USIC_CH_t *const channel)
{
  channel->SCTR |= (uint32_t)USIC_CH_SCTR_SDIR_Msk;
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param event Protocol events which have to be enabled.
 *              Refer @ Cy_I2S_CH_EVENT_t for valid values. <b>OR</b> combinations of these enum items can be used
 *              as input.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the I2S protocol specific events, by configuring PCR register.\n\n
 * Events can be enabled as needed using Cy_I2S_CH_EnableEvent().
 * Cy_I2S_CH_DisableEvent() can be used to disable the events.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_DisableEvent()
 */
void Cy_I2S_CH_EnableEvent(Cy_USIC_CH_t *const channel, const uint32_t event);

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param event Protocol events which have to be disabled.
 *              Refer @ Cy_I2S_CH_EVENT_t for valid values. <b>OR</b> combinations of these enum item can be used
 *              as input.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the I2S protocol specific events, by configuring PCR register.\n\n
 * After disabling the events, Cy_I2S_CH_EnableEvent() has to be invoked to re-enable the events.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_EnableEvent()
 */
void Cy_I2S_CH_DisableEvent(Cy_USIC_CH_t *const channel, const uint32_t event);

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return uint32_t Status of I2S protocol events.
 *
 * \par<b>Description:</b><br>
 * Returns the status of the events, by reading PSR register.\n\n
 * This indicates the status of the all the events, for I2S communication.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_ClearStatusFlag()
 */
__STATIC_INLINE uint32_t Cy_I2S_CH_GetStatusFlag(Cy_USIC_CH_t *const channel)
{
  return channel->PSR_IISMode;
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param flag Protocol event status to be cleared for detection of next occurence.
 *              Refer @ Cy_I2S_CH_STATUS_FLAG_t for valid values. <b>OR</b> combinations of these enum item can be used
 *              as input.
 * @return None
 *
 * \par<b>Description:</b><br>
 * Clears the events specified, by setting PSCR register.\n\n
 * During communication the events occurred have to be cleared to detect their next occurence.\n
 * e.g: During transmission Transmit buffer event occurs to indicating data word transfer has started. This
 *       event has to be cleared after transmission of each data word. Otherwise next event cannot be recognized.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_GetStatusFlag()
 */
__STATIC_INLINE void Cy_I2S_CH_ClearStatusFlag(Cy_USIC_CH_t *const channel, const uint32_t flag)
{
  channel->PSCR |= flag;
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the generation of Master clock by setting PCR.MCLK bit.\n\n
 * This clock can be used as a clock reference for external devices. This is not enabled during initialization in
 * Cy_I2S_CH_Init(). Invoke Cy_I2S_CH_EnableMasterClock() to enable as needed in the program, or if it is disabled by
 * Cy_I2S_CH_DisableMasterClock().
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_DisableMasterClock()
 */
__STATIC_INLINE void Cy_I2S_CH_EnableMasterClock(Cy_USIC_CH_t *const channel)
{
  channel->PCR_IISMode |= (uint32_t)USIC_CH_PCR_IISMode_MCLK_Msk;
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the generation of Master clock by clearing PCR.MCLK bit.\n\n
 * This clock can be enabled by invoking Cy_I2S_CH_EnableMasterClock() as needed in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_EnableMasterClock()
 */
__STATIC_INLINE void Cy_I2S_CH_DisableMasterClock(Cy_USIC_CH_t *const channel)
{
  channel->PCR_IISMode &= (uint32_t)~USIC_CH_PCR_IISMode_MCLK_Msk;
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param clock_output shift clock source.\n
 *                     Refer @ref Cy_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT_t for valid inputs.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the shift clock source by setting BRG.SCLKOSEL.\n\n
 * In Master mode operation, shift clock is generated by the internal baud rate generator. This SCLK is made available
 * for external slave devices by SCLKOUT signal.\n
 * In Slave mode, the signal is received from the external master. So the DX1(input) stage has to be connected to input.\n
 */
__STATIC_INLINE void Cy_I2S_CH_ConfigureShiftClockOutput(Cy_USIC_CH_t *const channel,
    const Cy_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT_t clock_output)
{
  Cy_USIC_CH_ConfigureShiftClockOutput(channel, (Cy_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_t)0U,
                                        (Cy_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_t)clock_output);
}

/**
 * @param  channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param  word_length Number of bits to be configured for a data word. \n
 *         \b Range: 1 to 16.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Defines the data word length.\n\n
 * Sets the number of bits to represent a data word. Frame length should be a multiple of word length.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_SetFrameLength()
 */
__STATIC_INLINE void Cy_I2S_CH_SetWordLength(Cy_USIC_CH_t *const channel, const uint8_t word_length)
{
  Cy_USIC_CH_SetWordLength(channel, word_length);
}

/**
 * @param  channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param  frame_length Number of bits in a frame. \n
 *                \b Range: 1 to 64.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Define the data frame length.\n\n
 * Set the number of bits to be serially transmitted in a frame.
 * The frame length should be multiples of word length.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_USIC_CH_SetWordLength()
 */
__STATIC_INLINE void Cy_I2S_CH_SetFrameLength(Cy_USIC_CH_t *const channel, const uint8_t frame_length)
{
  Cy_USIC_CH_SetFrameLength(channel, frame_length);
}

/**
 * @param  channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param  input I2S channel input stage.\n
 *               Refer @ref Cy_I2S_CH_INPUT_t for valid values
 * @param  source Input source select for the input stage.
 *                Range : [0 to 7]
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Selects the data source for I2S input stage, by configuring DXCR.DSEL bits.\n\n
 * Selects the input data signal source among DXnA, DXnB.. DXnG for the input stage. The API can be used for all the
 * input stages like DX0CR, DX1CR etc. This is not done during initialization. This has to be configured before starting
 * the I2S communication.
 */
__STATIC_INLINE void Cy_I2S_CH_SetInputSource(Cy_USIC_CH_t *const channel,
    const Cy_I2S_CH_INPUT_t input,
    const uint8_t source)
{
  channel->DXCR[input] = (uint32_t)(channel->DXCR[input] & (~USIC_CH_DX0CR_DSEN_Msk)) | USIC_CH_DX0CR_INSW_Msk;
  Cy_USIC_CH_SetInputSource(channel, (Cy_USIC_CH_INPUT_t)input, source);
}

/**
 * @param  channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param  wa_inversion Polarity of the word address signal.\n
 *               Refer @ref Cy_I2S_CH_WA_POLARITY_t for valid values
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Set the polarity of the word address signal, by configuring PCR.SELINV bit.\n\n
 * Normally WA signal is active low level signal. This is configured
 * in Cy_I2S_CH_Init() during initialization. Invoke Cy_I2S_CH_WordAddressSignalPolarity() with desired settings as
 * needed later in the program.
 */
__STATIC_INLINE void Cy_I2S_CH_WordAddressSignalPolarity(Cy_USIC_CH_t *const channel,
    const Cy_I2S_CH_WA_POLARITY_t wa_inversion)
{
  /* Configuration of Protocol Control Register */
  channel->PCR_IISMode = (uint32_t)((channel->PCR_IISMode & (~USIC_CH_PCR_IISMode_SELINV_Msk)) | (uint32_t)wa_inversion);
}

/**
 * @param  channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param  input I2S channel input stage.\n
 *               Refer @ref Cy_I2S_CH_INPUT_t for valid inputs.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the polarity inversion of input data signal, by setting DXyCR.DPOL(where y = \a input).\n\n
 * This is not set in Cy_I2S_CH_Init(). Invoke Cy_I2S_CH_EnableInputInversion() as needed later in the program. To
 * disable the inversion Cy_I2S_CH_DisableInputInversion() can be invoked.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_DisableInputInversion()
 */
__STATIC_INLINE void Cy_I2S_CH_EnableInputInversion(Cy_USIC_CH_t *const channel,
    const Cy_I2S_CH_INPUT_t input)
{
  Cy_USIC_CH_EnableInputInversion(channel, (Cy_USIC_CH_INPUT_t)input);
}

/**
 * @param  channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param  input I2S channel input stage.\n
 *               Refer @ref Cy_I2S_CH_INPUT_t for valid inputs.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the polarity inversion of input data signal, by clearing DXyCR.DPOL(where y = \a input).\n\n
 * Resets the input data polarity. Invoke Cy_I2S_CH_EnableInputInversion() to apply inversion.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_EnableInputInversion()
 */
__STATIC_INLINE void Cy_I2S_CH_DisableInputInversion(Cy_USIC_CH_t *const channel,
    const Cy_I2S_CH_INPUT_t input)
{
  Cy_USIC_CH_DisableInputInversion(channel, (Cy_USIC_CH_INPUT_t)input);
}

/**
 * @param  channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 * @param service_request Service request number.
                          Range: [0 to 5]
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the interrupt node for I2S channel events.\n\n
 * For an event to generate interrupt, node pointer should be configured with service request(SR0, SR1..SR5).
 * The NVIC node gets linked to the interrupt event by doing so. This is not configured in Cy_I2S_CH_Init() during
 * initialization.
 *
 * \par<b>Note::</b><BR>
 * 1. NVIC node should be separately enabled to generate the interrupt.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_USIC_CH_EnableEvent()
 */
__STATIC_INLINE void Cy_I2S_CH_SetInterruptNodePointer(Cy_USIC_CH_t *const channel,
    const uint8_t service_request)
{
  Cy_USIC_CH_SetInterruptNodePointer(channel, CY_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL, (uint32_t)service_request);
}


/**
 * @param channel Pointer to USIC channel handler of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_I2S0_CH0, @ref CY_I2S0_CH1,@ref CY_I2S1_CH0,@ref CY_I2S1_CH1,@ref CY_I2S2_CH0,@ref CY_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @param  interrupt_node Interrupt node pointer to be configured. \n
 * 						  \b Range: @ref CY_I2S_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT,
 * 						  			@ref CY_I2S_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER etc.
 * @param service_request Service request number.\n
 * 						  \b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the interrupt node for USIC channel events. \n\n
 * For an event to generate interrupt, node pointer should be configured with service request(SR0, SR1..SR5).
 * The NVIC node gets linked to the interrupt event by doing so.<br>
 * Note: NVIC node should be separately enabled to generate the interrupt.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_EnableEvent() \n\n\n
 */
__STATIC_INLINE void Cy_I2S_CH_SelectInterruptNodePointer(Cy_USIC_CH_t *const channel,
    const Cy_I2S_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
    const uint32_t service_request)
{
  Cy_USIC_CH_SetInterruptNodePointer(channel, (Cy_USIC_CH_INTERRUPT_NODE_POINTER_t)interrupt_node,
                                      (uint32_t)service_request);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref Cy_USIC_CH_t \n
 * 				   \b Range: @ref CY_I2S0_CH0, @ref CY_I2S0_CH1,@ref CY_I2S1_CH0,@ref CY_I2S1_CH1,@ref CY_I2S2_CH0,@ref CY_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @param  service_request_line service request number of the event to be triggered. \n
 * 			\b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Trigger a I2S interrupt service request.\n\n
 * When the I2S service request is triggered, the NVIC interrupt associated with it will be
 * generated if enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_SelectInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void Cy_I2S_CH_TriggerServiceRequest(Cy_USIC_CH_t *const channel, const uint32_t service_request_line)
{
  Cy_USIC_CH_TriggerServiceRequest(channel, (uint32_t)service_request_line);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref Cy_USIC_CH_t \n
 * 				   \b Range: @ref CY_I2S0_CH0, @ref CY_I2S0_CH1,@ref CY_I2S1_CH0,@ref CY_I2S1_CH1,@ref CY_I2S2_CH0,@ref CY_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables delay compensation. \n\n
 *
 * Delay compensation can be applied to the receive path.
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_DisableDelayCompensation()\n\n\n
 */
__STATIC_INLINE void Cy_I2S_CH_EnableDelayCompensation(Cy_USIC_CH_t *const channel)
{
  Cy_USIC_CH_EnableDelayCompensation(channel);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref Cy_USIC_CH_t \n
 * 				   \b Range: @ref CY_I2S0_CH0, @ref CY_I2S0_CH1,@ref CY_I2S1_CH0,@ref CY_I2S1_CH1,@ref CY_I2S2_CH0,@ref CY_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables delay compensation.. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_EnableDelayCompensation()\n\n\n
 */
__STATIC_INLINE void Cy_I2S_CH_DisableDelayCompensation(Cy_USIC_CH_t *const channel)
{
  Cy_USIC_CH_DisableDelayCompensation(channel);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 *          \b Range: @ref CY_I2S0_CH0, @ref CY_I2S0_CH1,@ref CY_I2S1_CH0,@ref CY_I2S1_CH1,@ref CY_I2S2_CH0,@ref CY_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enable data transmission.\n\n
 * Use this function in combination with Cy_I2S_CH_DisableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using Cy_USIC_CH_TXFIFO_PutData().
 * @note If you need more control over the start of transmission use Cy_USIC_CH_SetStartTransmisionMode()
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_DisableDataTransmission()\n\n\n
 */
__STATIC_INLINE void Cy_I2S_CH_EnableDataTransmission(Cy_USIC_CH_t *const channel)
{
  Cy_USIC_CH_SetStartTransmisionMode(channel, CY_USIC_CH_START_TRANSMISION_ON_TDV);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 *          \b Range: @ref CY_I2S0_CH0, @ref CY_I2S0_CH1,@ref CY_I2S1_CH0,@ref CY_I2S1_CH1,@ref CY_I2S2_CH0,@ref CY_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disable data transmission.\n\n
 * Use this function in combination with Cy_I2S_CH_EnableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using Cy_USIC_CH_TXFIFO_PutData().
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2S_CH_EnableDataTransmission()\n\n\n
 */
__STATIC_INLINE void Cy_I2S_CH_DisableDataTransmission(Cy_USIC_CH_t *const channel)
{
  Cy_USIC_CH_SetStartTransmisionMode(channel, CY_USIC_CH_START_TRANSMISION_DISABLED);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* CY_I2S_H_ */
