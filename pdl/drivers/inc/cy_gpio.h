/**
 * @file cy_gpio.h
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

#ifndef CY_GPIO_H
#define CY_GPIO_H

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "cy_common.h"
#include "cy_gpio_map.h"

/**
 * @addtogroup GPIO
 * @brief General Purpose Input Output (GPIO) driver for the PSC1M microcontroller family.
 *
 * GPIO driver provide a generic and very flexible software interface for all standard digital I/O port pins.
 * Each port slice has individual interfaces for the operation as General Purpose I/O and it further provides the
 * connectivity to the on-chip periphery and the control for the pad characteristics.
 *
 * The driver is divided into Input and Output mode.
 *
 * Input mode features:
 * -# Configuration structure Cy_GPIO_CONFIG_t and initialization function Cy_GPIO_Init()
 * -# Allows the selection of weak pull-up or pull-down device. Configuration structure Cy_GPIO_MODE_t and function Cy_GPIO_SetMode()
 * -# Allows the selection of input hysteresis. Cy_GPIO_SetInputHysteresis()
 *
 *
 * Output mode features:
 * -# Allows the selection of push pull/open drain and Alternate output. Configuration structure Cy_GPIO_MODE_t and function Cy_GPIO_SetMode()
 *
 * -# Allows the selection of initial output level. Configuration structure Cy_GPIO_OUTPUT_LEVEL_t and function Cy_GPIO_SetOutputLevel()
 *
 *@{
 */

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

#define PORT_IOCR_PC_Pos PORT0_IOCR0_PC0_Pos
#define PORT_IOCR_PC_Msk PORT0_IOCR0_PC0_Msk

#define PORT_IOCR_PC_Size 				(8U)


#define CY_GPIO_CHECK_OUTPUT_LEVEL(level) ((level == CY_GPIO_OUTPUT_LEVEL_LOW) || \
                                            (level == CY_GPIO_OUTPUT_LEVEL_HIGH))

#define CY_GPIO_CHECK_HWCTRL(hwctrl) ((hwctrl == CY_GPIO_HWCTRL_DISABLED) || \
                                       (hwctrl == CY_GPIO_HWCTRL_PERIPHERAL1) || \
                                       (hwctrl == CY_GPIO_HWCTRL_PERIPHERAL2))

#if defined(PORT0)
#define CY_GPIO_PORT0 ((Cy_GPIO_PORT_t *) PORT0_BASE)
#define CY_GPIO_CHECK_PORT0(port) (port == CY_GPIO_PORT0)
#else
#define CY_GPIO_CHECK_PORT0(port) 0
#endif

#if defined(PORT1)
#define CY_GPIO_PORT1 ((Cy_GPIO_PORT_t *) PORT1_BASE)
#define CY_GPIO_CHECK_PORT1(port) (port == CY_GPIO_PORT1)
#else
#define CY_GPIO_CHECK_PORT1(port) 0
#endif

#if defined(PORT2)
#define CY_GPIO_PORT2 ((Cy_GPIO_PORT_t *) PORT2_BASE)
#define CY_GPIO_CHECK_PORT2(port) (port == CY_GPIO_PORT2)
#else
#define CY_GPIO_CHECK_PORT2(port) 0
#endif

#if defined(PORT3)
#define CY_GPIO_PORT3 ((Cy_GPIO_PORT_t *) PORT3_BASE)
#define CY_GPIO_CHECK_PORT3(port) (port == CY_GPIO_PORT3)
#else
#define CY_GPIO_CHECK_PORT3(port) 0
#endif

#if defined(PORT4)
#define CY_GPIO_PORT4 ((Cy_GPIO_PORT_t *) PORT4_BASE)
#define CY_GPIO_CHECK_PORT4(port) (port == CY_GPIO_PORT4)
#else
#define CY_GPIO_CHECK_PORT4(port) 0
#endif

#define CY_GPIO_CHECK_PORT(port) (CY_GPIO_CHECK_PORT0(port) || \
                                   CY_GPIO_CHECK_PORT1(port) || \
                                   CY_GPIO_CHECK_PORT2(port) || \
								   CY_GPIO_CHECK_PORT3(port) || \
								   CY_GPIO_CHECK_PORT4(port))

