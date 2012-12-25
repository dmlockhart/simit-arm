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

#define disasm_null disasm_default
#define _STUB_DEC(a) char *disasm_stub_##a(arm_inst_t,arm_addr_t,char*);

#define _TABLE_DEF_BEGIN(a,b) \
  char *(*disasm_table_## a [b])(arm_inst_t,arm_addr_t,char *) = {
#define _TABLE_DEF_END };

#define _STUB_ENTRY(a) \
  char *disasm_stub_##a(arm_inst_t inst, arm_addr_t pc, char *buf)

#define _FUNC_CALL(a) return disasm_##a(inst, pc, buf)
#define _FUNC_NAME(a) disasm_##a
#define _STUB_NAME(a) disasm_stub_##a
#define _FUNC_DEFAULT disasm_default

#define _TABLE_JUMP(a,b,c) return disasm_table_##a[(inst>>c)&b](inst,pc,buf)
#define _PATTERN_TRUE(a,b) ((inst&a)==b)

#define _MAIN_DECODE_ENTRY \
  char *disasm_main(arm_inst_t inst, arm_addr_t pc, char *buf)

