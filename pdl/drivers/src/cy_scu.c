/**
 * @file cy_scu.c
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
 * @brief SCU low level driver API prototype definition for CY_FAMILY_PSC1 family of microcontrollers <br>
 *
 * <b>Detailed description of file</b> <br>
 * APIs provided in this file cover the following functional blocks of SCU: <br>
 * -- GCU (APIs prefixed with CY_SCU_GEN_) <br>
 * ----Temperature Monitoring, Voltage Monitoring, CCU Start etc
 *
 * -- CCU (APIs prefixed with CY_SCU_CLOCK_)<br>
 * ---- Clock initialization, Clock Gating, Sleep Management etc
 *
 * -- RCU (APIs prefixed with CY_SCU_RESET_) <br>
 * ---- Reset Init, Cause, Manual Reset Assert/Deassert etc
 *
 * -- INTERRUPT (APIs prefixed with CY_SCU_INTERRUPT_)<br>
 * ---- Initialization, Manual Assert/Deassert, Acknowledge etc
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_scu.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define SCU_IRQ_NUM             (3U)  /**< array index value for list of events that can generate SCU interrupt */
#define SCU_GCU_PASSWD_PROT_ENABLE  (195UL) /**< Password for enabling protection */
#define SCU_GCU_PASSWD_PROT_DISABLE (192UL) /**< Password for disabling protection */


#define CY_SCU_CHECK_RTCCLKSRC(source)       ( (source == CY_SCU_CLOCK_RTCCLKSRC_DCO2) || \
                                                (source == CY_SCU_CLOCK_RTCCLKSRC_ERU_IOUT0) || \
                                                (source == CY_SCU_CLOCK_RTCCLKSRC_ACMP0_OUT) || \
                                                (source == CY_SCU_CLOCK_RTCCLKSRC_ACMP1_OUT) || \
                                                (source == CY_SCU_CLOCK_RTCCLKSRC_ACMP2_OUT) ) /**< Used to verify
                                                                                                whether provided RTC
                                                                                                clock source is valid
                                                                                                or not */
#define SCU_GENERAL_INTCR_INTSEL_Msk SCU_GENERAL_INTCR0_INTSEL0_Msk /**< Mask value of Interrupt Source Select
                                                                         for Node 0 */
#define SCU_GENERAL_INTCR_INTSEL_Size SCU_GENERAL_INTCR0_INTSEL1_Pos /**< Bit position value of Interrupt Source Select
                                                                         for Node 1 */

#define ANA_TSE_T1   (0x10000F30U) /**< d is a constant data can be retrieved from Flash sector 0 to calculate OFFSET
                                         value for DCO calibration */
#define ANA_TSE_T2   (0x10000F31U) /**< e is a constant data can be retrieved from Flash sector 0 to calculate OFFSET
                                         value for DCO calibration */
#define DCO_ADJLO_T1 (0x10000F32U) /**< b is a constant data can be retrieved from Flash sector 0 to calculate OFFSET
                                         value for DCO calibration */
#define DCO_ADJLO_T2 (0x10000F33U) /**< a is a constant data can be retrieved from Flash sector 0 to calculate OFFSET
                                         value for DCO calibration */

#define CY_SCU_INTERRUPT_EVENT_MAX (64U) /**< Maximum supported SCU events. */

#define DCO1_DIV2_FREQUENCY_KHZ_Q22_10 (48000U << 10) /**< used to configures main clock (MCLK) frequency to requested
                                                         frequency value during runtime. */

#define ROM_BmiInstallationReq \
        (*((uint32_t (**)(uint32_t requestedBmiValue))0x00000108U)) /**< Pointer to Request BMI installation routine is
                                                                         available inside ROM. */

#define ROM_CalcTemperature \
        (*((uint32_t (**)(void))0x0000010cU)) /**<  Pointer to Calculate chip temperature routine is
                                                    available inside ROM. */

#define ROM_CalcTSEVAR \
        (*((uint32_t (**)(uint32_t temperature))0x00000120U)) /**<  Pointer to Calculate target level for temperature
                                                                    comparison routine is available inside ROM. */

/*********************************************************************************************************************
 * LOCAL DATA
 ********************************************************************************************************************/
static Cy_SCU_INTERRUPT_EVENT_HANDLER_t event_handler_list[CY_SCU_INTERRUPT_EVENT_MAX]; /**< For registering callback
                                                                                        functions on SCU event
                                                                                        occurrence. */

