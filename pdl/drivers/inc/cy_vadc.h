/**
 * @file cy_vadc.h
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

#ifndef CY_VADC_H
#define CY_VADC_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_common.h"
#include "cy_scu.h"
#include "cy_vadc_map.h"


/**
 * @addtogroup VADC
 * @brief Versatile Analog to Digital Converter (VADC) driver for PSC1M microcontroller family.
 *
 * The PSC1M microcontroller provides a series of analog input channels connected to a cluster of Analog/Digital
 * Converters using the Successive Approximation Register (SAR) principle to convert analog input values (voltages)
 * to discrete digital values.
 * \if CY_FAMILY_PSC1
 * The PCS1M is based on Sample & Hold converters, where a cluster contains 2 Sample&Hold units which share a common
 * converter.
 * \endif
 *
 * Each converter of the ADC cluster can operate independent of the others, controlled by a dedicated set of
 * registers and triggered by a dedicated group request source. The results of each channel can be stored in a
 * dedicated channel-specific result register or in a group-specific result register.<BR>
 *
 * The Versatile Analog to Digital Converter module (VADC) of the PSC1M comprises a set of converter blocks that
 * can be operated either independently or via a common request source that emulates a background converter.
 * Each converter block is equipped with a dedicated input multiplexer and dedicated request sources,
 * which together build separate groups.
 *
 * @image html "vadc_overview_psc1.png"
 *
 * The VADC LLD is split into GLOBAL and GROUP related APIs.<BR>
 * <B>GLOBAL: </B><BR>
 * <UL>
 * <LI>Global APIs act on the entire ADC module. Configures global configuration registers</LI>
 * <LI>Allows configuration of the background request source of the VADC.</LI>
 * <LI>The clock related configurations for the VADC module are configured in the Global APIs/</LI>
 * <LI>The Global API names are prefixed by the \b CY_VADC_GLOBAL_ and they accept ::Cy_VADC_GLOBAL_t as
 *     one of its arguments.</LI>
 * <LI>Configures the background request source of the VADC. The APIs which act on the background related registers
 *     are prefixed by \b Cy_VADC_GLOBAL_Background</LI>
 * <LI>Configures the sample and hold unit of the VADC. The APIs which act on the SHS related registers
 *     are prefixed by \b CY_VADC_GLOBAL_SHS_</LI>
 * </UL><BR>
 *
 * <B>GROUP: </B><BR>
 * <UL>
 * <LI>Group APIs act on a VADC group. Configures the group configuration registers</LI>
 * <LI>Configures the queue request source of the VADC. The APIs which act on the queue related registers
 *     are prefixed by \b Cy_VADC_GROUP_Queue</LI>
 * <LI>Configures the scan request source of the VADC. The APIs which act on the scan related registers
 *     are prefixed by \b Cy_VADC_GROUP_Scan</LI>
 * <LI>Configuration of the channels of each group are done by the API which have a prefix as
 *     \b Cy_VADC_GROUP_Channel.</LI>
 * <LI>The Group API names are prefixed by the \b CY_VADC_GROUP_ and they accept ::Cy_VADC_GROUP_t as
 *     one of its arguments.</LI>
 * </UL><BR>
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#define CY_VADC_GROUP_AVAILABLE         (1U) /*  Defines the availability of group resource in a device*/
#define CY_VADC_GSCAN_AVAILABLE         (1U) /*  Defines the availability of scan request resource in a device*/
#define CY_VADC_QUEUE_AVAILABLE         (1U) /*  Defines the availability of queue request resource in a device*/
#define CY_VADC_EMUX_AVAILABLE          (1U) /*  Defines the availability of external multiplexer support in a
                                                    device*/
#define CY_VADC_BOUNDARY_AVAILABLE      (1U) /*  Defines the availability of boundary check support in a device*/
#define CY_VADC_MULTIPLE_SLAVEGROUPS    (1U) /*  Defines the availability of synchronous request source in device*/
#define CY_VADC_MAXIMUM_NUM_GROUPS      (2U) /*  Defines the maximum number of groups available in a device*/
#define CY_VADC_BOUNDARY_FLAG_SELECT    (1U) /*  Defines the availability of boundary flags in a device*/
#define CY_VADC_EMUX_CH_SEL_STYLE       (1U) /*  Defines the external multiplexer channel selection mode of
                                                    operation for a particular device*/
#define CY_VADC_SHS_AVAILABLE           (1U) /* Defines the availability of sample and hold module*/
#define CY_VADC_SHS_FULL_SET_REG        (1U) /* Defines the availability of sample and hold module*/
#define CY_VADC_GROUP_SRCREG_AVAILABLE  (1U) /* Define the availability of a source specific result register */
#define CY_VADC_RESULT_PRIORITY_AVAILABLE (1U) /* Define the availability of a priority for result register */

#define CY_VADC_NUM_PORTS               (16U) /* Defines the number of hardware ports that can be configured
                                                    as triggers and gating signals */

#define CY_VADC_NUM_RESULT_REGISTERS    (16U) /* Defines the number of result holding registers per ADC group */

#define CY_VADC_NUM_CHANNELS_PER_GROUP  (8U)  /**< Defines the number of ADC channels per group */

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
typedef uint16_t Cy_VADC_RESULT_SIZE_t; /**< Type defined the converted result size to unsigned 16 bit integer */
typedef VADC_GLOBAL_TypeDef Cy_VADC_GLOBAL_t; /**< Type defined the device header file vadc global register structure
                                             type to VADC type*/

#if(CY_VADC_GROUP_AVAILABLE == 1U)
typedef VADC_G_TypeDef Cy_VADC_GROUP_t; /**< Type defined the device header file vadc group register structure
                                             type to VADC Group type*/
#endif

#if(CY_VADC_SHS_AVAILABLE == 1U)
typedef SHS_Type Cy_VADC_GLOBAL_SHS_t; /**< Type defined the sample and hold register structure*/
#endif
/**
 *  Defines the return status after execution of VADC specific API's. Use @ref Cy_VADC_STATUS_t for this enumeration.
 */
typedef enum CY_VADC_STATUS
{
  CY_VADC_STATUS_SUCCESS = 0,   /**< Returned when the API has been able to fulfill the callers request */
  CY_VADC_STATUS_ERROR          /**< Returned when the API cannot fulfill the request */
} Cy_VADC_STATUS_t;

/**
 *  Defines the various service requests lines. Each group can raise up to 4 service requests independently. While
 *  all groups together have the possibility of raising 4 module wide service requests. Use @ref Cy_VADC_SR_t for this
 *  enumeration.
 */
typedef enum CY_VADC_SR
{
  CY_VADC_SR_GROUP_SR0 = 0, /**< Group specific Service Request-0 */
  CY_VADC_SR_GROUP_SR1,     /**< Group specific Service Request-1 */
  CY_VADC_SR_GROUP_SR2,     /**< Group specific Service Request-2 */
  CY_VADC_SR_GROUP_SR3,     /**< Group specific Service Request-3 */
  CY_VADC_SR_SHARED_SR0,    /**< Module Wide Common Service Request-0 */
  CY_VADC_SR_SHARED_SR1,    /**< Module Wide Common Service Request-1 */
  CY_VADC_SR_SHARED_SR2,    /**< Module Wide Common Service Request-2 */
  CY_VADC_SR_SHARED_SR3    /**< Module Wide Common Service Request-3 */
} Cy_VADC_SR_t;

/**
 *  Defines the mode of operation of a channel, when an ongoing conversion gets interrupted in between.
 *  Use @ref Cy_VADC_STARTMODE_t for this enumeration.
 */
typedef enum CY_VADC_STARTMODE
{
  CY_VADC_STARTMODE_WFS = 0, /**< An ongoing conversion completes without interruption */
  CY_VADC_STARTMODE_CIR,     /**< An ongoing conversion can be interrupted and resumed later*/
  CY_VADC_STARTMODE_CNR     /**< An ongoing conversion can be interrupted and never resumed */
} Cy_VADC_STARTMODE_t;

/**
 *  Defines the edge sensitivity of the trigger signal which can assert a conversion.
 *  Use @ref Cy_VADC_TRIGGER_EDGE_t for this enumeration.
 */
typedef enum CY_VADC_TRIGGER_EDGE
{
  CY_VADC_TRIGGER_EDGE_NONE = 0, /**< No external trigger. Conversion request can be asserted by software */
  CY_VADC_TRIGGER_EDGE_FALLING,  /**< The falling edge of the external trigger can assert conversion request */
  CY_VADC_TRIGGER_EDGE_RISING,   /**< The rising edge of the external trigger can assert conversion request */
  CY_VADC_TRIGGER_EDGE_ANY       /**< Both the edges can assert conversion request */
} Cy_VADC_TRIGGER_EDGE_t;

/**
 *  Defines the external trigger input selection possibilities, to assert a conversion. Refer the VADC interconnects
 *  section of the reference manual for details of peripherals which can be used. Also refer cy_vadc_map.h file for
 *  detailed definitions of the peripherals which can take the control of these enumeration items.
 *  Use @ref Cy_VADC_TRIGGER_INPUT_SELECT_t for this enumeration.
 */
typedef enum CY_VADC_TRIGGER_INPUT_SELECT
{
  CY_VADC_REQ_TR_A = 0, /**< Trigger select signal A */
  CY_VADC_REQ_TR_B,     /**< Trigger select signal B */
  CY_VADC_REQ_TR_C,     /**< Trigger select signal C */
  CY_VADC_REQ_TR_D,     /**< Trigger select signal D */
  CY_VADC_REQ_TR_E,     /**< Trigger select signal E */
  CY_VADC_REQ_TR_F,     /**< Trigger select signal F */
  CY_VADC_REQ_TR_G,     /**< Trigger select signal G */
  CY_VADC_REQ_TR_H,     /**< Trigger select signal H */
  CY_VADC_REQ_TR_I,     /**< Trigger select signal I */
  CY_VADC_REQ_TR_J,     /**< Trigger select signal J */
  CY_VADC_REQ_TR_K,     /**< Trigger select signal K */
  CY_VADC_REQ_TR_L,     /**< Trigger select signal L */
  CY_VADC_REQ_TR_M,     /**< Trigger select signal M */
  CY_VADC_REQ_TR_N,     /**< Trigger select signal N */
  CY_VADC_REQ_TR_O,     /**< Trigger select signal O */
  CY_VADC_REQ_TR_P      /**< Trigger select signal P */

} Cy_VADC_TRIGGER_INPUT_SELECT_t;

/**
 *  Defines the external gating input selection possibilities, to gate the conversion requests. Refer the VADC
 *  interconnects section of the reference manual for details of peripherals which can be used. Also refer
 *  cy_vadc_map.h file for detailed definitions of the peripherals which can take the control of these enumeration
 *  items. Use @ref Cy_VADC_GATE_INPUT_SELECT_t for this enumeration.
 */
typedef enum CY_VADC_GATE_INPUT_SELECT
{
  CY_VADC_REQ_GT_A = 0, /**< Gating select signal A */
  CY_VADC_REQ_GT_B,     /**< Gating select signal B */
  CY_VADC_REQ_GT_C,     /**< Gating select signal C */
  CY_VADC_REQ_GT_D,     /**< Gating select signal D */
  CY_VADC_REQ_GT_E,     /**< Gating select signal E */
  CY_VADC_REQ_GT_F,     /**< Gating select signal F */
  CY_VADC_REQ_GT_G,     /**< Gating select signal G */
  CY_VADC_REQ_GT_H,     /**< Gating select signal H */
  CY_VADC_REQ_GT_I,     /**< Gating select signal I */
  CY_VADC_REQ_GT_J,     /**< Gating select signal J */
  CY_VADC_REQ_GT_K,     /**< Gating select signal K */
  CY_VADC_REQ_GT_L,     /**< Gating select signal L */
  CY_VADC_REQ_GT_M,     /**< Gating select signal M */
  CY_VADC_REQ_GT_N,     /**< Gating select signal N */
  CY_VADC_REQ_GT_O,     /**< Gating select signal O */
  CY_VADC_REQ_GT_P      /**< Gating select signal P */

} Cy_VADC_GATE_INPUT_SELECT_t;

/**
 * Defines the condition for gating the conversion requests. It can be used to set the ENGT field
 * of ASMR/BSMR/QMR register respectively for auto_scan/background_scan/queue request sources.
 * Use @ref Cy_VADC_GATEMODE_t for this enumeration.
 */
typedef enum CY_VADC_GATEMODE
{
  CY_VADC_GATEMODE_BLOCK = 0,  /**< External triggers are permanently blocked */
  CY_VADC_GATEMODE_IGNORE,     /**< External triggers are unconditionally passed */
  CY_VADC_GATEMODE_ACTIVEHIGH, /**< External trigger is passed only if the gate signal is high */
  CY_VADC_GATEMODE_ACTIVELOW  /**< External trigger is passed only if the gate signal is low */
} Cy_VADC_GATEMODE_t;

/**
 * Defines the conversion result handling mode. Use @ref Cy_VADC_DMM_t for this enumeration.
 */
typedef enum CY_VADC_DMM
{
  CY_VADC_DMM_REDUCTION_MODE = 0, /**< Standard Data reduction mode*/
  CY_VADC_DMM_FILTERING_MODE,     /**< Provide option to select Finite Impulse Response Filter (FIR) or
                                        Infinite Impulse Response Filter (IIR)*/
  CY_VADC_DMM_DIFFERENCE_MODE,    /**< Difference mode is selected*/
} Cy_VADC_DMM_t;

/**
 *  Defines the conversion mode. It defines the resolution of conversion. Use Cy_VADC_CONVMODE_t for this enumeration.
 */
typedef enum CY_VADC_CONVMODE
{
  CY_VADC_CONVMODE_12BIT       = 0,  /**< Results of conversion are 12bits wide */
  CY_VADC_CONVMODE_10BIT       = 1,  /**< Results of conversion are 10bits wide */
  CY_VADC_CONVMODE_8BIT        = 2,  /**< Results of conversion are 8bits wide */
  CY_VADC_CONVMODE_FASTCOMPARE = 5  /**< Input signal compared with a preset range */
} Cy_VADC_CONVMODE_t;

/**
 *  Defines the output of a fast compare mode. Use @ref Cy_VADC_FAST_COMPARE_t for
 *  this enumeration.
 */
typedef enum CY_VADC_FAST_COMPARE
{
  CY_VADC_FAST_COMPARE_LOW = 0, /**< Input lower than than programmed reference */
  CY_VADC_FAST_COMPARE_HIGH,    /**< Input higher than than programmed reference */
  CY_VADC_FAST_COMPARE_UNKNOWN  /**< Unknown, Conversion probably still ongoing */
} Cy_VADC_FAST_COMPARE_t;

/**
 * Defines the type of scan request source to be used. It can choose between auto scan and background scan request
 * source methods. Use @ref Cy_VADC_SCAN_TYPE_t for this enumeration.
 */
typedef enum CY_VADC_SCAN_TYPE
{
  CY_VADC_SCAN_TYPE_GROUPSCAN = 0, /**< Auto scan mode of operation selected. Also called as Group scan*/
  CY_VADC_SCAN_TYPE_BACKGROUND    /**< Background scan mode of operation selected. Also called as Global scan*/
} Cy_VADC_SCAN_TYPE_t;

/**
 * Defines the behavior of load event for the scan request source. Use @ref Cy_VADC_SCAN_LOAD_t for this enumeration.
 */
typedef enum CY_VADC_SCAN_LOAD
{
  CY_VADC_SCAN_LOAD_OVERWRITE = 0, /**< The old set of channels is discarded in favor of the new set
                                         awaiting conversion */
  CY_VADC_SCAN_LOAD_COMBINE       /**< The new set of channels are combined with the pending channels from
                                         previous set */
} Cy_VADC_SCAN_LOAD_t;

/**
 * Defines the conversion classes that can be selected for each channel. The enumeration members holds the group or
 * global classes. The conversion classes decides the properties of conversion, like resolution, sampling time etc
 * Use @ref Cy_VADC_CHANNEL_CONV_t for this enumeration.
 */

typedef enum CY_VADC_CHANNEL_CONV
{
  CY_VADC_CHANNEL_CONV_GROUP_CLASS0 = 0, /**< Conversion property set-0 specific to the group */
  CY_VADC_CHANNEL_CONV_GROUP_CLASS1,     /**< Conversion property set-1 specific to the group */
  CY_VADC_CHANNEL_CONV_GLOBAL_CLASS0,    /**< Conversion property set-0, Module wide */
  CY_VADC_CHANNEL_CONV_GLOBAL_CLASS1    /**< Conversion property set-1, Module wide */
} Cy_VADC_CHANNEL_CONV_t;

/**
 * Defines the references to boundary values used for limit checking feature. Each of these can be assigned as
 * either an upper bound or a lower bound. Use @ref Cy_VADC_CHANNEL_BOUNDARY_t for this enumeration.
 */

typedef enum CY_VADC_CHANNEL_BOUNDARY
{
  CY_VADC_CHANNEL_BOUNDARY_GROUP_BOUND0 = 0, /**< Group specific Boundary-0 value */
  CY_VADC_CHANNEL_BOUNDARY_GROUP_BOUND1,     /**< Group specific Boundary-1 value */
  CY_VADC_CHANNEL_BOUNDARY_GLOBAL_BOUND0,    /**< Module wide Boundary-0 value */
  CY_VADC_CHANNEL_BOUNDARY_GLOBAL_BOUND1    /**< Module wide Boundary-1 value */
} Cy_VADC_CHANNEL_BOUNDARY_t;

/**
 * Defines the voltage which the capacitor is charged to. Used in Broken wire detection feature. Use
 * @ref Cy_VADC_CHANNEL_BWDCH_t for this enumeration.
 */
typedef enum CY_VADC_CHANNEL_BWDCH
{
  CY_VADC_CHANNEL_BWDCH_VAGND = 0, /**< Capacitor pre-charged to ground*/
  CY_VADC_CHANNEL_BWDCH_VAREF     /**< Capacitor pre-charged to reference voltage*/
} Cy_VADC_CHANNEL_BWDCH_t;

/**
 *   Defines the criteria for event generation by the channel. Use @ref Cy_VADC_CHANNEL_EVGEN_t for this enumeration.
 */
typedef enum CY_VADC_CHANNEL_EVGEN
{
  CY_VADC_CHANNEL_EVGEN_NEVER     = 0,  /**< No event generated */
  CY_VADC_CHANNEL_EVGEN_INBOUND   = 1U, /**< Event generated when the result is within the normal range */
  CY_VADC_CHANNEL_EVGEN_COMPHIGH  = 1U, /**< Event generated when the result of fast compare operation is high */
  CY_VADC_CHANNEL_EVGEN_OUTBOUND  = 2U, /**< Event generated when the result is outside the normal range */
  CY_VADC_CHANNEL_EVGEN_COMPLOW   = 2U, /**< Event generated when the result result of fast compare operation is low */
  CY_VADC_CHANNEL_EVGEN_ALWAYS    = 3U /**< Event generated always after conversion - unconditionally */
} Cy_VADC_CHANNEL_EVGEN_t;

/**
 * Defines the reference voltage selection for conversion. Use @ref Cy_VADC_CHANNEL_REF_t for this enumeration.
 */
typedef enum CY_VADC_CHANNEL_REF
{
  CY_VADC_CHANNEL_REF_INTREF = 0, /**< Internal VARef */
  CY_VADC_CHANNEL_REF_ALT_CH0    /**< External voltage available on Channel-0 of the perticular group */
} Cy_VADC_CHANNEL_REF_t;

/**
 * Defines the criteria for boundary flag assertion. Use @ref Cy_VADC_CHANNEL_BOUNDARY_CONDITION_t for this
 * enumeration.
 */
typedef enum CY_VADC_CHANNEL_BOUNDARY_CONDITION
{
  CY_VADC_CHANNEL_BOUNDARY_CONDITION_ABOVE_BAND = 0, /**< Set Boundary condition criteria to assert above the band */
  CY_VADC_CHANNEL_BOUNDARY_CONDITION_BELOW_BAND     /**< Set Boundary condition criteria to assert below the band */
} Cy_VADC_CHANNEL_BOUNDARY_CONDITION_t;

/**
 * Defines the event which can lead to a global service request assertion. Use @ref Cy_VADC_GLOBAL_EVENT_t for this
 * enumeration.
 */
typedef enum CY_VADC_GLOBAL_EVENT
{
  CY_VADC_GLOBAL_EVENT_BKGNDSOURCE = VADC_GLOBEFLAG_SEVGLB_Msk, /**< Background scan request source event */
  CY_VADC_GLOBAL_EVENT_RESULT      = VADC_GLOBEFLAG_REVGLB_Msk  /**< Global result event */
} Cy_VADC_GLOBAL_EVENT_t;

/**
 * Defines the power modes of a VADC Group. Use @ref Cy_VADC_GROUP_POWERMODE_t for this enumeration.
 */
typedef enum CY_VADC_GROUP_POWERMODE
{
  CY_VADC_GROUP_POWERMODE_OFF       = 0, /**< Group is powered down */
  CY_VADC_GROUP_POWERMODE_RESERVED1,     /**< Reserved */
  CY_VADC_GROUP_POWERMODE_RESERVED2,     /**< Reserved */
  CY_VADC_GROUP_POWERMODE_NORMAL        /**< Group is powered up */
} Cy_VADC_GROUP_POWERMODE_t;

/**
 *  Defines the status of a VADC group (also known as kernel). Use @ref Cy_VADC_GROUP_STATE_t for this enumeration.
 */
typedef enum CY_VADC_GROUP_STATE
{
  CY_VADC_GROUP_STATE_IDLE = 0, /**< Idle and can convert if requested */
  CY_VADC_GROUP_STATE_BUSY      /**< Busy with an ongoing conversion */
} Cy_VADC_GROUP_STATE_t;

/**
 *  Defines the reference to sample time and conversion mode settings. Use @ref Cy_VADC_GROUP_CONV_t for this
 *  enumeration.
 */
typedef enum CY_VADC_GROUP_CONV
{
  CY_VADC_GROUP_CONV_STD = 0, /**< Settings pertaining to channels directly attached to VADC module */
  CY_VADC_GROUP_CONV_EMUX    /**< Settings pertaining to channels connected to VADC via EMUX */
} Cy_VADC_GROUP_CONV_t;

/**
 *  Defines the request source arbiter behavior. Use @ref Cy_VADC_GROUP_ARBMODE_t for this enumeration.
 */
typedef enum CY_VADC_GROUP_ARBMODE
{
  CY_VADC_GROUP_ARBMODE_ALWAYS = 0, /**< Arbiter runs all the time */
  CY_VADC_GROUP_ARBMODE_ONDEMAND    /**< Arbiter runs only if a conversion request is asserted by any of the
                                          request sources */
} Cy_VADC_GROUP_ARBMODE_t;

/**
 *  Defines the EMUX mode of operation. Use @ref Cy_VADC_GROUP_EMUXMODE_t for this enumeration.
 */
typedef enum CY_VADC_GROUP_EMUXMODE
{
  CY_VADC_GROUP_EMUXMODE_SWCTRL = 0,     /**< Perform EMUX in Software control mode*/
  CY_VADC_GROUP_EMUXMODE_STEADYMODE,     /**< Perform EMUX in Steady mode (Use EMUX set value)*/
  CY_VADC_GROUP_EMUXMODE_SINGLEMODE,     /**< Perform EMUX in Single step mode*/
  CY_VADC_GROUP_EMUXMODE_SEQUENCEMODE,   /**< Perform EMUX in Sequence mode*/
} Cy_VADC_GROUP_EMUXMODE_t;

