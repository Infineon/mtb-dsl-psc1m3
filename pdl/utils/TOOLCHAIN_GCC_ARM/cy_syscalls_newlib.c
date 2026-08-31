/**
 * @file syscalls.c
 * @brief    Newlib stubs
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

/*
 * This file contains stubs for standard C library functionality that must
 * typically be provided for the underlying platform.
 *
 */
#if defined ( __GNUC__ ) && !defined (__ARMCC_VERSION)

#include <stdint.h>
#include <errno.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward prototypes.  */
caddr_t _sbrk(int nbytes) __attribute__((externally_visible));
void _init(void) __attribute__((externally_visible));

#ifdef __cplusplus
}
#endif

/* c++ destructor dynamic shared object needed if -fuse-cxa-atexit is used*/
void *__dso_handle __attribute__ ((weak));

// defined in linker script
extern caddr_t Heap_Bank1_Start;
extern caddr_t Heap_Bank1_End;
caddr_t _sbrk(int nbytes)
{
  static caddr_t heap_ptr = NULL;
  caddr_t base;

  if (heap_ptr == NULL) {
    heap_ptr = (caddr_t)&Heap_Bank1_Start;
  }

  base = heap_ptr;

  /* heap word alignment */
  nbytes = (nbytes + 3) & ~0x3U;
  if ((caddr_t)&Heap_Bank1_End > (heap_ptr + nbytes))
  {
    heap_ptr += nbytes;
    return (base);
  }
  else
  {
    /* Heap overflow */
    errno = ENOMEM;
    return ((caddr_t)-1);
  }
}

/* Init */
void _init(void)
{}

#endif /* __GNUC__ */
