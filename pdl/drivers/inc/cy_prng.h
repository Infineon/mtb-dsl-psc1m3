
/**
 * @file cy_prng.h
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

#ifndef CY_PRNG_H
#define CY_PRNG_H

/**
 * @addtogroup PRNG
 * @brief Pseudo Random Number Generator (PRNG) driver for PSC1M3 microcontroller family
 *
 * The pseudo random bit generator (PRNG) provides random data with fast generation times.
 * PRNG has to be initialized by the user software before use. The initialization consists
 * of two basic phases: key-loading and warm-up.
 *
 * The PRNG low level driver provides functions to configure and initialize the PRNG hardware
 * peripheral.
 * @{
 */

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "cy_common.h"

#if defined (PRNG) || defined(CY_DOXYGEN)

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

/**
 * Byte mask value for random data block size
 */
#define CY_PRNG_RDBS_BYTE_READ_MASK (0x00FFU)

/*********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * PRNG key load operation modes
 */
typedef enum CY_PRNG_KEY_LOAD_OP_MODE
{
  CY_PRNG_STRM_MODE = 0U, /**< Streaming mode (default) */
  CY_PRNG_KLD_MODE  = 1U  /**< Loading mode */
} Cy_PRNG_KEY_LOAD_OP_MODE_t;

/**
 * PRNG data block size
 */
typedef enum CY_PRNG_DATA_BLOCK_SIZE
{
  CY_PRNG_RDBS_RESET = 0U, /**< Reset state (no random data block size defined) */
  CY_PRNG_RDBS_BYTE  = 1U, /**< BYTE (8-bit) */
  CY_PRNG_RDBS_WORD  = 2U  /**< WORD (16-bit) */
} Cy_PRNG_DATA_BLOCK_SIZE_t;

/**
 * PRNG driver initialization status
 */
typedef enum CY_PRNG_INIT_STATUS
{
  CY_PRNG_NOT_INITIALIZED = 0U, /**< Reset state or Non-initialized state (Same as CY_PRNG_RDBS_RESET) */
  CY_PRNG_INITIALIZED     = 1U  /**< Initialized state */
} Cy_PRNG_INIT_STATUS_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/

/**
 * Key words and data block size configuration values of PRNG <br>
 *
 * The structure presents a convenient way to set/obtain the key word and data block configuration
 * values of PRNG.
 * The Cy_PRNG_Init() can be used to populate the structure with the key word and data block
 * configuration values of the PRNG module.
 */
typedef struct CY_PRNG_INIT
{
  uint16_t key_words[5];                 /**< Keywords */
  Cy_PRNG_DATA_BLOCK_SIZE_t block_size; /**< Block size */
} Cy_PRNG_INIT_t;

/*********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param prng Pointer to a constant instance of ::Cy_PRNG_INIT_t, pointing to
 *             the initialization configuration.
 * @return Cy_PRNG_INIT_STATUS_t CY_PRNG_INITIALIZED if initialized,
 *                                CY_PRNG_NOT_INITIALIZED otherwise.
 *
 * \par<b>Description: </b><br>
 * Initialize the PRNG peripheral with the configured key words and block size <br>
 *
 * \par
 * The function configures block size for key loading mode, enables key loading mode,
 * loads key words (80 bits) and wait till RDV is set, enables the streaming mode and
 * waits for warmup phase. This function programmes the CTRL and WORD registers.
 */
Cy_PRNG_INIT_STATUS_t Cy_PRNG_Init(const Cy_PRNG_INIT_t *prng);



/**
 * @param block_size Block size of type ::Cy_PRNG_DATA_BLOCK_SIZE_t for read access
 * @return None
 *
 * \par<b>Description: </b><br>
 * Programming Random Block Size <br>
 *
 * \par
 * The function sets the random data block size as byte or word by programming CTRL.RDBS bitfield.
 * block_size = 0 for Reset state, block_size = 1 for 'byte' and block_size = 2 for 'word'.
 */
__STATIC_INLINE void Cy_PRNG_SetRandomDataBlockSize(Cy_PRNG_DATA_BLOCK_SIZE_t block_size)
{
  PRNG->CTRL = (uint16_t)((PRNG->CTRL & (uint32_t)~PRNG_CTRL_RDBS_Msk) |
                          ((uint32_t)block_size << (uint32_t)PRNG_CTRL_RDBS_Pos));
}

/**
 * @return None
 *
 * \par<b>Description: </b><br>
 * Checks the validity (CHK.RDV bit) of the generated random data <br>
 *
 * \par
 * The function checks the validity (CHK.RDV bit) of the generated random data.
 * In key loading mode, this value indicates if the next partial key word can be written
 * to PRNG_WORD or not.
 */
__STATIC_INLINE uint16_t Cy_PRNG_CheckValidStatus(void)
{
  return (PRNG->CHK & PRNG_CHK_RDV_Msk);
}

/**
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enables the PRNG key loading mode <br>
 *
 * \par
 * The function initializes the key loading by setting the bit CTRL.KLD. In this mode, Register WORD
 * acts as always as a 16 bit destination register. After the complete key has been loaded, the CTRL.KLD
 * must be set to '0' to prepare the following warmup phase.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_PRNG_EnableStreamingMode()
 */
__STATIC_INLINE void Cy_PRNG_EnableKeyLoadingMode(void)
{
  PRNG->CTRL |= (uint16_t)PRNG_CTRL_KLD_Msk;
}

/**
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enables the Streaming mode <br>
 *
 * \par
 * The function enables the streaming mode and disables the PRNG key loading mode by resetting the
 * CTRL.KLD bit.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_PRNG_EnableKeyLoadingMode()
 */
__STATIC_INLINE void Cy_PRNG_EnableStreamingMode(void)
{
  PRNG->CTRL &= (uint16_t)~PRNG_CTRL_KLD_Msk;
}

/**
 * @param key Key word to load into PRNG WORD register
 * @return None
 *
 * \par<b>Description: </b><br>
 * Loads a partial key word to the PRNG WORD register <br>
 *
 * \par
 * The function loads partial key word to WORD registr. These partial
 * words are sequentially written and loading a key word will take 16 clock
 * cycles. The CHK.RDV bit is set to '0' while loading is in progress. '1' indicates
 * that the next partial key word can be written to WORD register.
 */
__STATIC_INLINE void Cy_PRNG_LoadKeyWords(uint16_t key)
{
  PRNG->WORD = key;
}

/**
 * @param None
 * @return uint16_t Generated random number
 *
 * \par<b>Description: </b><br>
 * Gets the generated random number <br>
 *
 * \par
 * The function gives the generated random number by returning the content of WORD
 * register. Before reading the WORD register to get the generated random number it is
 * required to check the bit CHK.RDV is set which indicates that the next random data block
 * can be read from WORD register. After a word has been read the bit CHK.RDV is reset
 * by the hardware and generation of new random bits starts.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_PRNG_CheckValidStatus()
 */
__STATIC_INLINE uint16_t Cy_PRNG_GetPseudoRandomNumber(void)
{
  return PRNG->WORD;
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* #if defined (PRNG) */

#endif /* CY_PRNG_H */
