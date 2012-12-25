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

#ifndef SIMIT_DECODE_H
#define SIMIT_DECODE_H

#include "misc.h"
#include "syscall.h"

#include <cstdio>

char *arm_disassemble(arm_inst_t inst, arm_addr_t pc, char *buf);
void arm_disassemble_to_stream(arm_inst_t inst, arm_addr_t pc, FILE *stream);

#endif
