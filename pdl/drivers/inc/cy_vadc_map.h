/**
 * @file cy_vadc_map.h
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

#ifndef CY_ADC_MAP_H
#define CY_ADC_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

/*  Group request source Gating input connection mappings */
#define CY_CCU_40_ST3          		CY_VADC_REQ_GT_A /**< @deprecated use instead  CY_VADC_REQ_GT_CCU40_ST3 */
#define CY_CCU_40_ST2          		CY_VADC_REQ_GT_B /**< @deprecated use instead  CY_VADC_REQ_GT_CCU40_ST2 */
#define CY_CCU_40_ST1          		CY_VADC_REQ_GT_C /**< @deprecated use instead  CY_VADC_REQ_GT_CCU40_ST1 */
#define CY_CCU_40_ST0          		CY_VADC_REQ_GT_D /**< @deprecated use instead  CY_VADC_REQ_GT_CCU40_ST0 */
#define CY_CCU_80_ST3_A        		CY_VADC_REQ_GT_E /**< @deprecated use instead  CY_VADC_REQ_GT_CCU80_ST3A */
#define CY_CCU_81_ST3          		CY_VADC_REQ_GT_F /**< @deprecated use instead  CY_VADC_REQ_GT_CCU80_ST3 */
#define CY_ERU_0_PDOUT2        		CY_VADC_REQ_GT_K /**< @deprecated use instead  CY_VADC_REQ_GT_ERU0_PDOUT2 */
#define CY_ERU_0_PDOUT3        		CY_VADC_REQ_GT_L /**< @deprecated use instead  CY_VADC_REQ_GT_ERU0_PDOUT3 */
#define CY_CCU_80_ST0          		CY_VADC_REQ_GT_M /**< @deprecated use instead  CY_VADC_REQ_GT_CCU80_ST0 */
#define CY_CCU_80_ST1          		CY_VADC_REQ_GT_N /**< @deprecated use instead  CY_VADC_REQ_GT_CCU80_ST1 */
#define CY_ERU_0_PDOUT0        		CY_VADC_REQ_GT_O /**< @deprecated use instead  CY_VADC_REQ_GT_ERU0_PDOUT0 */
#define CY_ERU_0_PDOUT1        		CY_VADC_REQ_GT_P /**< @deprecated use instead  CY_VADC_REQ_GT_ERU0_PDOUT1 */

/* Group request source Trigger input connection mappings */
#define CY_CCU_40_SR2          		CY_VADC_REQ_TR_A /**< @deprecated use instead  CY_VADC_REQ_TR_CCU40_SR2 */
#define CY_CCU_40_SR3          		CY_VADC_REQ_TR_B /**< @deprecated use instead  CY_VADC_REQ_TR_CCU40_SR3 */
#define CY_BCCU0_TRIGOUT       		CY_VADC_REQ_TR_F /**< @deprecated use instead  CY_VADC_REQ_TR_BCCU0_TRIGOUT0, CY_VADC_REQ_TR_G0_BCCU0_TRIGOUT0 or CY_VADC_REQ_TR_G1_BCCU0_TRIGOUT1 */
#define CY_ERU_0_IOUT2         		CY_VADC_REQ_TR_G /**< @deprecated use instead  CY_VADC_REQ_TR_ERU0_IOUT2 */
#define CY_ERU_0_IOUT3         		CY_VADC_REQ_TR_H /**< @deprecated use instead  CY_VADC_REQ_TR_ERU0_IOUT3 */
#define CY_CCU_80_SR2          		CY_VADC_REQ_TR_I /**< @deprecated use instead  CY_VADC_REQ_TR_CCU80_SR2 */
#define CY_CCU_80_SR3          		CY_VADC_REQ_TR_J /**< @deprecated use instead  CY_VADC_REQ_TR_CCU80_SR3 */
#define CY_ERU_0_IOUT0         		CY_VADC_REQ_TR_M /**< @deprecated use instead  CY_VADC_REQ_TR_ERU0_IOUT0 */
#define CY_ERU_0_IOUT1         		CY_VADC_REQ_TR_N /**< @deprecated use instead  CY_VADC_REQ_TR_ERU0_IOUT1 */
#define CY_POSIF_0_SR1         		CY_VADC_REQ_TR_O /**< @deprecated use instead  CY_VADC_REQ_TR_POSIF0_SR1 */
#define CY_REQ_GT_SEL          		CY_VADC_REQ_TR_P /**< @deprecated use instead  CY_VADC_REQ_TR_REQ_GT_SEL */

