/**
 * @file cy_pau.h
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

#ifndef CY_PAU_H
#define CY_PAU_H

/**
 * @addtogroup PAU
 * @brief Peripheral Access Unit (PAU) driver for the PSC1M3 microcontroller family
 *
 * The Peripheral Access Unit (PAU) supports access control of memories and peripherals.
 * It allows user application to enable/disable the access to the registers of a peripheral.
 * It generates a HardFault exception when there is an access to a disabled or unassigned
 * address location. It also provides information on the availability of peripherals and
 * sizes of memories.
 *
 * The PAU low level driver provides functions to check the availability of peripherals
 * and to enable/disable peripheral access.
 * @{
 */

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "cy_common.h"

#if defined(PAU) || defined(CY_DOXYGEN)

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

/**
 * A convenient symbol for the PAU peripheral base address
 */
#define CY_PAU ((Cy_PAU_t *) PAU_BASE)

/*
 * This macro is used in the LLD for assertion checks (CY_ASSERT)
 */
#define CY_PAU_CHECK_MODULE_PTR(p) ((p) == CY_PAU)

/*********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * Status return values for PAU low level driver
 */
typedef enum CY_PAU_STATUS
{
  CY_PAU_STATUS_OK    = 0U, /**< Operation successful */
  CY_PAU_STATUS_BUSY  = 1U, /**< Busy with a previous request */
  CY_PAU_STATUS_ERROR = 2U  /**< Operation unsuccessful */
} Cy_PAU_STATUS_t;

/**
 * PAU peripheral select
 */
