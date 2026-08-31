/**
 * @file cy_bccu.h
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

#ifndef CY_BCCU_H
#define CY_BCCU_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "cy_common.h"


/**
 * @addtogroup BCCU
 * @brief Brightness and Color Control Unit (BCCU) driver for the CY_FAMILY_PSC1 microcontroller family.
 *
 *   The Brightness and Color Control Unit (BCCU) is a dimming control peripheral for LED lighting applications. The BCCU
 * module can be used to control multiple LED channels. Every channel generates one-bit sigma-delta bit stream with a
 * user adjustable 12-bit average value. The dimming engine changes the brightness gradually (exponential curve) to appear
 * natural to the human eye. It supports color control by adjusting the relative intensity of selected channels using a
 * linear walk scheme for smooth color changes. It also supports high-power multi-channel LED lamps by optionally packing
 * the bitstream. The optional packer which decreases the average rate of output switching by enforcing a defined on-time.
 * The BCCU module generates two trigger signals to the ADC (BCCU_TRIGOUT0 and BCCU_TRIGOU1) to start conversions in a
 * synchronized manner. The module can also be used as a multi-channel digital-analog converter with low-pass filters on the
 * outputs. The BCCU module supports 3 independent dimming engines, 9 independent channels, Trap functions and 2 ADC
 * triggering modes.
 *
 * The driver is divided into global control (BCCU), channel control (BCCU_CH) and dimming control (BCCU_DIM).
 *
 * BCCU features:
 * -# Configuration structure Cy_BCCU_GLOBAL_CONFIG_t and initialization function Cy_BCCU_GlobalInit()
 * -# Allows configuring of clock settings (Fast clock, Bit clock and Dimming clock). Cy_BCCU_SetFastClockPrescaler(),
 * -# Cy_BCCU_SelectBitClock(), Cy_BCCU_SetDimClockPrescaler().
 * -# Allows configuring global trigger settings. Cy_BCCU_ConfigGlobalTrigger()
 * -# Allows enabling multiple channels together. Cy_BCCU_ConcurrentEnableChannels()
 * -# Allows enabling single channel. Cy_BCCU_EnableChannel()
 * -# Allows configuring global dimming level. Cy_BCCU_SetGlobalDimmingLevel()
 * -# Starts linear walk for multiple channels together. Cy_BCCU_ConcurrentStartLinearWalk().
 * -# Starts linear walk for single channel. Cy_BCCU_StartLinearWalk().
 * -# Starts dimming for multiple dimming engines together. Cy_BCCU_ConcurrentStartDimming().
 * -# Starts dimming for single dimming engine. Cy_BCCU_StartDimming().
 *
 * BCCU_CH features:
 * -# Configuration structure (Cy_BCCU_CH_t and initialization function Cy_BCCU_CH_Init()
 * -# Allows selecting dimming engine. Cy_BCCU_CH_SelectDimEngine().
 * -# Allows setting target channel intensity. Cy_BCCU_CH_SetTargetIntensity().
 * -# Allows knowing the status of linear walk completion. Cy_BCCU_IsLinearWalkComplete()
 * -# Allows setting flicker watchdog. Cy_BCCU_CH_EnableFlickerWatchdog().
 * -# Allows configuring packer settings. Cy_BCCU_CH_EnablePacker(), Cy_BCCU_CH_SetPackerThreshold(),
 * Cy_BCCU_CH_SetPackerOffCompare(), Cy_BCCU_CH_SetPackerOffCounte(), Cy_BCCU_CH_SetPackerOnCounter()
 * -# Allows selecting dimming bypass. Cy_BCCU_CH_DisableDimmingBypass()
 *
 * BCCU_DIM features:
 * -# Configuration structure (Cy_BCCU_DIM_t and initialization function Cy_BCCU_DIM_Init()
 * -# Allows setting target dimming engine intensity. Cy_BCCU_DIM_SetTargetDimmingLevel().
 * Cy_BCCU_DIM_SetTargetDimmingLevel
 * -# Allows knowing the status of dimming completion. Cy_BCCU_IsDimmingFinished()
 * -# Allows configuring dimming divider. Cy_BCCU_DIM_SetDimDivider()
 * -# Allows configuring dimming curve. Cy_BCCU_DIM_ConfigDimCurve()
 *
 * <b> Recommended programming sequence: </b>
 * -# Set output passive and active levels using Cy_BCCU_ConcurrentSetOutputPassiveLevel() or Cy_BCCU_SetOutputPassiveLevel()
 * -# Initializes global features using Cy_BCCU_GlobalInit()
 * -# Initializes channel features using Cy_BCCU_CH_Init()
 * -# Initializes dimming engine using Cy_BCCU_DIM_Init()
 * -# Enable channels using Cy_BCCU_ConcurrentEnableChannels() or Cy_BCCU_EnableChannel()
 * -# Enable dimming engines using Cy_BCCU_ConcurrentEnableDimmingEngine() or Cy_BCCU_EnableDimmingEngine()
 * -# Configure channel linear walk prescaler using Cy_BCCU_CH_SetLinearWalkPrescaler()
 * -# Configure dimming divider using Cy_BCCU_DIM_SetDimDivider()
 * -# Set target intensities of channels using Cy_BCCU_CH_SetTargetIntensity()
 * -# Set target dim levels of dimming engines using Cy_BCCU_DIM_SetTargetDimmingLevel()
 * -# Start linear walk of the channels using Cy_BCCU_ConcurrentStartLinearWalk() or Cy_BCCU_StartLinearWalk()
 * -# Start dimming of the dimming engines using Cy_BCCU_ConcurrentStartDimming() or Cy_BCCU_StartDimming()
 * -# Check the status of linear walk completion using Cy_BCCU_IsLinearWalkComplete()
 * -# Check the status of dimming completion Cy_BCCU_IsDimmingFinished()
 * @{
 */

#if defined (BCCU0) || defined(CY_DOXYGEN)

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
/**
 * Defines the status of BCCU driver, to verify the related API calls. Use type \a Cy_BCCU_STATUS_t for this enum.
 */
typedef enum CY_BCCU_STATUS
{
  CY_BCCU_STATUS_SUCCESS = 0U, /**< Operation completed successfully */
  CY_BCCU_STATUS_ERROR = 1U, /**< Operation has some errors */
} Cy_BCCU_STATUS_t;

/**
 * Provides the options to select bit clock mode.
 */
typedef enum CY_BCCU_BCLK_MODE
{
  CY_BCCU_BCLK_MODE_NORMAL = 0U, /**< Normal Mode: Bit clock runs at 1/4 of fast clock */
  CY_BCCU_BCLK_MODE_FAST = 1U, /**< Fast Mode: Bit clock runs at same as fast clock */
} Cy_BCCU_BCLK_MODE_t;

/**
 * Provides the options to select trigger mode.
 */
typedef enum CY_BCCU_TRIGMODE
{
  CY_BCCU_TRIGMODE0 = 0U, /**< Mode0: Trigger on Any Channel using OR logic */
  CY_BCCU_TRIGMODE1 = 1U, /**< Mode1: Trigger on Active channel using round-robin*/
} Cy_BCCU_TRIGMODE_t;

/**
 * Provides the options to select trigger delay, and only be used if Bit clock in Normal mode
 */
typedef enum CY_BCCU_TRIGDELAY
{
  CY_BCCU_TRIGDELAY_NO_DELAY = 0U, /**< BCCU trigger occurs on channel trigger(without delay) */
  CY_BCCU_TRIGDELAY_QUARTER_BIT = 1U, /**< BCCU trigger occurs on 1/4 bit time delayed after channel trigger */
  CY_BCCU_TRIGDELAY_HALF_BIT = 2U, /**< BCCU trigger occurs on 1/2 bit time delayed after channel trigger */
} Cy_BCCU_TRIGDELAY_t;

/**
 * Provides the options to select suspend mode
 */
typedef enum CY_BCCU_SUSPEND_MODE
{
  CY_BCCU_SUSPEND_MODE_IGNORE = 0U, /**< Request ignored, and module cannot get suspended */
  CY_BCCU_SUSPEND_MODE_FREEZE = 1U, /**< All running channels gets stopped, and freeze into a last state (without safe stop) */
  CY_BCCU_SUSPEND_MODE_SAFE_FREEZE = 2U, /**< All running channels gets stopped, and freeze into a last state (with safe stop) */
} Cy_BCCU_SUSPEND_MODE_t;

/**
 * Provides the options to select trap edge
 */
typedef enum CY_BCCU_TRAPEDGE
{
  CY_BCCU_TRAPEDGE_RISING = 0U, /**< Trap on rising edge of the BCCU.TRAPL signal */
  CY_BCCU_TRAPEDGE_FALLING = 1U, /**< Trap on falling edge of the BCCU.TRAPL signal */
} Cy_BCCU_TRAPEDGE_t;

/**
 * Provides the options to enable/disable the events.
 * The members can be combined using 'OR' operator for multiple selection.<br>
 */
typedef enum CY_BCCU_EVENT
{
  CY_BCCU_EVENT_TRIGGER0 = 0x1U, /**< Trigger 0 event */
  CY_BCCU_EVENT_TRIGGER1 = 0x2U, /**< Trigger 1 event */
  CY_BCCU_EVENT_FIFOFULL = 0x4U, /**< FIFO Full event */
  CY_BCCU_EVENT_FIFOEMPTY = 0x8U, /**< FIFO Empty event */
  CY_BCCU_EVENT_TRAP = 0x10U, /**< Trap event */
} Cy_BCCU_EVENT_t;

/**
 * Provides the options to know the status of the event flags.
 * The members can be combined using 'OR' operator for multiple selection.<br>
 */
