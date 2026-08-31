/**
 * @file cy_math.h
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

#ifndef CY_MATH_H
#define CY_MATH_H

/**
 * @addtogroup MATH
 * @{
 * @brief MATH Coprocessor (MATH) driver for the PSC1M microcontroller family <br>
 *
 * The MATH Coprocessor (MATH) module comprises of two independent sub-blocks to support the CPU in math-intensive
 * computations: a Divider Unit (DIV) for signed and unsigned 32-bit division operations and a CORDIC
 * (COrdinate Rotation DIgital Computer) Coprocessor for computation of trigonometric, linear or hyperbolic functions.<br>
 *
 * MATH driver features:
 * -# CORDIC Coprocessor is used for computation of trigonometric and hyperbolic functions
 * -# Supports result chaining between the Divider Unit and CORDIC Coprocessor
 * -# All MATH APIs are available in <B>Blocking</B> and <B>non-blocking</B> modes. Non-blocking APIs are suffixed with <B>NB</B>.
 * -# 32bit signed and unsigned division implementations available for __aeabi_uidiv(), __aeabi_idiv(), __aeabi_uidivmod(), __aeabi_idivmod()
 * -# Divider and CORDIC unit busy status can be checked by Cy_MATH_DIV_IsBusy() and Cy_MATH_CORDIC_IsBusy()
 * -# Individual APIs available to return the result of each non-blocking MATH function
 *
 * <B>Note:</B> <br>
 * All non-blocking MATH APIs are not atomic and hence occurence of interrupts during the normal execution of
 * these APIs may lead to erroneous results. User has to exercise caution while using these APIs.
 *
 * Example:
 * Execution of divide instruction (/) in an ISR during the normal execution of non-blocking APIs may give erroneous results.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_common.h"

#if defined(MATH) || defined(CY_DOXYGEN)
#include "cy_scu.h"


/*********************************************************************************************************************
 * TYPE DEFINITIONS
 ********************************************************************************************************************/
/**
 * @brief This typedef is used for Input and Output Data representation in blocking & non-blocking functions.
 * Cy_MATH_Q0_23_t => 1 Signed bit, 0 Integer bits, 23 fraction bits.
 */
typedef int32_t Cy_MATH_Q0_23_t;

/**
 * @brief This typedef is used for Input Data representation in blocking & non-blocking functions.
 * Cy_MATH_Q8_15_t => 1 Signed bit, 8 Integer bits, 15 fraction bits.
 */
typedef int32_t Cy_MATH_Q8_15_t;

/**
 * @brief This typedef is used for Output Data representation in blocking & non-blocking functions.
 * Cy_MATH_Q1_22_t => 1 Signed bit, 1 Integer bits, 22 fraction bits.
 */
typedef int32_t Cy_MATH_Q1_22_t;

/**
 * @brief This typedef is used for Output Data representation in blocking & non-blocking functions.
 * Cy_MATH_Q0_11_t => 1 Signed bit, 0 Integer bits, 11 fraction bits.
 */
typedef int32_t Cy_MATH_Q0_11_t;

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
/* Utility macros */
#define CY_MATH_Q0_23(x) ((Cy_MATH_Q0_23_t)(((x) >= 0) ? ((x) * (1 << 23) + 0.5) : ((x) * (1 << 23) - 0.5))) /**< Converts the given number to Cy_MATH_Q0_23_t format */
#define CY_MATH_Q0_11(x) ((Cy_MATH_Q0_11_t)(((x) >= 0) ? ((x) * (1 << 11) + 0.5) : ((x) * (1 << 11) - 0.5))) /**< Converts the given number to Cy_MATH_Q0_11_t format */

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/**
 * @brief Service request events for the DIV and CORDIC modules
 */
typedef enum CY_MATH_EVENT
{
  CY_MATH_EVENT_DIV_END_OF_CALC    = 1U, /**< Divider end of calculation event */
  CY_MATH_EVENT_DIV_ERROR          = 2U, /**< Divider error event */
  CY_MATH_EVENT_CORDIC_END_OF_CALC = 4U, /**< CORDIC end of calculation event */
  CY_MATH_EVENT_CORDIC_ERROR       = 8U  /**< CORDIC error event */
} Cy_MATH_EVENT_t;

