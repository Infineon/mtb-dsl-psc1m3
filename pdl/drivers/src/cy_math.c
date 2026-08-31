
/**
 * @file cy_math.c
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
 * @brief MATH driver - API implementation for PSC1M family MATH libraries. <br>
 *
 * <b>Detailed description of file</b> <br>
 * APIs provided in this file cover the following functional blocks of MATH: <br>
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_math.h"

#if defined (MATH)
/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
/* Reciprocal of Circular gain in Cy_MATH_Q0_23_t format ((2^23)/1.646760258121) */
#define CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023       (0x4DBA76U)
/* Reciprocal of Hyperbolic gain in Cy_MATH_Q1_22_t format ((2^22)/0.828159360960) */
#define CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22    (0x4D47A1U)
/* Signed division is selected */
#define CY_MATH_SIGNED_DIVISION                      ((uint32_t) 0 << MATH_DIVCON_USIGN_Pos)
/* Unsigned division is selected */
#define CY_MATH_UNSIGNED_DIVISION                    ((uint32_t) 1 << MATH_DIVCON_USIGN_Pos)

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * GLOBAL DATA
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * API IMPLEMENTATION - Utility functions
 ********************************************************************************************************************/
__attribute__((always_inline)) __STATIC_INLINE uint32_t critical_section_enter(void)
{
  uint32_t status;
  status = __get_PRIMASK();
  __disable_irq ();
  return status;
}

__attribute__((always_inline)) __STATIC_INLINE void critical_section_exit(uint32_t status)
{
  __set_PRIMASK(status);
}

/* Utility function to check if the DIV unit is busy */
bool Cy_MATH_DIV_IsBusy(void)
{
  bool status;
  if (MATH->DIVST & MATH_DIVST_BSY_Msk)
  {
    status = true;  /* DIV unit is busy running a division operation */
  }
  else
  {
    status = false; /* DIV unit is idle */
  }

  return (status);
}

/* Utility function to check if the CORDIC unit is busy */
bool Cy_MATH_CORDIC_IsBusy(void)
{
  bool status;
  if (MATH->STATC & MATH_STATC_BSY_Msk)
  {
    status = true;  /* CORDIC unit is busy running an operation */
  }
  else
  {
    status = false; /* CORDIC unit is idle */
  }

  return (status);
}

/* This functions returns the status of a requested event */
bool Cy_MATH_GetEventStatus(const Cy_MATH_EVENT_t event)
{
  bool status;
  if (MATH->EVFR & (uint32_t) event)
  {
    status = true; /* Requested event has been detected */
  }
  else
  {
    status = false; /* Requested event has not been detected */
  }
  return (status);
}

#ifndef CY_MATH_DISABLE_DIV_ABI

/* Forward prototypes.  */
#if defined ( __GNUC__ ) && !defined( __ARMCC_VERSION )
uint32_t __aeabi_uidiv(uint32_t dividend, uint32_t divisor) __attribute__((externally_visible));
int32_t __aeabi_idiv(int32_t dividend, int32_t divisor) __attribute__((externally_visible));
uint64_t __aeabi_uidivmod(uint32_t dividend, uint32_t divisor) __attribute__((externally_visible));
int64_t __aeabi_idivmod(int32_t dividend, int32_t divisor) __attribute__((externally_visible));
#else
uint32_t __aeabi_uidiv(uint32_t dividend, uint32_t divisor);
int32_t __aeabi_idiv(int32_t dividend, int32_t divisor);
uint64_t __aeabi_uidivmod(uint32_t dividend, uint32_t divisor);
int64_t __aeabi_idivmod(int32_t dividend, int32_t divisor);
#endif

/***********************************************************************************************************************
 * API IMPLEMENTATION - aeabi routines
 **********************************************************************************************************************/
/* This function performs unsigned integer division */
uint32_t __aeabi_uidiv(uint32_t dividend, uint32_t divisor)
{
  uint32_t result;
  uint32_t ics;
  ics = critical_section_enter();

  MATH->DIVCON  = CY_MATH_UNSIGNED_DIVISION;
  MATH->DVD     = dividend;
  MATH->DVS     = divisor;

  result = MATH->QUOT;

  critical_section_exit(ics);

  return result;
}