#define CY_GPIO_CHECK_OUTPUT_PORT(port) CY_GPIO_CHECK_PORT(port)

#define CY_GPIO_CHECK_ANALOG_PORT(port) (port == CY_GPIO_PORT2)

#define CY_GPIO_CHECK_INPUT_HYSTERESIS(hysteresis) ((hysteresis == CY_GPIO_INPUT_HYSTERESIS_STANDARD) || \
                                                     (hysteresis == CY_GPIO_INPUT_HYSTERESIS_LARGE))

/**********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * Defines output level of a pin. Use type \a Cy_GPIO_OUTPUT_LEVEL_t for this enum.
 */
typedef enum CY_GPIO_OUTPUT_LEVEL
{
  CY_GPIO_OUTPUT_LEVEL_LOW  = 0x10000U, /**<  Reset bit */
  CY_GPIO_OUTPUT_LEVEL_HIGH = 0x1U, 	/**< Set bit  */
} Cy_GPIO_OUTPUT_LEVEL_t;

/**
 * Defines direct hardware control characteristics of the pin . Use type \a Cy_GPIO_HWCTRL_t for this enum.
 */
typedef enum CY_GPIO_HWCTRL
{
  CY_GPIO_HWCTRL_DISABLED     = 0x0U, /**<  Software control only */
  CY_GPIO_HWCTRL_PERIPHERAL1  = 0x1U, /**<  HWI0/HWO0 control path can override the software configuration */
  CY_GPIO_HWCTRL_PERIPHERAL2  = 0x2U  /**<  HWI1/HWO1 control path can override the software configuration */
} Cy_GPIO_HWCTRL_t;

/**
 * Defines the direction and characteristics of a pin. Use type \a Cy_GPIO_MODE_t for this enum. For the operation
 * with alternate functions, the port pins are directly connected to input or output functions of the on-chip periphery.
 */
