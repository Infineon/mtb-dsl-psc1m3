/**
 * @file cy_device.h
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

#ifndef CY_DEVICE_H
#define CY_DEVICE_H

/* Family definitions  */
#define    CY_FAMILY_PSC1       (1)

/* Series definitions  */
#define    CY_SERIES_PSC1M      (1)

/* Device definitions  */

#define    CY_DEVICE_PSC1M2     (1)
#define    CY_DEVICE_PSC1M3     (2)


/* Package definitions */
#define    VQFN48     (1)
#define    TSSOP38    (2)
#define    VQFN40     (3)
#define    LQFP48     (4)

#if defined(PSC1M2DBTBQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       TSSOP38
#define CY_DEVICE_FLASH_SIZE    (64UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2EBTBQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       TSSOP38
#define CY_DEVICE_FLASH_SIZE    (128UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2FBTBQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       TSSOP38
#define CY_DEVICE_FLASH_SIZE    (200UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2DBLEQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       VQFN40
#define CY_DEVICE_FLASH_SIZE    (64UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2EBLEQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       VQFN40
#define CY_DEVICE_FLASH_SIZE    (128UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2FBLEQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       VQFN40
#define CY_DEVICE_FLASH_SIZE    (200UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2DBLGQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       VQFN48
#define CY_DEVICE_FLASH_SIZE    (64UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2EBLGQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       VQFN48
#define CY_DEVICE_FLASH_SIZE    (128UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2FBLGQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       VQFN48
#define CY_DEVICE_FLASH_SIZE    (200UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2DBABQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       LQFP48
#define CY_DEVICE_FLASH_SIZE    (64UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2EBABQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       LQFP48
#define CY_DEVICE_FLASH_SIZE    (128UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M2FBABQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M2
#define CY_DEVICE_PACKAGE       LQFP48
#define CY_DEVICE_FLASH_SIZE    (200UL)
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M3DBLGQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M3
#define CY_DEVICE_PACKAGE       VQFN48
#define CY_DEVICE_FLASH_SIZE    (64UL)
#define CY_MULTICAN_PLUS
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M3EBLGQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M3
#define CY_DEVICE_PACKAGE       VQFN48
#define CY_DEVICE_FLASH_SIZE    (128UL)
#define CY_MULTICAN_PLUS
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M3FBLGQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M3
#define CY_DEVICE_PACKAGE       VQFN48
#define CY_DEVICE_FLASH_SIZE    (200UL)
#define CY_MULTICAN_PLUS
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M3DBABQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M3
#define CY_DEVICE_PACKAGE       LQFP48
#define CY_DEVICE_FLASH_SIZE    (64UL)
#define CY_MULTICAN_PLUS
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M3EBABQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M3
#define CY_DEVICE_PACKAGE       LQFP48
#define CY_DEVICE_FLASH_SIZE    (128UL)
#define CY_MULTICAN_PLUS
#define CY_CCU4V3
#define CY_CCU8V3

#elif defined(PSC1M3FBABQ0AT)
#define CY_DEVICE_FAMILY        CY_FAMILY_PSC1
#define CY_DEVICE_SERIES        CY_SERIES_PSC1M
#define CY_DEVICE_ID            CY_DEVICE_PSC1M3
#define CY_DEVICE_PACKAGE       LQFP48
#define CY_DEVICE_FLASH_SIZE    (200UL)
#define CY_MULTICAN_PLUS
#define CY_CCU4V3
#define CY_CCU8V3

#else
#error "cy_device.h: device not supported"
#endif

#if CY_DEVICE_SERIES  == CY_SERIES_PSC1M
#include "PSC1M.h"
#define CY_IP_CLOCK_GATING
#endif

#endif /* CY_DEVICE_H */