static Cy_SCU_INTERRUPT_EVENT_t event_masks[SCU_IRQ_NUM] =
{
  (CY_SCU_INTERRUPT_EVENT_FLASH_ERROR |
   CY_SCU_INTERRUPT_EVENT_FLASH_COMPLETED |
   CY_SCU_INTERRUPT_EVENT_PESRAM |
   CY_SCU_INTERRUPT_EVENT_PEUSIC0 |
#if defined(USIC1)
   CY_SCU_INTERRUPT_EVENT_PEUSIC1 |
#endif
#if defined(CAN)
   CY_SCU_INTERRUPT_EVENT_PEMCAN |
#endif
   CY_SCU_INTERRUPT_EVENT_LOSS_EXT_CLOCK |
   CY_SCU_INTERRUPT_EVENT_LOCI),

  (CY_SCU_INTERRUPT_EVENT_STDBYCLKFAIL |
   CY_SCU_INTERRUPT_EVENT_DCO1_OUT_SYNC |
   CY_SCU_INTERRUPT_EVENT_VDDPI |
   CY_SCU_INTERRUPT_EVENT_VDROP |
   CY_SCU_INTERRUPT_EVENT_VCLIP |
   CY_SCU_INTERRUPT_EVENT_TSE_DONE |
   CY_SCU_INTERRUPT_EVENT_TSE_HIGH |
   CY_SCU_INTERRUPT_EVENT_TSE_LOW |
   CY_SCU_INTERRUPT_EVENT_WDT_WARN |
   CY_SCU_INTERRUPT_EVENT_RTC_PERIODIC |
   CY_SCU_INTERRUPT_EVENT_RTC_ALARM |
   CY_SCU_INTERRUPT_EVENT_RTCCTR_UPDATED |
   CY_SCU_INTERRUPT_EVENT_RTCATIM0_UPDATED |
   CY_SCU_INTERRUPT_EVENT_RTCATIM1_UPDATED |
   CY_SCU_INTERRUPT_EVENT_RTCTIM0_UPDATED |
   CY_SCU_INTERRUPT_EVENT_RTCTIM1_UPDATED),
  (
    CY_SCU_INTERRUPT_EVENT_ORC0 |
    CY_SCU_INTERRUPT_EVENT_ORC1 |
    CY_SCU_INTERRUPT_EVENT_ORC2 |
    CY_SCU_INTERRUPT_EVENT_ORC3 |
    CY_SCU_INTERRUPT_EVENT_ORC4 |
    CY_SCU_INTERRUPT_EVENT_ORC5 |
    CY_SCU_INTERRUPT_EVENT_ORC6 |
    CY_SCU_INTERRUPT_EVENT_ORC7 |
#if defined(COMPARATOR)
    CY_SCU_INTERRUPT_EVENT_ACMP0 |
    CY_SCU_INTERRUPT_EVENT_ACMP1 |
    CY_SCU_INTERRUPT_EVENT_ACMP2 |
    CY_SCU_INTERRUPT_EVENT_ACMP3 |
#endif
    0)
}; /**<   Defines list of events that can generate SCU interrupt and also indicates SCU events mapping to corresponding
         service request number. These event mask values can be used to verify which event is triggered that corresponds
         to service request number during runtime. All the event items are tabulated as per service request sources list
         table of SCU. */

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/
/* Utility routine to perform frequency up scaling */
static void Cy_SCU_CLOCK_lFrequencyUpScaling(uint32_t curr_idiv, uint32_t idiv);

/* Utility routine to perform frequency down scaling */
static void Cy_SCU_CLOCK_lFrequencyDownScaling(uint32_t curr_idiv, uint32_t idiv);

/* Calculates the value which must be installed in ANATSEIx register to get indication in
   SCU_INTERRUPT->SRRAW  bit when the chip temperature is above/below some target/threshold. */
static uint32_t Cy_SCU_CalcTSEVAR(uint32_t temperature)
{
  uint32_t limit;

  CY_ASSERT((temperature >= 233U) && (temperature <= 388U));

  #if defined(__GNUC__)
  _Pragma("GCC diagnostic push")
  _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
  #endif
  limit = ROM_CalcTSEVAR(temperature);
  #if defined(__GNUC__)
  _Pragma("GCC diagnostic pop")
  #endif
  if (limit == 0U)
  {
    #if defined(__GNUC__)
    _Pragma("GCC diagnostic push")
    _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
    #endif
    limit = ROM_CalcTSEVAR(temperature + 1U);
    #if defined(__GNUC__)
    _Pragma("GCC diagnostic pop")
    #endif
    if (limit == 0U)
    {
      #if defined(__GNUC__)
      _Pragma("GCC diagnostic push")
      _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
      #endif
      limit = ROM_CalcTSEVAR(temperature - 1U);
      #if defined(__GNUC__)
      _Pragma("GCC diagnostic pop")
      #endif
    }
  }

  return (limit);
}

