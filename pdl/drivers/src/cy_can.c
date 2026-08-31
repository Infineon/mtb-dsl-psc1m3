/**
 * @file cy_can.c
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

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/
#include "cy_can.h"

#if defined(CAN)
#include "cy_scu.h"

__STATIC_INLINE uint32_t max(uint32_t a, uint32_t b)
{
  return (a > b) ? a : b;
}

__STATIC_INLINE uint32_t min(uint32_t a, uint32_t b)
{
  return (a < b) ? a : b;
}

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

/* The max prescaler is the equal to max BRP setting (64) multiply by 8 (DIV8) */
#define CY_CAN_NODE_MAX_PRESCALER 512

/* maximum TSEG1 is 16 and maximum TSEG2 is 8, plus one fix sync tq */
#define CY_CAN_NODE_MAX_NTQ 25
#define CY_CAN_NODE_MIN_NTQ 8

#define CY_CAN_NODE_MIN_TSEG1 3
#define CY_CAN_NODE_MIN_TSEG2 2

#define CY_CAN_NODE_MAX_TSEG1 15
#define CY_CAN_NODE_MAX_TSEG2 7


int32_t Cy_CAN_NODE_NominalBitTimeConfigureEx(Cy_CAN_NODE_t *const can_node,
    const Cy_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t *const bit_time_config)
{
  /* Check that the CAN frequency is a multiple of the required baudrate */
  if ((bit_time_config->can_frequency % bit_time_config->baudrate) == 0)
  {
    uint32_t prescaler = 0;
    uint32_t div8 = 0;

    /* Calculate the factor between can frequency and required baudrate, this is equal to (prescaler x ntq) */
    uint32_t fcan_div = bit_time_config->can_frequency / bit_time_config->baudrate;

    /* start with highest ntq, i.e as much as possible time quanta should be used to construct a bit time */
    uint32_t ntq = CY_CAN_NODE_MAX_NTQ;
    uint32_t tseg1 = 0;
    uint32_t tseg2 = 0;
    while (ntq >= CY_CAN_NODE_MIN_NTQ)
    {
      /* consider this ntq, only if fcan_div is multiple of ntq */
      if ((fcan_div % ntq) == 0)
      {
        div8 = 0;
        prescaler = fcan_div / ntq;
        if ((prescaler > 0) && (prescaler <= CY_CAN_NODE_MAX_PRESCALER))
        {
          if (prescaler >= 64)
          {
            /* consider prescaler >=64, if it is integer divisible by 8*/
            if ((prescaler & 0x7U) != 0)
            {
              --ntq;
              continue;
            }
            else
            {
              div8 = 1;
            }
          }

          tseg1 = ((ntq - 1) * bit_time_config->sample_point) / 10000;
          tseg2 = ntq - tseg1 - 1;

          if ((CY_CAN_NODE_MIN_TSEG1 <= tseg1) && (tseg1 <= CY_CAN_NODE_MAX_TSEG1) &&
              (CY_CAN_NODE_MIN_TSEG2 <= tseg2) && (tseg2 < CY_CAN_NODE_MAX_TSEG2) && (tseg2 >= bit_time_config->sjw))
          {
            break;
          }


        }
      }
      --ntq;
    }

    if (ntq >= CY_CAN_NODE_MIN_NTQ)
    {

      CY_ASSERT((prescaler != 0));
      CY_ASSERT((tseg1 != 0));
      CY_ASSERT((tseg2 != 0));

      Cy_CAN_NODE_EnableConfigurationChange(can_node);

      /* Configure bit timing register */
      can_node->NBTR = (((tseg2 - 1u) << CAN_NODE_NBTR_TSEG2_Pos) & (uint32_t)CAN_NODE_NBTR_TSEG2_Msk) |
                       (((bit_time_config->sjw - 1U) << CAN_NODE_NBTR_SJW_Pos) & (uint32_t)CAN_NODE_NBTR_SJW_Msk) |
                       (((tseg1 - 1U) << CAN_NODE_NBTR_TSEG1_Pos) & (uint32_t)CAN_NODE_NBTR_TSEG1_Msk) |
                       ((((prescaler >> (3 * div8)) - 1U) << CAN_NODE_NBTR_BRP_Pos) & (uint32_t)CAN_NODE_NBTR_BRP_Msk) |
                       ((div8 << CAN_NODE_NBTR_DIV8_Pos) & (uint32_t)CAN_NODE_NBTR_DIV8_Msk);

      Cy_CAN_NODE_DisableConfigurationChange(can_node);

      return CY_CAN_STATUS_SUCCESS;
    }
  }

  return CY_CAN_STATUS_ERROR;
}

