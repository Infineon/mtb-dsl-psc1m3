/**
 * @file cy_i2c.h
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

#ifndef CY_I2C_H
#define CY_I2C_H

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include "cy_usic.h"


/**
 * @addtogroup I2C
 * @brief Inter Integrated Circuit(IIC) driver for the PSC1M microcontroller family.
 *
 * USIC IIC Features: <br>
 *  * Two-wire interface, with one line for shift clock transfer and synchronization (shift clock SCL), the other one for the data transfer (shift data SDA) <br>
 *	* Communication in standard mode (100 kBit/s) or in fast mode (up to 400 kBit/s) <br>
 *	* Support of 7-bit addressing, as well as 10-bit addressing <br>
 *	* Master mode operation, where the IIC controls the bus transactions and provides the clock signal. <br>
 *	* Slave mode operation, where an external master controls the bus transactions and provides the clock signal.<br>
 *	* Multi-master mode operation, where several masters can be connected to the bus and bus arbitration can take place, i.e. the IIC module can be master or slave. <br>
	  The master/slave operation of an IIC bus participant can change from frame to frame. <br>
 *	* Efficient frame handling (low software effort), also allowing DMA transfers <br>
 *	* Powerful interrupt handling due to multitude of indication flags <br>
 * @{
 */

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#if defined(USIC0)
#define CY_I2C0_CH0 CY_USIC0_CH0                   /**< USIC0 channel 0 base address */
#define CY_I2C0_CH1 CY_USIC0_CH1                   /**< USIC0 channel 1 base address */
#endif

#if defined(USIC1)
#define CY_I2C1_CH0 CY_USIC1_CH0                   /**< USIC1 channel 0 base address */
#define CY_I2C1_CH1 CY_USIC1_CH1                   /**< USIC1 channel 1 base address */
#endif

#if defined(USIC2)
#define CY_I2C2_CH0 CY_USIC2_CH0                   /**< USIC2 channel 0 base address */
#define CY_I2C2_CH1 CY_USIC2_CH1                   /**< USIC2 channel 1 base address */
#endif

#define CY_I2C_10BIT_ADDR_GROUP       (0x7800U)	 /**< Value to verify the address is 10-bit or not */

/*******************************************************************************
 * ENUMS
 *******************************************************************************/

/**
 * @brief I2C Status
 */
typedef enum CY_I2C_CH_STATUS
{
  CY_I2C_CH_STATUS_OK,      /**< Status OK */
  CY_I2C_CH_STATUS_ERROR,   /**< Status ERROR */
  CY_I2C_CH_STATUS_BUSY     /**< Status BUSY */
} Cy_I2C_CH_STATUS_t;

/**
 * @brief I2C status
 */
typedef enum CY_I2C_CH_STATUS_FLAG
{
  CY_I2C_CH_STATUS_FLAG_SLAVE_SELECT = USIC_CH_PSR_IICMode_SLSEL_Msk,                     /**< Slave select status */
  CY_I2C_CH_STATUS_FLAG_WRONG_TDF_CODE_FOUND = USIC_CH_PSR_IICMode_WTDF_Msk,              /**< Wrong TDF status */
  CY_I2C_CH_STATUS_FLAG_START_CONDITION_RECEIVED = USIC_CH_PSR_IICMode_SCR_Msk,           /**< Start condition received status */
  CY_I2C_CH_STATUS_FLAG_REPEATED_START_CONDITION_RECEIVED = USIC_CH_PSR_IICMode_RSCR_Msk, /**< Repeated start condition received status */
  CY_I2C_CH_STATUS_FLAG_STOP_CONDITION_RECEIVED = USIC_CH_PSR_IICMode_PCR_Msk,            /**< Stop condition received status */
  CY_I2C_CH_STATUS_FLAG_NACK_RECEIVED = USIC_CH_PSR_IICMode_NACK_Msk,                     /**< NACK received status */
  CY_I2C_CH_STATUS_FLAG_ARBITRATION_LOST = USIC_CH_PSR_IICMode_ARL_Msk,                   /**< Arbitration lost status */
  CY_I2C_CH_STATUS_FLAG_SLAVE_READ_REQUESTED = USIC_CH_PSR_IICMode_SRR_Msk,               /**< Slave read requested status */
  CY_I2C_CH_STATUS_FLAG_ERROR = USIC_CH_PSR_IICMode_ERR_Msk,                              /**< Error status */
  CY_I2C_CH_STATUS_FLAG_ACK_RECEIVED = USIC_CH_PSR_IICMode_ACK_Msk,                       /**< ACK received status */
  CY_I2C_CH_STATUS_FLAG_RECEIVER_START_INDICATION = USIC_CH_PSR_IICMode_RSIF_Msk,         /**< Receive start indication status */
  CY_I2C_CH_STATUS_FLAG_DATA_LOST_INDICATION = USIC_CH_PSR_IICMode_DLIF_Msk,              /**< Data lost indication status */
  CY_I2C_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION = USIC_CH_PSR_IICMode_TSIF_Msk,         /**< Transmit shift indication status */
  CY_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION = USIC_CH_PSR_IICMode_TBIF_Msk,        /**< Transmit buffer indication status */
  CY_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION = USIC_CH_PSR_IICMode_RIF_Msk,                 /**< Receive indication status */
  CY_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION = USIC_CH_PSR_IICMode_AIF_Msk,     /**< Alternate receive indication status */
  CY_I2C_CH_STATUS_FLAG_BAUD_RATE_GENERATOR_INDICATION = USIC_CH_PSR_IICMode_BRGIF_Msk    /**< Baud rate generator indication status */
} Cy_I2C_CH_STATUS_FLAG_t;

