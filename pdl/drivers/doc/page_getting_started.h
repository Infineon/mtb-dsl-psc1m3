/**
 * @page page_getting_started PDL Getting Started
 *
 * @tableofcontents
 *
 * The PSOC C1 Peripheral Driver Library (PDL) consists of low-level drivers for the PSC1M product family peripherals. <br>
 * Built on top of the Cortex Microcontroller Software Interface Standard (CMSIS), it provides access to all peripheral features. <br>
 *
 * @section support Supported devices and toolchains
 * The following 32-Bit Microcontrollers based on ARM Cortex-M0+ are supported:
 * - PSC1M2 series
 * - PSC1M3 series
 *
 * The following toolchains are supported:
 * - GCC Compiler 14.2.1
 * - IAR Compiler 9.50.2
 * - ARM Compiler 6.22
 *
 * @section overview Overview
 * PSOC C1 PDL consists of routines and data structures which cover all peripheral functions.
 *
 * It provides a peripheral register abstraction by a set of stateless APIs. It is possible to write a full application without a single peripheral register access.
 *
 * It is built on top of Cortex Microcontroller Software Interface Standard (CMSIS).
 *
 * @section coding Coding Rules and Conventions
 * This section describes the set of coding rules and conventions applied to the PSOC C1 Peripheral Driver Library:
 * - Strict C99 usage, in addition unions and bitfields are used for more compact code and data section.
 * - Use only standard data types
 * - Use enumerations in favor of defines
 * - Naming convention:
 *   - Variables use only lower case, underscore separated words.
 *   - Functions use CamelCase convention.
 * - Non blocking APIs for better integration in event driven applications. Polling is supported as well by API.
 * - Runtime error checking can be enabled by user
 * - CY_ASSERT() used to check input parameters of functions, result of calculations, ... . @note CY_ASSERT() is provided by core-lib asset
 * - Each driver defines its own namespace: every function and type definition is prefixed by a unique identifier, i.e. Cy_VADC
 *
 * PSOC C1 Peripheral Driver Library follows an object oriented approach, where instances of peripherals are treated as objects = attributes + functions.<br>
 * Attributes are the peripheral data structures defined in device header file. These gets configured using configuration data structures using initialization functions.<br>
 * Peripheral functions take a pointer to the peripheral data struct as the first argument.<br>
 * PSOC C1 PDL uses naming conventions to bind the data struct and the functions that operate on it, i.e. Cy_PERI_DoSomething(Cy_PERI_t const *peri, ...);
 *
 * Typically a driver contains the following functionality:
 * 1. Initialization function
 * @code
 * void Cy_PERI_Init(Cy_PERI_t *const peri, const Cy_PERI_CONFIG_t const *config);
 * @endcode
 *
 * 2. Enable/disable peripheral functions
 * @code
 * void Cy_PERI_Enable(Cy_PERI_t *const peri);
 * void Cy_PERI_Disable(Cy_PERI_t *const peri);
 * @endcode
 *
 * 3. Connectivity functions
 * @code
 * void Cy_PERI_SetInputSource(Cy_PERI_t *const peri, ...);
 * @endcode
 *
 * 4. Event handling functions
 * @code
 * void Cy_PERI_Enable(Cy_PERI_t const *peri);
 * void Cy_PERI_Disable(Cy_PERI_t const *peri);
 * void Cy_PERI_EnableEvent(Cy_PERI_t const *peri);
 * void Cy_PERI_DisableEvent(Cy_PERI_t const *peri);
 * void Cy_PERI_TriggerEvent(Cy_PERI_t const *peri);
 * uint32_t Cy_PERI_GetEventStatus(Cy_PERI_t const *peri);
 * void Cy_PERI_SetInterruptNode(Cy_PERI_t const *peri);
 * @endcode
 *
 * 5. Control functions
 * @code
 * void Cy_PERI_Start(Cy_PERI_t const *peri);
 * void Cy_PERI_Stop(Cy_PERI_t const *peri);
 * void Cy_PERI_Suspend(Cy_PERI_t const *peri);
 * void Cy_PERI_Resume(Cy_PERI_t const *peri);
 * @endcode
 *
 * 6. Get/setters functions, run time API to obtain or modify state of peripheral
 * @code
 * void Cy_PERI_SetSomething(Cy_PERI_t const *peri, ...);
 * uint32_t Cy_PERI_GetSomething(Cy_PERI_t const *peri);
 * @endcode
 *
 * 7. IRQ Handlers are provided which can be used optionally by the user to make easier the coding
 * @code
 * void Cy_PERI_IRQHandler(uint32_t sr_num);
 * @endcode
 *
 * A peripheral driver is structured as:
 * - Common API valid for all the supported devices, i.e. cy_gpio.h and cy_gpio.c
 * - Maps files to help user to configure connectivity, i.e. cy_gpio_map.h already has defined all the pins available depending on the target device selected.
 *
 * The peripheral driver abstracts the target device, helping the user in code porting to another family or device.
 *
 * @section usage How to use the PSOC C1 Peripheral Driver Library
 * The following steps are required:
 * 1. Include header file of required peripheral. No need to include device specific peripheral header files,
      this is done automatically using the information of the selected target device.
 * 2. Peripheral configuration and initialization.
 * 3. Connectivity configuration.
 * 4. Event/interrupt configuration.
 * 5. Start operation.
 * 6. IO configuration.
 * 7. Manage peripheral.
 *
 * @code
 * // Include files
 * #include "cy_gpio.h"
 * #include "cy_i2c.h"
 * ...
 * int main(void)
 * {
 *   // Peripheral configuration and initialization.
 *   Cy_I2C_CH_CONFIG_t i2c_cfg;
 *   i2c_cfg.baudrate = 400000U;
 *   Cy_I2C_CH_Init(CY_I2C0_CH0, &i2c_cfg);
 *
 *   // Connectivity configuration.
 *   Cy_I2C_CH_SetInputSource(CY_I2C0_CH0, CY_I2C_CH_INPUT_SDA, USIC0_C0_DX0_P1_1);
 *   Cy_I2C_CH_SetInputSource(CY_I2C0_CH0, CY_I2C_CH_INPUT_SCL, USIC0_C0_DX1_P1_0);
 *
 *   // Event/interrupt configuration.
 *   Cy_I2C_CH_EnableEvent(CY_I2C0_CH0, CY_I2C_CH_EVENT_ARBITRATION_LOST);
 *   NVIC_SetPriority(USIC0_0_IRQn, 3);
 *   NVIC_EnableIRQ(USIC0_0_IRQn);
 *
 *   // Start operation.
 *   Cy_I2C_CH_Start(CY_I2C0_CH0);
 *
 *   // IO configuration.
 *   Cy_GPIO_SetMode(P1_1, CY_GPIO_MODE_OUTPUT_OPEN_DRAIN);
 *   Cy_GPIO_SetMode(P1_0, CY_GPIO_MODE_OUTPUT_OPEN_DRAIN);
 *   ...
 *   // Manage peripheral
 *   Cy_I2C_CH_MasterStart(CY_I2C0_CH0, IO_EXPANDER_ADDRESS, CY_I2C_CH_CMD_WRITE);
 *   while((Cy_I2C_CH_GetStatusFlag(CY_I2C0_CH0) & CY_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
 *   {
 *   // wait for ACK
 *   }
 *   Cy_I2C_CH_ClearStatusFlag(CY_I2C0_CH0, CY_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
 *   ...
 *
 * @endcode
 * @subsection devices Device Names
 * The selection of the target device is done through preprocessor macro, i.e. -DPSC1M3DBLGQ0AT (refer to devices names below)
 *
 * PSC1M2 Series device names:
 * - PSC1M2DBTBQ0AT
 * - PSC1M2EBTBQ0AT
 * - PSC1M2FBTBQ0AT
 * - PSC1M2DBLEQ0AT
 * - PSC1M2EBLEQ0AT
 * - PSC1M2FBLEQ0AT
 * - PSC1M2DBLGQ0AT
 * - PSC1M2EBLGQ0AT
 * - PSC1M2FBLGQ0AT
 * - PSC1M2DBABQ0AT
 * - PSC1M2EBABQ0AT
 * - PSC1M2FBABQ0AT
 *
 * PSC1M3 Series device names:
 * - PSC1M3DBLGQ0AT
 * - PSC1M3EBLGQ0AT
 * - PSC1M3FBLGQ0AT
 * - PSC1M3DBABQ0AT
 * - PSC1M3EBABQ0AT
 * - PSC1M3FBABQ0AT
 *
 * @section files Directories and Files
 *
 * The following diagram shows the most important directories and files provided in the distribution.
 *
 * @verbatim
 * mtb-psc1m3                           - PSOC C1 PDL root directory
 * +-- CMSIS/Infineon                        - Includes device header files, system and startup files
 * +-- Newlib                                - C standard library implementation intended for use on embedded systems
 * +-- drivers                                - PSOC C1 Peripheral Driver Library root directory
 * |   +-- doc                               - Doxygen configuration files to generate documentation
 * |   +-- inc                               - PSOC C1 Peripheral Driver Library interface files
 * |   +-- src                               - PSOC C1 Peripheral Driver Library implementation files
 * +-- device-info                           - Configuration files that defines a resource behavior for ModusToolbox Device Configurator
 * +-- docs                                  - PSOC C1 PDL documentation generated with Doxygen
 * +-- props.json                            - Device description file for ModusToolbox Device Configurator
 * +-- README.md                             - Description of the library
 * +-- RELEASE.md                            - Description of the changes in the release @endverbatim
 *
 * @section examples PSOC C1 PDL examples
 *
 * The PSOC C1 PDL examples could be found at https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software
 *
 * Create the project and open it using one of the following:
 *
 * @subsection mtbide In Eclipse IDE for ModusToolbox
 *
 * 1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox Application**).
 *    This launches the [Project Creator](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Project_Creator_2.0_User_Guide-UserManual-v01_00-EN.pdf?fileId=8ac78c8c8386267f0183a973003f59b3&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files&redirId=180675) tool.
 * 2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.
 * 3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.
 * 4. Enter the local path in the **Application(s) Root Path** field to indicate where the application needs to be created.
 * 5. Click **Create** to complete the application creation process.
 *
 * For more details, see the Eclipse IDE for [ModusToolbox User Guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_3.0_Eclipse_IDE_User_Guide-GettingStarted-v01_00-EN.pdf?fileId=8ac78c8c8386267f0183a8d7043b58ee&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files&redirId=188241)
 *
 * @subsection cli In Command-line Interface (CLI)
 *
 * ModusToolbox provides the Project Creator as both a GUI tool and a command line tool to easily create one or more
 * ModusToolbox applications. See the "Project Creator Tools" section of the
 * [ModusToolbox User Guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_3.0_Tools_Package_User_Guide-GettingStarted-v01_00-EN.pdf?fileId=8ac78c8c8386267f0183a8e9720c5915&redirId=188343) for more details.
 *
 * Alternatively, you can manually create the application using the following steps:
 *
 * 1. Download and unzip this repository onto your local machine, or clone the repository.
 * 2. Open a CLI terminal and navigate to the application folder.
 * 3. Import the required libraries by executing the `make getlibs` command.
 *
 * Various CLI tools include a `-h` option that prints help information to the terminal screen about that tool.
 * For more details, see the [ModusToolbox User Guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_3.0_Tools_Package_User_Guide-GettingStarted-v01_00-EN.pdf?fileId=8ac78c8c8386267f0183a8e9720c5915&redirId=188343).
 *
 * @section Licensing PSOC C1 Peripheral Driver Library Licensing
 *
 * <b> License Terms and Copyright Information </b>
 *
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
 *
 * To improve the quality of the software, users are encouraged to share
 * modifications, enhancements or bug fixes with Infineon Technologies AG
 * at Support@infineon.com.
 *
 * <b>Legal Disclaimer</b> The information given in this document shall in no event be regarded as a guarantee of conditions or characteristics. With respect to any examples or hints given herein, any typical values stated herein and/or any information regarding the application of the device, Infineon Technologies hereby disclaims any and all warranties and liabilities of any kind, including without limitation, warranties of non-infringement of intellectual property rights of any third party.
 *
 * <b>Information</b> For further information on technology, delivery terms and conditions and prices, please contact the nearest Infineon Technologies Office (www.infineon.com).
 *
 * <b>Warnings</b> Due to technical requirements, components may contain dangerous substances. For information on the types in question, please contact the nearest Infineon Technologies Office. Infineon Technologies components may be used in life-support devices or systems only with the express written approval of Infineon Technologies, if a failure of such components can reasonably be expected to cause the failure of that life-support device or system or to affect the safety or effectiveness of that device or system. Life support devices or systems are intended to be implanted in the human body or to support and/or maintain and sustain and/or protect human life. If they fail, it is reasonable to assume that the health of the user or other persons may be endangered.
 */