typedef enum CY_GPIO_MODE
{
  CY_GPIO_MODE_INPUT_TRISTATE = 0x0UL << PORT_IOCR_PC_Pos,           /**< No internal pull device active */
  CY_GPIO_MODE_INPUT_PULL_DOWN = 0x1UL << PORT_IOCR_PC_Pos,          /**< Internal pull-down device active */
  CY_GPIO_MODE_INPUT_PULL_UP = 0x2UL << PORT_IOCR_PC_Pos,            /**< Internal pull-up device active */
  CY_GPIO_MODE_INPUT_SAMPLING = 0x3UL << PORT_IOCR_PC_Pos,           /**< No internal pull device active; Pn_OUTx continuously samples the input value */
  CY_GPIO_MODE_INPUT_INVERTED_TRISTATE = 0x4UL << PORT_IOCR_PC_Pos,  /**< Inverted no internal pull device active */
  CY_GPIO_MODE_INPUT_INVERTED_PULL_DOWN = 0x5UL << PORT_IOCR_PC_Pos, /**< Inverted internal pull-down device active */
  CY_GPIO_MODE_INPUT_INVERTED_PULL_UP = 0x6UL << PORT_IOCR_PC_Pos,   /**< Inverted internal pull-up device active */
  CY_GPIO_MODE_INPUT_INVERTED_SAMPLING = 0x7UL << PORT_IOCR_PC_Pos,  /**< Inverted no internal pull device active;Pn_OUTx continuously samples the input value */
  CY_GPIO_MODE_OUTPUT_PUSH_PULL = 0x80UL,		                      /**< Push-pull general-purpose output */
  CY_GPIO_MODE_OUTPUT_OPEN_DRAIN = 0xc0UL, 	                      /**< Open-drain general-purpose output */
  CY_GPIO_MODE_OUTPUT_ALT1 = 0x1UL << PORT_IOCR_PC_Pos,
  CY_GPIO_MODE_OUTPUT_ALT2 = 0x2UL << PORT_IOCR_PC_Pos,
  CY_GPIO_MODE_OUTPUT_ALT3 = 0x3UL << PORT_IOCR_PC_Pos,
  CY_GPIO_MODE_OUTPUT_ALT4 = 0x4UL << PORT_IOCR_PC_Pos,
  CY_GPIO_MODE_OUTPUT_ALT5 = 0x5UL << PORT_IOCR_PC_Pos,
  CY_GPIO_MODE_OUTPUT_ALT6 = 0x6UL << PORT_IOCR_PC_Pos,
  CY_GPIO_MODE_OUTPUT_ALT7 = 0x7UL << PORT_IOCR_PC_Pos,
  CY_GPIO_MODE_OUTPUT_ALT8 = 0x8UL << PORT_IOCR_PC_Pos,
  CY_GPIO_MODE_OUTPUT_ALT9 = 0x9UL << PORT_IOCR_PC_Pos,
  CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1 = CY_GPIO_MODE_OUTPUT_PUSH_PULL | CY_GPIO_MODE_OUTPUT_ALT1, 	/**<  Push-pull alternate output function 1 */
  CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2 = CY_GPIO_MODE_OUTPUT_PUSH_PULL | CY_GPIO_MODE_OUTPUT_ALT2, 	/**<  Push-pull alternate output function 2 */
  CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT3 = CY_GPIO_MODE_OUTPUT_PUSH_PULL | CY_GPIO_MODE_OUTPUT_ALT3, 	/**<  Push-pull alternate output function 3 */
  CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4 = CY_GPIO_MODE_OUTPUT_PUSH_PULL | CY_GPIO_MODE_OUTPUT_ALT4, 	/**<  Push-pull alternate output function 4 */
  CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT5 = CY_GPIO_MODE_OUTPUT_PUSH_PULL | CY_GPIO_MODE_OUTPUT_ALT5, 	/**<  Push-pull alternate output function 5 */
  CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6 = CY_GPIO_MODE_OUTPUT_PUSH_PULL | CY_GPIO_MODE_OUTPUT_ALT6, 	/**<  Push-pull alternate output function 6 */
  CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 = CY_GPIO_MODE_OUTPUT_PUSH_PULL | CY_GPIO_MODE_OUTPUT_ALT7, 	/**<  Push-pull alternate output function 7 */
  CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT8 = CY_GPIO_MODE_OUTPUT_PUSH_PULL | CY_GPIO_MODE_OUTPUT_ALT8, 	/**<  Push-pull alternate output function 8 */
  CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9 = CY_GPIO_MODE_OUTPUT_PUSH_PULL | CY_GPIO_MODE_OUTPUT_ALT9, 	/**<  Push-pull alternate output function 9 */
  CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT1 = CY_GPIO_MODE_OUTPUT_OPEN_DRAIN | CY_GPIO_MODE_OUTPUT_ALT1, 	/**<  Open drain alternate output function 1 */
  CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT2 = CY_GPIO_MODE_OUTPUT_OPEN_DRAIN | CY_GPIO_MODE_OUTPUT_ALT2, 	/**<  Open drain alternate output function 2 */
  CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT3 = CY_GPIO_MODE_OUTPUT_OPEN_DRAIN | CY_GPIO_MODE_OUTPUT_ALT3, 	/**<  Open drain alternate output function 3 */
  CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT4 = CY_GPIO_MODE_OUTPUT_OPEN_DRAIN | CY_GPIO_MODE_OUTPUT_ALT4, 	/**<  Open drain alternate output function 4 */
  CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT5 = CY_GPIO_MODE_OUTPUT_OPEN_DRAIN | CY_GPIO_MODE_OUTPUT_ALT5, 	/**<  Open drain alternate output function 5 */
  CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT6 = CY_GPIO_MODE_OUTPUT_OPEN_DRAIN | CY_GPIO_MODE_OUTPUT_ALT6, 	/**<  Open drain alternate output function 6 */
  CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT7 = CY_GPIO_MODE_OUTPUT_OPEN_DRAIN | CY_GPIO_MODE_OUTPUT_ALT7, 	/**<  Open drain alternate output function 7 */
  CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT8 = CY_GPIO_MODE_OUTPUT_OPEN_DRAIN | CY_GPIO_MODE_OUTPUT_ALT8, 	/**<  Open drain alternate output function 8 */
  CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT9 = CY_GPIO_MODE_OUTPUT_OPEN_DRAIN | CY_GPIO_MODE_OUTPUT_ALT9 	/**<  Open drain alternate output function 9 */
} Cy_GPIO_MODE_t;

