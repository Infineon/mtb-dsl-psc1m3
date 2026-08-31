/**
* @file cy_uart.h
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

#ifndef CY_UART_H
#define CY_UART_H

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "cy_usic.h"


/**
 * @addtogroup UART
 * @brief Universal Asynchronous Receiver/Transmitter (UART) driver for PSC1M microcontroller family.
 *
 * The UART driver uses Universal Serial Interface Channel(USIC) module to implement UART protocol.
 * It provides APIs to configure USIC channel for UART communication. The driver enables the user
 * in getting the status of UART protocol events, configuring interrupt service requests, protocol
 * related parameter configuration etc.
 *
 * UART driver features:
 * -# Configuration structure Cy_UART_CH_CONFIG_t and initialization function Cy_UART_CH_Init()
 * -# Enumeration of events with their bit masks @ref Cy_UART_CH_EVENT_t, @ref Cy_UART_CH_STATUS_FLAG_t
 * -# Allows the selection of input source for the DX0 input stage using the API Cy_UART_CH_SetInputSource()
 * -# Allows configuration of baudrate using Cy_UART_CH_SetBaudrate() and configuration of data length using
 Cy_UART_CH_SetWordLength() and Cy_UART_CH_SetFrameLength()
 * -# Provides the status of UART protocol events, Cy_UART_CH_GetStatusFlag()
 * -# Allows transmission of data using Cy_UART_CH_Transmit() and gets received data using Cy_UART_CH_GetReceivedData()
 *
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

#if defined(USIC0)
#define CY_UART0_CH0 CY_USIC0_CH0 /**< USIC0 channel 0 base address */
#define CY_UART0_CH1 CY_USIC0_CH1 /**< USIC0 channel 1 base address */
#endif

#if defined(USIC1)
#define CY_UART1_CH0 CY_USIC1_CH0 /**< USIC1 channel 0 base address */
#define CY_UART1_CH1 CY_USIC1_CH1 /**< USIC1 channel 1 base address */
#endif

#if defined(USIC2)
#define CY_UART2_CH0 CY_USIC2_CH0 /**< USIC2 channel 0 base address */
#define CY_UART2_CH1 CY_USIC2_CH1 /**< USIC2 channel 1 base address */
#endif

/*********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * UART driver status
 */
typedef enum CY_UART_CH_STATUS
{
  CY_UART_CH_STATUS_OK,     /**< UART driver status : OK*/
  CY_UART_CH_STATUS_ERROR,  /**< UART driver status : ERROR */
  CY_UART_CH_STATUS_BUSY    /**< UART driver status : BUSY */
} Cy_UART_CH_STATUS_t;

/**
* UART portocol status. The enum values can be used for getting the status of UART channel.
*
*/
typedef enum CY_UART_CH_STATUS_FLAG
{
  CY_UART_CH_STATUS_FLAG_TRANSMISSION_IDLE = USIC_CH_PSR_ASCMode_TXIDLE_Msk,                 /**< UART Protocol Status transmit IDLE*/
  CY_UART_CH_STATUS_FLAG_RECEPTION_IDLE = USIC_CH_PSR_ASCMode_RXIDLE_Msk,                    /**< UART Protocol Status receive IDLE*/
  CY_UART_CH_STATUS_FLAG_SYNCHRONIZATION_BREAK_DETECTED = USIC_CH_PSR_ASCMode_SBD_Msk,       /**< UART Protocol Status synchronization break detected*/
  CY_UART_CH_STATUS_FLAG_COLLISION_DETECTED = USIC_CH_PSR_ASCMode_COL_Msk,                   /**< UART Protocol Status collision detected*/
  CY_UART_CH_STATUS_FLAG_RECEIVER_NOISE_DETECTED = USIC_CH_PSR_ASCMode_RNS_Msk,              /**< UART Protocol Status receiver noise detected */
  CY_UART_CH_STATUS_FLAG_FORMAT_ERROR_IN_STOP_BIT_0 = USIC_CH_PSR_ASCMode_FER0_Msk,          /**< UART Protocol Status format error in stop bit 0 */
  CY_UART_CH_STATUS_FLAG_FORMAT_ERROR_IN_STOP_BIT_1 = USIC_CH_PSR_ASCMode_FER1_Msk,          /**< UART Protocol Status format error in stop bit 1 */
  CY_UART_CH_STATUS_FLAG_RECEIVE_FRAME_FINISHED = USIC_CH_PSR_ASCMode_RFF_Msk,               /**< UART Protocol Status receive frame finished */
  CY_UART_CH_STATUS_FLAG_TRANSMITTER_FRAME_FINISHED = USIC_CH_PSR_ASCMode_TFF_Msk,           /**< UART Protocol Status transmit frame finished */
  CY_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY = USIC_CH_PSR_ASCMode_BUSY_Msk,                /**< UART Protocol Status transfer status busy */
  CY_UART_CH_STATUS_FLAG_RECEIVER_START_INDICATION = USIC_CH_PSR_ASCMode_RSIF_Msk,           /**< UART Protocol Status receive start indication flag*/
  CY_UART_CH_STATUS_FLAG_DATA_LOST_INDICATION = USIC_CH_PSR_ASCMode_DLIF_Msk,                /**< UART Protocol Status data lost indication flag*/
  CY_UART_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION = USIC_CH_PSR_ASCMode_TSIF_Msk,           /**< UART Protocol Status transmit shift indication flag*/
  CY_UART_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION = USIC_CH_PSR_ASCMode_TBIF_Msk,          /**< UART Protocol Status transmit buffer indication flag*/
  CY_UART_CH_STATUS_FLAG_RECEIVE_INDICATION = USIC_CH_PSR_ASCMode_RIF_Msk,                   /**< UART Protocol Status receive indication flag*/
  CY_UART_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION = USIC_CH_PSR_ASCMode_AIF_Msk,       /**< UART Protocol Status alternative receive  indication flag*/
  CY_UART_CH_STATUS_FLAG_BAUD_RATE_GENERATOR_INDICATION = USIC_CH_PSR_ASCMode_BRGIF_Msk      /**< UART Protocol Status baudrate generator indication flag*/
} Cy_UART_CH_STATUS_FLAG_t;