typedef enum CY_BCCU_EVENT_STATUS
{
  CY_BCCU_EVENT_STATUS_TRIGGER0 = 0x1U, /**< Trigger 0 Event flag status */
  CY_BCCU_EVENT_STATUS_TRIGGER1 = 0x2U, /**< Trigger 1 Event flag status */
  CY_BCCU_EVENT_STATUS_FIFOFULL = 0x4U, /**< FIFO Full Event flag status */
  CY_BCCU_EVENT_STATUS_FIFOEMPTY = 0x8U, /**< FIFO Empty Event flag status */
  CY_BCCU_EVENT_STATUS_TRAP = 0x10U, /**< Trap Event flag status (Without Trap Set) */
  CY_BCCU_EVENT_STATUS_TRAP_STATE = 0x40U, /**< Trap state flag status */
} Cy_BCCU_EVENT_STATUS_t;

/**
 * Provides the options to know the status of trap occurrence
 */
typedef enum CY_BCCU_TRAP_STATUS
{
  CY_BCCU_TRAP_STATUS_DEACTIVE = 0x0U, /**< BCCU module is not in a Trap State */
  CY_BCCU_TRAP_STATUS_ACTIVE = 0x1U, /**< BCCU module is in a Trap State */
} Cy_BCCU_TRAP_STATUS_t;

/**
 * Provides the options to know the current level of trap
 */
typedef enum CY_BCCU_TRAP_LEVEL
{
  CY_BCCU_TRAP_LEVEL_LOW = 0x0U, /**< BCCU.TRAPL is Low */
  CY_BCCU_TRAP_LEVEL_HIGH = 0x1U, /**< BCCU.TRAPL is High */
} Cy_BCCU_TRAP_LEVEL_t;

/**
 * Provides the options to select flicker watchdog enable/disable
 */
typedef enum CY_BCCU_CH_FLICKER_WD
{
  CY_BCCU_CH_FLICKER_WD_DS = 0U, /**< Disable: No control over a sigma-delta modulator output */
  CY_BCCU_CH_FLICKER_WD_EN = 1U, /**< Enable: Limit consecutive zeros at sigma-delta modulator output */
} Cy_BCCU_CH_FLICKER_WD_t;

/**
 * Provides the options to select gating functionality enable/disable, and be used for peak-current control
 */
typedef enum CY_BCCU_CH_GATING_FUNC
{
  CY_BCCU_CH_GATING_FUNC_DISABLE = 0U, /**< Disable: No control over a BCCU module output  */
  CY_BCCU_CH_GATING_FUNC_ENABLE = 1U, /**< Enable: External gating signal which controls BCCU module output  */
} Cy_BCCU_CH_GATING_FUNC_t;

/**
 * Provides the options to bypass dimming engine
 */
typedef enum CY_BCCU_CH_DIMMING_ENGINE_BYPASS
{
  CY_BCCU_CH_DIMMING_ENGINE_BYPASS_DISABLE = 0U, /**< Disable: Brightness = Dimming Level * Intensity */
  CY_BCCU_CH_DIMMING_ENGINE_BYPASS_ENABLE = 1U,   /**< Enable: Brightness = Intensity */
} Cy_BCCU_CH_DIMMING_ENGINE_BYPASS_t;

/**
 * Provides the options to select passive level of the channel output
 */
typedef enum CY_BCCU_CH_ACTIVE_LEVEL
{
  CY_BCCU_CH_ACTIVE_LEVEL_HIGH = 0U, /**< Default passive level of the channel is low */
  CY_BCCU_CH_ACTIVE_LEVEL_LOW = 1U, /**< Default passive level of the channel is high */
} Cy_BCCU_CH_ACTIVE_LEVEL_t;

/**
 * Provides the options to select trigger edge
 */
typedef enum CY_BCCU_CH_TRIG_EDGE
{
  CY_BCCU_CH_TRIG_EDGE_PASS_TO_ACT = 0U,  /**< Trigger on output transition from passive to active */
  CY_BCCU_CH_TRIG_EDGE_ACT_TO_PASS = 1U,  /**< Trigger on output transition from active to passive */
} Cy_BCCU_CH_TRIG_EDGE_t;

/**
 * Provides the options to select source of trap input
 */
typedef enum CY_BCCU_CH_TRAP_IN
{
  CY_BCCU_CH_TRAP_INA = 0x0U,  /**< Trap INA */
  CY_BCCU_CH_TRAP_INB = 0x1U,  /**< Trap INB */
  CY_BCCU_CH_TRAP_INC = 0x2U,  /**< Trap INC */
  CY_BCCU_CH_TRAP_IND = 0x3U,  /**< Trap IND */
  CY_BCCU_CH_TRAP_INE = 0x4U,  /**< Trap INE */
  CY_BCCU_CH_TRAP_INF = 0x5U,  /**< Trap INF */
  CY_BCCU_CH_TRAP_ING = 0x6U,  /**< Trap ING */
  CY_BCCU_CH_TRAP_INH = 0x7U,  /**< Trap INH */
  CY_BCCU_CH_TRAP_INI = 0x8U,  /**< Trap INI */
  CY_BCCU_CH_TRAP_INJ = 0x9U,  /**< Trap INJ */
  CY_BCCU_CH_TRAP_INK = 0xAU, /**< Trap INK */
  CY_BCCU_CH_TRAP_INL = 0xBU, /**< Trap INL */
  CY_BCCU_CH_TRAP_INM = 0xCU, /**< Trap INM */
  CY_BCCU_CH_TRAP_INN = 0xDU, /**< Trap INN */
  CY_BCCU_CH_TRAP_INO = 0xEU, /**< Trap INO */
  CY_BCCU_CH_TRAP_INP = 0xFU, /**< Trap INP */
} Cy_BCCU_CH_TRAP_IN_t;

/**
 * Provides the options to select edge for trap occurrence
 */
typedef enum CY_BCCU_CH_TRAP_EDGE
{
  CY_BCCU_CH_TRAP_EDGE_RISING = 0U, /**< Trap on rising edge of the BCCU.TRAPL signal */
  CY_BCCU_CH_TRAP_EDGE_FALLING = 1U  /**< Trap on falling edge of the BCCU.TRAPL signal */
} Cy_BCCU_CH_TRAP_EDGE_t;

/**
 * Provides the options to select trigger output, and only be used in CY_BCCU_TRIGMODE1
 */
typedef enum CY_BCCU_CH_TRIGOUT
{
  CY_BCCU_CH_TRIGOUT0 = 0U, /**< Trigger occurrence on BCCU_TRIGOUT0 signal */
  CY_BCCU_CH_TRIGOUT1 = 1U, /**< Trigger occurrence on BCCU_TRIGOUT1 signal */
} Cy_BCCU_CH_TRIGOUT_t;

/**
 * Provides the options to select dimming source of the channel
 */
typedef enum CY_BCCU_CH_DIMMING_SOURCE
{
  CY_BCCU_CH_DIMMING_SOURCE_GLOBAL = 7U, /**< Global Dimming Engine */
  CY_BCCU_CH_DIMMING_SOURCE_DE0 = 0U, /**< Dimming Engine 0 */
  CY_BCCU_CH_DIMMING_SOURCE_DE1 = 1U, /**< Dimming Engine 1 */
  CY_BCCU_CH_DIMMING_SOURCE_DE2 = 2U, /**< Dimming Engine 2 */
} Cy_BCCU_CH_DIMMING_SOURCE_t;

/**
 * Provides the options to select exponential dimming curve
 */
typedef enum CY_BCCU_DIM_CURVE
{
  CY_BCCU_DIM_CURVE_COARSE = 0U, /**< Coarse curve: Slope of the linear pieces doubles every time, when it passes specific
  thresholds of 16, 32, 64, 128, 256, 512, 1024, 2048 */
  CY_BCCU_DIM_CURVE_FINE = 1U, /**< Fine Curve: More pieces and different line slopes */
} Cy_BCCU_DIM_CURVE_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/
/**
 * Redefinition of BCCU module structure; pointer to bccu module base address
 */
typedef BCCU_Type Cy_BCCU_t;

/**
 * Redefinition of BCCU module channel structure; pointer to bccu module channel Base address
 */
typedef BCCU_CH_Type Cy_BCCU_CH_t;

/**
 * Redefinition of BCCU module dimming engine structure; pointer to bccu module dimming engine base address
 */
typedef BCCU_DE_Type Cy_BCCU_DIM_t;

/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined(__TASKING__)
#pragma warning 586
#endif

/**
 * Configures a global setting of the BCCU module.
 */
typedef struct CY_BCCU_GLOBAL_CONFIG
{
  union
  {
    struct
    {
      uint32_t trig_mode: 1; /**< Selects trigger Mode. Use type @ref Cy_BCCU_TRIGMODE_t */
      uint32_t : 1;
      uint32_t trig_delay: 2; /**< Selects trigger delay between channel & module trigger. \n Use type @ref Cy_BCCU_TRIGDELAY_t */
      uint32_t : 12;
      uint32_t maxzero_at_output: 12; /**< Configures maximum 0's allowed at modulator output */
    };
    uint32_t globcon; /* Not to use */
  };
  union
  {
    struct
    {
      uint32_t fclk_ps: 12; /**< Configures the ratio between fast clock and module clock */
      uint32_t : 3;
      uint32_t bclk_sel: 1; /**< Selects the bit clock. Use type @ref Cy_BCCU_BCLK_MODE_t */
      uint32_t dclk_ps: 12; /**< Configures the ratio between dimmer clock and module clock */
    };
    uint32_t globclk; /* Not to use */
  };
  uint32_t global_dimlevel; /**< Configures global dimming engine dimming level */
} Cy_BCCU_GLOBAL_CONFIG_t;


/**
 * Configures global trigger settings of the BCCU module.
 */
