/**
 * @file cy_pdl_personalities.h
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

/**
 * @defgroup PERSONALITIES Personalities Reference
 *
 * \section section_personality_introduction Personality Introduction
 * The PSC1M3 Peripheral Library includes a set of files to enable the ModusToolbox™ Device Configurator functionality.
 * These files are known as personalities. They describe the GUI to the microcontroller hardware
 * resources for the Device Configurator in the xml format. Each hardware resource of a microcontroller has its
 * own personality.<br>
 * Based on the user settings, personalities generate initialization code (configuration structures and initialization API calls) executed within the init_cycfg_all() function.
 * The init_cycfg_all() function is usually executed within the cybsp_init() function or may be called directly in the main() function by the user.
 * The system clock personalities generate a strong definition of the SystemCoreClockSetup() function executed at a startup (before jumping to main.c, not within the init_cycfg_all() unlike other ModusToolbox™ PDL products).
 *
 * For more details, refer to:<br>
 * - ModusToolbox™ <a href="https://www.infineon.com/dgdl/Infineon-ModusToolbox_Device_Configurator_Guide-Software-v01_00-EN.pdf" > <b>Device Configurator guide.</b> </a> <br>
 *
 * \subsection subsection_personality_common_groups Common Groups
 * The parameters of microcontroller resources are allocated in groups.
 * Most of the personalities have groups named:<b> Documentation, Connections, Advanced.</b>
 *
 * \anchor paragraph_personality_documentation \par Documentation
 * \parblock
 * The "Documentation" group displays one or more links to the documentation:
 * - <b>API Help:</b> is the link to the application programing interface (API) documentation of the hardware resource to which the current displayed personality is related.
 * - <b>Personality Help:</b> is the link to the documentation of the current personality.
 * \endparblock
 *
 * \anchor paragraph_personality_connections \par Connections
 * \parblock
 * The "Connections" group contains the parameters, which link different personalities and different hardware resources.
 * This way, configurations chains are formed. <br>
 * \endparblock
 *
 * \anchor paragraph_personality_advanced \par Advanced
 * \parblock
 * The "Advanced" group may contain all or one of the listed parameters:
 *
 * - <b>Start After Initialization:</b> If enabled, the peripheral will start at the end of the peripheral initialization.
 * - <b>Store Config in Flash:</b> Controls whether the configuration structure is stored in flash (const, true) or SRAM (not const, false).
 * \endparblock
 *
 * \section section_personality_peripheral Peripheral Personalities
 *
 * The following peripheral personalities are available:
 * - \subpage page_personality_pin_1_0 "Pin Personality"
 * - \subpage page_personality_ccu4_1_0 "CCU4 Personality"
 * - \subpage page_personality_dsd_1_0 "DSD Personality"
 * - \subpage page_personality_eru_1_0 "ERU Personality"
 * - \subpage page_personality_eth_0_5 "Ethernet Personality"
 * - \subpage page_personality_hrpwm_1_0 "HRPWM Personality"
 * - \subpage page_personality_posif_1_0 "POSIF Personality"
 * - \subpage page_personality_uart_1_0 "UART Personality"
 * - \subpage page_personality_wdt_1_0 "WDT Personality"
 */