/* This function performs signed integer division */
int32_t __aeabi_idiv(int32_t dividend, int32_t divisor)
{
  uint32_t result;
  uint32_t ics;
  ics = critical_section_enter();

  MATH->DIVCON  = CY_MATH_SIGNED_DIVISION;
  MATH->DVD     = dividend;
  MATH->DVS     = divisor;

  result = MATH->QUOT;

  critical_section_exit(ics);

  return result;
}

/* This function performs unsigned integer division modulo */
uint64_t __aeabi_uidivmod(uint32_t dividend, uint32_t divisor)
{
  uint64_t remainder;
  uint64_t quot;
  uint32_t ics;
  ics = critical_section_enter();

  MATH->DIVCON  = CY_MATH_UNSIGNED_DIVISION;
  MATH->DVD     = dividend;
  MATH->DVS     = divisor;

  remainder = (uint64_t)MATH->RMD;
  quot = (uint64_t)MATH->QUOT;

  critical_section_exit(ics);

  return ((remainder << 32) | quot);
}

/* This function performs signed integer division modulo */
int64_t __aeabi_idivmod(int32_t dividend, int32_t divisor)
{
  uint64_t remainder;
  uint64_t quot;
  uint32_t ics;
  ics = critical_section_enter();

  MATH->DIVCON  = CY_MATH_SIGNED_DIVISION;
  MATH->DVD     = dividend;
  MATH->DVS     = divisor;

  remainder = (uint64_t)MATH->RMD;;
  quot    = (uint64_t)MATH->QUOT;

  critical_section_exit(ics);

  return ((int64_t)((remainder << 32) | quot));
}
#endif

/***********************************************************************************************************************
 * API IMPLEMENTATION - Blocking functions
 **********************************************************************************************************************/
/* This function computes the cosine of a given angle in radians */
Cy_MATH_Q0_23_t Cy_MATH_CORDIC_Cos(Cy_MATH_Q0_23_t angle_in_radians)
{
  int32_t result;
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t) angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U;  /* Clear register */
  MATH->CORDX = CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
  result      = ((int32_t)MATH->CORRX) >> MATH_CORRX_RESULT_Pos;
  return ((Cy_MATH_Q0_23_t) result);
}

/* This function computes the sine of a given angle in radians */
Cy_MATH_Q0_23_t Cy_MATH_CORDIC_Sin(Cy_MATH_Q0_23_t angle_in_radians)
{
  int32_t result;
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
  result      = ((int32_t)MATH->CORRY) >> MATH_CORRY_RESULT_Pos;
  return ((Cy_MATH_Q0_23_t) result);
}

/* This function computes the tangent of a given angle in radians */
Cy_MATH_Q0_11_t Cy_MATH_CORDIC_Tan(Cy_MATH_Q0_23_t angle_in_radians)
{
  uint32_t result;
  MATH->GLBCON = (uint32_t) CY_MATH_DIV_DVDRC_CORRY_IS_SOURCE + \
                 (uint32_t) CY_MATH_DIV_DVSRC_CORRX_IS_SOURCE;      /* Chain the results of CORDIC operation to DIV unit */
  MATH->DIVCON = (uint32_t) 11 << MATH_DIVCON_DVSSRC_Pos;            /* Right Shifts Divisor by 11 places prior to division */
  MATH->STATC  = 0U; /* Clear register */
  MATH->CON    = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                 (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ  = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY  = 0U; /* Clear register */
  MATH->CORDX  = CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
  result       = MATH->QUOT;
  return ((Cy_MATH_Q0_11_t) result);
}

/* This function computes the arc tangent of a given angle in radians */
Cy_MATH_Q0_23_t Cy_MATH_CORDIC_ArcTan(Cy_MATH_Q8_15_t x, Cy_MATH_Q8_15_t y)
{
  uint32_t result;
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_CIRCULAR;
  MATH->CORDZ = 0U; /* Clear register */
  MATH->CORDY = ((uint32_t) y) << MATH_CORDY_DATA_Pos;
  MATH->CORDX = ((uint32_t) x) << MATH_CORDX_DATA_Pos;
  result      = ((int32_t)MATH->CORRZ) >> MATH_CORRZ_RESULT_Pos;
  return ((Cy_MATH_Q0_23_t) result);
}

/* This function computes the hyperbolic cosine of a given angle in radians */
Cy_MATH_Q1_22_t Cy_MATH_CORDIC_Cosh(Cy_MATH_Q0_23_t angle_in_radians)
{
  int32_t result;
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t) angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
  result      = ((int32_t)MATH->CORRX) >> MATH_CORRX_RESULT_Pos;
  return ((Cy_MATH_Q1_22_t) result);
}

