/**
 * @file cy_acmp.h
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

#ifndef CY_ACMP_H
#define CY_ACMP_H


/**
 * @addtogroup ACMP
 * @brief Analog Comparator(ACMP) low level driver for CY_FAMILY_PSC1 family of microcontrollers. <br>
 *
 * The ACMP module consists of minimum of 3 analog comparators. Each analog comparator has two inputs, INP and INN.
 * Input INP is compared with input INN in the pad voltage domain.
 * It generates a digital comparator output signal. The digital comparator output signal is shifted down from VDDP
 * power supply voltage level to VDDC core voltage level. The ACMP module provides the following functionalities.\n
 * -# Monitor external voltage level
 * -# Operates in low power mode
 * -# Provides Inverted ouput option\n

 * \par The ACMP low level driver funtionalities
 * <OL>
 * <LI>Initializes an instance of analog comparator module with the @ref Cy_ACMP_CONFIG_t configuration structure
 * using the API Cy_ACMP_Init().</LI>
 * <LI>Programs the source of input(INP) specified by @ref Cy_ACMP_INP_SOURCE_t parameter using the API
 * Cy_ACMP_SetInput(). </LI>
 * <LI>Sets the low power mode of operation using Cy_ACMP_SetLowPowerMode() API.</LI>
 * </OL>
 * @{
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_common.h"

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
/* If ACMP is available*/
#if defined (COMPARATOR) || defined(CY_DOXYGEN)

#define CY_ACMP0 (Cy_ACMP_t*)COMPARATOR /**< Comparator module base address defined*/

#define CY_ACMP_MAX_INSTANCES     (4U) /* Maximum number of Analog Comparators available*/

/* Checks if the pointer being passed is valid*/
#define CY_ACMP_CHECK_MODULE_PTR(PTR)  (((PTR)== (Cy_ACMP_t*)COMPARATOR))

/* Checks if the instance being addressed is valid*/
#define CY_ACMP_CHECK_INSTANCE(INST)   (((INST)< CY_ACMP_MAX_INSTANCES))

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/**
 * Defines the return value of an API.
 */
typedef enum CY_ACMP_STATUS
{
  CY_ACMP_STATUS_SUCCESS = 0U, /**< API completes the execution successfully */
  CY_ACMP_STATUS_ERROR,        /**< API cannot fulfill the request */
} Cy_ACMP_STATUS_t;

/**
 * Defines the hysteresis voltage levels to reduce noise sensitivity.
 */
typedef enum CY_ACMP_HYSTERESIS
{
  CY_ACMP_HYSTERESIS_OFF = 0U, /**< No hysteresis */
  CY_ACMP_HYSTERESIS_10,       /**< Hysteresis = 10mv */
  CY_ACMP_HYSTERESIS_15,       /**< Hysteresis = 15mv */
  CY_ACMP_HYSTERESIS_20        /**< Hysteresis = 20mv */
} Cy_ACMP_HYSTERESIS_t;

/**
 *  Defines the comparator output status options.
 */
typedef enum CY_ACMP_COMP_OUT
{
  CY_ACMP_COMP_OUT_NO_INVERSION = 0U, /**< ACMP output is HIGH when, Input Positive(INP) greater than Input
                                            Negative(INN). Vplus > Vminus */
  CY_ACMP_COMP_OUT_INVERSION          /**< ACMP output is HIGH when, Input Negative(INN) greater than Input
                                            Positive(INP). Vminus > Vplus*/
} Cy_ACMP_COMP_OUT_t;

/**
 *  Defines the analog comparator input connection method.
 */
typedef enum CY_ACMP_INP_SOURCE
{
  CY_ACMP_INP_SOURCE_STANDARD_PORT  = 0U,                                          /**< Input is connected to port */
  CY_ACMP_INP_SOURCE_ACMP1_INP_PORT = (uint16_t)(COMPARATOR_ANACMP0_ACMP0_SEL_Msk) /**< Input is connected to port
                                                                                     and ACMP1 INP */
} Cy_ACMP_INP_SOURCE_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/


/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined(__TASKING__)
#pragma warning 586
#endif

/**
 * ACMP module
 */
typedef struct CY_ACMP
{
  __IO uint32_t  ORCCTRL;
  __I  uint32_t  RESERVED[726];
  __IO uint32_t  ANACMP[CY_ACMP_MAX_INSTANCES];
} Cy_ACMP_t;