/**
* UART configuration events. The enums can be used for configuring events using the CCR register.
*/
typedef enum CY_CH_UART_EVENT
{
  CY_UART_CH_EVENT_RECEIVE_START       = USIC_CH_CCR_RSIEN_Msk,  /**< Receive start event */
  CY_UART_CH_EVENT_DATA_LOST           = USIC_CH_CCR_DLIEN_Msk,  /**< Data lost event */
  CY_UART_CH_EVENT_TRANSMIT_SHIFT      = USIC_CH_CCR_TSIEN_Msk,  /**< Transmit shift event */
  CY_UART_CH_EVENT_TRANSMIT_BUFFER     = USIC_CH_CCR_TBIEN_Msk,  /**< Transmit buffer event */
  CY_UART_CH_EVENT_STANDARD_RECEIVE    = USIC_CH_CCR_RIEN_Msk,   /**< Receive event */
  CY_UART_CH_EVENT_ALTERNATIVE_RECEIVE = USIC_CH_CCR_AIEN_Msk,   /**< Alternate receive event */
  CY_UART_CH_EVENT_BAUD_RATE_GENERATOR = USIC_CH_CCR_BRGIEN_Msk, /**< Baudrate generator event */

  CY_UART_CH_EVENT_SYNCHRONIZATION_BREAK = USIC_CH_PCR_ASCMode_SBIEN_Msk, /**< Event synchronization break */
  CY_UART_CH_EVENT_COLLISION = USIC_CH_PCR_ASCMode_CDEN_Msk,              /**< Event collision */
  CY_UART_CH_EVENT_RECEIVER_NOISE = USIC_CH_PCR_ASCMode_RNIEN_Msk,        /**< Event receiver noise */
  CY_UART_CH_EVENT_FORMAT_ERROR = USIC_CH_PCR_ASCMode_FEIEN_Msk,          /**< Event format error */
  CY_UART_CH_EVENT_FRAME_FINISHED = USIC_CH_PCR_ASCMode_FFIEN_Msk         /**< Event frame finished */
} Cy_UART_CH_EVENT_t;

/**
 * UART Input sampling frequency options
 */
typedef enum CY_UART_CH_INPUT_SAMPLING_FREQ
{
  CY_UART_CH_INPUT_SAMPLING_FREQ_FPERIPH            = CY_USIC_CH_INPUT_SAMPLING_FREQ_FPERIPH,           /**< Sampling frequency input fperiph*/
  CY_UART_CH_INPUT_SAMPLING_FREQ_FRACTIONAL_DIVIDER = CY_USIC_CH_INPUT_SAMPLING_FREQ_FRACTIONAL_DIVIDER /**< Sampling frequency input fractional divider*/
} Cy_UART_CH_INPUT_SAMPLING_FREQ_t;

/**
 * UART input stages
 */
typedef enum CY_UART_CH_INPUT
{
  CY_UART_CH_INPUT_RXD = 0UL,  /**< UART input stage DX0*/
  CY_UART_CH_INPUT_RXD1 = 3UL, /**< UART input stage DX3*/
  CY_UART_CH_INPUT_RXD2 = 5UL  /**< UART input stage DX5*/
} Cy_UART_CH_INPUT_t;


/**
 * UART channel interrupt node pointers
 */