/* This is a local function used to generate the delay until register get updated with new configured value.  */
__STATIC_FORCEINLINE void delay(uint32_t cycles)
{
  while (--cycles > 0U)
  {
    __NOP();
  }
}

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/
/* Helper macros to include optional events only when the corresponding IP is present. */
#if defined(USIC1)
#define CY_SCU_INT_EV_USIC1_MATCH(event)  ((event) == CY_SCU_INTERRUPT_EVENT_PEUSIC1) ||
#else
#define CY_SCU_INT_EV_USIC1_MATCH(event)
#endif
#if defined(CAN)
#define CY_SCU_INT_EV_CAN_MATCH(event)    ((event) == CY_SCU_INTERRUPT_EVENT_PEMCAN) ||
#else
#define CY_SCU_INT_EV_CAN_MATCH(event)
#endif
#if defined(COMPARATOR)
#define CY_SCU_INT_EV_ACMP_MATCH(event)   ((event) == CY_SCU_INTERRUPT_EVENT_ACMP0) || \
                                          ((event) == CY_SCU_INTERRUPT_EVENT_ACMP1) || \
                                          ((event) == CY_SCU_INTERRUPT_EVENT_ACMP2) || \
                                          ((event) == CY_SCU_INTERRUPT_EVENT_ACMP3) ||
#else
#define CY_SCU_INT_EV_ACMP_MATCH(event)
#endif

/* Macro to verify SCU event weather it is valid event or not */
#define Cy_SCU_INTERRUPT_IsValidEvent(event) ( \
            ((event) == CY_SCU_INTERRUPT_EVENT_WDT_WARN) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_RTC_PERIODIC) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_RTC_ALARM) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_VDDPI) || \
            CY_SCU_INT_EV_USIC1_MATCH(event) \
            CY_SCU_INT_EV_CAN_MATCH(event) \
            ((event) == CY_SCU_INTERRUPT_EVENT_LOSS_EXT_CLOCK) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_DCO1_OUT_SYNC) || \
            CY_SCU_INT_EV_ACMP_MATCH(event) \
            ((event) == CY_SCU_INTERRUPT_EVENT_VDROP) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_ORC0) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_ORC1) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_ORC2) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_ORC3) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_ORC4) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_ORC5) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_ORC6) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_ORC7) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_LOCI) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_PESRAM) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_PEUSIC0) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_FLASH_ERROR) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_FLASH_COMPLETED) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_VCLIP) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_STDBYCLKFAIL) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_RTCCTR_UPDATED) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_RTCATIM0_UPDATED) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_RTCATIM1_UPDATED) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_RTCTIM0_UPDATED) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_RTCTIM1_UPDATED) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_TSE_DONE) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_TSE_HIGH) || \
            ((event) == CY_SCU_INTERRUPT_EVENT_TSE_LOW))

/* API to enable the SCU event */
void Cy_SCU_INTERRUPT_EnableEvent(const Cy_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRMSK |= (uint32_t)event;
  SCU_INTERRUPT->SRMSK1 |= (uint32_t)(event >> 32U);
}

/* API to disable the SCU event */
void Cy_SCU_INTERRUPT_DisableEvent(const Cy_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRMSK &= ~(uint32_t)event;
  SCU_INTERRUPT->SRMSK1 &= (uint32_t)~(event >> 32U);
}

/* API to trigger the SCU event */
void Cy_SCU_INTERRUPT_TriggerEvent(const Cy_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRSET |= (uint32_t)event;
  SCU_INTERRUPT->SRSET1 |= (uint32_t)(event >> 32U);
}

/* API to get the SCU event status */
Cy_SCU_INTERRUPT_EVENT_t Cy_SCU_INTERRUPT_GetEventStatus(void)
{
  Cy_SCU_INTERRUPT_EVENT_t tmp;

  tmp = SCU_INTERRUPT->SRRAW;
  tmp |= ((int64_t)SCU_INTERRUPT->SRRAW1 << 32U);
  return (tmp);
}

/* API to clear the SCU event status */
void Cy_SCU_INTERRUPT_ClearEventStatus(const Cy_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRCLR |= (uint32_t)event;
  SCU_INTERRUPT->SRCLR1 |= (uint32_t)(event >> 32U);
}

/* API to lock protected bitfields from being modified */
void Cy_SCU_LockProtectedBits(void)
{
  SCU_GENERAL->PASSWD = SCU_GCU_PASSWD_PROT_ENABLE;
}

/* API to make protected bitfields available for modification */
void Cy_SCU_UnlockProtectedBits(void)
{
  SCU_GENERAL->PASSWD = SCU_GCU_PASSWD_PROT_DISABLE;

  while (((SCU_GENERAL->PASSWD) & SCU_GENERAL_PASSWD_PROTS_Msk))
  {
    /* Loop until the lock is removed */
  }
}