/**
 *  Defines the EMUX channel selection encoding scheme. Use @ref Cy_VADC_GROUP_EMUXCODE_t for this enumeration.
 */
typedef enum CY_VADC_GROUP_EMUXCODE
{
  CY_VADC_GROUP_EMUXCODE_BINARY = 0, /**< A linearly incrementing code serves are MUX-SEL */
  CY_VADC_GROUP_EMUXCODE_GRAY       /**< The MUX-SEL is gray encoded */
} Cy_VADC_GROUP_EMUXCODE_t;

/**
 *  Defines the service request set used. Use @ref Cy_VADC_GROUP_IRQ_t for this enumeration.
 */
typedef enum CY_VADC_GROUP_IRQ
{
  CY_VADC_GROUP_IRQ_KERNEL = 0, /**< Refers to Group specific service request */
  CY_VADC_GROUP_IRQ_SHARED     /**< Refers to Module wide service request */
} Cy_VADC_GROUP_IRQ_t;

/**
 *  Defines the alignment of the converted result. Use @ref Cy_VADC_RESULT_ALIGN_t for this enumeration.
 */
typedef enum CY_VADC_RESULT_ALIGN
{
  CY_VADC_RESULT_ALIGN_LEFT = 0,  /**< Always align result to left */
  CY_VADC_RESULT_ALIGN_RIGHT      /**< Always align result to right */
} Cy_VADC_RESULT_ALIGN_t;

typedef enum CY_VADC_RESULT_SUBTRATION
{
  CY_VADC_RESULT_SUBTRATION_12BIT_LEFT_ALIGN  = 0U,  /**< Always align result to left */
  CY_VADC_RESULT_SUBTRATION_12BIT_RIGHT_ALIGN = 0U,  /**< Always align result to right */
  CY_VADC_RESULT_SUBTRATION_10BIT_LEFT_ALIGN  = 2U,  /**< Always align result to left */
  CY_VADC_RESULT_SUBTRATION_10BIT_RIGHT_ALIGN = 0U,  /**< Always align result to right */
  CY_VADC_RESULT_SUBTRATION_8BIT_LEFT_ALIGN   = 4U,  /**< Always align result to left */
  CY_VADC_RESULT_SUBTRATION_8BIT_RIGHT_ALIGN  = 0U,  /**< Always align result to right */
} Cy_VADC_RESULT_SUBTRATION_t;

/**
 *  Defines the request source arbitration priority. Use @ref Cy_VADC_GROUP_RS_PRIORITY_t for this enumeration.
 */
typedef enum CY_VADC_GROUP_RS_PRIORITY
{
  CY_VADC_GROUP_RS_PRIORITY_0 = 0, /**< Lowest priority for the request source*/
  CY_VADC_GROUP_RS_PRIORITY_1,     /**< Second lowest priority for the request source*/
  CY_VADC_GROUP_RS_PRIORITY_2,     /**< Second highest priority for the request source*/
  CY_VADC_GROUP_RS_PRIORITY_3,     /**< Highest priority for the request source*/
} Cy_VADC_GROUP_RS_PRIORITY_t;

/**
 *  Defines the various modes for the boundary flag. Use @ref Cy_VADC_GROUP_BOUNDARY_FLAG_MODE_t for this enumeration.
 */
typedef enum CY_VADC_GROUP_BOUNDARY_FLAG_MODE
{
  CY_VADC_GROUP_BOUNDARY_FLAG_MODE_DISABLED = 0,       /**< Disable boundary flag*/
  CY_VADC_GROUP_BOUNDARY_FLAG_MODE_ENABLED,            /**< Always enable boundary*/
  CY_VADC_GROUP_BOUNDARY_FLAG_MODE_ENABLED_ACTIVE_LOW, /**< Enable boundary flag when gate level is 0*/
  CY_VADC_GROUP_BOUNDARY_FLAG_MODE_ENABLED_ACTIVE_HIGH /**< Enable boundary flag when gate level is 1*/
} Cy_VADC_GROUP_BOUNDARY_FLAG_MODE_t;


/**
 *  Defines the boundary select for Channel. Use @ref Cy_VADC_GROUP_BOUNDARY_FLAG_MODE_t for this enumeration.
 */
typedef enum CY_VADC_BOUNDARY_SELECT
{
  CY_VADC_BOUNDARY_SELECT_LOWER_BOUND = 0U,       /**< Select the lower boundary*/
  CY_VADC_BOUNDARY_SELECT_UPPER_BOUND = 2U        /**< Selects the upper boundary*/
} Cy_VADC_BOUNDARY_SELECT_t;


/**
 * Defines the group indices. Use @ref Cy_VADC_GROUP_INDEX_t for this enumeration.
 */
typedef enum CY_VADC_GROUP_INDEX
{
  CY_VADC_GROUP_INDEX_0   = 0,
  CY_VADC_GROUP_INDEX_1,
#if (CY_VADC_MAXIMUM_NUM_GROUPS > 2U)
  CY_VADC_GROUP_INDEX_2,
  CY_VADC_GROUP_INDEX_3
#endif
} Cy_VADC_GROUP_INDEX_t;

/**
* Defines channel alias.
* All enum items are available for channels 0 and 1. Other Channels can accept only CY_VADC_CHANNEL_ALIAS_DISABLED.
*/
typedef enum CY_VADC_CHANNEL_ALIAS
{
  CY_VADC_CHANNEL_ALIAS_DISABLED = -1,
  CY_VADC_CHANNEL_ALIAS_CH0 = 0,
  CY_VADC_CHANNEL_ALIAS_CH1 = 1,
  CY_VADC_CHANNEL_ALIAS_CH2 = 2,
  CY_VADC_CHANNEL_ALIAS_CH3 = 3,
  CY_VADC_CHANNEL_ALIAS_CH4 = 4,
  CY_VADC_CHANNEL_ALIAS_CH5 = 5,
  CY_VADC_CHANNEL_ALIAS_CH6 = 6,
  CY_VADC_CHANNEL_ALIAS_CH7 = 7
} Cy_VADC_CHANNEL_ALIAS_t;

#if(CY_VADC_SHS_AVAILABLE == 1U)

/**
 * Selection of the reference voltage that is required for conversions (VREF).
 */
typedef enum CY_VADC_GLOBAL_SHS_AREF
{
  CY_VADC_GLOBAL_SHS_AREF_EXTERNAL_VDD_UPPER_RANGE = 0 << SHS_SHSCFG_AREF_Pos, /**< External reference, upper supply range, e.g. VDD >= 3.0V */
  CY_VADC_GLOBAL_SHS_AREF_INTERNAL_VDD_UPPER_RANGE = 2 << SHS_SHSCFG_AREF_Pos, /**< Internal reference, upper supply range, e.g. VDD >= 3.0V  */
  CY_VADC_GLOBAL_SHS_AREF_INTERNAL_VDD_LOWER_RANGE = 3 << SHS_SHSCFG_AREF_Pos, /**< Internal reference, lower supply range, e.g. VDD <  3.0V  */
} Cy_VADC_GLOBAL_SHS_AREF_t;

#if(CY_VADC_SHS_FULL_SET_REG == 1U)
/**
 * Defines the gain calibration selection.
 */
typedef enum CY_VADC_SHS_GAIN_LEVEL
{
  CY_VADC_SHS_GAIN_LEVEL_0 = SHS_CALOC0_CALOFFVAL0_Pos, /**< Select the calibration value for gain level 0 */
  CY_VADC_SHS_GAIN_LEVEL_1 = SHS_CALOC0_CALOFFVAL1_Pos, /**< Select the calibration value for gain level 1 */
  CY_VADC_SHS_GAIN_LEVEL_2 = SHS_CALOC0_CALOFFVAL2_Pos, /**< Select the calibration value for gain level 2 */
  CY_VADC_SHS_GAIN_LEVEL_3 = SHS_CALOC0_CALOFFVAL3_Pos  /**< Select the calibration value for gain level 3 */
} Cy_VADC_SHS_GAIN_LEVEL_t;
#endif

/**
 * Defines the Delta sigma loop.
 */
typedef enum CY_VADC_SHS_LOOP_CH
{
  CY_VADC_SHS_LOOP_CH_0 = SHS_LOOP_LPCH0_Pos, /**< Select Delta-sigma loop 0*/
  CY_VADC_SHS_LOOP_CH_1 = SHS_LOOP_LPCH1_Pos /**< Select Delta-sigma loop 1*/
} Cy_VADC_SHS_LOOP_CH_t;

/**
 * Provides the order in which the SHS should do the calibration
 */
typedef enum CY_VADC_GLOBAL_SHS_CALIBRATION_ORDER
{
  CY_VADC_GLOBAL_SHS_CALIBRATION_ORDER_POST_CONV = 0, /**< Calibration occur after conversion takes place */
  CY_VADC_GLOBAL_SHS_CALIBRATION_ORDER_PRE_CONV   /**< Calibration occur before conversion takes place */
} Cy_VADC_GLOBAL_SHS_CALIBRATION_ORDER_t;
#endif

#if (CY_VADC_BOUNDARY_FLAG_SELECT == 1U)
/**
 * Provides possible routing values for the boundary flag.
 */
typedef enum CY_VADC_BOUNDARY_NODE
{
  CY_VADC_BOUNDARY_NODE_COMMON_BOUNDARY_FLAG_0 = 0U, /**<Route the Group boundary flag to Common boundary flag 0 */
  CY_VADC_BOUNDARY_NODE_COMMON_BOUNDARY_FLAG_1,      /**<Route the Group boundary flag to Common boundary flag 1 */
  CY_VADC_BOUNDARY_NODE_COMMON_BOUNDARY_FLAG_2,      /**<Route the Group boundary flag to Common boundary flag 2 */
  CY_VADC_BOUNDARY_NODE_COMMON_BOUNDARY_FLAG_3,      /**<Route the Group boundary flag to Common boundary flag 3 */
  CY_VADC_BOUNDARY_NODE_COMMON_SR_LINE_0,  /**<Route the Group boundary flag to Common Service Request line 0 */
  CY_VADC_BOUNDARY_NODE_COMMON_SR_LINE_1,  /**<Route the Group boundary flag to Common Service Request line 1 */
  CY_VADC_BOUNDARY_NODE_COMMON_SR_LINE_2,  /**<Route the Group boundary flag to Common Service Request line 2 */
  CY_VADC_BOUNDARY_NODE_COMMON_SR_LINE_3   /**<Route the Group boundary flag to Common Service Request line 3 */
} Cy_VADC_BOUNDARY_NODE_t;
#endif

#if (CY_VADC_GROUP_AVAILABLE == 1U)
/**
 * Defines the ready signal selection
 */
typedef enum CY_VADC_SYNCTR_EVAL
{
  CY_VADC_SYNCTR_EVAL_1 = VADC_G_SYNCTR_EVALR1_Msk, /**<Mask to set the EVAL1 bits of SYNCTR */
#if (CY_VADC_MAXIMUM_NUM_GROUPS > 2U)
  CY_VADC_SYNCTR_EVAL_2 = VADC_G_SYNCTR_EVALR2_Msk, /**<Mask to set the EVAL2 bits of SYNCTR */
  CY_VADC_SYNCTR_EVAL_3 = VADC_G_SYNCTR_EVALR3_Msk  /**<Mask to set the EVAL3 bits of SYNCTR */
#endif
} Cy_VADC_SYNCTR_EVAL_t;
#endif
/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/
/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined(__TASKING__)
#pragma warning 586
#endif

/**
 *  Structure initializing the VADC scan request source. Use type @ref Cy_VADC_SCAN_CONFIG_t for this structure.
 */
typedef struct CY_VADC_SCAN_CONFIG
{
  uint32_t conv_start_mode  : 2;  /**< One converter is shared between the queue and scan request sources of the same
                                         group. This field determines how scan request source would request for
                                         conversion. Uses @ref Cy_VADC_STARTMODE_t */
  uint32_t req_src_priority : 2;  /**< Request source priority for the arbiter. If the Conversion start mode has been
                                         selected as Cancel inject repeat/Never mode then this field would determine the
                                         priority of scan request source. Uses @ref Cy_VADC_GROUP_RS_PRIORITY_t */
  union
  {
    struct
    {
#if(CY_VADC_GROUP_SRCREG_AVAILABLE == (1U))
      uint32_t src_specific_result_reg : 4;  /**< Use any one Group related result register as the destination
                                                    for all conversions results. To use the  individual result register
                                                    from each channel configuration, configure this field with 0x0 */
#else
      uint32_t                         : 4;
#endif
      uint32_t                         : 4;
      uint32_t trigger_signal          : 4;  /**< Select one of the 16 possibilities for trigger. Uses @ref
                                                      Cy_VADC_TRIGGER_INPUT_SELECT_t enumeration*/
      uint32_t                         : 1;
      uint32_t trigger_edge            : 2;  /**< Edge selection for trigger signal. Uses @ref
                                                      Cy_VADC_TRIGGER_EDGE_t */
      uint32_t                         : 1;
      uint32_t gate_signal             : 4;  /**< Select one of the 16 possibilities for gating. Uses @ref
                                                      Cy_VADC_GATE_INPUT_SELECT_t */
      uint32_t                         : 8;
      uint32_t timer_mode              : 1;  /**< Decides whether timer mode for equi-distant sampling shall be
                                                      activated or not.*/
      uint32_t                         : 3;
    };
    uint32_t asctrl;
  };
  union
  {
    struct
    {
      uint32_t                    : 2;
      uint32_t external_trigger   : 1;  /**< Conversions be initiated by external hardware trigger */
      uint32_t req_src_interrupt  : 1;  /**< Request source event can be generated after a conversion sequence*/
      uint32_t enable_auto_scan   : 1;  /**< Enables the continuous conversion mode. Conversion completion
                                                 of the last channel in a scan sequence will cause a load event. */
      uint32_t load_mode          : 1;  /**< Selects load event mode. Uses @ref Cy_VADC_SCAN_LOAD_t */
      uint32_t                    : 26;
    };
    uint32_t asmr;
  };
} Cy_VADC_SCAN_CONFIG_t;


typedef Cy_VADC_SCAN_CONFIG_t Cy_VADC_BACKGROUND_CONFIG_t; /**< Type defined the scan configuration
                                                                 structure for background Request Source*/

/**
 *  Structure initializing the VADC channel. Use type Cy_VADC_CHANNEL_CONFIG_t for this enumeration.
 */
typedef struct CY_VADC_CHANNEL_CONFIG
{
  union
  {
    struct
    {
      uint32_t input_class                : 2;  /**< Input conversion class selection.
                                                         Uses @ref Cy_VADC_CHANNEL_CONV_t */
      uint32_t                            : 2;
      uint32_t lower_boundary_select      : 2;  /**< Which boundary register serves as lower bound?
                                                         Accepts enum @ref Cy_VADC_CHANNEL_BOUNDARY_t */
      uint32_t upper_boundary_select      : 2;  /**< Which boundary register serves as upper bound?
                                                         Accepts enum @ref Cy_VADC_CHANNEL_BOUNDARY_t */
      uint32_t event_gen_criteria         : 2;  /**< When should an event be generated?
                                                         Uses @ref Cy_VADC_CHANNEL_EVGEN_t */
      uint32_t sync_conversion            : 1;  /**< Enables synchronous conversion for the configured channel*/
      uint32_t alternate_reference        : 1;  /**< Input reference voltage selection either VARef or CH-0.
                                                         Uses @ref Cy_VADC_CHANNEL_REF_t*/
      uint32_t                            : 4;
      uint32_t result_reg_number          : 4;  /**< Group result register number */
      uint32_t use_global_result          : 1;  /**< Use global result register for background request source channels */
      uint32_t result_alignment           : 1;  /**< Alignment of the results read in the result register.
                                                         Uses @ref Cy_VADC_RESULT_ALIGN_t */
      uint32_t                            : 6;
      uint32_t broken_wire_detect_channel : 2;  /**< Source to be used to charge the capacitor for BWD feature.
                                                         Uses @ref Cy_VADC_CHANNEL_BWDCH_t. */
      uint32_t broken_wire_detect         : 1;  /**< Configures extra phase before the capacitor is sampled. */
    };
    uint32_t chctr;
  };
  union
  {
    struct
    {
      uint32_t                           : 8;
#if (CY_VADC_BOUNDARY_FLAG_SELECT == 1U)
      uint32_t flag_output_condition_ch0 : 1; /**< Condition for which the boundary flag should change.
                                                       Uses @ref Cy_VADC_CHANNEL_BOUNDARY_CONDITION_t */
      uint32_t flag_output_condition_ch1 : 1; /**< Condition for which the boundary flag should change.
                                                         Uses @ref Cy_VADC_CHANNEL_BOUNDARY_CONDITION_t */
      uint32_t flag_output_condition_ch2 : 1; /**< Condition for which the boundary flag should change.
                                                         Uses @ref Cy_VADC_CHANNEL_BOUNDARY_CONDITION_t */
      uint32_t flag_output_condition_ch3 : 1; /**< Condition for which the boundary flag should change.
                                                         Uses @ref Cy_VADC_CHANNEL_BOUNDARY_CONDITION_t */
#else
      uint32_t                           : 4;
#endif
      uint32_t                           : 4;
#if (CY_VADC_BOUNDARY_FLAG_SELECT == 1U)
      uint32_t invert_boundary_flag_ch0  : 1; /**< Inverts boundary flag output.*/
      uint32_t invert_boundary_flag_ch1  : 1; /**< Inverts boundary flag output.*/
      uint32_t invert_boundary_flag_ch2  : 1; /**< Inverts boundary flag output.*/
      uint32_t invert_boundary_flag_ch3  : 1; /**< Inverts boundary flag output.*/

#else
      uint32_t boundary_flag_output_ch0  : 1; /**< Enable the boundary flag output on the specific channel.*/
      uint32_t boundary_flag_output_ch1  : 1; /**< Enable the boundary flag output on the specific channel.*/
      uint32_t boundary_flag_output_ch2  : 1; /**< Enable the boundary flag output on the specific channel.*/
      uint32_t boundary_flag_output_ch3  : 1; /**< Enable the boundary flag output on the specific channel.*/
#endif
      uint32_t                           : 12;
    };
    uint32_t bfl;
  };
#if (CY_VADC_BOUNDARY_FLAG_SELECT == 1U)
  union
  {
    struct
    {
      uint32_t boundary_flag_mode_ch0    : 4; /**< Specify the basic operation of boundary flag 0
                                                      Uses @ref  Cy_VADC_GROUP_BOUNDARY_FLAG_MODE_t*/
      uint32_t boundary_flag_mode_ch1    : 4; /**< Specify the basic operation of boundary flag 1
                                                      Uses @ref  Cy_VADC_GROUP_BOUNDARY_FLAG_MODE_t*/
      uint32_t boundary_flag_mode_ch2    : 4; /**< Specify the basic operation of boundary flag 2
                                                      Uses @ref  Cy_VADC_GROUP_BOUNDARY_FLAG_MODE_t*/
      uint32_t boundary_flag_mode_ch3    : 4; /**< Specify the basic operation of boundary flag 3
                                                      Uses @ref  Cy_VADC_GROUP_BOUNDARY_FLAG_MODE_t*/
      uint32_t                           : 16;
    };
    uint32_t bflc;
  };
#endif
  bool    channel_priority;  /**< Only non priority channels can be converted by Background Request Source */
  int8_t  alias_channel;     /**< Specifies the channel which has to be aliased with CH0/CH1 (which ever is applicable).
                                    Force the value to \b(int8_t)-1 to bypass alias feature.
                                    Uses @ref Cy_VADC_CHANNEL_ALIAS_t  for configuration.*/
} Cy_VADC_CHANNEL_CONFIG_t;

/**
 *  Structure to initialize a queue entry. Use type @ref Cy_VADC_QUEUE_ENTRY_t.
 */
typedef struct CY_VADC_QUEUE_ENTRY
{
  union
  {
    struct
    {
      uint32_t channel_num        : 5;  /**< Channel number associated with this queue entry.<BR>Range:[0x0 to 0x7] */
      uint32_t refill_needed      : 1;  /**< Conversion completed channel gets inserted back into the queue */
      uint32_t generate_interrupt : 1;  /**< Generates a queue request source event */
      uint32_t external_trigger   : 1;  /**< Conversion requests are raised on an external trigger. */
      uint32_t                    : 24;

    };
    uint32_t qinr0;
  };
} Cy_VADC_QUEUE_ENTRY_t;

/**
 *  Structure initializing a VADC queue request source. Use type @ref Cy_VADC_QUEUE_CONFIG_t.
 */
typedef struct CY_VADC_QUEUE_CONFIG
{
  uint32_t conv_start_mode  : 2;  /**< One converter is shared between the queue and scan request sources of the same
                                         group. This field determines how queue request source would request for
                                         conversion. Uses @ref Cy_VADC_STARTMODE_t */
  uint32_t req_src_priority : 2;  /**< Request source priority for the arbiter.Uses @ref Cy_VADC_GROUP_RS_PRIORITY_t */
  union
  {
    struct
    {
#if(CY_VADC_GROUP_SRCREG_AVAILABLE == (1U))
      uint32_t src_specific_result_reg : 4;  /**< Uses any one Group related result register as the destination
                                                    for all conversions results. To use the  individual result register
                                                    from each channel configuration, configure this field with 0x0 */
#else
      uint32_t                         : 4;
#endif
      uint32_t                         : 4;
      uint32_t trigger_signal          : 4;  /**< Select one of the 16 possibilities for trigger.
                                                      Uses @ref Cy_VADC_TRIGGER_INPUT_SELECT_t */
      uint32_t                         : 1;
      uint32_t trigger_edge            : 2;  /**< Edge selection for trigger signal.
                                                      Uses @ref Cy_VADC_TRIGGER_EDGE_t */
      uint32_t                         : 1;
      uint32_t gate_signal             : 4;  /**< Select one of the 16 possibilities for gating.
                                                      Uses @ref Cy_VADC_GATE_INPUT_SELECT_t */
      uint32_t                         : 8;
      uint32_t timer_mode              : 1;  /**< Timer mode for equi-distant sampling shall be activated or not?  */
      uint32_t                         : 3;
    };
    uint32_t qctrl0;
  };
  union
  {
    struct
    {
      uint32_t                   : 2;
      uint32_t external_trigger  : 1;  /**< Are external triggers supported? */
      uint32_t                   : 29;
    };
    uint32_t qmr0;
  };
} Cy_VADC_QUEUE_CONFIG_t;


/**
 *  Structure to initialize the global input class configuration. Configured parameters are sample time and
 *  conversion Mode.
 */
typedef struct CY_VADC_GLOBAL_CLASS
{
  union
  {
    struct
    {
      uint32_t sample_time_std_conv            : 5;  /**< Sample time for channels directly connected to VADC
                                                              <BR>Range: [0x0 to 0x1F] */
      uint32_t                                 : 3;
      uint32_t conversion_mode_standard        : 3;  /**< Conversion mode for channels directly connected to VADC.
                                                              Uses @ref Cy_VADC_CONVMODE_t */
      uint32_t                                 : 5;
#if(CY_VADC_EMUX_AVAILABLE == 1U)
      uint32_t sampling_phase_emux_channel     : 5;  /**< Sample time for channels connected via EMUX
                                                              <BR>Range: [0x0 to 0x1F] */
      uint32_t                                 : 3;
      uint32_t conversion_mode_emux            : 3;  /**< Conversion mode for channels connected via EMUX to VADC.
                                                              Uses @ref Cy_VADC_CONVMODE_t */
      uint32_t                                 : 5;
#else
      uint32_t                                 : 16;
#endif
    };
    uint32_t globiclass;
  };
} Cy_VADC_GLOBAL_CLASS_t;

