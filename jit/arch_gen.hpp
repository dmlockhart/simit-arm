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

#include "arch.hpp"

using namespace simit;

typedef arm_emulator emulator_t;
typedef arm_inst_t target_inst_t;
typedef arm_addr_t target_addr_t;

#define EMIT_DELAY_SLOT
#define CURPC (curpc+8)
