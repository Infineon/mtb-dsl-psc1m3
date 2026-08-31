/**
 * @file cy_acmp.c
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
#include "cy_acmp.h"

/* If ACMP is available*/
#if defined (COMPARATOR)

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#define CY_ACMP_INSTANCE_1    (1U) /* Instance number for Slice-1 */

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* API to initialize an instance of ACMP module */
void Cy_ACMP_Init(Cy_ACMP_t *const peripheral, uint32_t instance, const Cy_ACMP_CONFIG_t *const config)
{

  CY_ASSERT((config != (Cy_ACMP_CONFIG_t *)NULL));
  CY_ASSERT(CY_ACMP_CHECK_MODULE_PTR(peripheral));
  CY_ASSERT(CY_ACMP_CHECK_INSTANCE(instance));

  /*
   * Initializes the comparator with configuration supplied. Low power node setting is retained during initialization.
   * All the instances passed are handled with low power setting, to avoid conditional check for ACMP0 instance.
   * This reduces the code size. No side effects, because this register bit field is empty for other instances.
   */
  peripheral->ANACMP[instance] = ((peripheral->ANACMP[instance] & (uint32_t)COMPARATOR_ANACMP0_CMP_LPWR_Msk)) |
                                 (uint32_t)config->anacmp;
}

/* API to select INP source */
void Cy_ACMP_SetInput(Cy_ACMP_t *const peripheral, uint32_t instance, Cy_ACMP_INP_SOURCE_t source)
{
  CY_ASSERT(CY_ACMP_CHECK_MODULE_PTR(peripheral));
  CY_ASSERT(((instance != CY_ACMP_INSTANCE_1) &&
             CY_ACMP_CHECK_INSTANCE(instance)));
  CY_ASSERT(((source == CY_ACMP_INP_SOURCE_STANDARD_PORT) ||
             (source == CY_ACMP_INP_SOURCE_ACMP1_INP_PORT)));

  /*
   * Three options of Input Setting are listed below
   * 1. The comparator inputs aren't connected to other comparator inputs
   * 2. Can program the comparators to connect ACMP0.INN to ACMP1.INP
   * 3. Can program the comparators to connect ACMP2.INP to ACMP1.INP
   * 4. Can program the comparators to connect ACMP3.INP to ACMP1.INP
   */
  peripheral->ANACMP[instance] = ((peripheral->ANACMP[instance] & (uint32_t)(~COMPARATOR_ANACMP0_ACMP0_SEL_Msk))) |
                                 (uint32_t)source;
}

#endif /* #ifdef ACMP_AVAILABLE */