/* Baudrate Configuration */
void Cy_CAN_NODE_NominalBitTimeConfigure (Cy_CAN_NODE_t *const can_node,
    const Cy_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t *const can_bit_time)
{
  uint32_t temp_brp = 12U ;
  uint32_t temp_tseg1 = 12U;
  uint32_t best_brp = 0U;
  uint32_t best_tseg1 = 1U;
  uint32_t best_tseg2 = 0U;
  uint32_t best_tbaud = 0U;
  uint32_t best_error = 10000U;

  CY_ASSERT((can_bit_time->baudrate < 1000000U) ||
             (can_bit_time->baudrate >= 100000U));
  CY_ASSERT(can_bit_time->can_frequency <= 120000000U);
  CY_ASSERT(can_bit_time->can_frequency > 5000000U);
  CY_ASSERT((can_bit_time->sample_point < 10000U) && ((can_bit_time->sample_point > 0U)));

  /*
   * Bit timing & sampling
   * Tq = (BRP+1)/Fcan if DIV8 = 0
   * Tq = 8*(BRP+1)/Fcan if DIV8 = 1
   * TSync = 1.Tq
   * TSeg1 = (TSEG1+1)*Tq                >= 3Tq
   * TSeg2 = (TSEG2+1)*Tq                >= 2Tq
   * Bit Time = TSync + TSeg1 + TSeg2    >= 8Tq
   *
   * Resynchronization:
   *
   * Tsjw = (SJW + 1)*Tq
   * TSeg1 >= Tsjw + Tprop
   * TSeg2 >= Tsjw
   */
  /* search for best baudrate */
  for (temp_brp = 1U; temp_brp <= 64U; temp_brp++)
  {

    uint32_t f_quanta = (uint32_t)((can_bit_time->can_frequency * 10U) / temp_brp);
    uint32_t temp_tbaud = (uint32_t)(f_quanta / (can_bit_time->baudrate));
    uint32_t temp_baudrate;
    uint32_t error;

    if ((temp_tbaud % 10U) > 5U)
    {
      temp_tbaud = (uint32_t)(temp_tbaud / 10U);
      temp_tbaud++;
    }
    else
    {
      temp_tbaud = (uint32_t)(temp_tbaud / 10U);
    }

    if (temp_tbaud > 0U)
    {
      temp_baudrate = (uint32_t) (f_quanta / (temp_tbaud * 10U));
    }
    else
    {
      temp_baudrate = f_quanta / 10U;
      temp_tbaud = 1;
    }

    if (temp_baudrate >= can_bit_time->baudrate)
    {
      error = temp_baudrate - can_bit_time->baudrate;
    }
    else
    {
      error = can_bit_time->baudrate - temp_baudrate;
    }

    if ((temp_tbaud <= 20U) && (best_error > error))
    {
      best_brp = temp_brp;
      best_tbaud = temp_tbaud;
      best_error = (error);

      if (error < 1000U)
      {
        break;
      }
    }
  }
  /* search for best sample point */
  best_error = 10000U;

  for (temp_tseg1 = 64U; temp_tseg1 >= 3U; temp_tseg1--)
  {
    uint32_t tempSamplePoint = ((temp_tseg1 + 1U) * 10000U) / best_tbaud;
    uint32_t error;
    if (tempSamplePoint >= can_bit_time->sample_point)
    {
      error = tempSamplePoint  - can_bit_time->sample_point;
    }
    else
    {
      error = can_bit_time->sample_point  - tempSamplePoint;
    }
    if (best_error > error)
    {
      best_tseg1 = temp_tseg1;
      best_error = error;
    }
    if (tempSamplePoint < (can_bit_time->sample_point))
    {
      break;
    }
  }

  best_tseg2 = best_tbaud - best_tseg1 - 1U;

  Cy_CAN_NODE_EnableConfigurationChange(can_node);
  /* Configure bit timing register */
  can_node->NBTR = (((uint32_t)(best_tseg2 - 1u) << CAN_NODE_NBTR_TSEG2_Pos) & (uint32_t)CAN_NODE_NBTR_TSEG2_Msk) |
                   ((((uint32_t)((uint32_t)(can_bit_time->sjw) - 1U) << CAN_NODE_NBTR_SJW_Pos)) & (uint32_t)CAN_NODE_NBTR_SJW_Msk) |
                   (((uint32_t)(best_tseg1 - 1U) << CAN_NODE_NBTR_TSEG1_Pos) & (uint32_t)CAN_NODE_NBTR_TSEG1_Msk) |
                   (((uint32_t)(best_brp - 1U) << CAN_NODE_NBTR_BRP_Pos) & (uint32_t)CAN_NODE_NBTR_BRP_Msk) |
                   (((uint32_t)0U << CAN_NODE_NBTR_DIV8_Pos) & (uint32_t)CAN_NODE_NBTR_DIV8_Msk);
  Cy_CAN_NODE_DisableConfigurationChange(can_node);
}
/* Function to allocate message object from free list to node list */
void Cy_CAN_AllocateMOtoNodeList(Cy_CAN_t *const obj, const uint8_t node_num, const uint8_t mo_num)
{
  /* wait while panel operation is in progress. */
  while (Cy_CAN_IsPanelControlReady(obj) == false)
  {
    /*Do nothing*/
  };

  /* Panel Command for  allocation of MO to node list */
  Cy_CAN_PanelControl(obj, CY_CAN_PANCMD_STATIC_ALLOCATE, mo_num, (node_num + 1U));
}

/* Disable CY_CAN Peripheral */
void Cy_CAN_Disable(Cy_CAN_t *const obj)
{
  /* Disable CAN Module */
  obj->CLC = CAN_CLC_DISR_Msk;
#if defined(CY_IP_PERIPHERAL_RESET)
  Cy_SCU_RESET_AssertPeripheralReset(CY_SCU_PERIPHERAL_RESET_MCAN);
#endif
#if defined(CY_IP_CLOCK_GATING)
  Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_MCAN);
#endif
}

/* Enable CY_CAN Peripheral */
void Cy_CAN_Enable(Cy_CAN_t *const obj)
{
#if defined(CY_IP_CLOCK_GATING)
  Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_MCAN);
