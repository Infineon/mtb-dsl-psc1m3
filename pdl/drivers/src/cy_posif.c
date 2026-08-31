/**
 * @file cy_posif.c
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
 ********************************************************************************************************************/
#include "cy_posif.h"

#if defined(POSIF0)
#include "cy_scu.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define Cy_POSIF_PCONF_INSEL_Msk        (0x3fUL << POSIF_PCONF_INSEL0_Pos) /*< Mask for input pins selection */
#define CY_POSIF_INSEL_MAX              (4U) /*< Maximum possible input selector */

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/
#if defined(POSIF1)
#define Cy_POSIF_IsPeripheralValid(peripheral) (((peripheral) == POSIF0) || ((peripheral) == POSIF1))
#else
#define Cy_POSIF_IsPeripheralValid(peripheral) ((peripheral) == POSIF0)
#endif
/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* API to enable the POSIF module */
void Cy_POSIF_Enable(Cy_POSIF_t *const peripheral)
{

  if (peripheral == POSIF0)
  {
#if defined(CY_IP_CLOCK_GATING)
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_POSIF0);
#endif
#if defined(CY_IP_PERIPHERAL_RESET)
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_POSIF0);
#endif
  }
#if defined(POSIF1)
  else if (peripheral == POSIF1)
  {
#if defined(CY_IP_CLOCK_GATING)
    Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_POSIF1);
#endif
#if defined(CY_IP_PERIPHERAL_RESET)
    Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_POSIF1);
#endif
  }
#endif
  else
  {
    CY_ASSERT(0);
  }
}

/* API to disable the POSIF module */
void Cy_POSIF_Disable(Cy_POSIF_t *const peripheral)
{
  if (peripheral == POSIF0)
  {
#if defined(CY_IP_PERIPHERAL_RESET)
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_POSIF0);
#endif
#if defined(CY_IP_CLOCK_GATING)
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_POSIF0);
#endif
  }
#if defined(POSIF1)
  else if (peripheral == POSIF1)
  {
#if defined(CY_IP_PERIPHERAL_RESET)
    Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_POSIF1);
#endif
#if defined(CY_IP_CLOCK_GATING)
    Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_POSIF1);
#endif
  }
#endif
  else
  {
    CY_ASSERT(0);
  }
}

/* API to initialize POSIF global resources */
void Cy_POSIF_Init(Cy_POSIF_t *const peripheral, const Cy_POSIF_CONFIG_t *const config)
{
  CY_ASSERT(Cy_POSIF_IsPeripheralValid(peripheral));
  CY_ASSERT((config != (Cy_POSIF_CONFIG_t *)NULL));

  /* Enable the POSIF module */
  Cy_POSIF_Enable(peripheral);

  /* Stop POSIF */
  Cy_POSIF_Stop(peripheral);

  /* Program the operational mode, input selectors and debounce filter */
  peripheral->PCONF = config->pconf;
}

/* API to initialize hall sensor interface */
Cy_POSIF_STATUS_t Cy_POSIF_HSC_Init(Cy_POSIF_t *const peripheral, const Cy_POSIF_HSC_CONFIG_t *const config)
{
  Cy_POSIF_STATUS_t retval;

  CY_ASSERT(Cy_POSIF_IsPeripheralValid(peripheral));
  CY_ASSERT((config != (Cy_POSIF_HSC_CONFIG_t *)NULL));

  if (CY_POSIF_MODE_HALL_SENSOR == (Cy_POSIF_MODE_t)((peripheral->PCONF) & (uint32_t)POSIF_PCONF_FSEL_Msk) )
  {
    peripheral->PCONF |= config->hall_config;
    retval = CY_POSIF_STATUS_OK;
  }
  else
  {
    retval = CY_POSIF_STATUS_ERROR;
  }
  return retval;
}

