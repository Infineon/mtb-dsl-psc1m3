/**
 * @file cy_can.h
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

#ifndef CY_CAN_H
#define CY_CAN_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_common.h"

/**
 * @addtogroup CAN
 * @brief Controller Area Network Controller (CAN) driver for the PSC1M microcontroller family.
 *
 * CAN driver provides transfer of CAN frames in accordance with CAN specificetion V2.0 B (active). Each CAN node
 * can receive and transmit standard frames with 11-bit identifiers as well as extended frames with 29-bit identifiers.
 * All CAN nodes share a common set of message objects. Each message object can be individually allocated to one of the
 * CAN nodes.
 * Besides serving as a storage container for incoming and outgoing frames, message objects can be combined to build
 * gateways between
 * the CAN nodes or to setup a FIFO buffer. The CAN module provides Analyzer mode,Loop-back mode and bit timming for
 * node analysis.
 *
 * The driver is divided into five sections:
 * \par CAN Global features:
 * -# Allows to configure module frequency using function Cy_CAN_Init().
 * -# Allows to configure Module interrupt using configuration structure Cy_CAN_NODE_INTERRUPT_TRIGGER_t and function
 *    Cy_CAN_EventTrigger().
 *
 * \par CAN_NODE features:
 * -# Allows to set baud rate by configuration structure Cy_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t and Baudrate Configuration
 *    function Cy_CAN_NODE_NominalBitTimeConfigure().
 * -# Allows to configure loop-back mode using fuction Cy_CAN_NODE_EnableLoopBack().
 * -# Allows to configure CAN Node analyzer using function Cy_CAN_NODE_SetAnalyzerMode().
 * -# Allows CAN node events enable/ disable by enum structure Cy_CAN_NODE_EVENT_t and functions Cy_CAN_NODE_EnableEvent()
 *    and Cy_CAN_NODE_DisableEvent().
 * -# Provides bit timming analysis, configuration structure Cy_CAN_NODE_FRAME_COUNTER_t and function
 *    Cy_CAN_NODE_FrameCounterConfigure().
 *
 * \par CAN_MO features:
 * -# Allows message object initialization by configuration structure Cy_CAN_MO_t and function Cy_CAN_MO_Config().
 * -# Allows transfer of message objects using functions Cy_CAN_MO_Transmit() and Cy_CAN_MO_Receive().
 * -# Allows to configure Single Data Transfer and Single Transmit Trial using functions
 * Cy_CAN_MO_EnableSingleTransmitTrial() and Cy_CAN_MO_EnableSingleTransmitTrial().
 * -# Allows to configure MO events using function Cy_CAN_MO_EnableEvent().
 *
 * \par CAN_FIFO features:
 * -# Allows message object FIFO structure by configuration structure Cy_CAN_FIFO_CONFIG_t and functions
 * Cy_CAN_TXFIFO_ConfigMOBaseObject() , Cy_CAN_RXFIFO_ConfigMOBaseObject() and Cy_CAN_TXFIFO_Transmit().
 *
 * \par CAN_GATEWAY features:
 * -# Provides Gateway mode, configuration structure Cy_CAN_GATEWAY_CONFIG_t and function Cy_CAN_GATEWAY_InitSourceObject().
 *
 * @{
 */

#if defined(CAN) || defined(CY_DOXYGEN)

#include "cy_scu.h"
#include "cy_can_map.h"
#include <string.h>

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define Cy_CAN_MO_MOAR_STDID_Pos (18U)   /**< Standard Identifier bitposition */

#define Cy_CAN_MO_MOAR_STDID_Msk ((0x000007FFUL) << Cy_CAN_MO_MOAR_STDID_Pos) /**< Standard Identifier bitMask */

#define CAN_NODE_NIPR_Msk         (0x7UL) /**< Node event mask */

#define CAN_MO_MOIPR_Msk          (0x7U)  /**< Message Object event mask */

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
/**
 *  Defines the status of CAN, to verify the CAN related API calls. Use type \a Cy_CAN_STATUS_t for this enum.
*/
typedef enum CY_CAN_STATUS
{
  CY_CAN_STATUS_SUCCESS,           /**< Driver accepted application request*/
  CY_CAN_STATUS_ERROR,             /**< Returned when unknown error occurred */
  CY_CAN_STATUS_BUSY,              /**< Driver is busy and can not handle request*/
  CY_CAN_STATUS_MO_NOT_ACCEPTABLE, /**< Message object type not allowed*/
  CY_CAN_STATUS_MO_DISABLED       /**< Returned if Message object is disabled */
} Cy_CAN_STATUS_t;

/**
* Defines CAN module Panel Commands . Use type \a Cy_CAN_PANCMD_t for this enum.
*/
typedef enum CY_CAN_PANCMD
{
  CY_CAN_PANCMD_INIT_LIST = 1U,              /**< Command to initialize a list */
  CY_CAN_PANCMD_STATIC_ALLOCATE = 2U,        /**< Command to activate static allocation */
  CY_CAN_PANCMD_DYNAMIC_ALLOCATE = 3U,       /**< Command to activate dynamic allocation */

  CY_CAN_PANCMD_STATIC_INSERT_BEFORE = 4U,   /**< Remove a message object from the list and insert it before a given object.*/
  CY_CAN_PANCMD_DYNAMIC_INSERT_BEFORE = 5U,  /**< Command to activate dynamic allocation */
  CY_CAN_PANCMD_STATIC_INSERT_BEHIND = 6U,   /**< Command to activate dynamic allocation */
  CY_CAN_PANCMD_DYNAMIC_INSERT_BEHIND = 7U   /**< Command to activate dynamic allocation */
} Cy_CAN_PANCMD_t;

/**
* Defines loop Back Mode, to enable/disable an in-system test of the MultiCAN module .
* Use type \a Cy_CAN_LOOKBACKMODE_t for this enum.
*/
typedef enum CY_CAN_LOOKBACKMODE
{
  CY_CAN_LOOKBACKMODE_ENABLED,   /**< Each CAN node can be connected to the internal CAN bus  */
  CY_CAN_LOOKBACKMODE_DISABLED   /**< Each CAN node can be connected to the external CAN bus */
} Cy_CAN_LOOKBACKMODE_t;

/**
 * Defines Message Object direction. Use type \a Cy_CAN_MO_TYPE_t for this enum.
 */
typedef enum CY_CAN_MO_TYPE
{
  CY_CAN_MO_TYPE_RECMSGOBJ,   /**< Receive Message Object selected */
  CY_CAN_MO_TYPE_TRANSMSGOBJ  /**< Transmit Message Object selected */
} Cy_CAN_MO_TYPE_t;

/**
 * Defines Data frame types. Use type \a Cy_CAN_FRAME_TYPE_t for this enum.
 */
typedef enum CY_CAN_FRAME_TYPE
{
  CY_CAN_FRAME_TYPE_STANDARD_11BITS, /**< Standard type identifier*/
  CY_CAN_FRAME_TYPE_EXTENDED_29BITS  /**< Extended type identifier*/
} Cy_CAN_FRAME_TYPE_t;


/**
 * Defines arbitration mode for transmit acceptance filtering. Use type \a Cy_CAN_ARBITRATION_MODE_t for this enum.
 */
typedef enum CY_CAN_ARBITRATION_MODE
{
  CY_CAN_ARBITRATION_MODE_ORDER_BASED_PRIO_1 = 1U,   /**< Transmit acceptance based in the order(prio) */
  CY_CAN_ARBITRATION_MODE_IDE_DIR_BASED_PRIO_2 = 2U, /**< Transmit acceptance filtering is based on the CAN identifier */
  CY_CAN_ARBITRATION_MODE_ORDER_BASED_PRIO_3 = 3U    /**< Transmit acceptance based in the order */
} Cy_CAN_ARBITRATION_MODE_t;

/**
 * Defines the operation mode of the frame counter. Use type \a Cy_CAN_FRAME_COUNT_MODE_t for this enum.
 */
typedef enum CY_CAN_FRAME_COUNT_MODE
{
  CY_CAN_FRAME_COUNT_MODE = 0U,       /**< Frame Count Mode */
  CY_CAN_FRAME_COUNT_MODE_TIME_STAMP = 1U,  /**< The frame counter is incremented with the beginning of a new bit time*/
  CY_CAN_FRAME_COUNT_MODE_BIT_TIMING = 2U   /**< Used for baud rate detection and analysis of the bit timing */
} Cy_CAN_FRAME_COUNT_MODE_t;

/**
 * Defines the Divider Modes. Use type \a Cy_CAN_DM_t for this enum.
 */
typedef enum CY_CAN_DM
{
  CY_CAN_DM_NORMAL = 1U,      /**< Normal divider mode */
  CY_CAN_DM_FRACTIONAL = 2U,  /**< Fractional divider mode */
  CY_CAN_DM_OFF = 3U          /**< Divider Mode in off-state*/
} Cy_CAN_DM_t;

/**
 * Defines mask value for CAN Message Object set status. Use type \a Cy_CAN_MO_SET_STATUS_t for this enum.
 */
typedef enum CY_CAN_MO_SET_STATUS
{
  CY_CAN_MO_SET_STATUS_RX_PENDING = CAN_MO_MOCTR_SETRXPND_Msk,     /**< Set receive pending */
  CY_CAN_MO_SET_STATUS_TX_PENDING = CAN_MO_MOCTR_SETTXPND_Msk,     /**< Set transmit pending */
  CY_CAN_MO_SET_STATUS_RX_UPDATING = CAN_MO_MOCTR_SETRXUPD_Msk,    /**< Set receive updating */
  CY_CAN_MO_SET_STATUS_NEW_DATA = CAN_MO_MOCTR_SETNEWDAT_Msk,      /**< Set new data */
  CY_CAN_MO_SET_STATUS_MESSAGE_LOST = CAN_MO_MOCTR_SETMSGLST_Msk,  /**< Set message lost */
  CY_CAN_MO_SET_STATUS_MESSAGE_VALID = CAN_MO_MOCTR_SETMSGVAL_Msk, /**< Set message valid */
  CY_CAN_MO_SET_STATUS_RX_TX_SELECTED = CAN_MO_MOCTR_SETRTSEL_Msk, /**< Set transmit/receive selected */
  CY_CAN_MO_SET_STATUS_RX_ENABLE = CAN_MO_MOCTR_SETRXEN_Msk,       /**< Set receive enable */
  CY_CAN_MO_SET_STATUS_TX_REQUEST = CAN_MO_MOCTR_SETTXRQ_Msk,      /**< Set transmit request */
  CY_CAN_MO_SET_STATUS_TX_ENABLE0 = CAN_MO_MOCTR_SETTXEN0_Msk,     /**< Set transmit enable 0 */
  CY_CAN_MO_SET_STATUS_TX_ENABLE1 = CAN_MO_MOCTR_SETTXEN1_Msk,     /**< Set transmit enable 1 */
  CY_CAN_MO_SET_STATUS_MESSAGE_DIRECTION = CAN_MO_MOCTR_SETDIR_Msk /**< Set message direction */
} Cy_CAN_MO_SET_STATUS_t;