/**
 * @brief I2C receiver status. The received data byte is available at the bit
 * positions RBUF[7:0], whereas the additional information is monitored at the bit positions
*  RBUF[12:8].
 */
typedef enum CY_I2C_CH_RECEIVER_STATUS_FLAG
{
  CY_I2C_CH_RECEIVER_STATUS_FLAG_ACK = 0x1U,      /**< Bit 8:  Value of Received Acknowledgement bit */
  CY_I2C_CH_RECEIVER_STATUS_FLAG_FIN = 0x2U,      /**< Bit 9:  A 1 at this bit position indicates that after a (repeated) start condition
                                                                followed by the address reception the first data byte of a new frame has
                                                                been received. A 0 at this bit position indicates further data bytes */
  CY_I2C_CH_RECEIVER_STATUS_FLAG_MODE = 0x4U,     /**< Bit 10: A 0 at this bit position indicates that the data byte has been received
                                                                when the device has been in slave mode, whereas a 1 indicates a reception in master mode */
  CY_I2C_CH_RECEIVER_STATUS_FLAG_ERR = 0x8U,      /**< Bit 11: A 1 at this bit position indicates an incomplete/erroneous
                                                                data byte in the receive buffer */
  CY_I2C_CH_RECEIVER_STATUS_FLAG_ADR = 0x10       /**< Bit 12: A 0 at this bit position indicates that the programmed address
                                                                has been received. A 1 indicates a general call address. */
} Cy_I2C_CH_RECEIVER_STATUS_FLAG_t;

/**
 * @brief I2C commands
 */
typedef enum CY_I2C_CH_CMD
{
  CY_I2C_CH_CMD_WRITE,                            /**< I2C Command Write */
  CY_I2C_CH_CMD_READ                              /**< I2C Command Read */
} Cy_I2C_CH_CMD_t;

/**
 * @brief I2C events
 */
