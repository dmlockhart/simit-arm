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

#ifndef SIMIT_ARCH_H
#define SIMIT_ARCH_H


#ifdef SIMIT_SYSTEM_LEVEL

#define MMU_READ_WORD(addr,val)  fault = emu->mmu->read_word(addr,val)
#define MMU_WRITE_WORD(addr,val) fault = emu->mmu->write_word(addr,val)

#define MMU_READ_HALF_WORD(addr,val)  fault = emu->mmu->read_hword(addr,val)
#define MMU_WRITE_HALF_WORD(addr,val) fault = emu->mmu->write_hword(addr,val)

#define MMU_READ_BYTE(addr,val)  fault = emu->mmu->read_byte(addr,val)
#define MMU_WRITE_BYTE(addr,val) fault = emu->mmu->write_byte(addr,val)





#else

#define MMU_READ_WORD(addr,val)  \
		fault = (*val = emu->mem->read_word(addr), NO_FAULT)
#define MMU_WRITE_WORD(addr,val) \
		fault = (emu->mem->write_word(addr,val), NO_FAULT)

#define MMU_READ_HALF_WORD(addr,val)  \
		fault = (*val = emu->mem->read_hword(addr), NO_FAULT)
#define MMU_WRITE_HALF_WORD(addr,val) \
		fault = (emu->mem->write_hword(addr,val), NO_FAULT)

#define MMU_READ_BYTE(addr,val)  \
		fault = (*val = emu->mem->read_byte(addr), NO_FAULT)
#define MMU_WRITE_BYTE(addr,val) \
		fault = (emu->mem->write_byte(addr,val), NO_FAULT)

#endif


/* processor control */
#ifdef SIMIT_SYSTEM_LEVEL

#define CPU_ID            emu->get_cpu_id()
#define CPU_MODE          emu->get_cpu_mode()
#define IN_EXCEPTION_MODE emu->is_exception_mode()

#define TRANSLATION_MODE(new_mode)      emu->translation_mode(new_mode)
#define SWITCH_BANK(old_mode, new_mode) emu->switch_reg_bank(old_mode, new_mode)

#define IN_PRVLG          (CPU_MODE != simit::USR_MODE)
#define HAS_SPSR          IN_EXCEPTION_MODE

#define CP_ACCESS_ALLOWED(CP) \
    		((CP >= 14) || emu->mmu->cp_access_allowed(CP))

#define XSCALE_UPDATE_FSR_FAR(status,addr) 
		//	(emu->mmu->xscale_update_fsr_far(status,addr))

#define DO_SWI(a) emu->raise_exception(SWIV)

#define ABORT(v) do \
			{emu->raise_exception(v); return;} while (0)

#define ABORT2(v) do \
			{emu->raise_exception(v); return;} while (0)

//#define UPDATE_FSR_FAR(fault,addr) (emu->mmu->update_fsr_far(fault,addr))

#define SIMIT_IMP(a) impl_##a

#else

#define CPU_ID            emu->get_cpu_id()
#define CPU_MODE          simit::USR_MODE
#define IN_EXCEPTION_MODE false

#define TRANSLATION_MODE(new_mode) do_nothing(simit::USR_MODE)
#define SWITCH_BANK(old_mode, new_mode)

#define IN_PRVLG          false
#define HAS_SPSR          false

#define CP_ACCESS_ALLOWED(CP) false

#define XSCALE_UPDATE_FSR_FAR(status,addr) 

#define DO_SWI(a) do_syscall(emu,a);

// data abort should never occur
#define ABORT(v) 

// ignores undefined instruction and other faults
#define ABORT2(v) do {impl_default(emu, inst); return;} while (0)

#define SIMIT_IMP(a) uimpl_##a

#endif




#include <cassert>


#include "armemul.h"
#include "armmmu.h"
#include "armcopro.h"

using namespace simit;

#ifndef SIMIT_EMULATOR_T
typedef arm_emulator emulator_t;
#endif

typedef arm_inst_t target_inst_t;
typedef arm_addr_t target_addr_t;

extern void do_syscall(arm_emulator *emu, arm_addr_t pc);
extern void impl_default(arm_emulator *emu, arm_addr_t pc);

#if 0
static inline uint32_t rotate_right(uint32_t val, uint32_t imm)
{
    return (val >> imm ) | (val << (32 - imm));
}
#endif


/* created to suppress a gcc warning about no-effect statement */
static inline cpu_mode_t do_nothing(cpu_mode_t m) {return m;}

static inline uint32_t popcount16(uint32_t x)
{
	x = (x & 0x5555) + ((x >> 1) & 0x5555);
	x = (x & 0x3333) + ((x >> 2) & 0x3333);
	x = (x & 0x0F0F) + ((x >> 4) & 0x0F0F);
	x = (x & 0x00FF) + ((x >> 8) & 0x00FF);
	return x;
}

/* right most bit-one of a 16 bit number, returns 15 if x=0 */
static inline uint32_t rmo16(uint32_t x)
{
	unsigned res = 1;
	if ((x & 0xff) == 0) {res += 8; x >>= 8;}
	if ((x & 0x0f) == 0) {res += 4; x >>= 4;}
	if ((x & 0x03) == 0) {res += 2; x >>= 2;}
	return res - (x & 1);
}


#define UPDATE_CPSR(aa,bb)  emu->update_CPSR(aa,bb)
#define UPDATE_SPSR(aa, bb) emu->update_SPSR(aa,bb)


#define EMIT_DELAY_SLOT
#define GET_PC() PC
#define SET_PC(aa) WRITE_REG(15,aa)

#ifdef SIMIT_SYSTEM_LEVEL
#define SIMIT_IMP(a) impl_##a
#else
#define SIMIT_IMP(a) uimpl_##a
#endif

#endif
