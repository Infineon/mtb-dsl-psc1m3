
/**
 * @file cy_prng.c
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
 */

#include "cy_prng.h"

#if defined (PRNG)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/

/*
 * Initializes the PRNG peripheral with the settings in the
 * initialization structure Cy_PRNG_INIT_t
 */
Cy_PRNG_INIT_STATUS_t Cy_PRNG_Init(const Cy_PRNG_INIT_t *prng)
{
  volatile uint16_t read_warm_up;
  uint16_t reg_val, iter;
  Cy_PRNG_INIT_STATUS_t status = CY_PRNG_INITIALIZED;

  CY_ASSERT((prng != (Cy_PRNG_INIT_t *)NULL));

  /* Configure block size for key loading mode */
  Cy_PRNG_SetRandomDataBlockSize(CY_PRNG_RDBS_WORD);

  /* Enable key loading mode */
  Cy_PRNG_EnableKeyLoadingMode();

  /* Load key words (80 bits) and wait till RDV is set */
  for (iter = (uint16_t)0UL; iter < (uint16_t)5UL; iter++)
  {
    Cy_PRNG_LoadKeyWords(prng->key_words[iter]);
    while (PRNG_CHK_RDV_Msk != Cy_PRNG_CheckValidStatus());
  }

  Cy_PRNG_EnableStreamingMode();

  /* Warm up phase: Read and discard 64 bits */
  read_warm_up = PRNG->WORD;
  read_warm_up = PRNG->WORD;
  read_warm_up = PRNG->WORD;
  reg_val      = PRNG->WORD;
  read_warm_up &= reg_val;

  /* Configure block size either byte (8 bit) or word (16 bit) */
  Cy_PRNG_SetRandomDataBlockSize(prng->block_size);

  /*
   * Checks for reset value for "random data block size". If reset,
   * PRNG is not initialized
   */
  if ((uint16_t)CY_PRNG_RDBS_RESET == (PRNG->CTRL & (uint16_t)PRNG_CTRL_RDBS_Msk))
  {
    status = CY_PRNG_NOT_INITIALIZED;
  }

  return status;
}

#endif /* #if defined (PRNG) */
