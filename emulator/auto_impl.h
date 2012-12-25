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


extern void impl_default(arm_emulator *emu, arm_inst_t inst);


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

#ifdef SIMIT_SYSTEM_LEVEL

#define _FUNC_DEFAULT impl_default

#define SIMIT_IMP(a) impl_##a

#define impl_null impl_default

#define _STUB_DEC(a) \
		static void stub_##a(arm_emulator *emu, arm_inst_t inst);

#define _TABLE_DEF_BEGIN(a,b) \
		static void (*table_## a [b])(arm_emulator *emu, arm_inst_t inst) = {
#define _TABLE_DEF_END };

#define _STUB_ENTRY(a) \
		static void stub_##a(arm_emulator *emu, arm_inst_t inst)

#define _FUNC_CALL(a) impl_##a(emu, inst)
#define _FUNC_NAME(a) impl_##a
#define _STUB_NAME(a) stub_##a

#define _TABLE_JUMP(a,b,c) table_##a[(inst>>c)&b](emu, inst)
#define _PATTERN_TRUE(a,b) ((inst&a)==b)

#define _MAIN_DECODE_ENTRY \
		static void decode_main_system(arm_emulator *emu, arm_inst_t inst)

#else

#define SIMIT_IMP(a) uimpl_##a

#define uimpl_null impl_default

#define _STUB_DEC(a) \
		static void ustub_##a(arm_emulator *emu, arm_inst_t inst);

#define _TABLE_DEF_BEGIN(a,b) \
		static void (*utable_## a [b])(arm_emulator *emu, arm_inst_t inst) = {
#define _TABLE_DEF_END };

#define _STUB_ENTRY(a) \
		static void ustub_##a(arm_emulator *emu, arm_inst_t inst)

#define _FUNC_CALL(a) uimpl_##a(emu, inst)
#define _FUNC_NAME(a) uimpl_##a
#define _STUB_NAME(a) ustub_##a

#define _TABLE_JUMP(a,b,c) utable_##a[(inst>>c)&b](emu, inst)
#define _PATTERN_TRUE(a,b) ((inst&a)==b)

#define _MAIN_DECODE_ENTRY \
		static void decode_main_user(arm_emulator *emu, arm_inst_t inst)

#endif //SIMIT_SYSTEM_LEVEL
