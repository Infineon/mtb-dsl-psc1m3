/**
 * @file cy_gpio.c
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

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include "cy_gpio.h"

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define PORT_HWSEL_Msk PORT0_HWSEL_HW0_Msk
#define PORT_PHCR_Msk             PORT0_PHCR0_PH0_Msk
#define PORT_PHCR_Size            PORT0_PHCR0_PH0_Msk
#define PORT_HWSEL_Msk            PORT0_HWSEL_HW0_Msk
#define CY_GPIO_MODE_OE           CY_GPIO_MODE_OUTPUT_PUSH_PULL

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

void Cy_GPIO_SetMode(Cy_GPIO_PORT_t *const port, const uint8_t pin, const Cy_GPIO_MODE_t mode)
{
  CY_ASSERT(CY_GPIO_CHECK_PORT(port));
  CY_ASSERT(Cy_GPIO_IsModeValid(mode));

  port->IOCR[(uint32_t)pin >> 2U] &= ~(uint32_t)((uint32_t)PORT_IOCR_PC_Msk << ((uint32_t)PORT_IOCR_PC_Size * ((uint32_t)pin & 0x3U)));
  port->IOCR[(uint32_t)pin >> 2U] |= (uint32_t)mode << ((uint32_t)PORT_IOCR_PC_Size * ((uint32_t)pin & 0x3U));
}

void Cy_GPIO_SetHardwareControl(Cy_GPIO_PORT_t *const port, const uint8_t pin, const Cy_GPIO_HWCTRL_t hwctrl)
{
  CY_ASSERT(CY_GPIO_CHECK_PORT(port));
  CY_ASSERT(CY_GPIO_CHECK_HWCTRL(hwctrl));

  port->HWSEL &= ~(uint32_t)((uint32_t)PORT_HWSEL_Msk << ((uint32_t)pin << 1U));
  port->HWSEL |= (uint32_t)hwctrl << ((uint32_t)pin << 1U);
}

void Cy_GPIO_Init(Cy_GPIO_PORT_t *const port, const uint8_t pin, const Cy_GPIO_CONFIG_t *const config)
{
  CY_ASSERT(CY_GPIO_CHECK_PORT(port));
  CY_ASSERT(Cy_GPIO_IsModeValid(config->mode));
  CY_ASSERT(CY_GPIO_CHECK_INPUT_HYSTERESIS(config->input_hysteresis));

  /* Switch to input */
  port->IOCR[pin >> 2U] &= ~(uint32_t)((uint32_t)PORT_IOCR_PC_Msk << (PORT_IOCR_PC_Size * (pin & 0x3U)));

  /* HW port control is disabled */
  port->HWSEL &= ~(uint32_t)((uint32_t)PORT_HWSEL_Msk << ((uint32_t)pin << 1U));

  /* Set input hysteresis */
  port->PHCR[(uint32_t)pin >> 3U] &= ~(uint32_t)((uint32_t)PORT_PHCR_Msk << ((uint32_t)PORT_PHCR_Size * ((uint32_t)pin & 0x7U)));
  port->PHCR[(uint32_t)pin >> 3U] |= (uint32_t)config->input_hysteresis << ((uint32_t)PORT_PHCR_Size * ((uint32_t)pin & 0x7U));

  /* Enable digital input */
  if (CY_GPIO_CHECK_ANALOG_PORT(port))
  {
    port->PDISC &= ~(uint32_t)((uint32_t)0x1U << pin);
  }

  if ((config->mode & CY_GPIO_MODE_OE) != 0)
  {
    /* If output is enabled */

    /* Set output level */
    port->OMR = (uint32_t)config->output_level << pin;
  }

  /* Set mode */
  port->IOCR[pin >> 2U] |= (uint32_t)config->mode << (PORT_IOCR_PC_Size * (pin & 0x3U));
}

void Cy_GPIO_SetInputHysteresis(Cy_GPIO_PORT_t *const port,
                                 const uint8_t pin,
                                 const Cy_GPIO_INPUT_HYSTERESIS_t hysteresis)
{
  CY_ASSERT(CY_GPIO_CHECK_PORT(port));
  CY_ASSERT(CY_GPIO_CHECK_INPUT_HYSTERESIS(hysteresis));

  port->PHCR[(uint32_t)pin >> 3U] &= ~(uint32_t)((uint32_t)PORT_PHCR_Msk << ((uint32_t)PORT_PHCR_Size * ((uint32_t)pin & 0x7U)));
  port->PHCR[(uint32_t)pin >> 3U] |= (uint32_t)hysteresis << ((uint32_t)PORT_PHCR_Size * ((uint32_t)pin & 0x7U));
}
