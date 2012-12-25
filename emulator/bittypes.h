/*************************************************************************
    Copyright (C) 2002 - 2007 Wei Qin
    See file COPYING for more information.

    This program is free software; you can redistribute it and/or modify    
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*************************************************************************/

#ifndef __BITTYPE_H_
#define __BITTYPE_H_

#include <config.h>

#if HAVE_INTTYPES_H
#include <inttypes.h>
#elif HAVE_STDINT_H
#include <stdint.h>
#else

#if SIZEOF_CHAR == 1
typedef signed char int8_t;
typedef unsigned char uint8_t;
#else
#error "8-bit types cannot be determined!\n"
#endif

#if SIZEOF_SHORT == 2
typedef signed short int16_t;
typedef unsigned short uint16_t;
#else
#error "16-bit types cannot be determined!\n"
#endif

#if SIZEOF_INT == 4
typedef signed int int32_t;
typedef unsigned int uint32_t;
#else
#error "32-bit types cannot be determined!\n"
#endif

#if SIZEOF_LONG == 8
typedef signed long int64_t;
typedef unsigned long uint64_t;
#elif SIZEOF_LONG_LONG == 8
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
#else
#error "64-bit types cannot be determined!\n"
#endif

#endif
#endif