/**
 *  Structure for initializing the ACMP module. It configures the ANACMP register of the respective input.
 */
typedef struct CY_ACMP_CONFIG
{
  union
  {
    struct
    {
      uint32_t                  : 1;
      uint32_t filter_disable   : 1; /**< Comparator filter option for removing glitches. By default this option
                                          is selected in ANACMP register. Setting this option disables the filter */
      uint32_t                  : 1;
      uint32_t output_invert    : 1; /**< Option to invert the comparator output. Use Cy_@ref Cy_ACMP_COMP_OUT_t type*/
      uint32_t hysteresis       : 2; /**< Hysteresis voltage to reduce noise sensitivity. Select the voltage levels
                                          from the values defined in @ref Cy_ACMP_HYSTERESIS_t. */
      uint32_t                  : 26;
    };
    uint32_t anacmp;
  };
} Cy_ACMP_CONFIG_t;

/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
#pragma pop
#elif defined(__TASKING__)
#pragma warning restore
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************************
 * API Prototypes
 ********************************************************************************************************************/

/**
 * @param peripheral Constant pointer to analog comparator module, of @ref Cy_ACMP_t type. Use @ref CY_ACMP0 macro.
 * @param instance ACMP instance number. <BR>
 *                 Range:<BR> 0 - ACMP0<BR>
 *                            1 - ACMP1<BR>
 *                            2 - ACMP2<BR>
 *                            3 - ACMP3<BR>
 *
 * @param config Pointer to configuration data. Refer data structure @ref Cy_ACMP_CONFIG_t for settings.
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Initializes an instance of analog comparator module.<BR>\n
 *  Configures the ANACMP resister with hysteresis, comparator filter and inverted comparator output.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
void Cy_ACMP_Init(Cy_ACMP_t *const peripheral, uint32_t instance, const Cy_ACMP_CONFIG_t *const config);

/**
 * @param peripheral Constant pointer to analog comparator module, of @ref Cy_ACMP_t type. Use @ref CY_ACMP0 macro.
 * @param instance ACMP instance number. <BR>
 *                 Range:<BR> 0 - ACMP0<BR>
 *                            1 - ACMP1<BR>
 *                            2 - ACMP2<BR>
 *                            3 - ACMP3<BR>
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables an instance of ACMP module.<BR>\n
 * Starts the comparator by setting CMP_EN bit of respective ANACMP \a instance register. The \a instance number
 * determines which analog comparator to be switched on. Call this API after the successful completion of the comparator
 * initilization and input selection.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_ACMP_DisableComparator().<BR>
 */
__STATIC_INLINE void Cy_ACMP_EnableComparator(Cy_ACMP_t *const peripheral, uint32_t instance)
{
  CY_ASSERT(CY_ACMP_CHECK_MODULE_PTR(peripheral));
  CY_ASSERT(CY_ACMP_CHECK_INSTANCE(instance));

  peripheral->ANACMP[instance] |= (uint16_t)COMPARATOR_ANACMP0_CMP_EN_Msk;

}


/**
 * @param peripheral Constant pointer to analog comparator module, of @ref Cy_ACMP_t type. Use @ref CY_ACMP0 macro.
 * @param instance ACMP instance number. <BR>
 *                 Range:<BR> 0 - ACMP0<BR>
 *                            1 - ACMP1<BR>
 *                            2 - ACMP2<BR>
 *                            3 - ACMP3<BR>
 * @return
 *    None<BR>
 * \par<b>Description:</b><br>
 * Disables an instance of ACMP module.<BR>\n
 * Stops the comparator by resetting CMP_EN bit of respective ANACMP \a instance register. The \a instance number
 * determines which analog comparator to be switched off.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_ACMP_EnableComparator().
 */
__STATIC_INLINE void Cy_ACMP_DisableComparator(Cy_ACMP_t *const peripheral, uint32_t instance)
{
  CY_ASSERT(CY_ACMP_CHECK_MODULE_PTR(peripheral));
  CY_ASSERT(CY_ACMP_CHECK_INSTANCE(instance));

  peripheral->ANACMP[instance] &= (uint16_t)(~((uint32_t)COMPARATOR_ANACMP0_CMP_EN_Msk));
}