#if (CY_VADC_GROUP_AVAILABLE != 0U)
/**
 *   Structure to initialize converter and arbiter clock configuration
 */
typedef struct CY_VADC_GLOBAL_CLOCK
{
  union
  {
    struct
    {

      uint32_t analog_clock_divider     : 5;  /**< Clock for the converter. <BR>Range: [0x0 to 0x1F] */
      uint32_t                          : 2;
      uint32_t msb_conversion_clock     : 1;  /**< Additional clock cycle for analog converter */
      uint32_t arbiter_clock_divider    : 2;  /**< Request source arbiter clock divider. <BR>Range: [0x0 to 0x3] */
      uint32_t                          : 5;
      uint32_t                          : 17;
    };
    uint32_t globcfg;
  };
} Cy_VADC_GLOBAL_CLOCK_t;
#endif


/**
 *   Structure to initialize the VADC Global functions
 */
typedef struct CY_VADC_GLOBAL_CONFIG
{
  union
  {
    struct
    {
      uint32_t boundary0  : 12;  /**< Boundary value for results comparison*/
      uint32_t            : 4;
      uint32_t boundary1  : 12;  /**< Boundary value for results comparison*/
      uint32_t            : 4;
    };
    uint32_t globbound;
  };
#if (CY_VADC_GROUP_AVAILABLE != 0U)
  Cy_VADC_GLOBAL_CLOCK_t clock_config; /**< ADC clock configurations*/
#endif
  Cy_VADC_GLOBAL_CLASS_t  class0;      /**< ADC input conversion configurations for GLOBICLASS[0]*/
  Cy_VADC_GLOBAL_CLASS_t  class1;      /**< ADC input conversion configurations for GLOBICLASS[1]*/
  union
  {
    struct
    {
      uint32_t                         : 16;
      uint32_t data_reduction_control  : 4;  /**< Data reduction stages */
      uint32_t                         : 4;
      uint32_t wait_for_read_mode      : 1;  /**< Results of the next conversion will not be overwritten in the
                                                      result register until the previous value is read*/
      uint32_t                         : 6;
      uint32_t event_gen_enable        : 1;  /**< Generates an event on availability of new result. */
    };
    uint32_t globrcr;
  };
  union
  {
    struct
    {
      uint32_t module_disable              : 1;  /**< Disables the module clock.*/
      uint32_t                             : 2;
      uint32_t disable_sleep_mode_control  : 1;  /**< Set it to true in order to disable the Sleep mode */
      uint32_t                             : 28;
    };
    uint32_t clc;
  };
} Cy_VADC_GLOBAL_CONFIG_t;


/**
 *  Structure to initialize the group input class configuration. Configured parameters are sample time and
 *  conversion Mode.
 */
typedef struct CY_VADC_GROUP_CLASS
{
  union
  {
    struct
    {
      uint32_t sample_time_std_conv            : 5;  /**< Sample time for channels directly connected to VADC
                                                              <BR>Range: [0x0 to 0x1F] */
      uint32_t                                 : 3;
      uint32_t conversion_mode_standard        : 3;  /**< Conversion mode for channels directly connected to VADC.
                                                              Uses @ref Cy_VADC_CONVMODE_t */
      uint32_t                                 : 5;
      uint32_t sampling_phase_emux_channel     : 5;  /**< Sample time for channels connected via EMUX
                                                              <BR>Range: [0x0 to 0x1F] */
      uint32_t                                 : 3;
      uint32_t conversion_mode_emux            : 3;  /**< Conversion mode for channels connected via EMUX to VADC.
                                                              Uses @ref Cy_VADC_CONVMODE_t */
      uint32_t                                 : 5;
    };
    uint32_t g_iclass0;
  };
} Cy_VADC_GROUP_CLASS_t;


/**
 *  EMUX related configuration structure.
 */
typedef struct CY_VADC_GROUP_EMUXCFG
{
  union
  {
    struct
    {
      uint32_t starting_external_channel : 3;  /**< External channel number to which the VADC will
                                                        generate a control signal (needed to select the analog input in
                                                        the analog multiplexer)*/
      uint32_t                           : 13;
#if (CY_VADC_EMUX_CH_SEL_STYLE == 1U)
      uint32_t connected_channel         : 10; /**< The Channel to which the EMUX is connected. */
#else
      uint32_t connected_channel         : 5;  /**< The Channel to which the EMUX is connected. */
      uint32_t                           : 5;
#endif
      uint32_t emux_mode                 : 2;  /**< Selects the external multiplexer modes: Steady, Single Mode, step etc
                                                        Uses @ref Cy_VADC_GROUP_EMUXMODE_t*/
      uint32_t emux_coding               : 1;  /**< Select Binary or Gray coding. Uses @ref Cy_VADC_GROUP_EMUXCODE_t*/
      uint32_t stce_usage                : 1;  /**< Use STCE for each conversion of an external channel */
#if (CY_VADC_EMUX_CH_SEL_STYLE == 1U)
      uint32_t emux_channel_select_style : 1;  /**< Selects the style of configuring the \b connected_channel
                                                        (Each bit represents the channel or entire field represents the channel number )  */
      uint32_t                           : 1;
#else
      uint32_t                           : 2;
#endif
    };
    uint32_t g_emuxctr;
  };
} Cy_VADC_GROUP_EMUXCFG_t;


/**
 *   Group Configuration Data Structures
 */

typedef struct CY_VADC_GROUP_CONFIG
{
  Cy_VADC_GROUP_EMUXCFG_t emux_config;    /**< External multiplexer related configurations */
  Cy_VADC_GROUP_CLASS_t class0;           /**< ADC input conversion configurations for GxICLASS[0]*/
  Cy_VADC_GROUP_CLASS_t class1;           /**< ADC input conversion configurations for GxICLASS[1]*/
  union
  {
    struct
    {
      uint32_t boundary0  : 12;  /**< Boundary value for results comparison*/
      uint32_t            : 4;
      uint32_t boundary1  : 12;  /**< Boundary value for results comparison*/
      uint32_t            : 4;
    };
    uint32_t g_bound;
  };
  union
  {
    struct
    {
      uint32_t                          : 4;
      uint32_t arbitration_round_length : 2;  /**< Number of arbiter slots to be considered */
      uint32_t                          : 1;
      uint32_t arbiter_mode             : 1;  /**< Arbiter mode - Select either Continuous mode or Demand based.
                                                      Uses @ref Cy_VADC_GROUP_ARBMODE_t */
      uint32_t                          : 24;
    };
    uint32_t g_arbcfg;
  };
} Cy_VADC_GROUP_CONFIG_t;

/**
 *   Structure to initialize VADC Group result register.
 */

typedef struct CY_VADC_RESULT_CONFIG
{
  union
  {
    struct
    {
      uint32_t                         : 16;
      uint32_t data_reduction_control  : 4;  /**< Configures the data reduction stages */
      uint32_t post_processing_mode    : 2;  /**< Result data processing mode. Uses @ref  Cy_VADC_DMM_t
                                                      For normal operation select
                                                      Cy_VADC_DMM_t::CY_VADC_DMM_REDUCTION_MODE
                                                      and data_reduction_control as 0*/
      uint32_t                         : 2;
      uint32_t wait_for_read_mode      : 1;  /**< Allow the conversion only after previous results are read*/
      uint32_t part_of_fifo            : 2;  /**< Make the result register a part of Result FIFO? */
      uint32_t                         : 4;
      uint32_t event_gen_enable        : 1;  /**< Generates an event on availability of new result. */
    };
    uint32_t g_rcr;
  };
} Cy_VADC_RESULT_CONFIG_t;

#if(CY_VADC_SHS_AVAILABLE == 1U)
#if(CY_VADC_SHS_FULL_SET_REG == 1U)
/**
 * Structure to initialize the Stepper configurations
 */
typedef struct CY_VADC_GLOBAL_SHS_STEP_CONFIG
{
  union
  {
    struct
    {
      uint32_t sh_unit_step0            : 3;  /**< Select a Sample and hold unit for the stepper's step number 0.
                                                      Uses @ref Cy_VADC_GROUP_INDEX_t*/
      uint32_t enable_step0             : 1; /**< Should the step be added to the sequence */
      uint32_t sh_unit_step1            : 3;  /**< Select a Sample and hold unit for the stepper's step number 1.
                                                      Uses @ref Cy_VADC_GROUP_INDEX_t*/
      uint32_t enable_step1             : 1; /**< Should the step be added to the sequence */
      uint32_t sh_unit_step2            : 3;  /**< Select a Sample and hold unit for the stepper's step number 2.
                                                      Uses @ref Cy_VADC_GROUP_INDEX_t*/
      uint32_t enable_step2             : 1; /**< Should the step be added to the sequence */
      uint32_t sh_unit_step3            : 3;  /**< Select a Sample and hold unit for the stepper's step number 3.
                                                      Uses @ref Cy_VADC_GROUP_INDEX_t*/
      uint32_t enable_step3             : 1; /**< Should the step be added to the sequence */
      uint32_t sh_unit_step4            : 3;  /**< Select a Sample and hold unit for the stepper's step number 4.
                                                      Uses @ref Cy_VADC_GROUP_INDEX_t*/
      uint32_t enable_step4             : 1; /**< Should the step be added to the sequence */
      uint32_t sh_unit_step5            : 3;  /**< Select a Sample and hold unit for the stepper's step number 5.
                                                      Uses @ref Cy_VADC_GROUP_INDEX_t*/
      uint32_t enable_step5             : 1; /**< Should the step be added to the sequence */
      uint32_t sh_unit_step6            : 3;  /**< Select a Sample and hold unit for the stepper's step number 6.
                                                      Uses @ref Cy_VADC_GROUP_INDEX_t*/
      uint32_t enable_step6             : 1; /**< Should the step be added to the sequence */
      uint32_t sh_unit_step7            : 3;  /**< Select a Sample and hold unit for the stepper's step number 7.
                                                      Uses @ref Cy_VADC_GROUP_INDEX_t*/
      uint32_t enable_step7             : 1; /**< Should the step be added to the sequence */

    };
    uint32_t stepcfg;
  };
} Cy_VADC_GLOBAL_SHS_STEP_CONFIG_t;
#endif
/**
 * Sample and hold Initialization structure
 */
typedef struct CY_VADC_GLOBAL_SHS_CONFIG
{
  union
  {
    struct
    {
#if(CY_VADC_SHS_FULL_SET_REG == 1U)
      uint32_t shs_clock_divider        : 4; /**< The divider value for the SHS clock. Range: [0x0 to 0xF]*/
      uint32_t                          : 6;
#else
      uint32_t                          : 10;
#endif
      uint32_t analog_reference_select  : 2; /**< It is possible to different reference voltage for the SHS modules*/
      uint32_t                          : 20;
    };
    uint32_t shscfg;
  };
#if(CY_VADC_SHS_FULL_SET_REG == 1U)
  Cy_VADC_GLOBAL_SHS_CALIBRATION_ORDER_t calibration_order; /**< order in which the calibration should be taken up*/
#endif
} Cy_VADC_GLOBAL_SHS_CONFIG_t;

#endif

/**
 * Detailed global result structure
 */
typedef struct CY_VADC_GLOBAL_DETAILED_RESULT
{
  union
  {
    struct
    {
      uint32_t result                   : 16; /**< Result of the Analog to digital conversion*/
      uint32_t group_number             : 4;  /**< Indicates the group to which the channel_number refers*/
      uint32_t channel_number           : 5;  /**< Converted channel number*/
      uint32_t emux_channel_number      : 3;   /**< Converted external multiplexer channel number.
                                                 Only applicable for GxRES[0] result register*/
      uint32_t converted_request_source : 2;  /**< Converted request source*/
      uint32_t fast_compare_result      : 1;  /**< Fast compare result if conversion mode is fast compare mode.*/
      uint32_t vaild_result             : 1;  /**< Valid flag is set when a new result is available*/
    };
    uint32_t res;
  };
} Cy_VADC_GLOBAL_DETAILED_RESULT_t;

/**
 * Detailed channel result structure
 */
typedef struct CY_VADC_DETAILED_RESULT
{
  union
  {
    struct
    {
      uint32_t result                   : 16; /**< Result of the Analog to digital conversion*/
      uint32_t data_reduction_counter   : 4;  /**< Results reduction counter value*/
      uint32_t channel_number           : 5;  /**< Converted channel number*/
      uint32_t emux_channel_number      : 3;   /**< Converted external multiplexer channel number.
                                                 Only applicable for GxRES[0] result register*/
      uint32_t converted_request_source : 2;  /**< Converted request source*/
      uint32_t fast_compare_result      : 1;  /**< Fast compare result if conversion mode is fast compare mode.*/
      uint32_t vaild_result             : 1;  /**< Valid flag is set when a new result is available*/
    };
    uint32_t res;
  };
} Cy_VADC_DETAILED_RESULT_t;


/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
#pragma pop
#elif defined(__TASKING__)
#pragma warning restore
#endif
/*********************************************************************************************************************
 * static inline functions
 ********************************************************************************************************************/

#if (CY_VADC_GROUP_AVAILABLE == 1U)
__STATIC_INLINE bool CY_VADC_CHECK_GROUP_PTR(Cy_VADC_GROUP_t *const group_ptr)
{
#if (CY_VADC_MAXIMUM_NUM_GROUPS == 4U)
  return ((group_ptr == VADC_G0) || (group_ptr == VADC_G1) || (group_ptr == VADC_G2) || (group_ptr == VADC_G3));
#else
  return ((group_ptr == VADC_G0) || (group_ptr == VADC_G1));
#endif
}
#endif
/*********************************************************************************************************************
 * API Prototypes
 ********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the VADC module.<BR>\n
 * This API would ungate the clock to the VADC module (if applicable). Also this API would bring
 * the VADC module out of reset state(if applicable), by asserting the appropriate registers.
 * This API would invoke Cy_SCU_CLOCK_UngatePeripheralClock() and Cy_SCU_RESET_DeassertPeripheralReset()
 * if needed. Directly accessed register is COMPARATOR.ORCCTRL.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_DisableModule().
 */
void Cy_VADC_GLOBAL_EnableModule(void);

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the VADC module.<BR>\n
 * This API would gate the clock to the VADC module (if applicable). Also this API would put
 * the VADC module into the reset state(if applicable) by asserting the appropriate registers.
 * This API would invoke Cy_SCU_CLOCK_GatePeripheralClock() and Cy_SCU_RESET_AssertPeripheralReset() if needed.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_EnableModule().
 */
void Cy_VADC_GLOBAL_DisableModule(void);

/**
 *
 * @param global_ptr   Constant pointer to the VADC module.
 * @param config Pointer to initialization data structure
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Initializes the VADC global module with the associated configuration structure pointed by \a config.\n\n It
 * enables the global access to registers by configuring reset and clock un-gating for selected devices. It
 * initializes global class, boundary , result resources by setting GLOBICLASS,GLOBBOUND,GLOBRCR registers. It also
 * configures the global analog and digital clock dividers by setting GLOBCFG register. Refer related API's to change
 * the configurations later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 *  Cy_VADC_GLOBAL_ClockInit()<BR>
 */
void Cy_VADC_GLOBAL_Init(Cy_VADC_GLOBAL_t *const global_ptr, const Cy_VADC_GLOBAL_CONFIG_t *config);

/**
 *
 * @param global_ptr Constant pointer to the VADC module.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the VADC module clock.\n\n Call this API before any further configuration of VADC. It sets the DISR bit of CLC
 * register to enable.
 *
 * \par<b>Related APIs:</b><BR>
 *  Cy_VADC_GLOBAL_Init()
 *
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_EnableModuleClock(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  global_ptr->CLC &= ~((uint32_t)VADC_CLC_DISR_Msk);
}

/**
 *
 * @param global_ptr Constant pointer to the VADC module.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the VADC module clock.\n\n After this API call, no conversion will occur. Call
 * Cy_VADC_GLOBAL_EnableModuleClock() to enable the VADC module later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 *  Cy_VADC_GLOBAL_Init()
 *
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_DisableModuleClock(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  global_ptr->CLC |= (uint32_t) ((uint32_t)1 <<  VADC_CLC_DISR_Pos);
}

/**
 *
 * @param global_ptr    Constant pointer to the VADC module.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables VADC module to sleep if a sleep request comes.\n\n
 * It resets the EDIS bit of CLC register for enabling the sleep mode.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_DisableSleepMode().
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_EnableSleepMode(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  global_ptr->CLC &= ~((uint32_t)VADC_CLC_EDIS_Msk);
}

/**
 *
 * @param global_ptr    Constant pointer to the VADC module.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Ignores the sleep mode request for the VADC.\n\n
 * With the sleep feature enabled, the module will respond to sleep
 * requests by going into a low power mode. It resets the EDIS bit of CLC register for enabling the sleep mode.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_EnableSleepMode().
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_DisableSleepMode(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  global_ptr->CLC |= (uint32_t) ((uint32_t)1 <<  VADC_CLC_EDIS_Pos);
}

#if (CY_VADC_GROUP_AVAILABLE == 1U)
/**
 *
 * @param global_ptr Constant pointer to the VADC module.
 * @param config  Pointer to the data structure containing clock configuration data
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the VADC clock.<BR>\n
 * Sets up the clock configuration of the VADC module using the config structure pointed by \a config.
 * The clock to the analog converter and to the request source arbiter is configured by setting the GLOBCFG register.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_ClockInit(Cy_VADC_GLOBAL_t *const global_ptr, const Cy_VADC_GLOBAL_CLOCK_t *config)
{
  CY_ASSERT((global_ptr == VADC));

  /* Write the Clock configuration into the GLOBCFG register */
  global_ptr->GLOBCFG = (uint32_t)(config->globcfg | (VADC_GLOBCFG_DIVWC_Msk));
}
#endif

/**
 *
 * @param global_ptr Constant pointer to the VADC module.
 * @param config  Conversion class parameter structure
 * @param conv_type configure the input call for either standard conversion or EMUX related conversion.
 * @param set_num Conversion class set<BR>
 *                Range: [0x0, 0x1]
 *
 * \par<b>Description:</b><br>
 * Configures the ADC conversion settings like sample time and resolution.<BR>\n
 * Sets up the conversion settings for vadc global resource associated with \a config structure. It configures the
 * conversion class properties like sampling time and resolution for selected \a conv_type channels. It initializes
 * the GLOBALICLASS register specified by \a set_num with the required settings.
 *
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 */

void Cy_VADC_GLOBAL_InputClassInit(Cy_VADC_GLOBAL_t *const global_ptr, const Cy_VADC_GLOBAL_CLASS_t config,
                                    const Cy_VADC_GROUP_CONV_t conv_type, const uint32_t set_num);

/**
 *
 * @param global_ptr Constant pointer to the VADC global module
 * @param config  Pointer to result configuration data structure
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Initializes global result register.<BR>\n
 * Initializes Global Result Register with specified settings configured in the \a config structure.\n\n This API
 * results in configuration of GLOBRCR register. This helps in configuring the Data reduction mode, global result event
 * , wait for read mode on the GLOBRES register.
 *
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 */

__STATIC_INLINE void Cy_VADC_GLOBAL_ResultInit(Cy_VADC_GLOBAL_t *const global_ptr, const Cy_VADC_RESULT_CONFIG_t *config)
{
  CY_ASSERT((global_ptr == VADC));

  /* Configure GLOBRCR*/
  global_ptr->GLOBRCR = config->g_rcr;
}

/**
 *
 * @param global_ptr Constant pointer to the VADC module.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the startup calibration feature of the VADC module.\n\n It configures the SUCAL bit of GLOBCFG register to
 * enable the startup calibration feature. After turning it on, it loops until all active groups finish calibration.
 * Call Cy_VADC_GLOBAL_Enable() and Cy_VADC_GLOBAL_ClockInit() before calling this API in sequence. Calling the API
 * Cy_VADC_GLOBAL_DisableStartupCalibration() can disable the calibration feature at runtime.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_Enable()<BR>
 * Cy_VADC_GLOBAL_ClockInit()<BR>
 * None
 */
void Cy_VADC_GLOBAL_StartupCalibration(Cy_VADC_GLOBAL_t *const global_ptr);


/**
 *
 * @param global_ptr  Constant pointer to the VADC module.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the startup calibration feature of the VADC module.\n\n It configures the SUCAL bit of GLOBCFG register to
 * disable the startup calibration feature. Calling the API Cy_VADC_GLOBAL_EnsableStartupCalibration() can enable the
 * calibration feature at runtime.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */

__STATIC_INLINE void Cy_VADC_GLOBAL_DisableStartupCalibration(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  global_ptr->GLOBCFG &= ~((uint32_t)VADC_GLOBCFG_SUCAL_Msk);
}

#if (CY_VADC_GROUP_AVAILABLE == 1U)
/**
 *
 * @param global_ptr Constant pointer to the VADC module
 * @param group_number  group number whose post calibration feature is to be disabled. <BR>
 *                      Range[0x0 to 0x3] Accepts the enum ::Cy_VADC_GROUP_INDEX_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the post calibration for a particular group specified as \a group_number.\n\n It configures the DPCAL0 bit
 * of GLOBCFG register to disable the post calibration feature. Call Cy_VADC_GLOBAL_Enable() and
 * Cy_VADC_GLOBAL_ClockInit() before calling this API in sequence. Calling the API
 * Cy_VADC_GLOBAL_EnablePostCalibration() can enable back the calibration feature at runtime.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_Enable()<BR>
 * Cy_VADC_GLOBAL_ClockInit()<BR>
 * Cy_VADC_GLOBAL_DisablePostCalibration()<BR>
 * None
 */

__STATIC_INLINE void Cy_VADC_GLOBAL_DisablePostCalibration(Cy_VADC_GLOBAL_t *const global_ptr, uint32_t group_number)
{
  CY_ASSERT((global_ptr == VADC));

  global_ptr->GLOBCFG |= (uint32_t)((uint32_t)1 << ((uint32_t)VADC_GLOBCFG_DPCAL0_Pos + group_number));
}

/**
 *
 * @param global_ptr Constant pointer to the VADC module
 * @param group_number  group number whose post calibration feature is to be enabled. <BR>
 *                      Range[0x0 to 0x3] Accepts the enum ::Cy_VADC_GROUP_INDEX_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the post calibration for a particular group specified as \a group_number.\n\n It configures the DPCAL0 bit
 * of GLOBCFG register to enable the post calibration feature. Calling the API Cy_VADC_GLOBAL_DisablePostCalibration()
 * can disable the calibration feature at runtime.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_DisablePostCalibration()<BR>
 * None
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_EnablePostCalibration(Cy_VADC_GLOBAL_t *const global_ptr, uint32_t group_number)
{
  CY_ASSERT((global_ptr == VADC));

  global_ptr->GLOBCFG &= (~ (uint32_t)((uint32_t)1 << ((uint32_t)VADC_GLOBCFG_DPCAL0_Pos + group_number)));
}
#endif

#if (CY_VADC_BOUNDARY_AVAILABLE == 1U)
/**
 *
 * @param global_ptr Constant pointer to the VADC module.
 * @param boundary0  Boundary-0 Value<BR>Range[0 - 4095]
 * @param boundary1  Boundary-1 Value<BR>Range[0 - 4095]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Programs the boundaries with \a boundary0 and boundary1 for result comparison.\n\n These two boundaries can serve as
 * absolute boundaries. They define a range against which the result of a conversion can be compared. In the
 * fast compare mode, the two boundaries provide hysteresis capability to a compare value. In any case, these boundary
 * values entered here form a boundary pallete. There are dedicated upper and lower boundary registers GLOBBOUND0 and
 * GLOBBOUND1 who will derive their values from this palette.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 */