typedef struct CY_BCCU_TRIG_CONFIG
{
  Cy_BCCU_TRIGMODE_t  mode; /**< Selects global trigger mode which decides when to occur BCCU trigger */
  Cy_BCCU_TRIGDELAY_t delay; /**< Selects global trigger delay between channel trigger & BCCU trigger */
  uint16_t             mask_chans; /**< Channel mask to configure trigger settings for multiple channels <b>For example:</b>
                                        If channel 0 and 7, wants to configure then the channel mask is 01000 0001 = 0x81\n*/
  uint16_t             mask_trig_lines; /**< Trigger line mask */
} Cy_BCCU_TRIG_CONFIG_t;

/**
 * Configures channel settings of the BCCU module.
 */
#ifdef CY_DOXYGEN
typedef struct CY_BCCU_CH_CONFIG
{
  uint32_t pack_thresh: 3; /**< Configures packer threshold value of FIFO */
  uint32_t pack_en: 1; /**< Enables a packed output bitstream */
  uint32_t dim_sel: 3; /**< Selects a dimming engine source of the channel. \n Use type @ref Cy_BCCU_CH_DIMMING_SOURCE_t  */
  uint32_t dim_bypass: 1; /**< Selects dimming engine bypass enable. \n Use type @ref Cy_BCCU_CH_DIMMING_ENGINE_BYPASS_t */
  uint32_t gate_en: 1; /**< Selects gating enable. Use type @ref Cy_BCCU_CH_GATING_FUNC_t */
  uint32_t flick_wd_en: 1; /**< Selects flicker watchdog enable. Use type @ref Cy_BCCU_CH_FLICKER_WD_t */
  uint32_t trig_edge: 1; /**< Selects trigger edge. Use type @ref Cy_BCCU_CH_TRIG_EDGE_t */
  uint32_t force_trig_en: 1; /**< Selects force trigger enable; generates a trigger if modulator output do not change\n for 256 bclk cycles */
  uint32_t pack_offcmp_lev: 8; /**< Configures a packer off-time compare level. When the off-time counter reaches this, the measured on & off time counters are stored into FIFO */
  uint32_t pack_oncmp_lev: 8; /**< Configures a packer on-time compare level. When the on-time counter reaches this,\n
                                        the measured on & off time counters are stored into FIFO */
  uint32_t pack_offcnt_val: 8; /**< Configures an initial packer off-time counter level, only if channel is disabled. <br>
                                        Controls phase shift of the modulator output */
  uint32_t pack_oncnt_val: 8; /**< Configures an initial packer on-time counter level, only if channel is disabled. <br>
                                        Controls phase shift of the modulator output */
} Cy_BCCU_CH_CONFIG_t;
#endif

typedef struct CY_BCCU_CH_CONFIG
{
  union
  {
    struct
    {
      uint32_t pack_thresh: 3; /**< Configures packer threshold value of FIFO */
      uint32_t pack_en: 1; /**< Enables a packed output bitstream */
      uint32_t dim_sel: 3; /**< Selects a dimming engine source of the channel. \n Use type @ref Cy_BCCU_CH_DIMMING_SOURCE_t */
      uint32_t dim_bypass: 1; /**< Selects dimming engine bypass enable. \n Use type @ref Cy_BCCU_CH_DIMMING_ENGINE_BYPASS_t */
      uint32_t gate_en: 1; /**< Selects gating enable. Use type @ref Cy_BCCU_CH_GATING_FUNC_t */
      uint32_t flick_wd_en: 1; /**< Selects flicker watchdog enable. Use type @ref Cy_BCCU_CH_FLICKER_WD_t */
      uint32_t trig_edge: 1; /**< Selects trigger edge. Use type @ref Cy_BCCU_CH_TRIG_EDGE_t */
      uint32_t force_trig_en: 1; /**< Selects force trigger enable; generates a trigger if modulator output do not change\n for 256 bclk cycles */
    };
    uint32_t chconfig; /* Not to use */
  };
  union
  {
    struct
    {
      uint32_t pack_offcmp_lev: 8; /**< Configures a packer off-time compare level. When the off-time counter reaches \n
                                       this, the measured on & off time counters are stored into FIFO */
      uint32_t : 8;
      uint32_t pack_oncmp_lev: 8; /**< Configures a packer on-time compare level. When the on-time counter reaches this,\n
                                        the measured on & off time counters are stored into FIFO */
    };
    uint32_t pkcmp; /* Not to use */
  };
  union
  {
    struct
    {
      uint32_t pack_offcnt_val: 8; /**< Configures an initial packer off-time counter level, only if channel is disabled. <br>
                                        Controls phase shift of the modulator output */
      uint32_t : 8;
      uint32_t pack_oncnt_val: 8; /**< Configures an initial packer on-time counter level, only if channel is disabled. <br>
                                        Controls phase shift of the modulator output */
    };
    uint32_t pkcntr; /* Not to use */
  };
} Cy_BCCU_CH_CONFIG_t;

/**
 * Configures dimming engine settings of the BCCU module.
 */
#ifdef CY_DOXYGEN
typedef struct CY_BCCU_DIM_CONFIG
{
  uint32_t dim_div: 10; /**< Configures a dimming clock divider, used to adjust the fade rate. If 0, the dimming level <br>
                                 as same as target dimming level on shadow transfer */
  uint32_t dither_en: 1; /**< Selects a dither enable. Dithering added for every dimming step if dimming level < 128. */
  uint32_t cur_sel: 1; /**< Selects a type of exponential curve. Use type @ref Cy_BCCU_DIM_CURVE_t. If dither <br>
                                enabled, the configuration is being ignored */
} Cy_BCCU_DIM_CONFIG_t;
#endif

typedef struct CY_BCCU_DIM_CONFIG
{
  union
  {
    struct
    {
      uint32_t dim_div: 10; /**< Configures a dimming clock divider, used to adjust the fade rate. If 0, the dimming level <br>
                                 as same as target dimming level on shadow transfer */
      uint32_t : 6;
      uint32_t dither_en: 1; /**< Selects a dither enable. Dithering added for every dimming step if dimming level < 128. */
      uint32_t cur_sel: 1; /**< Selects a type of exponential curve. Use type @ref Cy_BCCU_DIM_CURVE_t. If dither <br>
                                enabled, the configuration is being ignored */
    };
    uint32_t dtt; /* Not to use */
  };
} Cy_BCCU_DIM_CONFIG_t;

/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
#pragma pop
#elif defined(__TASKING__)
#pragma warning restore
#endif
/*********************************************************************************************************************
 * API PROTOTYPES
 ********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param config Pointer to constant bccu global configuration data structure. Use type @ref Cy_BCCU_GLOBAL_CONFIG_t.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Initializes three main clocks (fast clock, bit clock, dimmer clock) by using \a fclk_ps \a bclk_sel \a dclk_ps parameters
 * and writing into a GLOBCLK register.\n
 * And also configures a trigger mode, trigger delay, maximum 0's allowed at modulator output by writing into a GLOBCON
 * register.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_Init(), Cy_BCCU_DIM_Init()\n\n\n
*/
void Cy_BCCU_GlobalInit (Cy_BCCU_t *const bccu, const Cy_BCCU_GLOBAL_CONFIG_t *const config);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mode Trigger mode selection. \b Range: CY_BCCU_TRIGMODE0, CY_BCCU_TRIGMODE1.\n
 *                             1. CY_BCCU_TRIGMODE0 - Trigger on Any Channel
 *                             2. CY_BCCU_TRIGMODE1 - Trigger on Active channel
 * @param delay Delay to avoid sampling during switching noise. Use type @ref Cy_BCCU_TRIGDELAY_t. \n
 *              \b Range: CY_BCCU_NO_DELAY, CY_BCCU_QUARTER_BIT_DELAY, CY_BCCU_HALF_BIT_DELAY.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures trigger mode and trigger delay by writing register bits GLOBCON.TM, GLOBCON.TRDEL. \a mode and \a delay
 * parameters which decides when to trigger a conversion of vadc module for voltage measurement. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_EnableChannelTrigger(), Cy_BCCU_ReadGlobalTrigger(), Cy_BCCU_ConcurrentConfigTrigger(), Cy_BCCU_GlobalInit(),
 * Cy_BCCU_ReadLastTrigChanNr(), Cy_BCCU_GetChannelOutputLvlAtLastTrigger(), Cy_BCCU_CH_ConfigTrigger()\n\n\n
 */