/**
 * @param None
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the reference divider for analog comparator instance 1.<BR>\n
 * ACMP1 input INP is driven by an internal reference voltage by setting DIV_EN bit of ANACMP1 register.
 * Other comparator instances can also share this reference divider option by calling the Cy_ACMP_SetInput() API.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_ACMP_SetInput().
 */
__STATIC_INLINE void Cy_ACMP_EnableReferenceDivider(void)
{
  /* Enable the divider switch and connect the divided reference to ACMP1.INP */
  COMPARATOR->ANACMP1 |= (uint16_t)(COMPARATOR_ANACMP1_REF_DIV_EN_Msk);
}

/**
 * @param None
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the reference divider for analog comparator instance 1.<BR>\n
 * ACMP1 input INP is disconnected from the reference divider. This is achieved by reseting DIV_EN bit of ANACMP1
 * register.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
__STATIC_INLINE void Cy_ACMP_DisableReferenceDivider(void)
{
  /* Disable the divider switch and use ACMP1.INP as standard port*/
  COMPARATOR->ANACMP1 &= (uint16_t)(~(COMPARATOR_ANACMP1_REF_DIV_EN_Msk));
}

/**
 * @param peripheral Constant pointer to analog comparator module, of @ref Cy_ACMP_t type. Use @ref CY_ACMP0 macro.
 * @param instance ACMP instance number. <BR>
 *                 Range:<BR> 0 - ACMP0<BR>
 *                            2 - ACMP2<BR>
 *                            3 - ACMP3<BR>
 * @param source ACMP input source selection options.<BR>
 *                 Range:<BR> CY_ACMP_INP_SOURCE_STANDARD_PORT  - Input is connected to port<BR>
 *                            CY_ACMP_INP_SOURCE_ACMP1_INP_PORT - Input is connected to port and ACMP1 INP <BR>
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Sets the analog comparartor input selection for ACMP0, ACMP2 instances.<BR>\n
 * Apart from ACMP1 instance, each ACMP instances can be connected to its own port and ACMP1 INP.
 * Calling @ref Cy_ACMP_EnableReferenceDivider() API, after this API can share the reference divider to one of the
 * comparartor input as explained in the following options.<br>
 * The hardware options to set input are listed below.<br>
 * <OL>
 * <LI>The comparator inputs aren't connected to other ACMP1 comparator inputs.</LI>
 * <LI>Can program the comparator-0 to connect ACMP0.INN to ACMP1.INP</LI>
 * <LI>Can program the comparator-2 to connect ACMP2.INP to ACMP1.INP</LI>
 * </OL><br>
 * Directly accessed registers are ANACMP0, ANACMP2 according to the availability of instance in the devices.
 *
 * \par<b>Related APIs:</b><br>
 * @ref Cy_ACMP_EnableReferenceDivider.<BR>
 * @ref Cy_ACMP_DisableReferenceDivider.
 */
void Cy_ACMP_SetInput(Cy_ACMP_t *const peripheral, uint32_t instance, const Cy_ACMP_INP_SOURCE_t source);


/**
 * @param None
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Set the comparartors to operate in low power mode, by setting the LPWR bit of ANACMP0 register.<BR>\n
 * The low power mode is controlled by ACMP0 instance. Low power mode is applicable for all instances of the
 * comparator. In low power mode, blanking time will be introduced to ensure the stability of comparartor output. This
 * will slow down the comparator operation.
 *
 * \par<b>Related APIs:</b><br>
 * Cy_ACMP_ClearLowPowerMode().
 */
__STATIC_INLINE void Cy_ACMP_SetLowPowerMode(void)
{
  COMPARATOR->ANACMP0 |= (uint16_t)COMPARATOR_ANACMP0_CMP_LPWR_Msk;
}

/**
 * @param None
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Exits the low power mode by reseting LPWR bit of ANACMP0 register.<BR>\n
 * The low power mode is controlled by ACMP0 module.  Low power mode is applicable for all instances of the
 * comparator. To re-enable the low power mode, call the related API @ref Cy_ACMP_SetLowPowerMode().
 *
 * \par<b>Related APIs:</b><br>
 * Cy_ACMP_SetLowPowerMode().
 */
__STATIC_INLINE void Cy_ACMP_ClearLowPowerMode(void)
{
  COMPARATOR->ANACMP0 &= (uint16_t)(~(uint16_t)COMPARATOR_ANACMP0_CMP_LPWR_Msk);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* If ACMP is available*/

#endif /* CY_ACMP_H */