/* API to initialize quadrature decoder interface */
Cy_POSIF_STATUS_t Cy_POSIF_QD_Init(Cy_POSIF_t *const peripheral, const Cy_POSIF_QD_CONFIG_t *const config)
{
  uint8_t reg;
  Cy_POSIF_STATUS_t retval;

  CY_ASSERT(Cy_POSIF_IsPeripheralValid(peripheral));
  CY_ASSERT((config != (Cy_POSIF_QD_CONFIG_t *)NULL));

  reg = (uint8_t)((peripheral->PCONF) & (uint32_t)POSIF_PCONF_FSEL_Msk);
  if (((uint32_t)CY_POSIF_MODE_QD == reg) || ((uint32_t)CY_POSIF_MODE_MCM_QD == reg))
  {
    /* Program the quadrature mode */
    peripheral->PCONF |= (uint32_t)(config->mode) << POSIF_PCONF_QDCM_Pos;
    peripheral->QDC = config->qdc;
    retval = CY_POSIF_STATUS_OK;
  }
  else
  {
    retval = CY_POSIF_STATUS_ERROR;
  }

  return retval;
}

/* API to initialize multi-channel mode.
 * This is used in Hall mode, standalone multi-channel mode and quadrature with multi-channel mode
 */
Cy_POSIF_STATUS_t Cy_POSIF_MCM_Init(Cy_POSIF_t *const peripheral, const Cy_POSIF_MCM_CONFIG_t *const config)
{
  Cy_POSIF_STATUS_t retval;

  CY_ASSERT(Cy_POSIF_IsPeripheralValid(peripheral));
  CY_ASSERT((config != (Cy_POSIF_MCM_CONFIG_t *)NULL));

  if ((Cy_POSIF_MODE_t)((peripheral->PCONF) & (uint32_t)POSIF_PCONF_FSEL_Msk) != CY_POSIF_MODE_QD)
  {
    peripheral->PCONF |= config->mcm_config;
    retval = CY_POSIF_STATUS_OK;
  }
  else
  {
    retval = CY_POSIF_STATUS_ERROR;
  }
  return retval;
}

/* API to configure input source */
void Cy_POSIF_SelectInputSource (Cy_POSIF_t *const peripheral, const Cy_POSIF_INPUT_PORT_t input0,
                                  const Cy_POSIF_INPUT_PORT_t input1, const Cy_POSIF_INPUT_PORT_t input2)
{
  uint32_t reg;
  CY_ASSERT(Cy_POSIF_IsPeripheralValid(peripheral));
  CY_ASSERT((input0 < CY_POSIF_INSEL_MAX));
  CY_ASSERT((input1 < CY_POSIF_INSEL_MAX));
  CY_ASSERT((input2 < CY_POSIF_INSEL_MAX));

  reg = (uint32_t)((((uint32_t)input0 << POSIF_PCONF_INSEL0_Pos) & (uint32_t)POSIF_PCONF_INSEL0_Msk) |
                   (((uint32_t)input1 << POSIF_PCONF_INSEL1_Pos) & (uint32_t)POSIF_PCONF_INSEL1_Msk) |
                   (((uint32_t)input2 << POSIF_PCONF_INSEL2_Pos) & (uint32_t)POSIF_PCONF_INSEL2_Msk));
  peripheral->PCONF = ((peripheral->PCONF & ~(uint32_t)Cy_POSIF_PCONF_INSEL_Msk) | reg);
}

/* API to select an interrupt node */
void Cy_POSIF_SetInterruptNode(Cy_POSIF_t *const peripheral, const Cy_POSIF_IRQ_EVENT_t event, const Cy_POSIF_SR_ID_t sr)
{
  uint32_t reg;

  CY_ASSERT(Cy_POSIF_IsPeripheralValid(peripheral));
  CY_ASSERT((event <= CY_POSIF_IRQ_EVENT_PCLK));
  CY_ASSERT((sr <= CY_POSIF_SR_ID_1));

  reg = peripheral->PFLGE;
  reg &= ~((uint32_t)1 << ((uint32_t)event + (uint32_t)POSIF_PFLGE_CHESEL_Pos));
  reg |= (uint32_t)sr << ((uint32_t)event + (uint32_t)POSIF_PFLGE_CHESEL_Pos);
  peripheral->PFLGE = reg;
}
#endif /* #if defined(POSIF0) */
