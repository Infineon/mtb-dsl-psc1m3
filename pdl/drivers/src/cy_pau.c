/**
 * @file cy_pau.c
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
 * @brief PAU driver for CY_FAMILY_PSC1 microcontroller family.
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include "cy_pau.h"

#if defined(PAU)

/**********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/

/*
 * Enable peripheral access
 */
void Cy_PAU_EnablePeripheralAccess(Cy_PAU_PERIPHERAL_t peripheral)
{
  uint32_t reg_num;

  reg_num = ((uint32_t)peripheral & 0xf0000000U) >> 28U;
  CY_PAU->PRIVDIS[reg_num] &= (uint32_t)~((uint32_t)peripheral & 0x0fffffffUL);
}

/*
 * Disable peripheral access
 */
void Cy_PAU_DisablePeripheralAccess(Cy_PAU_PERIPHERAL_t peripheral)
{
  uint32_t reg_num;

  reg_num = ((uint32_t)peripheral & 0xf0000000U) >> 28U;
  CY_PAU->PRIVDIS[reg_num] |= (uint32_t)((uint32_t)peripheral & 0x0fffffffUL);
}

/*
 * Check if peripheral access is enabled
 */
bool Cy_PAU_IsPeripheralAccessEnabled(Cy_PAU_PERIPHERAL_t peripheral)
{
  uint32_t reg_num;

  reg_num = ((uint32_t)peripheral & 0xf0000000U) >> 28U;
  return (bool)(CY_PAU->PRIVDIS[reg_num] & ((uint32_t)peripheral & 0x0fffffffUL));
}

/*
 * Check if peripheral is available
 */
bool Cy_PAU_IsPeripheralAvailable(Cy_PAU_PERIPHERAL_t peripheral)
{
  uint32_t reg_num;

  reg_num = ((uint32_t)peripheral & 0xf0000000U) >> 28U;
  return (bool)(CY_PAU->AVAIL[reg_num] & ((uint32_t)peripheral & 0x0fffffffUL));
}

#endif /* defined(PAU) */
