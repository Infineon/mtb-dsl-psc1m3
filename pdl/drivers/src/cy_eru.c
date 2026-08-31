/**
 * @file cy_eru.c
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

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include "cy_eru.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#define ERU_EXISEL_BITSIZE (4UL) /* Used to set the input for path A and path B based on the channel */
#define ERU_EXISEL_INPUT_BITSIZE (2UL)

#define CY_ERU_ETL_CHECK_INPUT_A(input) \
    ((input == CY_ERU_ETL_INPUT_A0) || \
     (input == CY_ERU_ETL_INPUT_A1) || \
     (input == CY_ERU_ETL_INPUT_A2) || \
     (input == CY_ERU_ETL_INPUT_A3))

#define CY_ERU_ETL_CHECK_INPUT_B(input) \
    ((input == CY_ERU_ETL_INPUT_B0) || \
     (input == CY_ERU_ETL_INPUT_B1) || \
     (input == CY_ERU_ETL_INPUT_B2) || \
     (input == CY_ERU_ETL_INPUT_B3))

#define CY_ERU_ETL_CHECK_STATUS_FLAG_MODE(mode) \
    ((mode == CY_ERU_ETL_STATUS_FLAG_MODE_SWCTRL) || \
     (mode == CY_ERU_ETL_STATUS_FLAG_MODE_HWCTRL))

#define CY_ERU_ETL_CHECK_EVENT_SOURCE(source) \
    ((source == CY_ERU_ETL_SOURCE_A)              || \
     (source == CY_ERU_ETL_SOURCE_B)              || \
     (source == CY_ERU_ETL_SOURCE_A_OR_B)         || \
     (source == CY_ERU_ETL_SOURCE_A_AND_B)        || \
     (source == CY_ERU_ETL_SOURCE_NOT_A)          || \
     (source == CY_ERU_ETL_SOURCE_NOT_A_OR_B)     || \
     (source == CY_ERU_ETL_SOURCE_NOT_A_AND_B)    || \
     (source == CY_ERU_ETL_SOURCE_NOT_B)          || \
     (source == CY_ERU_ETL_SOURCE_A_OR_NOT_B)     || \
     (source == CY_ERU_ETL_SOURCE_A_AND_NOT_B)    || \
     (source == CY_ERU_ETL_SOURCE_NOT_A_OR_NOT_B) || \
     (source == CY_ERU_ETL_SOURCE_NOT_A_AND_NOT_B))

#define CY_ERU_ETL_CHECK_TRIGGER_EDGE(edge) \
    ((edge == CY_ERU_ETL_EDGE_DETECTION_DISABLED) || \
     (edge == CY_ERU_ETL_EDGE_DETECTION_RISING) || \
     (edge == CY_ERU_ETL_EDGE_DETECTION_FALLING) || \
     (edge == CY_ERU_ETL_EDGE_DETECTION_BOTH))

#define CY_ERU_ETL_CHECK_TRIGGER_CHANNEL(channel) \
    ((channel == CY_ERU_ETL_OUTPUT_TRIGGER_CHANNEL0) || \
     (channel == CY_ERU_ETL_OUTPUT_TRIGGER_CHANNEL1) || \
     (channel == CY_ERU_ETL_OUTPUT_TRIGGER_CHANNEL2) || \
     (channel == CY_ERU_ETL_OUTPUT_TRIGGER_CHANNEL3))

#define CY_ERU_OGU_CHECK_PATTERN_INPUT(input) \
    ((input == CY_ERU_OGU_PATTERN_DETECTION_INPUT0) || \
     (input == CY_ERU_OGU_PATTERN_DETECTION_INPUT1) || \
     (input == CY_ERU_OGU_PATTERN_DETECTION_INPUT2) || \
     (input == CY_ERU_OGU_PATTERN_DETECTION_INPUT3))

#define CY_ERU_OGU_CHECK_PERIPHERAL_TRIGGER(trigger) \
    ((trigger == CY_ERU_OGU_PERIPHERAL_TRIGGER1) || \
     (trigger == CY_ERU_OGU_PERIPHERAL_TRIGGER2) || \
     (trigger == CY_ERU_OGU_PERIPHERAL_TRIGGER3))