typedef enum CY_UART_CH_INTERRUPT_NODE_POINTER
{
  CY_UART_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT      = CY_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT, /**< Node pointer for transmit shift interrupt */
  CY_UART_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER     = CY_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER, /**< Node pointer for transmit buffer interrupt */
  CY_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE             = CY_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE,  /**< Node pointer for receive interrupt */
  CY_UART_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE   = CY_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,  /**< Node pointer for alternate receive interrupt */
  CY_UART_CH_INTERRUPT_NODE_POINTER_PROTOCOL            = CY_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL   /**< Node pointer for protocol related interrupts */
} Cy_UART_CH_INTERRUPT_NODE_POINTER_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/

/**
 * UART initialization structure
*/
typedef struct CY_UART_CH_CONFIG
{
  uint32_t baudrate;                         /**< Desired baudrate. \b Range: minimum= 100, maximum= (fPERIPH * 1023)/(1024 * oversampling) */
  bool normal_divider_mode;                  /**< Selects normal divider mode for baudrate generator instead of default fractional divider decreasing jitter at cost of frequency selection */
  uint8_t data_bits;                         /**< Number of bits for the data field. Value configured as USIC channel word length. \n
                                                  \b Range: minimum= 1, maximum= 16*/
  uint8_t frame_length;                      /**< Indicates nmber of bits in a frame. Configured as USIC channel frame length. \n
                                                  \b Range: minimum= 1, maximum= 63*/
  uint8_t stop_bits;                         /**< Number of stop bits. \b Range: minimum= 1, maximum= 2 */
  uint8_t oversampling;						           /**< Number of samples for a symbol(DCTQ).\b Range: minimum= 1, maximum= 32*/
  Cy_USIC_CH_PARITY_MODE_t parity_mode;     /**< Parity mode. \b Range: @ref CY_USIC_CH_PARITY_MODE_NONE, @ref CY_USIC_CH_PARITY_MODE_EVEN, \n
                                                  @ref CY_USIC_CH_PARITY_MODE_ODD*/
} Cy_UART_CH_CONFIG_t;

/*********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, CY_UART0_CH1,CY_UART1_CH0, CY_UART1_CH1,CY_UART2_CH0, CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param config Constant pointer to UART configuration structure of type @ref Cy_UART_CH_CONFIG_t.
 * @param init_brg Selects if the baudrate generator should be configured automatically.
 * @return Cy_UART_CH_STATUS_t Status of initializing the USIC channel for UART protocol.\n
 *          \b Range: @ref CY_UART_CH_STATUS_OK if initialization is successful.\n
 *                    @ref CY_UART_CH_STATUS_ERROR if configuration of baudrate failed.
 *
 * \par<b>Description</b><br>
 * Initializes the USIC channel for UART protocol.\n\n
 * During the initialization, USIC channel is enabled, baudrate is configured with the defined oversampling value
 * in the intialization structure. If the oversampling value is set to 0 in the structure, the default oversampling of 16
 * is considered. Sampling point for each symbol is configured at the half of sampling period. Symbol value is decided by the
 * majority decision among 3 samples.
 * Word length is configured with the number of data bits. If the value of \a frame_length is 0, then USIC channel frame length
 * is set to the same value as word length. If \a frame_length is greater than 0, it is set as the USIC channel frame length.
 * Parity mode is set to the value configured for \a parity_mode.
 * The USIC channel should be set to UART mode by calling the Cy_UART_CH_Start() API after the initialization.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_Start(), Cy_UART_CH_Stop(), Cy_UART_CH_Transmit()\n\n\n
 */