/**
 * @brief Dividend Register Result Chaining
 */
typedef enum CY_MATH_DIV_DVDRC
{
  CY_MATH_DIV_DVDRC_DISABLED        = 0U << MATH_GLBCON_DVDRC_Pos, /**< No result chaining is selected */
  CY_MATH_DIV_DVDRC_QUOT_IS_SOURCE  = 1U << MATH_GLBCON_DVDRC_Pos, /**< QUOT register is the selected source */
  CY_MATH_DIV_DVDRC_RMD_IS_SOURCE   = 2U << MATH_GLBCON_DVDRC_Pos, /**< RMD register is the selected source */
  CY_MATH_DIV_DVDRC_CORRX_IS_SOURCE = 3U << MATH_GLBCON_DVDRC_Pos, /**< CORRX is the selected source */
  CY_MATH_DIV_DVDRC_CORRY_IS_SOURCE = 4U << MATH_GLBCON_DVDRC_Pos, /**< CORRY is the selected source */
  CY_MATH_DIV_DVDRC_CORRZ_IS_SOURCE = 5U << MATH_GLBCON_DVDRC_Pos  /**< CORRZ is the selected source */
} Cy_MATH_DIV_DVDRC_t;

/**
 * @brief Divisor Register Result Chaining
 */
typedef enum CY_MATH_DIV_DVSRC
{
  CY_MATH_DIV_DVSRC_DISABLED        = 0U << MATH_GLBCON_DVSRC_Pos, /**< No result chaining is selected */
  CY_MATH_DIV_DVSRC_QUOT_IS_SOURCE  = 1U << MATH_GLBCON_DVSRC_Pos, /**< QUOT register is the selected source */
  CY_MATH_DIV_DVSRC_RMD_IS_SOURCE   = 2U << MATH_GLBCON_DVSRC_Pos, /**< RMD register is the selected source */
  CY_MATH_DIV_DVSRC_CORRX_IS_SOURCE = 3U << MATH_GLBCON_DVSRC_Pos, /**< CORRX is the selected source */
  CY_MATH_DIV_DVSRC_CORRY_IS_SOURCE = 4U << MATH_GLBCON_DVSRC_Pos, /**< CORRY is the selected source */
  CY_MATH_DIV_DVSRC_CORRZ_IS_SOURCE = 5U << MATH_GLBCON_DVSRC_Pos  /**< CORRZ is the selected source */
} Cy_MATH_DIV_DVSRC_t;

/**
 * @brief CORDX Register Result Chaining
 */
typedef enum CY_MATH_CORDIC_CORDXRC
{
  CY_MATH_CORDIC_CORDXRC_DISABLED       = 0U << MATH_GLBCON_CORDXRC_Pos, /**< No result chaining is selected */
  CY_MATH_CORDIC_CORDXRC_QUOT_IS_SOURCE = 1U << MATH_GLBCON_CORDXRC_Pos, /**< QUOT register is the selected source */
  CY_MATH_CORDIC_CORDXRC_RMD_IS_SOURCE  = 2U << MATH_GLBCON_CORDXRC_Pos  /**< RMD register is the selected source */
} Cy_MATH_CORDIC_CORDXRC_t;

/**
 * @brief CORDY Register Result Chaining
 */
typedef enum CY_MATH_CORDIC_CORDYRC
{
  CY_MATH_CORDIC_CORDYRC_DISABLED       = 0U << MATH_GLBCON_CORDYRC_Pos, /**< No result chaining is selected */
  CY_MATH_CORDIC_CORDYRC_QUOT_IS_SOURCE = 1U << MATH_GLBCON_CORDYRC_Pos, /**< QUOT register is the selected source */
  CY_MATH_CORDIC_CORDYRC_RMD_IS_SOURCE  = 2U << MATH_GLBCON_CORDYRC_Pos  /**< RMD register is the selected source */
} Cy_MATH_CORDIC_CORDYRC_t;

/**
 * @brief CORDZ Register Result Chaining
 */