/* API to initialize power supply monitoring unit */
void Cy_SCU_SupplyMonitorInit(const Cy_SCU_SUPPLYMONITOR_t *obj)
{
  uint32_t anavdel;
  uint32_t irqmask;

  anavdel = 0UL;

  anavdel |= (uint32_t)((obj-> ext_supply_threshold) << SCU_ANALOG_ANAVDEL_VDEL_SELECT_Pos);
  anavdel |= (uint32_t)((obj->ext_supply_monitor_speed) << SCU_ANALOG_ANAVDEL_VDEL_TIM_ADJ_Pos);

  if (true == (obj->enable_at_init))
  {
    anavdel |= (uint32_t)SCU_ANALOG_ANAVDEL_VDEL_EN_Msk;
  }
  SCU_ANALOG->ANAVDEL = (uint16_t) anavdel;

  irqmask = 0UL;

  if (true == (obj->enable_prewarning_int))
  {
    irqmask |= (uint32_t)SCU_INTERRUPT_SRMSK_VDDPI_Msk;
  }

  if (true == (obj->enable_vdrop_int))
  {
    irqmask |= (uint32_t)SCU_INTERRUPT_SRMSK_VDROPI_Msk;
  }

  if (true == (obj->enable_vclip_int))
  {
    irqmask |= (uint32_t)SCU_INTERRUPT_SRMSK_VCLIPI_Msk;
  }
  SCU_INTERRUPT->SRMSK |= (uint32_t)irqmask;
}

/* API to program lower temperature limit */
Cy_SCU_STATUS_t Cy_SCU_SetTempLowLimit(uint32_t limit)
{
  Cy_SCU_STATUS_t status = CY_SCU_STATUS_OK;

  limit = Cy_SCU_CalcTSEVAR(limit);

  if (limit != 0)
  {
    SCU_ANALOG->ANATSEIL = (uint16_t)limit;
  }
  else
  {
    status = CY_SCU_STATUS_ERROR;
  }

  return (status);
}

/* API to program higher temperature limit */
Cy_SCU_STATUS_t Cy_SCU_SetTempHighLimit(uint32_t limit)
{
  Cy_SCU_STATUS_t status = CY_SCU_STATUS_OK;

  limit = Cy_SCU_CalcTSEVAR(limit);

  if (limit != 0)
  {
    SCU_ANALOG->ANATSEIH = (uint16_t)limit;
  }
  else
  {
    status = CY_SCU_STATUS_ERROR;
  }

  return (status);
}

/* API to program temperature limits as raw digital values into temperature sensor */
void Cy_SCU_SetRawTempLimits(const uint32_t lower_temp, const uint32_t upper_temp)
{
  SCU_ANALOG->ANATSEIH = (uint16_t)(upper_temp & SCU_ANALOG_ANATSEIH_TSE_IH_Msk);
  SCU_ANALOG->ANATSEIL = (uint16_t)(lower_temp & SCU_ANALOG_ANATSEIL_TSE_IL_Msk);
}

/* API to start temperature measurement */
void Cy_SCU_StartTempMeasurement(void)
{
  SCU_ANALOG->ANATSECTRL |= (uint16_t)SCU_ANALOG_ANATSECTRL_TSE_EN_Msk;
}

/* API to stop temperature measurement */
void Cy_SCU_StopTempMeasurement(void)
{
  SCU_ANALOG->ANATSECTRL &= (uint16_t)~SCU_ANALOG_ANATSECTRL_TSE_EN_Msk;
}

/* API to check if the temperature has gone past the ceiling */
bool Cy_SCU_HighTemperature(void)
{
  return ((SCU_INTERRUPT->SRRAW & SCU_INTERRUPT_SRRAW_TSE_HIGH_Msk) == SCU_INTERRUPT_SRRAW_TSE_HIGH_Msk);
}

/* API to check if the temperature is lower than normal */
bool Cy_SCU_LowTemperature(void)
{
  return ((SCU_INTERRUPT->SRRAW & SCU_INTERRUPT_SRRAW_TSE_LOW_Msk) == SCU_INTERRUPT_SRRAW_TSE_LOW_Msk);
}

/* API to retrieve the device temperature */
uint32_t Cy_SCU_GetTemperature(void)
{
  uint32_t temperature;

  temperature = (uint32_t)(SCU_ANALOG->ANATSEMON);

  return (temperature);
}

/* Calculates the die temperature value using ROM function */
uint32_t Cy_SCU_CalcTemperature(void)
{
  #if defined(__GNUC__)
  _Pragma("GCC diagnostic push")
  _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
  #endif
  return (ROM_CalcTemperature());
  #if defined(__GNUC__)
  _Pragma("GCC diagnostic pop")
  #endif
}