typedef enum CY_PAU_PERIPHERAL
{
  CY_PAU_PERIPHERAL_FLASH                = PAU_PRIVDIS0_PDIS2_Msk, /**< Flash SFRs Privilege Disable Flag */
  CY_PAU_PERIPHERAL_RAM_BLOCK1           = PAU_PRIVDIS0_PDIS5_Msk, /**< RAM Block 1 Privilege Disable Flag */
  CY_PAU_PERIPHERAL_RAM_BLOCK2           = PAU_PRIVDIS0_PDIS6_Msk, /**< RAM Block 2 Privilege Disable Flag */
  CY_PAU_PERIPHERAL_RAM_BLOCK3           = PAU_PRIVDIS0_PDIS7_Msk, /**< RAM Block 3 Privilege Disable Flag */
#if defined(WDT)
  CY_PAU_PERIPHERAL_WDT                 = PAU_PRIVDIS0_PDIS19_Msk, /**< WDT Privilege Disable Flag */
#endif
#if defined(MATH)
  CY_PAU_PERIPHERAL_MATH_GLOBAL_AND_DIV = PAU_PRIVDIS0_PDIS20_Msk, /**< MATH Global SFRs and Divider Privilege Disable Flag */
#endif
#if defined(MATH)
  CY_PAU_PERIPHERAL_MATH_CORDIC         = PAU_PRIVDIS0_PDIS21_Msk, /**< MATH CORDIC Privilege Disable Flag  */
#endif
#if defined(PORT0)
  CY_PAU_PERIPHERAL_PORT0               = PAU_PRIVDIS0_PDIS22_Msk, /**< Port 0 Privilege Disable Flag */
#endif
#if defined(PORT1)
  CY_PAU_PERIPHERAL_PORT1               = PAU_PRIVDIS0_PDIS23_Msk, /**< Port 1 Privilege Disable Flag */
#endif
#if defined(PORT2)
  CY_PAU_PERIPHERAL_PORT2               = PAU_PRIVDIS0_PDIS24_Msk, /**< Port 2 Privilege Disable Flag */
#endif
#if defined(PORT3)
  CY_PAU_PERIPHERAL_PORT3               = PAU_PRIVDIS0_PDIS25_Msk, /**< Port 3 Privilege Disable Flag */
#endif
#if defined(PORT4)
  CY_PAU_PERIPHERAL_PORT4               = PAU_PRIVDIS0_PDIS26_Msk, /**< Port 4 Privilege Disable Flag */
#endif
#if defined(USIC0)
  CY_PAU_PERIPHERAL_USIC0_CH0           = PAU_PRIVDIS1_PDIS0_Msk | 0x10000000U, /**< USIC0 Channel 0 Privilege Disable Flag */
  CY_PAU_PERIPHERAL_USIC0_CH1           = PAU_PRIVDIS1_PDIS1_Msk | 0x10000000U, /**< USIC0 Channel 1 Privilege Disable Flag */
#endif
#if defined(USIC1)
  CY_PAU_PERIPHERAL_USIC1_CH0           = PAU_PRIVDIS1_PDIS16_Msk | 0x10000000U, /**< USIC1 Channel 0 Privilege Disable Flag */
  CY_PAU_PERIPHERAL_USIC1_CH1           = PAU_PRIVDIS1_PDIS17_Msk | 0x10000000U, /**< USIC1 Channel 1 Privilege Disable Flag */
#endif
#if defined(PRNG)
  CY_PAU_PERIPHERAL_PRNG                = PAU_AVAIL1_AVAIL4_Msk | 0x10000000U, /**< PRNG Availability Flag*/
#endif
#if defined(VADC)
  CY_PAU_PERIPHERAL_VADC_GLOBAL         = PAU_PRIVDIS1_PDIS5_Msk | 0x10000000U, /**< VADC0 Basic SFRs Privilege Disable Flag */
#if defined(VADC_G0)
  CY_PAU_PERIPHERAL_VADC_GROUP0         = PAU_PRIVDIS1_PDIS6_Msk | 0x10000000U, /**< VADC0 Group 0 SFRs Privilege Disable Flag */
#endif
#if defined(VADC_G1)
  CY_PAU_PERIPHERAL_VADC_GROUP1         = PAU_PRIVDIS1_PDIS7_Msk | 0x10000000U, /**< VADC0 Group 1 SFRs Privilege Disable Flag */
#endif
#endif
#if defined(SHS0)
  CY_PAU_PERIPHERAL_VADC_SHS0           = PAU_PRIVDIS1_PDIS8_Msk | 0x10000000U, /**< SHS0 Privilege Disable Flag */
#endif
#if defined(CCU40)
  CY_PAU_PERIPHERAL_CCU40_CC40_AND_GLOBAL     = PAU_PRIVDIS1_PDIS9_Msk | 0x10000000U, /**< CCU40_CC40 and CCU40 Kernel SFRs Privilege Disable Flag */
#if defined(CCU40_CC41)
  CY_PAU_PERIPHERAL_CCU40_CC41                = PAU_PRIVDIS1_PDIS10_Msk | 0x10000000U, /**< CCU40_CC41 Privilege Disable Flag */
#endif
#if defined(CCU40_CC42)
  CY_PAU_PERIPHERAL_CCU40_CC42                = PAU_PRIVDIS1_PDIS11_Msk | 0x10000000U, /**< CCU40_CC42 Privilege Disable Flag */
#endif
#if defined(CCU40_CC43)
  CY_PAU_PERIPHERAL_CCU40_CC43                = PAU_PRIVDIS1_PDIS12_Msk | 0x10000000U, /**< CCU40_CC43 Privilege Disable Flag */
#endif
#endif
#if defined(CCU41)
  CY_PAU_PERIPHERAL_CCU41_CC40_AND_GLOBAL     = PAU_PRIVDIS1_PDIS25_Msk | 0x10000000U, /**< CCU41_CC40 and CCU41 Kernel SFRs Privilege Disable Flag */
#if defined(CCU41_CC41)
  CY_PAU_PERIPHERAL_CCU41_CC41                = PAU_PRIVDIS1_PDIS26_Msk | 0x10000000U, /**< CCU41_CC41 Privilege Disable Flag */
#endif
#if defined(CCU41_CC42)
  CY_PAU_PERIPHERAL_CCU41_CC42                = PAU_PRIVDIS1_PDIS27_Msk | 0x10000000U, /**< CCU41_CC42 Privilege Disable Flag */
#endif
#if defined(CCU41_CC43)
  CY_PAU_PERIPHERAL_CCU41_CC43                = PAU_PRIVDIS1_PDIS28_Msk | 0x10000000U, /**< CCU41_CC43 Privilege Disable Flag */
#endif
#endif
#if defined(CCU80)
  CY_PAU_PERIPHERAL_CCU80_CC80_AND_GLOBAL     = PAU_PRIVDIS2_PDIS0_Msk | 0x20000000U, /**< CCU80_CC80 and CCU80 Kernel SFRs Privilege Disable Flag */
#if defined(CCU80_CC81)
  CY_PAU_PERIPHERAL_CCU80_CC81                = PAU_PRIVDIS2_PDIS1_Msk | 0x20000000U, /**< CCU80_CC81 Privilege Disable Flag */
#endif
#if defined(CCU80_CC82)
  CY_PAU_PERIPHERAL_CCU80_CC82                = PAU_PRIVDIS2_PDIS2_Msk | 0x20000000U, /**< CCU80_CC82 Privilege Disable Flag */
#endif
#if defined(CCU80_CC83)
  CY_PAU_PERIPHERAL_CCU80_CC83                = PAU_PRIVDIS2_PDIS3_Msk | 0x20000000U, /**< CCU80_CC83 Privilege Disable Flag */
#endif
#endif
#if defined(CCU81)
  CY_PAU_PERIPHERAL_CCU81_CC80_AND_GLOBAL     = PAU_PRIVDIS2_PDIS16_Msk | 0x20000000U, /**< CCU81_CC80 and CCU81 Kernel SFRs Privilege Disable Flag */
#if defined(CCU81_CC81)
  CY_PAU_PERIPHERAL_CCU81_CC81                = PAU_PRIVDIS2_PDIS17_Msk | 0x20000000U, /**< CCU81_CC81 Privilege Disable Flag */
#endif
#if defined(CCU81_CC82)
  CY_PAU_PERIPHERAL_CCU81_CC82                = PAU_PRIVDIS2_PDIS18_Msk | 0x20000000U, /**< CCU81_CC82 Privilege Disable Flag */
#endif
#if defined(CCU81_CC83)
  CY_PAU_PERIPHERAL_CCU81_CC83                = PAU_PRIVDIS2_PDIS19_Msk | 0x20000000U, /**< CCU81_CC83 Privilege Disable Flag */
#endif
#endif
#if defined(POSIF0)
  CY_PAU_PERIPHERAL_POSIF0              = PAU_PRIVDIS2_PDIS12_Msk | 0x20000000U, /**< POSIF0 Privilege Disable Flag */
#endif
#if defined(POSIF1)
  CY_PAU_PERIPHERAL_POSIF1              = PAU_PRIVDIS2_PDIS28_Msk | 0x20000000U, /**< POSIF1 Privilege Disable Flag */
#endif
#if defined(BCCU0)
  CY_PAU_PERIPHERAL_BCCU0               = PAU_PRIVDIS2_PDIS15_Msk | 0x20000000U, /**< BCCU0 Privilege Disable Flag */
#endif
#if defined(CAN)
#if defined(CAN_NODE0)
  CY_PAU_PERIPHERAL_MCAN_NODE0_AND_GLOBAL = PAU_PRIVDIS2_PDIS21_Msk | 0x20000000U, /**< MCAN NODE0 and Global SFRs Privilege */
#endif
#if defined(CAN_NODE1)
  CY_PAU_PERIPHERAL_MCAN_NODE1_AND_GLOBAL = PAU_PRIVDIS2_PDIS23_Msk | 0x20000000U, /**< MCAN NODE1 Privilege Disable Flag */
#endif
  CY_PAU_PERIPHERAL_MCAN_OBJECTS = PAU_PRIVDIS2_PDIS28_Msk | 0x20000000U, /**< MCAN Message Objects Privilege Disable Flag */
#endif
} Cy_PAU_PERIPHERAL_t;