void Cy_VADC_GLOBAL_SetBoundaries(Cy_VADC_GLOBAL_t *const global_ptr, const uint32_t boundary0, const uint32_t boundary1);

/**
 *
 * @param global_ptr Constant pointer to the VADC module.
 * @param selection The boundary value selected for \b boundary_value.
 * @param boundary_value  Boundary Value<BR>Range[0 - 4095]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Programs either the boundary 0 or boundary 1 for result comparison.\n\n This defines a range against which
 * the result of a conversion can be compared. In the fast compare mode, the two boundaries provide hysteresis
 * capability to a compare value.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 */
void Cy_VADC_GLOBAL_SetIndividualBoundary(Cy_VADC_GLOBAL_t *const global_ptr,
    const Cy_VADC_CHANNEL_BOUNDARY_t selection,
    const uint16_t boundary_value);
#endif

#if (CY_VADC_EMUX_AVAILABLE== 1U)
/**
 *
 * @param global_ptr   Constant pointer to the VADC module
 * @param emuxif       The EMUX interface<BR>Range[0x0 - 0x1]
 * @param group        The VADC group which must be bound to the desired emux
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Binds a VADC \a group to an EMUX interface specified in \a emuxif.<BR>\n
 * Selects which group's scan request source will control the EMUX interface (set of control select lines for the EMUX).
 * By passing \b group it would configure that group's scan request source to control the EMUX select lines of the set
 * \b emuxif.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void Cy_VADC_GLOBAL_BindGroupToEMux(Cy_VADC_GLOBAL_t *const global_ptr, const uint32_t emuxif, const uint32_t group);
#endif

/**
 *
 * @param global_ptr Constant pointer to the VADC module.
 *
 * @return uint32_t Complete global result register value GLOBRES
 *
 * \par<b>Description:</b><br>
 * Retrieves the complete result from the global result register associated with the \a global_ptr.\n\n This API audits
 * the result register GLOBRES for the validity of the data. If the validity is assured, data is first read
 * the global result register, cached locally next and subsequently returned to the caller.
 *
 * @note You can cast the return to a varible of type Cy_VADC_GLOBAL_DETAILED_RESULT_t to easily access the register bit fields
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_GetResult()
 */
__STATIC_INLINE uint32_t Cy_VADC_GLOBAL_GetDetailedResult(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));

  return (global_ptr->GLOBRES);
}

/**
 *
 * @param global_ptr Constant pointer to the VADC module.
 *
 * @return Cy_VADC_RESULT_SIZE_t 16 bit result register value.<BR>
 *         Range[0x0 - 0X0FFF]
 *
 * \par<b>Description:</b><br>
 * Retrieves the conversion result from the global result register associated with the \a global_ptr.\n\n This is a
 * lightweight version of Cy_VADC_GLOBAL_GetDetailedResult(). The behavior is exactly the same, just that it is
 * only the 16 bit numeric result returned back to the application instead of the complete GLOBRES register value.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_GetDetailedResult()
 */
__STATIC_INLINE Cy_VADC_RESULT_SIZE_t Cy_VADC_GLOBAL_GetResult(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));

  return ((Cy_VADC_RESULT_SIZE_t)global_ptr->GLOBRES);
}

/**
 *
 * @param global_ptr Constant pointer to the VADC module
 * @param compare_val Compare value which the result of a conversion will be compared against.
 *         <BR>Range[0x0 - 0X0FFF]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set compare value in the global result register for fast compare mode.\n\n The result of a conversion will directly
 * be compared to the compare value entered as part of \a compare_val. The prerequisite is that the channel associated
 * with this global register must select an ICLASS which has the conversion mode configured as fast compare mode. Call
 * @ref Cy_VADC_GLOBAL_GetCompareResult() after this API to
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void Cy_VADC_GLOBAL_SetCompareValue(Cy_VADC_GLOBAL_t *const global_ptr, const Cy_VADC_RESULT_SIZE_t compare_val);

/**
 *
 * @param global_ptr Constant pointer to the VADC module
 * @return compare high or low. Refer @ref Cy_VADC_FAST_COMPARE_t enum
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Determines the result of fast compare operation.\n\n This API returns the result of fast compare operation provided
 * the valid flag in the global result register GLOBRES is set.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
Cy_VADC_FAST_COMPARE_t Cy_VADC_GLOBAL_GetCompareResult(Cy_VADC_GLOBAL_t *const global_ptr);

/**
 *
 * @param global_ptr         Constant pointer to the VADC module
 * @param event_type   Desired event that must be manually asserted
 *                    Use the enum ::Cy_VADC_GLOBAL_EVENT_t to create a mask to be used with this argument
 * @return None
 *
 * \par<b>Description:</b><br>
 * Manually asserts an event that can lead to an interrupt.\n\n This API manually asserts the requested event
 * (Background request source event or a global result event) by setting the GLOBEVFLAG register with the specified
 * \a event_type.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */

__STATIC_INLINE void Cy_VADC_GLOBAL_TriggerEvent(Cy_VADC_GLOBAL_t *const global_ptr, const uint32_t event_type)
{
  CY_ASSERT((global_ptr == VADC));
  CY_ASSERT(((CY_VADC_GLOBAL_EVENT_BKGNDSOURCE == event_type) || (CY_VADC_GLOBAL_EVENT_RESULT == event_type)));

  global_ptr->GLOBEFLAG = event_type;
}

/**
 *
 * @param global_ptr Constant pointer to the VADC module
 * @param event_type Event that must be acknowledged
 *                    Use the enum ::Cy_VADC_GLOBAL_EVENT_t to create a mask to be used with this argument
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Acknowledges an event that has been asserted manually or automatically.\n\n This API acknowledges the requested event
 * by clearing GLOBEFLAG sticky flag.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_ClearEvent(Cy_VADC_GLOBAL_t *const global_ptr, const uint32_t event_type)
{
  CY_ASSERT((global_ptr == VADC));
  CY_ASSERT(((CY_VADC_GLOBAL_EVENT_BKGNDSOURCE == event_type) || (CY_VADC_GLOBAL_EVENT_RESULT == event_type)));

  global_ptr->GLOBEFLAG = ((uint32_t)(event_type << (uint32_t)16));
}

/**
 *
 * @param global_ptr Constant pointer to the VADC module
 * @param sr The service request to which the global result event is connected. Refer @ref Cy_VADC_SR_t enum
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Binds the global result event to one of the 4 shared service requests.\n\n This API binds the global result event
 *  to one of the 4 module wide shared service requests .Sets GLOBEVNP register with the corresponding \a sr line.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_BackgroundSetReqSrcEventInterruptNode()
 */
void Cy_VADC_GLOBAL_SetResultEventInterruptNode(Cy_VADC_GLOBAL_t *const global_ptr, Cy_VADC_SR_t sr);

/**
 *
 * @param global_ptr Constant pointer to the VADC module
 * @param sr The service request to which the global request source event is connected. Refer @ref Cy_VADC_SR_t enum
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Binds the background request source event to one of the 4 shared service requests.\n\n This API binds the background
 * request source event to one of the 4 module wide shared service requests. Sets GLOBEVNP register with the
 * corresponding \a sr line.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_SetResultEventInterruptNode()
 */
void Cy_VADC_GLOBAL_BackgroundSetReqSrcEventInterruptNode(Cy_VADC_GLOBAL_t *const global_ptr, Cy_VADC_SR_t sr);

#if(CY_VADC_SHS_AVAILABLE == 1U)
/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  config Struct consisting of various SHS related configurations.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configure the basic SHS parameters.<BR>\n
 * API would initialize the clock divider configuration, the analog reference selection and
 * the calibration order for the Sample and Hold unit.
 *
 * \par<b>Related APIs:</b><BR>
 * None.
 */
void Cy_VADC_GLOBAL_SHS_Init(Cy_VADC_GLOBAL_SHS_t *const shs_ptr, const Cy_VADC_GLOBAL_SHS_CONFIG_t *config);

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param aref    Analog reference used for conversions. Refer @ref Cy_VADC_GLOBAL_SHS_AREF_t enum
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Selection of the reference voltage that is required for conversions (VREF).
 *
 * \par<b>Related APIs:</b><BR>
 * None.
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_SHS_SetAnalogReference(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    const Cy_VADC_GLOBAL_SHS_AREF_t aref)
{
  CY_ASSERT((shs_ptr == (Cy_VADC_GLOBAL_SHS_t *)(void *)SHS0));

  shs_ptr->SHSCFG = (shs_ptr->SHSCFG & (uint32_t)~SHS_SHSCFG_AREF_Msk) | (uint32_t)aref | SHS_SHSCFG_SCWC_Msk;
}

#if(CY_VADC_SHS_FULL_SET_REG == 1U)
/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  config Struct consisting of various step configurations.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the stepper sequence for the converter.<BR>\n
 * Stepper of the SHS can be configured to take up a specific sequence of groups for conversion.
 * The stepper sequence is configured using this API.
 *
 * \par<b>Related APIs:</b><BR>
 * None.
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_SHS_SetStepperSequence(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    const Cy_VADC_GLOBAL_SHS_STEP_CONFIG_t *config)
{
  CY_ASSERT((shs_ptr == (Cy_VADC_GLOBAL_SHS_t *)(void *)SHS0));
  CY_ASSERT((config == (Cy_VADC_GLOBAL_SHS_STEP_CONFIG_t *)NULL));

  shs_ptr->STEPCFG = (uint32_t) config->stepcfg;
}
#endif

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 *
 * @return bool returns true if the analog converter is operable
 *              returns false if the analog converter is powered down
 *
 * \par<b>Description:</b><br>
 * Returns the converter status.<BR>\n
 * Returns the ANRDY bit field of the SHSCFG register.
 *
 * \par<b>Related APIs:</b><BR>
 * None.
 */
__STATIC_INLINE bool Cy_VADC_GLOBAL_SHS_IsConverterReady(Cy_VADC_GLOBAL_SHS_t *const shs_ptr)
{
  CY_ASSERT((shs_ptr == (Cy_VADC_GLOBAL_SHS_t *)(void *)SHS0));

  return ((bool)((shs_ptr->SHSCFG >> (uint32_t)SHS_SHSCFG_ANRDY_Pos) & (uint32_t)0x1));
}

#if(CY_VADC_SHS_FULL_SET_REG == 1U)
/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param group_num group number for which the accelerated mode needs to be enabled.<BR>Range: [0x0 to 0x1]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the Accelerated timing mode.<BR>\n
 * This API is needed when a switch from compatible mode to accelerated mode of conversion is needed. In
 * this mode the ADC module will convert the input depending on the value stored in the SST bit of the SHS0_TIMCFGx.
 * This API would configure the accelerated mode in the SHS0_TIMCFG0 and SHS0_TIMCFG1 registers.
 *
 * \par<b>Related APIs:</b><BR>
 * None.
 */
void Cy_VADC_GLOBAL_SHS_EnableAcceleratedMode(Cy_VADC_GLOBAL_SHS_t *const shs_ptr, Cy_VADC_GROUP_INDEX_t group_num);

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param group_num group number for which the accelerated mode needs to be disabled.<BR>Range: [0x0 to 0x1]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the Accelerated timing mode.<BR>\n
 * This API is needed when a switch from accelerated mode to compatible mode of conversion is needed.
 * This API would clear the accelerated mode in the SHS0_TIMCFG0 and SHS0_TIMCFG1 registers.
 *
 * \par<b>Related APIs:</b><BR>
 * None.
 */
void Cy_VADC_GLOBAL_SHS_DisableAcceleratedMode(Cy_VADC_GLOBAL_SHS_t *const shs_ptr, Cy_VADC_GROUP_INDEX_t group_num);

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param group_num group number for which the accelerated mode needs to be enabled.<BR>Range: [0x0 to 0x1]
 * @param sst_value Value of short sample time that needs to be configured.<BR>
 *                  Range: [0x0 to 0x3F]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the Accelerated timing mode sample time.<BR>\n
 * This API is needed when a switch from compatible mode to accelerated mode of conversion is needed. In
 * Accelerated mode the ADC module will convert the input depending on the value stored in the SST bit of the
 *  SHS0_TIMCFGx. This API would configure the shot sample time either in SHS0_TIMCFG0.SST or SHS0_TIMCFG1.SST .
 *
 * \par<b>Related APIs:</b><BR>
 * None.
 */
void Cy_VADC_GLOBAL_SHS_SetShortSampleTime(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    Cy_VADC_GROUP_INDEX_t group_num,
    uint8_t sst_value);

#endif
/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  divs_value The clock divider value that is possible
 *                    <BR>Range:[0x0 to 0xF]
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configure Sample and hold clock divider value.<BR>\n
 * API would initialize the clock divider configuration. This determines the frequency of conversion
 * of the Sample and hold converter.
 *
 * \par<b>Related APIs:</b><BR>
 * None.
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_SHS_SetClockDivider(Cy_VADC_GLOBAL_SHS_t *const shs_ptr, uint8_t divs_value)
{
  CY_ASSERT((shs_ptr == (Cy_VADC_GLOBAL_SHS_t *)(void *)SHS0));
  CY_ASSERT((divs_value < (uint32_t)0x10));

  shs_ptr->SHSCFG =  (shs_ptr->SHSCFG & (~(uint32_t)SHS_SHSCFG_DIVS_Msk)) | (uint32_t)SHS_SHSCFG_SCWC_Msk;
  shs_ptr->SHSCFG |=  ((uint32_t)divs_value << SHS_SHSCFG_DIVS_Pos) | (uint32_t)SHS_SHSCFG_SCWC_Msk;
}

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  gain_value gain value possible
 *                    Range:[0x0 to 0x3]
 * @param  group_num The Group number for which the configurations applies
 * @param  ch_num The channel number for which the gain has to be configured
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configure the gain value for SHS.<BR>\n
 * API would set the gain factor for a selected channel.
 *
 * \par<b>Related APIs:</b><BR>
 * None.
 */
void Cy_VADC_GLOBAL_SHS_SetGainFactor(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
                                       uint8_t gain_value,
                                       Cy_VADC_GROUP_INDEX_t group_num,
                                       uint8_t ch_num);

#if(CY_VADC_SHS_FULL_SET_REG == 1U)
/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  max_calibration_time calibration time
 *                              Range:[0x0 to 0x3F]
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configure the Maximum calibration timing.<BR>\n
 * API would initialize the Maximum time after which the calibration should occur. If no adc conversion
 * occur during this duration then the calibration would run irrespective of conversions. The max time the
 * converter can go without a calibration is set in this API.
 *
 * \par<b>Related APIs:</b><BR>
 * None.
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_SHS_SetMaxCalTime(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    uint32_t max_calibration_time)
{
  CY_ASSERT((shs_ptr == (Cy_VADC_GLOBAL_SHS_t *)(void *)SHS0));

  shs_ptr->CALCTR &= ~((uint32_t)SHS_CALCTR_CALMAX_Msk);
  shs_ptr->CALCTR |=  ((uint32_t)max_calibration_time << SHS_CALCTR_CALMAX_Pos);
}


/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  group_num The Group number for which the configurations applies
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enable the Gain and offset calibration.<BR>\n
 * Enable the gain and offset calibration for all the Sample and hold units.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_SHS_DisableGainAndOffsetCalibrations()<BR>.
 */
void Cy_VADC_GLOBAL_SHS_EnableGainAndOffsetCalibrations(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    Cy_VADC_GROUP_INDEX_t group_num);

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  group_num The Group number for which the configurations applies
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disable the Gain and offset calibration.<BR>\n
 * Disable the gain and offset calibration for all the Sample and hold units.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_SHS_EnableGainAndOffsetCalibrations()<BR>.
 */
void Cy_VADC_GLOBAL_SHS_DisableGainAndOffsetCalibrations(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    Cy_VADC_GROUP_INDEX_t group_num);

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  group_num The Group number for which the configurations applies
 * @param  gain_level The gain level whose calibration value has to read.
 * @return None
 *
 * \par<b>Description:</b><br>
 * Read the calibration value for the selected gain level.<BR>\n
 * Each gain value has a offset calibration value, this API would return the offset calibration value of the
 * selected gain level. This is applicable for all the channels in the group that use the particular gain level.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_SHS_SetOffsetCalibrationValue()<BR>.
 */
uint8_t Cy_VADC_GLOBAL_SHS_GetOffsetCalibrationValue(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    Cy_VADC_GROUP_INDEX_t group_num,
    Cy_VADC_SHS_GAIN_LEVEL_t gain_level);

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  group_num The Group number for which the configurations applies
 * @param  gain_level The gain level whose calibration value has to read.
 * @param  offset_calibration_value The offset calibration value to be set.
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the calibration value for the selected gain level.<BR>\n
 * Each gain value has a offset calibration value, this API would set the offset value of the selected gain level.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_SHS_GetOffsetCalibrationValue()<BR>.
 */
void Cy_VADC_GLOBAL_SHS_SetOffsetCalibrationValue(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    Cy_VADC_GROUP_INDEX_t group_num,
    Cy_VADC_SHS_GAIN_LEVEL_t gain_level,
    uint8_t offset_calibration_value);
#endif

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  group_num The Group number for which the configurations applies
 * @param  loop_select The delta sigma loop number for which the configurations applies
 * @param  ch_num Channel number for which the configurations applies
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the delta sigma loop of the SHS.<BR>\n
 * There are 2 Delta-Sigma loops that can be configured. This API would configure the loop (loop_select)
 * with the appropriate group_num and channel_num.
 * Configures the SHS_LOOP bit fields.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_SHS_EnableSigmaDeltaLoop()<BR>.
 */
void Cy_VADC_GLOBAL_SHS_SetSigmaDeltaLoop(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    Cy_VADC_GROUP_INDEX_t group_num,
    Cy_VADC_SHS_LOOP_CH_t loop_select,
    uint8_t ch_num);

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  loop_select The delta sigma loop number for which the configurations applies
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enable the selected Delta-Sigma loop.<BR>\n
 * Configures the SHS_LOOP.LPENx bit field.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_SHS_SetSigmaDeltaLoop()<BR>.
 * Cy_VADC_GLOBAL_SHS_EnableGainAndOffsetCalibrations()<BR>.
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_SHS_EnableSigmaDeltaLoop(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    Cy_VADC_SHS_LOOP_CH_t loop_select)
{
  CY_ASSERT((shs_ptr == (Cy_VADC_GLOBAL_SHS_t *)(void *)SHS0));
  CY_ASSERT((loop_select == CY_VADC_SHS_LOOP_CH_0) || (loop_select == CY_VADC_SHS_LOOP_CH_1));

  shs_ptr->LOOP |= (uint32_t)SHS_LOOP_LPEN0_Msk << (uint32_t)loop_select;
}

/**
 * @param shs_ptr Constant pointer to the VADC Sample and hold module
 * @param  loop_select The delta sigma loop number for which the configurations applies
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disable the selected delta sigma loop.<BR>\n
 * Configures the SHS_LOOP.LPENx bit field.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_SHS_SetSigmaDeltaLoop()<BR>.
 * Cy_VADC_GLOBAL_SHS_EnableGainAndOffsetCalibrations()<BR>.
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_SHS_DisableSigmaDeltaLoop(Cy_VADC_GLOBAL_SHS_t *const shs_ptr,
    Cy_VADC_SHS_LOOP_CH_t loop_select)
{
  CY_ASSERT((shs_ptr == (Cy_VADC_GLOBAL_SHS_t *)(void *)SHS0));
  CY_ASSERT((loop_select == CY_VADC_SHS_LOOP_CH_0) || (loop_select == CY_VADC_SHS_LOOP_CH_1));

  shs_ptr->LOOP &= ~((uint32_t)SHS_LOOP_LPEN0_Msk << (uint32_t)loop_select);

}

#endif
#if (CY_VADC_GROUP_AVAILABLE == 1U)
/**
 *
 * @param group_ptr   Constant pointer to the VADC group.
 * @param config Pointer to the initialization data structure
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Initializes the VADC group module with the associated configuration structure pointed by \a config.\n\n It
 * initializes the group specified as part of the \a group_ptr. It initializes group conversion class, arbiter
 * configuration , boundary configuration by setting GxICLASS,GxARBCFG,GxBOUND, registers. It also
 * configures the EMUX control register if applicable. Refer related API's to change the configurations later in the
 *  program.
 *
 * \par<b>Related APIs:</b><BR>
 *  Cy_VADC_GROUP_InputClassInit()<BR>
 *  Cy_VADC_GROUP_SetPowerMode()<BR>
 *  Cy_VADC_GROUP_SetBoundaries()<BR>
 *  Cy_VADC_GROUP_ExternalMuxControlInit()<BR>
 */
void Cy_VADC_GROUP_Init(Cy_VADC_GROUP_t *const group_ptr, const Cy_VADC_GROUP_CONFIG_t *config);

/**
 *
 * @param group_ptr Constant pointer to the VADC group.
 * @param config  group related conversion class parameter structure
 * @param conv_type Use direct channels or EMUX channels. Refer @ref Cy_VADC_GROUP_CONV_t enum
 * @param set_num Conversion class set<BR>
 *                Range[0x0, 0x1]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets up the conversion settings for vadc group resource associated with \a config structure. It configures the
 * conversion class properties like sampling time and resolution for selected \a conv_type channels. It initializes
 * the G_ICLASS register specified by \a set_num with the required settings.
 *
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GROUP_Init()
 *
 */
void Cy_VADC_GROUP_InputClassInit(Cy_VADC_GROUP_t *const group_ptr, const Cy_VADC_GROUP_CLASS_t config,
                                   const Cy_VADC_GROUP_CONV_t conv_type, const uint32_t set_num);

/**
 *
 * @param group_ptr Constant pointer to the VADC Group which must be set as a slave
 * @param master_grp The master group number<BR>
 *                   Range: [0x0 - 0x3]
 * @param slave_group The slave group number<BR>
 *                   Range: [0x0 - 0x3]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures a VADC Group as a slave group.\n\n Conversion of identically numbered channels across groups can be
 * synchronized. For example, when the trigger to convert CH-1 of Group-2 is received, it is possible to
 * simultaneously request conversion of CH-1 of Group-0 and Group-3. Group-2 in this example is therefore the
 * master group while Groups-0 and 3 are the slave groups. It uses the SYNCCTR register for the configuration settings.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GROUP_SetSyncMaster()<BR>
 * Cy_VADC_GROUP_CheckSlaveReadiness()<BR>
 * Cy_VADC_GROUP_EnableChannelSyncRequest()<BR>
 *
 */
void Cy_VADC_GROUP_SetSyncSlave(Cy_VADC_GROUP_t *const group_ptr, uint32_t master_grp, uint32_t slave_group);