/**
 * Defines mask value for CAN Message Object reset status. Use type \a Cy_CAN_MO_RESET_STATUS_t for this enum.
 */
typedef enum CY_CAN_MO_RESET_STATUS
{
  CY_CAN_MO_RESET_STATUS_RX_PENDING = CAN_MO_MOCTR_RESRXPND_Msk,     /**< Reset receive pending */
  CY_CAN_MO_RESET_STATUS_TX_PENDING = CAN_MO_MOCTR_RESTXPND_Msk,     /**< Reset transmit pending */
  CY_CAN_MO_RESET_STATUS_RX_UPDATING = CAN_MO_MOCTR_RESRXUPD_Msk,    /**< Reset receive updating */
  CY_CAN_MO_RESET_STATUS_NEW_DATA = CAN_MO_MOCTR_RESNEWDAT_Msk,      /**< Reset new data */
  CY_CAN_MO_RESET_STATUS_MESSAGE_LOST = CAN_MO_MOCTR_RESMSGLST_Msk,  /**< Reset message lost */
  CY_CAN_MO_RESET_STATUS_MESSAGE_VALID = CAN_MO_MOCTR_RESMSGVAL_Msk, /**< Reset message valid */
  CY_CAN_MO_RESET_STATUS_RX_TX_SELECTED = CAN_MO_MOCTR_RESRTSEL_Msk, /**< Reset transmit/receive selected */
  CY_CAN_MO_RESET_STATUS_RX_ENABLE = CAN_MO_MOCTR_RESRXEN_Msk,       /**< Reset receive enable */
  CY_CAN_MO_RESET_STATUS_TX_REQUEST = CAN_MO_MOCTR_RESTXRQ_Msk,      /**< Reset transmit request */
  CY_CAN_MO_RESET_STATUS_TX_ENABLE0 = CAN_MO_MOCTR_RESTXEN0_Msk,     /**< Reset transmit enable 0 */
  CY_CAN_MO_RESET_STATUS_TX_ENABLE1 = CAN_MO_MOCTR_RESTXEN1_Msk,     /**< Reset transmit enable 1 */
  CY_CAN_MO_RESET_STATUS_MESSAGE_DIRECTION = CAN_MO_MOCTR_RESDIR_Msk /**< Reset message direction */
} Cy_CAN_MO_RESET_STATUS_t;

/**
 * Defines mask value for CAN Message Object status. Use type \a Cy_CAN_MO_STATUS_t for this enum.
 */
typedef enum CY_CAN_MO_STATUS
{
  CY_CAN_MO_STATUS_RX_PENDING = CAN_MO_MOSTAT_RXPND_Msk,                   /**< Defines message has been successfully received or not received */
  CY_CAN_MO_STATUS_TX_PENDING = CAN_MO_MOSTAT_TXPND_Msk,                   /**< Defines message has been successfully transmitted or not transmitted */
  CY_CAN_MO_STATUS_RX_UPDATING = CAN_MO_MOSTAT_RXUPD_Msk,                  /**< Defines Message identifier, DLC, and data of the message object are currently updated or not updated */
  CY_CAN_MO_STATUS_NEW_DATA = CAN_MO_MOSTAT_NEWDAT_Msk,                    /**< Defines no update of the message object since last flag reset or Message object has been updated */
  CY_CAN_MO_STATUS_MESSAGE_LOST = CAN_MO_MOSTAT_MSGLST_Msk,                /**< CAN message is lost because NEWDAT has become set again when it has already been set or No CAN message is lost */
  CY_CAN_MO_STATUS_MESSAGE_VALID = CAN_MO_MOSTAT_MSGVAL_Msk,               /**< Message valid */
  CY_CAN_MO_STATUS_RX_TX_SELECTED = CAN_MO_MOSTAT_RTSEL_Msk,               /**< Transmit/Receive selected */
  CY_CAN_MO_STATUS_RX_ENABLE = CAN_MO_MOSTAT_RXEN_Msk,                     /**< Receive enable */
  CY_CAN_MO_STATUS_TX_REQUEST = CAN_MO_MOSTAT_TXRQ_Msk,                    /**< Transmit request */
  CY_CAN_MO_STATUS_TX_ENABLE0 = CAN_MO_MOSTAT_TXEN0_Msk,                   /**< Transmit enable 0 */
  CY_CAN_MO_STATUS_TX_ENABLE1 = CAN_MO_MOSTAT_TXEN1_Msk,                   /**< Transmit enable 1 */
  CY_CAN_MO_STATUS_MESSAGE_DIRECTION = CAN_MO_MOSTAT_DIR_Msk,              /**< Message direction */
  CY_CAN_MO_STATUS_LIST = CAN_MO_MOSTAT_LIST_Msk,                          /**< List allocation */
  CY_CAN_MO_STATUS_POINTER_TO_PREVIOUS_MO = CAN_MO_MOSTAT_PPREV_Msk,     /**< Pointer to previous Message Object */
  CY_CAN_MO_STATUS_POINTER_TO_NEXT_MO = (int32_t)CAN_MO_MOSTAT_PNEXT_Msk   /**< Pointer to next Message Object */
} Cy_CAN_MO_STATUS_t;

/**
 * Defines mask value for CAN Node status. Use type \a Cy_CAN_NODE_STATUS_t for this enum.
 */
typedef enum CY_CAN_NODE_STATUS
{
  CY_CAN_NODE_STATUS_LAST_ERROR_CODE = CAN_NODE_NSR_LEC_Msk,       /**< Last Error Code */
  CY_CAN_NODE_STATUS_TX_OK = CAN_NODE_NSR_TXOK_Msk,                /**< Message transmitted successfully */
  CY_CAN_NODE_STATUS_RX_OK = CAN_NODE_NSR_RXOK_Msk,                /**< Message received successfully */
  CY_CAN_NODE_STATUS_ALERT_WARNING = CAN_NODE_NSR_ALERT_Msk,       /**< Alert warning */
  CY_CAN_NODE_STATUS_ERROR_WARNING_STATUS = CAN_NODE_NSR_EWRN_Msk, /**< Error warning status */
  CY_CAN_NODE_STATUS_BUS_OFF = CAN_NODE_NSR_BOFF_Msk,              /**< Bus-off status */
  CY_CAN_NODE_STATUS_LIST_LENGTH_ERROR = CAN_NODE_NSR_LLE_Msk,     /**< List length error */
  CY_CAN_NODE_STATUS_LIST_OBJECT_ERROR = CAN_NODE_NSR_LOE_Msk,     /**< List object error */
#if !defined(CY_MULTICAN_PLUS)
  CY_CAN_NODE_STATUS_SUSPENDED_ACK = CAN_NODE_NSR_SUSACK_Msk       /**< Suspend Acknowledge */
#endif
} Cy_CAN_NODE_STATUS_t;

/**
 * Defines mask value for CAN Node control like initialization, node disable and analyzer mode .
 * Use type \a Cy_CAN_NODE_CONTROL_t for this enum.
 */
typedef enum CY_CAN_NODE_CONTROL
{
  CY_CAN_NODE_CONTROL_NODE_INIT = CAN_NODE_NCR_INIT_Msk,            /**< Node initialization */
  CY_CAN_NODE_CONTROL_TX_INT_ENABLE = CAN_NODE_NCR_TRIE_Msk,        /**< Transfer event enable */
  CY_CAN_NODE_CONTROL_LEC_INT_ENABLE = CAN_NODE_NCR_LECIE_Msk,      /**< LEC Indicated Error Event Enable */
  CY_CAN_NODE_CONTROL_ALERT_INT_ENABLE = CAN_NODE_NCR_ALIE_Msk,     /**< Alert Event Enable */
  CY_CAN_NODE_CONTROL_CAN_DISABLE = CAN_NODE_NCR_CANDIS_Msk,        /**< CAN disable */
  CY_CAN_NODE_CONTROL_CONF_CHANGE_ENABLE = CAN_NODE_NCR_CCE_Msk,    /**< Configuration change enable */
  CY_CAN_NODE_CONTROL_CAN_ANALYZER_NODEDE = CAN_NODE_NCR_CALM_Msk,  /**< CAN Analyzer mode */
#if !defined(CY_MULTICAN_PLUS)
  CY_CAN_NODE_CONTROL_SUSPENDED_ENABLE = CAN_NODE_NCR_SUSEN_Msk     /**< Suspend Enable */
#endif
} Cy_CAN_NODE_CONTROL_t;

/**
 * Defines mask value for CAN Node events. Use type \a Cy_CAN_NODE_EVENT_t for this enum.
 */
typedef enum CY_CAN_NODE_EVENT
{
  CY_CAN_NODE_EVENT_TX_INT = CAN_NODE_NCR_TRIE_Msk,   /**< Node Transfer OK Event */
  CY_CAN_NODE_EVENT_ALERT = CAN_NODE_NCR_ALIE_Msk,    /**< Node Alert Event */
  CY_CAN_NODE_EVENT_LEC = CAN_NODE_NCR_LECIE_Msk,     /**< Last Error code Event */
  CY_CAN_NODE_EVENT_CFCIE = CAN_NODE_NFCR_CFCIE_Msk   /**< CAN Frame Count Event  */
} Cy_CAN_NODE_EVENT_t;

/**
 * Defines mask value for CAN node pointer events position. Use type \a Cy_CAN_NODE_POINTER_EVENT_t for this enum.
 */
