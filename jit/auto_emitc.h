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

#undef  SIMIT_IMP
#undef  _FUNC_DEFAULT
#undef  _STUB_DEC
#undef  _TABLE_DEF_BEGIN
#undef  _TABLE_DEF_END
#undef  _STUB_ENTRY
#undef  _FUNC_CALL
#undef  _FUNC_NAME
#undef  _STUB_NAME
#undef  _TABLE_JUMP
#undef  _PATTERN_TRUE
#undef  _MAIN_DECODE_ENTRY

#define EMITC_FORMALS \
	char *buf, unsigned curpc, target_inst_t inst, unsigned size

#define EMITC_ARGS buf, curpc, inst, size

extern char* emitc_default(EMITC_FORMALS);

#define emitc_null emitc_default
#define _STUB_DEC(a) static char* cstub_##a(EMITC_FORMALS);

#define _TABLE_DEF_BEGIN(a,b) static char* (*ctable_##a [b])(EMITC_FORMALS) = {
#define _TABLE_DEF_END };

#define _STUB_ENTRY(a) static char* cstub_##a(EMITC_FORMALS)

#define _FUNC_CALL(a) return emitc_##a(EMITC_ARGS)
#define _FUNC_NAME(a) emitc_##a
#define _STUB_NAME(a) cstub_##a
#define _FUNC_DEFAULT emitc_default

//#define EINST emm->mem->read_word_fast(curpc)
//#define EINST emm->fetch_inst(curpc)
#define _TABLE_JUMP(a,b,c) return ctable_##a[(inst>>c)&b](EMITC_ARGS)
#define _PATTERN_TRUE(a,b) ((inst&a)==b)

#define _MAIN_DECODE_ENTRY static char* emit_code(EMITC_FORMALS)