/**
 * Configures input hysteresis mode of pin. Use type \a Cy_GPIO_INPUT_HYSTERESIS_t for this enum.
 * Selecting the appropriate pad hysteresis allows optimized pad oscillation behavior
 * for touch-sensing applications.
 */
typedef enum CY_GPIO_INPUT_HYSTERESIS
{
  CY_GPIO_INPUT_HYSTERESIS_STANDARD = 0x0U, /**< Standard hysteresis */
  CY_GPIO_INPUT_HYSTERESIS_LARGE    = 0x4U  /**< Large hysteresis */
} Cy_GPIO_INPUT_HYSTERESIS_t;

/**********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/
/**
 *  Structure points  port hardware registers. Use type Cy_GPIO_PORT_t for this structure.
 */
typedef struct CY_GPIO_PORT
{
  __IO uint32_t  OUT;				/**< The port output register determines the value of a GPIO pin when it is
										 selected by Pn_IOCRx as output */
  __O  uint32_t  OMR;				/**< The port output modification register contains control bits that make it
										 possible to individually set, reset, or toggle the logic state of a single port
									     line*/
  __I  uint32_t  RESERVED0[2];
  __IO uint32_t  IOCR[4];			/**< The port input/output control registers select the digital output and input
										 driver functionality and characteristics of a GPIO port pin */
  __I  uint32_t  RESERVED1;
  __I  uint32_t  IN;				/**< The logic level of a GPIO pin can be read via the read-only port input register
										 Pn_IN */
  __I  uint32_t  RESERVED2[6];
  __IO uint32_t  PHCR[2];			/**< Pad hysteresis control register */
  __I  uint32_t  RESERVED3[6];
  __IO uint32_t  PDISC;				/**< Pin Function Decision Control Register is to disable/enable the digital pad
										 structure in shared analog and digital ports*/
  __I  uint32_t  RESERVED4[3];
  __IO uint32_t  PPS;				/**< Pin Power Save Register */
  __IO uint32_t  HWSEL;				/**< Pin Hardware Select Register */
} Cy_GPIO_PORT_t;

/**
 *  Structure initializes port pin. Use type Cy_GPIO_CONFIG_t for this structure.
 */
typedef struct CY_GPIO_CONFIG
{
  Cy_GPIO_MODE_t mode;								/**< Defines the direction and characteristics of a pin */
  Cy_GPIO_INPUT_HYSTERESIS_t input_hysteresis;		/**< Defines input pad hysteresis of a pin */
  Cy_GPIO_OUTPUT_LEVEL_t output_level;				/**< Defines output level of a pin */
} Cy_GPIO_CONFIG_t;

/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param  port	  Constant pointer pointing to GPIO port, to access port registers like Pn_OUT,Pn_OMR,Pn_IOCR etc.
 * @param  pin	  Port pin number.
 * @param  config GPIO configuration data structure. Refer data structure @ref Cy_GPIO_CONFIG_t for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Initializes input / output mode settings like, pull up / pull down devices,hysteresis, push pull /open drain.
 * Also configures alternate function outputs and clears hardware port control for a selected \a port \a and \a pin.
 * \a config provides selected I/O settings. It configures hardware registers Pn_IOCR,Pn_OUT, Pn_OMR,Pn_PDISC and Pn_PHCR.
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 * \par<b>Note:</b><br>
 * This API is called in definition of DAVE_init by code generation and therefore should not be explicitly called
 * for the normal operation. Use other APIs only after DAVE_init is called successfully (returns DAVE_STATUS_SUCCESS).
 *
 *
 */