#endif
#if defined(CY_IP_PERIPHERAL_RESET)
  Cy_SCU_RESET_DeassertPeripheralReset(CY_SCU_PERIPHERAL_RESET_MCAN);
#endif
  /* Enable CAN Module */
  obj->CLC &= ~(uint32_t)CAN_CLC_DISR_Msk;
  while (obj->CLC & CAN_CLC_DISS_Msk)
  {
    /*Do nothing*/
  };
}

#if defined(CY_MULTICAN_PLUS)
void Cy_CAN_Init(Cy_CAN_t *const obj, Cy_CAN_CANCLKSRC_t clksrc, uint32_t can_frequency)
{
  uint32_t  step_n, step_f;
  bool normal_divider;
  uint32_t freq_n, freq_f;
  uint32_t step;
  uint32_t can_frequency_khz;
  uint32_t peripheral_frequency_khz;
  Cy_CAN_DM_t can_divider_mode;

  uint32_t peripheral_frequency;
  /*Enabling the module*/
  Cy_CAN_Enable(obj);

  Cy_CAN_SetBaudrateClockSource(obj, clksrc);

  peripheral_frequency = Cy_CAN_GetBaudrateClockFrequency(obj);

  CY_ASSERT(can_frequency <= peripheral_frequency);

  /* Normal divider mode */
  step_n = (uint32_t)min(max(0U, (1024U - (peripheral_frequency / can_frequency))), 1023U);
  freq_n = (uint32_t) (peripheral_frequency / (1024U - step_n));

  /* Fractional divider mode */
  can_frequency_khz = (uint32_t) (can_frequency >> 6);
  peripheral_frequency_khz = (uint32_t)(peripheral_frequency >> 6);

  step_f = (uint32_t)(min( (((1024U * can_frequency_khz) / peripheral_frequency_khz) ), 1023U ));
  freq_f = (uint32_t)((peripheral_frequency_khz * step_f) / 1024U);
  freq_f = freq_f << 6;

  normal_divider  = (uint32_t)(can_frequency - freq_n) <= (can_frequency - freq_f);

  step   = (normal_divider != 0U) ? step_n : step_f;
  can_divider_mode = (normal_divider != 0U) ? CY_CAN_DM_NORMAL : CY_CAN_DM_FRACTIONAL;

  obj->FDR &= (uint32_t) ~(CAN_FDR_DM_Msk | CAN_FDR_STEP_Msk);
  obj->FDR |= ((uint32_t)can_divider_mode << CAN_FDR_DM_Pos) | ((uint32_t)step << CAN_FDR_STEP_Pos);

}

#else
/* Initialization of CY_CAN GLOBAL Object */
void Cy_CAN_Init(Cy_CAN_t *const obj, uint32_t can_frequency)
{
  uint32_t  step_n, step_f;
  bool normal_divider;
  uint32_t freq_n, freq_f;
  uint32_t step;
  uint32_t can_frequency_khz;
  uint32_t peripheral_frequency_khz;
  Cy_CAN_DM_t can_divider_mode;

  uint32_t peripheral_frequency = (Cy_SCU_CLOCK_GetPeripheralClockFrequency());

  CY_ASSERT(can_frequency <= peripheral_frequency);

  /*Enabling the module*/
  Cy_CAN_Enable(obj);

  /* Normal divider mode */
  step_n = (uint32_t)min(max(0U, (1024U - (peripheral_frequency / can_frequency))), 1023U);
  freq_n = (uint32_t) (peripheral_frequency / (1024U - step_n));

  /* Fractional divider mode */
  can_frequency_khz = (uint32_t) (can_frequency >> 6);
  peripheral_frequency_khz = (uint32_t)(peripheral_frequency >> 6);

  step_f = (uint32_t)(min( (((1024U * can_frequency_khz) / peripheral_frequency_khz) ), 1023U ));
  freq_f = (uint32_t)((peripheral_frequency_khz * step_f) / 1024U);
  freq_f = freq_f << 6;

  normal_divider  = (uint32_t)(can_frequency - freq_n) <= (can_frequency - freq_f);

  step   = (normal_divider != 0U) ? step_n : step_f;
  can_divider_mode = (normal_divider != 0U) ? CY_CAN_DM_NORMAL : CY_CAN_DM_FRACTIONAL;

  obj->FDR &= (uint32_t) ~(CAN_FDR_DM_Msk | CAN_FDR_STEP_Msk);
  obj->FDR |= ((uint32_t)can_divider_mode << CAN_FDR_DM_Pos) | ((uint32_t)step << CAN_FDR_STEP_Pos);
}
#endif

void Cy_CAN_SetBaudrateClockSource(Cy_CAN_t *const obj, const Cy_CAN_CANCLKSRC_t source)
{
#if defined(CY_MULTICAN_PLUS)
  obj->MCR = (obj->MCR & ~CAN_MCR_CLKSEL_Msk) | source ;
#else
  CY_UNUSED_ARG(obj);
  CY_UNUSED_ARG(source);
#endif
}

Cy_CAN_CANCLKSRC_t Cy_CAN_GetBaudrateClockSource(Cy_CAN_t *const obj)
{
  return ((Cy_CAN_CANCLKSRC_t)((obj->MCR & CAN_MCR_CLKSEL_Msk) >> CAN_MCR_CLKSEL_Pos));
}