typedef enum CY_CAN_NODE_POINTER_EVENT
{
  CY_CAN_NODE_POINTER_EVENT_ALERT = CAN_NODE_NIPR_ALINP_Pos,           /**< Node Alert Event node pointer */
  CY_CAN_NODE_POINTER_EVENT_LEC = CAN_NODE_NIPR_LECINP_Pos,            /**< Last Error code Event node pointer */
  CY_CAN_NODE_POINTER_EVENT_TRANSFER_OK = CAN_NODE_NIPR_TRINP_Pos,     /**< Transmit Event node pointer */
  CY_CAN_NODE_POINTER_EVENT_FRAME_COUNTER = CAN_NODE_NIPR_CFCINP_Pos   /**< CAN Frame Count Event node pointer */
} Cy_CAN_NODE_POINTER_EVENT_t;

/**
 * Defines CAN Message Object event node pointer position. Use type \a Cy_CAN_MO_POINTER_EVENT_t for this enum.
 */
typedef enum CY_CAN_MO_POINTER_EVENT
{
  CY_CAN_MO_POINTER_EVENT_TRANSMIT = CAN_MO_MOIPR_TXINP_Pos,     /**< Transmit Event node pointer */
  CY_CAN_MO_POINTER_EVENT_RECEIVE = CAN_MO_MOIPR_RXINP_Pos       /**< Receive Event node pointer */
} Cy_CAN_MO_POINTER_EVENT_t;

/**
 * Defines mask value for CAN Message Object event type. Use type \a Cy_CAN_MO_EVENT_t for this enum.
 */
typedef enum CY_CAN_MO_EVENT
{
  CY_CAN_MO_EVENT_TRANSMIT = CAN_MO_MOFCR_TXIE_Msk,  /**< Message Object transmit event */
  CY_CAN_MO_EVENT_RECEIVE = CAN_MO_MOFCR_RXIE_Msk,   /**< Message Object receive event */
  CY_CAN_MO_EVENT_OVERFLOW = CAN_MO_MOFCR_OVIE_Msk,  /**< Message Object overflow event */
} Cy_CAN_MO_EVENT_t;

/**
 * Defines the possible receive inputs. Use type \a Cy_CAN_NODE_RECEIVE_INPUT_t for this enum.
 */
typedef enum CY_CAN_NODE_RECEIVE_INPUT
{
  CY_CAN_NODE_RECEIVE_INPUT_RXDCA,   /**< CAN Receive Input A */
  CY_CAN_NODE_RECEIVE_INPUT_RXDCB,   /**< CAN Receive Input B */
  CY_CAN_NODE_RECEIVE_INPUT_RXDCC,   /**< CAN Receive Input C */
  CY_CAN_NODE_RECEIVE_INPUT_RXDCD,   /**< CAN Receive Input D */
  CY_CAN_NODE_RECEIVE_INPUT_RXDCE,   /**< CAN Receive Input E */
  CY_CAN_NODE_RECEIVE_INPUT_RXDCF,   /**< CAN Receive Input F */
  CY_CAN_NODE_RECEIVE_INPUT_RXDCG,   /**< CAN Receive Input G */
  CY_CAN_NODE_RECEIVE_INPUT_RXDCH    /**< CAN Receive Input H */
} Cy_CAN_NODE_RECEIVE_INPUT_t;

/**
 * Defines last error transfer direction. Use type \a Cy_CAN_NODE_LAST_ERROR_DIR_t for this enum.
 */
typedef enum CY_CAN_NODE_LAST_ERROR_DIR
{
  CY_CAN_NODE_LAST_ERROR_DIR_WHILE_NODE_RECEPCION,    /**< The last error occurred while the CAN node x was receiver */
  CY_CAN_NODE_LAST_ERROR_DIR_WHILE_NODE_TRANSMISSION  /**< The last error occurred while the CAN node x was transmitter */
} Cy_CAN_NODE_LAST_ERROR_DIR_t;


/**
 * Defines last error increment. Use type \a Cy_CAN_NODE_LAST_ERROR_INC_t for this enum.
 */
typedef enum CY_CAN_NODE_LAST_ERROR_INC
{
  CY_CAN_NODE_LAST_ERROR_INC_1, /**< The last error led to an error counter increment of 1. */
  CY_CAN_NODE_LAST_ERROR_INC_8  /**< The last error led to an error counter increment of 8. */
} Cy_CAN_NODE_LAST_ERROR_INC_t;

/**
 * Defines interrupt request on interrupt output line INT_O[n]. Use type \a Cy_CAN_NODE_INTERRUPT_TRIGGER_t for this enum.
 */
typedef enum CY_CAN_NODE_INTERRUPT_TRIGGER
{
  CY_CAN_NODE_INTR_TRIGGER_0 = 0x1U,
  CY_CAN_NODE_INTR_TRIGGER_1 = 0x2U,
  CY_CAN_NODE_INTR_TRIGGER_2 = 0x4U,
  CY_CAN_NODE_INTR_TRIGGER_3 = 0x8U,
  CY_CAN_NODE_INTR_TRIGGER_4 = 0x16U,
  CY_CAN_NODE_INTR_TRIGGER_5 = 0x32U,
  CY_CAN_NODE_INTR_TRIGGER_6 = 0x64U,
  CY_CAN_NODE_INTR_TRIGGER_7 = 0x128U,
} Cy_CAN_NODE_INTERRUPT_TRIGGER_t;

/**
 * Defines the Clock source used for the MCAN baudrate generator
 */
typedef enum CY_CAN_CANCLKSRC
{
#if defined(CY_MULTICAN_PLUS) || defined(CY_DOXYGEN)
  CY_CAN_CANCLKSRC_MCLK = 0x1U, /**< Use peripheral clock as MCAN baudrate generator input clock. */
  CY_CAN_CANCLKSRC_FOHP = 0x2U, /**< Use high performance oscillator (fOHP) as MCAN baudrate generator input clock.*/
#else
  CY_CAN_CANCLKSRC_FPERI = 0x1U, /**< Use peripheral clock as MCAN baudrate generator input clock. */
#endif
} Cy_CAN_CANCLKSRC_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/
/**
 * Defines CAN node Nominal Bit Time. Use type \a Cy_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t for this structure.
*/
typedef struct CY_CAN_NODE_NOMINAL_BIT_TIME_CONFIG
{
  uint32_t can_frequency;     /**< Frequency of the CAN module(fCAN). \a can_frequency shall be range of 5MHz to 144MHz */
  uint32_t baudrate;          /**< Specifies the node baud rate. Unit: baud \a baudrate shall be range of 100Kbps to 1000Kbps*/
  uint16_t sample_point;      /**< Sample point is used to compensate mismatch between transmitter and receiver clock phases detected in
                                   the synchronization segment. Sample point. Range = [0, 10000] with respect [0%, 100%] of the total bit time.*/
  uint16_t sjw;               /**< (Re) Synchronization Jump Width. Range:0-3 */
} Cy_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t;

/**
 * Defines base, top and bottom of CAN Message Object FIFO Structure. Use type \a Cy_CAN_FIFO_CONFIG_t for this structure.
 * A FIFO consists of one base object and n slave objects.
 */
typedef struct CY_CAN_FIFO_CONFIG
{
  uint8_t fifo_bottom;    /**< points to the first element(slave object) in a FIFO structure.Range :0-63*/
  uint8_t fifo_top;     /**< points to the last element(slave object) in a FIFO structure. Range :0-63*/
  uint8_t fifo_base;    /**< points to the actual target object(Base object) within a FIFO/Gateway structure. Range :0-63*/
} Cy_CAN_FIFO_CONFIG_t;

/**
 * Defines CAN Gateway FIFO structure and provides additional options for gateway destination object.
 * Use type \a Cy_CAN_GATEWAY_CONFIG_t for this structure.
 */
typedef struct CY_CAN_GATEWAY_CONFIG
{
  uint8_t gateway_bottom;       /**< points to the first element(gateway destination object) in a FIFO structure. Range :0-63*/
  uint8_t gateway_top;        /**< points to the last element(gateway destination object) in a FIFO structure.  Range :0-63*/
  uint8_t gateway_base;       /**< points to the actual target object within a FIFO/Gateway structure. Range :0-63*/
  bool gateway_data_frame_send;   /**< TXRQ updated in the gateway destination object after the internal transfer from the gateway source
                                         to the gateway destination object */
  bool gateway_identifier_copy;   /**< The identifier of the gateway source object (after storing the received frame in the source) is copied
                                         to the gateway destination object. */

  bool gateway_data_length_code_copy; /**< Data length code of the gateway source object (after storing the received frame in the source) is copied to the
                                           gateway destination object */
  bool gateway_data_copy;  /**< Data fields in registers MODATALn and MODATAHn of the gateway source object (after storing the received frame in the source)
                                are copied to the gateway destination.*/

} Cy_CAN_GATEWAY_CONFIG_t;

/**
* Defines CAN Global Initialization structure
*/
typedef CAN_GLOBAL_TypeDef Cy_CAN_t;

/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined(__TASKING__)
#pragma warning 586
#endif

/**
 * Defines frame counter configuration. Use type \a Cy_CAN_NODE_FRAME_COUNTER_t for this structure.
 * It provides configuration of frame counter that counts transmitted/received CAN frames or obtains information about the time
 * when a frame has been started to transmit or be received by the CAN node.
*/
typedef struct CY_CAN_NODE_FRAME_COUNTER
{

  union
  {
    struct
    {
      uint32_t : 16;
      uint32_t can_frame_count_selection: 3;  /**<   Defines function of the frame counter  */
      uint32_t can_frame_count_mode: 2;     /**<   Determines the operation mode of the frame counter */
      uint32_t : 11;
    };
    uint32_t nfcr;

  };
} Cy_CAN_NODE_FRAME_COUNTER_t;

/**
 *Defines Node Runtime structure.
*/
typedef CAN_NODE_TypeDef  Cy_CAN_NODE_t;            /**<  pointer to the Node CAN register */

/**
 * Defines CAN Message Object runtime elements. Use type \a Cy_CAN_MO_t for this structure.
 */