/*********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/


/**
 * External Peripheral Access Unit (PAU) device structure <br>
 *
 * The structure represents a collection of all hardware registers
 * used to configure the PAU peripheral on the PSC1M microcontroller.
 * The registers can be accessed with ::CY_PAU.
 */
typedef struct
{
  __I  uint32_t  RESERVED0[16];
  __I  uint32_t  AVAIL[3];
  __I  uint32_t  RESERVED1[13];
  __IO uint32_t  PRIVDIS[3];
  __I  uint32_t  RESERVED2[221];
  __I  uint32_t  ROMSIZE;
  __I  uint32_t  FLSIZE;
  __I  uint32_t  RESERVED3[2];
  __I  uint32_t  RAM0SIZE;
} Cy_PAU_t;

/*********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param peripheral Peripheral of type ::Cy_PAU_PERIPHERAL_t for which access needs to be enabled
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable the peripheral access <br>
 *
 * \par
 * The function resets the PRIVDISx.PDISy bit to enable the access to the registers of a peripheral
 * during run time.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_PAU_DisablePeripheralAccess()
 */
void Cy_PAU_EnablePeripheralAccess(Cy_PAU_PERIPHERAL_t peripheral);

/**
 * @param peripheral Peripheral of type ::Cy_PAU_PERIPHERAL_t for which access needs to be disabled
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable the peripheral access <br>
 *
 * \par
 * The function sets the PRIVDISx.PDISy bit to disable the access to the registers of a peripheral
 * during run time. An access to a disabled or unassigned address location generates a hardfault
 * exception.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_PAU_EnablePeripheralAccess()
 */