/**
 *
 * @param group_ptr  Constant pointer to the VADC Group.
 * @param power_mode  Desired power mode
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the power mode of a VADC group.\n\n For a VADC group to actually convert an analog signal, its analog
 * converter must be turned on.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void Cy_VADC_GROUP_SetPowerMode(Cy_VADC_GROUP_t *const group_ptr, const Cy_VADC_GROUP_POWERMODE_t power_mode);

/**
 *
 * @param group_ptr  Constant pointer to the VADC Group.
 *
 * @return
 *    Cy_VADC_GROUP_POWERMODE_t Current power mode
 *
 * \par<b>Description:</b><br>
 * Retrieves the power mode of a VADC group.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GROUP_SetPowerMode()<BR>
 */
__STATIC_INLINE int32_t Cy_VADC_GROUP_GetPowerMode(Cy_VADC_GROUP_t *const group_ptr)
{
  return ((group_ptr->ARBCFG & (uint32_t)VADC_G_ARBCFG_ANONS_Msk) >> VADC_G_ARBCFG_ANONS_Pos);
}

/**
 *
 * @param group_ptr     Constant pointer to the VADC group.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures a VADC Group as a master group.<BR>\n
 * Conversion of identically numbered channels across groups can be
 * synchronized. For example, when the trigger to convert CH-1 of Group-2 is received, it is possible to simultaneously
 * request conversion of CH-1 of Group-0 and Group-3. Group-2 in this example is therefore the master group while
 * Groups-0 and 3 are the slave groups.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void Cy_VADC_GROUP_SetSyncMaster(Cy_VADC_GROUP_t *const group_ptr);

/**

 * @param group_ptr       Pointer to the master VADC Group
 * @param slave_group  The slave VADC Group number
 *                     <BR>Range: [0x0 to 0x3]
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the ready signal for master group.<BR>\n
 * This API would read the \b slave_group number and determine which EVAL configuration to apply for the given master
 * slave set. Checks the readiness of slaves in synchronized conversions. Conversion of identically numbered channels
 * across groups can be synchronized. For example, when the trigger to convert CH-1 of Group-2 is received, it is
 * possible to simultaneously request conversion of CH-1 of Group-0 and Group-3. Group-2 in this example is
 * therefore the master group while Groups-0 and 3 are the slave groups. Before the master can request its slaves
 * for synchronized conversion, it has the option of checking the readiness of the slaves.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GROUP_IgnoreSlaveReadiness()<BR> Cy_VADC_GROUP_SetSyncMaster()
 */
void Cy_VADC_GROUP_CheckSlaveReadiness(Cy_VADC_GROUP_t *const group_ptr, uint32_t slave_group);

/**
 *
 * @param group_ptr  Constant Pointer to the master VADC Group
 * @param slave_group  The slave VADC Group number
 * @return None
 *
 * \par<b>Description:</b><br>
 * Clears the ready signal for master group.<BR>\n
 * Ignores the readiness of slaves in synchronized conversions.This API would read the \b slave_group number and
 * determine which EVAL configuration to apply for the given master slave set. Then clears the configuration if present.
 * This API is called when the master should issue the conversion request without waiting for the slave to
 * assert a ready signal. The ready signal is asserted by the slave group(s) when the conversion is completed
 * in these channels.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GROUP_CheckSlaveReadiness()<BR> Cy_VADC_GROUP_SetSyncMaster()<BR>
 */
void Cy_VADC_GROUP_IgnoreSlaveReadiness(Cy_VADC_GROUP_t *const group_ptr, uint32_t slave_group);

/**
 *
 * @param group_ptr  Constant Pointer to the VADC Group waiting for ready signal
 * @param eval_waiting_group  The VADC Group which expects a ready signal to start it's conversion.
 * @param eval_origin_group  The VADC Group from which the eval_waiting_group will expect a ready signal
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets the ready signal in the eval_waiting_group .<BR>\n
 * For Synchronized conversion all the slaves participating need to configure the ready signal.
 * A slave group will also need to configure the ready signals coming from the other slave groups.
 * A call to this API would configure the Sync.slave's EVAL Bits (GxSYNCTR.EVALy).
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GROUP_CheckSlaveReadiness()<BR> Cy_VADC_GROUP_IgnoreSlaveReadiness()<BR>
 */
void Cy_VADC_GROUP_SetSyncSlaveReadySignal(Cy_VADC_GROUP_t *const group_ptr,
    uint32_t eval_waiting_group,
    uint32_t eval_origin_group);

/**
 *
 * @param group_ptr  Constant Pointer to the VADC Group
 * @return
 *         uint32_t EVAL bits for the group
 *
 * \par<b>Description:</b><br>
 * Get the Eval bits of the group.<BR>\n
 * For Synchronized conversion the master's ready signal configuration must be copied onto the slaves.
 * A call to this API would return the Sync EVAL Bits (GxSYNCTR.EVALy) which can be used to set in the slaves.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GROUP_CheckSlaveReadiness()<BR> Cy_VADC_GROUP_IgnoreSlaveReadiness()<BR>
 */
__STATIC_INLINE uint32_t Cy_VADC_GROUP_GetSyncReadySignal(Cy_VADC_GROUP_t *const group_ptr)
{
  uint32_t eval_mask;
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

#if (CY_VADC_MAXIMUM_NUM_GROUPS > 2U)
  eval_mask = VADC_G_SYNCTR_EVALR1_Msk | VADC_G_SYNCTR_EVALR2_Msk | VADC_G_SYNCTR_EVALR3_Msk;
#else
  eval_mask = VADC_G_SYNCTR_EVALR1_Msk;
#endif
  return ( group_ptr->SYNCTR & eval_mask);
}

/**
 * @param group_ptr  Constant Pointer to the VADC Group
 * @param eval_mask  mask to configure the eval bits
 *                   Use Cy_VADC_SYNCTR_EVAL_t to create the mask.
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the Eval bits of the group.<BR>\n
 * For Synchronized conversion the master's ready signal configuration must be copied onto the slaves.
 * A call to this API would configure the Sync EVAL Bits (GxSYNCTR.EVALy).
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GROUP_CheckSlaveReadiness()<BR> Cy_VADC_GROUP_IgnoreSlaveReadiness()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_SetSyncReadySignal(Cy_VADC_GROUP_t *const group_ptr, uint32_t mask)
{
  uint32_t eval_mask;
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

#if (CY_VADC_MAXIMUM_NUM_GROUPS > 2U)
  eval_mask = VADC_G_SYNCTR_EVALR1_Msk | VADC_G_SYNCTR_EVALR2_Msk | VADC_G_SYNCTR_EVALR3_Msk;
#else
  eval_mask = VADC_G_SYNCTR_EVALR1_Msk;
#endif
  group_ptr->SYNCTR &= ~(eval_mask);
  group_ptr->SYNCTR |= mask;
}

/**
 *
 * @param group_ptr   Constant pointer to the master VADC Group
 * @param ch_num  Channel whose conversion triggers conversion in slave groups
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets up a channel for synchronized conversion.\n\n Conversion of identically numbered channels across groups
 * can be synchronized. For example, when the trigger to
 * convert CH-1 of Group-2 is received, it is possible to simultaneously request conversion of CH-1 of Group-0 and
 * Group-3. Group-2 in this example is therefore the master group while Groups-0 and 3 are the slave groups.<br>
 * Before the master can request its slaves for synchronized conversion, it has the option of checking the readiness
 * of the slaves.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void Cy_VADC_GROUP_EnableChannelSyncRequest(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_num);

/**
 *
 * @param group_ptr       Pointer to the master VADC Group
 * @param ch_num     Channel whose conversion triggers conversion in slave groups
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disable the synchronization request for the particular channel specified as ch_num. To enable the synchronization
 * call the API @ref Cy_VADC_GROUP_EnableChannelSyncRequest().
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void Cy_VADC_GROUP_DisableChannelSyncRequest(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_num);

/**
 *
 * @param group_ptr     Constant pointer to the VADC group.
 *
 * @return retuns IDLE if converter is free else returns busy. Refer @ref Cy_VADC_GROUP_STATE_t enum
 *
 * \par<b>Description:</b><br>
 * Checks the live status of the analog to digital converter. The converter can either idle doing nothing or busy
 * sampling + converting.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
Cy_VADC_GROUP_STATE_t Cy_VADC_GROUP_IsConverterBusy(Cy_VADC_GROUP_t *const group_ptr);

/**
 *
 * @param group_ptr Constant pointer to the VADC group whose global boundary registers are to be programmed
 * @param boundary0  Boundary-0 Value<BR>
 *                   Range: [0x0 - 0x0FFF]
 * @param boundary1  Boundary-1 Value<BR>
 *                   Range: [0x0 - 0x0FFF]
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Programs the boundaries with \a boundary0 and boundary1 for result comparison.\n\n These two boundaries can serve as
 * absolute boundaries. They defines a range against which the result of a conversion can be compared. In the
 * fast compare mode, the two boundaries provide hysteresis capability to a compare value. In any case, these boundary
 * values entered here form a boundary pallete. There are dedicated upper and lower boundary registers G_BOUND0 and
 * G_BOUND1 who will derive their values from this palette.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 */
void Cy_VADC_GROUP_SetBoundaries(Cy_VADC_GROUP_t *const group_ptr,
                                  const uint32_t boundary0,
                                  const uint32_t boundary1);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param selection The boundary value selected for \b boundary_value.
 * @param boundary_value Select the boundary value.
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Programs the boundary with \a boundary_value for result comparison.\n\n This defines a range against which
 * the result of a conversion can be compared. In the fast compare mode, the two boundaries provide hysteresis
 * capability to a compare value.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
void Cy_VADC_GROUP_SetIndividualBoundary(Cy_VADC_GROUP_t *const group_ptr,
    const Cy_VADC_CHANNEL_BOUNDARY_t selection,
    const uint16_t boundary_value);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param sr_num   The service request number (0 through 3)
 * @param type     IRQ type (Kernel specific interrupt vs Module wide shared interrupt )
 * @return None
 *
 * \par<b>Description:</b><br>
 * Activates a Service Request line(manually trigger).<BR>\n
 * VADC provides few SR lines for each group and a few more which is shared across all the groups.
 * These SR lines can be connected to an NVIC node which in-turn would generate an interrupt.
 * This API would manually trigger the given SR line. Could be used for evaluation and testing purposes.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
void Cy_VADC_GROUP_TriggerServiceRequest(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t sr_num,
    const Cy_VADC_GROUP_IRQ_t type);

/**
 *
 * @param group_ptr  Constant pointer to the VADC group
 * @param emux_cfg   EMUX configuration structure
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures group EMUX parameters associated with the \a emux_cfg configuration structure.\n\n An external emux
 * interface allows additional channels to be connected to a VADC group. The conversion properties
 * of such channels can be different from the standard channels which are directly connected to the VADC group.
 * This API configures conversion properties of channels connected via EMUX interface.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 */
__STATIC_INLINE void Cy_VADC_GROUP_ExternalMuxControlInit(Cy_VADC_GROUP_t *const group_ptr,
    const Cy_VADC_GROUP_EMUXCFG_t emux_cfg)
{
  uint32_t   emux_config;

  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

  emux_config = ((uint32_t)emux_cfg.starting_external_channel << (uint32_t)VADC_G_EMUXCTR_EMUXSET_Pos) |
                ((uint32_t)emux_cfg.connected_channel << (uint32_t)VADC_G_EMUXCTR_EMUXCH_Pos);

  group_ptr->EMUXCTR  = emux_config;
  emux_config = ((uint32_t)emux_cfg.emux_coding << (uint32_t)VADC_G_EMUXCTR_EMXCOD_Pos)  |
                ((uint32_t)emux_cfg.emux_mode  << (uint32_t)VADC_G_EMUXCTR_EMUXMODE_Pos) |
                ((uint32_t)emux_cfg.stce_usage << (uint32_t)VADC_G_EMUXCTR_EMXST_Pos);

#if (CY_VADC_EMUX_CH_SEL_STYLE == 1U)
  emux_config |= ((uint32_t)emux_cfg.emux_channel_select_style << (uint32_t)VADC_G_EMUXCTR_EMXCSS_Pos);
#endif
  group_ptr->EMUXCTR  |= (emux_config | ((uint32_t)VADC_G_EMUXCTR_EMXWC_Msk)) ;
}

#if CY_VADC_BOUNDARY_FLAG_SELECT == 1U

/**
 * @param group_ptr Constant pointer to the VADC group
 * @param boundary_flag_num The Boundary flag for which the interrupt node needs to be configured.
 *                          Range: [0x0 to 0x3]
 * @param node    Service Request node Id
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Connects the boundary event to the SR line of VADC or to a common boundary flag.<BR>\n
 * This API will connect a Service Request line(SR) to a boundary event. Hence to get a interrupt on this
 * Service request line one has to enable the required NVIC node.  A call to this API would configure the register bit
 * field GxBFLNP.BFLxNP.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
void Cy_VADC_GROUP_SetBoundaryEventInterruptNode(Cy_VADC_GROUP_t *const group_ptr,
    const uint8_t boundary_flag_num,
    const Cy_VADC_BOUNDARY_NODE_t node);
#endif

/**
 * @param group_ptr Constant pointer to the VADC group
 * @return
 *    uint32_t The complete GxALIAS register
 *
 * \par<b>Description:</b><br>
 * Returns the ALIAS values.\n The ALIAS value that is configured for Channel-0 and channel-1 are returned.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE uint32_t Cy_VADC_GROUP_GetAlias(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  return (group_ptr->ALIAS);
}

/**
 * @param group_ptr Constant pointer to the VADC group
 * @param conv_class  conversion property to be extracted
 * @return
 *    Cy_VADC_GROUP_CLASS_t The complete GxICLASSy register
 *
 * \par<b>Description:</b><br>
 * Returns the input class configuration values.\n
 * This returns the sampling time configuration and resolution configured in the appropriate group input class
 * \b conv_class. A call to this API would return the register GxICLASSy.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE Cy_VADC_GROUP_CLASS_t Cy_VADC_GROUP_GetInputClass(Cy_VADC_GROUP_t *const group_ptr,
    const Cy_VADC_CHANNEL_CONV_t conv_class)
{
  Cy_VADC_GROUP_CLASS_t input_value;
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT((CY_VADC_CHANNEL_CONV_GROUP_CLASS0 == conv_class) || (CY_VADC_CHANNEL_CONV_GROUP_CLASS1 == conv_class));

  input_value.g_iclass0 = (uint32_t) 0xFFFFFFFF;
  if ((CY_VADC_CHANNEL_CONV_GROUP_CLASS0 == conv_class) || (CY_VADC_CHANNEL_CONV_GROUP_CLASS1 == conv_class))
  {
    input_value.g_iclass0 = group_ptr->ICLASS[(uint32_t)conv_class];
  }

  return (input_value);
}
#endif

#if (CY_VADC_GSCAN_AVAILABLE == 1U)
/**
 * @param group_ptr Pointer to the VADC group
 * @param config     Pointer to Scan configuration
 * @return None
 *
 * \par<b>Description:</b><br>
 * Initializes the VADC SCAN functional block.<BR>\n
 * The GROUP SCAN request source functional block converts channels sequentially starting with the highest numbered
 * channel to the lowest. Channels must register themselves as being part of the the scan sequence.
 * A call to this API will first disable the arbitration slot for queue (Cy_VADC_GROUP_ScanEnableArbitrationSlot())
 * and then it would configure all the related registers with the required configuration values.
 * The arbitration slot is re-enabled at the end of init by invoking Cy_VADC_GROUP_ScanDisableArbitrationSlot().
 * A call to this API would configure the registers GxARBPR, GxASCTRL, GxASMR needed scan request source.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanEnableArbitrationSlot()<BR> Cy_VADC_GROUP_ScanDisableArbitrationSlot()<BR>
 *  Cy_VADC_GROUP_ScanSelectTrigger()<BR> Cy_VADC_GROUP_ScanSelectGating()<BR>
 */
