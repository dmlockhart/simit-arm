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

#ifndef SIMIT_ARCH_JIT_H
#define SIMIT_ARCH_JIT_H


#include <arch.hpp>


#ifdef SIMIT_COMPILED_ISS



#undef READ_REG
#define READ_REG(ind) ((ind==15)?(CISS_PC+8+_virt_blk_ind):emu->read_gpr(ind))

#undef WRITE_REG
#define WRITE_REG(ind, val) \
	do { \
		if (ind==15) \
			LONG_JUMP2(CISS_PC, val); \
		emu->write_gpr2(ind, val); \
	} while (0)


#undef N_FLAG
#undef Z_FLAG
#undef C_FLAG 
#undef V_FLAG 

#define N_FLAG (emu->read_NZF()>>31)
#define Z_FLAG (emu->read_NZF()==0)
#define C_FLAG (emu->read_CF())
#define V_FLAG (emu->read_VF()>>31)


#undef ASGN_NZ
#undef ASGN_NZC
#undef ASGN_NZCV

#define ASGN_NZ(res_) emu->write_NZF(res_)
#define ASGN_NZC(res_,c_) {\
	emu->write_NZF(res_);\
	emu->write_CF(c_);\
}
#define ASGN_NZCV(res_,c_,v_) {\
	emu->write_NZF(res_);\
	emu->write_CF(c_);\
	emu->write_VF(v_);\
}

#undef READ_CPSR
#define READ_CPSR (emu->cfs_to_cc(), emu->read_cpsr())

#undef WRITE_CPSR
#define WRITE_CPSR(val) do {emu->write_cpsr(val); emu->cc_to_cfs();} while(0)

#undef PC
#define PC (CISS_PC + 8 + _virt_blk_ind)


/*************************************************************************/

#ifdef SIMIT_SYSTEM_LEVEL

#define LONG_JUMP(_cpc, _tpc) \
	do { \
		emu->decrement_pcount((_cpc>>2)+1-_lpc); \
		emu->write_gpr2(PC_REAL_IND, _tpc+_virt_blk_ind); \
		return; \
	} while (0)

#define LONG_JUMP2(_cpc, _tpc) \
	do { \
		emu->decrement_pcount((_cpc>>2)+1-_lpc); \
		emu->write_gpr2(PC_REAL_IND, _tpc); \
		return; \
	} while (0)

#define JUMP(_cpc, _tpc, _targ) \
	do { \
		emu->decrement_pcount((_cpc>>2)+1-_lpc); \
		if (emu->check_return()) { \
			emu->write_gpr2(PC_REAL_IND, _tpc+_virt_blk_ind); \
			return; \
		} \
		_lpc = _tpc >> 2; \
		goto _targ; \
	} while (0)

#undef ABORT
#define ABORT(v) \
	do { \
		emu->decrement_pcount((CISS_PC>>2)+1-_lpc); \
		if (fault != JIT_ALARM_FAULT) { \
			emu->write_gpr2(PC_REAL_IND, CISS_PC+4+_virt_blk_ind); \
			emu->cfs_to_cc(); emu->raise_exception(v); emu->cc_to_cfs(); \
		} \
		else {\
			/* cache the special DLL invaildation fault */\
			emu->write_gpr2(PC_REAL_IND, CISS_PC+_virt_blk_ind); \
		}\
		return; \
	} while (0)

#undef ABORT2
#define ABORT2(v) \
	do { \
		emu->decrement_pcount((CISS_PC>>2)+1-_lpc); \
		emu->write_gpr2(PC_REAL_IND, CISS_PC+4+_virt_blk_ind); \
		emu->cfs_to_cc(); emu->raise_exception(v); emu->cc_to_cfs(); \
		return; \
	} while (0)

#undef DO_SWI
#define DO_SWI(v) ABORT2(SWIV)


#else // !SIMIT_SYSTEM_LEVEL


#define LONG_JUMP(_cpc, _tpc) \
	do { \
		emu->increment_compile_count((_cpc>>2)+1-_lpc); \
		emu->write_gpr2(PC_REAL_IND, _tpc+_virt_blk_ind); \
		return; \
	} while (0)

#define LONG_JUMP2(_cpc, _tpc) \
	do { \
		emu->increment_compile_count((_cpc>>2)+1-_lpc); \
		emu->write_gpr2(PC_REAL_IND, _tpc); \
		return; \
	} while (0)

#define JUMP(_cpc, _tpc, _targ) \
	do { \
		emu->increment_compile_count((_cpc>>2)+1-_lpc); \
		_lpc = _tpc >>2; \
		goto _targ; \
	} while (0)


#undef ABORT
#define ABORT(v)

#undef ABORT2
#define ABORT2(v) \
	do { \
		impl_default(emu, CISS_INST); \
		/* skip the instruction */ \
		emu->increment_compile_count((CISS_PC>>2)+1-_lpc); \
		emu->write_gpr2(PC_REAL_IND, CISS_PC+_virt_blk_ind+4); \
		return; \
	} while (0)

#undef DO_SWI
#define DO_SWI(a)  \
	do { \
		do_syscall(emu, a); \
		emu->increment_compile_count((CISS_PC>>2)+1-_lpc); \
		return; \
	} while (0);

#endif // SIMIT_SYSTEM_LEVEL

 

#else /* !SIMIT_COMPILED_ISS, interpretation starts here */


#ifdef  SIMIT_SYSTEM_LEVEL

#undef ABORT
#define ABORT(v) do { \
		if (fault != JIT_ALARM_FAULT) \
			emu->raise_exception(v); \
		else \
			emu->write_gpr2(PC_REAL_IND, PC - 8); /* restart */ \
		return; \
		} while (0);


#endif // SIMIT_SYSTEM_LEVEL



#endif // SIMIT_COMPILED_ISS


#endif // SIMIT_ARCH_JIT_H