uint32_t Cy_CAN_GetBaudrateClockFrequency(Cy_CAN_t *const obj)
{
  uint32_t frequency = 0;

#if defined(CY_MULTICAN_PLUS)
  switch (Cy_CAN_GetBaudrateClockSource(obj))
  {
    case CY_CAN_CANCLKSRC_MCLK:
      frequency = Cy_SCU_CLOCK_GetPeripheralClockFrequency();
      break;
    case CY_CAN_CANCLKSRC_FOHP:
      frequency = OSCHP_GetFrequency();
      break;
  }
#else
  CY_UNUSED_ARG(obj);
  frequency = Cy_SCU_CLOCK_GetPeripheralClockFrequency();
#endif

  return frequency;
}

uint32_t Cy_CAN_InitEx(Cy_CAN_t *const obj, Cy_CAN_CANCLKSRC_t clksrc, uint32_t can_frequency)
{
  uint32_t step_n;
  uint32_t freq_n;
  uint32_t peripheral_frequency;

  /*Enabling the module*/
  Cy_CAN_Enable(obj);

  Cy_CAN_SetBaudrateClockSource(obj, clksrc);
  peripheral_frequency = Cy_CAN_GetBaudrateClockFrequency(obj);
  CY_ASSERT(can_frequency <= peripheral_frequency);

  /* Normal divider mode */
  step_n = (uint32_t)min(max(0U, (1024U - (peripheral_frequency / can_frequency))), 1023U);
  freq_n = (uint32_t)(peripheral_frequency / (1024U - step_n));

  obj->FDR &= (uint32_t) ~(CAN_FDR_DM_Msk | CAN_FDR_STEP_Msk);
  obj->FDR |= ((uint32_t)CY_CAN_DM_NORMAL << CAN_FDR_DM_Pos) | ((uint32_t)step_n << CAN_FDR_STEP_Pos);

  return freq_n;
}

uint32_t Cy_CAN_GetClockFrequency(Cy_CAN_t *const obj)
{
  uint32_t step_n = (obj->FDR & CAN_FDR_STEP_Msk) >> CAN_FDR_STEP_Pos;
  return (Cy_CAN_GetBaudrateClockFrequency(obj) * (1024U - step_n));
}

/* Sets the Identifier of the MO */
void Cy_CAN_MO_SetIdentifier(Cy_CAN_MO_t *const can_mo, const uint32_t can_identifier)
{
  if ((can_mo->can_mo_ptr->MOAR & CAN_MO_MOAR_IDE_Msk) != (uint32_t)CAN_MO_MOAR_IDE_Msk)
  {
    can_mo->can_mo_ptr->MOAR = ((can_mo->can_mo_ptr->MOAR) & ~(uint32_t)(CAN_MO_MOAR_ID_Msk)) |
                               ((can_identifier << Cy_CAN_MO_MOAR_STDID_Pos) & (uint32_t)CAN_MO_MOAR_ID_Msk);
  }
  else
  {
    can_mo->can_mo_ptr->MOAR = ((can_mo->can_mo_ptr->MOAR) & ~(uint32_t)(CAN_MO_MOAR_ID_Msk)) |
                               (can_identifier & (uint32_t)CAN_MO_MOAR_ID_Msk);
  }
  can_mo->can_identifier = can_identifier;
}


/* Gets the Identifier of the MO */
uint32_t Cy_CAN_MO_GetIdentifier(const Cy_CAN_MO_t *const can_mo)
{
  uint32_t identifier;
  if ((can_mo->can_mo_ptr->MOAR & CAN_MO_MOAR_IDE_Msk) != (uint32_t)CAN_MO_MOAR_IDE_Msk)
  {
    identifier = ((can_mo->can_mo_ptr->MOAR) & (uint32_t)(CAN_MO_MOAR_ID_Msk)) >> Cy_CAN_MO_MOAR_STDID_Pos;
  }
  else
  {
    identifier = ((can_mo->can_mo_ptr->MOAR) & (uint32_t)(CAN_MO_MOAR_ID_Msk));
  }
  return identifier;
}

/* Gets the acceptance mask for the CAN MO. */
uint32_t Cy_CAN_MO_GetAcceptanceMask(const Cy_CAN_MO_t *const can_mo)
{
  uint32_t identifier_mask;
  if (((can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_MIDE_Msk) != (uint32_t)CAN_MO_MOAMR_MIDE_Msk)
      && ((can_mo->can_mo_ptr->MOAR & CAN_MO_MOAR_IDE_Msk) != (uint32_t)CAN_MO_MOAR_IDE_Msk))
  {
    identifier_mask = ((can_mo->can_mo_ptr->MOAMR) & (uint32_t)(CAN_MO_MOAMR_AM_Msk)) >> Cy_CAN_MO_MOAR_STDID_Pos;
  }
  else
  {
    identifier_mask = ((can_mo->can_mo_ptr->MOAMR) & (uint32_t)(CAN_MO_MOAMR_AM_Msk));
  }
  return identifier_mask;
}