void Cy_VADC_GROUP_ScanInit(Cy_VADC_GROUP_t *const group_ptr, const Cy_VADC_SCAN_CONFIG_t *config);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables arbitration slot of the scan request source.<BR>\n
 * If the scan request source must have its conversion request considered by the arbiter, it must participate in
 * the arbitration rounds. Even if a load event occurs the scan channel can only be converted when the arbiter comes
 * to the scan slot. Thus this must be enabled if any conversion need to take place.
 * A call to this API would configure the register bit field GxARBPR.ASEN1.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanDisableArbitrationSlot()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanEnableArbitrationSlot(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->ARBPR |= (uint32_t)VADC_G_ARBPR_ASEN1_Msk;
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disables arbitration slot of the scan request source.<BR>\n
 * If the scan request source must have its conversion request considered by the arbiter, it must participate in
 * the arbitration rounds. Even if a load event occurs the scan channel can only be converted when the arbiter comes
 * to the scan slot.A call to this API will lead to all conversions request by scan to be blocked.
 * A call to this API would configure the register bit field GxARBPR.ASEN1.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanEnableArbitrationSlot()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanDisableArbitrationSlot(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->ARBPR &= ~((uint32_t)VADC_G_ARBPR_ASEN1_Msk);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    bool  returns true if the arbitration is enabled else returns false.
 *
 * \par<b>Description:</b><br>
 * Returns the arbitration status of the scan request source.<BR>\n
 * If the scan request source must have its conversion request considered by the arbiter, it must participate in
 * the arbitration rounds. Even if a load event occurs the scan channel can only be converted when the arbiter comes
 * to the scan slot. A call to this API would return the status of the arbitration slot of scan.
 * A call to this API would read the register bit field GxARBPR.ASEN1.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanEnableArbitrationSlot(),<BR>  Cy_VADC_GROUP_ScanDisableArbitrationSlot()<BR>
 */
__STATIC_INLINE bool Cy_VADC_GROUP_ScanIsArbitrationSlotEnabled(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

  return ((group_ptr->ARBPR & (uint32_t)VADC_G_ARBPR_ASEN1_Msk) >> VADC_G_ARBPR_ASEN1_Pos);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param trigger_input  Choice of the input earmarked as a trigger line
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Select Trigger signal for scan request source.<BR>\n
 * A scan request source will raise conversion request only if there were either a request from application or
 * occurrence of a hardware trigger. This API selects one of the 16 input lines as a trigger line. This is
 * needed when a hardware trigger is needed for the conversion of the scan request source.
 * Refer to the reference manual to determine the signal that needs to be connected.
 * A call to this API would configure the register bit field GxASCTRL.XTSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanSelectGating()<BR> Cy_VADC_GROUP_ScanEnableExternalTrigger()<BR>
 */
void Cy_VADC_GROUP_ScanSelectTrigger(Cy_VADC_GROUP_t *const group_ptr, Cy_VADC_TRIGGER_INPUT_SELECT_t trigger_input);


/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param trigger_edge  Trigger edge selection
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Selects the trigger edge for scan request source.<BR>\n
 * A scan request source will raise conversion request only if there were either a request from application or
 * occurrence of a hardware trigger. This API selects one of the 4 possible trigger edges. This is
 * needed when a hardware trigger is needed for the conversion of the scan request source.
 * A call to this API would configure the register bit field GxASCTRL.XTMODE.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanSelectTrigger()<BR>
 */
void Cy_VADC_GROUP_ScanSelectTriggerEdge(Cy_VADC_GROUP_t *const group_ptr, const Cy_VADC_TRIGGER_EDGE_t trigger_edge);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param gating_input  Module input signal meant to be selected as gating input
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Select Gating signal for scan request source.<BR>\n
 * Passage of the trigger input to the request source can be controlled via a gating signal. Any one of the 16 input
 * lines can be chosen as a gating signal. Trigger signal can be given to the scan request source only
 * when the gating signal's active level is detected. Additionally the GxASMR.ENGT has to be configured for
 * the gating signal's active level. A call to this API would configure the register bit field GxASCTRL.GTSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanSelectTrigger()<BR>
 */
void Cy_VADC_GROUP_ScanSelectGating(Cy_VADC_GROUP_t *const group_ptr, Cy_VADC_GATE_INPUT_SELECT_t gating_input);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param mode_sel  Select how the gating is applied to the scan request source
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Selects the gating mode of scan request source.<BR>\n
 * Passage of the trigger input to the request source can be controlled via a gating signal.
 * This API determines how the gating signal behaves, either active low or active high.
 * If gating signal needs to ignored CY_VADC_GATEMODE_IGNORE should be used as the \a mode_sel.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GROUP_ScanSelectGating();
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanSetGatingMode(Cy_VADC_GROUP_t *const group_ptr, Cy_VADC_GATEMODE_t mode_sel)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT((mode_sel <= CY_VADC_GATEMODE_ACTIVELOW));

  /* Clear the existing gate configuration */
  group_ptr->ASMR &= (uint32_t) (~((uint32_t)VADC_G_ASMR_ENGT_Msk));
  /* Set the new gating mode */
  group_ptr->ASMR |= (uint32_t)((uint32_t)mode_sel << VADC_G_ASMR_ENGT_Pos);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables continuous conversion mode.<BR>\n
 * Typically for a scan request source to generate conversion request, either a hardware trigger or a software
 * request is a pre-requisite. Using autoscan feature it is possible to start the conversion once and allow the
 * sequence to repeat without any further triggers. Once all channels belonging to a scan request source have
 * been converted, a request source completion event is generated. Generation of this event can restart the scan
 * sequence. Every request source event will cause a load event to occur. A call to this API would configure
 * the register bit field GxASMR.SCAN.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanDisableContinuousMode()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanEnableContinuousMode(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->ASMR |= (uint32_t)VADC_G_ASMR_SCAN_Msk;
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disables continuous conversion mode.<BR>\n
 * Typically for a scan request source to generate conversion request, either a hardware trigger or a software
 * request is a pre-requisite. Using autoscan feature it is possible to start the conversion once and allow the
 * sequence to repeat without any further triggers. Once all channels belonging to a scan request source have
 * been converted, a request source completion event is generated. Generation of this event can restart the scan
 * sequence. By invoking this feature the Autoscan mode of operations is disabled. A call to this API would configure
 * the register bit field GxASMR.SCAN.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanEnableContinuousMode()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanDisableContinuousMode(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->ASMR &= ~((uint32_t)VADC_G_ASMR_SCAN_Msk);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Generates conversion request (Software initiated conversion).<BR>\n
 * A conversion request can be raised either upon detection of a hardware trigger, or by software. This API forces
 * the scan unit to generate a conversion request to the analog converter. It is assumed that the scan has already
 * been filled up with entries. A call to this API would configure the register bit field GxASMR.LDEV.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanTriggerConversion(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->ASMR |= (uint32_t)VADC_G_ASMR_LDEV_Msk;
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Aborts an ongoing scan sequence conversion.<BR>\n
 * An ongoing sequence can be aborted at any time. The scan unit picks the pending channels one by one from a
 * pending register and requests for their conversion. This API essentially clears the channel pending register thus
 * creating an illusion that there are no more channels left in the sequence.
 * A call to this API would configure the registers GxASMR, GxASCTRL, GxARBPR to achieve the sequence abort.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
void Cy_VADC_GROUP_ScanSequenceAbort(Cy_VADC_GROUP_t *const group_ptr);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num     The channel meant to be added to scan sequence
 *                   <BR>Range: [0x0 to 0x7]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Adds a channel to the scan sequence.<BR>\n
 * Call this API to insert a new single channel into the scan request source. This will be added to the scan
 * sequence. The added channel will be part of the conversion sequence when the next load event occurs.
 * A call to this API would configure the register bit fields of GxASSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanAddMultipleChannels()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanAddChannelToSequence(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_num)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((ch_num) < CY_VADC_NUM_CHANNELS_PER_GROUP));
  group_ptr->ASSEL |= (uint32_t)((uint32_t)1 << ch_num);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_mask    Mask word indicating channels which form part of scan conversion sequence
 *                   Bit location 0/1/2/3/4/5/6/7 represents channels-0/1/2/3/4/5/6/7 respectively.
 *                   To Add the channel to the scan sequence enable the respective bit.
 *                   Passing a 0x0 will clear all the selected channels
 *                   <BR>Range: [0x0 to 0xFF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Adds multiple channels to the scan sequence.<BR>\n
 * Call this API to insert a multiple channels into the scan request source. This will be added to a scan
 * sequence. The added channels will be a part of the conversion sequence when the next load event occurs.
 * A call to this API would configure the register bit fields of GxASSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanAddChannelToSequence()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanAddMultipleChannels(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_mask)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->ASSEL = ch_mask;
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num     The channel being audited for completion of conversion
 *                   <BR>Range: [0x0 to 0x7]
 * @return
 *     bool   returns true if the channel is pending conversion else returns false
 *
 * \par<b>Description:</b><br>
 * Determine if the channel is pending for conversion.<BR>\n
 * This API will check if the Channel in question is awaiting conversion in the current arbitration round.
 * When a load event occurs the scan sequence is pushed to a pending conversion register.
 * From the pending register the channels are taken up by the converter. This API would return true
 * if the channel is found in the pending register (GxASPND).
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanGetNumChannelsPending()<BR>
 */
__STATIC_INLINE bool Cy_VADC_GROUP_ScanIsChannelPending(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_num)
{

  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((ch_num) < CY_VADC_NUM_CHANNELS_PER_GROUP));

  return ( (bool)((uint32_t)(group_ptr->ASPND >> ch_num) & 1U));
}

/**
 * @param  group_ptr     Constant pointer to the VADC group
 * @return  <BR>
 *   uint32_t   Returns the total channels pending for conversion.
 *              <BR>Range: [0x0 to 0x8]
 *
 * \par<b>Description:</b><br>
 * Returns the total number of pending channels.<BR>\n
 * This API will read the pending channels register and will return the number of channels that are awaiting conversion.
 * When a load event occurs the scan sequence is pushed to a pending conversion register.
 * From the pending register the channels are taken up by the converter. When the API is called it would
 * return the total number of channels pending (GxASPND).
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanIsChannelPending()<BR>
 */
uint32_t Cy_VADC_GROUP_ScanGetNumChannelsPending(Cy_VADC_GROUP_t *const group_ptr);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Manually asserts the conversion complete request source event.<BR>\n
 * This API will set the request source event for scan. This will trigger a interrupt if the
 * service node pointer for the scan has been configured.
 * A call to this API would configure the register bit field GxSEFLAG.SEV1.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanTriggerReqSrcEvent(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->SEFLAG    |= (uint32_t)VADC_G_SEFLAG_SEV1_Msk;
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Acknowledges the scan conversion complete request source event.<BR>\n
 * This API will clear the request source event that occurred. This will clear a interrupt if it was raised.
 * A call to this API would configure the register bit field GxSEFLAG.SEV1.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanTriggerReqSrcEvent()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanClearReqSrcEvent(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->SEFCLR |= (uint32_t)VADC_G_SEFCLR_SEV1_Msk;
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *   bool   returns true if the service request event is raised.
 *          returns false if the service request event was not raised.
 *
 * \par<b>Description:</b><br>
 * Determines if the request source event is asserted.<BR>
 * This API will get the status of the scan request source event. Will return a true
 * if the event has occurred for scan. A call to this API would access the register bit field GxSEFLAG.SEV1.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE bool Cy_VADC_GROUP_ScanGetReqSrcEventStatus(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  return ( (bool)(group_ptr->SEFLAG & (uint32_t)VADC_G_SEFLAG_SEV1_Msk));
}

/**
 * @param group_ptr Constant pointer to the VADC group
 * @param sr    Service Request Id
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Connects the scan request source event to the SR line of VADC.<BR>\n
 * This API will connect a Service Request line(SR) to a scan request source event. Hence to get a interrupt on this
 * Service request line one has to enable the required NVIC node.  A call to this API would configure the register bit
 * field GxSEVNP.SEV1NP .
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
void Cy_VADC_GROUP_ScanSetReqSrcEventInterruptNode(Cy_VADC_GROUP_t *const group_ptr, const Cy_VADC_SR_t sr);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables the trigger for scan request source.<BR>\n
 * By using this API, the trigger signal will be activated for the scan request source. The trigger signal and trigger
 * edge will be selected from the ASCTRL register. The Selection of a input will be done by
 * Cy_VADC_GROUP_ScanSelectTrigger(). A call to this API would configure the register bit field GxASMR.ENTR.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanSelectTrigger()<BR> Cy_VADC_GROUP_ScanDisableExternalTrigger()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanEnableExternalTrigger(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

  group_ptr->ASMR |= (uint32_t)VADC_G_ASMR_ENTR_Msk;
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disables the trigger for scan request source.<BR>
 * By using this API the trigger will be deactivated for the scan request source.
 * This will just deactivate the H/W trigger for the scan request source. If any configuration were done
 * to select the trigger input in GxASCTRL, it will be not be effected by this API.
 * A call to this API would configure the register bit field GxASMR.ENTR.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanEnableExternalTrigger()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanDisableExternalTrigger(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

  group_ptr->ASMR &= ~((uint32_t)VADC_G_ASMR_ENTR_Msk);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param channel_num   channel number to be removed from the scan sequence.
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Removes a channel from the scan sequence.<BR>
 * By using this API the it is possible to remove a single channel from the conversion sequence.
 * The remaining channels will continue however they are.
 * A call to this API would configure the register GxASSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
void Cy_VADC_GROUP_ScanRemoveChannel(Cy_VADC_GROUP_t *const group_ptr, const uint32_t channel_num);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables the scan request source event .<BR>
 * By using this API the request source event will be activated for the scan request source.
 * Other configurations w.r.t service node pointer are not done in this API.
 * A call to this API would configure the register bit field GxASMR.ENSI.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanDisableEvent(),<BR> Cy_VADC_GROUP_ScanSetReqSrcEventInterruptNode()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanEnableEvent(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

  group_ptr->ASMR |= ((uint32_t)VADC_G_ASMR_ENSI_Msk);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disables the scan request source event .<BR>
 * By using this API the request source event will be deactivated for the scan request source.
 * Other configurations w.r.t service node pointer are not done in this API.
 * A call to this API would configure the register bit field GxASMR.ENSI.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_ScanEnableEvent(),<BR> Cy_VADC_GROUP_ScanSetReqSrcEventInterruptNode()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ScanDisableEvent(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

  group_ptr->ASMR &= ~((uint32_t)VADC_G_ASMR_ENSI_Msk);
}
#endif

/**
 * @param global_ptr Pointer to the VADC module
 * @param config     Pointer to initialization data structure
 *
 * \par<b>Description:</b><br>
 * Initializes the Background scan functional block.<BR>\n
 * The BACKGROUND SCAN request source functional block converts channels of all VADC groups that have not
 * been assigned as a priority channel (priority channels can be converted only by queue and scan). Background Scan
 * request source converts the unprioritized channels. Unprioritized channels however can also be used with queue
 * and scan. But a channel which is prioritized can not be used with background request source.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_BackgroundEnableArbitrationSlot()<BR> Cy_VADC_GROUP_BackgroundDisableArbitrationSlot()<BR>
 *  Cy_VADC_GLOBAL_BackgroundSelectTrigger()<BR> Cy_VADC_GLOBAL_BackgroundSelectGating()<BR>
 */
void Cy_VADC_GLOBAL_BackgroundInit(Cy_VADC_GLOBAL_t *const global_ptr, const Cy_VADC_BACKGROUND_CONFIG_t *config);

#if (CY_VADC_GROUP_AVAILABLE == 1U)
/**
 * @param group_ptr Constant pointer to the VADC group which may receive a
 *                  conversion request from background request source
 *
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables arbitration slot of the Background request source.<BR>\n
 * If the Background request source must have its conversion request considered by the arbiter, it must participate in
 * the arbitration rounds. Even if a load event occurs the Background channel can only be converted when the arbiter
 * comes to the Background slot. Thus this must be enabled if any conversion need to take place.
 * A call to this API would configure the register bit field GxARBPR.ASEN2.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_BackgroundDisableArbitrationSlot()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_BackgroundEnableArbitrationSlot(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->ARBPR |= (uint32_t)VADC_G_ARBPR_ASEN2_Msk;
}

/**
 * @param group_ptr  Constant pointer to the VADC group which may receive a conversion request
 *                     from background request source
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disables arbitration slot of the Background request source.<BR>\n
 * If the Background request source must have its conversion request considered by the arbiter, it must participate in
 * the arbitration rounds. Even if a load event occurs the Background channel can only be converted when the arbiter
 * comes to the Background slot.A call to this API will lead to all conversions request by Background to be blocked.
 * A call to this API would configure the register bit field GxARBPR.ASEN2
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_BackgroundEnableArbitrationSlot()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_BackgroundDisableArbitrationSlot(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->ARBPR &= ~((uint32_t)VADC_G_ARBPR_ASEN2_Msk);
}
#endif

/**
 * @param global_ptr       Pointer to the VADC module
 * @param input_num  Choice of the input earmarked as a trigger line
 *                   Accepts enum ::Cy_VADC_TRIGGER_INPUT_SELECT_t
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Select Trigger signal for Background request source.<BR>\n
 * A Background request source will raise conversion request only if there were either a request from application or
 * occurrence of a hardware trigger. This API selects one of the 16 input lines as a trigger line. This is
 * needed when a hardware trigger is needed for the conversion of the Background request source.
 * Refer to the reference manual to determine the signal that needs to be connected.
 * A call to this API would configure the register bit field BRSCTRL.XTSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundSelectGating()<BR> Cy_VADC_GLOBAL_BackgroundEnableExternalTrigger()<BR>
 */
void Cy_VADC_GLOBAL_BackgroundSelectTrigger(Cy_VADC_GLOBAL_t *const global_ptr, const uint32_t input_num);


/**
 * @param global_ptr       Pointer to the VADC module
 * @param trigger_edge  Select the trigger edge
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Select Trigger edge for Background request source.<BR>\n
 * A Background request source will raise conversion request only if there were either a request from application or
 * occurrence of a hardware trigger. This API selects one of the 4 possible values for the trigger edge. This is
 * needed when a hardware trigger is needed for the conversion of the Background request source.
 * A call to this API would configure the register bit field BRSCTRL.XTMODE.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundSelectGating()<BR> Cy_VADC_GLOBAL_BackgroundEnableExternalTrigger()<BR>
 */
void Cy_VADC_GLOBAL_BackgroundSelectTriggerEdge(Cy_VADC_GLOBAL_t *const global_ptr, const Cy_VADC_TRIGGER_EDGE_t trigger_edge);

/**
 * @param global_ptr       Pointer to the VADC module
 * @param input_num  Module input signal meant to be selected as gating input
 *                   Accepts enum ::Cy_VADC_GATE_INPUT_SELECT_t
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Select Gating signal for Background request source.<BR>\n
 * Passage of the trigger input to the request source can be controlled via a gating signal. Any one of the 16 input
 * lines can be chosen as a gating signal. Trigger signal can be given to the Background request source only
 * when the gating signal's active level is detected. Additionally the GxBRSMR.ENGT has to be configured for
 * the gating signal's active level. A call to this API would configure the register bit field BRSCTRL.GTSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundSelectTrigger()<BR>
 */
void Cy_VADC_GLOBAL_BackgroundSelectGating(Cy_VADC_GLOBAL_t *const global_ptr, const uint32_t input_num);

/**
 * @param global_ptr       Pointer to the VADC module
 * @param mode_sel  Select how the gating is applied to the background scan request source
 * @return
 *    None
 *
 * <b>Details of function</b><br>
 * Selects the gating mode of background request source.<BR>\n
 * Passage of the trigger input to the request source can be controlled via a gating signal.
 * This API determines how the gating signal behaves, either active low or active high.
 * If gating signal needs to ignored CY_VADC_GATEMODE_IGNORE should be used as the \a mode_sel.
 * A call to this API would configure the register bit field BRSMR.ENGT.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GLOBAL_BackgroundSelectGating();
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundSetGatingMode(Cy_VADC_GLOBAL_t *const global_ptr,
    Cy_VADC_GATEMODE_t mode_sel)
{
  CY_ASSERT((global_ptr == VADC));
  CY_ASSERT((mode_sel <= CY_VADC_GATEMODE_ACTIVELOW));

  /* Clear the existing gate configuration */
  global_ptr->BRSMR &= (uint32_t)(~((uint32_t)VADC_BRSMR_ENGT_Msk));
  /* Configure the new gating mode*/
  global_ptr->BRSMR |= (uint32_t)((uint32_t)mode_sel << VADC_BRSMR_ENGT_Pos);
}


/**
 * @param global_ptr       Pointer to the VADC module
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables continuous conversion mode.<BR>\n
 * Typically for a Background request source to generate conversion request, either a hardware trigger or a software
 * request is needed. Using autoscan (continuous conversion)feature it is possible to start the conversion
 * once and allow the sequence to repeat without any further triggers. Once all channels belonging to a Background
 * request source have been converted, a request source completion event is generated. Generation of this event
 * can restart the Background configure sequence. Every request source event will cause a load event to occur.
 * A call to this API would access the register bit field BRSMR.SCAN.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_BackgroundDisableContinuousMode()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundEnableContinuousMode(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  global_ptr->BRSMR |= (uint32_t)VADC_BRSMR_SCAN_Msk;
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disables continuous conversion mode.<BR>\n
 * Typically for a Background request source to generate conversion request, either a hardware trigger or a software
 * request is a pre-requisite. Using autoscan feature it is possible to start the conversion once and allow the
 * sequence to repeat without any further triggers. Once all channels belonging to a Background request source have
 * been converted, a request source completion event is generated. Generation of this event can restart the Background
 * sequence. By invoking this API the Autoscan mode of operations is disabled. A call to this API would configure the
 * register bit field BRSMR.SCAN.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundEnableContinuousMode()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundDisableContinuousMode(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  global_ptr->BRSMR &= ~((uint32_t)VADC_BRSMR_SCAN_Msk);
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Generates conversion request (Software initiated conversion).<BR>\n
 * A conversion request can be raised either upon detection of a hardware trigger, or by software. This API forces
 * the scan unit to generate a conversion request to the analog converter. It is assumed that the background scan
 * has already been filled up with entries. A call to this API would set the register bit field BRSMR.LDEV.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundTriggerConversion(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  global_ptr->BRSMR |= (uint32_t)VADC_BRSMR_LDEV_Msk;
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Aborts an ongoing background scan conversion(sequence).<BR>\n
 * An ongoing sequence can be aborted at any time. The scan unit picks the pending channels one by one from a
 * pending register and requests for their conversion. This API essentially clears the channel pending register thus
 * creating an illusion that there are no more channels left in the sequence.
 * A call to this API would configure the registers BRSMR, BRSCTRL, GxARBPR(if group is available) to abort the
 * current scan sequence.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
void Cy_VADC_GLOBAL_BackgroundAbortSequence(Cy_VADC_GLOBAL_t *const global_ptr);

/**
 * @param global_ptr       Pointer to the VADC module
 * @param grp_num    ID of the VADC group whose unprioritized channels have been assigned to background scan
 *                   Request source
 * @param ch_num     The unprioritized channel meant to be added to the scan sequence
 *                   <BR>Range: [0x0 to 0x7]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Adds a channel to the background scan sequence.<BR>\n
 * Call this API to insert a new single channel into the background scan request source. This will be added to the scan
 * sequence. The added channel will be part of the conversion sequence when the next load event occurs.
 * A call to this API would configure the register bit fields of BRSSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundAddMultipleChannels()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundAddChannelToSequence(Cy_VADC_GLOBAL_t *const global_ptr,
    const uint32_t grp_num,
    const uint32_t ch_num)
{
  CY_ASSERT((global_ptr == VADC));
  CY_ASSERT(((grp_num) < CY_VADC_MAXIMUM_NUM_GROUPS));
  CY_ASSERT(((ch_num) < CY_VADC_NUM_CHANNELS_PER_GROUP));

  global_ptr->BRSSEL[grp_num] |= (uint32_t)((uint32_t)1 << ch_num);
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @param grp_num    ID of the VADC group whose unprioritized channels have been assigned to background scan
 * @param ch_mask    Mask word indicating channels which form part of scan conversion sequence
 *                   Bit location 0/1/2/3/4/5/6/7 represents channels-0/1/2/3/4/5/6/7 respectively.
 *                   To Add the channel to the scan sequence enable the respective bit.
 *                   Passing a 0x0 will clear all the previously selected channels
 *                   <BR>Range: [0x0 to 0xFF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Adds multiple channels to the scan sequence.<BR>\n
 * Call this API to insert a multiple channels into the scan request source. This will be added to a scan
 * sequence. The added channels will be a part of the conversion sequence when the next load event occurs.
 * A call to this API would configure the register bit fields of BRSSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundAddChannelToSequence()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgndAddMultipleChannels(Cy_VADC_GLOBAL_t *const global_ptr,
    const uint32_t grp_num,
    const uint32_t ch_mask)
{
  CY_ASSERT((global_ptr == VADC));
  CY_ASSERT(((grp_num) < CY_VADC_MAXIMUM_NUM_GROUPS));
  global_ptr->BRSSEL[grp_num] |= ch_mask;
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @param grp_num    ID of the VADC group whose unprioritized channels have been assigned to background scan
 *                   Request source
 * @param ch_num     The unprioritized channel meant to be added to the scan sequence
 *                   <BR>Range: [0x0 to 0x7]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Removes a channel to the background scan sequence.<BR>\n
 * Call this API to insert a new single channel into the background scan request source. This will be added to the scan
 * sequence. The added channel will be part of the conversion sequence when the next load event occurs.
 * A call to this API would configure the register bit fields of BRSSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundAddChannelToSequence()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundRemoveChannelFromSequence(Cy_VADC_GLOBAL_t *const global_ptr,
    const uint32_t grp_num,
    const uint32_t ch_num)
{
  CY_ASSERT((global_ptr == VADC));
  CY_ASSERT(((grp_num) < CY_VADC_MAXIMUM_NUM_GROUPS));
  CY_ASSERT(((ch_num) < CY_VADC_NUM_CHANNELS_PER_GROUP));

  global_ptr->BRSSEL[grp_num] &= (uint32_t)~((uint32_t)1 << ch_num);
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @param grp_num    ID of the VADC group whose unprioritized channels have been assigned to background scan
 * @param ch_mask    Mask word indicating channels which form part of scan conversion sequence
 *                   Bit location 0/1/2/3/4/5/6/7 represents channels-0/1/2/3/4/5/6/7 respectively.
 *                   To Add the channel to the scan sequence enable the respective bit.
 *                   Passing a 0x0 will clear all the previously selected channels
 *                   <BR>Range: [0x0 to 0xFF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Removes multiple channels to the scan sequence.<BR>\n
 * Call this API to insert a multiple channels into the scan request source. This will be added to a scan
 * sequence. The added channels will be a part of the conversion sequence when the next load event occurs.
 * A call to this API would configure the register bit fields of BRSSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundAddChannelToSequence()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgndRemoveMultipleChannels(Cy_VADC_GLOBAL_t *const global_ptr,
    const uint32_t grp_num,
    const uint32_t ch_mask)
{
  CY_ASSERT((global_ptr == VADC));
  CY_ASSERT(((grp_num) < CY_VADC_MAXIMUM_NUM_GROUPS));
  global_ptr->BRSSEL[grp_num] &= (uint32_t)~ch_mask;
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @param grp_num    ID of the VADC group whose unprioritized channels have been assigned to background scan RS
 * @param ch_num     The channel being audited for completion of conversion
 *                   <BR>Range: [0x0 to 0x7]
 * @return
 *     bool   returns true if the channel is pending conversion else returns false
 *
 * \par<b>Description:</b><br>
 * Determine if the channel is pending.<BR>\n
 * This API will check if the Channel in question is awaiting conversion in the current arbitration round.
 * When a load event occurs the scan sequence is pushed to a pending conversion register.
 * From the pending register the channels are taken up by the converter. This API would return true
 * if the channel is found in the pending register (BRSPND[\b grp_num]).
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundGetNumChannelsPending()<BR>
 */
__STATIC_INLINE bool Cy_VADC_GLOBAL_BackgroundIsChannelPending(Cy_VADC_GLOBAL_t *const global_ptr,
    const uint32_t grp_num,
    const uint32_t ch_num)
{
  CY_ASSERT((global_ptr == VADC));
  CY_ASSERT(((grp_num) < CY_VADC_MAXIMUM_NUM_GROUPS));
  CY_ASSERT(((ch_num) < CY_VADC_NUM_CHANNELS_PER_GROUP));

  return ( (bool)(global_ptr->BRSPND[grp_num] & (uint32_t)((uint32_t)1 << ch_num)));
}

/**
 * @param global_ptr       Pointer to the VADC module
  * @return  <BR>
 *   uint32_t   Returns the total channels pending for conversion.
 *              <BR>Range: [0x0 to (0x8*number of groups)]
 *
 * \par<b>Description:</b><br>
 * Returns the number of pending channels.<BR>\n
 * This API will read the pending channels register and will return the number of channels that are awaiting conversion.
 * When a load event occurs the scan sequence is pushed to a pending conversion register.
 * From the pending register the channels are taken up by the converter. When the API is called it would
 * return the total number of channels pending (BRSPND[\b grp_num]).
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundIsChannelPending()<BR>
 */
uint32_t Cy_VADC_GLOBAL_BackgroundGetNumChannelsPending(Cy_VADC_GLOBAL_t *const global_ptr);

/**
 * @param global_ptr       Pointer to the VADC module
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Manually asserts the conversion complete request source event.<BR>\n
 * This API will set the request source event for background scan. This will trigger a interrupt if the
 * service node pointer for the scan has been configured.
 * A call to this API would configure the register bit field GLOBEFLAG.SEVGLB.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundTriggerReqSrcEvent(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  global_ptr->GLOBEFLAG |= (uint32_t)VADC_GLOBEFLAG_SEVGLB_Msk;
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Acknowledges the background scan conversion complete request source event.<BR>\n
 * This API will clear the request source event that occurred. This will clear a interrupt if it was raised.
 * A call to this API would configure the register bit field GLOBEFLAG.SEVGLB
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundTriggerReqSrcEvent()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundClearReqSrcEvent(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  global_ptr->GLOBEFLAG |= (uint32_t)VADC_GLOBEFLAG_SEVGLBCLR_Msk;
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @return
 *   bool   returns true if the service request event is raised.
 *          returns false if the service request event was not raised.
 *
 * \par<b>Description:</b><br>
 * Determines if the request source event is asserted.<BR>
 * This API will get the status of the background scan request source event. Will return a true
 * if the event has occurred for background scan. A call to this API would configure the register
 * bit field GLOBEFLAG.SEVGLB.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE bool Cy_VADC_GLOBAL_BackgroundGetReqSrcEventStatus(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));
  return ((bool)(global_ptr->GLOBEFLAG & (uint32_t)VADC_GLOBEFLAG_SEVGLB_Msk));
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables the trigger for background scan request source.<BR>\n
 * By using this API the trigger will be activated for the scan request source. The trigger signal and trigger
 * edge will be selected from the BRSCTRL register. The Selection of a input will be done by
 * Cy_VADC_GLOBAL_BackgroundSelectTrigger(). A call to this API would configure the register bit field BRSMR.ENTR.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundSelectTrigger()<BR> Cy_VADC_GLOBAL_BackgroundDisableExternalTrigger()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundEnableExternalTrigger(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));

  global_ptr->BRSMR |= (uint32_t)VADC_BRSMR_ENTR_Msk;
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disables the trigger for background scan request source.<BR>
 * By using this API the trigger will be deactivated for the background scan request source.
 * This will just deactivate the H/W trigger for the background scan request source. If any configuration was done
 * to select the trigger input in BRSCTRL will be not be effected. A call to this API would configure the register
 * bit field BRSMR.ENTR.
 *
  * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundEnableExternalTrigger()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundDisableExternalTrigger(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));

  global_ptr->BRSMR &= ~((uint32_t)VADC_BRSMR_ENTR_Msk);
}

/**
 * @param global_ptr       Pointer to the VADC module
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables the background scan request source event .<BR>
 * By using this API the request source event will be activated for the background scan request source.
 * Other configurations w.r.t service node pointer are not done in this API.
 * A call to this API would configure the register bit field BRSMR.ENSI.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GLOBAL_BackgroundEnableEvent(),<BR> Cy_VADC_GLOBAL_BackgroundSetReqSrcEventInterruptNode()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundEnableEvent(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));

  global_ptr->BRSMR |= ((uint32_t)VADC_BRSMR_ENSI_Msk);
}

/**
 * @param global_ptr     Pointer to the VADC module
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disables the background scan request source event .<BR>
 * By using this API the request source event will be deactivated for the background scan request source.
 * Other configurations w.r.t service node pointer are not done in this API.
 * A call to this API would configure the register bit field BRSMR.ENSI.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_BackgroundEnableEvent(),<BR> Cy_VADC_GLOBAL_BackgroundSetReqSrcEventInterruptNode()<BR>
 */
__STATIC_INLINE void Cy_VADC_GLOBAL_BackgroundDisableEvent(Cy_VADC_GLOBAL_t *const global_ptr)
{
  CY_ASSERT((global_ptr == VADC));

  global_ptr->BRSMR &= ~((uint32_t)VADC_BRSMR_ENSI_Msk);
}

#if (CY_VADC_QUEUE_AVAILABLE == 1U)
/**
 * @param group_ptr Pointer to the VADC group
 * @param config     Pointer to initialization data structure
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Initializes VADC QUEUE functional block.<BR>\n
 * The QUEUE request source functional block converts channels stored in a queue. The first channel entered into the
 * queue is converted first. A channel once converted, can be placed back into the queue if desired(refill).
 * A call to this API will first disable the arbitration slot for queue (Cy_VADC_GROUP_QueueEnableArbitrationSlot())
 * and then it would configure all the related registers with the required configuration values.
 * The arbitration slot is re-enabled at the end of init by invoking Cy_VADC_GROUP_QueueDisableArbitrationSlot().
 * A call to this API would configure the registers GxARBPR, GxQCTRL0, GxQMR0 to configure the queue request
 * source.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueEnableArbitrationSlot()<BR> Cy_VADC_GROUP_QueueDisableArbitrationSlot()<BR>
 *  Cy_VADC_GROUP_QueueSelectTrigger()<BR> Cy_VADC_GROUP_QueueSelectGating()<BR>
 */
void Cy_VADC_GROUP_QueueInit(Cy_VADC_GROUP_t *const group_ptr, const Cy_VADC_QUEUE_CONFIG_t *config);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables arbitration slot of the queue request source.<BR>\n
 * If the QUEUE request source must have its conversion request considered by the arbiter, it must participate in
 * the arbitration rounds. Even if a load event occurs the queue channel can only be converted when the arbiter comes
 * to the queue slot. Thus this must be enabled if any conversion need to take place.
 * A call to this API would configure the register bit field GxARBPR.ASEN0.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueDisableArbitrationSlot()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_QueueEnableArbitrationSlot(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->ARBPR |= (uint32_t)((uint32_t)1 << VADC_G_ARBPR_ASEN0_Pos);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disables arbitration slot of the queue request source.<BR>\n
 * If the QUEUE request source must have its conversion request considered by the arbiter, it must participate in
 * the arbitration rounds. Even if a load event occurs the queue channel can only be converted when the arbiter comes
 * to the queue slot.A call to this API will lead to all conversions request by queue to be blocked.
 * A call to this API would configure the register bit field GxARBPR.ASEN0.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueEnableArbitrationSlot()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_QueueDisableArbitrationSlot(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->ARBPR &= ~((uint32_t)VADC_G_ARBPR_ASEN0_Msk);
}


/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    bool  returns true if the arbitration is enabled else returns false.
 *
 * \par<b>Description:</b><br>
 * Returns the arbitration status of the queue request source.<BR>\n
 * If the queue request source must have its conversion request considered by the arbiter, it must participate in
 * the arbitration rounds. Even if a load event occurs the queue channel can only be converted when the arbiter comes
 * to the queue slot. A call to this API would return the status of the arbitration slot of queue.
 * A call to this API would read the register bit field GxARBPR.ASEN1.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueEnableArbitrationSlot(),<BR>  Cy_VADC_GROUP_QueueDisableArbitrationSlot()<BR>
 */
__STATIC_INLINE bool Cy_VADC_GROUP_QueueIsArbitrationSlotEnabled(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

  return ((group_ptr->ARBPR & (uint32_t)VADC_G_ARBPR_ASEN0_Msk) >> VADC_G_ARBPR_ASEN0_Pos);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param input_num  Choice of the input earmarked as a trigger line
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Select Trigger signal for queue request source.<BR>\n
 * A queue request source will raise conversion request only if there were either a request from application or
 * occurrence of a hardware trigger. This API selects one of the 16 input lines as a trigger line. This is
 * needed when a hardware trigger is needed for the conversion of the queue request source.
 * Refer to the reference manual to determine the signal that needs to be connected.
 * A call to this API would configure the register bit field GxQCTRL0.XTSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueSelectGating()<BR> Cy_VADC_GROUP_QueueEnableExternalTrigger()<BR>
 */
void Cy_VADC_GROUP_QueueSelectTrigger(Cy_VADC_GROUP_t *const group_ptr,
                                       const Cy_VADC_TRIGGER_INPUT_SELECT_t input_num);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param trigger_edge  Choice of the trigger edge
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Select Trigger signal edge for queue request source.<BR>\n
 * A queue request source will raise conversion request only if there were either a request from application or
 * occurrence of a hardware trigger. This API selects one of the 4 trigger edges. This is
 * needed when a hardware trigger is needed for the conversion of the queue request source.
 * Refer to the reference manual to determine the signal that needs to be connected.
 * A call to this API would configure the register bit field GxQCTRL0.XTMODE.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueSelectGating()<BR> Cy_VADC_GROUP_QueueEnableExternalTrigger()<BR>
 */
void Cy_VADC_GROUP_QueueSelectTriggerEdge(Cy_VADC_GROUP_t *const group_ptr, const Cy_VADC_TRIGGER_EDGE_t trigger_edge);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param input_num  Choice of the input earmarked as the gating line
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Select Gating signal for queue request source.<BR>\n
 * Passage of the trigger input to the request source can be controlled via a gating signal. Any one of the 16 input
 * lines can be chosen as a gating signal. Trigger signal can be given to the queue request source only
 * when the gating signal's active level is detected. Additionally the GxQMR0.ENGT has to be configured for
 * the gating signal's active level. A call to this API would configure the register bit field GxQCTRL0.GTSEL.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueSelectTrigger()<BR>
 */
void Cy_VADC_GROUP_QueueSelectGating(Cy_VADC_GROUP_t *const group_ptr, const Cy_VADC_GATE_INPUT_SELECT_t input_num);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param mode_sel  Select how the gating is applied to the queue request source
 * @return
 *    None
 *
 * <b>Details of function</b><br>
 * Selects the gating mode of queue request source.<BR>\n
 * Passage of the trigger input to the request source can be controlled via a gating signal.
 * This API determines how the gating signal behaves, either active low or active high.
 * If gating signal needs to ignored CY_VADC_GATEMODE_IGNORE should be used as the \a mode_sel.
 * A call to this API would configure the register bit field GxQMR0.ENGT.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_VADC_GROUP_QueueSelectGating();
 */
__STATIC_INLINE void Cy_VADC_GROUP_QueueSetGatingMode(Cy_VADC_GROUP_t *const group_ptr, Cy_VADC_GATEMODE_t mode_sel)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT((mode_sel <= CY_VADC_GATEMODE_ACTIVELOW));

  /* Clear the existing gate configuration */
  group_ptr->QMR0 &= (uint32_t)(~((uint32_t) VADC_G_QMR0_ENGT_Msk));
  /* Set the new gating mode */
  group_ptr->QMR0 |= (uint32_t)((uint32_t)mode_sel << VADC_G_QMR0_ENGT_Pos);
}


/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Generates conversion request (Software initiated conversion).<BR>
 * A conversion request can be raised either upon detection of a hardware trigger, or by software. This API forces
 * the queue unit to generate a conversion request to the analog converter. It is assumed that the queue has already
 * been filled up with entries. A call to this API would configure the register bit field GxQMR0.TREV.
 *
 * \par<b>Note:</b><br>
 * The conversion of queue entry will start immediately after the entry has been loaded into GxQINR0.
 * This happens only if the queue entry has been loaded into the register without the need for the H/W trigger.\n
 * If a H/W Trigger is selected while loading the entry, the conversion will occur in one of the 2 ways:
 * <ul>
 * <li> The H/W generates a trigger needed for the queue request source.
 * <li> The Conversion is triggered manually by calling this API.
 * </ul>
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueInsertChannel()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_QueueTriggerConversion(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->QMR0 |= (uint32_t)((uint32_t)1 << VADC_G_QMR0_TREV_Pos);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    uint32_t returns the total number of channels.
 *             <BR>Range: [0x0 to 0x8]
 *
 * \par<b>Description:</b><br>
 * Returns the number of channels present in the queue.<BR>\n
 * This API will return the queue buffer size. This buffer will be consisting of valid queue entries which
 * will be converted when a trigger event occurs. All the entries that are loaded onto the GxQINR0 will
 * be added to the queue buffer. Hence if an application needs to get the number of valid queue entries
 * this API would provide the interface. A call to this API would access the registers GxQBUR0, GxQSR0 in order
 * to determine the queue length.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueInsertChannel()<BR>
 */
uint32_t Cy_VADC_GROUP_QueueGetLength(Cy_VADC_GROUP_t *const group_ptr);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Aborts an ongoing conversion by flushing the queue.<BR>\n
 * This API will flush the queue buffer. Ongoing conversion of the Queue request source will
 * not be effected by this API. This would clear all the contents that are present in the queue buffer.
 * A call to this API would configure the registers GxQCTRL0, GxQMR0, GxARBPR in order to abort
 * the queue sequence.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueRemoveChannel()<BR> Cy_VADC_GROUP_QueueFlushEntries() <BR>
 */
void Cy_VADC_GROUP_QueueAbortSequence(Cy_VADC_GROUP_t *const group_ptr);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Flushing the queue Entry.<BR>\n
 * This API will flush one entry in the queue buffer. Ongoing conversion of the Queue request source will
 * not be effected by this API. This would clear all the contents that are present in the queue buffer.
 * A call to this API would configure the registers  GxQMR0. This is a Blocking API, i.e will only exit when
 * all the entries are removed from the queue.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueRemoveChannel()<BR> Cy_VADC_GROUP_QueueAbortSequence(0<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_QueueFlushEntries(Cy_VADC_GROUP_t *const group_ptr)
{
  /* Initiate flushing of the queue */
  group_ptr->QMR0 |= (uint32_t)VADC_G_QMR0_FLUSH_Msk;

  while ( !((group_ptr->QSR0) & (uint32_t)VADC_G_QSR0_EMPTY_Msk))
  {
    /* Wait until the queue is indeed flushed */
  }
}


/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Clears the next valid channel in the queue buffer.<BR>\n
 * A queue entry lined up for conversion can be removed and replaced by its successor. The call to this API will
 * first check if a valid queue entry is present in the queue backup register if present would clear its valid flag.
 * If no valid queue entries are present in the backup then the first channel
 * present in the queue buffer would be cleared.
 * A call to this API would configure the registers GxQCTRL0, GxQMR0, GxARBPR in order to clear a
 * channel from the queue.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueInsertChannel()<BR>
 */
void Cy_VADC_GROUP_QueueRemoveChannel(Cy_VADC_GROUP_t *const group_ptr);

/**
 *
 * @param group_ptr     Constant pointer to the VADC group
 * @param entry      Details of the node being added
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Inserts a queue entry to the tail of the queue buffer.<BR>\n
 * This API will insert a new channel into the queue buffer. The Queue will start conversion of
 * the channels from the head of the buffer. This Insert will place the entry after the last valid entry.
 * If no valid entries are present then this API will place the Queue entry at the head of the buffer.
 * Then the successive call to the insert will place the new entry after the last entry.
 * A call to this API would configure the register GxQINR0 for a single queue entry.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueRemoveChannel()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_QueueInsertChannel(Cy_VADC_GROUP_t *const group_ptr,
    const Cy_VADC_QUEUE_ENTRY_t entry)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  /* Insert the channel physically and get the length of the queue*/
  group_ptr->QINR0 = entry.qinr0;
}

/**
 * @param group_ptr Constant pointer to the VADC group
 * @return
 *     int32_t Returns -1 if there are no channels for conversion
 *             Else would return the next valid channel for conversion.
 *             <BR>Range: [0x0 to 0x8]
 *
 * \par<b>Description:</b><br>
 * Returns the next entry in the queue request source for conversion.<BR>\n
 * Identifies the channel in the queue lined up for conversion next.
 * API will return a valid queue entry from the queue buffer. First checks for the valid channel entry
 * in the backup register and returns if present. If the valid entry has not been found in the backup register
 * then the queue buffer is searched for a valid entry. A call to this API would access the registers GxQ0R0,
 * GxQBUR0 to determine the next channel.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueRemoveChannel()<BR> Cy_VADC_GROUP_QueueInsertChannel()<BR>
 */
int32_t Cy_VADC_GROUP_QueueGetNextChannel(Cy_VADC_GROUP_t *const group_ptr);

/**
 * @param group_ptr Constant pointer to the VADC group
 * @return
 *     int32_t Returns -1 if there is no channel that have been interrupted.
 *             Else would return the channel that is interrupted.
 *             <BR>Range: [0x0 to 0x8]
 *
 * \par<b>Description:</b><br>
 * Identifies the channel whose conversion was suspended.<BR>\n
 * When using cancel inject repeat mode the canceled conversion will be placed in the backup register.
 * This API will return the valid queue channel number from the backup register. This happens when ever
 * there is a high priority conversion interrupts the conversion of queue request source. This forces the channel
 * to goto the backup register. A call to this API would access the register GxQBUR0 to determine the
 * interrupted channel.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
int32_t Cy_VADC_GROUP_QueueGetInterruptedChannel(Cy_VADC_GROUP_t *const group_ptr);

/**
 * @param group_ptr Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Manually asserts the conversion complete request source event.<BR>\n
 * This API will set the request source event for queue. This will trigger a interrupt if the
 * service node pointer for the scan has been configured.
 * A call to this API would configure the register bit field GxSEFLAG.SEV0.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueClearReqSrcEvent()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_QueueTriggerReqSrcEvent(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->SEFLAG |= 1U;
}

/**
 * @param group_ptr Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Acknowledges the conversion complete request source event.<BR>\n
 * This API will clear the request source event that occurred. This will clear a interrupt if it was raised.
 * A call to this API would configure the register bit field GxSEFCLR.SEV0.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueTriggerReqSrcEvent()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_QueueClearReqSrcEvent(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->SEFCLR = (uint32_t)VADC_G_SEFCLR_SEV0_Msk;
}

/**
 * @param group_ptr Constant pointer to the VADC group
 * @return
 *   bool   returns true if the service request event is raised.
 *          returns false if the service request event was not raised.
 *
 * \par<b>Description:</b><br>
 * Determines if the request source event is asserted.<BR>
 * This API will get the status of the queue request source event. Will return a true
 * if the event has occurred for queue. A call to this API would acces the register bit field  GxSEFLAG.SEV0.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE bool Cy_VADC_GROUP_QueueGetReqSrcEventStatus(Cy_VADC_GROUP_t *const group_ptr)
{

  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  return (group_ptr->SEFLAG & (uint32_t)VADC_G_SEFLAG_SEV0_Msk);
}

/**
 * @param group_ptr Constant pointer to the VADC group
 * @param sr     The service request line (Common SR lines, Group specific SR lines)
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Connects the event to the SR line of VADC.<BR>\n
 * This API will connect a Service Request line(SR) to a queue request source event. Hence to get a interrupt on this
 * Service request line one has to enable the required NVIC node. A call to this API would configure the register
 * bit field GxSEVNP.SEVNP0.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
void Cy_VADC_GROUP_QueueSetReqSrcEventInterruptNode(Cy_VADC_GROUP_t *const group_ptr, const Cy_VADC_SR_t sr);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables the trigger for queue request source.<BR>\n
 * By using this API the trigger will be activated for the queue request source. The trigger signal and trigger
 * edge will be selected from the QCTRL register. The Selection of a input will be done by
 * Cy_VADC_GROUP_QueueSelectTrigger(). A call to this API would configure the register bit field GxQMR0.ENTR
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueSelectTrigger()<BR> Cy_VADC_GROUP_QueueDisableExternalTrigger()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_QueueEnableExternalTrigger(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

  group_ptr->QMR0 |= (uint32_t)VADC_G_QMR0_ENTR_Msk;
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disables the trigger for queue request source.<BR>
 * By using this API the trigger will be deactivated for the queue request source.
 * This will just deactivate the H/W trigger for the queue request source. If any configuration was done
 * to select the trigger input in GxQCTRL0 will be not be effected.  A call to this API would configure the
 * register bit field GxQMR0.ENTR
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_QueueEnableExternalTrigger()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_QueueDisableExternalTrigger(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));

  group_ptr->QMR0 &= ~((uint32_t)VADC_G_QMR0_ENTR_Msk);
}
#endif

#if (CY_VADC_GROUP_AVAILABLE == 1U)
/**
 * @param group_ptr  Constant pointer to the VADC group
 * @param ch_num  The channel being initialized
 *                <BR>Range: [0x0 to 0x7]
 * @param config  Pointer to initialization data
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Initializes the ADC channel for conversion.<BR>\n
 * This API will do the channel related initializations. This includes configuration of the CHCTR settings
 * and boundary flag settings. This must be called in the application in order to enable the conversion of
 * a channel. After a request source has been initialized this API has to be called for each channel that
 * has to be converted.  A call to this API would configure the registers GxCHCTR GxBFL GxALIAS GxCHASS
 * GxBFLC(depending on device) in order to configure the channel.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
void Cy_VADC_GROUP_ChannelInit(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_num,
                                const Cy_VADC_CHANNEL_CONFIG_t *config);

/**
 * @param group_ptr    Constant pointer to the VADC group
 * @param src_ch_num   Channel which will be converted by \b alias_ch_num, when called by the request source.
 *                     <BR>Range:[0x0 to 0x7]
 * @param alias_ch_num This is the alias channel (Ch-0 or Ch-1)
 *                     <BR>Range:[0x0, 0x1]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Sets the Alias channel(\b alias_ch_num) to convert from the source channel(\b src_ch_num).<BR>\n
 * When a alias configuration takes place the request source(queue/scan/background) will not call channel \b src_ch_num.
 * The Request sources will call the channel \b alias_ch_num , this would invoke the conversion of
 * the pin associated with \b src_ch_num. The configuration of the alias channel (\b alias_ch_num) will be used
 * for the conversion.\n
 * When an alias channel (Ch-0 or Ch-1) receives a trigger, it converts the aliased channel (\b src_ch_num).
 * The properties of Ch-0 or Ch-1 (as indicated in \b alias_ch_num ) apply when \b src_ch_num is converted.
 *  A call to this API would configure the register GxALIAS.
 *
 * \par<b>Note:</b><br>
 * Alias Channel (\b alias_ch_num) and the source channel (\b src_ch_num) cannot be the same.
 * If they are, that alias feature is not used for the conversion. In order to Reset the alias
 * feature that was previously selected this method can be used.
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_GetChannelAlias().
 */
__STATIC_INLINE void Cy_VADC_GROUP_SetChannelAlias(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t src_ch_num,
    const uint32_t alias_ch_num)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((alias_ch_num == 0) || (alias_ch_num == 1U)));
  CY_ASSERT(((src_ch_num < 8U)));

  group_ptr->ALIAS = (group_ptr->ALIAS & (uint32_t)~(VADC_G_ALIAS_ALIAS0_Msk << (VADC_G_ALIAS_ALIAS1_Pos * src_ch_num))) |
                     (alias_ch_num << (VADC_G_ALIAS_ALIAS1_Pos * src_ch_num));
}

/**
 * @param group_ptr    Constant pointer to the VADC group
 * @param alias_ch_num This is the alias channel (Ch-0 or Ch-1)
 *                     <BR>Range:[0x0, 0x1]
 * @return
 *    uint8_t Channel which will be converted by \b alias_ch_num, when called by the request source.
 *
 * \par<b>Description:</b><br>
 * Gets the channel to convert when a conversion on alias channel(\b alias_ch_num) is requested.<BR>\n
 *
 * \par<b>Related APIs:</b><br>
 *  Cy_VADC_GROUP_SetChannelAlias().
 */
__STATIC_INLINE uint8_t Cy_VADC_GROUP_GetChannelAlias(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t alias_ch_num)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((alias_ch_num == 0) || (alias_ch_num == 1U)));

  return ((group_ptr->ALIAS >> (VADC_G_ALIAS_ALIAS1_Pos * alias_ch_num)) & VADC_G_ALIAS_ALIAS0_Msk);
}

/**
 * @param group_ptr    Constant pointer to the VADC group
 * @param ch_num  Channel whose input was converted
 *                <BR>Range: [0x0 to 0x7]
 * @return
 *   bool  Returns true if there was violation w.r.t the specified boundaries.
 *
 * \par<b>Description:</b><br>
 * Determines if the result of the channel confines with the specified boundaries.<BR>\n
 * An application may not necessarily always need to know the exact value of the converted result, but merely
 * an indication if the generated result is within stipulated boundaries. Generation of Channel event can be subject
 * to channel event generation criteria (Generate always, Never generate, Generate if result is out of bounds,
 * Generate if result is within bounds). When interrupts are not enabled, this API can be used to determine the
 * nature of the result.  A call to this API would access the registers GxCHCTR and GxCEFLAG in order to determine
 * if a violation has occured.
 *
 * \par<b>Related APIs:</b><br>
 * None
 */
bool Cy_VADC_GROUP_ChannelIsResultOutOfBounds(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_num);

/**
 * @param group_ptr    Constant pointer to the VADC group
 * @param ch_num  Channel whose input is to be converted
 *                <BR>Range: [0x0 to 0x7]
 * @param ref     Reference voltage
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Selects the reference voltage for conversion.<BR>\n
 * An internal voltage reference (VARef) or an external voltage reference fed to Ch-0 can serve as a voltage reference
 * for conversions.  A call to this API would configure the register bit field GxCHCTR.REFSEL.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
void Cy_VADC_GROUP_ChannelSetInputReference(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_num,
    const Cy_VADC_CHANNEL_REF_t ref);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num   Channel whose i/p is to be converted
 *                <BR>Range: [0x0 to 0x7]
 * @param result_reg_num  Result Register associated with this channel
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Selects the target result register.<BR>\n
 * There are upto 16 result registers which a channel can choose from to store the results of conversion.
 * This selects only the group related result registers. A call to this API would configure the register
 * bit field GxCHCTR.RESREG.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
void Cy_VADC_GROUP_ChannelSetResultRegister(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t ch_num,
    const uint32_t result_reg_num);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num   Channel whose conversion class is to be configured
 *                <BR>Range: [0x0 to 0x7]
 * @param conversion_class  conversion property to be associated with this channel
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Selects the conversion class registers.<BR>\n
 * It configures the channel to have a particular conversion class properties like sampling
 * time and resolution.  A call to this API would configure the register
 * bit field GxCHCTR.ICLSEL.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_ChannelGetInputClass().
 */
void Cy_VADC_GROUP_ChannelSetIclass(Cy_VADC_GROUP_t *const group_ptr,
                                     const uint32_t ch_num,
                                     const Cy_VADC_CHANNEL_CONV_t conversion_class);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num   Channel whose result alignment is to be returned
 *                <BR>Range: [0x0 to 0x7]
 * @return
 *    CY_VADC_RESULT_ALIGN_LEFT if the result are aligned to the left
 *    CY_VADC_RESULT_ALIGN_RIGHT if the result are aligned to the right
 *
 * \par<b>Description:</b><br>
 * Returns the channel result alignment.<BR>\n
 * The results are aligned either to the left or to the right. A left aligned 10bit resolution has its LSB
 * at bit2 where as a left aligned 8bit resolution starts at bit4. A call to this API would return the currently
 * configured alignment value.
 * A call to this API would read the register bit field GxCHCTR.RESPOS.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
__STATIC_INLINE Cy_VADC_RESULT_ALIGN_t Cy_VADC_GROUP_ChannelGetResultAlignment(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t ch_num)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((ch_num) < CY_VADC_NUM_CHANNELS_PER_GROUP));

  return ((Cy_VADC_RESULT_ALIGN_t)((group_ptr->CHCTR[ch_num] & (uint32_t)VADC_G_CHCTR_RESPOS_Msk) >>
                                    (uint32_t)VADC_G_CHCTR_RESPOS_Pos) );
}


/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num   Channel whose result alignment is to be returned
 *                <BR>Range: [0x0 to 0x7]
 * @return
 *    Cy_VADC_CHANNEL_CONV_t Returns the configured input class for the \b ch_num
 *
 * \par<b>Description:</b><br>
 * Returns the channel's input class for conversion for the required channel.<BR>\n
 * The sampling time and resolution can be taken from any of the 4 possible Input class registers.
 * This API would return the input class register that is taken up by \b ch_num for conversion.
 * A call to this API would read the register bit field GxCHCTR.RESPOS.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_ChannelSetIclass().
 */
__STATIC_INLINE Cy_VADC_CHANNEL_CONV_t Cy_VADC_GROUP_ChannelGetInputClass(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t ch_num)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((ch_num) < CY_VADC_NUM_CHANNELS_PER_GROUP));

  return ((Cy_VADC_CHANNEL_CONV_t)((group_ptr->CHCTR[ch_num] & (uint32_t)VADC_G_CHCTR_ICLSEL_Msk) >>
                                    (uint32_t)VADC_G_CHCTR_ICLSEL_Pos) );
}