void Cy_GPIO_Init(Cy_GPIO_PORT_t *const port, const uint8_t pin, const Cy_GPIO_CONFIG_t *const config);

/**
 *
 * @param  port	Constant pointer pointing to GPIO port, to access hardware register Pn_IOCR.
 * @param  pin	Port pin number.
 * @param  mode input / output functionality selection. Refer @ref Cy_GPIO_MODE_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets digital input and output driver functionality and characteristics of a GPIO port pin. It configures hardware
 * registers Pn_IOCR. \a mode is initially configured during initialization in Cy_GPIO_Init(). Call this API to alter
 * the port direction functionality as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 */

void Cy_GPIO_SetMode(Cy_GPIO_PORT_t *const port, const uint8_t pin, const Cy_GPIO_MODE_t mode);


/**
 *
 * @param  port	 Constant pointer pointing to GPIO port, to access hardware register Pn_OMR.
 * @param  pin	 Port pin number.
 * @param  level output level selection. Refer @ref Cy_GPIO_OUTPUT_LEVEL_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set port pin output level to high or low.It configures hardware registers Pn_OMR.\a level is initially
 * configured during initialization in Cy_GPIO_Init(). Call this API to alter output level as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 *  Cy_GPIO_SetOutputHigh(), Cy_GPIO_SetOutputLow().
 *
 * \par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to output mode using Cy_GPIO_SetMode().
 *
 */


__STATIC_INLINE void Cy_GPIO_SetOutputLevel(Cy_GPIO_PORT_t *const port, const uint8_t pin, const Cy_GPIO_OUTPUT_LEVEL_t level)
{
  CY_ASSERT(CY_GPIO_CHECK_OUTPUT_PORT(port));
  CY_ASSERT(CY_GPIO_CHECK_OUTPUT_LEVEL(level));

  port->OMR = (uint32_t)level << pin;
}


/**
 * @param  port constant pointer pointing to GPIO port, to access hardware register Pn_OMR.
 * @param  pin	Port pin number.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *  Sets port pin output to high. It configures hardware registers Pn_OMR.
 *
 *  \par<b>Related APIs:</b><BR>
 *  Cy_GPIO_SetOutputLow()
 *
 * \par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to output mode using Cy_GPIO_SetMode().\n
 * Register Pn_OMR is virtual and does not contain any flip-flop. A read action delivers the value of 0.
 *
 */

__STATIC_INLINE void Cy_GPIO_SetOutputHigh(Cy_GPIO_PORT_t *const port, const uint8_t pin)
{
  CY_ASSERT(CY_GPIO_CHECK_OUTPUT_PORT(port));

  port->OMR = (uint32_t)0x1U << pin;
}

/**
 *
 * @param  port	constant pointer pointing to GPIO port, to access hardware register Pn_OMR.
 * @param  pin	port pin number.
 *
 * @return  None
 *
 *\par<b>Description:</b><br>
 * Sets port pin output to low. It configures hardware registers Pn_OMR.\n
 *
 * \par<b>Related APIs:</b><BR>>
 * Cy_GPIO_SetOutputHigh()
 *
 *\par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to output mode using Cy_GPIO_SetMode().
 * Register Pn_OMR is virtual and does not contain any flip-flop. A read action delivers the value of 0.\n
 *
 */

__STATIC_INLINE void Cy_GPIO_SetOutputLow(Cy_GPIO_PORT_t *const port, const uint8_t pin)
{
  CY_ASSERT(CY_GPIO_CHECK_OUTPUT_PORT(port));

  port->OMR = 0x10000U << pin;
}

/**
 *
 * @param port constant pointer pointing to GPIO port, to access hardware register Pn_OMR.
 * @param pin  port pin number.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures port pin output to Toggle. It configures hardware registers Pn_OMR.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_GPIO_SetOutputHigh(), Cy_GPIO_SetOutputLow().
 *
 * \par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to output mode using Cy_GPIO_SetMode(). Register Pn_OMR is virtual
 * and does not contain any flip-flop. A read action delivers the value of 0.
 *
 */