#define CY_ERU_OGU_CHECK_SERIVCE_REQUEST(service) \
    ((service == CY_ERU_OGU_SERVICE_REQUEST_DISABLED) || \
     (service == CY_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER) || \
     (service == CY_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER_AND_PATTERN_MATCH) || \
     (service == CY_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER_AND_PATTERN_MISMATCH))

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/
/* Initializes the selected ERU_ETLx channel with the config structure.  */
void Cy_ERU_ETL_Init(Cy_ERU_t *const eru,
                      const uint8_t channel,
                      const Cy_ERU_ETL_CONFIG_t *const config)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));

  Cy_ERU_Enable(eru);

  eru->EXISEL = (eru->EXISEL &
                 ~((uint32_t)(ERU_EXISEL_EXS0A_Msk | ERU_EXISEL_EXS0B_Msk) << (channel * ERU_EXISEL_BITSIZE))) |
                (config->input << (channel * (uint32_t)ERU_EXISEL_BITSIZE));

  eru->EXICON[channel] = config->raw;
}

/* Initializes the selected ERU_OGUy channel with the config structure.  */
void Cy_ERU_OGU_Init(Cy_ERU_t *const eru,
                      const uint8_t channel,
                      const Cy_ERU_OGU_CONFIG_t *const config)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));

  Cy_ERU_Enable(eru);

  eru->EXOCON[channel] = config->raw;
}

/* Configures the event source for path A and path B, with selected input_a and input_b respectively.*/
void Cy_ERU_ETL_SetInput(Cy_ERU_t *const eru,
                          const uint8_t channel,
                          const Cy_ERU_ETL_INPUT_A_t input_a,
                          const Cy_ERU_ETL_INPUT_B_t input_b)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));
  CY_ASSERT(CY_ERU_ETL_CHECK_INPUT_A(input_a));
  CY_ASSERT(CY_ERU_ETL_CHECK_INPUT_B(input_b));

  eru->EXISEL = (eru->EXISEL & ~((uint32_t)(ERU_EXISEL_EXS0A_Msk | ERU_EXISEL_EXS0B_Msk) << (channel * ERU_EXISEL_BITSIZE))) |
                (((uint32_t)input_a | (uint32_t)(input_b << ERU_EXISEL_INPUT_BITSIZE))  << (channel * ERU_EXISEL_BITSIZE));
}

/* Select input path combination along with polarity for event generation by setting (SS, NA, NB) bits of
   EXICONx(x = [0 to 3]) register */
void Cy_ERU_ETL_SetSource(Cy_ERU_t *const eru,
                           const uint8_t channel,
                           const Cy_ERU_ETL_SOURCE_t source)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));
  CY_ASSERT(CY_ERU_ETL_CHECK_EVENT_SOURCE(source));

  eru->EXICON_b[channel].SS = (uint8_t)source;
}

/* Configure event trigger edge/s by setting (RE, FE) bits of EXICONx(x = [0 to 3]) register.*/
void Cy_ERU_ETL_SetEdgeDetection(Cy_ERU_t *const eru,
                                  const uint8_t channel,
                                  const Cy_ERU_ETL_EDGE_DETECTION_t edge_detection)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));
  CY_ASSERT(CY_ERU_ETL_CHECK_TRIGGER_EDGE(edge_detection));

  eru->EXICON_b[channel].ED = (uint8_t)edge_detection;
}

/* Returns the configured event trigger edge/s by reading (RE, FE) bits of EXICONx(x = [0 to 3]) register. */
Cy_ERU_ETL_EDGE_DETECTION_t Cy_ERU_ETL_GetEdgeDetection(Cy_ERU_t *const eru, const uint8_t channel)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));
  return ((Cy_ERU_ETL_EDGE_DETECTION_t)(eru->EXICON_b[channel].ED));
}