void Cy_UART_CH_InitEx(Cy_USIC_CH_t *channel, const Cy_UART_CH_CONFIG_t *const config, bool init_brg);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, CY_UART0_CH1,CY_UART1_CH0, CY_UART1_CH1,CY_UART2_CH0, CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param config Constant pointer to UART configuration structure of type @ref Cy_UART_CH_CONFIG_t.
 * @return Cy_UART_CH_STATUS_t Status of initializing the USIC channel for UART protocol.\n
 *          \b Range: @ref CY_UART_CH_STATUS_OK if initialization is successful.\n
 *                    @ref CY_UART_CH_STATUS_ERROR if configuration of baudrate failed.
 *
 * \par<b>Description</b><br>
 * Initializes the USIC channel for UART protocol.\n\n
 * During the initialization, USIC channel is enabled, baudrate is configured with the defined oversampling value
 * in the intialization structure. If the oversampling value is set to 0 in the structure, the default oversampling of 16
 * is considered. Sampling point for each symbol is configured at the half of sampling period. Symbol value is decided by the
 * majority decision among 3 samples.
 * Word length is configured with the number of data bits. If the value of \a frame_length is 0, then USIC channel frame length
 * is set to the same value as word length. If \a frame_length is greater than 0, it is set as the USIC channel frame length.
 * Parity mode is set to the value configured for \a parity_mode.
 * The USIC channel should be set to UART mode by calling the Cy_UART_CH_Start() API after the initialization.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_Start(), Cy_UART_CH_Stop(), Cy_UART_CH_Transmit()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_Init(Cy_USIC_CH_t *const channel, const Cy_UART_CH_CONFIG_t *const config)
{
  Cy_UART_CH_InitEx(channel, config, true);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the USIC channel operation mode to UART mode.\n\n
 * CCR register bitfield \a Mode is set to 2(UART mode). This API should be called after configuring
 * the USIC channel. Transmission and reception can happen only when the UART mode is set.
 * This is an inline function.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_Stop(), Cy_UART_CH_Transmit()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_Start(Cy_USIC_CH_t *const channel)
{
  channel->CCR = (uint32_t)(((channel->CCR) & (~USIC_CH_CCR_MODE_Msk)) | (uint32_t)CY_USIC_CH_OPERATING_MODE_UART);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return Cy_UART_CH_STATUS_t Status to indicate if the communication channel is stopped successfully.\n
 *                              @ref CY_UART_CH_STATUS_OK if the communication channel is stopped.
 *                              @ref CY_UART_CH_STATUS_BUSY if the communication channel is busy.
 *
 * \par<b>Description</b><br>
 * Stops the UART communication.\n\n
 * CCR register bitfield \a Mode is reset. This disables the communication.
 * Before starting the communication again, the channel has to be reconfigured.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_Init() \n\n\n
 */
Cy_UART_CH_STATUS_t Cy_UART_CH_Stop(Cy_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				   \b Range: @ref CY_UART0_CH0, CY_UART0_CH1 ,CY_UART1_CH0, CY_UART1_CH1, CY_UART2_CH0, CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param rate Desired baudrate. \n
 *           \b Range: minimum value = 100, maximum value depends on the peripheral clock frequency\n
 * 				   and \a oversampling. Maximum baudrate can be derived using the formula: (fperiph * 1023)/(1024 * oversampling)
 * @param  oversampling Required oversampling. The value indicates the number of time quanta for one symbol of data.\n
 * 					 This can be related to the number of samples for each logic state of the data signal.\n
 * 					 \b Range: 4 to 32. Value should be chosen based on the protocol used.
 * @return Cy_UART_CH_STATUS_t Status indicating the baudrate configuration.\n
 * 			     \b Range: @ref CY_USIC_CH_STATUS_OK if baudrate is successfully configured,
 * 					 @ref CY_USIC_CH_STATUS_ERROR if desired baudrate or oversampling is invalid.
 *
 * \par<b>Description:</b><br>
 * Sets the bus speed in bits per second.\n\n
 * Derives the values of \a STEP and PDIV to arrive at the optimum realistic speed possible.
 * \a oversampling is the number of samples to be taken for each symbol of UART protocol.
 * Default \a oversampling of 16 is considered if the input \a oversampling is less than 4. It is recommended to keep
 * a minimum oversampling of 4 for UART.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_Init(), Cy_UART_CH_Stop(), Cy_USIC_CH_GetBaudrate()
 */
Cy_UART_CH_STATUS_t Cy_UART_CH_SetBaudrate(Cy_USIC_CH_t *const channel, uint32_t rate, uint32_t oversampling);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				   \b Range: @ref CY_UART0_CH0, CY_UART0_CH1 ,CY_UART1_CH0, CY_UART1_CH1, CY_UART2_CH0, CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param rate Desired baudrate. \n
 *           \b Range: minimum value = 100, maximum value depends on the peripheral clock frequency\n
 * 				   and \a oversampling. Maximum baudrate can be derived using the formula: (fperiph * 1023)/(1024 * oversampling)
 * @param  oversampling Required oversampling. The value indicates the number of time quanta for one symbol of data.\n
 * 					 This can be related to the number of samples for each logic state of the data signal.\n
 * 					 \b Range: 4 to 32. Value should be chosen based on the protocol used.
 * @param normal_divider_mode Selects normal divider mode for baudrate generator instead of default fractional divider decreasing jitter of signal at the cost of frequency selection
 * @return Cy_UART_CH_STATUS_t Status indicating the baudrate configuration.\n
 * 			     \b Range: @ref CY_USIC_CH_STATUS_OK if baudrate is successfully configured,
 * 					 @ref CY_USIC_CH_STATUS_ERROR if desired baudrate or oversampling is invalid.
 *
 * \par<b>Description:</b><br>
 * Sets the bus speed in bits per second.\n\n
 * Derives the values of \a STEP and PDIV to arrive at the optimum realistic speed possible.
 * \a oversampling is the number of samples to be taken for each symbol of UART protocol.
 * Default \a oversampling of 16 is considered if the input \a oversampling is less than 4. It is recommended to keep
 * a minimum oversampling of 4 for UART.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_Init(), Cy_UART_CH_Stop(), Cy_USIC_CH_GetBaudrate()
 */
Cy_UART_CH_STATUS_t Cy_UART_CH_SetBaudrateEx(Cy_USIC_CH_t *const channel, uint32_t rate, uint32_t oversampling, bool normal_divider_mode);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  data Data to be transmitted. \n
 *          \b Range: 16 bit unsigned data within the range 0 to 65535. Actual size of
 *          data transmitted depends on the configured number of bits for the UART protocol in the register SCTR.
 * @return None
 *
 * \par<b>Description</b><br>
 * Transmits data over serial communication channel using UART protocol.\n\n
 * Based on the channel configuration, data is either put to the transmit FIFO or to TBUF register.
 * Before putting data to TBUF, the API waits for TBUF to finish shifting its contents to shift register.
 * So user can continuously execute the API without checking for TBUF busy status. Based on the number of
 * data bits configured, the lower significant bits will be extracted for transmission.
 *
 * \par<b>Note:</b><br>
 * When FIFO is not configured, the API waits for the TBUF to be available.
 * This makes the execution a blocking call.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_GetReceivedData() \n\n\n
 */
void Cy_UART_CH_Transmit(Cy_USIC_CH_t *const channel, const uint16_t data);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return uint16_t Received data over UART communication channel.
 * \par<b>Description</b><br>
 * Provides one word of data received over UART communication channel.\n\n
 * Based on the channel configuration, data is either read from the receive FIFO or RBUF register.
 * Before returning the value, there is no check for data validity. User should check the appropriate
 * data receive flags(standard receive/alternative receive/FIFO standard receive/FIFO alternative receive)
 * before executing the API. Reading from an empty receive FIFO can generate a receive error event.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_GetStatusFlag(), Cy_UART_CH_Transmit() \n\n\n
 */
uint16_t Cy_UART_CH_GetReceivedData(Cy_USIC_CH_t *const channel);

/**
 * @param  channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  word_length Data word length. \n
 *          \b Range: minimum= 1, maximum= 16.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the data word length in number of bits.\n\n
 * Word length can range from 1 to 16. It indicates the number of data bits in a data word.
 * The value of \a word_length will be decremented by 1 before setting the value to \a SCTR register.
 * If the UART data bits is more than 16, then the frame length should be set to the actual number of bits and
 * word length should be configured with the number of bits expected in each transaction. For example, if number of data bits
 * for UART communication is 20 bits, then the frame length should be set as 20. Word length can be set based on the
 * transmit and receive handling. If data is stored as 8bit array, then the word length can be set to 8. In this case,
 * a full message of UART data should be transmitted/ received as 3 data words.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_SetFrameLength() \n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_SetWordLength(Cy_USIC_CH_t *const channel, const uint8_t word_length)
{
  Cy_USIC_CH_SetWordLength(channel, word_length);
}

/**
 * @param  channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  frame_length Number of data bits in each UART frame. \n
 *          \b Range: minimum= 1, maximum= 64.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the number of data bits for UART communication.\n\n
 * The frame length is configured by setting the input value to \a SCTR register.
 * The value of \a frame_length will be decremented by 1, before setting it to the register.
 * Frame length should not be set to 64 for UART communication.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_SetWordLength() \n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_SetFrameLength(Cy_USIC_CH_t *const channel, const uint8_t frame_length)
{
  Cy_USIC_CH_SetFrameLength(channel, frame_length);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  event Event bitmasks to enable. Use the type @ref Cy_UART_CH_EVENT_t for naming events. \n
 * 				  \b Range: @ref CY_UART_CH_EVENT_RECEIVE_START, @ref CY_UART_CH_EVENT_DATA_LOST,
 * 				  @ref CY_UART_CH_EVENT_TRANSMIT_SHIFT, @ref CY_UART_CH_EVENT_TRANSMIT_BUFFER,
 * 				  etc.
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables interrupt events for UART communication.\n\n
 * Multiple events can be combined using the bitwise OR operation and configured in one function call.
 * @ref Cy_UART_CH_EVENT_t enumerates multiple event bitmasks. These enumerations can be used as input to the API.
 * Events are configured by setting bits in the CCR register.
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_DisableEvent(), Cy_UART_CH_SetInterruptNodePointer(), Cy_UART_CH_GetStatusFlag() \n\n\n
 */
void Cy_UART_CH_EnableEvent(Cy_USIC_CH_t *const channel, const uint32_t event);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  event Bitmask of events to disable. Use the type @ref Cy_UART_CH_EVENT_t for naming events.\n
 * 				  \b Range: @ref CY_UART_CH_EVENT_RECEIVE_START, @ref CY_UART_CH_EVENT_DATA_LOST,
 * 				  @ref CY_UART_CH_EVENT_TRANSMIT_SHIFT, @ref CY_UART_CH_EVENT_TRANSMIT_BUFFER,
 * 				  etc.
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the interrupt events by clearing the bits in CCR register.\n\n
 * Multiple events can be combined using the bitwise OR operation and configured in one function call.
 * Cy_UART_CH_EVENT_FLAG_t enumerates multiple event bitmasks. These enumerations can be used as input to the API.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_ClearStatusFlag(), Cy_UART_CH_EnableEvent() \n\n\n
 */
void Cy_UART_CH_DisableEvent(Cy_USIC_CH_t *const channel, const uint32_t event);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param service_request Service request number for generating protocol interrupts.\n
 * 				  \b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the interrupt node for UART channel protocol events.\n\n
 * For all the protocol events enlisted in the enumeration Cy_UART_CH_EVENT_t, one common
 * interrupt gets generated. The service request connects the interrupt node to the UART
 * protocol events.
 *
 * \par<b>Note:</b><br>
 * NVIC node should be separately enabled to generate the interrupt.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableEvent() \n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_SetInterruptNodePointer(Cy_USIC_CH_t *const channel,
    const uint8_t service_request)
{
  Cy_USIC_CH_SetInterruptNodePointer(channel, CY_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL,
                                      (uint32_t)service_request);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  interrupt_node Interrupt node pointer to be configured. \n
 * 						  \b Range: @ref CY_UART_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT,
 * 						  			@ref CY_UART_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER etc.
 * @param service_request Service request number.\n
 * 						  \b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the interrupt node for USIC channel events. \n\n
 * For an event to generate interrupt, node pointer should be configured with service request(SR0, SR1..SR5).
 * The NVIC node gets linked to the interrupt event by doing so.<br>
 *
 * \par<b>Note:</b><br>
 * NVIC node should be separately enabled to generate the interrupt.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableEvent() \n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_SelectInterruptNodePointer(Cy_USIC_CH_t *const channel,
    const Cy_UART_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
    const uint32_t service_request)
{
  Cy_USIC_CH_SetInterruptNodePointer(channel, (Cy_USIC_CH_INTERRUPT_NODE_POINTER_t)interrupt_node,
                                      (uint32_t)service_request);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref Cy_USIC_CH_t \n
 * 				   \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  service_request_line service request number of the event to be triggered. \n
 * 			\b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Trigger a UART interrupt service request.\n\n
 * When the UART service request is triggered, the NVIC interrupt associated with it will be
 * generated if enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_SelectInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_TriggerServiceRequest(Cy_USIC_CH_t *const channel, const uint32_t service_request_line)
{
  Cy_USIC_CH_TriggerServiceRequest(channel, (uint32_t)service_request_line);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return Status of UART channel events. \n
 *          \b Range: Use @ref Cy_UART_CH_STATUS_FLAG_t enumerations for
 * 					event bitmasks. @ref CY_UART_CH_STATUS_FLAG_TRANSMISSION_IDLE, @ref CY_UART_CH_STATUS_FLAG_RECEPTION_IDLE,
 * 					@ref CY_UART_CH_STATUS_FLAG_SYNCHRONIZATION_BREAK_DETECTED etc.
 *
 * \par<b>Description</b><br>
 * Provides the status of UART channel events.\n\n
 * Status provided by the API represents the status of multiple events at their bit positions. The bitmasks can be
 * obtained using the enumeration Cy_UART_CH_STATUS_FLAG_t. Event status is obtained by reading
 * the register PSR_ASCMode.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableEvent(),  Cy_UART_CH_ClearStatusFlag()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_UART_CH_GetStatusFlag(Cy_USIC_CH_t *const channel)
{
  return channel->PSR_ASCMode;
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param flag UART events to be cleared. \n
 *          \b Range: Use @ref Cy_UART_CH_STATUS_FLAG_t enumerations for
 * 					event bitmasks. @ref CY_UART_CH_STATUS_FLAG_TRANSMISSION_IDLE, @ref CY_UART_CH_STATUS_FLAG_RECEPTION_IDLE,
 * 					@ref CY_UART_CH_STATUS_FLAG_SYNCHRONIZATION_BREAK_DETECTED etc.
 * @return None
 *
 * \par<b>Description</b><br>
 * Clears the status of UART channel events.\n\n
 * Multiple events can be combined using the bitwise OR operation and configured in one function call.
 * Cy_UART_CH_STATUS_FLAG_t enumerates multiple event bitmasks. These enumerations can be used as input to the API.
 * Events are cleared by setting the bitmask to the PSCR register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_DisableEvent(),  Cy_UART_CH_GetStatusFlag()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_ClearStatusFlag(Cy_USIC_CH_t *const channel, const uint32_t flag)
{
  channel->PSCR = flag;
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref Cy_UART_CH_INPUT_t. \n
 *          \b Range: @ref CY_UART_CH_INPUT_RXD (for DX0),
 * 					@ref CY_UART_CH_INPUT_RXD1 (for DX3), @ref CY_UART_CH_INPUT_RXD2 (for DX5).
 * @param source Input source select for the input stage. The table provided below maps the decimal value with the input source.
 * <table><tr><td>0</td><td>DXnA</td></tr><tr><td>1</td><td>DXnB</td></tr><tr><td>2</td><td>DXnC</td></tr><tr><td>3</td><td>DXnD</td></tr>
 * <tr><td>4</td><td>DXnE</td></tr><tr><td>5</td><td>DXnF</td></tr><tr><td>6</td><td>DXnG</td></tr><tr><td>7</td><td>Always 1</td>
 * </tr></table>
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets input soource for the UART communication.\n\n
 * It is used for configuring the input stage for data reception.
 * Selects the input data signal source among DXnA, DXnB.. DXnG for the input stage.
 * The API can be used for the input stages DX0, DX3 and DX5.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableInputInversion() \n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_SetInputSource(Cy_USIC_CH_t *const channel, const Cy_UART_CH_INPUT_t input, const uint8_t source)
{
  channel->DXCR[input] = (uint32_t)(channel->DXCR[input] & (~(USIC_CH_DX0CR_INSW_Msk | USIC_CH_DX0CR_DSEN_Msk)));
  Cy_USIC_CH_SetInputSource(channel, (Cy_USIC_CH_INPUT_t)input, source);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param pulse_length Length of the zero pulse in number of time quanta. \n
 *          \b Range: 0 to 7.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the length of zero pulse in number of time quanta. Value 0 indicates one time quanta.\n\n
 * Maximum possible is 8 time quanta with the value configured as 7.\n
 * The value is set to PCR_ASCMode register.
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableInputInversion(), Cy_UART_CH_SetSamplePoint() \n\n\n
 *
*/
__STATIC_INLINE void Cy_UART_CH_SetPulseLength(Cy_USIC_CH_t *const channel, const uint8_t pulse_length)
{
  channel->PCR_ASCMode = (uint32_t)(channel->PCR_ASCMode & (~USIC_CH_PCR_ASCMode_PL_Msk)) |
                         ((uint32_t)pulse_length << USIC_CH_PCR_ASCMode_PL_Pos);
}


/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the generation of the master clock MCLK.\n\n
 * \par<b>Related APIs:</b><BR>
 * Cy_USIC_CH_SetMclkOutputPassiveLevel()\n\n\n
 *
*/
__STATIC_INLINE void Cy_UART_CH_EnableMasterClock(Cy_USIC_CH_t *const channel)
{
  channel->PCR_ASCMode |= USIC_CH_PCR_ASCMode_MCLK_Msk;
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the generation of the master clock MCLK.\n\n
 * \par<b>Related APIs:</b><BR>
 * Cy_USIC_CH_SetMclkOutputPassiveLevel()\n\n\n
 *
*/
__STATIC_INLINE void Cy_UART_CH_DisableMasterClock(Cy_USIC_CH_t *const channel)
{
  channel->PCR_ASCMode &= (uint32_t)~USIC_CH_PCR_ASCMode_MCLK_Msk;
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param sample_point Sample point among the number of samples. \n
 * 				  \b Range: minimum= 0, maximum= \a oversampling (DCTQ).
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the sample point among the multiple samples for each UART symbol.\n\n
 * The sample point is the one sample among number of samples set as oversampling. The value should be less than
 * the oversampling value. Cy_UART_CH_Init() sets the sample point to the sample at the centre. For
 * example if the oversampling is 16, then the sample point is set to 9.
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableInputInversion(), Cy_UART_CH_SetSamplePoint() \n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_SetSamplePoint(Cy_USIC_CH_t *const channel, const uint32_t sample_point)
{
  channel->PCR_ASCMode = (uint32_t)((channel->PCR_ASCMode & (uint32_t)(~USIC_CH_PCR_ASCMode_SP_Msk)) |
                                    (sample_point << USIC_CH_PCR_ASCMode_SP_Pos));
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref Cy_UART_CH_INPUT_t. \n
 *          \b Range: @ref CY_UART_CH_INPUT_RXD (for DX0),
 * 					@ref CY_UART_CH_INPUT_RXD1 (for DX3), @ref CY_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables input inversion for UART input data signal.\n\n
 * Polarity of the input source can be changed to provide inverted data input.
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_DisableInputInversion()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_EnableInputInversion(Cy_USIC_CH_t *const channel, const Cy_UART_CH_INPUT_t input)
{
  Cy_USIC_CH_EnableInputInversion(channel, (Cy_USIC_CH_INPUT_t)input);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref Cy_UART_CH_INPUT_t. \n
 *          \b Range: @ref CY_UART_CH_INPUT_RXD (for DX0),
 * 					@ref CY_UART_CH_INPUT_RXD1 (for DX3), @ref CY_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables input inversion for UART input data signal.\n\n
 * Resets the input data polarity for the UART input data signal.
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableInputInversion()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_DisableInputInversion(Cy_USIC_CH_t *const channel, const Cy_UART_CH_INPUT_t input)
{
  Cy_USIC_CH_DisableInputInversion(channel, (Cy_USIC_CH_INPUT_t)input);
}
/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref Cy_UART_CH_INPUT_t. \n
 *          \b Range: @ref CY_UART_CH_INPUT_RXD (for DX0),
 * 					@ref CY_UART_CH_INPUT_RXD1 (for DX3), @ref CY_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the digital filter for UART input stage.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_DisableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_EnableInputDigitalFilter(Cy_USIC_CH_t *const channel, const Cy_UART_CH_INPUT_t input)
{
  Cy_USIC_CH_EnableInputDigitalFilter(channel, (Cy_USIC_CH_INPUT_t)input);
}
/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref Cy_UART_CH_INPUT_t. \n
 *          \b Range: @ref CY_UART_CH_INPUT_RXD (for DX0),
 * 					@ref CY_UART_CH_INPUT_RXD1 (for DX3), @ref CY_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the digital filter for UART input stage.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_DisableInputDigitalFilter(Cy_USIC_CH_t *const channel, const Cy_UART_CH_INPUT_t input)
{
  Cy_USIC_CH_DisableInputDigitalFilter(channel, (Cy_USIC_CH_INPUT_t)input);
}
/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref Cy_UART_CH_INPUT_t. \n
 *          \b Range: @ref CY_UART_CH_INPUT_RXD (for DX0),
 * 					@ref CY_UART_CH_INPUT_RXD1 (for DX3), @ref CY_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 * \par<b>Description</b><br>
 * Enables synchronous input for the UART input stage.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_DisableInputSync(), Cy_UART_CH_EnableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_EnableInputSync(Cy_USIC_CH_t *const channel, const Cy_UART_CH_INPUT_t input)
{
  Cy_USIC_CH_EnableInputSync(channel, (Cy_USIC_CH_INPUT_t)input);
}
/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref Cy_UART_CH_INPUT_t. \n
 *          \b Range: @ref CY_UART_CH_INPUT_RXD (for DX0),
 * 					@ref CY_UART_CH_INPUT_RXD1 (for DX3), @ref CY_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables synchronous input for the UART input stage.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableInputSync(), Cy_UART_CH_EnableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_DisableInputSync(Cy_USIC_CH_t *const channel, const Cy_UART_CH_INPUT_t input)
{
  Cy_USIC_CH_DisableInputSync(channel, (Cy_USIC_CH_INPUT_t)input);
}
/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref Cy_UART_CH_INPUT_t. \n
 *          \b Range: @ref CY_UART_CH_INPUT_RXD (for DX0),
 * 					@ref CY_UART_CH_INPUT_RXD1 (for DX3), @ref CY_UART_CH_INPUT_RXD2 (for DX5).
 * @param sampling_freq Input sampling frequency. \n
 *          \b Range: @ref CY_UART_CH_INPUT_SAMPLING_FREQ_FPERIPH, @ref CY_UART_CH_INPUT_SAMPLING_FREQ_FRACTIONAL_DIVIDER.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the sampling frequency for the UART input stage.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableInputSync(), Cy_UART_CH_EnableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_SetInputSamplingFreq(Cy_USIC_CH_t *const channel,
    const Cy_UART_CH_INPUT_t input,
    const Cy_UART_CH_INPUT_SAMPLING_FREQ_t sampling_freq)
{
  Cy_USIC_CH_SetInputSamplingFreq(channel, (Cy_USIC_CH_INPUT_t)input, (Cy_USIC_CH_INPUT_SAMPLING_FREQ_t)sampling_freq);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 *          \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enable data transmission.\n\n
 * Use this function in combination with Cy_UART_CH_DisableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using Cy_USIC_CH_TXFIFO_PutData().
 *
 * \par<b>Note:</b><br>
 * If you need more control over the start of transmission use Cy_USIC_CH_SetStartTransmisionMode()
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_DisableDataTransmission()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_EnableDataTransmission(Cy_USIC_CH_t *const channel)
{
  Cy_USIC_CH_SetStartTransmisionMode(channel, CY_USIC_CH_START_TRANSMISION_ON_TDV);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 *          \b Range: @ref CY_UART0_CH0, @ref CY_UART0_CH1,@ref CY_UART1_CH0,@ref CY_UART1_CH1,@ref CY_UART2_CH0,@ref CY_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disable data transmission.\n\n
 * Use this function in combination with Cy_UART_CH_EnableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using Cy_USIC_CH_TXFIFO_PutData().
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_UART_CH_EnableDataTransmission()\n\n\n
 */
__STATIC_INLINE void Cy_UART_CH_DisableDataTransmission(Cy_USIC_CH_t *const channel)
{
  Cy_USIC_CH_SetStartTransmisionMode(channel, CY_USIC_CH_START_TRANSMISION_DISABLED);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */


#endif