/* API which initializes the clock tree ofthe device */
void Cy_SCU_CLOCK_Init(const Cy_SCU_CLOCK_CONFIG_t *const config)
{
  /* Remove protection */
  Cy_SCU_UnlockProtectedBits();

  /* OSCHP source selection - OSC mode */
  if (config->oschp_mode != CY_SCU_CLOCK_OSCHP_MODE_DISABLED)
  {
    if (OSCHP_GetFrequency() > 20000000U)
    {
      SCU_ANALOG->ANAOSCHPCTRL |= (uint16_t)SCU_ANALOG_ANAOSCHPCTRL_HYSCTRL_Msk;
    }

    SCU_ANALOG->ANAOSCHPCTRL = (uint16_t)(SCU_ANALOG->ANAOSCHPCTRL & ~(SCU_ANALOG_ANAOSCHPCTRL_SHBY_Msk | SCU_ANALOG_ANAOSCHPCTRL_MODE_Msk)) |
                               config->oschp_mode;

    do
    {
      /* clear the status bit before restarting the detection. */
      SCU_INTERRUPT->SRCLR1 = SCU_INTERRUPT_SRCLR1_LOECI_Msk;

      /* According to errata SCU_CM.023, to reset the XOWD it is needed to disable/enable the watchdog,
         keeping in between at least one DCO2 cycle */

      /* Disable XOWD */
      SCU_CLK->OSCCSR &= ~SCU_CLK_OSCCSR_XOWDEN_Msk;

      /* Clock domains synchronization, at least 1 DCO2 cycle */
      /* delay value calculation assuming worst case DCO1=48Mhz and 3cycles per delay iteration */
      delay(538);

      /* Enable XOWD */
      SCU_CLK->OSCCSR |= SCU_CLK_OSCCSR_XOWDEN_Msk | SCU_CLK_OSCCSR_XOWDRES_Msk;

      /* OSCCSR.XOWDRES bit will be automatically reset to 0 after XOWD is reset */
      while (SCU_CLK->OSCCSR & SCU_CLK_OSCCSR_XOWDRES_Msk);

      /* Wait a at least 5 DCO2 cycles for the update of the XTAL OWD result */
      /* delay value calculation assuming worst case DCO1=48Mhz and 3cycles per delay iteration */
      delay(2685);

    }
    while (SCU_INTERRUPT->SRRAW1 & SCU_INTERRUPT_SRRAW1_LOECI_Msk);
  }
  else /* (config->oschp_mode == CY_SCU_CLOCK_OSCHP_MODE_DISABLED) */
  {
    SCU_ANALOG->ANAOSCHPCTRL |= SCU_ANALOG_ANAOSCHPCTRL_MODE_Msk;
  }

  SCU_ANALOG->ANAOSCLPCTRL = (uint16_t)config->osclp_mode;
#ifndef DISABLE_WAIT_RTC_XTAL_OSC_STARTUP
  if (config->osclp_mode == CY_SCU_CLOCK_OSCLP_MODE_OSC)
  {
    /* Wait oscillator startup time ~5s */
    delay(6500000);
  }
#endif

  SCU_CLK->CLKCR1 = (SCU_CLK->CLKCR1 & ~SCU_CLK_CLKCR1_DCLKSEL_Msk) |
                    config->dclk_src;

  /* Update PCLK selection mux. */
  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_PCLKSEL_Msk | SCU_CLK_CLKCR_RTCCLKSEL_Msk)) |
                   config->rtc_src |
                   config->pclk_src;

  /* Close the lock opened above. */
  Cy_SCU_LockProtectedBits();

  /* Update the dividers now */
  Cy_SCU_CLOCK_ScaleMCLKFrequency(config->idiv, config->fdiv);

}

/* API which selects one of the available parent clock nodes for a given child clock node */
void Cy_SCU_CLOCK_SetRtcClockSource(const Cy_SCU_CLOCK_RTCCLKSRC_t source)
{
  CY_ASSERT(CY_SCU_CHECK_RTCCLKSRC(source));

  Cy_SCU_UnlockProtectedBits();

  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~SCU_CLK_CLKCR_RTCCLKSEL_Msk) |
                   source;

  Cy_SCU_LockProtectedBits();
}

/* API to program the divider placed between fperiph and its parent */
void Cy_SCU_CLOCK_SetFastPeripheralClockSource(const Cy_SCU_CLOCK_PCLKSRC_t source)
{
  Cy_SCU_UnlockProtectedBits();

  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~SCU_CLK_CLKCR_PCLKSEL_Msk) |
                   source;

  Cy_SCU_LockProtectedBits();
}