typedef enum CY_MATH_CORDIC_CORDZRC
{
  CY_MATH_CORDIC_CORDZRC_DISABLED       = 0U << MATH_GLBCON_CORDZRC_Pos, /**< No result chaining is selected */
  CY_MATH_CORDIC_CORDZRC_QUOT_IS_SOURCE = 1U << MATH_GLBCON_CORDZRC_Pos, /**< QUOT register is the selected source */
  CY_MATH_CORDIC_CORDZRC_RMD_IS_SOURCE  = 2U << MATH_GLBCON_CORDZRC_Pos  /**< RMD register is the selected source */
} Cy_MATH_CORDIC_CORDZRC_t;
/**
 * @brief CORDIC operating mode
 */
typedef enum CY_MATH_CORDIC_OPERATING_MODE
{
  CY_MATH_CORDIC_OPERATING_MODE_LINEAR     = 0U << MATH_CON_MODE_Pos,    /**< Linear mode */
  CY_MATH_CORDIC_OPERATING_MODE_CIRCULAR   = 1U << MATH_CON_MODE_Pos,    /**< Circular mode */
  CY_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC = 3U << MATH_CON_MODE_Pos     /**< Hyperbolic mode */
} Cy_MATH_CORDIC_OPERATING_MODE_t;

/**
 * @brief Rotation vectoring selection
 */
typedef enum CY_MATH_CORDIC_ROTVEC_MODE
{
  CY_MATH_CORDIC_ROTVEC_MODE_VECTORING     = 0U << MATH_CON_ROTVEC_Pos,  /**< Vectoring mode */
  CY_MATH_CORDIC_ROTVEC_MODE_ROTATION      = 1U << MATH_CON_ROTVEC_Pos   /**< Rotation mode */
} Cy_MATH_CORDIC_ROTVEC_MODE_t;

/**
 * @brief Calculated value of CORRX and CORRY are each divided by this factor to yield the result.
 */
typedef enum CY_MATH_CORDIC_MAGNITUDE
{
  CY_MATH_CORDIC_MAGNITUDE_DIVBY1 = 0U << MATH_CON_MPS_Pos,              /**< Divide by 1 */
  CY_MATH_CORDIC_MAGNITUDE_DIVBY2 = 1U << MATH_CON_MPS_Pos,              /**< Divide by 2 */
  CY_MATH_CORDIC_MAGNITUDE_DIVBY4 = 2U << MATH_CON_MPS_Pos,              /**< Divide by 4 */
} Cy_MATH_CORDIC_MAGNITUDE_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * API Prototypes - General
 ********************************************************************************************************************/

/**
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the Math module by un-gating the clock.
 *
 * \par
 * MATH coprocessor's clock is enabled by setting \a MATH bit of \a CGATCLR0 register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_Disable()\n\n\n
 *
 */
__STATIC_INLINE void Cy_MATH_Enable(void)
{
  /* Un-gates clock to the MATH kernel */
  Cy_SCU_CLOCK_UngatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_MATH);
}

/**
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the Math module by gating the clock.
 *
 * \par
 * MATH coprocessor's clock is disabled by setting \a MATH bit of \a CGATSET0 register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_Disable()\n\n\n
 *
 */
__STATIC_INLINE void Cy_MATH_Disable(void)
{
  /* Gates clock to the MATH kernel */
  Cy_SCU_CLOCK_GatePeripheralClock(CY_SCU_PERIPHERAL_CLOCK_MATH);
}

/**
 *
 * @return bool \n
 * true  - if DIV unit is busy
 * false - if DIV unit is not busy
 *
 * \par<b>Description:</b><br>
 * Utility function to check if the DIV unit is busy.
 *
 * \par
 * Divider unit status is determined by reading \a BSY bit of \a DIVST register.
 *
 */
bool Cy_MATH_DIV_IsBusy(void);

/**
 *
 * @return bool \n
 * true  - if CORDIC unit is busy\n
 * false - if CORDIC unit is not busy
 *
 * \par<b>Description:</b><br>
 * Utility function to check if the DIV unit is busy.
 *
 * \par
 * CORDIC coprocessor's status is determined by reading \a BSY bit of \a STATC register.
 *
 */