/* Sets the acceptance mask of the MO */
void Cy_CAN_MO_SetAcceptanceMask(Cy_CAN_MO_t *const can_mo, const uint32_t can_id_mask)
{
  if (((can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_MIDE_Msk) != 0)
      && ((can_mo->can_mo_ptr->MOAR & CAN_MO_MOAR_IDE_Msk) == 0))
  {
    /* Message object n receives frames only with matching IDE bit. */
    can_mo->can_mo_ptr->MOAMR = ((can_mo->can_mo_ptr->MOAMR) & ~(uint32_t)(CAN_MO_MOAMR_AM_Msk)) |
                                ((can_id_mask << Cy_CAN_MO_MOAR_STDID_Pos) & (uint32_t)Cy_CAN_MO_MOAR_STDID_Msk);
  }
  else
  {
    can_mo->can_mo_ptr->MOAMR = ((can_mo->can_mo_ptr->MOAMR) & ~(uint32_t)(CAN_MO_MOAMR_AM_Msk)) |
                                (can_id_mask & (uint32_t)CAN_MO_MOAMR_AM_Msk);
  }

  can_mo->can_id_mask = can_id_mask;
}

/* Initialization of CY_CAN MO Object */
void Cy_CAN_MO_Config(const Cy_CAN_MO_t *const can_mo)
{
  uint32_t reg;

  /* Configure MPN */
  uint32_t num = ((uint32_t)(can_mo->can_mo_ptr) - CAN_BASE - 0x1000U) / 0x0020U;
  uint32_t set = (((uint32_t)(num / 32) << (CAN_MO_MOIPR_MPN_Pos + 5U)) | ((uint32_t)(num % 32) << CAN_MO_MOIPR_MPN_Pos));
  can_mo->can_mo_ptr->MOIPR &= ~(CAN_MO_MOIPR_MPN_Msk);
  can_mo->can_mo_ptr->MOIPR |= set;

  if (((can_mo->can_id_mode != (uint32_t) CY_CAN_FRAME_TYPE_STANDARD_11BITS) &&
       (can_mo->can_id_mode != (uint32_t) CY_CAN_FRAME_TYPE_EXTENDED_29BITS)) ||
      ((can_mo->can_mo_type != CY_CAN_MO_TYPE_RECMSGOBJ) &&
       (can_mo->can_mo_type != CY_CAN_MO_TYPE_TRANSMSGOBJ)))
  {
    /*Do nothing*/
  }
  else
  {

    /* Disable Message object */
    can_mo->can_mo_ptr->MOCTR = CAN_MO_MOCTR_RESMSGVAL_Msk;
    if (can_mo->can_id_mode == (uint32_t)CY_CAN_FRAME_TYPE_STANDARD_11BITS)
    {
      reg = can_mo->mo_ar;
      reg &= (uint32_t) ~(CAN_MO_MOAR_ID_Msk);
      reg |= (can_mo->can_identifier << Cy_CAN_MO_MOAR_STDID_Pos);
      can_mo->can_mo_ptr->MOAR = reg;

      reg = can_mo->mo_amr;
      reg &= (uint32_t) ~(CAN_MO_MOAMR_AM_Msk);
      reg |= (can_mo->can_id_mask << Cy_CAN_MO_MOAR_STDID_Pos);
      can_mo->can_mo_ptr->MOAMR = reg;
    }
    else
    {
      can_mo->can_mo_ptr->MOAR = can_mo->mo_ar;
      can_mo->can_mo_ptr->MOAMR = can_mo->mo_amr;
    }
    /* Check whether message object is transmit message object */
    if (can_mo->can_mo_type == CY_CAN_MO_TYPE_TRANSMSGOBJ)
    {
      /* Set MO as Transmit message object  */
      Cy_CAN_MO_UpdateData(can_mo);
      can_mo->can_mo_ptr->MOCTR = CAN_MO_MOCTR_SETDIR_Msk;

      /* Reset RTSEL and Set MSGVAL, TXEN0 and TXEN1 bits */
      can_mo->can_mo_ptr->MOCTR = (CAN_MO_MOCTR_SETTXEN0_Msk | CAN_MO_MOCTR_SETTXEN1_Msk | CAN_MO_MOCTR_SETMSGVAL_Msk |
                                   CAN_MO_MOCTR_RESRXEN_Msk  | CAN_MO_MOCTR_RESRTSEL_Msk);
    }
    else
    {
      /* Set MO as Receive message object and set RXEN bit */
      can_mo->can_mo_ptr->MOCTR = CAN_MO_MOCTR_RESDIR_Msk;

      /* Reset RTSEL, TXEN1 and TXEN2 and Set MSGVAL and RXEN bits */
      can_mo->can_mo_ptr->MOCTR = (CAN_MO_MOCTR_RESTXEN0_Msk | CAN_MO_MOCTR_RESTXEN1_Msk | CAN_MO_MOCTR_SETMSGVAL_Msk |
                                   CAN_MO_MOCTR_SETRXEN_Msk | CAN_MO_MOCTR_RESRTSEL_Msk);
    }

  }
}

/* Update of CY_CAN Object */
Cy_CAN_STATUS_t Cy_CAN_MO_UpdateData(const Cy_CAN_MO_t *const can_mo)
{
  Cy_CAN_STATUS_t error = CY_CAN_STATUS_MO_NOT_ACCEPTABLE;
  /* Check whether message object is transmit message object */
  if (can_mo->can_mo_type == CY_CAN_MO_TYPE_TRANSMSGOBJ)
  {
    can_mo->can_mo_ptr->MOCTR = CAN_MO_MOCTR_RESMSGVAL_Msk;
    /* Configure data length */
    can_mo->can_mo_ptr->MOFCR = ((can_mo->can_mo_ptr->MOFCR) & ~(uint32_t)(CAN_MO_MOFCR_DLC_Msk)) |
                                (((uint32_t) can_mo->can_data_length << CAN_MO_MOFCR_DLC_Pos) & (uint32_t)CAN_MO_MOFCR_DLC_Msk);
    /* Configure Data registers*/
    can_mo->can_mo_ptr->MODATAL = can_mo->can_data[0];
    can_mo->can_mo_ptr->MODATAH = can_mo->can_data[1];
    /* Reset RTSEL and Set MSGVAL ,TXEN0 and TXEN1 bits */
    can_mo->can_mo_ptr->MOCTR = (CAN_MO_MOCTR_SETNEWDAT_Msk | CAN_MO_MOCTR_SETMSGVAL_Msk | CAN_MO_MOCTR_RESRTSEL_Msk);
    error = CY_CAN_STATUS_SUCCESS;
  }
  else
  {
    error = CY_CAN_STATUS_MO_NOT_ACCEPTABLE;
  }
  return error;
}