/* This function computes the hyperbolic sine of a given angle in radians */
Cy_MATH_Q1_22_t Cy_MATH_CORDIC_Sinh(Cy_MATH_Q0_23_t angle_in_radians)
{
  int32_t result;
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
  result      = ((int32_t)MATH->CORRY) >> MATH_CORRY_RESULT_Pos;
  return ((Cy_MATH_Q1_22_t) result);
}

/* This function computes the hyperbolic tangent of a given angle in radians */
Cy_MATH_Q0_11_t Cy_MATH_CORDIC_Tanh(Cy_MATH_Q0_23_t angle_in_radians)
{
  uint32_t result;
  MATH->GLBCON = (uint32_t) CY_MATH_DIV_DVDRC_CORRY_IS_SOURCE + \
                 (uint32_t) CY_MATH_DIV_DVSRC_CORRX_IS_SOURCE;      /* Chain the results of CORDIC operation to DIV unit */
  MATH->DIVCON = (uint32_t) 11 << MATH_DIVCON_DVSSRC_Pos;            /* Right Shifts Divisor by 11 places prior to division */
  MATH->STATC  = 0U; /* Clear register */
  MATH->CON    = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                 (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ  = ((uint32_t) angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY  = 0U; /* Clear register */
  MATH->CORDX  = CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
  result       = MATH->QUOT;
  return ((Cy_MATH_Q0_11_t) result);
}

/***********************************************************************************************************************
 * API IMPLEMENTATION - Non blocking functions
 **********************************************************************************************************************/
/* This function computes the cosine of a given angle in radians */
void Cy_MATH_CORDIC_CosNB(Cy_MATH_Q0_23_t angle_in_radians)
{
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t) angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U;  /* Clear register */
  MATH->CORDX = CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
}

/* This function computes the sine of a given angle in radians */
void Cy_MATH_CORDIC_SinNB(Cy_MATH_Q0_23_t angle_in_radians)
{
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
}

/* This function computes the tangent of a given angle in radians */
void Cy_MATH_CORDIC_TanNB(Cy_MATH_Q0_23_t angle_in_radians)
{
  MATH->GLBCON = (uint32_t) CY_MATH_DIV_DVDRC_CORRY_IS_SOURCE + \
                 (uint32_t) CY_MATH_DIV_DVSRC_CORRX_IS_SOURCE;      /* Chain the results of CORDIC operation to DIV unit */
  MATH->DIVCON = (uint32_t) 11 << MATH_DIVCON_DVSSRC_Pos;            /* Right Shifts Divisor by 11 places prior to division */

  MATH->STATC  = 0U; /* Clear register */
  MATH->CON    = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                 (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ  = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY  = 0U; /* Clear register */
  MATH->CORDX  = CY_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
}

/* This function computes the arc tangent of a given value */
void Cy_MATH_CORDIC_ArcTanNB(Cy_MATH_Q8_15_t x, Cy_MATH_Q8_15_t y)
{
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_CIRCULAR;
  MATH->CORDZ = 0U; /* Clear register */
  MATH->CORDY = ((uint32_t) y) << MATH_CORDY_DATA_Pos;
  MATH->CORDX = ((uint32_t) x) << MATH_CORDX_DATA_Pos;
}

/* This function computes the hyperbolic cosine of a given angle in radians */
void Cy_MATH_CORDIC_CoshNB(Cy_MATH_Q0_23_t angle_in_radians)
{
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
}

/* This function computes the hyperbolic sine of a given angle in radians */
void Cy_MATH_CORDIC_SinhNB(Cy_MATH_Q0_23_t angle_in_radians)
{
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
}

/* This function computes the hyperbolic tangent of a given angle in radians */
void Cy_MATH_CORDIC_TanhNB(Cy_MATH_Q0_23_t angle_in_radians)
{
  MATH->GLBCON = (uint32_t) CY_MATH_DIV_DVDRC_CORRY_IS_SOURCE + \
                 (uint32_t) CY_MATH_DIV_DVSRC_CORRX_IS_SOURCE;      /* Chain the results of CORDIC operation to DIV unit */
  MATH->DIVCON = (uint32_t) 11 << MATH_DIVCON_DVSSRC_Pos;            /* Right Shifts Divisor by 11 places prior to division */
  MATH->STATC  = 0U; /* Clear register */
  MATH->CON    = (uint32_t) CY_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                 (uint32_t) CY_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ  = ((uint32_t) angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY  = 0U; /* Clear register */
  MATH->CORDX  = CY_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
}

/* This function performs division for given two unsigned arguments */
void Cy_MATH_DIV_UnsignedDivNB(uint32_t dividend, uint32_t divisor)
{
  MATH->DIVCON = CY_MATH_UNSIGNED_DIVISION;
  MATH->DVD    = dividend;
  MATH->DVS    = divisor;
}

/* This function performs division for given two signed arguments */
void Cy_MATH_DIV_SignedDivNB(int32_t dividend, int32_t divisor)
{
  MATH->DIVCON = CY_MATH_SIGNED_DIVISION;
  MATH->DVD    = dividend;
  MATH->DVS    = divisor;
}

/* This function performs modulo operation for given two unsigned arguments */
void Cy_MATH_DIV_UnsignedModNB(uint32_t dividend, uint32_t divisor)
{
  MATH->DIVCON = CY_MATH_UNSIGNED_DIVISION;
  MATH->DVD    = dividend;
  MATH->DVS    = divisor;
}

/* This function performs modulo operation for given two signed arguments */
void Cy_MATH_DIV_SignedModNB(int32_t dividend, int32_t divisor)
{
  MATH->DIVCON = CY_MATH_SIGNED_DIVISION;
  MATH->DVD    = dividend;
  MATH->DVS    = divisor;
}

int16_t Cy_MATH_CORDIC_Q15_Sqrt(int16_t x)
{
  int32_t temp;
  MATH->STATC = 0U; /* Clear register */

  MATH->CON   = (uint32_t)CY_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC |
                (uint32_t)CY_MATH_CORDIC_ROTVEC_MODE_VECTORING;

  temp = (int32_t)x << 15; /* Q30 to handle numbers > 1.0 */

  MATH->CORDY = (temp - 0x10000000U); /* x - 0.25 */
  MATH->CORDX = (temp + 0x10000000U); /* x + 0.25 */

  return (int16_t)(((MATH->CORRX >> 14) * 39568) >> 16); /* Q16 * Q15 */
}

int32_t Cy_MATH_CORDIC_Q31_Sqrt(int32_t x)
{
  MATH->STATC = 0U; /* Clear register */

  MATH->CON   = (uint32_t)CY_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC |
                (uint32_t)CY_MATH_CORDIC_ROTVEC_MODE_VECTORING;

  x >>= 1;  /* Q30 to handle numbers > 1.0 */

  MATH->CORDY = (x - 0x10000000U); /* x - 0.25 */
  MATH->CORDX = (x + 0x10000000U); /* x + 0.25 */

  return ((MATH->CORRX >> 14) * 39568); /* Q16 * Q15 */
}

#endif /* end of #if defined (MATH) */