__STATIC_INLINE void Cy_GPIO_ToggleOutput(Cy_GPIO_PORT_t *const port, const uint8_t pin)
{
  CY_ASSERT(CY_GPIO_CHECK_OUTPUT_PORT(port));

  port->OMR = 0x10001U << pin;
}

/**
 *
 * @param port constant pointer pointing to GPIO port, to access hardware register Pn_IN.
 * @param pin  Port pin number.
 *
 * @return uint32_t pin logic level status.
 *
 *\par<b>Description:</b><br>
 * Reads the Pn_IN register and returns the current logical value at the GPIO pin.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 * \par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to input mode using Cy_GPIO_SetMode().
 *
 */

__STATIC_INLINE uint32_t Cy_GPIO_GetInput(Cy_GPIO_PORT_t *const port, const uint8_t pin)
{
  CY_ASSERT(CY_GPIO_CHECK_PORT(port));

  return (((port->IN) >> pin) & 0x1U);
}

/**
 * @param port constant pointer pointing to GPIO port, to access hardware register Pn_PPS.
 * @param pin  port pin number.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables pin power save mode and configures Pn_PPS register.This configuration is useful when the controller enters
 * Deep Sleep mode.Port pin enabled with power save mode option are set to a defined state and the input Schmitt-Trigger
 * as well as the output driver stage are switched off. By default port pin does not react to power save mode request.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_GPIO_DisablePowerSaveMode()
 *
 * <b>Note:</b><br>
 * Do not enable the Pin Power Save function for pins configured for Hardware Control (Pn_HWSEL.HWx != 00B). Doing so
 * may result in an undefined behavior of the pin when the device enters the Deep Sleep state.
 *
 */


__STATIC_INLINE void Cy_GPIO_EnablePowerSaveMode(Cy_GPIO_PORT_t *const port, const uint8_t pin)
{
  CY_ASSERT(CY_GPIO_CHECK_PORT(port));

  port->PPS |= (uint32_t)0x1U << pin;
}


/**
 *
 * @param port	constant pointer pointing to GPIO port, to access hardware register Pn_PPS.
 * @param pin	port pin number.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables pin power save mode and configures Pn_PPS register.This configuration is useful when the controller enters
 * Deep Sleep mode. This configuration enables input Schmitt-Trigger and output driver stage(if pin is enabled power
 * save mode previously). By default port \a pin does not react to power save mode request.
 *
 * \par<b>Related APIs:</b><BR>
 *  Cy_GPIO_EnablePowerSaveMode()
 *
 *\par<b>Note:</b><br>
 * Do not enable the Pin Power Save function for pins configured for Hardware Control (Pn_HWSEL.HWx != 00B). Doing so
 * may result in an undefined behavior of the pin when the device enters the Deep Sleep state.
 *
 */

__STATIC_INLINE void Cy_GPIO_DisablePowerSaveMode(Cy_GPIO_PORT_t *const port, const uint8_t pin)
{
  CY_ASSERT(CY_GPIO_CHECK_PORT(port));

  port->PPS &= ~(uint32_t)((uint32_t)0x1U << pin);
}


/**
 * @param port	 constant pointer pointing to GPIO port, to access hardware register Pn_HWSEL.
 * @param pin	 port pin number.
 * @param hwctrl direct hardware control selection. Refer @ref Cy_GPIO_HWCTRL_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Selects direct hard ware control and configures Pn_HWSEL register.This configuration is useful for the port pins
 * overlaid with peripheral functions for which the connected peripheral needs hardware control.
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 *\par<b>Note:</b><br>
 * Do not enable the Pin Power Save function for pins configured for Hardware Control (Pn_HWSEL.HWx != 00B).
 * Doing so may result in an undefined behavior of the pin when the device enters the Deep Sleep state.
 *
 */

void Cy_GPIO_SetHardwareControl(Cy_GPIO_PORT_t *const port, const uint8_t pin, const Cy_GPIO_HWCTRL_t hwctrl);