bool Cy_MATH_CORDIC_IsBusy(void);

/**
 * @param event Event of type Cy_MATH_EVENT_t
 *
 * @return bool\n
 * true  - if status is set\n
 * false - if status is not set
 *
 * \par<b>Description:</b><br>
 * Returns the status of the requested event.
 *
 * \par
 * Status of DIV & CORDIC unit's event (end of calculation & error) status is determined by reading \a EVFR register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_EnableEvent(), Cy_MATH_DisableEvent(), Cy_MATH_SetEvent(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
bool Cy_MATH_GetEventStatus(const Cy_MATH_EVENT_t event);

/**
 * @param event Event of type Cy_MATH_EVENT_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the requested event.
 *
 * \par
 * DIV & CORDIC unit's event (end of calculation & error) is enabled by setting bit-fields of \a EVIER register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_GetEventStatus(), Cy_MATH_DisableEvent()\n\n\n
 *
 */
__STATIC_INLINE void Cy_MATH_EnableEvent(const Cy_MATH_EVENT_t event)
{
  MATH->EVIER |= (uint32_t) event;
}

/**
 * @param event Event of type Cy_MATH_EVENT_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the requested event.
 *
 * \par
 * DIV & CORDIC unit's event (end of calculation & error) is disabled by clearing bit-fields of \a EVIER register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_GetEventStatus(), Cy_MATH_EnableEvent()\n\n\n
 *
 */
__STATIC_INLINE void Cy_MATH_DisableEvent(const Cy_MATH_EVENT_t event)
{
  MATH->EVIER &= ~((uint32_t) event);
}

/**
 * @param event Event of type Cy_MATH_EVENT_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets the requested event. This is a software setting for the event.
 *
 * \par
 * DIV & CORDIC unit's event (end of calculation & error) is set by setting bit-fields of \a EVFSR register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_ClearEvent()\n\n\n
 *
 */
__STATIC_INLINE void Cy_MATH_SetEvent(const Cy_MATH_EVENT_t event)
{
  MATH->EVFSR |= (uint32_t) event;
}

/**
 * @param event Event of type Cy_MATH_EVENT_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Clears the requested event.
 *
 * \par
 * DIV & CORDIC unit's event (end of calculation & error) is cleared by setting bit-fields of \a EVFCR register.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_SetEvent()\n\n\n
 *
 */
__STATIC_INLINE void Cy_MATH_ClearEvent(const Cy_MATH_EVENT_t event)
{
  MATH->EVFCR |= (uint32_t) event;
}

/**
 * @return Cy_MATH_Q0_23_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Cosine operation.
 *
 * \par
 * Most significant 24 bits of \a CORRX register returns the result of Cosine operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_CosNB()\n\n\n
 *
 */
__STATIC_INLINE Cy_MATH_Q0_23_t Cy_MATH_CORDIC_GetCosResult(void)
{
  return ((Cy_MATH_Q0_23_t) (((int32_t)MATH->CORRX) >> MATH_CORRX_RESULT_Pos));
}

/**
 * @return Cy_MATH_Q0_23_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Sine operation.
 *
 * \par
 * Most significant 24 bits of \a CORRY register returns the result of Sine operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_SinNB()\n\n\n
 *
 */
__STATIC_INLINE Cy_MATH_Q0_23_t Cy_MATH_CORDIC_GetSinResult(void)
{
  return ((Cy_MATH_Q0_23_t) (((int32_t)MATH->CORRY) >> MATH_CORRY_RESULT_Pos));
}

/**
 * @return Cy_MATH_Q0_11_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Tangent operation.
 *
 * \par
 * \a QUOT register returns the result of Tangent operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_TanNB()\n\n\n
 *
 */
__STATIC_INLINE Cy_MATH_Q0_11_t Cy_MATH_CORDIC_GetTanResult(void)
{
  return ((Cy_MATH_Q0_11_t) MATH->QUOT);
}