void Cy_SCU_CLOCK_SetAdcClockSrc(Cy_SCU_CLOCK_ADCCLKSRC_t adcclk_src)
{
  Cy_SCU_UnlockProtectedBits();

  SCU_CLK->CLKCR1 = (SCU_CLK->CLKCR1 & ~SCU_CLK_CLKCR1_ADCCLKSEL_Msk) | adcclk_src;

  Cy_SCU_LockProtectedBits();
}

/* API which gates a clock node at its source */
void Cy_SCU_CLOCK_GatePeripheralClock(const Cy_SCU_PERIPHERAL_CLOCK_t peripheral)
{
  Cy_SCU_UnlockProtectedBits();
  SCU_CLK->CGATSET0 |= (uint32_t)peripheral;
  Cy_SCU_LockProtectedBits();
}

/* API which ungates a clock note at its source */
void Cy_SCU_CLOCK_UngatePeripheralClock(const Cy_SCU_PERIPHERAL_CLOCK_t peripheral)
{
  Cy_SCU_UnlockProtectedBits();
  SCU_CLK->CGATCLR0 |= (uint32_t)peripheral;
  while ((SCU_CLK->CLKCR) & SCU_CLK_CLKCR_VDDC2LOW_Msk)
  {
    /* Wait voltage suply stabilization */
  }
  Cy_SCU_LockProtectedBits();
}

/* Checks the status of peripheral clock gating */
bool Cy_SCU_CLOCK_IsPeripheralClockGated(const Cy_SCU_PERIPHERAL_CLOCK_t peripheral)
{
  return (bool)((SCU_CLK->CGATSTAT0 & peripheral) != 0);
}

/* This API configures main clock (MCLK) frequency to requested frequency value. */
void Cy_SCU_CLOCK_SetMCLKFrequency(uint32_t freq_khz)
{
  uint32_t ratio;
  uint32_t ratio_int;
  uint32_t ratio_frac;

  if (((SCU_CLK->CLKCR1) & SCU_CLK_CLKCR1_DCLKSEL_Msk) == 0U)
  {
    ratio = DCO1_DIV2_FREQUENCY_KHZ_Q22_10 / freq_khz;
  }
  else
  {
    ratio = ((OSCHP_GetFrequency() / 1000U) << 10U) / freq_khz;
  }

  /* Manage overflow */
  if (ratio > 0xffffffU)
  {
    ratio = 0xffffffU;
  }

  ratio_int = ratio >> 10U;
  ratio_frac = ratio & 0x3ffU;

  Cy_SCU_CLOCK_ScaleMCLKFrequency(ratio_int, ratio_frac);
}


/* A utility routine which updates the fractional dividers in steps */
void Cy_SCU_CLOCK_ScaleMCLKFrequency(uint32_t idiv, uint32_t fdiv)
{
  /* Find out current and target value of idiv */
  uint32_t curr_idiv;

  Cy_SCU_UnlockProtectedBits();

  /* Take a snapshot of value already programmed into IDIV */
  curr_idiv = (SCU_CLK->CLKCR & SCU_CLK_CLKCR_IDIV_Msk) >> SCU_CLK_CLKCR_IDIV_Pos;

  SCU_CLK->CLKCR1 = (SCU_CLK->CLKCR1 & (uint32_t)~(SCU_CLK_CLKCR1_FDIV_Msk)) |
                    (uint32_t)((fdiv >> 8U) << SCU_CLK_CLKCR1_FDIV_Pos);

  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_FDIV_Msk | SCU_CLK_CLKCR_CNTADJ_Msk)) |
                   (uint32_t)((fdiv & 0xffU) << SCU_CLK_CLKCR_FDIV_Pos) |
                   (uint32_t)(1023UL << SCU_CLK_CLKCR_CNTADJ_Pos);

  while ((SCU_CLK->CLKCR)& SCU_CLK_CLKCR_VDDC2LOW_Msk)
  {
    /* Spin until the core supply stabilizes */
  }

  if (curr_idiv <= idiv)
  {
    /* Requested IDIV is greater than currently programmed IDIV. So downscale the frequency */
    Cy_SCU_CLOCK_lFrequencyDownScaling(curr_idiv, idiv);
  }
  else
  {
    /* Requested IDIV is lower than currently programmed IDIV. So upscale the frequency */
    Cy_SCU_CLOCK_lFrequencyUpScaling(curr_idiv, idiv);
  }

  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_IDIV_Msk | SCU_CLK_CLKCR_CNTADJ_Msk)) |
                   (uint32_t)(idiv << SCU_CLK_CLKCR_IDIV_Pos) | (uint32_t)(1023UL << SCU_CLK_CLKCR_CNTADJ_Pos);

  while ((SCU_CLK->CLKCR) & SCU_CLK_CLKCR_VDDC2LOW_Msk)
  {
    /* Wait voltage suply stabilization */
  }

  Cy_SCU_LockProtectedBits();

  SystemCoreClockUpdate();

}

