/**
 * @file cy_can_map.h
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

#ifndef CY_CAN_MAP_H
#define CY_CAN_MAP_H

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#if (CY_DEVICE_ID == CY_DEVICE_PSC1M3) && (CY_DEVICE_PACKAGE == VQFN48)
#define CAN_NODE0_RXD_P0_4   	CY_CAN_NODE_RECEIVE_INPUT_RXDCA
#define CAN_NODE0_RXD_P0_5   	CY_CAN_NODE_RECEIVE_INPUT_RXDCB
#define CAN_NODE0_RXD_P0_14   	CY_CAN_NODE_RECEIVE_INPUT_RXDCC
#define CAN_NODE0_RXD_P0_15   	CY_CAN_NODE_RECEIVE_INPUT_RXDCD
#define CAN_NODE0_RXD_P2_0   	CY_CAN_NODE_RECEIVE_INPUT_RXDCE
#define CAN_NODE0_RXD_P2_1   	CY_CAN_NODE_RECEIVE_INPUT_RXDCF
#define CAN_NODE0_RXD_P1_0   	CY_CAN_NODE_RECEIVE_INPUT_RXDCG
#define CAN_NODE0_RXD_P1_1      CY_CAN_NODE_RECEIVE_INPUT_RXDCH
#define CAN_NODE1_RXD_P0_12   	CY_CAN_NODE_RECEIVE_INPUT_RXDCA
#define CAN_NODE1_RXD_P0_13   	CY_CAN_NODE_RECEIVE_INPUT_RXDCB
#define CAN_NODE1_RXD_P2_10   	CY_CAN_NODE_RECEIVE_INPUT_RXDCE
#define CAN_NODE1_RXD_P2_11   	CY_CAN_NODE_RECEIVE_INPUT_RXDCF
#define CAN_NODE1_RXD_P1_2   	CY_CAN_NODE_RECEIVE_INPUT_RXDCG
#define CAN_NODE1_RXD_P1_3      CY_CAN_NODE_RECEIVE_INPUT_RXDCH
#endif


#if (CY_DEVICE_ID == CY_DEVICE_PSC1M3) && (CY_DEVICE_PACKAGE == LQFP48)
#define CAN_NODE0_RXD_P0_4   	CY_CAN_NODE_RECEIVE_INPUT_RXDCA
#define CAN_NODE0_RXD_P0_5   	CY_CAN_NODE_RECEIVE_INPUT_RXDCB
#define CAN_NODE0_RXD_P0_14   	CY_CAN_NODE_RECEIVE_INPUT_RXDCC
#define CAN_NODE0_RXD_P0_15   	CY_CAN_NODE_RECEIVE_INPUT_RXDCD
#define CAN_NODE0_RXD_P2_0   	CY_CAN_NODE_RECEIVE_INPUT_RXDCE
#define CAN_NODE0_RXD_P2_1   	CY_CAN_NODE_RECEIVE_INPUT_RXDCF
#define CAN_NODE0_RXD_P1_0   	CY_CAN_NODE_RECEIVE_INPUT_RXDCG
#define CAN_NODE0_RXD_P1_1      CY_CAN_NODE_RECEIVE_INPUT_RXDCH
#define CAN_NODE1_RXD_P0_12   	CY_CAN_NODE_RECEIVE_INPUT_RXDCA
#define CAN_NODE1_RXD_P0_13   	CY_CAN_NODE_RECEIVE_INPUT_RXDCB
#define CAN_NODE1_RXD_P4_8   	CY_CAN_NODE_RECEIVE_INPUT_RXDCC
#define CAN_NODE1_RXD_P4_9   	CY_CAN_NODE_RECEIVE_INPUT_RXDCD
#define CAN_NODE1_RXD_P2_10   	CY_CAN_NODE_RECEIVE_INPUT_RXDCE
#define CAN_NODE1_RXD_P2_11   	CY_CAN_NODE_RECEIVE_INPUT_RXDCF
#define CAN_NODE1_RXD_P1_2   	CY_CAN_NODE_RECEIVE_INPUT_RXDCG
#define CAN_NODE1_RXD_P1_3      CY_CAN_NODE_RECEIVE_INPUT_RXDCH
#endif

#define CAN_MO0 ((CAN_MO_TypeDef *)&(CAN_MO->MO[0]))
#define CAN_MO1 ((CAN_MO_TypeDef *)&(CAN_MO->MO[1]))
#define CAN_MO2 ((CAN_MO_TypeDef *)&(CAN_MO->MO[2]))
#define CAN_MO3 ((CAN_MO_TypeDef *)&(CAN_MO->MO[3]))
#define CAN_MO4 ((CAN_MO_TypeDef *)&(CAN_MO->MO[4]))
#define CAN_MO5 ((CAN_MO_TypeDef *)&(CAN_MO->MO[5]))
#define CAN_MO6 ((CAN_MO_TypeDef *)&(CAN_MO->MO[6]))
#define CAN_MO7 ((CAN_MO_TypeDef *)&(CAN_MO->MO[7]))
#define CAN_MO8 ((CAN_MO_TypeDef *)&(CAN_MO->MO[8]))
#define CAN_MO9 ((CAN_MO_TypeDef *)&(CAN_MO->MO[9]))
#define CAN_MO10 ((CAN_MO_TypeDef *)&(CAN_MO->MO[10]))
#define CAN_MO11 ((CAN_MO_TypeDef *)&(CAN_MO->MO[11]))
#define CAN_MO12 ((CAN_MO_TypeDef *)&(CAN_MO->MO[12]))
#define CAN_MO13 ((CAN_MO_TypeDef *)&(CAN_MO->MO[13]))
#define CAN_MO14 ((CAN_MO_TypeDef *)&(CAN_MO->MO[14]))
#define CAN_MO15 ((CAN_MO_TypeDef *)&(CAN_MO->MO[15]))
#define CAN_MO16 ((CAN_MO_TypeDef *)&(CAN_MO->MO[16]))
#define CAN_MO17 ((CAN_MO_TypeDef *)&(CAN_MO->MO[17]))
#define CAN_MO18 ((CAN_MO_TypeDef *)&(CAN_MO->MO[18]))
#define CAN_MO19 ((CAN_MO_TypeDef *)&(CAN_MO->MO[19]))
#define CAN_MO20 ((CAN_MO_TypeDef *)&(CAN_MO->MO[20]))
#define CAN_MO21 ((CAN_MO_TypeDef *)&(CAN_MO->MO[21]))
#define CAN_MO22 ((CAN_MO_TypeDef *)&(CAN_MO->MO[22]))
#define CAN_MO23 ((CAN_MO_TypeDef *)&(CAN_MO->MO[23]))
#define CAN_MO24 ((CAN_MO_TypeDef *)&(CAN_MO->MO[24]))
#define CAN_MO25 ((CAN_MO_TypeDef *)&(CAN_MO->MO[25]))
#define CAN_MO26 ((CAN_MO_TypeDef *)&(CAN_MO->MO[26]))
#define CAN_MO27 ((CAN_MO_TypeDef *)&(CAN_MO->MO[27]))
#define CAN_MO28 ((CAN_MO_TypeDef *)&(CAN_MO->MO[28]))
#define CAN_MO29 ((CAN_MO_TypeDef *)&(CAN_MO->MO[29]))
#define CAN_MO30 ((CAN_MO_TypeDef *)&(CAN_MO->MO[30]))
#define CAN_MO31 ((CAN_MO_TypeDef *)&(CAN_MO->MO[31]))

#endif /* CY_CAN_MAP_H*/