/* Set the status flag bit(FL) in EXICONx(x = [0 to 3]). */
void Cy_ERU_ETL_SetStatusFlagMode(Cy_ERU_t *const eru,
                                   const uint8_t channel,
                                   const Cy_ERU_ETL_STATUS_FLAG_MODE_t mode)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));
  CY_ASSERT(CY_ERU_ETL_CHECK_STATUS_FLAG_MODE(mode));

  eru->EXICON_b[channel].LD = (uint8_t)mode;
}

/* Configure which Channel of OGUy(Output gating unit y = [0 to 3]) to be mapped by the trigger pulse generated by
 * ETLx(Event Trigger Logic, x = [0 to 3]) by setting (OCS and PE) bit fields. */
void Cy_ERU_ETL_EnableOutputTrigger(Cy_ERU_t *const eru,
                                     const uint8_t channel,
                                     const Cy_ERU_ETL_OUTPUT_TRIGGER_CHANNEL_t trigger)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));
  CY_ASSERT(CY_ERU_ETL_CHECK_TRIGGER_CHANNEL(trigger));

  eru->EXICON_b[channel].OCS = (uint8_t)trigger;
  eru->EXICON_b[channel].PE = (uint8_t)true;
}

/* Disables the trigger pulse generation by clearing the (PE) of the EXICONx(x = [0 to 3]). */
void Cy_ERU_ETL_DisableOutputTrigger(Cy_ERU_t *const eru, const uint8_t channel)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));

  eru->EXICON_b[channel].PE = false;
}

/* Configures ERU_ETLx(x = [0 to 3]) for pattern match detection by setting IPENx(x = [0 to 3) and GEEN bits. */
void Cy_ERU_OGU_EnablePatternDetection(Cy_ERU_t *const eru,
                                        const uint8_t channel,
                                        const Cy_ERU_OGU_PATTERN_DETECTION_INPUT_t input)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));
  CY_ASSERT(CY_ERU_OGU_CHECK_PATTERN_INPUT(input));

  eru->EXOCON_b[channel].IPEN = (uint8_t)input;
  eru->EXOCON_b[channel].GEEN = true;
}

/* Disable the pattern detection by clearing (GEEN) bit. */
void Cy_ERU_OGU_DisablePatternDetection(Cy_ERU_t *const eru, const uint8_t channel)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));

  eru->EXOCON_b[channel].GEEN = false;
}

/* Configures peripheral trigger input, by setting (ISS) bit. */
void Cy_ERU_OGU_EnablePeripheralTrigger(Cy_ERU_t *const eru,
    const uint8_t channel,
    const Cy_ERU_OGU_PERIPHERAL_TRIGGER_t peripheral_trigger)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));
  CY_ASSERT(CY_ERU_OGU_CHECK_PERIPHERAL_TRIGGER(peripheral_trigger));

  eru->EXOCON_b[channel].ISS = (uint8_t)peripheral_trigger;
}

/* Disables event generation based on peripheral trigger by clearing (ISS) bit. */
void Cy_ERU_OGU_DisablePeripheralTrigger(Cy_ERU_t *const eru,
    const uint8_t channel)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));

  eru->EXOCON_b[channel].ISS = (uint8_t)0;
}

/* Configures the gating scheme for service request generation by setting (GP) bit. */
void Cy_ERU_OGU_SetServiceRequestMode(Cy_ERU_t *const eru,
                                       const uint8_t channel,
                                       const Cy_ERU_OGU_SERVICE_REQUEST_t mode)
{
  CY_ASSERT(CY_ERU_CHECK_MODULE_PTR(eru));
  CY_ASSERT((channel < 4U));
  CY_ASSERT(CY_ERU_OGU_CHECK_SERIVCE_REQUEST(mode));

  eru->EXOCON_b[channel].GP = (uint8_t)mode;

}

void Cy_ERU_Enable(Cy_ERU_t *const eru)
{
  CY_UNUSED_ARG(eru);
}

void Cy_ERU_Disable(Cy_ERU_t *const eru)
{
  CY_UNUSED_ARG(eru);
}