/* This function is will put a transmit request to transmit message object */
Cy_CAN_STATUS_t Cy_CAN_MO_Transmit(const Cy_CAN_MO_t *const can_mo)
{
  Cy_CAN_STATUS_t error = CY_CAN_STATUS_ERROR;
  uint32_t mo_type = (uint32_t)(((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_MSGVAL_Msk) >> CAN_MO_MOSTAT_MSGVAL_Pos);
  uint32_t mo_transmission_ongoing = (uint32_t) ((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_TXRQ_Msk) >> CAN_MO_MOSTAT_TXRQ_Pos;
  /* check if message is disabled */
  if (mo_type == 0U)
  {
    error = CY_CAN_STATUS_MO_DISABLED;
  }
  /* check if transmission is ongoing on message object */
  else if (mo_transmission_ongoing == 1U)
  {
    error = CY_CAN_STATUS_BUSY;
  }
  else
  {
    /* set TXRQ bit */
    can_mo->can_mo_ptr-> MOCTR = CAN_MO_MOCTR_SETTXRQ_Msk | CAN_MO_MOCTR_SETTXEN0_Msk | CAN_MO_MOCTR_SETTXEN1_Msk;
    error = CY_CAN_STATUS_SUCCESS;
  }
  return error;
}

/* This function is will read the message object data bytes */
Cy_CAN_STATUS_t Cy_CAN_MO_ReceiveData (Cy_CAN_MO_t *can_mo)
{
  Cy_CAN_STATUS_t error = CY_CAN_STATUS_ERROR;
  uint8_t rx_pnd = 0U;
  uint8_t new_data = 0U;
  uint32_t mo_type = (uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_DIR_Msk) >> CAN_MO_MOSTAT_DIR_Pos;
  uint32_t mo_recepcion_ongoing = (uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_RXUPD_Msk) >> CAN_MO_MOSTAT_RXUPD_Pos;
  /* check if message object is a receive message object */
  if (mo_type != (uint32_t)CY_CAN_MO_TYPE_RECMSGOBJ)
  {
    error = CY_CAN_STATUS_MO_NOT_ACCEPTABLE;
  }
  /* check if reception is ongoing on message object */
  else if (mo_recepcion_ongoing == 1U)
  {
    error = CY_CAN_STATUS_BUSY;
  }
  else
  {
    /* read message parameters */
    do
    {
      can_mo->can_data[0] = can_mo->can_mo_ptr->MODATAL;
      can_mo->can_data[1] = can_mo->can_mo_ptr->MODATAH;

      rx_pnd = (uint8_t)((uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_RXUPD_Msk) >> CAN_MO_MOSTAT_RXUPD_Pos);
      new_data = (uint8_t)((uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_NEWDAT_Msk) >> CAN_MO_MOSTAT_NEWDAT_Pos);
    }
    while ((rx_pnd != 0U) || (new_data != 0U));

    error = CY_CAN_STATUS_SUCCESS;
  }
  return error;
}


/* This function is will read the message object data bytes */
Cy_CAN_STATUS_t Cy_CAN_MO_Receive (Cy_CAN_MO_t *can_mo)
{
  Cy_CAN_STATUS_t error = CY_CAN_STATUS_ERROR;
  uint8_t rx_pnd = 0U;
  uint8_t new_data = 0U;
  uint32_t mo_type = (uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_DIR_Msk) >> CAN_MO_MOSTAT_DIR_Pos;
  uint32_t mo_recepcion_ongoing = (uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_RXUPD_Msk) >> CAN_MO_MOSTAT_RXUPD_Pos;
  /* check if message object is a receive message object */
  if (mo_type != (uint32_t)CY_CAN_MO_TYPE_RECMSGOBJ)
  {
    error = CY_CAN_STATUS_MO_NOT_ACCEPTABLE;
  }
  /* check if reception is ongoing on message object */
  else if (mo_recepcion_ongoing == 1U)
  {
    error = CY_CAN_STATUS_BUSY;
  }
  else
  {
    /* read message parameters */
    do
    {
      can_mo->can_mo_ptr->MOCTR = CAN_MO_MOCTR_RESNEWDAT_Msk;
      if ((((can_mo->can_mo_ptr->MOAR) & CAN_MO_MOAR_IDE_Msk) >> CAN_MO_MOAR_IDE_Pos) == 0U)
      {
        can_mo->can_id_mode = (uint32_t)CY_CAN_FRAME_TYPE_STANDARD_11BITS;
        can_mo->can_identifier = (can_mo->can_mo_ptr->MOAR & Cy_CAN_MO_MOAR_STDID_Msk) >> Cy_CAN_MO_MOAR_STDID_Pos;
        can_mo->can_ide_mask = (uint32_t)(can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_MIDE_Msk) >> CAN_MO_MOAMR_MIDE_Pos;
        if (can_mo->can_ide_mask == 1U)
        {
          can_mo->can_id_mask = (uint32_t)(can_mo->can_mo_ptr->MOAMR & Cy_CAN_MO_MOAR_STDID_Msk) >> Cy_CAN_MO_MOAR_STDID_Pos;
        }
        else
        {
          can_mo->can_id_mask = (uint32_t)(can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_AM_Msk);
        }
      }
      else
      {
        can_mo->can_id_mode = (uint32_t)CY_CAN_FRAME_TYPE_EXTENDED_29BITS;
        can_mo->can_identifier = (can_mo->can_mo_ptr->MOAR & CAN_MO_MOAR_ID_Msk);
        can_mo->can_id_mask = (uint32_t)(can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_AM_Msk);
        can_mo->can_ide_mask = (uint32_t)(can_mo->can_mo_ptr->MOAMR & CAN_MO_MOAMR_MIDE_Msk) >> CAN_MO_MOAMR_MIDE_Pos;
      }
      can_mo->can_data_length = (uint8_t)((uint32_t)((can_mo->can_mo_ptr->MOFCR) & CAN_MO_MOFCR_DLC_Msk) >> CAN_MO_MOFCR_DLC_Pos);

      can_mo->can_data[0] = can_mo->can_mo_ptr->MODATAL;
      can_mo->can_data[1] = can_mo->can_mo_ptr->MODATAH;

      rx_pnd = (uint8_t)((uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_RXUPD_Msk) >> CAN_MO_MOSTAT_RXUPD_Pos);
      new_data = (uint8_t)((uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_NEWDAT_Msk) >> CAN_MO_MOSTAT_NEWDAT_Pos);
    }
    while ((rx_pnd != 0U) && (new_data != 0U));

    can_mo->can_mo_type = CY_CAN_MO_TYPE_RECMSGOBJ;
    error = CY_CAN_STATUS_SUCCESS;
  }
  return error;
}

/* Function to enable node event */
void Cy_CAN_NODE_EnableEvent(Cy_CAN_NODE_t *const can_node, const Cy_CAN_NODE_EVENT_t event)
{
  if (event != CY_CAN_NODE_EVENT_CFCIE)
  {
    can_node->NCR |= (uint32_t)event;
  }
  else
  {
    can_node->NFCR |= (uint32_t)event;
  }
}

/* Function to disable node event */
void Cy_CAN_NODE_DisableEvent(Cy_CAN_NODE_t *const can_node, const Cy_CAN_NODE_EVENT_t event)
{
  if (event != CY_CAN_NODE_EVENT_CFCIE)
  {
    can_node->NCR &= ~(uint32_t)event;
  }
  else
  {
    can_node->NFCR &= ~(uint32_t)event;
  }
}
/* Function to transmit MO from the FIFO */
Cy_CAN_STATUS_t Cy_CAN_TXFIFO_Transmit(const Cy_CAN_MO_t *const can_mo)
{
  Cy_CAN_STATUS_t error = CY_CAN_STATUS_ERROR;
  uint32_t mo_type = ((uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_MSGVAL_Msk) >> CAN_MO_MOSTAT_MSGVAL_Pos);
  uint32_t mo_transmission_ongoing = (uint32_t)((can_mo->can_mo_ptr->MOSTAT) & CAN_MO_MOSTAT_TXRQ_Msk) >> CAN_MO_MOSTAT_TXRQ_Pos;
  uint32_t mo_cur =  (uint32_t)(can_mo->can_mo_ptr-> MOFGPR & CAN_MO_MOFGPR_CUR_Msk) >> CAN_MO_MOFGPR_CUR_Pos;
  CAN_MO_TypeDef  *mo = (CAN_MO_TypeDef *)(CAN_BASE + 0x1000UL + (mo_cur * 0x0020UL));
  /* check if message is disabled */
  if (mo_type == 0U)
  {
    error = CY_CAN_STATUS_MO_DISABLED;
  }
  /* check if transmission is ongoing on message object */
  else if (mo_transmission_ongoing == 1U)
  {
    error = CY_CAN_STATUS_BUSY;
  }
  else
  {
    mo->MOCTR = CAN_MO_MOCTR_SETTXRQ_Msk | CAN_MO_MOCTR_SETTXEN0_Msk | CAN_MO_MOCTR_SETTXEN1_Msk;
    error = CY_CAN_STATUS_SUCCESS;
  }
  return error;
}

/* Function to initialize the transmit FIFO MO base object */
void Cy_CAN_TXFIFO_ConfigMOBaseObject(const Cy_CAN_MO_t *const can_mo, const Cy_CAN_FIFO_CONFIG_t can_fifo)
{
  can_mo->can_mo_ptr->MOFCR = ((can_mo->can_mo_ptr->MOFCR ) & ~(uint32_t)(CAN_MO_MOFCR_MMC_Msk)) |
                              (((uint32_t)0x2U << CAN_MO_MOFCR_MMC_Pos) & (uint32_t)CAN_MO_MOFCR_MMC_Msk);
  can_mo->can_mo_ptr->MOFGPR = ((can_mo->can_mo_ptr->MOFGPR ) & ~(uint32_t)(CAN_MO_MOFGPR_BOT_Msk |
                                CAN_MO_MOFGPR_TOP_Msk |
                                CAN_MO_MOFGPR_CUR_Msk)) |
                               (((uint32_t)can_fifo.fifo_bottom << CAN_MO_MOFGPR_BOT_Pos) & (uint32_t)CAN_MO_MOFGPR_BOT_Msk) |
                               (((uint32_t)can_fifo.fifo_base << CAN_MO_MOFGPR_CUR_Pos) & (uint32_t) CAN_MO_MOFGPR_CUR_Msk) |
                               (((uint32_t)can_fifo.fifo_top << CAN_MO_MOFGPR_TOP_Pos) & (uint32_t) CAN_MO_MOFGPR_TOP_Msk);
}
/* Function to Initialize the receive FIFO MO base object */
void Cy_CAN_RXFIFO_ConfigMOBaseObject(const Cy_CAN_MO_t *const can_mo, const Cy_CAN_FIFO_CONFIG_t can_fifo)
{
  can_mo->can_mo_ptr->MOFCR = ((can_mo->can_mo_ptr->MOFCR ) & ~(uint32_t)(CAN_MO_MOFCR_MMC_Msk)) |
                              (((uint32_t)0x1U << CAN_MO_MOFCR_MMC_Pos) & (uint32_t)CAN_MO_MOFCR_MMC_Msk);
  can_mo->can_mo_ptr->MOFGPR = ((can_mo->can_mo_ptr->MOFGPR ) & ~( uint32_t)(CAN_MO_MOFGPR_BOT_Msk |
                                CAN_MO_MOFGPR_TOP_Msk |
                                CAN_MO_MOFGPR_CUR_Msk)) |
                               (((uint32_t)can_fifo.fifo_bottom << CAN_MO_MOFGPR_BOT_Pos) & (uint32_t)CAN_MO_MOFGPR_BOT_Msk) |
                               (((uint32_t)can_fifo.fifo_base << CAN_MO_MOFGPR_CUR_Pos) & (uint32_t)CAN_MO_MOFGPR_CUR_Msk) |
                               (((uint32_t)can_fifo.fifo_top << CAN_MO_MOFGPR_TOP_Pos) & (uint32_t)CAN_MO_MOFGPR_TOP_Msk);
}

/* Function to Initialize the FIFO MO slave object */
void Cy_CAN_TXFIFO_ConfigMOSlaveObject(const Cy_CAN_MO_t *const can_mo, const Cy_CAN_FIFO_CONFIG_t can_fifo)
{
  can_mo->can_mo_ptr->MOFCR = ((can_mo->can_mo_ptr->MOFCR ) & ~(uint32_t)(CAN_MO_MOFCR_MMC_Msk)) |
                              (((uint32_t)0x3U << CAN_MO_MOFCR_MMC_Pos) & (uint32_t)CAN_MO_MOFCR_MMC_Msk);
  can_mo->can_mo_ptr->MOFGPR = ((can_mo->can_mo_ptr->MOFGPR ) & ~(uint32_t)(CAN_MO_MOFGPR_CUR_Msk)) |
                               (((uint32_t)can_fifo.fifo_base << CAN_MO_MOFGPR_CUR_Pos) & (uint32_t)CAN_MO_MOFGPR_CUR_Msk);

  can_mo->can_mo_ptr->MOCTR  = CAN_MO_MOCTR_SETTXEN0_Msk |
                               CAN_MO_MOCTR_RESTXEN1_Msk;
}

/* Function to Initialize the Gateway Source Object */
void Cy_CAN_GATEWAY_InitSourceObject(const Cy_CAN_MO_t *const can_mo, const Cy_CAN_GATEWAY_CONFIG_t can_gateway)
{
  can_mo->can_mo_ptr->MOFCR = (((uint32_t)0x4U << CAN_MO_MOFCR_MMC_Pos) & (uint32_t)CAN_MO_MOFCR_MMC_Msk) |
                              ((((uint32_t)can_gateway.gateway_data_frame_send) << CAN_MO_MOFCR_GDFS_Pos) & (uint32_t)CAN_MO_MOFCR_GDFS_Msk) |
                              ((((uint32_t)can_gateway.gateway_data_length_code_copy) << CAN_MO_MOFCR_DLCC_Pos) & (uint32_t)CAN_MO_MOFCR_DLCC_Msk) |
                              ((((uint32_t)can_gateway.gateway_identifier_copy) << CAN_MO_MOFCR_IDC_Pos) & (uint32_t)CAN_MO_MOFCR_IDC_Msk) |
                              ((((uint32_t)can_gateway.gateway_data_copy) << CAN_MO_MOFCR_DATC_Pos) & (uint32_t)CAN_MO_MOFCR_DATC_Msk) ;
  can_mo->can_mo_ptr->MOFGPR = (uint32_t)((((uint32_t)can_gateway.gateway_bottom << CAN_MO_MOFGPR_BOT_Pos) & (uint32_t)CAN_MO_MOFGPR_BOT_Msk) |
                                          (((uint32_t)can_gateway.gateway_base << CAN_MO_MOFGPR_CUR_Pos) & (uint32_t)CAN_MO_MOFGPR_CUR_Msk) |
                                          (((uint32_t)can_gateway.gateway_top << CAN_MO_MOFGPR_TOP_Pos) & (uint32_t)CAN_MO_MOFGPR_TOP_Msk));
}

#endif /* CY_CAN_H */