typedef enum CY_I2C_CH_EVENT
{
  CY_I2C_CH_EVENT_RECEIVE_START       = USIC_CH_CCR_RSIEN_Msk,  /**< Receive start event */
  CY_I2C_CH_EVENT_DATA_LOST           = USIC_CH_CCR_DLIEN_Msk,  /**< Data lost event */
  CY_I2C_CH_EVENT_TRANSMIT_SHIFT      = USIC_CH_CCR_TSIEN_Msk,  /**< Transmit shift event */
  CY_I2C_CH_EVENT_TRANSMIT_BUFFER     = USIC_CH_CCR_TBIEN_Msk,  /**< Transmit buffer event */
  CY_I2C_CH_EVENT_STANDARD_RECEIVE    = USIC_CH_CCR_RIEN_Msk,   /**< Receive event */
  CY_I2C_CH_EVENT_ALTERNATIVE_RECEIVE = USIC_CH_CCR_AIEN_Msk,   /**< Alternate receive event */
  CY_I2C_CH_EVENT_BAUD_RATE_GENERATOR = USIC_CH_CCR_BRGIEN_Msk, /**< Baudrate generator event */

  CY_I2C_CH_EVENT_START_CONDITION_RECEIVED = USIC_CH_PCR_IICMode_SCRIEN_Msk,             /**< Start condition received event */
  CY_I2C_CH_EVENT_REPEATED_START_CONDITION_RECEIVED = USIC_CH_PCR_IICMode_RSCRIEN_Msk,   /**< Repeated start condition received event */
  CY_I2C_CH_EVENT_STOP_CONDITION_RECEIVED = USIC_CH_PCR_IICMode_PCRIEN_Msk,              /**< Stop condition received event */
  CY_I2C_CH_EVENT_NACK = USIC_CH_PCR_IICMode_NACKIEN_Msk,                                /**< NACK received event */
  CY_I2C_CH_EVENT_ARBITRATION_LOST = USIC_CH_PCR_IICMode_ARLIEN_Msk,                     /**< Arbitration lost event */
  CY_I2C_CH_EVENT_SLAVE_READ_REQUEST = USIC_CH_PCR_IICMode_SRRIEN_Msk,                   /**< Slave read request event */
  CY_I2C_CH_EVENT_ERROR = USIC_CH_PCR_IICMode_ERRIEN_Msk,	                              /**< Error condition event */
  CY_I2C_CH_EVENT_ACK = USIC_CH_PCR_IICMode_ACKIEN_Msk                                   /**< ACK received event */
} Cy_I2C_CH_EVENT_t;

/**
 * @brief I2C input stage selection
 */
typedef enum CY_I2C_CH_INPUT
{
  CY_I2C_CH_INPUT_SDA = 0U,   /**< selection of sda input stage */
  CY_I2C_CH_INPUT_SDA1 = 3U,
  CY_I2C_CH_INPUT_SDA2 = 5U,
  CY_I2C_CH_INPUT_SCL = 1U,  /**< selection of scl input stage */
  CY_I2C_CH_INPUT_SCL1 = 4U
} Cy_I2C_CH_INPUT_t;

/**
 * I2C channel interrupt node pointers
 */
typedef enum CY_I2C_CH_INTERRUPT_NODE_POINTER
{
  CY_I2C_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT      = CY_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT, /**< Node pointer for transmit shift interrupt */
  CY_I2C_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER     = CY_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER, /**< Node pointer for transmit buffer interrupt */
  CY_I2C_CH_INTERRUPT_NODE_POINTER_RECEIVE             = CY_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE,  /**< Node pointer for receive interrupt */
  CY_I2C_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE   = CY_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,  /**< Node pointer for alternate receive interrupt */
  CY_I2C_CH_INTERRUPT_NODE_POINTER_PROTOCOL            = CY_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL   /**< Node pointer for protocol related interrupts */
} Cy_I2C_CH_INTERRUPT_NODE_POINTER_t;

/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/
/**
 * @brief I2C_CH configuration structure
 */
typedef struct CY_I2C_CH_CONFIG
{
  uint32_t baudrate;   /**< baud rate configuration upto max of 400KHz */
  bool normal_divider_mode; /**< Selects normal divider mode for baudrate generator instead of default fractional divider decreasing jitter at cost of frequency selection */
  uint16_t address;    /**< slave address
                            A 7-bit address needs to be left shifted it by 1.
                            A 10-bit address needs to be ORed with CY_I2C_10BIT_ADDR_GROUP. */
} Cy_I2C_CH_CONFIG_t;

/*******************************************************************************
 * API PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param config  Constant pointer to I2C channel config structure of type @ref Cy_I2C_CH_CONFIG_t
 * @param init_brg Selects if the baudrate generator should be configured automatically.
 *
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Initializes the I2C \a channel.<br>
 *
 * \par
 * Configures the data format in SCTR register. Sets the slave address, baud rate. Enables transmit data valid, clears status flags
 * and disables parity generation.<br>
 *
 * \par<b>Related APIs:</b><br>
 * Cy_USIC_CH_Enable()\n\n
 */