void Cy_BCCU_ConfigGlobalTrigger(Cy_BCCU_t *const bccu, Cy_BCCU_TRIGMODE_t mode, Cy_BCCU_TRIGDELAY_t delay);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 *
 * @return Source of Trigger mode. \b Range: 0 or 1 \n
 *         0 - Trigger mode 0 (Trigger on Any Channel) \n
 *         1 - Trigger mode 1 (Trigger on Active Channel)\n\n
 * \par<b>Description:</b><br>
 * Retrieves global trigger mode of the BCCU module by reading the register bit GLOBCON_TM. Use Cy_BCCU_TRIGMODE_t type to
 * validate a returned value.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ConfigGlobalTrigger(), Cy_BCCU_ConcurrentConfigTrigger(), Cy_BCCU_GlobalInit()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_ReadGlobalTrigger (Cy_BCCU_t *const bccu)
{
  return (uint32_t)(bccu->GLOBCON & BCCU_GLOBCON_TM_Msk);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param input Trap input selection. Use type @ref Cy_BCCU_TRIGDELAY_t.
 *              \b Range: CY_BCCU_TRIGDELAY_NO_DELAY, CY_BCCU_TRIGDELAY_QUARTER_BIT, CY_BCCU_TRIGDELAY_HALF_BIT.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Selects input of trap functionality by writing register bit GLOBCON_TRAPIS. The trap functionality is used to switch
 * off the connected power devices when trap input becomes active.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetTrapEdge(), Cy_BCCU_ReadTrapInput(), Cy_BCCU_EnableTrap(), Cy_BCCU_ConcurrentEnableTrap()\n\n\n
 */
void Cy_BCCU_SelectTrapInput (Cy_BCCU_t *const bccu, Cy_BCCU_CH_TRAP_IN_t input);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 *
 * @return Source of trap input. \b Range: 0 to 15 \n
 *                           0 - TRAPINA \n
 *                           1 - TRAPINB and so on. \n
 * \par<b>Description:</b><br>
 * Retrieves trap input of the channel by reading the register bit GLOBCON_TRAPIS. Use Cy_BCCU_CH_TRAP_IN_t type to
 * validate a returned value.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SelectTrapInput()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_ReadTrapInput (Cy_BCCU_t *const bccu)
{
  return (uint32_t)(( (bccu->GLOBCON) & BCCU_GLOBCON_TRAPIS_Msk) >> BCCU_GLOBCON_TRAPIS_Pos);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param edge Trap edge selection. Use type @ref Cy_BCCU_CH_TRAP_EDGE_t.  \n
 *              \b Range: CY_BCCU_CH_TRAP_EDGE_RISING, CY_BCCU_CH_TRAP_EDGE_FALLING.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Selects active edge which detects trap on TRAPL signal by writing register bit GLOBCON_TRAPED.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SelectTrapInput(), Cy_BCCU_ReadTrapEdge(), Cy_BCCU_EnableTrap(), Cy_BCCU_ConcurrentEnableTrap()\n\n\n
 */
void Cy_BCCU_SetTrapEdge (Cy_BCCU_t *const bccu, Cy_BCCU_CH_TRAP_EDGE_t edge);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 *
 * @return Trap edge selection. \b Range: 0 or 1 \n
 *                           0 - CY_BCCU_CH_TRAP_EDGE_RISING \n
 *                           1 - CY_BCCU_CH_TRAP_EDGE_FALLING. \n
 * \par<b>Description:</b><br>
 * Retrieves trap edge by reading the register bit GLOBCON_TRAPED. Use Cy_BCCU_CH_TRAP_EDGE_t type to
 * validate a returned value.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetTrapEdge()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_ReadTrapEdge (Cy_BCCU_t *const bccu)
{
  return (uint32_t)(( (bccu->GLOBCON) & BCCU_GLOBCON_TRAPED_Msk) >> BCCU_GLOBCON_TRAPED_Pos);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mode Suspend mode selection. Use type @ref Cy_BCCU_SUSPEND_MODE_t. \n
 *             \b Range: CY_BCCU_SUSPEND_MODE_IGNORE, CY_BCCU_SUSPEND_MODE_FREEZE, CY_BCCU_USPEND_MODE_SAFE_FREEZE. \n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures suspend mode by writing register bit GLOBCON_SUSCFG.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ReadSuspendMode()\n\n\n
 */
void Cy_BCCU_ConfigSuspendMode (Cy_BCCU_t *const bccu, Cy_BCCU_SUSPEND_MODE_t mode);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 *
 * @return Trap edge selection. \b Range: 0, 1, 2 \n
 *              0 - CY_BCCU_SUSPEND_MODE_IGNORE \n
 *              1 - CY_BCCU_SUSPEND_MODE_FREEZE. \n
 *              2 - CY_BCCU_USPEND_MODE_SAFE_FREEZE. \n
 * \par<b>Description:</b><br>
 * Retrieves the state of suspend mode by reading the register bit GLOBCON_TRAPIS. Use Cy_BCCU_SUSPEND_MODE_t type to
 * validate a returned value.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ConfigSuspendMode()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_ReadSuspendMode (Cy_BCCU_t *const bccu)
{
  return (uint32_t)( ((bccu->GLOBCON) & BCCU_GLOBCON_SUSCFG_Msk) >> BCCU_GLOBCON_SUSCFG_Pos);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 *
 * @return Last triggered channel number. \b Range: 0 to 8\n
 *              0 - BCCU Channel 0\n
 *              1 - BCCU Channel 1 and so on.\n
 * \par<b>Description:</b><br>
 * Retrieves last triggered channel number of a BCCU module by reading the register bit GLOBCON_LTRS.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_GetChannelOutputLvlAtLastTrigger(), Cy_BCCU_ConfigGlobalTrigger(), Cy_BCCU_ConcurrentConfigTrigger(),
 * Cy_BCCU_ReadGlobalTrigger(), Cy_BCCU_GlobalInit()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_ReadLastTrigChanNr (Cy_BCCU_t *const bccu)
{
  return (uint32_t)(( (bccu->GLOBCON) & BCCU_GLOBCON_LTRS_Msk) >> BCCU_GLOBCON_LTRS_Pos);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param threshold_no Number of consecutive zeroes at modulator output. \b Range: 0 to 4095.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures number of consecutive zeroes allowed at modulator output (flicker watch-dog number) by writing register
 * bit GLOBCON_WDMBN.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_EnableFlickerWatchdog(), Cy_BCCU_ReadFlickerWDThreshold(), Cy_BCCU_CH_Init(), Cy_BCCU_CH_DisableFlickerWatchdog()\n\n\n
 */
void Cy_BCCU_SetFlickerWDThreshold (Cy_BCCU_t *const bccu, uint32_t threshold_no);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 *
 * @return Number of consecutive zeroes at modulator output. \b Range: 0 to 4095 \n
 * \par<b>Description:</b><br>
 * Retrieves number of consecutive zeroes at modulator output (flicker watchdog number) by reading the register bit
 * GLOBCON_WDMBN.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetFlickerWDThreshold(), Cy_BCCU_CH_EnableFlickerWatchdog(), Cy_BCCU_CH_Init(), Cy_BCCU_CH_DisableFlickerWatchdog()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_ReadFlickerWDThreshold (Cy_BCCU_t *const bccu)
{
  return (uint32_t)(( (bccu->GLOBCON) & BCCU_GLOBCON_WDMBN_Msk) >> BCCU_GLOBCON_WDMBN_Pos);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param div Prescaler factor. \b Range: 0 to 4095.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures trigger functionality clock prescaler factor of a BCCU module by writing register bit GLOBCLK_FCLK_PS.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetDimClockPrescaler(), Cy_BCCU_SelectBitClock(), Cy_BCCU_ReadFastClock()\n\n\n
*/
void Cy_BCCU_SetFastClockPrescaler (Cy_BCCU_t *const bccu, uint32_t div);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 *
 * @return Prescaler factor. \b Range: 0 to 4095
 * \par<b>Description:</b><br>
 * Retrieves fast clock prescaler factor of a BCCU module by reading the register bit GLOBCLK_FCLK_PS. The fast clock is
 * derived from the bccu clock by prescaler factor i.e. fdclk = fbccu_clk / prescaler factor.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetFastClockPrescaler(), Cy_BCCU_ReadBitClock(), Cy_BCCU_ReadDimClock()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_ReadFastClock (Cy_BCCU_t *const bccu)
{
  return (uint32_t)(((bccu->GLOBCLK) & BCCU_GLOBCLK_FCLK_PS_Msk) >> BCCU_GLOBCLK_FCLK_PS_Pos);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param div Prescaler factor. \b Range: 0 to 4095.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures dimmer clock prescaler factor of a BCCU module by writing register bit GLOBCLK_DCLK_PS.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DIM_SetDimDivider(), Cy_BCCU_SetFastClockPrescaler(), Cy_BCCU_SelectBitClock(), Cy_BCCU_ReadDimClock()\n\n\n
 */
void Cy_BCCU_SetDimClockPrescaler (Cy_BCCU_t *const bccu, uint32_t div);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 *
 * @return Prescaler factor. \b Range: 0 to 4095
 * \par<b>Description:</b><br>
 * Retrieves dimmer clock prescaler factor of a BCCU module by reading the register bit GLOBCLK_DCLK_PS. The dim clock is
 * derived from the bccu clock by prescaler factor. \n i.e. <b> fdclk = fbccu_clk / prescaler factor.</b>\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetDimClockPrescaler(), Cy_BCCU_ReadBitClock(), Cy_BCCU_ReadFastClock()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_ReadDimClock (Cy_BCCU_t *const bccu)
{
  return (uint32_t)(((bccu->GLOBCLK) & BCCU_GLOBCLK_DCLK_PS_Msk) >> BCCU_GLOBCLK_DCLK_PS_Pos);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param div Prescaler factor. Use type @ref Cy_BCCU_BCLK_MODE_t.  \n
 *            \b Range: CY_BCCU_BCLK_MODE_NORMAL or CY_BCCU_BCLK_MODE_FAST. \n
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures modulator output (bit-time) clock prescaler factor of a BCCU module by writing register bit GLOBCLK_BCS.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetFastClockPrescaler(), Cy_BCCU_SetDimClockPrescaler(), Cy_BCCU_ReadBitClock()\n\n\n
 */
void Cy_BCCU_SelectBitClock (Cy_BCCU_t *const bccu, Cy_BCCU_BCLK_MODE_t div);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 *
 * @return Prescaler factor. \b Range: 0 or 1 \n
 *                           0 - CY_BCCU_BCLK_MODE_NORMAL \n
 *                           1 - CY_BCCU_BCLK_MODE_FAST \n
 * \par<b>Description:</b><br>
 * Retrieves modulator output (bit-time) clock prescaler factor of a BCCU module by reading the register bit GLOBCLK_BCS.
 * Use Cy_BCCU_BCLK_MODE_t type to validate a returned value.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SelectBitClock(), Cy_BCCU_ReadDimClock(), Cy_BCCU_ReadFastClock()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_ReadBitClock (Cy_BCCU_t *const bccu)
{
  return (uint32_t)(((bccu->GLOBCLK) & BCCU_GLOBCLK_BCS_Msk) >> BCCU_GLOBCLK_BCS_Pos);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mask Channel mask to enable multiple channels.\n
 *        <b>For example:</b> If channel 0, channel 7, channel 1 wants to enable at a same time, \n
 *        then channel mask is 01000 0011 = 0x83\n
 *        ------------------------------------------------------------------------------------------------------\n
 *        | Chan8 | Chan7 | Chan6 | Chan5 | Chan4 | Chan3 | Chan2 | Chan1 | Chan0 |\n
 *        ------------------------------------------------------------------------------------------------------\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables multiple channels at a same time using \a mask by writing a register CHEN.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_EnableChannel(), Cy_BCCU_ConcurrentDisableChannels()\n\n\n
 */
void Cy_BCCU_ConcurrentEnableChannels (Cy_BCCU_t *const bccu, uint32_t mask);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mask Channel mask to disable multiple channels.\n
 *        <b>For example:</b> If channel 0, channel 7, channel 1 wants to disable at a same time, \n
 *        then channel mask is 01000 0011 = 0x83\n
 *        ------------------------------------------------------------------------------------------------------\n
 *        | Chan8 | Chan7 | Chan6 | Chan5 | Chan4 | Chan3 | Chan2 | Chan1 | Chan0 |\n
 *        ------------------------------------------------------------------------------------------------------\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables multiple channels at a same time using \a mask by writing a register CHEN.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ConcurrentEnableChannels(), Cy_BCCU_DisableChannel()\n\n\n
 */
void Cy_BCCU_ConcurrentDisableChannels (Cy_BCCU_t *const bccu, uint32_t mask);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_mask Channel mask to enable multiple channels.\n
 *        <b>For example:</b> If channel 0, channel 7, channel 1 wants to enable at a same time, \n
 *        then channel mask is 01000 0011 = 0x83\n
 *        ------------------------------------------------------------------------------------------------------\n
 *        | Chan8 | Chan7 | Chan6 | Chan5 | Chan4 | Chan3 | Chan2 | Chan1 | Chan0 |\n
 *        ------------------------------------------------------------------------------------------------------\n
 * @param level Passive level selection. Use type @ref Cy_BCCU_CH_ACTIVE_LEVEL_t.\n
 *              \b Range: CY_BCCU_CH_ACTIVE_LEVEL_HIGH or CY_BCCU_CH_ACTIVE_LEVEL_LOW
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures passive levels of multiple channels at a same time using \a mask by writing a register bit CHOCON_CHyOP.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetOutputPassiveLevel()\n\n\n
 */
void Cy_BCCU_ConcurrentSetOutputPassiveLevel(Cy_BCCU_t *const bccu, uint32_t chan_mask, Cy_BCCU_CH_ACTIVE_LEVEL_t level);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mask Channel mask to enable multiple channels trap functionality.\n
 *        <b>For example:</b> If channel 0, channel 7, channel 1 wants to enable a trap functionality at a same time, \n
 *        then channel mask is 01000 0011 = 0x83\n
 *        ------------------------------------------------------------------------------------------------------\n
 *        | Chan8 | Chan7 | Chan6 | Chan5 | Chan4 | Chan3 | Chan2 | Chan1 | Chan0 |\n
 *        ------------------------------------------------------------------------------------------------------\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables multiple channels trap functionality at the same time using \a mask by writing a register bit CHOCON_CHyTPE.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_EnableTrap(), Cy_BCCU_ConcurrentDisableTrap(), Cy_BCCU_SelectTrapInput(), Cy_BCCU_SetTrapEdge()\n\n\n
 */
void Cy_BCCU_ConcurrentEnableTrap (Cy_BCCU_t *const bccu, uint32_t mask);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mask Channel mask to disable multiple channels trap functionality.
 *        <b>For example:</b> If channel 0, channel 7, channel 1 wants to disable a trap functionality at a same time, \n
 *        then channel mask is 01000 0011 = 0x83\n
 *        ------------------------------------------------------------------------------------------------------\n
 *        | Chan8 | Chan7 | Chan6 | Chan5 | Chan4 | Chan3 | Chan2 | Chan1 | Chan0 |\n
 *        ------------------------------------------------------------------------------------------------------\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables multiple channels trap functionality at the same time using \a mask by writing a register bit CHOCON_CHyTPE.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DisableTrap(), Cy_BCCU_ConcurrentEnableTrap()\n\n\n
 */
void Cy_BCCU_ConcurrentDisableTrap (Cy_BCCU_t *const bccu, uint32_t mask);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param trig Pointer to a trigger configuration data structure. Use type @ref Cy_BCCU_TRIG_CONFIG_t.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures global trigger settings: trigger mode, trigger delay, individual trigger lines and channel mask by writing a \n
 * registers GLOBCON and CHTRIG. Trigger mode decides when to generate a BCCU trigger, trigger delay postpones the channel \n
 * trigger by 1/4, or 1/2 bclk cycles\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ConfigGlobalTrigger(), Cy_BCCU_ReadLastTrigChanNr(), Cy_BCCU_ReadGlobalTrigger(), Cy_BCCU_GlobalInit()\n\n\n
 */
void Cy_BCCU_ConcurrentConfigTrigger (Cy_BCCU_t *const bccu, Cy_BCCU_TRIG_CONFIG_t *trig);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mask Channel mask to start a linear walk for multiple channels at a same time.\n
 *        <b>For example:</b> If channel 0, channel 7, channel 1 wants to start a linear walk at a same time, \n
 *        then channel mask is 01000 0011 = 0x83\n
 *        ------------------------------------------------------------------------------------------------------\n
 *        | Chan8 | Chan7 | Chan6 | Chan5 | Chan4 | Chan3 | Chan2 | Chan1 | Chan0 |\n
 *        ------------------------------------------------------------------------------------------------------\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * After channel initialization, the outcome of executing the API starts changing the color smoothly towards to target \n
 * for multiple channels at a same time using \a mask by writing a register CHSTRCON.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_StartLinearWalk(), Cy_BCCU_ConcurrentAbortLinearWalk()\n\n\n
 */
void Cy_BCCU_ConcurrentStartLinearWalk (Cy_BCCU_t *const bccu, uint32_t mask);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mask Channel mask to stop a linear walk for multiple channels at a same time.\n
 *        <b>For example:</b> If channel 0, channel 7, channel 1 wants to abort a linear walk at a same time, \n
 *        then channel mask is 01000 0011 = 0x83\n
 *        ------------------------------------------------------------------------------------------------------\n
 *        | Chan8 | Chan7 | Chan6 | Chan5 | Chan4 | Chan3 | Chan2 | Chan1 | Chan0 |\n
 *        ------------------------------------------------------------------------------------------------------\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * When the linear walk in progress, the outcome of executing the API is stopping the linear walk (i.e. color change)
 * immediately for multiple channels at a same time using \a mask by writing a register CHSTRCON.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_AbortLinearWalk(), Cy_BCCU_ConcurrentStartLinearWalk()\n\n\n
 */
void Cy_BCCU_ConcurrentAbortLinearWalk (Cy_BCCU_t *const bccu, uint32_t mask);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mask Dimming engine mask to enable multiple dimming engine at a same time.\n
 *        <b>For example:</b> If dimming engine 0, channel 2 wants to enable a dimming at a same time,
 *        then dimming engine mask is 0101 = 0x03\n
 *        --------------------------\n
 *        | DE2 | DE1 | DE0 |\n
 *        --------------------------\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables multiple dimming engines at a same time using \a mask by writing a register DEEN.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_EnableDimmingEngine(), Cy_BCCU_ConcurrentDisableDimmingEngine()\n\n\n
 */
void Cy_BCCU_ConcurrentEnableDimmingEngine (Cy_BCCU_t *const bccu, uint32_t mask);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mask Dimming engine mask to disable multiple dimming engine at a same time.\n
 *        <b>For example:</b> If dimming engine 0, channel 2 wants to disable a dimming at a same time,
 *        then dimming engine mask is 0101 = 0x03\n
 *        --------------------------\n
 *        | DE2 | DE1 | DE0 |\n
 *        --------------------------\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables multiple dimming engines at a same time using \a mask by writing a register DEEN.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DisableDimmingEngine(), Cy_BCCU_ConcurrentEnableDimmingEngine()\n\n\n
 */
void Cy_BCCU_ConcurrentDisableDimmingEngine (Cy_BCCU_t *const bccu, uint32_t mask);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mask Dimming engine mask to start a dimming for multiple dimming engines at a same time.\n
 *        <b>For example:</b> If dimming engine 0, channel 2 wants to start a dimming at a same time,
 *        then dimming engine mask is 0101 = 0x03\n
 *        --------------------------\n
 *        | DE2 | DE1 | DE0 |\n
 *        --------------------------\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * After dimming engine initialization, the outcome of executing the API starts changing the brightness towards to target
 * for multiple dimming engines at a same time using \a mask by writing a register DESTRCON.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_StartDimming(), Cy_BCCU_ConcurrentAbortDimming()\n\n\n
 */
void Cy_BCCU_ConcurrentStartDimming (Cy_BCCU_t *const bccu, uint32_t mask);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param mask Dimming engine mask to abort a dimming for multiple dimming engines at a same time.\n
 *        <b>For example:</b> If dimming engine 0, channel 2 wants to abort a dimming at a same time,
 *        then dimming engine mask is 0101 = 0x03\n
 *        --------------------------\n
 *        | DE2 | DE1 | DE0 |\n
 *        --------------------------\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * When the dimming in progress, the outcome of executing the API is stopping the dimming (i.e. fading)
 * immediately for specific dimming engine number \a dim_no by writing a register DESTRCON.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_AbortDimming(), Cy_BCCU_ConcurrentStartDimming()\n\n\n
 */
void Cy_BCCU_ConcurrentAbortDimming (Cy_BCCU_t *const bccu, uint32_t mask);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param level Dimming level of global dimming engine. \b Range: 0 to 4095\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures a global dimming level by writing a register GLOBDIM. This is useful only if global dimming engine selected.
 * Otherwise the configuration is ignored. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DIM_SetTargetDimmingLevel()\n\n\n
 */
void  Cy_BCCU_SetGlobalDimmingLevel (Cy_BCCU_t *const bccu, uint32_t level);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param event Event mask to enable multiple events at a time using ORed values of @ref Cy_BCCU_EVENT_t.\n
 *        <b>For example:</b> If CY_BCCU_EVENT_TRIGGER0, CY_BCCU_EVENT_TRIGGER1, CY_BCCU_EVENT_FIFOEMPTY wants to enable
 *        at a same time,\n then event mask is = <b>(CY_BCCU_EVENT_TRIGGER0 | CY_BCCU_EVENT_TRIGGER1 |
 *        CY_BCCU_EVENT_FIFOEMPTY) </b> \n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables multiple interrupt events at a same time using ORed values of @ref Cy_BCCU_EVENT_t by writing a register EVIER.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DisableInterrupt()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_EnableInterrupt (Cy_BCCU_t *const bccu, uint32_t event)
{
  bccu->EVIER |= event;
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param event Event mask to disable multiple events at a time using ORed values of @ref Cy_BCCU_EVENT_t.\n
 *        <b>For example:</b> If CY_BCCU_EVENT_TRIGGER0, CY_BCCU_EVENT_TRIGGER1, CY_BCCU_EVENT_FIFOEMPTY wants to disable\n
 *         at a same time, then event mask is = <b>(CY_BCCU_EVENT_TRIGGER0 | CY_BCCU_EVENT_TRIGGER1 |
 *         CY_BCCU_EVENT_FIFOEMPTY)</b> \n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables multiple interrupt events at a same time using ORed values of @ref Cy_BCCU_EVENT_t by writing a register EVIER.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_EnableInterrupt()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_DisableInterrupt (Cy_BCCU_t *const bccu, uint32_t event)
{
  bccu->EVIER &= (uint32_t)~(event);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 *
 * @return Interrupt events flags at a same time using ORed values of @ref Cy_BCCU_EVENT_t.
 * \par<b>Description:</b><br>
 * Retrieves interrupt event flags at the same time using ORed values of @ref Cy_BCCU_EVENT_t by reading the register \n
 * EVFR. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetEventFlag()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_ReadEventFlag (Cy_BCCU_t *const bccu)
{
  return (bccu->EVFR);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param flag_type Event flag mask to configure multiple events at a time using ORed values of @ref
 * Cy_BCCU_EVENT_STATUS_t.\n
 *        <b>For example:</b> If CY_BCCU_EVENT_STATUS_TRIGGER0, CY_BCCU_EVENT_STATUS_TRIGGER1, CY_BCCU_EVENT_STATUS_FIFOEMPTY
 * wants to configure at a same time, then event mask is = <b>(CY_BCCU_EVENT_STATUS_TRIGGER0 | CY_BCCU_EVENT_STATUS_TRIGGER1 |
 * CY_BCCU_EVENT_STATUS_FIFOEMPTY)</b> \n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures multiple interrupt event flags at a same time using ORed values of @ref Cy_BCCU_EVENT_STATUS_t by writing a register EVFSR.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ClearEventFlag(), Cy_BCCU_ReadEventFlag()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_SetEventFlag (Cy_BCCU_t *const bccu, uint32_t flag_type)
{
  bccu->EVFSR = flag_type;
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param flag_type event flag mask to clear multiple events at a time using ORed values of @ref
 * Cy_BCCU_EVENT_STATUS_t.\n
 *        <b>For example:</b> If CY_BCCU_EVENT_STATUS_TRIGGER0, CY_BCCU_EVENT_STATUS_TRIGGER1, CY_BCCU_EVENT_STATUS_FIFOEMPTY
 * wants to clear at a same time, then event mask is = <b>(CY_BCCU_EVENT_STATUS_TRIGGER0 | CY_BCCU_EVENT_STATUS_TRIGGER1 |
 * CY_BCCU_EVENT_STATUS_FIFOEMPTY)</b> \n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Clears multiple interrupt event flags at a same time using ORed values of @ref Cy_BCCU_EVENT_STATUS_t by writing a
 * register EVFSR.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetEventFlag()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_ClearEventFlag (Cy_BCCU_t *const bccu, uint32_t flag_type)
{
  bccu->EVFCR = flag_type;
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Specific channel number to enable. \b Range: 0 to 8\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables a specific channel number using \a chan_no by writing a register CHEN.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ConcurrentEnableChannels(), Cy_BCCU_DisableChannel()\n\n\n
 */
void Cy_BCCU_EnableChannel (Cy_BCCU_t *const bccu, uint32_t chan_no);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Specific channel number to disable. \b Range: 0 to 8\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables a specific channel number using \a chan_no by writing a register CHEN.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ConcurrentDisableChannels(), Cy_BCCU_EnableChannel()\n\n\n
 */
void Cy_BCCU_DisableChannel (Cy_BCCU_t *const bccu, uint32_t chan_no);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Specific channel number to enable specific channel. \b Range: 0 to 8\n
 * @param level Passive level selection. Use type @ref Cy_BCCU_CH_ACTIVE_LEVEL_t.\n
 *              \b Range: CY_BCCU_CH_ACTIVE_LEVEL_HIGH or CY_BCCU_CH_ACTIVE_LEVEL_LOW
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures passive level of specific channel using \a chan_no by writing a register bit CHOCON_CHyOP.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ConcurrentSetOutputPassiveLevel()\n\n\n
 */
void Cy_BCCU_SetOutputPassiveLevel(Cy_BCCU_t *const bccu, uint32_t chan_no, Cy_BCCU_CH_ACTIVE_LEVEL_t level);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Channel number to enable specific channel trap functionality. \b Range: 0 to 8
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables specific channel trap functionality using \a chan_no by writing a register bit CHOCON_CHyTPE.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DisableTrap(), Cy_BCCU_ConcurrentEnableTrap(), Cy_BCCU_SelectTrapInput(), Cy_BCCU_SetTrapEdge()\n\n\n
 */
void Cy_BCCU_EnableTrap (Cy_BCCU_t *const bccu, uint32_t chan_no);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Channel number to disable specific channel trap functionality. \b Range: 0 to 8
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables specific channel trap functionality using \a chan_no by writing a register bit CHOCON_CHyTPE.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_EnableTrap(), Cy_BCCU_ConcurrentDisableTrap()\n\n\n
 */
void Cy_BCCU_DisableTrap (Cy_BCCU_t *const bccu, uint32_t chan_no);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Channel number to trigger a specific channel. \b Range: 0 to 8
 * @param trig_line Trigger line number to trigger a vadc. Use type @ref Cy_BCCU_CH_TRIGOUT_t.\n
 *                  \b Range: CY_BCCU_CH_TRIGOUT0 or CY_BCCU_CH_TRIGOUT1
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables specific channel trigger using \a chan_no by writing a register bit CHOCON_CHyTPE.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ConfigGlobalTrigger(), Cy_BCCU_DisableChannelTrigger(), Cy_BCCU_CH_ConfigTrigger()\n\n\n
 */
void Cy_BCCU_EnableChannelTrigger (Cy_BCCU_t *const bccu, uint32_t chan_no, Cy_BCCU_CH_TRIGOUT_t trig_line);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Channel number to trigger a specific channel. \b Range: 0 to 8
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables specific channel trigger using \a chan_no by writing a register bit CHOCON_CHyTPE.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_EnableChannelTrigger()\n\n\n
 */
void Cy_BCCU_DisableChannelTrigger (Cy_BCCU_t *const bccu, uint32_t chan_no);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param config Pointer to constant bccu channel configuration data structure. Use type @ref Cy_BCCU_CH_CONFIG_t.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures dimming engine source, dimming bypass selection, channel trigger edge, flicker watchdog selection and force
 * trigger selection by using \a dim_sel, \a dim_bypass, \a trig_edge, \a flick_wd_en, \a force_trig_en by writing into a
 * CHCONFIG register. And also configures packer settings: threshold, off and on compare levels, initial values of off & on
 * counters, by writing into a CHCONFIG, PKCMP and PKCNTR registers.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_GlobalInit(), Cy_BCCU_DIM_Init()\n\n\n
 */
void Cy_BCCU_CH_Init (Cy_BCCU_CH_t *const channel, const Cy_BCCU_CH_CONFIG_t *const config);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param edge Output transition selection. Use type @ref Cy_BCCU_CH_TRIG_EDGE_t. \n
 *             \b Range: CY_BCCU_CH_TRIG_EDGE_PASS_TO_ACT or CY_BCCU_CH_TRIG_EDGE_ACT_TO_PASS\n
 * @param force_trig_en Forcing a trigger at output. \b Range: 0 or 1\n
 *                      Generates a trigger if modulator output do not change for 256 bclk cycles\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures global trigger settings: trigger edge, force trigger enable by writing a register CHCONFIG.
 * And also configures force trigger enable, generates a trigger if modulator output do not change for 256 bclk cycles
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ConfigGlobalTrigger(), Cy_BCCU_EnableChannelTrigger()\n\n\n
 */
void Cy_BCCU_CH_ConfigTrigger (Cy_BCCU_CH_t *const channel, Cy_BCCU_CH_TRIG_EDGE_t edge, uint32_t force_trig_en);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Specific channel number to start color change \b Range: 0 to 8\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * After channel initialization, the outcome of executing the API starts changing the color smoothly towards to target
 * by writing a register bit CHSTRCON_CHyS.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_AbortLinearWalk(), Cy_BCCU_CH_SetTargetIntensity(), Cy_BCCU_IsLinearWalkComplete(),
 * Cy_BCCU_ConcurrentStartLinearWalk()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_StartLinearWalk (Cy_BCCU_t *const bccu, uint32_t chan_no)
{
  bccu->CHSTRCON |= (uint32_t)(BCCU_CHSTRCON_CH0S_Msk << chan_no);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Specific channel number to stop color change \b Range: 0 to 8\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * When the linear walk in progress, the outcome of executing the API is stopping the linear walk (i.e. color change)
 * immediately for specific channels number using \a mask by writing a register CHSTRCON_CHyA.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_StartLinearWalk(), Cy_BCCU_ConcurrentAbortLinearWalk()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_AbortLinearWalk (Cy_BCCU_t *const bccu, uint32_t chan_no)
{
  bccu->CHSTRCON |= (uint32_t)(BCCU_CHSTRCON_CH0A_Msk << chan_no);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Specific channel number to get an output level on last trigger. \b Range: 0 to 8\n
 *
 * @return Trap channel output level. \b Range: 0 or 1
 * \par<b>Description:</b><br>
 * Retrieves output level of specific channel number when last trigger occurred by reading the register bit LTCHOL_LTOLy. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_ReadLastTrigChanNr(), Cy_BCCU_ConfigGlobalTrigger(), Cy_BCCU_ConcurrentConfigTrigger(),
 * Cy_BCCU_ReadGlobalTrigger(), Cy_BCCU_GlobalInit()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_GetChannelOutputLvlAtLastTrigger (Cy_BCCU_t *const bccu, uint32_t chan_no)
{
  return (uint32_t)((bccu->LTCHOL & (BCCU_LTCHOL_LTOL0_Msk << chan_no)) >> chan_no);
}

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param clk_div Prescaler factor. \b Range: 0 to 1023
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * configure the linear walker clock prescaler factor by writing register bit CHCONFIG_LINPRES.\n\n
 *
 */
void Cy_BCCU_CH_SetLinearWalkPrescaler (Cy_BCCU_CH_t *const channel, uint32_t clk_div);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param chan_no Channel number to know the linear walk completion status. \b Range: 0 to 8\n
 *
 * @return Linear walk completion status. \b Range: 0-Completed or 1-intensity start changing towards the target
 * \par<b>Description:</b><br>
 * Retrieves linear walk completion status for specific channel using \a chan_no by reading the register bit CHSTRCON_CHyS. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SetTargetIntensity(), Cy_BCCU_StartLinearWalk()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_IsLinearWalkComplete (Cy_BCCU_t *const bccu, uint32_t chan_no)
{
  return (uint32_t)((bccu->CHSTRCON & (BCCU_CHSTRCON_CH0S_Msk << chan_no)) >> chan_no);
}

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param ch_int Target channel intensity. \b Range: 0 to 4095
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures target channel intensity by writing register INTS, only be written if no shadow transfer of linear walk.
 * Use Cy_BCCU_IsLinearWalkComplete() to know shadow transfer finished \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_IsLinearWalkComplete(), Cy_BCCU_StartLinearWalk(), Cy_BCCU_CH_ReadIntensity()\n\n\n
 */
void Cy_BCCU_CH_SetTargetIntensity (Cy_BCCU_CH_t *const channel, uint32_t ch_int);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return Current channel intensity. \b Range: 0 or 4095
 * \par<b>Description:</b><br>
 * Retrieves current channel intensity by reading the register INT.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SetTargetIntensity()\n\n\n
 */
uint32_t Cy_BCCU_CH_ReadIntensity (Cy_BCCU_CH_t *const channel);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param thresh Packer threshold value of FIFO. It defines number of queue stages must be filled before output generator
 *               starts generating the pulses. Until that, only off-bits are generated at the output.\n
 * @param off_comp Packer off-time compare level. When the off-time counter reaches this, the measured on off time
 *                 counters are stored into FIFO
 * @param on_comp Packer on-time compare level. When the on-time counter reaches this, the measured on & off time
 *                counters are stored into FIFO
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables packer by writing register bit CHCONFIG_PEN. And also configures packer threshold, off and on compare levels
 * by writing register PKCMP. The main purpose of the packer is to decrease the average rate of switching of the output
 * signal, to decrease the load on external switching circuits and improve EMC behavior\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_DisablePacker(), Cy_BCCU_CH_SetPackerThreshold(), Cy_BCCU_CH_SetPackerOffCompare(), Cy_BCCU_CH_SetPackerOnCompare\n\n\n
 */
void Cy_BCCU_CH_EnablePacker (Cy_BCCU_CH_t *const channel, uint32_t thresh, uint32_t off_comp, uint32_t on_comp);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param val Packer threshold value of FIFO
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures packer threshold by writing register bit CHCONFIG_PKTH\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SetPackerOffCompare(), Cy_BCCU_CH_SetPackerOnCompare, Cy_BCCU_CH_ReadPackerThreshold()\n\n\n
 */
void Cy_BCCU_CH_SetPackerThreshold (Cy_BCCU_CH_t *const channel, uint32_t val);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param level Packer off-time compare level. When the off-time counter reaches this, the measured on & off time counters
 *              are stored into FIFO
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures packer off compare level by writing register bit PKCMP_OFFCMP\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SetPackerThreshold(), Cy_BCCU_CH_SetPackerOnCompare(), Cy_BCCU_CH_SetPackerOffCounter(),
 * Cy_BCCU_CH_ReadPackerOffCompare()\n\n\n
 */
void Cy_BCCU_CH_SetPackerOffCompare (Cy_BCCU_CH_t *const channel, uint32_t level);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param level Packer on-time compare level. When the on-time counter reaches this, the measured on & off time counters
 *              are stored into FIFO
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures packer on compare level by writing register bit PKCMP_ONCMP\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SetPackerThreshold(), Cy_BCCU_CH_SetPackerOffCompare(), Cy_BCCU_CH_SetPackerOnCounter(),
 * Cy_BCCU_CH_ReadPackerOnCompare()\n\n\n
 */
void Cy_BCCU_CH_SetPackerOnCompare (Cy_BCCU_CH_t *const channel, uint32_t level);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return Packer threshold value of FIFO
 * \par<b>Description:</b><br>
 * Retrieves packer threshold value by reading the register bit CHCONFIG_PKTH.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SetPackerThreshold()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_CH_ReadPackerThreshold (Cy_BCCU_CH_t *const channel)
{
  return (uint32_t)((channel->CHCONFIG) & BCCU_CH_CHCONFIG_PKTH_Msk);
}

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return Packer off-time compare level. When the off-time counter reaches this, the measured on & off time counters
 *         are stored into FIFO
 * \par<b>Description:</b><br>
 * Retrieves packer off compare level by reading the register bit PKCMP_OFFCMP.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SetPackerOffCompare()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_CH_ReadPackerOffCompare (Cy_BCCU_CH_t *const channel)
{
  return (uint32_t)((channel->PKCMP) & BCCU_CH_PKCMP_OFFCMP_Msk);
}

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return Packer on-time compare level. When the on-time counter reaches this, <br>
 *         the measured on & off time counters are stored into FIFO
 * \par<b>Description:</b><br>
 * Retrieves packer on compare level by reading the register bit PKCMP_ONCMP.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SetPackerOnCompare()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_CH_ReadPackerOnCompare (Cy_BCCU_CH_t *const channel)
{
  return (uint32_t)(((channel->PKCMP) & BCCU_CH_PKCMP_ONCMP_Msk) >> BCCU_CH_PKCMP_ONCMP_Pos);
}

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables packer by clearing writing register bit CHCONFIG_PEN. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_EnablePacker()\n\n\n
 */
void Cy_BCCU_CH_DisablePacker (Cy_BCCU_CH_t *const channel);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param cnt_val Configures an initial packer off-time counter level, only if channel is disabled. Controls phase
 *                shift of the modulator output
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures packer initial off counter value by writing register bit PKCNTR_OFFCNTVAL\n
 * Note: Shall only be called if channel disabled.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SetPackerOnCounter(), Cy_BCCU_CH_SetPackerOffCompare()\n\n\n
 */
void Cy_BCCU_CH_SetPackerOffCounter (Cy_BCCU_CH_t *const channel, uint32_t cnt_val);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param cnt_val Configures an initial packer on-time counter level, only if channel is disabled. Controls phase shift
 *                of the modulator output
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures packer initial on counter value by writing register bit PKCNTR_ONCNTVAL\n
 * Note: Shall only be called if channel disabled.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SetPackerOffCounter(), Cy_BCCU_CH_SetPackerOnCompare()\n\n\n
 */
void Cy_BCCU_CH_SetPackerOnCounter (Cy_BCCU_CH_t *const channel, uint32_t cnt_val);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 * @param sel Selects a dimming engine source of the channel. Use type @ref Cy_BCCU_CH_DIMMING_SOURCE_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures dimming engine source by writing register bit CHCONFIG_DSEL\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_EnableDimmingBypass()\n\n\n
 */
void Cy_BCCU_CH_SelectDimEngine (Cy_BCCU_CH_t *const channel, Cy_BCCU_CH_DIMMING_SOURCE_t sel);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables dimming engine bypass by writing register bit CHCONFIG_DBP. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_SelectDimEngine(), Cy_BCCU_CH_DisableDimmingBypass()\n\n\n
 */
void Cy_BCCU_CH_EnableDimmingBypass (Cy_BCCU_CH_t *const channel);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables dimming engine bypass by clearing register bit CHCONFIG_DBP. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_EnableDimmingBypass()\n\n\n
 */
void Cy_BCCU_CH_DisableDimmingBypass (Cy_BCCU_CH_t *const channel);

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables gating feature by writing register bit CHCONFIG_GEN. The gating feature is used to enable fast control schemes,
 * such as peak-current control and this has been controlled by Analog Comparator module.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_DisableGating()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_CH_EnableGating (Cy_BCCU_CH_t *const channel)
{
  channel->CHCONFIG |= (uint32_t)(BCCU_CH_CHCONFIG_GEN_Msk);
}

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables gating feature by writing register bit CHCONFIG_GEN. The gating feature is used to enable/disable fast control
 * schemes, such as peak-current control and this has been controlled by Analog Comparator module. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_CH_EnableGating()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_CH_DisableGating (Cy_BCCU_CH_t *const channel)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_GEN_Msk);
}

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables flicker watchdog by writing register bit CHCONFIG_WEN. And limits the sigma-delta modulator output\n
 * according to Watchdog threshold\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetFlickerWDThreshold(), Cy_BCCU_ReadFlickerWDThreshold(), Cy_BCCU_CH_Init(),
 * Cy_BCCU_CH_DisableFlickerWatchdog()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_CH_EnableFlickerWatchdog (Cy_BCCU_CH_t *const channel)
{
  channel->CHCONFIG |= (uint32_t)(BCCU_CH_CHCONFIG_WEN_Msk);
}

/**
 *
 * @param channel Base address of the bccu channel. \b Range: BCCU0_CH0, BCCU0_CH1..
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables flicker watchdog by writing register bit CHCONFIG_WEN. No limits the sigma-delta modulator output
 * according to Watchdog threshold\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetFlickerWDThreshold(), Cy_BCCU_ReadFlickerWDThreshold(), Cy_BCCU_CH_Init(),
 * Cy_BCCU_CH_EnableFlickerWatchdog()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_CH_DisableFlickerWatchdog (Cy_BCCU_CH_t *const channel)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_WEN_Msk);
}

/**
 *
 * @param dim_engine Base address of the bccu dimming engine. \b Range: BCCU0_DE0, BCCU0_DE1, BCCU0_DE2
 * @param config Pointer to constant dimming engine configuration data structure. Use type @ref Cy_BCCU_DIM_CONFIG_t.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures dimming clock divider to adjust the fade rate, dither selection and exponential curve selection using \a
 * dim_div, \a dither_en, \a cur_sel parameters and by writing into a DTT register.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_GlobalInit(), Cy_BCCU_CH_Init()\n\n\n
 */
void Cy_BCCU_DIM_Init (Cy_BCCU_DIM_t *const dim_engine, const Cy_BCCU_DIM_CONFIG_t *const config);

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param dim_no Specific dimming engine number to enable. \b Range: 0 to 2\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables a specific dimming engine number using \a dim_no by writing a register bit DEEN_EDEy.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DisableDimmingEngine(), Cy_BCCU_ConcurrentEnableDimmingEngine()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_EnableDimmingEngine (Cy_BCCU_t *const bccu, uint32_t dim_no)
{
  bccu->DEEN |= (uint32_t)(BCCU_DEEN_EDE0_Msk << dim_no);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param dim_no Specific dimming engine number to disable. \b Range: 0 to 2\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables a specific dimming engine number using \a dim_no by clearing a register bit DEEN_EDEy.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_EnableDimmingEngine(), Cy_BCCU_ConcurrentDisableDimmingEngine()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_DisableDimmingEngine (Cy_BCCU_t *const bccu, uint32_t dim_no)
{
  bccu->DEEN &= ~(uint32_t)(BCCU_DEEN_EDE0_Msk << dim_no);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param dim_no Specific dimming engine number to start dimming process \b Range: 0 to 2\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * After dimming engine initialization, the outcome of executing the API starts changing the brightness towards to target
 * by writing a register bit DESTRCON_DEyS.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_AbortDimming(), Cy_BCCU_ConcurrentStartDimming()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_StartDimming (Cy_BCCU_t *const bccu, uint32_t dim_no)
{
  bccu->DESTRCON = (uint32_t)(BCCU_DESTRCON_DE0S_Msk << dim_no);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param dim_no Specific dimming engine number to stop dimming process \b Range: 0 to 2\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * When the dimming in progress, the outcome of executing the API is stopping the dimming (i.e. fading)
 * immediately for specific dimming engine number \a dim_no by writing a register bit DESTRCON_DEyA.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_StartDimming(), Cy_BCCU_ConcurrentAbortDimming()\n\n\n
 */
__STATIC_INLINE void Cy_BCCU_AbortDimming (Cy_BCCU_t *const bccu, uint32_t dim_no)
{
  bccu->DESTRCON = (uint32_t)(BCCU_DESTRCON_DE0A_Msk << dim_no);
}

/**
 *
 * @param bccu Base address of the bccu module. \b Range: BCCU0
 * @param dim_no Specific dimming engine number to know the dimming status. \b Range: 0 to 2\n
 *
 * @return Dimming completion status. \b Range: 0-Completed or 1-start change towards the target
 * \par<b>Description:</b><br>
 * Retrieves dimming completion status for specific dimming engine number using \a dim_no by reading the register bit
 * DESTRCON_DEyS. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DIM_SetTargetDimmingLevel(), Cy_BCCU_StartDimming()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_IsDimmingFinished (Cy_BCCU_t *const bccu, uint32_t dim_no)
{
  return (uint32_t)((bccu->DESTRCON & (BCCU_DESTRCON_DE0S_Msk << dim_no)) >> dim_no);
}

/**
 *
 * @param dim_engine Base address of the bccu dimming engine. \b Range: BCCU0_DE0, BCCU0_DE1, BCCU0_DE2
 * @param level Target dimming level. \b Range: 0 to 4095
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures target dimming level by writing register DLS, only be written if no shadow transfer of dimming.
 * Use Cy_BCCU_IsDimmingFinished() to know shadow transfer finished \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_StartDimming(), Cy_BCCU_IsDimmingFinished(), Cy_BCCU_SetGlobalDimmingLevel(), \n
 * Cy_BCCU_DIM_ReadDimmingLevel()\n\n\n
 */
void Cy_BCCU_DIM_SetTargetDimmingLevel (Cy_BCCU_DIM_t *const dim_engine, uint32_t level);

/**
 *
 * @param dim_engine Base address of the bccu dimming engine. \b Range: BCCU0_DE0, BCCU0_DE1, BCCU0_DE2
 *
 * @return Current dimming level. \b Range: 0 or 4095
 * \par<b>Description:</b><br>
 * Retrieves current dimming level by reading the register DE_DL_DLEV.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DIM_SetTargetDimmingLevel()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_DIM_ReadDimmingLevel (Cy_BCCU_DIM_t *const dim_engine)
{
  return (uint32_t)(dim_engine->DL & BCCU_DE_DL_DLEV_Msk);
}

/**
 *
 * @param dim_engine Base address of the bccu dimming engine. \b Range: BCCU0_DE0, BCCU0_DE1, BCCU0_DE2
 * @param div Dimming clock divider, used to adjust the fade rate. If 0, the dimming level
              as same as target dimming level on shadow transfer\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures dimming clock divider by writing register bit DE_DTT_DIMDIV.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_SetDimClockPrescaler(), Cy_BCCU_DIM_ReadDimDivider()\n\n\n
 */
void Cy_BCCU_DIM_SetDimDivider (Cy_BCCU_DIM_t *const dim_engine, uint32_t div);

/**
 *
 * @param dim_engine Base address of the bccu dimming engine. \b Range: BCCU0_DE0, BCCU0_DE1, BCCU0_DE2
 *
 * @return Dimming clock divider value. \b Range: 0 to 1023
 * \par<b>Description:</b><br>
 * Retrieves dimming clock divider value by reading the register BCCU_DE_DTT_DIMDIV.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DIM_SetDimDivider()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_DIM_ReadDimDivider(Cy_BCCU_DIM_t *const dim_engine)
{
  return (uint32_t)(dim_engine->DTT & BCCU_DE_DTT_DIMDIV_Msk);
}

/**
 *
 * @param dim_engine Base address of the bccu dimming engine. \b Range: BCCU0_DE0, BCCU0_DE1, BCCU0_DE2
 * @param dither_en Dither enable. Dithering added for every dimming step if dimming level < 128. <br>
 * @param sel Type of exponential curve. Use type @ref Cy_BCCU_DIM_CURVE_t. Note: If dither
              enabled, the configuration is being ignored\n
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures dimming clock curve by writing register bit DE_DTT_CSEL.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DIM_Init()\n\n\n
 */
void Cy_BCCU_DIM_ConfigDimCurve (Cy_BCCU_DIM_t *const dim_engine, uint32_t dither_en, Cy_BCCU_DIM_CURVE_t sel);

/**
 *
 * @param dim_engine Base address of the bccu dimming engine. \b Range: BCCU0_DE0, BCCU0_DE1, BCCU0_DE2
 *
 * @return Type of exponential curve \b Range: CY_BCCU_DIM_CURVE_COARSE or CY_BCCU_DIM_CURVE_FINE
 * \par<b>Description:</b><br>
 * Retrieves exponential curve type by reading the register bit BCCU_DE_DTT_CSEL. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DIM_ConfigDimCurve(), Cy_BCCU_IsDitherEnable()\n\n\n
 */
__STATIC_INLINE Cy_BCCU_DIM_CURVE_t Cy_BCCU_DIM_GetDimCurve (Cy_BCCU_DIM_t *const dim_engine)
{
  return (Cy_BCCU_DIM_CURVE_t)((dim_engine->DTT & BCCU_DE_DTT_CSEL_Msk) >> BCCU_DE_DTT_CSEL_Pos);
}

/**
 *
 * @param dim_engine Base address of the bccu dimming engine. \b Range: BCCU0_DE0, BCCU0_DE1, BCCU0_DE2
 *
 * @return Dither enable status. \b Range: 1-Enabled or 0-Disabled
 * \par<b>Description:</b><br>
 * Retrieves dither enable status by reading the register bit BCCU_DE_DTT_DTEN. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * Cy_BCCU_DIM_ConfigDimCurve(), Cy_BCCU_DIM_GetDimCurve()\n\n\n
 */
__STATIC_INLINE uint32_t Cy_BCCU_IsDitherEnable(Cy_BCCU_DIM_t *const dim_engine)
{
  return (uint32_t)((dim_engine->DTT & BCCU_DE_DTT_DTEN_Msk) >> BCCU_DE_DTT_DTEN_Pos);
}

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* BCCU0 */

#endif /* CY_BCCU_H */
