################################################################################
# \file defines.mk
#
# \brief
# Defines, needed for the build recipe.
#
################################################################################
# \copyright
# Copyright (c) 2026, Infineon Technologies AG, or an affiliate of
# Infineon Technologies AG. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

ifeq ($(WHICHFILE),true)
$(info Processing $(lastword $(MAKEFILE_LIST)))
endif

include $(MTB_TOOLS__RECIPE_DIR)/make/recipe/defines_common.mk


################################################################################
# General
################################################################################
_MTB_RECIPE__DEFAULT_PROGRAM_INTERFACE:=JLink
_MTB_RECIPE__PROGRAM_INTERFACE_SUPPORTED:=JLink
#
# Compatibility interface for this recipe make
#
MTB_RECIPE__INTERFACE_VERSION:=2
MTB_RECIPE__EXPORT_INTERFACES:=4 5

MTB_RECIPE__NINJA_SUPPORT:=2

#
# List the supported toolchains
#
ifdef CY_SUPPORTED_TOOLCHAINS
MTB_SUPPORTED_TOOLCHAINS?=$(CY_SUPPORTED_TOOLCHAINS)
else
MTB_SUPPORTED_TOOLCHAINS?=GCC_ARM ARM IAR
endif

# For BWC with Makefiles that do anything with CY_SUPPORTED_TOOLCHAINS
CY_SUPPORTED_TOOLCHAINS:=$(MTB_SUPPORTED_TOOLCHAINS)

#
# Architecture specifics
#
_MTB_RECIPE__START_FLASH:=0x10001000

# Add the series name to the standard components list
# to enable auto-discovery of CMSIS startup templates
MTB_RECIPE__COMPONENT+=$(_MTB_RECIPE__DEVICE_DIE)

ifeq ($(TOOLCHAIN),GCC_ARM)
MTB_TOOLCHAIN_GCC_ARM__LDFLAGS+=-nostartfiles
endif