typedef struct CY_CAN_MO
{
  CAN_MO_TypeDef *can_mo_ptr;                     /**< Pointer to the Message Object CAN register */
  union
  {
    struct
    {
      uint32_t can_identifier: 29;           /**< standard (11 bit)/Extended (29 bit) message identifier */
      uint32_t can_id_mode: 1;               /**< Standard/Extended identifier support */
      uint32_t can_priority: 2;              /**< Arbitration Mode/Priority */
    };
    uint32_t mo_ar;
  };
  union
  {
    struct
    {
      uint32_t can_id_mask: 29;             /**< CAN Identifier of Message Object */
      uint32_t can_ide_mask: 1;             /**< Identifier Extension Bit of Message Object */
    };
    uint32_t mo_amr;
  };
  uint8_t can_data_length;                        /**< Message data length, Range:0-8 */

  union
  {

    uint8_t can_data_byte[8];                 /**< Each position of the array represents a data byte*/
    uint16_t can_data_word[4];                /**< Each position of the array represents a 16 bits data word*/
    uint32_t can_data[2];                     /**< can_data[0] lower 4 bytes of the data. can_data[1], higher 4 bytes
                              of the data */
    uint64_t can_data_long;                   /** Data of the Message Object*/
  };

  Cy_CAN_MO_TYPE_t can_mo_type;                  /**<  Message Type */

} Cy_CAN_MO_t;
/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
#pragma pop
#elif defined(__TASKING__)
#pragma warning restore
#endif
/*********************************************************************************************************************
 * API Prototypes
 ********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *        for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Disables CAN module. In disabled state, no registers of CAN module can be read or written except the CAN_CLC register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_Enable()\n\n\n
 *
 */

void Cy_CAN_Disable(Cy_CAN_t *const obj);

/**
 *
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *         for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Enables CAN module and initializes all CAN registers to reset values. It is required to re-configure desired CAN nodes,
 *  before any data transfer. It configures CAN_CLC.DISR bit.
 *
 * \par<b>Related APIs:</b><BR>
 *  Cy_CAN_Disable()\n\n\n
 *
 */

void Cy_CAN_Enable(Cy_CAN_t *const obj);

/**
 *
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *       for details.
 *
 * @param node_num  CAN node number,Range : 0-2
 * @param mo_num  CAN Message Object number,Range : 0-63
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Allocates Message Object from free list to node list. Each \a node_num is linked to one unique list of message objects.
 *  A CAN node performs message transfer only with the \a mo_num message objects that are allocated to the list of the CAN node.
 *  It configures PANCTR register.
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 */

void Cy_CAN_AllocateMOtoNodeList(Cy_CAN_t *const obj, const uint8_t node_num, const uint8_t mo_num);

/**
 *
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *       for details.
 *
 * @return Ready status of list controller
 *
 * \par<b>Description:</b><br>
 *  Returns ready status of the list controller
 *
 * \par<b>Related APIs:</b><BR>
 *  Cy_CAN_PanelControl()
 *
 */
__STATIC_INLINE bool Cy_CAN_IsPanelControlReady(Cy_CAN_t *const obj)
{
  return (bool)((obj->PANCTR & (CAN_PANCTR_BUSY_Msk | CAN_PANCTR_RBUSY_Msk)) == 0);
}

/**
 *
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *        for details.
 *
 * @param pancmd panal command selection. Refer @ref Cy_CAN_PANCMD_t for valid values.
 * @param arg1   Panel Argument 1,Range : 0-2
 * @param arg2   Panel Argument 2, Range : 0-63
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures the panel command and panel arguments. A panel operation consists of a command code (PANCMD) and up to two
 *  panel arguments (PANAR1, PANAR2). Commands that have a return value deliver it to the PANAR1 bit field. Commands that
 *  return an error flag deliver it to bit 31 of the Panel Control Register, this means bit 7 of PANAR2. \a arg1 represents
 *  panel argument PANAR1,\a arg2 represents panel argument PANAR2 and \a pancmd represents command code. It configures PANCTR
 *  register.
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 */

__STATIC_INLINE void Cy_CAN_PanelControl(Cy_CAN_t *const obj,
    const Cy_CAN_PANCMD_t pancmd,
    const uint8_t arg1,
    const uint8_t arg2)
{
  obj->PANCTR = (((uint32_t)pancmd << CAN_PANCTR_PANCMD_Pos) & (uint32_t)CAN_PANCTR_PANCMD_Msk) |
                (((uint32_t)arg1 << CAN_PANCTR_PANAR1_Pos) & (uint32_t)CAN_PANCTR_PANAR1_Msk) |
                (((uint32_t)arg2 << CAN_PANCTR_PANAR2_Pos) & (uint32_t)CAN_PANCTR_PANAR2_Msk);
}


/**
 *
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *        for details.
 *
 * @param service_requestr  Interrupt trigger number selection. Refer @ref Cy_CAN_NODE_INTERRUPT_TRIGGER_t for valid values.
 *               Multiple interrupt trigger events can be ORed.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures multiple interrupt requests with a single write operation. \a service_requestr represents single interrupt
 *  request or multiple.It configures MITR register.
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 */

__STATIC_INLINE void Cy_CAN_EventTrigger(Cy_CAN_t *const obj, const Cy_CAN_NODE_INTERRUPT_TRIGGER_t service_requestr)
{
  obj->MITR = ((uint32_t)service_requestr << CAN_MITR_IT_Pos) & (uint32_t)CAN_MITR_IT_Msk;
}


/**
 *
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *        for details.
 *
 * @param clksrc CAN module frequency clock source
 * @param can_frequency CAN module frequency(fCAN).
 *        The can_frequency shall be an integer divisor of the CAN clock source frequency (fPERI/fMCLK or fOHP) and
 *        it should be selected such that if fulfills the minimum operating frequency specified in the clock control section of the reference manual.
 *        In addition the can_frequency shall be an integer multiple of the baudrate.
 *        Max. value: Max. clock source frequency, i.e. if using fOHP and using a XTAL of 20MHz then fCAN max. value is 20MHz.
 *
 * @return Actual frequency
 *
 * \par<b>Description:</b><br>
 * Similar to Cy_CAN_Init() configures clock rate of the module timer clock fCAN.
 * The difference being that only the normal divider is used to avoid jitter.
 * In addition the actual achievable frequency is returned.
 * @note Use Cy_CAN_InitEx() for new projects. Cy_CAN_Init() is kept for backward compatibility reasons.
 *
 * Altering CAN module \a can_frequency shall affects baud rate, call Cy_CAN_NODE_NominalBitTimeConfigureEx() to configure baud rate for current CAN frequency.
 * It configures FDR register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_NominalBitTimeConfigure(), Cy_CAN_NODE_NominalBitTimeConfigureEx()\n\n\n
 *
 */
uint32_t Cy_CAN_InitEx(Cy_CAN_t *const obj, Cy_CAN_CANCLKSRC_t clksrc, uint32_t can_frequency);

/**
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *        for details.
 *
 * @return Baudrate generator clock source
 *
 * \par<b>Description:</b><br>
 * Returns CAN baudrate generator clock source
 */
Cy_CAN_CANCLKSRC_t Cy_CAN_GetBaudrateClockSource(Cy_CAN_t *const obj);

/**
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *        for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets CAN baudrate generator clock source
 */
void Cy_CAN_SetBaudrateClockSource(Cy_CAN_t *const obj, const Cy_CAN_CANCLKSRC_t source);

/**
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *        for details.
 *
 * @return baudrate generator input clock frequency
 *
 * \par<b>Description:</b><br>
 * Gets CAN baudrate generator input clock frequency
 */
uint32_t Cy_CAN_GetBaudrateClockFrequency(Cy_CAN_t *const obj);

/**
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *        for details.
 *
 * @return fCAN clock frequency
 *
 * \par<b>Description:</b><br>
 * Gets CAN base clock frequency
 *
 */
uint32_t Cy_CAN_GetClockFrequency(Cy_CAN_t *const obj);

#if defined(CY_MULTICAN_PLUS) || defined(CY_DOXYGEN)
/**
 *
 * @param obj Pointer pointing to CY_CAN Global Initialization structure. Defines CAN global registers,refer CAN_NODE_TypeDef
 *        for details.
 *
 * @param clksrc CAN module frequency clock source.
 * @param can_frequency CAN module frequency(fCAN). Max. value: Max. clock source frequency, i.e. if using fOHP and using a XTAL of 20MHz then fCAN max. value is 20MHz.
 * @note Use clksrc parameter only for PSC1M series projects, otherwise omit this parameter.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures clock rate of the module timer clock fCAN. Altering CAN module \a can_frequency shall affects baud rate,
 * call Cy_CAN_NODE_NominalBitTimeConfigure() or Cy_CAN_NODE_NominalBitTimeConfigureEx() to configure baud rate for current CAN frequency.
 * It configures FDR register.
 *
 * @note Use Cy_CAN_InitEx() for new projects. Cy_CAN_Init() is kept for backward compatibility reasons.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_NominalBitTimeConfigure(), Cy_CAN_NODE_NominalBitTimeConfigureEx(), Cy_CAN_InitEx()\n\n\n
 *
 */
void Cy_CAN_Init(Cy_CAN_t *const obj, Cy_CAN_CANCLKSRC_t clksrc, uint32_t can_frequency);
#else
void Cy_CAN_Init(Cy_CAN_t *const obj, uint32_t can_frequency);
#endif

/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Initializes CAN message object. Initialization includes configuring Message Object identifier type, Message Object
 * identifier value, Message Object type, and transfer requests. It configures FDR register.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 */
void Cy_CAN_MO_Config(const Cy_CAN_MO_t *const can_mo);

/* NODE APIs */

/**
 *
 * @param can_node      Pointer pointing to CAN_NODE Structure. Defines CAN_NODE registers.
 * @param ptr_event     CAN_NODE interrupt pointer position. Refer @ref Cy_CAN_NODE_POINTER_EVENT_t structure for valid values.
 * @param service_request Interrupt service request number. Range : 0-7
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures node interrupt pointer \a service_request for CAN Node events \a ptr_event. It configures NIPR register.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_SetEventNodePointer(Cy_CAN_NODE_t *const can_node,
    const Cy_CAN_NODE_POINTER_EVENT_t ptr_event,
    const uint32_t service_request)
{
  can_node->NIPR = (uint32_t)((can_node->NIPR) & ~(uint32_t)(CAN_NODE_NIPR_Msk << (uint32_t)ptr_event)) | (service_request << (uint32_t)ptr_event);
}


/**
 *
 * @param can_node    Pointer pointing to CAN_NODE Structure. Defines CAN_NODE registers. Range :CAN_NODE0-CAN_NODE2
 * @param can_bit_time  Nominal bit time configuration data structure. Refer @ref Cy_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures CAN node Baudrate. \a can_bit_time specifies required baudrate for a specified \a can_node.
 * It configures NBTR register.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 */