/*  Group request source Gating input connection mappings */
#define CY_VADC_REQ_GT_CCU40_ST3  		CY_VADC_REQ_GT_A /**< VADC Gating input A */
#define CY_VADC_REQ_GT_CCU40_ST2   		CY_VADC_REQ_GT_B /**< VADC Gating input B */
#define CY_VADC_REQ_GT_CCU40_ST1   		CY_VADC_REQ_GT_C /**< VADC Gating input C */
#define CY_VADC_REQ_GT_CCU40_ST0   		CY_VADC_REQ_GT_D /**< VADC Gating input D */
#define CY_VADC_REQ_GT_CCU80_ST3A 		CY_VADC_REQ_GT_E /**< VADC Gating input E */
#define CY_VADC_REQ_GT_CCU80_ST3   		CY_VADC_REQ_GT_F /**< VADC Gating input F */
#define CY_VADC_REQ_GT_ERU0_PDOUT2 		CY_VADC_REQ_GT_K /**< VADC Gating input K */
#define CY_VADC_REQ_GT_ERU0_PDOUT3 		CY_VADC_REQ_GT_L /**< VADC Gating input L */
#define CY_VADC_REQ_GT_CCU80_ST0   		CY_VADC_REQ_GT_M /**< VADC Gating input M */
#define CY_VADC_REQ_GT_CCU80_ST1   		CY_VADC_REQ_GT_N /**< VADC Gating input N */
#define CY_VADC_REQ_GT_ERU0_PDOUT0 		CY_VADC_REQ_GT_O /**< VADC Gating input O */
#define CY_VADC_REQ_GT_ERU0_PDOUT1 		CY_VADC_REQ_GT_P /**< VADC Gating input P */

/* Group request source Trigger input connection mappings */
#define CY_VADC_REQ_TR_CCU40_SR2   		CY_VADC_REQ_TR_A /**< VADC Trigger input A */
#define CY_VADC_REQ_TR_CCU40_SR3   		CY_VADC_REQ_TR_B /**< VADC Trigger input B */
#define CY_VADC_REQ_TR_BCCU0_TRIGOUT0 		CY_VADC_REQ_TR_F /**< VADC Global Background Source Trigger input F */
#define CY_VADC_REQ_TR_G0_BCCU0_TRIGOUT0 	CY_VADC_REQ_TR_F /**< VADC Group 0 Trigger input F */
#define CY_VADC_REQ_TR_G1_BCCU0_TRIGOUT1 	CY_VADC_REQ_TR_F /**< VADC Group1 Trigger input F */
#define CY_VADC_REQ_TR_ERU0_IOUT2      	CY_VADC_REQ_TR_G /**< VADC Trigger input G */
#define CY_VADC_REQ_TR_ERU0_IOUT3      	CY_VADC_REQ_TR_H /**< VADC Trigger input H */
#define CY_VADC_REQ_TR_CCU80_SR2       	CY_VADC_REQ_TR_I /**< VADC Trigger input I */
#define CY_VADC_REQ_TR_CCU80_SR3       	CY_VADC_REQ_TR_J /**< VADC Trigger input J */
#define CY_VADC_REQ_TR_ERU0_IOUT0      	CY_VADC_REQ_TR_M /**< VADC Trigger input M */
#define CY_VADC_REQ_TR_ERU0_IOUT1      	CY_VADC_REQ_TR_N /**< VADC Trigger input N */
#define CY_VADC_REQ_TR_POSIF0_SR1      	CY_VADC_REQ_TR_O /**< VADC Trigger input O */
#define CY_VADC_REQ_TR_REQ_GT_SEL      	CY_VADC_REQ_TR_P /**< VADC Trigger input P */

#ifdef __cplusplus
}
#endif

#endif
