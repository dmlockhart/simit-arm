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

#ifndef SIMIT_MISC_H
#define SIMIT_MISC_H

#include <config.h>
#include "bittypes.h"
#include <cstdio>

/***************************************************************************\
                       Target and host endianness
\***************************************************************************/

#define TARGET_LITTLE_ENDIAN 1

/* host endianness */
#ifndef WORDS_BIGENDIAN
#define WORDS_BIGENDIAN 0
#endif

#if WORDS_BIGENDIAN==TARGET_LITTLE_ENDIAN 

#define DIRECT_READ_WORD(_p,_v)\
	do{\
		word_t temp = *reinterpret_cast<const word_t*>(_p);\
		_v = (temp >> 24) | ((temp >> 8) & 0xFF00) | ((temp & 0xFF00) << 8) | \
		(temp << 24);\
	}while(0)

#define DIRECT_READ_HALF_WORD(_p,_v)\
	do{\
		hword_t temp = *reinterpret_cast<const hword_t*>(_p);\
		_v = (temp << 8) | (temp >> 8);\
	}while(0)

#define DIRECT_READ_BYTE(_p,_v)\
		_v = *(_p)

#define DIRECT_WRITE_WORD(_p,_v)\
		*reinterpret_cast<word_t*>(_p) = (_v >> 24) | \
			((_v >> 8) & 0xFF00) | ((_v & 0xFF00) << 8) | (_v << 24)
 
#define DIRECT_WRITE_HALF_WORD(_p,_v)\
		*reinterpret_cast<hword_t*>(_p) = (_v << 8) | (_v >> 8)

#define DIRECT_WRITE_BYTE(_p,_v)\
		*(_p) = _v

#else

#define DIRECT_READ_WORD(_p,_v)\
		_v = *reinterpret_cast<const word_t*>(_p)

#define DIRECT_READ_HALF_WORD(_p,_v)\
		_v = *reinterpret_cast<const hword_t*>(_p)

#define DIRECT_READ_BYTE(_p,_v)\
		_v = *(_p)

#define DIRECT_WRITE_WORD(_p,_v)\
		*reinterpret_cast<word_t*>(_p) = _v

#define DIRECT_WRITE_HALF_WORD(_p,_v)\
		*reinterpret_cast<hword_t*>(_p) = _v

#define DIRECT_WRITE_BYTE(_p,_v)\
		*(_p) = _v

#endif

/***************************************************************************\
                         Some basic ARM data types
\***************************************************************************/

typedef uint64_t dword_t;
typedef int64_t  sdword_t;
typedef uint32_t word_t;
typedef int32_t  sword_t;
typedef uint16_t hword_t;
typedef int16_t  shword_t;
typedef uint8_t  byte_t;
typedef int8_t   sbyte_t;
typedef uint32_t arm_inst_t;
typedef uint32_t arm_addr_t;

typedef arm_addr_t target_addr_t;
typedef arm_inst_t target_inst_t;


/***************************************************************************\
                          Bit manipulation macros 
\***************************************************************************/

#define BIT0(a)     ((a)&1)
#define BIT31(a)    ((uint32_t)(a)>>31)
#define BITn(a,b)   (((uint32_t)(a)>>(b))&1)
#define BITS(a,b,c) ((word_t)(a<<(31-(c))) >> ((31-(c))+(b))) /*bits b to c*/



/***************************************************************************\
                            Some opcode fields
\***************************************************************************/

#define COND (inst>>28)
#define OPCD ((inst>>20)&255)
#define DOPC ((inst>>21)&15)	/*DPI's opcode*/

#define RDFLD ((inst>>12)&15)
#define RNFLD ((inst>>16)&15)
#define RMFLD (inst&15)
#define RSFLD ((inst>>8)&15)

#define SFLD ((inst>>20)&1)


/***************************************************************************\
                        macros for accessing registers
\***************************************************************************/