void Cy_CAN_NODE_NominalBitTimeConfigure(Cy_CAN_NODE_t *const can_node,
    const Cy_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t *const can_bit_time);

/**
 *
 * @param can_node    Pointer pointing to CAN_NODE Structure. Defines CAN_NODE registers. Range :CAN_NODE0-CAN_NODE2
 * @param can_bit_time  Nominal bit time configuration data structure. Refer @ref Cy_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t for details.
 *
 * @return Status on the sucess setting the desired bitrate Cy_CAN_STATUS_t
 *
 * \par<b>Description:</b><br>
 * Similar to Cy_CAN_NODE_NominalBitTimeConfigure() configures the CAN node baudrate.
 * The difference being that allows bitrates below 100kpbs.
 * @note Consider using the new API for new projects. Cy_CAN_NODE_NominalBitTimeConfigure() is kept for backward compatibility reasons.
 *
 * Configures CAN node Baudrate. \a can_bit_time specifies required baudrate for a specified \a can_node.
 * It configures NBTR register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_InitEx()
 *
 */
int32_t Cy_CAN_NODE_NominalBitTimeConfigureEx(Cy_CAN_NODE_t *const can_node,
    const Cy_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t *const can_bit_time);

/**
 * @param can_node Pointer pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 * @param input    CAN receive input selection. Refer @ref Cy_CAN_NODE_RECEIVE_INPUT_t for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * \a input specifies CAN input receive pin. This API Configures NPCRx register,it is required to call
 * Cy_CAN_NODE_EnableConfigurationChange(), before configuring NPCRx register, call Cy_CAN_NODE_DisableConfigurationChange()
 * API after configuring NPCRx register. CAN input receive pins which falls under analog port pins should call
 * Cy_GPIO_EnableDigitalInput(),to enable digital pad.
 *
 *\par<b>Related APIs:</b><BR>
 * None
 *
 * \par<b>Note:</b><br>
 * NPCRx can be written only if bit NCRx.CCE is set.
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_SetReceiveInput(Cy_CAN_NODE_t *const can_node,
    const Cy_CAN_NODE_RECEIVE_INPUT_t input)
{
  can_node->NPCR = ((can_node->NPCR) & ~(uint32_t)(CAN_NODE_NPCR_RXSEL_Msk)) |
                   (((uint32_t)input << CAN_NODE_NPCR_RXSEL_Pos) & (uint32_t)CAN_NODE_NPCR_RXSEL_Msk);
}


/**
 * @param can_node  Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enable Node \a can_node in Loop-Back Mode. A Node is connected to an internal (virtual) loop-back CAN bus. All CAN
 * nodes which are in Loop- Back Mode are connected to this virtual CAN bus so that they can communicate with each
 * other internally. The external transmit line is forced recessive in Loop-Back Mode. This API Configures NPCRx register.
 * call Cy_CAN_NODE_EnableConfigurationChange() API before NPCRx configuration, same way Cy_CAN_NODE_DisableConfigurationChange()
 * API after NPCRx configuration configuration.
 *
 * \par<b>Related APIs:</b><BR>]
 *  Cy_CAN_NODE_DisableLoopBack().
 *
 *  \par<b>Note:</b><br>
 *  NPCRx can be written only if bit NCRx.CCE is set.
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_EnableLoopBack(Cy_CAN_NODE_t *const can_node)
{
  can_node->NPCR |= (uint32_t)CAN_NODE_NPCR_LBM_Msk;
}


/**
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disable Node Loop-Back Mode, disables internal (virtual) loop-back CAN bus. This API Configures NPCRx register.
 * Call Cy_CAN_NODE_EnableConfigurationChange() API before NPCRx configuration, same way Cy_CAN_NODE_DisableConfigurationChange()
 * API after NPCRx configuration.
 *
 * \par<b>Related APIs:</b><BR>]
 * Cy_CAN_NODE_EnableLoopBack()
 *
 *
 * \par<b>Note:</b><br>
 * NPCRx can be written only if bit NCRx.CCE is set.
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_DisableLoopBack(Cy_CAN_NODE_t *const can_node)
{
  can_node->NPCR &= ~(uint32_t)CAN_NODE_NPCR_LBM_Msk;
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 * @param event    CAN node event mask value. Refer @ref Cy_CAN_NODE_EVENT_t structure for valid values.
 *           multiple events can be ORed.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Enable CAN Node events. It configures NFCR register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_DisableEvent()
 *
 */

void Cy_CAN_NODE_EnableEvent(Cy_CAN_NODE_t *const can_node, const Cy_CAN_NODE_EVENT_t event);


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 * @param event    CAN node event mask value. Refer @ref Cy_CAN_NODE_EVENT_t structure for valid values.
 *           multiple events can be ORed.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Disable CAN Node events. It configures NFCR register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_EnableEvent()
 *
 */

void Cy_CAN_NODE_DisableEvent(Cy_CAN_NODE_t *const can_node, const Cy_CAN_NODE_EVENT_t event);


/**
 *
 * @param node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return Cy_CAN_NODE_LAST_ERROR_DIR_t Last error transfer direction. Refer @ref Cy_CAN_NODE_LAST_ERROR_DIR_t.
 *
 * \par<b>Description:</b><br>
 * Returns NODE Last Error occurred during Transmit/receive direction. It returns value of NFCR register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_GetLastErrTransferInc()\n\n\n
 *
 */

__STATIC_INLINE Cy_CAN_NODE_LAST_ERROR_DIR_t Cy_CAN_NODE_GetLastErrTransferDir(Cy_CAN_NODE_t *const node)
{
  return (Cy_CAN_NODE_LAST_ERROR_DIR_t)(((node->NECNT) & CAN_NODE_NECNT_LETD_Msk) >> CAN_NODE_NECNT_LETD_Pos);
}


/**
 *
 * @param node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return Cy_CAN_NODE_LAST_ERROR_INC_t Last error transfer increment. Refer @ref Cy_CAN_NODE_LAST_ERROR_INC_t.
 *
 * \par<b>Description:</b><br>
 *  Returns NODE Last Error Transfer Increment. It returns value of NFCR register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_GetLastErrTransferDir()\n\n\n
 *
 */