/**
 * @param port	constant pointer pointing to GPIO port, to access hardware register Pn_PDISC.
 * @param pin	port pin number.
 *
 * @return None
 *
 * \par<b>Related APIs:</b><BR>
 *  None
 *
 * \par<b>Description:</b><br>
 * Enable digital input path for analog pins and configures Pn_PDISC register.This configuration is applicable only for
 * analog port pins.
 *
 */
__STATIC_INLINE void Cy_GPIO_EnableDigitalInput(Cy_GPIO_PORT_t *const port, const uint8_t pin)
{
  CY_ASSERT(CY_GPIO_CHECK_ANALOG_PORT(port));

  port->PDISC &= ~(uint32_t)((uint32_t)0x1U << pin);
}


/**
 * @param  port	constant pointer pointing to GPIO port, to access hardware register Pn_PDISC.
 * @param  pin	port pin number.
 *
 * @return None
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 * \par<b>Description:</b><br>
 * Disable digital input path for analog pins and configures Pn_PDISC register.This configuration is applicable only
 * for analog port pins.
 *
 */

__STATIC_INLINE void Cy_GPIO_DisableDigitalInput(Cy_GPIO_PORT_t *const port, const uint8_t pin)
{
  CY_ASSERT(CY_GPIO_CHECK_ANALOG_PORT(port));

  port->PDISC |= (uint32_t)0x1U << pin;
}

__STATIC_INLINE bool Cy_GPIO_IsModeValid(Cy_GPIO_MODE_t mode)
{
  return ((mode == CY_GPIO_MODE_INPUT_TRISTATE) ||
          (mode == CY_GPIO_MODE_INPUT_PULL_DOWN) ||
          (mode == CY_GPIO_MODE_INPUT_PULL_UP) ||
          (mode == CY_GPIO_MODE_INPUT_SAMPLING) ||
          (mode == CY_GPIO_MODE_INPUT_INVERTED_TRISTATE) ||
          (mode == CY_GPIO_MODE_INPUT_INVERTED_PULL_DOWN) ||
          (mode == CY_GPIO_MODE_INPUT_INVERTED_PULL_UP) ||
          (mode == CY_GPIO_MODE_INPUT_INVERTED_SAMPLING) ||
          (mode == CY_GPIO_MODE_OUTPUT_PUSH_PULL) ||
          (mode == CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1) ||
          (mode == CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2) ||
          (mode == CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT3) ||
          (mode == CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4) ||
          (mode == CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT5) ||
          (mode == CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6) ||
          (mode == CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7) ||
          (mode == CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT8) ||
          (mode == CY_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9) ||
          (mode == CY_GPIO_MODE_OUTPUT_OPEN_DRAIN) ||
          (mode == CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT1) ||
          (mode == CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT2) ||
          (mode == CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT3) ||
          (mode == CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT4) ||
          (mode == CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT5) ||
          (mode == CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT6) ||
          (mode == CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT7) ||
          (mode == CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT8) ||
          (mode == CY_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT9)
         );
}


/**
 * @brief Sets pad hysteresis.
 * @param  port		  Constant pointer pointing to GPIO port, to access hardware register Pn_PHCR.
 * @param  pin		  Port pin number.
 * @param  hysteresis input hysteresis selection. Refer data structure @ref Cy_GPIO_INPUT_HYSTERESIS_t
 * 					  for details.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets port pin input hysteresis. It configures hardware registers Pn_PHCR.\a hysteresis is initially configured during
 * initialization in Cy_GPIO_Init(). Call this API to alter pad hysteresis as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * None
 *
 * \par<b>Note:</b><br>
 * Prior to this api, user has to configure port pin to input mode using Cy_GPIO_SetMode().
 *
 */

void Cy_GPIO_SetInputHysteresis(Cy_GPIO_PORT_t *const port,
                                 const uint8_t pin,
                                 const Cy_GPIO_INPUT_HYSTERESIS_t hysteresis);


#ifdef __cplusplus
}
#endif

/**
 * @} (end addtogroup GPIO)
 */

#endif /* CY_GPIO_H */