void Cy_PAU_DisablePeripheralAccess(Cy_PAU_PERIPHERAL_t peripheral);

/**
 * @param peripheral Peripheral of type ::Cy_PAU_PERIPHERAL_t for which access enabled status to be checked
 * @return bool "false" if peripheral access is enabled, "true" otherwise
 *
 * \par<b>Description: </b><br>
 * Checks if the peripheral access is enabled or not <br>
 *
 * \par
 * The function checks the PRIVDISx.PDISy bit to know whether the access to the registers of a peripheral
 * during run time is enabled or not.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_PAU_DisablePeripheralAccess(), Cy_PAU_EnablePeripheralAccess()
 */
bool Cy_PAU_IsPeripheralAccessEnabled(Cy_PAU_PERIPHERAL_t peripheral);

/**
 * @param peripheral Peripheral of type ::Cy_PAU_PERIPHERAL_t for which access needs to be disabled
 * @return bool Returns "true" if peripheral is available, "false" otherwise
 *
 * \par<b>Description: </b><br>
 * Checks if a peripheral is available or not <br>
 *
 * \par
 * The function checks the AVAILx.AVAILy bit to know whether the peripheral
 * is available or not for the particular device variant.
 */
bool Cy_PAU_IsPeripheralAvailable(Cy_PAU_PERIPHERAL_t peripheral);

/**
 * @return uint32_t Returns ROM size
 *
 * \par<b>Description: </b><br>
 * Gets the ROM size <br>
 *
 * \par
 * The function checks the ROMSIZE.ADDR bitfield to indicate the available size of ROM in the device in bytes.
 */
__STATIC_INLINE uint32_t Cy_PAU_GetROMSize(void)
{
  return (uint32_t)(((CY_PAU->ROMSIZE & PAU_ROMSIZE_ADDR_Msk) >> PAU_ROMSIZE_ADDR_Pos) * 256U);
}

/**
 * @return uint32_t Returns flash size
 *
 * \par<b>Description: </b><br>
 * Gets the flash size <br>
 *
 * \par
 * The function checks the FLSIZE.ADDR bitfield to indicate the available size of FLASH in the device in Kbytes.
 */
__STATIC_INLINE uint32_t Cy_PAU_GetFlashSize(void)
{
  return (uint32_t)((((CY_PAU->FLSIZE & PAU_FLSIZE_ADDR_Msk) >> PAU_FLSIZE_ADDR_Pos) - 1U) * 4U);
}

/**
 * @return uint32_t Returns RAM size
 *
 * \par<b>Description: </b><br>
 * Gets RAM size <br>
 *
 * \par
 * The function checks the RAM0SIZE.ADDR bitfield to indicate the available size of RAM in the device in bytes.
 */
__STATIC_INLINE uint32_t Cy_PAU_GetRAMSize(void)
{
  return (uint32_t)(((CY_PAU->RAM0SIZE & PAU_RAM0SIZE_ADDR_Msk) >> PAU_RAM0SIZE_ADDR_Pos) * 256U);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* defined(PAU) */

#endif /* CY_PAU_H */