#define READ_REG(ind) (emu->read_gpr(ind))
#define WRITE_REG(ind, val) (emu->write_gpr(ind,val))
#define WRITE_REG2(ind, val) (emu->write_gpr2(ind,val))

#define READ_CPSR (emu->read_cpsr())
#define READ_SPSR (emu->read_spsr())
#define READ_CC (emu->read_cc())

#define WRITE_CPSR(val) (emu->write_cpsr(val))
#define WRITE_SPSR(val) (emu->write_spsr(val))
#define WRITE_CC(val) (emu->write_cc(val))

#define CPSR READ_CPSR
#define SPSR READ_SPSR
#define CC READ_CC

#define N_FLAG ((CC>>3)&1)
#define Z_FLAG ((CC>>2)&1)
#define C_FLAG ((CC>>1)&1)
#define V_FLAG ((CC>>0)&1)

#define SET_N WRITE_CC(CC|(1<<3))
#define SET_Z WRITE_CC(CC|(1<<2))
#define SET_C WRITE_CC(CC|(1<<1))
#define SET_V WRITE_CC(CC|(1<<0))

#define SET_Q WRITE_CPSR(CPSR |(1<<27))

#define CLEAR_N WRITE_CC(CC&(~(1<<3)))
#define CLEAR_Z WRITE_CC(CC&(~(1<<2)))
#define CLEAR_C WRITE_CC(CC&(~(1<<1)))
#define CLEAR_V WRITE_CC(CC&(~(1<<0)))

#define ASGN_N(n) {if (n) SET_N; else CLEAR_N;}
#define ASGN_Z(n) {if (n) SET_Z; else CLEAR_Z;}
#define ASGN_C(n) {if (n) SET_C; else CLEAR_C;}
#define ASGN_V(n) {if (n) SET_V; else CLEAR_V;}

#define ASGN_Q(n) {if (n) SET_Q;}

#define ASGN_NZ(res_) \
	WRITE_CC((((res_)>>28)&0x8) | (((res_)==0)<<2) | (CC & 3))

#define ASGN_NZC(res_,c_) \
	WRITE_CC((((res_)>>28)&0x8) | (((res_)==0)<<2) | ((c_)<<1) | (CC & 1))

#define ASGN_NZCV(res_,c_,v_) \
	WRITE_CC((((res_)>>28)&0x8) | (((res_)==0)<<2) | ((c_)<<1) | ((v_)>>31))


/* some ABI registers */
#define FPIND 11
#define IPIND 12
#define SPIND 13
#define LRIND 14
#define PC_AHEAD_IND 15
#define PC_REAL_IND 16
#define NUM_GPR 17

#define RD READ_REG(RDFLD)
#define RN READ_REG(RNFLD)
#define RM READ_REG(RMFLD)
#define RS READ_REG(RSFLD)
#define FP READ_REG(FPIND)
#define IP READ_REG(IPIND)
#define SP READ_REG(SPIND)
#define LR READ_REG(LRIND)
#define PC READ_REG(PC_AHEAD_IND)


#define MEM_READ(buf, addr, size) (emu->mem->read_block(buf, addr, size))
#define MEM_WRITE(buf, addr, size) (emu->mem->write_block(buf, addr, size))


#define MEM_READ_WORD(addr, ptr) (emu->mem->read_word(addr, ptr))
#define MEM_WRITE_WORD(addr,val) (emu->mem->write_word(addr,val))

#define MEM_READ_HALF_WORD(addr, ptr) (emu->mem->read_hword(addr, ptr))
#define MEM_WRITE_HALF_WORD(addr,val) (emu->mem->write_hword(addr,val))

#define MEM_READ_BYTE(addr, ptr) (emu->mem->read_byte(addr, ptr))
#define MEM_WRITE_BYTE(addr,val) (emu->mem->write_byte(addr,val))

uint64_t ato_uint64(char *input);
void dump_int64(uint64_t, FILE *);
void dump_int64_smart(uint64_t, FILE *);
void term_init(void);
void term_exit(void);


#endif