/* Utility routine to perform frequency up scaling */
static void Cy_SCU_CLOCK_lFrequencyUpScaling(uint32_t curr_idiv, uint32_t target_idiv)
{
  while (curr_idiv > (target_idiv * 4UL))
  {
    curr_idiv = (uint32_t)(curr_idiv >> 2UL);   /* Divide by 4. */

    SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_IDIV_Msk | SCU_CLK_CLKCR_CNTADJ_Msk)) |
                     (uint32_t)(curr_idiv << SCU_CLK_CLKCR_IDIV_Pos) |
                     (uint32_t)(1023UL << SCU_CLK_CLKCR_CNTADJ_Pos);

    while (SCU_CLK->CLKCR & SCU_CLK_CLKCR_VDDC2LOW_Msk)
    {
      /* Wait voltage suply stabilization */
    }
  }
}

/* Utility routine to perform frequency down scaling */
static void Cy_SCU_CLOCK_lFrequencyDownScaling(uint32_t curr_idiv, uint32_t target_idiv)
{

  while ((curr_idiv * 4UL) < target_idiv)
  {
    if (0U == curr_idiv)
    {
      curr_idiv = 1U;
    }
    curr_idiv  = (uint32_t)(curr_idiv << 2UL);   /* Multiply by 4. */
    SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_IDIV_Msk | SCU_CLK_CLKCR_CNTADJ_Msk)) |
                     (uint32_t)(curr_idiv << SCU_CLK_CLKCR_IDIV_Pos) |
                     (uint32_t)(1023UL << SCU_CLK_CLKCR_CNTADJ_Pos);

    while (SCU_CLK->CLKCR & SCU_CLK_CLKCR_VDDC2LOW_Msk)
    {
      /* Wait voltage suply stabilization */
    }
  }
}

/*
 * API to retrieve clock frequency of peripherals on the peripheral bus using a shared functional clock
 */
uint32_t Cy_SCU_CLOCK_GetPeripheralClockFrequency(void)
{
  return (SystemCoreClock);
}

/* Provides the clock frequency of peripherals on the peripheral bus that are using a shared functional clock */
uint32_t Cy_SCU_CLOCK_GetFastPeripheralClockFrequency(void)
{
  return (SystemCoreClock << ((SCU_CLK->CLKCR & SCU_CLK_CLKCR_PCLKSEL_Msk) >> SCU_CLK_CLKCR_PCLKSEL_Pos));
}

/* DCO1 clock frequency can be calibrated during runtime to achieve a better accuracy */
void Cy_SCU_CLOCK_CalibrateOscillatorOnTemperature(int32_t temperature)
{
  int32_t a;
  int32_t b;
  int32_t d;
  int32_t e;
  int32_t offset;

  a = *((uint8_t *)DCO_ADJLO_T2);
  b = *((uint8_t *)DCO_ADJLO_T1);
  d = *((uint8_t *)ANA_TSE_T1);
  e = *((uint8_t *)ANA_TSE_T2);

  offset = b + (((a - b) * (temperature - d)) / (e - d));
  offset &= SCU_ANALOG_ANAOFFSET_ADJL_OFFSET_Msk;

  Cy_SCU_UnlockProtectedBits();
  SCU_ANALOG->ANAOFFSET = (uint16_t)offset;
  Cy_SCU_LockProtectedBits();
}

/*
 * API to assign the event handler function to be executed on occurrence of the selected event
 */
Cy_SCU_STATUS_t Cy_SCU_INTERRUPT_SetEventHandler(Cy_SCU_INTERRUPT_EVENT_t event, Cy_SCU_INTERRUPT_EVENT_HANDLER_t handler)
{
  uint32_t index;
  Cy_SCU_STATUS_t status;

  CY_ASSERT(Cy_SCU_INTERRUPT_IsValidEvent(event));
  CY_ASSERT(handler != NULL);

  index = 0U;

  while (((event & ((Cy_SCU_INTERRUPT_EVENT_t)1 << index)) == 0U) && (index < CY_SCU_INTERRUPT_EVENT_MAX))
  {
    index++;
  }

  if (index == CY_SCU_INTERRUPT_EVENT_MAX)
  {
    status = CY_SCU_STATUS_ERROR;
  }
  else
  {
    event_handler_list[index] = handler;
    status = CY_SCU_STATUS_OK;
  }

  return (status);
}

/*
 * A common function to execute callback functions for multiple events
 */