/**
 * @return Cy_MATH_Q0_23_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Arc Tangent operation.
 *
 * \par
 * Most significant 24 bits of \a CORRZ register returns the result of Arc Tangent operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_ArcTanNB()\n\n\n
 *
 */
__STATIC_INLINE Cy_MATH_Q0_23_t Cy_MATH_CORDIC_GetArcTanResult(void)
{
  return ((Cy_MATH_Q0_23_t) (((int32_t)MATH->CORRZ) >> MATH_CORRZ_RESULT_Pos));
}

/**
 * @return Cy_MATH_Q1_22_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Hyperbolic Cosine operation.
 *
 * \par
 * Most significant 24 bits of \a CORRX register returns the result of Hyperbolic Cosine operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_CoshNB()\n\n\n
 *
 */
__STATIC_INLINE Cy_MATH_Q1_22_t Cy_MATH_CORDIC_GetCoshResult(void)
{
  return ((Cy_MATH_Q1_22_t) (((int32_t)MATH->CORRX) >> MATH_CORRX_RESULT_Pos));
}

/**
 * @return Cy_MATH_Q1_22_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Hyperbolic Sine operation.
 *
 * \par
 * Most significant 24 bits of \a CORRY register returns the result of Hyperbolic Sine operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_SinhNB()\n\n\n
 *
 */
__STATIC_INLINE Cy_MATH_Q1_22_t Cy_MATH_CORDIC_GetSinhResult(void)
{
  return ((Cy_MATH_Q1_22_t) (((int32_t)MATH->CORRY) >> MATH_CORRY_RESULT_Pos));
}

/**
 * @return Cy_MATH_Q0_11_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Hyperbolic Tangent operation.
 *
 * \par
 * \a QUOT register returns the result of Hyperbolic Tangent operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_TanhNB()\n\n\n
 *
 */
__STATIC_INLINE Cy_MATH_Q0_11_t Cy_MATH_CORDIC_GetTanhResult(void)
{
  return ((Cy_MATH_Q0_11_t) MATH->QUOT);
}

/**
 * @return uint32_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Unsigned Division operation.
 *
 * \par
 * \a QUOT register returns the result of Unsigned Division operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_DIV_UnsignedDivNB()\n\n\n
 *
 */
__STATIC_INLINE uint32_t Cy_MATH_DIV_GetUnsignedDivResult(void)
{
  return ((uint32_t) MATH->QUOT);
}

/**
 * @return uint32_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Unsigned Modulo operation.
 *
 * \par
 * \a RMD register returns the result of Unsigned Modulo operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_DIV_UnsignedModNB()\n\n\n
 *
 */
__STATIC_INLINE uint32_t Cy_MATH_DIV_GetUnsignedModResult(void)
{
  return ((uint32_t) MATH->RMD);
}

/**
 * @return int32_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Signed Division operation.
 *
 * \par
 * \a QUOT register returns the result of Signed Division operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_DIV_SignedDivNB()\n\n\n
 *
 */
__STATIC_INLINE int32_t Cy_MATH_DIV_GetSignedDivResult(void)
{
  return ((int32_t) MATH->QUOT);
}

/**
 * @return int32_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Signed Modulo operation.
 *
 * \par
 * \a RMD register returns the result of Signed Modulo operation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_DIV_SignedModNB()\n\n\n
 *
 */
__STATIC_INLINE int32_t Cy_MATH_DIV_GetSignedModResult(void)
{
  return ((int32_t) MATH->RMD);
}

/***********************************************************************************************************************
 * API Prototypes - Blocking functions
 **********************************************************************************************************************/
/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return Cy_MATH_Q0_23_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the cosine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 * Most significant 24 bits of \a CORRX register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_Sin(), Cy_MATH_CORDIC_Tan()\n\n\n
 *
 */