/**
 *
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num   Channel whose associated result register is to be found
 *                <BR>Range: [0x0 to 0x7]
 * @return
 *  uint8_t  returns the Group result register to which it is linked to.
 *           <BR>Range: [0x0 to 0xF]
 *
 * \par<b>Description:</b><br>
 *  Returns the result register associated with this channel.<br>\n
 *  There are upto 16 result registers which a channel can choose from to store the results of conversion.
 *  This returns only the group related result registers.  A call to this API would access the register
 * bit field GxCHCTR.RESREG.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
uint8_t Cy_VADC_GROUP_ChannelGetResultRegister(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_num);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num   Channel whose channel event is to be asserted
 *                <BR>Range: [0x0 to 0x7]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Manually asserts a Channel event.<BR>\n
 * It is merely the channel event which is asserted. For this asserted event to lead to an interrupt, it must
 * have been bound to an SR and that SR must have been enabled. It can potentially lead to an interrupt if the
 * SR line is connected to an NVIC node.  A call to this API would configure the register bit fields of GxCEFLAG.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_ChannelClearEvent().
 */
void Cy_VADC_GROUP_ChannelTriggerEvent(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_num);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *      uint32_t returns the asserted channel events
 *
 * \par<b>Description:</b><br>
 * Returns the Channel event flag register.<BR>\n
 * The return is merely the channel events which are asserted.
 * A call to this API would read the register bit fields of GxCEFLAG.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_ChannelClearEvent().
 */