void Cy_SCU_IRQHandler(uint32_t sr_num)
{
  CY_ASSERT(sr_num < SCU_IRQ_NUM);

  uint32_t index;
  Cy_SCU_INTERRUPT_EVENT_t event;
  Cy_SCU_INTERRUPT_EVENT_HANDLER_t event_handler;

  index = 0U;
  event = Cy_SCU_INTERRUPT_GetEventStatus() & event_masks[sr_num];
  Cy_SCU_INTERRUPT_ClearEventStatus(event);

  while ((event != 0) && (index < CY_SCU_INTERRUPT_EVENT_MAX))
  {
    if ((event & ((Cy_SCU_INTERRUPT_EVENT_t)1 << index)) != 0U)
    {
      event &= ~((Cy_SCU_INTERRUPT_EVENT_t)1 << index);
      event_handler = event_handler_list[index];
      if (event_handler != NULL)
      {
        event_handler();
      }

      /* break; CY_FAMILY_PSC1: Only PULSE interrupts */
    }
    index++;
  }
}

/* Install BMI */
uint32_t Cy_SCU_SetBMI(uint32_t flags, uint8_t timeout)
{
  #if defined(__GNUC__)
  _Pragma("GCC diagnostic push")
  _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
  #endif
  return ROM_BmiInstallationReq((flags & 0x0fffU) | ((timeout << 12) & 0xf000U));
  #if defined(__GNUC__)
  _Pragma("GCC diagnostic pop")
  #endif
}

/* DCO1 clock frequency can be calibrated during runtime to achieve a better accuracy */
void Cy_SCU_CLOCK_EnableDCO1ExtRefCalibration(Cy_SCU_CLOCK_SYNC_CLKSRC_t sync_clk, uint32_t prescaler, uint32_t syn_preload)
{

  Cy_SCU_UnlockProtectedBits();
  SCU_ANALOG->ANASYNC2 = (uint16_t)(prescaler << SCU_ANALOG_ANASYNC2_PRESCALER_Pos);
  SCU_ANALOG->ANASYNC1 = (uint16_t)(syn_preload |
                                    sync_clk |
                                    SCU_ANALOG_ANASYNC1_SYNC_DCO_EN_Msk);

  Cy_SCU_LockProtectedBits();

}

/* This function stops the automatic DCO1 calibration based on the selected clock source */
void Cy_SCU_CLOCK_DisableDCO1ExtRefCalibration(void)
{
  Cy_SCU_UnlockProtectedBits();
  SCU_ANALOG->ANASYNC2 = 0U;
  SCU_ANALOG->ANASYNC1 = 0U;
  Cy_SCU_LockProtectedBits();
}

/* This functions checks the status of the synchronisation */
bool Cy_SCU_CLOCK_IsDCO1ExtRefCalibrationReady(void)
{
  return (bool)((SCU_ANALOG->ANASYNC2 & SCU_ANALOG_ANASYNC2_SYNC_READY_Msk) != 0U);
}

/* This function selects service request source for a NVIC interrupt node */
void Cy_SCU_SetInterruptControl(uint8_t irq_number, Cy_SCU_IRQCTRL_t source)
{
  CY_ASSERT(irq_number < 32);
  CY_ASSERT((source >> 8) == irq_number);

  source &= 0x3U;
  if (irq_number < 16U)
  {
    SCU_GENERAL->INTCR0 = (SCU_GENERAL->INTCR0 & ~(SCU_GENERAL_INTCR_INTSEL_Msk << (irq_number * SCU_GENERAL_INTCR_INTSEL_Size))) |
                          (source << (irq_number * SCU_GENERAL_INTCR_INTSEL_Size));
  }
  else
  {
    irq_number &= 0x0fU;
    SCU_GENERAL->INTCR1 = (SCU_GENERAL->INTCR1 & ~(SCU_GENERAL_INTCR_INTSEL_Msk << (irq_number  * SCU_GENERAL_INTCR_INTSEL_Size))) |
                          (source << (irq_number  * SCU_GENERAL_INTCR_INTSEL_Size));
  }
}

void Cy_SCU_CLOCK_SetHighPerformanceOscillatorMode(Cy_SCU_CLOCK_OSCHP_MODE_t mode)
{
  Cy_SCU_UnlockProtectedBits();
  SCU_ANALOG->ANAOSCHPCTRL = (SCU_ANALOG->ANAOSCHPCTRL & (uint32_t)~(SCU_ANALOG_ANAOSCHPCTRL_MODE_Msk | SCU_ANALOG_ANAOSCHPCTRL_SHBY_Msk))
                             | mode;
  Cy_SCU_LockProtectedBits();
}

void Cy_SCU_CLOCK_SetLowPerformanceOscillatorMode(Cy_SCU_CLOCK_OSCLP_MODE_t mode)
{
  SCU_ANALOG->ANAOSCLPCTRL = mode;
}