Cy_MATH_Q0_23_t Cy_MATH_CORDIC_Cos(Cy_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return Cy_MATH_Q0_23_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the sine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 * Most significant 24 bits of \a CORRY register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_Cos(), Cy_MATH_CORDIC_Tan()\n\n\n
 *
 */
Cy_MATH_Q0_23_t Cy_MATH_CORDIC_Sin(Cy_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return Cy_MATH_Q0_11_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the tangent for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Chains the results of Cosine (\a CORRX) and Sine (\a CORRY) as a dividend and divisor by configuring \a GLBCON register.
 * \a CORDZ register is programmed with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 * Most significant 24 bits of \a CORRY register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_Sin(), Cy_MATH_CORDIC_Cos()\n\n\n
 *
 */
Cy_MATH_Q0_11_t Cy_MATH_CORDIC_Tan(Cy_MATH_Q0_23_t angle_in_radians);

/**
 *
 * @param x Value representing the proportion of the x-coordinate (Cy_MATH_Q8_15_t format)
 * @param y Value representing the proportion of the y-coordinate (Cy_MATH_Q8_15_t format)
 *
 * @return Cy_MATH_Q0_23_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the principal value arc tangent of an angle of y/x expressed in radians.
 * The input radians must be in Cy_MATH_Q8_15_t format.
 *
 * \par
 * This function programs CORDIC as circular mode.
 * \a CORDY register is programmed with input \a y and \a CORDX register is programmed with input \a x.
 * Most significant 24 bits of \a CORRZ register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 */
Cy_MATH_Q0_23_t Cy_MATH_CORDIC_ArcTan(Cy_MATH_Q8_15_t x, Cy_MATH_Q8_15_t y);

/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return Cy_MATH_Q1_22_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic cosine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 * Most significant 24 bits of \a CORRX register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_Sinh(), Cy_MATH_CORDIC_Tanh()\n\n\n
 *
 */
Cy_MATH_Q1_22_t Cy_MATH_CORDIC_Cosh(Cy_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return Cy_MATH_Q1_22_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic sine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 * Most significant 24 bits of \a CORRY register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_Cosh(), Cy_MATH_CORDIC_Tanh()\n\n\n
 *
 */
Cy_MATH_Q1_22_t Cy_MATH_CORDIC_Sinh(Cy_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return Cy_MATH_Q0_11_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic tangent for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Chains the results of Cosine (\a CORRX) and Sine (\a CORRY) as a dividend and divisor by configuring \a GLBCON register.
 * \a CORDZ register is programmed with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 * \a QUOT register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_Sinh(), Cy_MATH_CORDIC_Cosh()()\n\n\n
 *
 */
Cy_MATH_Q0_11_t Cy_MATH_CORDIC_Tanh(Cy_MATH_Q0_23_t angle_in_radians);

/***********************************************************************************************************************
 * API Prototypes - Non blocking functions
 **********************************************************************************************************************/
/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the cosine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 * Call Cy_MATH_CORDIC_GetCosResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_GetCosResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_CORDIC_CosNB(Cy_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the sine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 * Call Cy_MATH_CORDIC_GetSinResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_GetSinResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_CORDIC_SinNB(Cy_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the tangent for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 * Call Cy_MATH_CORDIC_GetTanResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Chains the results of Cosine (\a CORRX) and Sine (\a CORRY) as a dividend and divisor by configuring \a GLBCON register.
 * \a CORDZ register is programmed with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_GetTanResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_CORDIC_TanNB(Cy_MATH_Q0_23_t angle_in_radians);

/**
 *
 * @param x Value representing the proportion of the x-coordinate (Cy_MATH_Q8_15_t format)
 * @param y Value representing the proportion of the y-coordinate (Cy_MATH_Q8_15_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the principal value arc tangent of an angle of y/x expressed in radians.
 * The input radians must be in Cy_MATH_Q8_15_t format.
 * Call Cy_MATH_CORDIC_GetArcTanResult() API to get the result.
 *
 * \par
 * This function programs CORDIC as circular mode.
 * \a CORDY register is programmed with input \a y and \a CORDX register is programmed with input \a x.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_GetArcTanResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_CORDIC_ArcTanNB(Cy_MATH_Q8_15_t x, Cy_MATH_Q8_15_t y);

/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic cosine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 * Call Cy_MATH_CORDIC_GetCoshResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_GetCoshResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_CORDIC_CoshNB(Cy_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic sine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 * Call Cy_MATH_CORDIC_GetSinhResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_GetSinhResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_CORDIC_SinhNB(Cy_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (Cy_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic tangent for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in Cy_MATH_Q0_23_t format.
 * Call Cy_MATH_CORDIC_GetTanhResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Chains the results of Cosine (\a CORRX) and Sine (\a CORRY) as a dividend and divisor by configuring \a GLBCON register.
 * \a CORDZ register is programmed with input \a angle_in_radians and \a CORDX register with gain \a CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_CORDIC_GetTanhResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_CORDIC_TanhNB(Cy_MATH_Q0_23_t angle_in_radians);

/**
 * @param dividend - Dividend
 * @param divisor  - Divisor
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Performs unsigned integer division and computes quotient of the division.
 * Call Cy_MATH_DIV_GetUnsignedDivResult() API to get the result.
 *
 * \par
 * Divider unit is configured for unsigned division.
 * \a DVD & \a DVS registers are programmed with \a dividend and \a divisor values.
 * The division is started with the write to DVS register.
 *
 * \par<b>Note:</b><br>
 * Ensure \e divisor is smaller than \e dividend.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_DIV_GetUnsignedDivResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_DIV_UnsignedDivNB(uint32_t dividend, uint32_t divisor);

/**
 * @param dividend - Dividend
 * @param divisor  - Divisor
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Performs signed integer division and computes quotient of the division.
 * Call Cy_MATH_DIV_GetSignedDivResult() API to get the result.
 *
 * \par
 * Divider unit is configured for signed division.
 * \a DVD & \a DVS registers are programmed with \a dividend and \a divisor values.
 * The division is started with the write to DVS register.
 *
 * \par<b>Note:</b><br>
 * Ensure \e divisor is smaller than \e dividend.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_DIV_GetSignedDivResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_DIV_SignedDivNB(int32_t dividend, int32_t divisor);

/**
 * @param dividend - Dividend
 * @param divisor  - Divisor
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Performs unsigned modulo operation and computes remainder of the division.
 * Call Cy_MATH_DIV_GetUnsignedModResult() API to get the result.
 *
 * \par
 * Divider unit is configured for unsigned division.
 * \a DVD & \a DVS registers are programmed with \a dividend and \a divisor values.
 * The division is started with the write to DVS register.
 *
 * \par<b>Note:</b><br>
 * Ensure \e divisor is smaller than \e dividend.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_DIV_GetUnsignedModResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_DIV_UnsignedModNB(uint32_t dividend, uint32_t divisor);

/**
 * @param dividend - Dividend
 * @param divisor  - Divisor
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Performs signed modulo operation and computes remainder of the division.
 * Call Cy_MATH_DIV_GetSignedModResult() API to get the result.
 *
 * \par
 * Divider unit is configured for signed division.
 * \a DVD & \a DVS registers are programmed with \a dividend and \a divisor values.
 * The division is started with the write to DVS register.
 *
 * \par<b>Note:</b><br>
 * Ensure \e divisor is smaller than \e dividend.
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_MATH_DIV_GetSignedModResult(), Cy_MATH_EnableEvent(), Cy_MATH_GetEventStatus(), Cy_MATH_ClearEvent()\n\n\n
 *
 */
void Cy_MATH_DIV_SignedModNB(int32_t dividend, int32_t divisor);

/**
 * @param x - Value whose square root is computed
 *
 * @return Square root of x <BR>
 *
 * \par<b>Description:</b><br>
 * Computes square root of Q15 number
 *
 * \par<b>Note:</b><br>
 * x > 0
 *
 */
int16_t Cy_MATH_CORDIC_Q15_Sqrt(int16_t x);

/**
 * @param x - Value whose square root is computed
 *
 * @return Square root of x <BR>
 *
 * \par<b>Description:</b><br>
 * Computes square root of Q31 number
 *
 * \par<b>Note:</b><br>
 * x > 0
 *
 */
int32_t Cy_MATH_CORDIC_Q31_Sqrt(int32_t x);
/**
 * @}
 */

#endif /* end of #if defined(MATH) */

#ifdef __cplusplus
}
#endif

#endif /* CY_MATH_H */