__STATIC_INLINE uint32_t Cy_VADC_GROUP_ChannelGetAssertedEvents(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  return (group_ptr->CEFLAG);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num   Channel whose channel event is to be acknowledged
 *                <BR>Range: [0x0 to 0x7]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Acknowledges a Channel event.<BR>\n
 * When a channel event is raised after the conversion of that channel, it has to be cleared. This API would clear
 * the Channel event of a particular channel if it has occurred.  A call to this API would configure the register
 * bit fields of GxCEFCLR.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_ChannelTriggerEvent().
 */
__STATIC_INLINE void Cy_VADC_GROUP_ChannelClearEvent(Cy_VADC_GROUP_t *const group_ptr, const uint32_t ch_num)
{

  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((ch_num) < CY_VADC_NUM_CHANNELS_PER_GROUP));
  group_ptr->CEFCLR = (uint32_t)((uint32_t)1 << ch_num);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num   Channel whose channel event is to be connected to a service request line
 *                <BR>Range: [0x0 to 0x7]
 * @param sr       The service request line to which the channel event is to be connected
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Binds a channel event to a requested Service Request line.<BR>\n
 * The channel event is connected to a service request line. For an event to result in an interrupt, this service
 * request line must be enabled in VADC and the NVIC node which this service request line is connected to must have
 * interrupt generation enabled.  A call to this API would configure the register bit fields of GxCEVNP0.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_ChannelTriggerEvent()<BR> Cy_VADC_GROUP_ChannelClearEvent()
 */
void Cy_VADC_GROUP_ChannelSetEventInterruptNode(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t ch_num,
    const Cy_VADC_SR_t sr);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num   Channel whose channel event is being configured
 *                <BR>Range: [0x0 to 0x7]
 * @param criteria The condition under which the channel may assert its channel event
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 *  Defines the conditions under which a channel may assert its channel event.<BR>\n
 * The channel event can be generated under the following conditions - Always, Never, Result Out of bounds and Result
 * inside the boundaries.  A call to this API would configure the register bit field GxCHCTR.CHEVMODE.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_ChannelTriggerEvent()<BR> Cy_VADC_GROUP_ChannelClearEvent()<BR>
 * Cy_VADC_GROUP_ChannelSetEventInterruptNode()<BR>
 */
void Cy_VADC_GROUP_ChannelTriggerEventGenCriteria(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t ch_num,
    const Cy_VADC_CHANNEL_EVGEN_t criteria);


/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param ch_num   Channel whose channel event is being configured
 *                <BR>Range: [0x0 to 0x7]
 * @param boundary_sel Select the upper/lower boundary configuration .
 * @param selection The boundary value selected for \b boundary_sel.
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Configure the boundary selection for the given channel<BR>\n
 * The channel event can be generated under the following conditions - Always, Never, Result Out of bounds and Result
 * inside the boundaries. The boundary values to which results are compared can be selected from several sources.
 * A call to this API would configure the register bit field GxCHCTR.BNDSELL or GxCHCTR.BNDSELU  .
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
void  Cy_VADC_GROUP_ChannelSetBoundarySelection(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t ch_num,
    Cy_VADC_BOUNDARY_SELECT_t boundary_sel,
    Cy_VADC_CHANNEL_BOUNDARY_t selection);

/**
 * @param group_ptr   Constant pointer to the VADC group
 * @param res_reg_num  Result register which is intended to be initialized
 *                     <BR>Range: [0x0 to 0xF]
 * @param config       Pointer to initialization data
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Initializes a Group Result Register.<BR>
 * Various options needed for the working of the result result will be configured with this API.
 * This would determine the result handling of the group registers. This API must be called after
 * the channel Init (Cy_VADC_GROUP_ChannelInit())to initialize the result register that is selected for the channel.
 * This API would also determine if the result register that is being configured has to a part of a FIFO buffer.
 * In this API one can also configure the various result handling options line FIR/IIR filters and it order.
 * Also configures the Data reduction to accumulate 2/3/4 results need to be done. This API will also configure
 * the result event generation.  A call to this API would configure the register GxRCR with the \b config .
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_AddResultToFifo()<BR> Cy_VADC_GROUP_EnableResultEvent()<br> Cy_VADC_GROUP_DisableResultEvent()<BR>
 */
__STATIC_INLINE void Cy_VADC_GROUP_ResultInit(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t res_reg_num,
    const Cy_VADC_RESULT_CONFIG_t *config)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->RCR[res_reg_num] = config->g_rcr;

}


/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Register which is required to be a part of results FIFO
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 *  Adds result register to Result FIFO.<BR>\n
 * Sometimes, the rate of consumption of results by application software may not match the rate at which the
 * results are produced. A Result FIFO thus helps a slow consumer to read out results without loss of data.
 * When a result register is added to fifo, it is in fact chained to its higher numbered neighbor. For example, if
 * Result Register-5 is to be added to FIFO, it gets chained to Result Register-6. Results are written to Register-6
 * while the same can be read out of Register-5 leisurely by software.
 * A call to this API would configure the register bit field GxRCR.FEN.
 *
 * \par<b>Note:</b><br>
 * The FIFO is always read by the software with the lowest numbered result register.
 * The hardware will write the results from the highest numbered result register.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
void Cy_VADC_GROUP_AddResultToFifo(Cy_VADC_GROUP_t *const group_ptr, const uint32_t res_reg);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Result Register for which event generation is to be enabled
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Enables result event generation.<BR>\n
 * Once the results of conversion are available, the result event (which is being enabled in this function)
 * if connected to a service request line(Group or Shared service request) can lead to an interrupt. It is therefore
 * not only necessary to enable the event, but also to connect it to a service request line. The
 * service request generation capability must also be enabled and so should the corresponding NVIC node.
 * A call to this API would configure the register bit field GxRCR.SRGEN.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_SetResultInterruptNode().
 */
__STATIC_INLINE void Cy_VADC_GROUP_EnableResultEvent(Cy_VADC_GROUP_t *const group_ptr, const uint32_t res_reg)
{

  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((res_reg) < CY_VADC_NUM_RESULT_REGISTERS));
  group_ptr->RCR[res_reg] |= (uint32_t)VADC_G_RCR_SRGEN_Msk;
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Result Register for which event generation is to be disabled
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Disable result event generation.<BR>\n
 * This would just disable the event. It would not alter anything w.r.t the SR line if it was configured.
 * A call to this API would configure the register bit field GxRCR.SRGEN.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_EnableResultEvent().
 */
__STATIC_INLINE void Cy_VADC_GROUP_DisableResultEvent(Cy_VADC_GROUP_t *const group_ptr, const uint32_t res_reg)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((res_reg) < CY_VADC_NUM_RESULT_REGISTERS));
  group_ptr->RCR[res_reg] &= ~((uint32_t)VADC_G_RCR_SRGEN_Msk);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Result Register from which the result of conversion is to be read out
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 *    uint32_t  returns the complete result register GxRESy.
 *
 * \par<b>Description:</b><br>
 * Returns the result register completely (result of conversion as well as other info).<BR>\n
 * The Result register will have information regarding the channel that is requesting the conversion,
 * if the result is valid, if the fast compare bit, Data Reduction Counter, and the request source information.
 * All these information will be returned back. And if the user is polling for the result he can use the
 * result if the valid bit is set. A call to this API would return the complete register GxRES.
 *
 * @note You can cast the return to a varible of type Cy_VADC_DETAILED_RESULT_t to easily access the register bit fields
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_GetResult().
 */
__STATIC_INLINE uint32_t Cy_VADC_GROUP_GetDetailedResult(Cy_VADC_GROUP_t *const group_ptr, const uint32_t res_reg)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((res_reg) < CY_VADC_NUM_RESULT_REGISTERS));
  return (group_ptr->RES[res_reg]);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Result Register from which the result of conversion is to be read out
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 *    Cy_VADC_RESULT_SIZE_t Result register values.
 *                <BR>Range:[ 0x0 to 0xFFF] (Result of single conversion. Accumulated results not considered for range)
 *
 * \par<b>Description:</b><br>
 * Returns the result of the conversion.<BR>\n
 * This API will only return the result of the conversion and will strip out the other information that is present
 * in the result register. A call to this API would access the register bit field GxRES.RESULT.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_GetDetailedResult().
 */
__STATIC_INLINE Cy_VADC_RESULT_SIZE_t Cy_VADC_GROUP_GetResult(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t res_reg)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((res_reg) < CY_VADC_NUM_RESULT_REGISTERS));

  return ((Cy_VADC_RESULT_SIZE_t)group_ptr->RES[res_reg]);
}

/**
 *
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Result Register for which the compare value is being set
 *                 <BR>Range: [0x0 to 0xF]
 * @param compare_val The compare value itself
 *                    <BR>Range: [0x0 to 0xFFF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Configures the compare value (relevant to the Fast Compare Mode).<BR>\n
 * A channel input can be converted and its value stored in its result register. Alternatively, the channel input can
 * be converted and compared against a compare value. This is the fast compare mode typically utilized by applications
 * that are not interested in absolute converted value of an analog input, but rather a binary decision on how the
 * input fares against a preset compare value. The channel should have had already chosen the correct ICLASS with
 * the fast compare mode enabled. \b compare_val would be the compare value on which FCM bit in the result
 * register will be set. The FCM bit will be set if the analog voltage is greater than the compare value.
 * A call to this API would configure the register bit field GxRES.RESULT.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_GetFastCompareResult().
 */
void Cy_VADC_GROUP_SetResultFastCompareValue(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t res_reg,
    const Cy_VADC_RESULT_SIZE_t compare_val);

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Result Register for which the compare value is being set
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 *  ::Cy_VADC_FAST_COMPARE_t If the input is greater or lower than the compare value returns the appropriate enum.
 *                            if the valid flag was not set then it would return CY_VADC_FAST_COMPARE_UNKNOWN.
 *
 * \par<b>Description:</b><br>
 * Determines the input is greater/lower than the compare value.<BR>\n
 * This API determines if the input is greater/lower than the preset compare value.
 * A call to this API would access the register bit field GxRES.FCM.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_SetResultFastCompareValue().
 */
Cy_VADC_FAST_COMPARE_t Cy_VADC_GROUP_GetFastCompareResult(Cy_VADC_GROUP_t *const group_ptr, const uint32_t res_reg);

/**
 *
 * @param group_ptr       Constant pointer to the VADC group
 * @param subtraction_val 12 bit subtraction value
 *                        <BR>Range: [0x0 to 0xFFF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Configures the subtraction value (relevant to the Difference Mode).<BR>\n
 * A channel input can be converted and its value stored in its result register. Alternatively, the channel input can
 * be converted and subtracted with the value stored in GxRES[0]. This Difference Mode typically utilized by
 * applications that are not interested in absolute converted value of an analog input, but rather a difference of
 * converted values. Subtraction value will always be present in the GxRES[0] and thus this API would configure
 * that register.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
void Cy_VADC_GROUP_SetResultSubtractionValue(Cy_VADC_GROUP_t *const group_ptr,
    const uint16_t subtraction_val);
/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Result Register for which the result event is being asserted
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Manually asserts the result event.<BR>\n
 * The result event must necessarily be connected to a SR line. The SR in turn must have been enabled along with the
 * corresponding NVIC node. Only then will the assertion of RES event lead to an interrupt.
 * A call to this API would access the register bit fieldS OF GxREFLAG.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_ClearResultEvent().
 */
__STATIC_INLINE void Cy_VADC_GROUP_TriggerResultEvent(Cy_VADC_GROUP_t *const group_ptr, const uint32_t res_reg)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((res_reg) < CY_VADC_NUM_RESULT_REGISTERS));
  group_ptr->REFLAG = (uint32_t)((uint32_t)1 << res_reg);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *      uint32_t returns the asserted result events
 *
 * \par<b>Description:</b><br>
 * Returns the Result event flag register.<BR>\n
 * The return is merely the result events which are asserted.
 * A call to this API would read the register bit fields of GxREFLAG.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_TriggerResultEvent().
 */
__STATIC_INLINE uint32_t Cy_VADC_GROUP_GetAssertedResultEvents(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  return (group_ptr->REFLAG);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Result Register for which the result event is being acknowledged
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Acknowledges a Result event.<BR>\n
 * When a Result event is raised after the conversion of that associated channel has produced a result and
 * it has to be cleared. This API would clear the Channel event of a particular channel if it has occurred.
 * A call to this API would access the register bit fields of GxREFCLR.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_ChannelTriggerEvent().
 */
__STATIC_INLINE void Cy_VADC_GROUP_ClearResultEvent(Cy_VADC_GROUP_t *const group_ptr, const uint32_t res_reg)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((res_reg) < CY_VADC_NUM_RESULT_REGISTERS));
  group_ptr->REFCLR = (uint32_t)((uint32_t)1 << res_reg);
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Result Register for which the result event is being asserted
 *                 <BR>Range: [0x0 to 0xF]
 * @param sr       The SR line to which the result event must be connected
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Binds a result event to a requested Service Request line.<BR>\n
 * The result event is connected to a service request line. For an event to result in an interrupt, this service
 * request line must be enabled in VADC and the NVIC node which this service request line is connected to must have
 * interrupt generation enabled. A call to this API would access the registers GxREVNP0 GxREVNP1.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_TriggerResultEvent()<BR> Cy_VADC_GROUP_ClearResultEvent()
 */
void Cy_VADC_GROUP_SetResultInterruptNode(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t res_reg,
    const Cy_VADC_SR_t sr);

/**
 * @param group_ptr  Constant pointer to the VADC group
 * @param res_reg  Result Register which forms a part of FIFO
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 * uint32_t returns the Result register number which is the tail of the FIFO,\b res_reg is apart of this FIFO.
 *
 * \par<b>Description:</b><br>
 * Returns the the FIFO tail (register from where to read the results).<BR>\n
 * The analog converter writes to the head of the FIFO. It is the head of the FIFO which is bound to the channel.
 * Applications read the result from the tail of the FIFO. This API would return the result
 * register from where a user can call the API Cy_VADC_GROUP_GetResult() to read the result stored in the FIFO.
 * A call to this API would access the register bit field GxRCR.FEN.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_GetResultFifoHead()<BR>
 */
uint32_t Cy_VADC_GROUP_GetResultFifoTail(Cy_VADC_GROUP_t *const group_ptr, uint32_t res_reg);

/**
 * @param group_ptr  Constant pointer to the VADC group
 * @param res_reg  Result Register which forms a part of fifo
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 * uint32_t returns the Result register number which is the head of the FIFO,\b res_reg is apart of this FIFO.
 *
 * \par<b>Description:</b><br>
 * Returns the the FIFO head (register to which the results are written by H/W).<BR>\n
 * The analog converter writes to the head of the FIFO. It is the head of the FIFO which is bound to the channel.
 * Applications read the result from the tail of the FIFO. This API would just return the head of the FIFO
 * from where the results are being added to the FIFO.
 * A call to this API would access the register bit field GxRCR.FEN.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_GetResultFifoHead()<BR>
 */
uint32_t Cy_VADC_GROUP_GetResultFifoHead(Cy_VADC_GROUP_t *const group_ptr, const uint32_t res_reg);

/**
 *
 * @param group_ptr  Constant pointer to the VADC group
 * @param res_reg  Result Register in question
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 *  bool returns true if the \b res_reg is the FIFO head.
 *
 * \par<b>Description:</b><br>
 * Determines if the requested register is the head of a FIFO.<BR>\n
 * The analog converter writes to the head of the FIFO. It is the head of the FIFO which is bound to the channel.
 * Applications read the result from the tail of the FIFO.
 * A call to this API would access the register bit field GxRCR.FEN.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_GetResultFifoHead()<BR>
 */
bool Cy_VADC_GROUP_IsResultRegisterFifoHead(Cy_VADC_GROUP_t *const group_ptr, const uint32_t res_reg);

/**
 *
 * @param group_ptr Constant pointer to the VADC group
 * @param res_reg  Result Register number<BR>
 *                 <BR>Range: [0x0 to 0xF]
 * @return
 *  bool returns true if the \b res_reg is the FIFO member, else false.
 *
 * \par<b>Description:</b><br>
 * Determines whether the specified register is a FIFO member or not.<BR>\n
 * The analog converter writes to the head of the FIFO. It is the head of the FIFO which is bound to the channel.
 * Applications read the result from the tail of the FIFO.
 * A call to this API would access the register bit field GxRCR.FEN.
 *
 */
__STATIC_INLINE bool Cy_VADC_GROUP_IsResultRegisterInFifo(Cy_VADC_GROUP_t *const group_ptr,
    const uint32_t res_reg)
{

  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  CY_ASSERT(((res_reg) < CY_VADC_NUM_RESULT_REGISTERS));
  return ( (bool)(group_ptr->RCR[res_reg] & (uint32_t)VADC_G_RCR_FEN_Msk));
}

#if CY_VADC_RESULT_PRIORITY_AVAILABLE == 1U
/**
 * @param group_ptr     Constant pointer to the VADC group
 * @param res_reg  Result Registers which need to be set for priority conversions
 *                 Bit location 0..15 represents Result Register-0..15 respectively.
 *                 To add the result register as priority.
 *                 Passing a 0x0 will clear all the selected channels
 *                 <BR>Range: [0x0 to 0xFFFF]
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Prioritize a Result register for group conversions.<BR>\n
 * Applications that need to reserve certain result registers only for Queue and scan request sources should
 * use this API. A call to this API would access the register bit fields of GxRRASS.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_GetResultRegPriority().
 */
__STATIC_INLINE void Cy_VADC_GROUP_SetResultRegPriority(Cy_VADC_GROUP_t *const group_ptr, const uint32_t res_mask)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  group_ptr->RRASS = (uint32_t)res_mask;
}

/**
 * @param group_ptr     Constant pointer to the VADC group
 * @return
 *    None
 *
 * \par<b>Description:</b><br>
 * Get the priority of all Result register.<BR>\n
 * A call to this API would access the register bit fields of GxRRASS.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_VADC_GROUP_SetResultRegPriority().
 */
__STATIC_INLINE uint32_t Cy_VADC_GROUP_GetResultRegPriority(Cy_VADC_GROUP_t *const group_ptr)
{
  CY_ASSERT(CY_VADC_CHECK_GROUP_PTR(group_ptr));
  return (group_ptr->RRASS);
}
#endif
#endif

#ifdef __cplusplus
}
#endif

/**
 * @}
 */


#endif
