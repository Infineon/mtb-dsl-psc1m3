/**
 * @file cy_flash.c
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

#include "cy_flash.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
/* FLASH programming / erase options */
typedef enum FLASH_ACTION
{
  FLASH_ACTION_IDLE                        =  (uint32_t)0x00,
  FLASH_ACTION_ONESHOT_WRITE_VERIFY        = ((uint32_t)0x51 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_ONESHOT_WRITE               = ((uint32_t)0x91 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_CONTINUOUS_WRITE_VERIFY     = ((uint32_t)0x61 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_CONTINUOUS_WRITE            = ((uint32_t)0xa1 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_ONESHOT_PAGE_ERASE          = ((uint32_t)0x92 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_CONTINUOUS_PAGE_ERASE       = ((uint32_t)0xa2 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_ONESHOT_VERIFY_ONLY         = ((uint32_t)0xd0 << NVM_NVMPROG_ACTION_Pos),
  FLASH_ACTION_CONTINUOUS_VERIFY_ONLY      = ((uint32_t)0xe0 << NVM_NVMPROG_ACTION_Pos)
} FLASH_ACTION_t;


/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/*
 * This API shall clear the ECC and VERIFICATION error status.
 */
void Cy_FLASH_ClearStatus(void)
{
  NVM->NVMPROG |= (uint16_t)((uint16_t)NVM_NVMPROG_RSTVERR_Msk | (uint16_t)NVM_NVMPROG_RSTECC_Msk);
}

/*
 * This API shall return the status of NVM.
 */
uint32_t Cy_FLASH_GetStatus(void)
{
  return NVM->NVMSTATUS;
}

/*
 * This API shall enable the the flash interrupt event.
 */
void Cy_FLASH_EnableEvent(const uint32_t event_msk)
{
  NVM->NVMCONF |= (uint16_t)event_msk;
}

/*
 * This API shall disable the the flash interrupt event.
 */
void Cy_FLASH_DisableEvent(const uint32_t event_msk)
{
  NVM->NVMCONF &= (uint16_t)(~(uint16_t)event_msk);
}

int32_t Cy_FLASH_ErasePage(uint32_t *address)
{
  #if defined(__GNUC__)
  _Pragma("GCC diagnostic push")
  _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
  #endif
  return PSC1M3_NvmErasePage(address);
  #if defined(__GNUC__)
  _Pragma("GCC diagnostic pop")
  #endif
}

int32_t Cy_FLASH_ProgramVerifyPage(uint32_t *address, const uint32_t *data)
{
  #if defined(__GNUC__)
  _Pragma("GCC diagnostic push")
  _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
  #endif
  return PSC1M3_NvmProgVerify(data, address);
  #if defined(__GNUC__)
  _Pragma("GCC diagnostic pop")
  #endif
}

/* Write blocks of data into flash*/
void Cy_FLASH_WriteBlocks(uint32_t *address, const uint32_t *data, uint32_t num_blocks, bool verify)
{
  uint32_t word;
  uint32_t block;

  CY_ASSERT(((uint32_t)address & FLASH_BLOCK_ADDR_MASK) == 0U);

  /* Configure the continuous Write option command and reset the NVM error / verification status*/
  NVM->NVMPROG &= (uint16_t)(~(uint16_t)NVM_NVMPROG_ACTION_Msk);
  NVM->NVMPROG |= (uint16_t)(NVM_NVMPROG_RSTVERR_Msk | NVM_NVMPROG_RSTECC_Msk);

  if (verify == true)
  {
    NVM->NVMPROG |= (uint16_t)FLASH_ACTION_CONTINUOUS_WRITE_VERIFY;
  }
  else
  {
    NVM->NVMPROG |= (uint16_t)FLASH_ACTION_CONTINUOUS_WRITE;
  }

  for (block = 0U; block < num_blocks; ++block)
  {
    for (word = 0U; word < CY_FLASH_WORDS_PER_BLOCK; ++word)
    {
      *address = *data;
      data++;
      address++;
    }

    while (Cy_FLASH_IsBusy() == true)
    {
    }
  }

  /* Stop continuous write operation */
  NVM->NVMPROG &= (uint16_t)(~(uint16_t)NVM_NVMPROG_ACTION_Msk);
}

/* Erase flash pages */
int32_t Cy_FLASH_ErasePages(uint32_t *address, uint32_t num_pages)
{
  uint32_t page;

  CY_ASSERT(((uint32_t)address & FLASH_PAGE_ADDR_MASK) == 0U);

  int32_t status = NVM_E_FAIL;
  for (page = 0U; page < num_pages; ++page)
  {
    #if defined(__GNUC__)
    _Pragma("GCC diagnostic push")
    _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
    #endif
    status = PSC1M3_NvmErasePage(address);
    #if defined(__GNUC__)
    _Pragma("GCC diagnostic pop")
    #endif
    if (status != NVM_PASS)
    {
      return status;
    }

    /* Increment the page address for the next erase */
    address += CY_FLASH_WORDS_PER_PAGE;
  }

  return status;

}

/* Write multiple data blocks and verify the written data */
void Cy_FLASH_VerifyBlocks(uint32_t *address, const uint32_t *data, uint32_t num_blocks)
{
  uint32_t word;
  uint32_t block;

  CY_ASSERT(((uint32_t)address & FLASH_BLOCK_ADDR_MASK) == 0U);

  /* Configure the Continuous write with verify option command and reset the NVM error / verification status*/
  NVM->NVMPROG &= (uint16_t)~NVM_NVMPROG_ACTION_Msk;
  NVM->NVMPROG |= (uint16_t)((uint16_t)NVM_NVMPROG_RSTVERR_Msk |
                             (uint16_t)NVM_NVMPROG_RSTECC_Msk |
                             (uint16_t)FLASH_ACTION_CONTINUOUS_VERIFY_ONLY);

  for (block = 0U; block < num_blocks; ++block)
  {
    for (word = 0U; word < CY_FLASH_WORDS_PER_BLOCK; ++word)
    {
      *address = *data;
      data++;
      address++;
    }

    while (Cy_FLASH_IsBusy() == true)
    {
    }
  }

  /* Stop continuous verify operation */
  NVM->NVMPROG &= (uint16_t)(~(uint16_t)NVM_NVMPROG_ACTION_Msk);
}

/* Read data blocks from flash */
void Cy_FLASH_ReadBlocks(uint32_t *address, uint32_t *data, uint32_t num_blocks)
{
  uint32_t word;
  uint32_t block;

  CY_ASSERT(((uint32_t)address & FLASH_BLOCK_ADDR_MASK) == 0U);

  for (block = 0U; block < num_blocks; ++block)
  {
    for (word = 0U; word < CY_FLASH_WORDS_PER_BLOCK; ++word)
    {
      *data = *address;
      data++;
      address++;
    }
  }
}

/* Erase single sector */
int32_t Cy_FLASH_EraseSector(uint32_t *address)
{
  CY_ASSERT(((uint32_t)address & FLASH_SECTOR_ADDR_MASK) == 0U);
  return Cy_FLASH_ErasePages(address, CY_FLASH_PAGES_PER_SECTOR);
}

/* Program single page */
int32_t Cy_FLASH_ProgramPage(uint32_t *address, const uint32_t *data)
{
  return Cy_FLASH_ProgramVerifyPage(address, data);
}