__STATIC_INLINE Cy_CAN_NODE_LAST_ERROR_INC_t Cy_CAN_NODE_GetLastErrTransferInc(Cy_CAN_NODE_t *const node)
{
  return (Cy_CAN_NODE_LAST_ERROR_INC_t)(((node->NECNT) & CAN_NODE_NECNT_LEINC_Msk) >> CAN_NODE_NECNT_LEINC_Pos);
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 * @param  error_warning_level  Error Warning level value. Range :0-255.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures error warning level in order to set the corresponding error warning bit EWRN. It configures \a error_warning_level
 * into NECNT register. Before this configuration call Cy_CAN_NODE_EnableConfigurationChange() API.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_GetErrorWarningLevel()\n\n\n
 *
 * \par<b>Note:</b><br>
 *  NECNTx can be written only if bit NCRx.CCE is set.
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_SetErrorWarningLevel(Cy_CAN_NODE_t *const can_node, uint8_t error_warning_level)
{
  can_node->NECNT = ((can_node->NECNT) & ~(uint32_t)(CAN_NODE_NECNT_EWRNLVL_Msk)) |
                    (((uint32_t)error_warning_level << CAN_NODE_NECNT_EWRNLVL_Pos) & (uint32_t)CAN_NODE_NECNT_EWRNLVL_Msk);
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 * @param can_tec  transmit error counter value. Range :0-255
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures Transmit error counter. It configures \a can_tec into NECNT register. Before this configuration call
 *  Cy_CAN_NODE_EnableConfigurationChange() API.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_GetTransmitErrorCounter()\n\n\n
 *
 *  \par<b>Note:</b><br>
 *  NECNTx can be written only if bit NCRx.CCE is set.
 *
 */


__STATIC_INLINE void Cy_CAN_NODE_SetTransmitErrorCounter(Cy_CAN_NODE_t *const can_node, uint8_t can_tec)
{
  can_node->NECNT = ((can_node->NECNT) & ~(uint32_t)(CAN_NODE_NECNT_TEC_Msk)) |
                    (((uint32_t)can_tec << CAN_NODE_NECNT_TEC_Pos) & (uint32_t)CAN_NODE_NECNT_TEC_Msk);
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 * @param can_rec  receive error counter value. Range :0-255
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures Receive Error Counter. It configures \a can_rec into NECNT register. Before this configuration call
 * Cy_CAN_NODE_EnableConfigurationChange() API.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_GetReceiveErrorCounter()\n\n\n
 *
 * \par<b>Note:</b><br>
 *  NECNTx can be written only if bit NCRx.CCE is set.
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_SetReceiveErrorCounter(Cy_CAN_NODE_t *const can_node, uint8_t can_rec)
{
  can_node->NECNT = ((can_node->NECNT) & ~(uint32_t)(CAN_NODE_NECNT_REC_Msk)) |
                    (((uint32_t)can_rec << CAN_NODE_NECNT_REC_Pos) & (uint32_t)CAN_NODE_NECNT_REC_Msk);
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return uint8_t Error Warning Level. Range :0 - 255
 *
 * \par<b>Description:</b><br>
 *  Returns error warning level. This determines the threshold value (warning level, default 96) to be reached in order
 *  to set the corresponding error warning bit EWRN. It reads NECNT register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_SetErrorWarningLevel()\n\n\n
 *
 */


__STATIC_INLINE uint8_t Cy_CAN_NODE_GetErrorWarningLevel(Cy_CAN_NODE_t *const can_node)
{
  return (uint8_t)((uint32_t)((can_node->NECNT)  & CAN_NODE_NECNT_EWRNLVL_Msk) >> CAN_NODE_NECNT_EWRNLVL_Pos);
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return uint8_t transmit error counter value. Range :0 - 255
 *
 * \par<b>Description:</b><br>
 *  Returns Transmit Error Counter value. If the Bitstream Processor detects an error while a transmit operation is
 *  running, the Transmit Error Counter is incremented by 8. An increment of 1 is used when the error condition was
 *  reported by an external CAN node via an Error Frame generation. It reads NECNT register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_SetTransmitErrorCounter()
 *
 * \par<b>Note:</b><br>
 *  NECNTx can be written only if bit NCRx.CCE is set.
 *
 */

__STATIC_INLINE uint8_t Cy_CAN_NODE_GetTransmitErrorCounter(Cy_CAN_NODE_t *const can_node)
{
  return (uint8_t)((uint32_t)((can_node->NECNT)  & CAN_NODE_NECNT_TEC_Msk) >> CAN_NODE_NECNT_TEC_Pos);
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return uint8_t  receive error counter value.
 *
 * \par<b>Description:</b><br>
 *  Returns Receive Error Counter value. It reads NECNT register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_SetReceiveErrorCounter()
 *
 */

__STATIC_INLINE uint8_t Cy_CAN_NODE_GetReceiveErrorCounter(Cy_CAN_NODE_t *const can_node)
{
  return (uint8_t)((uint32_t)((can_node->NECNT)  & CAN_NODE_NECNT_REC_Msk) >> CAN_NODE_NECNT_REC_Pos);
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return uint32_t Current status of the node.
 *
 * \par<b>Description:</b><br>
 * Returns errors status as well as successfully transferred CAN frames status.
 *
 * \par<b>Related APIs:</b><BR>
 *  Cy_CAN_NODE_ClearStatus()
 *
 */

__STATIC_INLINE uint32_t Cy_CAN_NODE_GetStatus(Cy_CAN_NODE_t *const can_node)
{
  return ((can_node->NSR));
}

/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 * @param can_node_status Status to clear.Refer @ref Cy_CAN_NODE_STATUS_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Clear errors status as well as successfully transferred CAN frames status.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_GetStatus()
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_ClearStatus(Cy_CAN_NODE_t *const can_node, Cy_CAN_NODE_STATUS_t can_node_status)
{
  can_node->NSR &= ~(uint32_t)can_node_status;
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Allow to change the configuration of the CAN node, like bit timing, CAN bus transmit/receive ports and error
 * counters read. It configures NCRx.CCE bit.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_CAN_NODE_DisableConfigurationChange()
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_EnableConfigurationChange(Cy_CAN_NODE_t *const can_node)
{
  can_node->NCR |= (uint32_t)CAN_NODE_NCR_CCE_Msk;
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Forbid to change the configuration of the CAN node. It configures NCRx.CCE bit.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_NODE_EnableConfigurationChange()
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_DisableConfigurationChange(Cy_CAN_NODE_t *const can_node)
{
  can_node->NCR &= ~(uint32_t)CAN_NODE_NCR_CCE_Msk;
}



/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disable CAN node participation in CAN traffic. Bit INIT is automatically set when the CAN node enters the bus-off
 * state. It configures NCR.INIT bit.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_NODE_ResetInitBit()
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_SetInitBit(Cy_CAN_NODE_t *const can_node)
{
  can_node->NCR |= (uint32_t)CAN_NODE_NCR_INIT_Msk;
}

/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enable CAN node participation in CAN traffic. Bit INIT is automatically set when the CAN node enters the bus-off
 * state. It configures NCR.INIT bit.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_NODE_SetInitBit()
 *
 */
__STATIC_INLINE void Cy_CAN_NODE_ResetInitBit(Cy_CAN_NODE_t *const can_node)
{
  can_node->NCR &= ~(uint32_t)CAN_NODE_NCR_INIT_Msk;
}

/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enable the CAN node, starts the participation in CAN traffic. It configures NCR.CANDIS and the NCR.INIT bits.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */
__STATIC_INLINE void Cy_CAN_NODE_Enable(Cy_CAN_NODE_t *const can_node)
{
  can_node->NCR &= ~(uint32_t)CAN_NODE_NCR_CANDIS_Msk;
  Cy_CAN_NODE_ResetInitBit(can_node);
}

/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disable the CAN node, terminates participation in CAN traffic. It configures NCR.CANDIS bit.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */
__STATIC_INLINE void Cy_CAN_NODE_Disable(Cy_CAN_NODE_t *const can_node)
{
  can_node->NCR |= (uint32_t)CAN_NODE_NCR_CANDIS_Msk;
}

/**
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configure CAN node in Analyzer Mode. This means that messages may be received, but not transmitted. No acknowledge
 * is sent on the CAN bus upon frame reception. Active-error flags are sent recessive instead of dominant.
 * The transmit line is continuously held at recessive (1) level. Cy_CAN_NODE_SetInitBit() should be called before
 * set / reset AnalyzerMode. It configures NCR.CALM bit.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_NODE_ReSetAnalyzerMode()
 *
 */


__STATIC_INLINE void Cy_CAN_NODE_SetAnalyzerMode(Cy_CAN_NODE_t *const can_node)
{
  can_node->NCR |= (uint32_t)CAN_NODE_NCR_CALM_Msk;
}


/**
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Reset the Analyzer mode. CAN node is no more in Analyzer Mode. Please refer Cy_CAN_NODE_SetAnalyzerMode().
 * It configures NCR.CALM bit. Cy_CAN_NODE_SetInitBit() should be called before set / reset AnalyzerMode.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_NODE_SetAnalyzerMode()
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_ReSetAnalyzerMode(Cy_CAN_NODE_t *const can_node)
{
  can_node->NCR &= ~(uint32_t)CAN_NODE_NCR_CALM_Msk;
}

#if !defined(CY_MULTICAN_PLUS)
/**
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Trigger the suspension of the CAN node. An OCDS(on chip debug support) suspend trigger disables the CAN node: As
 * soon as the CAN node becomes bus-idle or bus-off, bit INIT is internally forced to 1 to disable the CAN node.
 * The actual value of bit INIT remains unchanged. It configures NCR.SUSEN bit
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 *\par<b>Note:</b><br>
 * Bit SUSEN is reset via OCDS(on chip debug support) Reset.
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_EnableSuspend(Cy_CAN_NODE_t *const can_node)
{
  can_node->NCR |= (uint32_t)CAN_NODE_NCR_SUSEN_Msk;
}
#else
/**
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the transmission on CAN node x as soon as bus-idle is reached.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_DisableTransmit(Cy_CAN_NODE_t *const can_node)
{
  can_node->NCR |= (uint32_t)CAN_NODE_NCR_TXDIS_Msk;
}
#endif


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 * @param can_node_init frame counter mode selection. Refer @ref Cy_CAN_NODE_FRAME_COUNTER_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures frame counter functions. Each CAN \a can_node is equipped with a frame counter that counts transmitted/received
 * CAN frames or obtains information about the time when a frame has been started to transmit or be received by the CAN
 * node. It configures NFCR register.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE void Cy_CAN_NODE_FrameCounterConfigure(Cy_CAN_NODE_t *const can_node,
    const Cy_CAN_NODE_FRAME_COUNTER_t *const can_node_init)
{
  can_node->NFCR = (can_node->NFCR & ~(uint32_t)(CAN_NODE_NFCR_CFMOD_Msk |
                    CAN_NODE_NFCR_CFSEL_Msk)) |
                   can_node_init->nfcr;
}


/**
 *
 * @param can_node Pointer Pointing to CAN_NODE Structure. Defines CAN_NODE registers, Range :CAN_NODE0-CAN_NODE2.
 *
 * @return uint16_t current value of the CAN frame counter. Range :0-65535
 *
 * \par<b>Description:</b><br>
 *  Returns the frame counter value \a can_node of the CAN node. In Frame Count Mode (CFMOD = 00B), this API returns the frame
 *  count value. In Time Stamp Mode (CFMOD = 01B), this API returns the captured bit time count value, captured with
 *  the start of a new frame. In all Bit Timing Analysis Modes (CFMOD = 10B), this API always returns the number of
 *  fCLC clock cycles (measurement result) minus 1.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE uint16_t Cy_CAN_NODE_GetCANFrameCounter(Cy_CAN_NODE_t *const can_node)
{
  return (uint16_t)(((uint32_t)(can_node->NFCR & CAN_NODE_NFCR_CFC_Msk) >> CAN_NODE_NFCR_CFC_Pos));
}

/*MO APIs*/

/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return Cy_CAN_STATUS_t CAN Node status. Refer @ref Cy_CAN_STATUS_t structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures Data to be transmitted and data length code.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_Config()\n\n\n
 *
 */


Cy_CAN_STATUS_t Cy_CAN_MO_UpdateData(const Cy_CAN_MO_t *const can_mo);

/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return Cy_CAN_STATUS_t CAN Node status. Refer @ref Cy_CAN_STATUS_t structure for details.
 *
 * \par<b>Description:</b><br>
 *  Configures transmit request for sending data frame. It configures MOCTR register. Data shall be updated
 *  using Cy_CAN_MO_UpdateData() before calling this API.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_UpdateData()\n\n\n
 *
 */

Cy_CAN_STATUS_t Cy_CAN_MO_Transmit(const Cy_CAN_MO_t *const can_mo);


/**
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return Cy_CAN_STATUS_t CAN Node status. Refer @ref Cy_CAN_STATUS_t structure for details.
 *
 * \par<b>Description:</b><br>
 *  Reads the Message Object data bytes, into message pointer passed as input parameter \a can_mo.
 *  can_data[0] of can_mo holds lower 4 bytes, can_data[1] of can_mo holds higher 4 bytes.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

Cy_CAN_STATUS_t Cy_CAN_MO_Receive(Cy_CAN_MO_t *can_mo);


/**
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return Cy_CAN_STATUS_t CAN Node status. Refer @ref Cy_CAN_STATUS_t structure for details.
 *
 * \par<b>Description:</b><br>
 *  Reads the Message Object data bytes, into message pointer passed as input parameter \a can_mo.
 *  can_data[0] of can_mo holds lower 4 bytes, can_data[1] of can_mo holds higher 4 bytes.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */
Cy_CAN_STATUS_t Cy_CAN_MO_ReceiveData (Cy_CAN_MO_t *can_mo);
/**
 * @param can_mo       Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param can_mo_ptr_int   Message Object event node pointer selection. Refer @ref Cy_CAN_MO_POINTER_EVENT_t structure
 *                for valid values.
 * @param service_request interrupt output line of multiCAN module.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures Message Object event node pointer with \a service_request number. It configures MOIPR register.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE void Cy_CAN_MO_SetEventNodePointer(const Cy_CAN_MO_t *const can_mo,
    const Cy_CAN_MO_POINTER_EVENT_t can_mo_ptr_int,
    const uint32_t service_request)
{
  can_mo->can_mo_ptr->MOIPR = ((can_mo->can_mo_ptr->MOIPR ) & ~(uint32_t)((uint32_t)CAN_MO_MOIPR_Msk << (uint32_t)can_mo_ptr_int)) |
                              (service_request << (uint32_t)can_mo_ptr_int);
}

/**
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return uint32_t Current Message Object status.
 *
 * \par<b>Description:</b><br>
 *  Returns Message Object status, that indicates message object transfer status and message object list status
 *  information such as the number of the current message object predecessor and successor message object, as well as
 *  the list number to which the message object is assigned.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE uint32_t Cy_CAN_MO_GetStatus(const Cy_CAN_MO_t *const can_mo)
{
  return ((can_mo->can_mo_ptr->MOSTAT));
}

/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param mask   Message Object set status selection. Refer @ref Cy_CAN_MO_SET_STATUS_t for details.
 *         Multiple enums can be ORed.
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures Message Object set Status. It configures MOCTR register.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_ResetStatus()\n\n\n
 *
 */

__STATIC_INLINE void Cy_CAN_MO_SetStatus(const Cy_CAN_MO_t *const can_mo, const uint32_t mask)
{
  can_mo->can_mo_ptr->MOCTR = mask;
}

/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param mask   Message Object set status selection. Refer @ref Cy_CAN_MO_RESET_STATUS_t for details.
 *             Multiple enums can be ORed.
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Clears Message Object interrupt events. It configures MOCTR register.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE void Cy_CAN_MO_ResetStatus(const Cy_CAN_MO_t *const can_mo, const uint32_t mask)
{
  can_mo->can_mo_ptr->MOCTR = mask;
}



/**
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param event  CAN node Message Object event selection. Refer @ref Cy_CAN_MO_EVENT_t for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enable CAN Message Object events. \a event can be ORed multiple Message Object events. It configures MOFCR register.
 *
 * \par<b>Related API's:</b><br>
 *  Cy_CAN_MO_DisableEvent()
 *
 */

__STATIC_INLINE void Cy_CAN_MO_EnableEvent(const Cy_CAN_MO_t *const can_mo,
    const uint32_t event)
{
  can_mo->can_mo_ptr->MOFCR |= event;
}


/**
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param event  CAN node Message Object event selection. Refer @ref Cy_CAN_MO_EVENT_t for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Disable CAN Message Object events. \a event can be ORed multiple Message Object events. It configures MOFCR register.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_EnableEvent()
 *
 *
 */

__STATIC_INLINE void Cy_CAN_MO_DisableEvent(const Cy_CAN_MO_t *const can_mo,
    const uint32_t event)
{
  can_mo->can_mo_ptr->MOFCR &= ~event;
}

/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Enable the single transmit trial(STT). In STT,TXRQ is cleared on transmission start of message object n. Thus,
 *  no transmission retry is performed in case of transmission failure. It configures MOFCR.STT bit.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_DisableSingleTransmitTrial()
 *
 */

__STATIC_INLINE void Cy_CAN_MO_EnableSingleTransmitTrial(const Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_mo_ptr->MOFCR |= (uint32_t) CAN_MO_MOFCR_STT_Msk;
}


/**
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Disable the single transmit trial(STT). In STT,TXRQ is cleared on transmission start of message object n. Thus,
 *  no transmission retry is performed in case of transmission failure. It configures MOFCR.STT bit.
 *
 * \par<b>Related API's:</b><br>
 *  Cy_CAN_MO_EnableSingleTransmitTrial()
 *
 */

__STATIC_INLINE void Cy_CAN_MO_DisableSingleTransmitTrial(const Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_mo_ptr->MOFCR &= ~(uint32_t)CAN_MO_MOFCR_STT_Msk;
}

/**
 *
 * @param can_mo            Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param data_length_code  transfer data length. Range:0-8
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures CAN Message Object Data Length Code. It configures MOFCR register.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE void Cy_CAN_MO_DataLengthCode(const Cy_CAN_MO_t *const can_mo, const uint8_t data_length_code)
{
  can_mo->can_mo_ptr->MOFCR = ((can_mo->can_mo_ptr->MOFCR ) & ~(uint32_t)(CAN_MO_MOFCR_DLC_Msk)) |
                              (((uint32_t)data_length_code << CAN_MO_MOFCR_DLC_Pos) & (uint32_t)CAN_MO_MOFCR_DLC_Msk);
}

/**
 *
 * @param can_mo            Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param data_length_code  transfer data length. Range:0-8
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures CAN Message Object Data Length Code. It configures MOFCR register.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_GetDataLengthCode()
 *
 */

__STATIC_INLINE void Cy_CAN_MO_SetDataLengthCode(Cy_CAN_MO_t *const can_mo, const uint8_t data_length_code)
{
  can_mo->can_data_length = data_length_code;
  can_mo->can_mo_ptr->MOFCR = ((can_mo->can_mo_ptr->MOFCR ) & ~(uint32_t)(CAN_MO_MOFCR_DLC_Msk)) |
                              (((uint32_t)data_length_code << CAN_MO_MOFCR_DLC_Pos) & (uint32_t)CAN_MO_MOFCR_DLC_Msk);
}

/**
 *
 * @param can_mo           Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return Data length code
 *
 * \par<b>Description:</b><br>
 *   Gets the Data Length Code.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_SetDataLengthCode()
 *
 */

__STATIC_INLINE uint8_t Cy_CAN_MO_GetDataLengthCode(const Cy_CAN_MO_t *const can_mo)
{
  return (((can_mo->can_mo_ptr->MOFCR) & (uint32_t)(CAN_MO_MOFCR_DLC_Msk)) >> CAN_MO_MOFCR_DLC_Pos);
}

/**
 *
 * @param can_mo            Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param can_identifier  Identifier.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures CAN Message Object Identifier. It configures MOAR register.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_GetIdentifier()
 *
 */

void Cy_CAN_MO_SetIdentifier(Cy_CAN_MO_t *const can_mo, const uint32_t can_identifier);

/**
 *
 * @param can_mo            Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return CAN MO identifier
 *
 * \par<b>Description:</b><br>
 *  Gets the Identifier of the MO
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_SetIdentifier()
 *
 */

uint32_t Cy_CAN_MO_GetIdentifier(const Cy_CAN_MO_t *const can_mo);

/**
 *
 * @param can_mo            Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return Acceptance mask
 *
 * \par<b>Description:</b><br>
 *  Gets the acceptance mask for the CAN MO.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_SetAcceptanceMask()
 *
 */

uint32_t Cy_CAN_MO_GetAcceptanceMask(const Cy_CAN_MO_t *const can_mo);

/**
 *
 * @param can_mo            Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param can_id_mask     CAN MO acceptance mask.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets the acceptance mask of the MO
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_GetAcceptanceMask()
 *
 */

void Cy_CAN_MO_SetAcceptanceMask(Cy_CAN_MO_t *const can_mo, const uint32_t can_id_mask);

/**
 *
 * @param can_mo            Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Message object receives frames only with matching IDE bit.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_AcceptStandardAndExtendedID()
 *
 */

__STATIC_INLINE void Cy_CAN_MO_AcceptOnlyMatchingIDE(Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_ide_mask = 1U;
  can_mo->can_mo_ptr->MOAMR |= (uint32_t)(CAN_MO_MOAMR_MIDE_Msk);
}

/**
 *
 * @param can_mo            Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Message object accepts the reception of both, standard and extended frames.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_AcceptOnlyMatchingIDE()
 *
 */

__STATIC_INLINE void Cy_CAN_MO_AcceptStandardAndExtendedID(Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_ide_mask = 0U;
  can_mo->can_mo_ptr->MOAMR &= ~(uint32_t)(CAN_MO_MOAMR_MIDE_Msk);
}

/**
 *
 * @param can_mo            Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Message object handles standard frames with 11-bit identifier.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_SetExtendedID()
 *
 * \par<b>Note:</b><br>
 * After setting the identifier type user has to set the identifier value by using @ref Cy_CAN_MO_SetIdentifier() API.
 */

__STATIC_INLINE void Cy_CAN_MO_SetStandardID(Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_id_mode = (uint32_t)CY_CAN_FRAME_TYPE_STANDARD_11BITS;
  can_mo->can_mo_ptr->MOAR &= ~(uint32_t)(CAN_MO_MOAR_IDE_Msk);
}

/**
 *
 * @param can_mo            Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Message object handles extended frames with 29-bit identifier.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_MO_SetStandardID()
 *
 * \par<b>Note:</b><br>
 * After setting the identifier type user has to set the identifier value by using @ref Cy_CAN_MO_SetIdentifier() API.
 *
 */

__STATIC_INLINE void Cy_CAN_MO_SetExtendedID(Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_id_mode = (uint32_t)CY_CAN_FRAME_TYPE_EXTENDED_29BITS;
  can_mo->can_mo_ptr->MOAR |= (uint32_t)CAN_MO_MOAR_IDE_Msk;
}

/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configuration allows to enable the FIFO Foreign Remote Request. This Specifies TXRQ of the message object referenced
 *  by the pointer CUR is set on reception of a matching Remote Frame. It configures MOFCR.FRREN register.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_FIFO_DisableForeingRemoteRequest()\n\n\n
 *
 */

__STATIC_INLINE void Cy_CAN_FIFO_EnableForeignRemoteRequest(const Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_mo_ptr->MOFCR |= (uint32_t)CAN_MO_MOFCR_FRREN_Msk;
}

/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configuration allows to disable the FIFO Foreign Remote Request. TXRQ of message object n is set on reception
 * of a matching Remote Frame. It configures MOFCR.FRREN register.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_FIFO_EnableForeignRemoteRequest()\n\n\n
 *
 */

__STATIC_INLINE void Cy_CAN_FIFO_DisableForeingRemoteRequest(const Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_mo_ptr->MOFCR &= ~(uint32_t)CAN_MO_MOFCR_FRREN_Msk;
}


/**
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configuration allows to enable the FIFO Remote Monitoring. This Specifies identifier, IDE(Identifier Extension) bit,
 * and DLC of a matching Remote Frame are copied to transmit object n in order to monitor incoming Remote Frames.
 * It configures MOFCR.RMM bit.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_FIFO_DisableRemoteMonitoring()\n\n\n
 *
 * \par<b>Note:</b><br>
 *  Remote monitoring(RMM) applies only to transmit objects and has no effect on receive objects.
 *
 */
__STATIC_INLINE void Cy_CAN_FIFO_EnableRemoteMonitoring(const Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_mo_ptr->MOFCR |= (uint32_t)CAN_MO_MOFCR_RMM_Msk;
}


/**
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configuration allows to disable the FIFO Remote Monitoring. This Specifies Identifier, Identifier Extension bit,
 * and DLC of message object n remain unchanged upon the reception of a matching Remote Frame.
 * It configures MOFCR.RMM bit.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_FIFO_EnableRemoteMonitoring()\n\n\n
 *
 * \par<b>Note:</b><br>
 *  Remote monitoring(RMM) applies only to transmit objects and has no effect on receive objects.
 *
 */

__STATIC_INLINE void Cy_CAN_FIFO_DisableRemoteMonitoring(const Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_mo_ptr->MOFCR &= ~(uint32_t)CAN_MO_MOFCR_RMM_Msk;
}


/**
 * @param can_mo     Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param select_pointer Selected Message Object number. Range:0-63
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set Object Select Pointer. If the current pointer CUR of FIFO base object becomes equal \a select_pointer,
 * a FIFO overflow interrupt request is generated. Used for FIFO monitoring purpose.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE void Cy_CAN_FIFO_SetSELMO(const Cy_CAN_MO_t *const can_mo, const uint8_t select_pointer)
{
  can_mo->can_mo_ptr->MOFGPR = ((can_mo->can_mo_ptr->MOFGPR ) & ~(uint32_t)(CAN_MO_MOFGPR_SEL_Msk)) |
                               (((uint32_t)select_pointer << CAN_MO_MOFGPR_SEL_Pos) & (uint32_t)CAN_MO_MOFGPR_SEL_Msk);
}


/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return uint8_t  Current Message Object Number. Range:0-63
 *
 * \par<b>Description:</b><br>
 * Returns the current FIFO Message Object,points to the actual target object within a FIFO/Gateway structure.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE uint8_t Cy_CAN_FIFO_GetCurrentMO(const Cy_CAN_MO_t *const can_mo)
{
  return (uint8_t)((uint32_t)(can_mo->can_mo_ptr->MOFGPR & CAN_MO_MOFGPR_CUR_Msk) >> CAN_MO_MOFGPR_CUR_Pos);
}


/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configuration allows to enable the Single Data Transfer of the FIFO Message Object. If SDT = 1 and message object n
 * is not a FIFO base object, then MSGVAL is reset when this object has taken part in a successful data transfer
 * (receive or transmit). If SDT = 1 and message object n is a FIFO base object, then MSGVAL is reset when the pointer
 * to the current object CUR reaches the value of SEL in the FIFO/Gateway Pointer Register. It configures MOFCR.SDT bit.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_FIFO_DisableSingleDataTransfer()
 *
 */

__STATIC_INLINE void Cy_CAN_FIFO_EnableSingleDataTransfer(const Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_mo_ptr->MOFCR |= (uint32_t)CAN_MO_MOFCR_SDT_Msk;
}


/**
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configuration allows to disable the Single Data Transfer of the FIFO Message Object, with this configuration bit
 *  MSGVAL is not affected. It configures MOFCR.SDT bit.
 *
 * \par<b>Related API's:</b><br>
 * Cy_CAN_FIFO_EnableSingleDataTransfer()
 *
 */

__STATIC_INLINE void Cy_CAN_FIFO_DisableSingleDataTransfer(const Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_mo_ptr->MOFCR &= ~(uint32_t)CAN_MO_MOFCR_SDT_Msk;
}


/**
 *
 * @param can_mo   Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param can_fifo CAN FIFO configuration data structure. Refer @ref Cy_CAN_FIFO_CONFIG_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures the transmit FIFO. A FIFO consists of one base object and n slave objects. Please refer
 *  reference manual \b Transmit FIFO for more info.
 *
 * \par<b>Related API's:</b><br>
 * None.
 *
 */

void Cy_CAN_TXFIFO_ConfigMOBaseObject(const Cy_CAN_MO_t *const can_mo, const Cy_CAN_FIFO_CONFIG_t can_fifo);


/**
 *
 * @param can_mo   Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param can_fifo CAN FIFO configuration data structure. Refer @ref Cy_CAN_FIFO_CONFIG_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures the base object of receive FIFO.
 *
 * \par<b>Related API's:</b><br>
 *  None
 */

void Cy_CAN_RXFIFO_ConfigMOBaseObject(const Cy_CAN_MO_t *const can_mo, const Cy_CAN_FIFO_CONFIG_t can_fifo);


/**
 *
 * @param can_mo  Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param can_fifo  CAN FIFO configuration data structure. Refer @ref Cy_CAN_FIFO_CONFIG_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the slave object of transmit FIFO.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */
void Cy_CAN_TXFIFO_ConfigMOSlaveObject(const Cy_CAN_MO_t *const can_mo, const Cy_CAN_FIFO_CONFIG_t can_fifo);


/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures the slave Object of receive FIFO. It configures MOCTR.RESRXEN bit.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE void Cy_CAN_RXFIFO_ConfigMOSlaveObject(const Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_mo_ptr->MOCTR = (uint32_t)CAN_MO_MOCTR_RESRXEN_Msk;
}


/**
 *
 * @param can_mo    Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 * @param can_gateway CAN  gateway configuration data structure. Refer Cy_CAN_GATEWAY_CONFIG_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures the Gateway source object. The Gateway Mode \a can_gateway makes it possible to establish an automatic
 *  information transfer between two independent CAN buses without CPU interaction. Please refer reference manual
 *  \b GatewayMode for more info.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

void Cy_CAN_GATEWAY_InitSourceObject(const Cy_CAN_MO_t *const can_mo, const Cy_CAN_GATEWAY_CONFIG_t can_gateway);


/**
 *
 * @param can_mo    Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Configures the Gateway destination object. The Gateway Mode \a can_gateway makes it possible to establish an automatic
 *  information transfer between two independent CAN buses without CPU interaction. Please refer reference manual
 *  \b GatewayMode for more info.
 *
 * \par<b>Related API's:</b><br>
 * None
 *
 */

__STATIC_INLINE void Cy_CAN_GATEWAY_InitDesObject(const Cy_CAN_MO_t *const can_mo)
{
  can_mo->can_mo_ptr->MOCTR  = CAN_MO_MOCTR_RESRXEN_Msk |
                               CAN_MO_MOCTR_RESNEWDAT_Msk;
}

/**
 *
 * @param can_mo Pointer to Message Object structure. Refer @ref Cy_CAN_MO_t data structure for details.
 *
 * @return Cy_CAN_STATUS_t CAN Node status. Refer @ref Cy_CAN_STATUS_t structure for details.
 *
 * \par<b>Description:</b><br>
 *  To transmit Message Object from the FIFO. Prior to this CAN node Message Object FIFO structure shall be made using
 *  Cy_CAN_TXFIFO_ConfigMOBaseObject(), Cy_CAN_TXFIFO_ConfigMOSlaveObject(),Cy_CAN_RXFIFO_ConfigMOBaseObject() API's.
 *  Please refer reference manual \b MessageObject \b FIFOStructure for more info.
 *
 *
 */
Cy_CAN_STATUS_t Cy_CAN_TXFIFO_Transmit(const Cy_CAN_MO_t *const can_mo);

/**
 * @param obj Pointer pointing to CY_CAN Global Initialization structure
 * @param list_idx List index
 *
 * @return List empty status.
 *
 * \par<b>Description:</b><br>
 * This function returns false in case at least one message object is allocated to list list_idx.
 */
__STATIC_INLINE bool Cy_CAN_IsListEmpty(Cy_CAN_t *const obj, uint8_t list_idx)
{
  return (bool)(obj->LIST[list_idx + 1] & CAN_LIST_EMPTY_Msk);
}

/**
 * @param obj Pointer pointing to CY_CAN Global Initialization structure
 * @param list_idx List index
 *
 * @return Object index of the first message object in the list list_idx.
 *
 * \par<b>Description:</b><br>
 * This function returns object index of the first message object in the list list_idx.
 */
__STATIC_INLINE uint32_t Cy_CAN_GetListBegin(Cy_CAN_t *const obj, uint8_t list_idx)
{
  return ((obj->LIST[list_idx + 1] & CAN_LIST_BEGIN_Msk) >> CAN_LIST_BEGIN_Pos);
}

/**
 * @param obj Pointer pointing to CY_CAN Global Initialization structure
 * @param list_idx List index
 *
 * @return Number of message objects allocated to list list_idx
 *
 * \par<b>Description:</b><br>
 * This function returns number of message objects allocated to list list_idx
 */
__STATIC_INLINE uint32_t Cy_CAN_GetListSize(Cy_CAN_t *const obj, uint8_t list_idx)
{
  return ((obj->LIST[list_idx + 1] & CAN_LIST_SIZE_Msk) >> CAN_LIST_SIZE_Pos) + 1;
}

/**
 * @param can_mo Pointer to Message Object structure.
 * @param mo_idx Message object index.
 *
 * @return None.
 *
 * \par<b>Description:</b><br>
 * This function updates the hardware message object pointer to message object with index mo_idx
 */
__STATIC_INLINE void Cy_CAN_MO_GetMessageObject(Cy_CAN_MO_t *const can_mo, uint8_t mo_idx)
{
#if defined(CAN_MO0)
  CAN_MO_TypeDef *can_mo_ptr = &CAN_MO0[mo_idx];
#elif defined(CAN_MO)
  CAN_MO_TypeDef *can_mo_ptr = &CAN_MO->MO[mo_idx];
#endif
  can_mo->can_mo_ptr = can_mo_ptr;
}

/**
 * @param can_mo Pointer to Message Object structure.
 *
 * @return object index of next linked message object.
 *
 * \par<b>Description:</b><br>
 * This function returns the object index of next linked message object.
 */
__STATIC_INLINE uint32_t Cy_CAN_MO_GetNextMessageObjectIdx(Cy_CAN_MO_t *can_mo)
{
  return ((can_mo->can_mo_ptr->MOSTAT & CAN_MO_MOSTAT_PNEXT_Msk) >> CAN_MO_MOSTAT_PNEXT_Pos);
}

#ifdef __cplusplus
}
#endif

/**
  * @}
  */


#endif /* defined(CAN) */

#endif /* CY_CAN_H */