void Cy_I2C_CH_InitEx(Cy_USIC_CH_t *const channel, const Cy_I2C_CH_CONFIG_t *const config, bool init_brg);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param config  Constant pointer to I2C channel config structure of type @ref Cy_I2C_CH_CONFIG_t
 *
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Initializes the I2C \a channel.<br>
 *
 * \par
 * Configures the data format in SCTR register. Sets the slave address, baud rate. Enables transmit data valid, clears status flags
 * and disables parity generation.<br>
 *
 * \par<b>Related APIs:</b><br>
 * Cy_USIC_CH_Enable()\n\n
 */
__STATIC_INLINE void Cy_I2C_CH_Init(Cy_USIC_CH_t *const channel, const Cy_I2C_CH_CONFIG_t *const config)
{
  Cy_I2C_CH_InitEx(channel, config, true);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param rate  baud rate of I2C channel
 *
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sets the rate of I2C \a channel.
 *
 * \par<b>Note:</b><br>
 * Standard over sampling is considered if rate <= 100KHz and fast over sampling is considered if rate > 100KHz.<br>
 *
 * \par<b>Related APIs:</b><br>
 * Cy_USIC_CH_SetBaudrate()\n\n
 */
Cy_I2C_CH_STATUS_t Cy_I2C_CH_SetBaudrate(Cy_USIC_CH_t *const channel, const uint32_t rate);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param rate  baud rate of I2C channel
 * @param normal_divider_mode Selects normal divider mode for baudrate generator instead of default fractional divider decreasing jitter of signal at the cost of frequency selection
 *
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sets the rate of I2C \a channel.
 *
 * \par<b>Note:</b><br>
 * Standard over sampling is considered if rate <= 100KHz and fast over sampling is considered if rate > 100KHz.<br>
 *
 * \par<b>Related APIs:</b><br>
 * Cy_USIC_CH_SetBaudrate()\n\n
 */
Cy_I2C_CH_STATUS_t Cy_I2C_CH_SetBaudrateEx(Cy_USIC_CH_t *const channel, uint32_t rate, bool normal_divider_mode);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 *
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Starts the I2C \a channel.
 *
 * \par
 * Sets the USIC input operation mode to I2C mode using CCR register.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_USIC_CH_SetMode()\n\n
 */
__STATIC_INLINE void Cy_I2C_CH_Start(Cy_USIC_CH_t *const channel)
{
  Cy_USIC_CH_SetMode(channel, CY_USIC_CH_OPERATING_MODE_I2C);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 *
 * @return @ref Cy_I2C_CH_STATUS_t<br>
 *
 * \par<b>Description:</b><br>
 * Stops the I2C \a channel.<br>
 *
 * \par
 * Sets the USIC input operation to IDLE mode using CCR register.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_USIC_CH_SetMode()\n\n
 */
Cy_I2C_CH_STATUS_t Cy_I2C_CH_Stop(Cy_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param service_request Service request number in the range of 0-5
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sets the interrupt node for protocol interrupt.<br>
 *
 * \par
 * To generate interrupt for an event, node pointer should be configured with service request number(SR0, SR1..SR5).
 * The NVIC node gets linked to the interrupt event by doing so.<br>
 *
 * \par<b>Note:</b><br>
 * NVIC node should be separately enabled to generate the interrupt. After setting the node pointer, desired event must be enabled.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_EnableEvent(), NVIC_SetPriority(), NVIC_EnableIRQ(), Cy_I2C_CH_SetInputSource()<br>
 */
__STATIC_INLINE void Cy_I2C_CH_SetInterruptNodePointer(Cy_USIC_CH_t *const channel,
    const uint8_t service_request)
{
  Cy_USIC_CH_SetInterruptNodePointer(channel, CY_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL, service_request);
}


/**
 * @param channel Pointer to USIC channel handler of type @ref Cy_USIC_CH_t \n
 * 				  \b Range: @ref CY_I2C0_CH0, @ref CY_I2C0_CH1,@ref CY_I2C1_CH0,@ref CY_I2C1_CH1,@ref CY_I2C2_CH0,@ref CY_I2C2_CH1 @note Availability of I2C1 and I2C2 depends on device selection
 * @param  interrupt_node Interrupt node pointer to be configured. \n
 * 						  \b Range: @ref CY_I2C_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT,
 * 						  			@ref CY_I2C_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER etc.
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
 * Cy_I2C_CH_EnableEvent() \n\n\n
 */
__STATIC_INLINE void Cy_I2C_CH_SelectInterruptNodePointer(Cy_USIC_CH_t *const channel,
    const Cy_I2C_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
    const uint32_t service_request)
{
  Cy_USIC_CH_SetInterruptNodePointer(channel, (Cy_USIC_CH_INTERRUPT_NODE_POINTER_t)interrupt_node,
                                      (uint32_t)service_request);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref Cy_USIC_CH_t \n
 * 				   \b Range: @ref CY_I2C0_CH0, @ref CY_I2C0_CH1,@ref CY_I2C1_CH0,@ref CY_I2C1_CH1,@ref CY_I2C2_CH0,@ref CY_I2C2_CH1 @note Availability of I2C1 and I2C2 depends on device selection
 * @param  service_request_line service request number of the event to be triggered. \n
 * 			\b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Trigger a I2C interrupt service request.\n\n
 * When the I2C service request is triggered, the NVIC interrupt associated with it will be
 * generated if enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2C_CH_SelectInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void Cy_I2C_CH_TriggerServiceRequest(Cy_USIC_CH_t *const channel, const uint32_t service_request_line)
{
  Cy_USIC_CH_TriggerServiceRequest(channel, (uint32_t)service_request_line);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param input I2C channel input stage of type @ref Cy_I2C_CH_INPUT_t
 * @param source Input source select for the input stage(0->DX0A, 1->DX1A, .. 7->DX7G)
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sets the input source for I2C \a channel.<br>
 * Defines the input stage for the corresponding input line.
 *
 * @note After configuring the input source for corresponding channel, interrupt node pointer is set.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_USIC_CH_SetInptSource(), Cy_USIC_CH_SetInterruptNodePointer()
 *
 */
__STATIC_INLINE void Cy_I2C_CH_SetInputSource(Cy_USIC_CH_t *const channel, const Cy_I2C_CH_INPUT_t input, const uint8_t source)
{
  channel->DXCR[input] =  (uint32_t)(channel->DXCR[input] & (~USIC_CH_DX0CR_INSW_Msk)) | USIC_CH_DX0CR_DSEN_Msk;
  Cy_USIC_CH_SetInputSource(channel, (Cy_USIC_CH_INPUT_t)input, source);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param address I2C slave address
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sets the I2C \a channel slave address.<br>
 *
 * \par
 * Address is set in PCR_IICMode register by checking if it is in 10-bit address group or 7-bit address group.
 * (If first five bits of address are assigned with 0xF0, then address mode is 10-bit mode otherwise it is 7-bit mode)\n
 * @note A 7-bit address should include an additional bit at the LSB for read/write indication. For example, address 0x05 should
 * be provided as 0x0a. A 10-bit address should be provided with the identifier 0b11110xx at the most significant bits. For example,
 * address 0x305 should be provided as 0x7b05(bitwise OR with 0x7800).
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_GetSlaveAddress()\n\n
 */
void Cy_I2C_CH_SetSlaveAddress(Cy_USIC_CH_t *const channel, const uint16_t address);

/**
 * @param channel Constant pointer to USIC channel handler of type @ref Cy_USIC_CH_t
 * @return uint16_t Slave address<br>
 *
 * \par<b>Description:</b><br>
 * Gets the I2C \a channel slave address.<br>
 *
 * \par
 * Returns the address using PCR_IICMode register by checking if it is in 10-bit address group or 7-bit address group.<br>
 * (If first five bits of address are assigned with 0xF0, then address mode is considered as 10-bit mode otherwise it is 7-bit mode)\n
 * @note A 7-bit address will include an additional bit at the LSB. For example, address 0x05 will be returned as 0x0a.
 * 10-bit address will not include the 10-bit address identifier 0b11110xx at the most signifcant bits.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_SetSlaveAddress()\n\n
 */
uint16_t Cy_I2C_CH_GetSlaveAddress(const Cy_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param addr I2C master address
 * @param command read/write command
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Starts the I2C master \a channel.<br>
 *
 * \par
 * Sends the Start condition with read/write command by updating IN/TBUF register based on FIFO/non-FIFO modes.\n
 * @note Address(addr) should reserve an additional bit at the LSB for read/write indication. For example, address 0x05 should
 * be provided as 0x0a. If the address is 10-bit, only most significant bits with the 10-bit identifier should be sent using this function.
 * For example, if the 10-bit address is 0x305, the address should be provided as 0xf6(prepend with 0b11110, upper two bits of address 0b11,
 * followed by 1-bit field for read/write).
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_MasterTransmit(), Cy_USIC_CH_GetTransmitBufferStatus()\n\n
 */
void Cy_I2C_CH_MasterStart(Cy_USIC_CH_t *const channel, const uint16_t addr, const Cy_I2C_CH_CMD_t command);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param addr I2C master address
 * @param command read/write command
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sends the repeated start condition from I2C master \a channel.<br>
 *
 * \par
 * Sends the repeated start condition with read/write command by updating IN/TBUF register based on FIFO/non-FIFO modes.\n
 * @note Address(addr) should reserve an additional bit at the LSB for read/write indication. For example, address 0x05 should
 * be provided as 0x0a. If the address is 10-bit, only most significant bits with the 10-bit identifier should be sent using this function.
 * For example, if the 10-bit address is 0x305, the address should be provided as 0xf6(prepend with 0b11110, upper two bits of address 0b11,
 * followed by 1-bit field for read/write).
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_MasterTransmit(), Cy_USIC_CH_GetTransmitBufferStatus()\n\n
 */
void Cy_I2C_CH_MasterRepeatedStart(Cy_USIC_CH_t *const channel, const uint16_t addr, const Cy_I2C_CH_CMD_t command);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Stops the I2C master \a channel.<br>
 *
 * \par
  * Reads the transmit buffer status is busy or not and thereby updates IN/TBUF register based on FIFO/non-FIFO modes using Stop command.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_MasterTransmit(), Cy_USIC_CH_GetTransmitBufferStatus()\n\n
 */
void Cy_I2C_CH_MasterStop(Cy_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param data data to transmit from I2C \a channel
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Transmit the data from the I2C master \a channel.<br>
 *
 * \par
 * Reads the transmit buffer status is busy or not and thereby updates IN/TBUF register based on FIFO/non-FIFO modes using Master Send command.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_USIC_CH_GetTransmitBufferStatus()\n\n
 */
void Cy_I2C_CH_MasterTransmit(Cy_USIC_CH_t *const channel, const uint8_t data);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param data data to transmit from I2C \a channel
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Transmit the data from the I2C slave \a channel.<br>
 *
 * \par
 * Reads the transmit buffer status is busy or not and thereby updates IN/TBUF register based on FIFO/non-FIFO modes using Slave Send command.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_USIC_CH_GetTransmitBufferStatus(),Cy_I2C_CH_ClearStatusFlag()\n\n
 */
void Cy_I2C_CH_SlaveTransmit(Cy_USIC_CH_t *const channel, const uint8_t data);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sends the Ack request from I2C master \a channel.<br>
 *
 * \par
* Reads the transmit buffer status is busy or not and thereby updates IN/TBUF register based on FIFO/non-FIFO modes using Master Receive Ack command.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_MasterTransmit()\n\n
 */
void Cy_I2C_CH_MasterReceiveAck(Cy_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sends the Nack request from I2C master \a channel.<br>
 *
 * \par
 * Reads the transmit buffer status is busy or not and thereby updates IN/TBUF register based on FIFO/non-FIFO modes using Master Receive Nack command.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_MasterTransmit()\n\n
 */
void Cy_I2C_CH_MasterReceiveNack(Cy_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @return uint8_t OUTR/RBUF register data<br>
 *
 * \par<b>Description:</b><br>
 * Reads the data from I2C \a channel.<br>
 *
 * \par
 * Data is read by using OUTR/RBUF register based on FIFO/non-FIFO modes.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_MasterTransmit()\n\n
 */
uint8_t Cy_I2C_CH_GetReceivedData(const Cy_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @return uint8_t Receiver status flag<br>
 *
 * \par<b>Description:</b><br>
 * Gets the receiver status of I2C \a channel using RBUF register of bits 8-12 which gives information about receiver status.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_MasterTransmit()\n\n
 */
__STATIC_INLINE uint8_t Cy_I2C_CH_GetReceiverStatusFlag(Cy_USIC_CH_t *const channel)
{
  return ((uint8_t)((channel->RBUF) >> 8U));
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param event ORed values of @ref Cy_I2C_CH_EVENT_t enum
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Enables the input parameter @ref Cy_I2C_CH_EVENT_t event using PCR_IICMode register.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_DisableEvent()\n\n
 */
void Cy_I2C_CH_EnableEvent(Cy_USIC_CH_t *const channel, uint32_t event);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param event ORed values of @ref Cy_I2C_CH_EVENT_t enum
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Disables the input parameter @ref Cy_I2C_CH_EVENT_t event using PCR_IICMode register.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_EnableEvent()\n\n
 */
void Cy_I2C_CH_DisableEvent(Cy_USIC_CH_t *const channel, uint32_t event);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @return uint32_t Status byte<br>
 *
 * \par<b>Description:</b><br>
 * Retrieves the status byte of I2C \a channel using PSR_IICMode register.\n
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_ClearStatusFlag()\n\n
 */
__STATIC_INLINE uint32_t Cy_I2C_CH_GetStatusFlag(Cy_USIC_CH_t *const channel)
{
  return (channel->PSR_IICMode);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @param flag Status flag
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Clears the status flag of I2C \a channel by setting the input parameter \a flag in PSCR register.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_GetStatusFlag()\n\n
 */
__STATIC_INLINE void Cy_I2C_CH_ClearStatusFlag(Cy_USIC_CH_t *const channel, uint32_t flag)
{
  channel->PSCR |= flag;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref Cy_USIC_CH_t \n
 * 				   \b Range: @ref CY_I2C0_CH0, @ref CY_I2C0_CH1,@ref CY_I2C1_CH0,@ref CY_I2C1_CH1,@ref CY_I2C2_CH0,@ref CY_I2C2_CH1 @note Availability of I2C1 and I2C2 depends on device selection
 * @param  pdiv Desired divider for the external frequency input. \b Range: minimum value = 1, maximum value = 1024 \n
 * @param  oversampling Required oversampling. The value indicates the number of time quanta for one symbol of data. \n
 * 						This can be related to the number of samples for each logic state of the data signal. \n
 * 						\b Range: 1 to 32. Value should be chosen based on the protocol used.
 * @param  combination_mode  USIC channel input combination mode \n
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the external frequency input for the Baudrate Generator and configures the divider, oversampling and
 * the combination mode of the USIC channel. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_USIC_CH_SetBRGInputClockSource(), Cy_USIC_CH_SetInputTriggerCombinationMode() \n\n\n
 */
__STATIC_INLINE void Cy_I2C_CH_ConfigExternalInputSignalToBRG(Cy_USIC_CH_t *const channel,
    const uint16_t pdiv,
    const uint32_t oversampling,
    const Cy_USIC_CH_INPUT_COMBINATION_MODE_t combination_mode)
{
  Cy_USIC_CH_ConfigExternalInputSignalToBRG(channel, pdiv, oversampling, combination_mode);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @return None
 *
 * \par<b>Description:</b><br>
 * Retrieves the status byte of I2C \a channel using PSR_IICMode register.\n
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_DisableAcknowledgeAddress0()\n\n
 */
__STATIC_INLINE void Cy_I2C_CH_EnableAcknowledgeAddress0(Cy_USIC_CH_t *const channel)
{
  channel->PCR_IICMode |= USIC_CH_PCR_IICMode_ACK00_Msk;
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref Cy_USIC_CH_t
 * @return None
 *
 * \par<b>Description:</b><br>
 * This bit defines that slave device should not be sensitive to the slave address 00H.\n
 *
 * \par<b>Related APIs:</b><br>
 * Cy_I2C_CH_EnableAcknowledgeAddress0()\n\n
 */
__STATIC_INLINE void Cy_I2C_CH_DisableAcknowledgeAddress0(Cy_USIC_CH_t *const channel)
{
  channel->PCR_IICMode &= ~USIC_CH_PCR_IICMode_ACK00_Msk;
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 *          \b Range: @ref CY_I2C0_CH0, @ref CY_I2C0_CH1,@ref CY_I2C1_CH0,@ref CY_I2C1_CH1,@ref CY_I2C2_CH0,@ref CY_I2C2_CH1 @note Availability of I2C1 and I2C2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enable data transmission.\n\n
 * Use this function in combination with Cy_I2C_CH_DisableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using Cy_USIC_CH_TXFIFO_PutData().
 * @note If you need more control over the start of transmission use Cy_USIC_CH_SetStartTransmisionMode()
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2C_CH_DisableDataTransmission()\n\n\n
 */
__STATIC_INLINE void Cy_I2C_CH_EnableDataTransmission(Cy_USIC_CH_t *const channel)
{
  Cy_USIC_CH_SetStartTransmisionMode(channel, CY_USIC_CH_START_TRANSMISION_ON_TDV);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref Cy_USIC_CH_t \n
 *          \b Range: @ref CY_I2C0_CH0, @ref CY_I2C0_CH1,@ref CY_I2C1_CH0,@ref CY_I2C1_CH1,@ref CY_I2C2_CH0,@ref CY_I2C2_CH1 @note Availability of I2C1 and I2C2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disable data transmission.\n\n
 * Use this function in combination with Cy_I2C_CH_EnableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using Cy_USIC_CH_TXFIFO_PutData().
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2C_CH_EnableDataTransmission()\n\n\n
 */
__STATIC_INLINE void Cy_I2C_CH_DisableDataTransmission(Cy_USIC_CH_t *const channel)
{
  Cy_USIC_CH_SetStartTransmisionMode(channel, CY_USIC_CH_START_TRANSMISION_DISABLED);
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the generation of Master clock by setting PCR.MCLK bit.\n\n
 * This clock can be used as a clock reference for external devices. This is not enabled during initialization in
 * Cy_I2C_CH_Init(). Invoke Cy_I2C_CH_EnableMasterClock() to enable as needed in the program, or if it is disabled by
 * Cy_I2C_CH_DisableMasterClock().
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2C_CH_DisableMasterClock()
 */
__STATIC_INLINE void Cy_I2C_CH_EnableMasterClock(Cy_USIC_CH_t *const channel)
{
  channel->PCR_IICMode |= (uint32_t)USIC_CH_PCR_IICMode_MCLK_Msk;
}

/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the generation of Master clock by clearing PCR.MCLK bit.\n\n
 * This clock can be enabled by invoking Cy_I2C_CH_EnableMasterClock() as needed in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_I2C_CH_EnableMasterClock()
 */
__STATIC_INLINE void Cy_I2C_CH_DisableMasterClock(Cy_USIC_CH_t *const channel)
{
  channel->PCR_IICMode &= (uint32_t)~USIC_CH_PCR_IICMode_MCLK_Msk;
}


/**
 * @param channel A constant pointer to Cy_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * To respect the specified SDA hold time of 300 ns for standard mode and fast mode after
 * a falling edge of signal SCL, a hold delay tHDEL has been introduced. It also prevents an
 * erroneous detection of a start or a stop condition. The length of this delay can be
 * programmed by bit field PCR.HDEL. Taking into account the input sampling and output
 * update, bit field HDEL can be programmed according to:
 *  - HDEL >= 300ns x fPPP - (3 x fPPP/fPERIH) + 1   (with digital filter and HDELmin = 2)
 *  - HDEL >= 300ns x fPPP - (3 x fPPP/fPERIH) + 2   (without digital filter and HDELmin = 1)
 * If the digital input filter is used, HDEL compensates the filter delay of 2 filter periods (fPPP
 * should be used) in case of a spike on the input signal. This ensures that a data bit on the
 * SDA line changing just before the rising edge or behind the falling edge of SCL will not
 * be treated as a start or stop condition.
 */
__STATIC_INLINE void Cy_I2C_CH_SetHoldDelay(Cy_USIC_CH_t *const channel, uint32_t hdel)
{
  channel->PCR_IICMode = (channel->PCR_IICMode & (uint32_t)~USIC_CH_PCR_IICMode_HDEL_Msk) | ((hdel << USIC_CH_PCR_IICMode_HDEL_Pos) & USIC_CH_PCR_IICMode_HDEL_Msk);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
