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
void SIMIT_IMP(movi_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, imm8);


}

void SIMIT_IMP(movi_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))));


}

void SIMIT_IMP(movis_imm_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (carry=C_FLAG,imm8);
	if (rd==15)
		WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movis_rot_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (rslt32=(imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))), carry=BIT31(rslt32), rslt32);
	if (rd==15)
		WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mov_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, (READ_REG(rm)<<shift_imm));


}

void SIMIT_IMP(mov_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, 0);


}

void SIMIT_IMP(mov_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, (READ_REG(rm)>>shift_imm));


}

void SIMIT_IMP(mov_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, (BIT31(READ_REG(rm))?~0:0));


}

void SIMIT_IMP(mov_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ((int32_t)(READ_REG(rm))>>shift_imm));


}

void SIMIT_IMP(mov_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ((READ_REG(rm)>>1)|(C_FLAG<<31)));


}

void SIMIT_IMP(mov_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;


	WRITE_REG(rd, (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))));


}

void SIMIT_IMP(mov_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0));


}

void SIMIT_IMP(mov_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0));


}

void SIMIT_IMP(mov_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)));


}

void SIMIT_IMP(mov_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));


}

void SIMIT_IMP(movs_lsz_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (carry=C_FLAG, READ_REG(rm));
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_lsl_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (val32=READ_REG(rm), carry=BITn(val32, 32-shift_imm), val32<<shift_imm);
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_zero_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (carry=BIT31(READ_REG(rm)), 0);
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_lsr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), val32>>shift_imm);
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_sign_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (val32=READ_REG(rm), carry=BIT31(val32), BIT31(val32)?~0:0);
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_asr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (int32_t)val32>>shift_imm);
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_rrx_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (val32=READ_REG(rm), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_ror_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_lsl_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_lsr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_asr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(movs_ror_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	if (rd==15)
    	WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvni_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ~imm8);


}

void SIMIT_IMP(mvni_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ~((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))));


}

void SIMIT_IMP(mvnis_imm_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(carry=C_FLAG,imm8);
	if (rd==15)
		WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvnis_rot_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(rslt32=(imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))), carry=BIT31(rslt32), rslt32);
	if (rd==15)
		WRITE_CPSR(SPSR);
	else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvn_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ~(READ_REG(rm)<<shift_imm));


}

void SIMIT_IMP(mvn_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ~0);


}

void SIMIT_IMP(mvn_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ~(READ_REG(rm)>>shift_imm));


}

void SIMIT_IMP(mvn_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ~(BIT31(READ_REG(rm))?~0:0));


}

void SIMIT_IMP(mvn_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ~((int32_t)(READ_REG(rm))>>shift_imm));


}

void SIMIT_IMP(mvn_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;



	WRITE_REG(rd, ~((READ_REG(rm)>>1)|(C_FLAG<<31)));


}

void SIMIT_IMP(mvn_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;


	WRITE_REG(rd, ~(val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))));


}

void SIMIT_IMP(mvn_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0));


}

void SIMIT_IMP(mvn_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0));


}

void SIMIT_IMP(mvn_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)));


}

void SIMIT_IMP(mvn_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));


}

void SIMIT_IMP(mvns_lsz_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(carry=C_FLAG, READ_REG(rm));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_lsl_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(val32=READ_REG(rm), carry=BITn(val32, 32-shift_imm), val32<<shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_zero_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(carry=BIT31(READ_REG(rm)), 0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_lsr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), val32>>shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_sign_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(val32=READ_REG(rm), carry=BIT31(val32), BIT31(val32)?~0:0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_asr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (int32_t)val32>>shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_rrx_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(val32=READ_REG(rm), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_ror_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (val32>>shift_imm)|(val32<<(32-shift_imm)));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_lsl_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_lsr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_asr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mvns_ror_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(addi_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + imm8);


}

void SIMIT_IMP(addi_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))));


}

void SIMIT_IMP(addis_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = imm8;
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(addis_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))));
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(add_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + (READ_REG(rm)<<shift_imm));


}

void SIMIT_IMP(add_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + 0);


}

void SIMIT_IMP(add_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + (READ_REG(rm)>>shift_imm));


}

void SIMIT_IMP(add_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + (BIT31(READ_REG(rm))?~0:0));


}

void SIMIT_IMP(add_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + ((int32_t)(READ_REG(rm))>>shift_imm));


}

void SIMIT_IMP(add_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + ((READ_REG(rm)>>1)|(C_FLAG<<31)));


}

void SIMIT_IMP(add_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	WRITE_REG(rd, READ_REG(rn) + (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))));


}

void SIMIT_IMP(add_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) + (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0));


}

void SIMIT_IMP(add_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) + (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0));


}

void SIMIT_IMP(add_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) + (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)));


}

void SIMIT_IMP(add_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) + (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));


}

void SIMIT_IMP(adds_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (READ_REG(rm)<<shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adds_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = 0;
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adds_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (READ_REG(rm)>>shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adds_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (BIT31(READ_REG(rm))?~0:0);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adds_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = ((int32_t)(READ_REG(rm))>>shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adds_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adds_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adds_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adds_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adds_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31));
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adds_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adci_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + imm8 + C_FLAG);


}

void SIMIT_IMP(adci_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))) + C_FLAG);


}

void SIMIT_IMP(adcis_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = imm8;
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcis_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))));
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adc_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + (READ_REG(rm)<<shift_imm) + C_FLAG);


}

void SIMIT_IMP(adc_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + 0 + C_FLAG);


}

void SIMIT_IMP(adc_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + (READ_REG(rm)>>shift_imm) + C_FLAG);


}

void SIMIT_IMP(adc_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + (BIT31(READ_REG(rm))?~0:0) + C_FLAG);


}

void SIMIT_IMP(adc_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + ((int32_t)(READ_REG(rm))>>shift_imm) + C_FLAG);


}

void SIMIT_IMP(adc_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) + ((READ_REG(rm)>>1)|(C_FLAG<<31)) + C_FLAG);


}

void SIMIT_IMP(adc_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	WRITE_REG(rd, READ_REG(rn) + (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))) + C_FLAG);


}

void SIMIT_IMP(adc_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) + (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0) + C_FLAG);


}

void SIMIT_IMP(adc_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) + (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0) + C_FLAG);


}

void SIMIT_IMP(adc_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) + (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)) + C_FLAG);


}

void SIMIT_IMP(adc_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) + (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))) + C_FLAG);


}

void SIMIT_IMP(adcs_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = (READ_REG(rm)<<shift_imm);
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcs_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = 0;
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcs_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = (READ_REG(rm)>>shift_imm);
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcs_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = (BIT31(READ_REG(rm))?~0:0);
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcs_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = ((int32_t)(READ_REG(rm))>>shift_imm);
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcs_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcs_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcs_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0);
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcs_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0);
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcs_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31));
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(adcs_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32  = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	val32  = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = tmp32+val32+1;
		carry = rslt32 <= val32;
	}
	else {
		rslt32 = tmp32+val32;
		carry = rslt32 < val32;
	}
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subi_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - imm8);


}

void SIMIT_IMP(subi_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))));


}

void SIMIT_IMP(subis_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32  = imm8;
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subis_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32  = ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))));
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sub_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - (READ_REG(rm)<<shift_imm));


}

void SIMIT_IMP(sub_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - 0);


}

void SIMIT_IMP(sub_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - (READ_REG(rm)>>shift_imm));


}

void SIMIT_IMP(sub_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - (BIT31(READ_REG(rm))?~0:0));


}

void SIMIT_IMP(sub_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - ((int32_t)(READ_REG(rm))>>shift_imm));


}

void SIMIT_IMP(sub_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - ((READ_REG(rm)>>1)|(C_FLAG<<31)));


}

void SIMIT_IMP(sub_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	WRITE_REG(rd, READ_REG(rn) - (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))));


}

void SIMIT_IMP(sub_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) - (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0));


}

void SIMIT_IMP(sub_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) - (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0));


}

void SIMIT_IMP(sub_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) - (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)));


}

void SIMIT_IMP(sub_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) - (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));


}

void SIMIT_IMP(subs_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (READ_REG(rm)<<shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subs_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = 0;
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subs_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (READ_REG(rm)>>shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subs_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (BIT31(READ_REG(rm))?~0:0);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subs_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = ((int32_t)(READ_REG(rm))>>shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subs_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subs_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subs_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subs_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subs_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31));
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(subs_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbci_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - imm8 - (1-C_FLAG));


}

void SIMIT_IMP(sbci_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))) - (1-C_FLAG));


}

void SIMIT_IMP(sbcis_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = imm8;
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcis_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))));
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbc_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - (READ_REG(rm)<<shift_imm) - (1-C_FLAG));


}

void SIMIT_IMP(sbc_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - 0 - (1-C_FLAG));


}

void SIMIT_IMP(sbc_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - (READ_REG(rm)>>shift_imm) - (1-C_FLAG));


}

void SIMIT_IMP(sbc_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - (BIT31(READ_REG(rm))?~0:0) - (1-C_FLAG));


}

void SIMIT_IMP(sbc_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - ((int32_t)(READ_REG(rm))>>shift_imm) - (1-C_FLAG));


}

void SIMIT_IMP(sbc_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) - ((READ_REG(rm)>>1)|(C_FLAG<<31)) - (1-C_FLAG));


}

void SIMIT_IMP(sbc_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	WRITE_REG(rd, READ_REG(rn) - (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))) - (1-C_FLAG));


}

void SIMIT_IMP(sbc_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) - (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0) - (1-C_FLAG));


}

void SIMIT_IMP(sbc_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) - (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0) - (1-C_FLAG));


}

void SIMIT_IMP(sbc_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) - (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)) - (1-C_FLAG));


}

void SIMIT_IMP(sbc_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) - (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))) - (1-C_FLAG));


}

void SIMIT_IMP(sbcs_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = (READ_REG(rm)<<shift_imm);
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcs_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = 0;
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcs_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = (READ_REG(rm)>>shift_imm);
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcs_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = (BIT31(READ_REG(rm))?~0:0);
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcs_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = ((int32_t)(READ_REG(rm))>>shift_imm);
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcs_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcs_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcs_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0);
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcs_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0);
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcs_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31));
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(sbcs_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	val32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbi_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, imm8 - READ_REG(rn));


}

void SIMIT_IMP(rsbi_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))) - READ_REG(rn));


}

void SIMIT_IMP(rsbis_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = imm8;
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbis_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))));
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsb_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, (READ_REG(rm)<<shift_imm) - READ_REG(rn));


}

void SIMIT_IMP(rsb_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, 0 - READ_REG(rn));


}

void SIMIT_IMP(rsb_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, (READ_REG(rm)>>shift_imm) - READ_REG(rn));


}

void SIMIT_IMP(rsb_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, (BIT31(READ_REG(rm))?~0:0) - READ_REG(rn));


}

void SIMIT_IMP(rsb_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, ((int32_t)(READ_REG(rm))>>shift_imm) - READ_REG(rn));


}

void SIMIT_IMP(rsb_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, ((READ_REG(rm)>>1)|(C_FLAG<<31)) - READ_REG(rn));


}

void SIMIT_IMP(rsb_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	WRITE_REG(rd, (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))) - READ_REG(rn));


}

void SIMIT_IMP(rsb_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0) - READ_REG(rn));


}

void SIMIT_IMP(rsb_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0) - READ_REG(rn));


}

void SIMIT_IMP(rsb_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)) - READ_REG(rn));


}

void SIMIT_IMP(rsb_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))) - READ_REG(rn));


}

void SIMIT_IMP(rsbs_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = (READ_REG(rm)<<shift_imm);
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbs_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = 0;
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbs_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = (READ_REG(rm)>>shift_imm);
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbs_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = (BIT31(READ_REG(rm))?~0:0);
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbs_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = ((int32_t)(READ_REG(rm))>>shift_imm);
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbs_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbs_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbs_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0);
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbs_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0);
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbs_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31));
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsbs_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	val32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	tmp32 = READ_REG(rn);
	rslt32 = val32 - tmp32;

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsci_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, imm8 - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsci_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rscis_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = imm8;
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscis_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))));
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rsc_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, (READ_REG(rm)<<shift_imm) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsc_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, 0 - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsc_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, (READ_REG(rm)>>shift_imm) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsc_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, (BIT31(READ_REG(rm))?~0:0) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsc_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, ((int32_t)(READ_REG(rm))>>shift_imm) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsc_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, ((READ_REG(rm)>>1)|(C_FLAG<<31)) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsc_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	WRITE_REG(rd, (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsc_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsc_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsc_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rsc_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))) - READ_REG(rn) - (1-C_FLAG));


}

void SIMIT_IMP(rscs_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = (READ_REG(rm)<<shift_imm);
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscs_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = 0;
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscs_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = (READ_REG(rm)>>shift_imm);
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscs_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = (BIT31(READ_REG(rm))?~0:0);
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscs_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = ((int32_t)(READ_REG(rm))>>shift_imm);
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscs_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscs_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscs_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0);
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscs_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0);
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscs_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31));
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(rscs_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	val32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	tmp32 = READ_REG(rn);
	if (C_FLAG) {
		rslt32 = val32 - tmp32;
		carry = val32 >= tmp32;
	}
	else {
		rslt32 = val32 - tmp32 - 1;
		carry = val32 > tmp32;
	}

    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(andi_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & imm8);


}

void SIMIT_IMP(andi_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))));


}

void SIMIT_IMP(andis_imm_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (carry=C_FLAG,imm8);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(andis_rot_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (rslt32=(imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))), carry=BIT31(rslt32), rslt32);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(and_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & (READ_REG(rm)<<shift_imm));


}

void SIMIT_IMP(and_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & 0);


}

void SIMIT_IMP(and_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & (READ_REG(rm)>>shift_imm));


}

void SIMIT_IMP(and_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & (BIT31(READ_REG(rm))?~0:0));


}

void SIMIT_IMP(and_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ((int32_t)(READ_REG(rm))>>shift_imm));


}

void SIMIT_IMP(and_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ((READ_REG(rm)>>1)|(C_FLAG<<31)));


}

void SIMIT_IMP(and_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	WRITE_REG(rd, READ_REG(rn) & (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))));


}

void SIMIT_IMP(and_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0));


}

void SIMIT_IMP(and_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0));


}

void SIMIT_IMP(and_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)));


}

void SIMIT_IMP(and_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));


}

void SIMIT_IMP(ands_lsz_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (carry=C_FLAG, READ_REG(rm));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_lsl_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BITn(val32, 32-shift_imm), val32<<shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_zero_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (carry=BIT31(READ_REG(rm)), 0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_lsr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), val32>>shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_sign_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BIT31(val32), BIT31(val32)?~0:0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_asr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (int32_t)val32>>shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_rrx_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_ror_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (val32>>shift_imm)|(val32<<(32-shift_imm)));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_lsl_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_lsr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_asr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ands_ror_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eori_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) ^ imm8);


}

void SIMIT_IMP(eori_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) ^ ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))));


}

void SIMIT_IMP(eoris_imm_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (carry=C_FLAG,imm8);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eoris_rot_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (rslt32=(imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))), carry=BIT31(rslt32), rslt32);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eor_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) ^ (READ_REG(rm)<<shift_imm));


}

void SIMIT_IMP(eor_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) ^ 0);


}

void SIMIT_IMP(eor_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) ^ (READ_REG(rm)>>shift_imm));


}

void SIMIT_IMP(eor_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) ^ (BIT31(READ_REG(rm))?~0:0));


}

void SIMIT_IMP(eor_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) ^ ((int32_t)(READ_REG(rm))>>shift_imm));


}

void SIMIT_IMP(eor_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) ^ ((READ_REG(rm)>>1)|(C_FLAG<<31)));


}

void SIMIT_IMP(eor_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	WRITE_REG(rd, READ_REG(rn) ^ (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))));


}

void SIMIT_IMP(eor_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0));


}

void SIMIT_IMP(eor_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0));


}

void SIMIT_IMP(eor_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)));


}

void SIMIT_IMP(eor_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));


}

void SIMIT_IMP(eors_lsz_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (carry=C_FLAG, READ_REG(rm));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_lsl_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BITn(val32, 32-shift_imm), val32<<shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_zero_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (carry=BIT31(READ_REG(rm)), 0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_lsr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), val32>>shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_sign_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BIT31(val32), BIT31(val32)?~0:0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_asr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (int32_t)val32>>shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_rrx_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_ror_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (val32>>shift_imm)|(val32<<(32-shift_imm)));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_lsl_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_lsr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_asr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(eors_ror_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orri_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) | imm8);


}

void SIMIT_IMP(orri_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) | ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))));


}

void SIMIT_IMP(orris_imm_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (carry=C_FLAG,imm8);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orris_rot_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (rslt32=(imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))), carry=BIT31(rslt32), rslt32);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orr_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) | (READ_REG(rm)<<shift_imm));


}

void SIMIT_IMP(orr_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) | 0);


}

void SIMIT_IMP(orr_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) | (READ_REG(rm)>>shift_imm));


}

void SIMIT_IMP(orr_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) | (BIT31(READ_REG(rm))?~0:0));


}

void SIMIT_IMP(orr_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) | ((int32_t)(READ_REG(rm))>>shift_imm));


}

void SIMIT_IMP(orr_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) | ((READ_REG(rm)>>1)|(C_FLAG<<31)));


}

void SIMIT_IMP(orr_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	WRITE_REG(rd, READ_REG(rn) | (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))));


}

void SIMIT_IMP(orr_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) | (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0));


}

void SIMIT_IMP(orr_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) | (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0));


}

void SIMIT_IMP(orr_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) | (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)));


}

void SIMIT_IMP(orr_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) | (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));


}

void SIMIT_IMP(orrs_lsz_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (carry=C_FLAG, READ_REG(rm));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_lsl_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (val32=READ_REG(rm), carry=BITn(val32, 32-shift_imm), val32<<shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_zero_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (carry=BIT31(READ_REG(rm)), 0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_lsr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), val32>>shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_sign_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (val32=READ_REG(rm), carry=BIT31(val32), BIT31(val32)?~0:0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_asr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (int32_t)val32>>shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_rrx_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (val32=READ_REG(rm), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_ror_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (val32>>shift_imm)|(val32<<(32-shift_imm)));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_lsl_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_lsr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_asr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(orrs_ror_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) | (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bici_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ~imm8);


}

void SIMIT_IMP(bici_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ~((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1)))));


}

void SIMIT_IMP(bicis_imm_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(carry=C_FLAG,imm8);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bicis_rot_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(rslt32=(imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))), carry=BIT31(rslt32), rslt32);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bic_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ~(READ_REG(rm)<<shift_imm));


}

void SIMIT_IMP(bic_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ~0);


}

void SIMIT_IMP(bic_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ~(READ_REG(rm)>>shift_imm));


}

void SIMIT_IMP(bic_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ~(BIT31(READ_REG(rm))?~0:0));


}

void SIMIT_IMP(bic_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ~((int32_t)(READ_REG(rm))>>shift_imm));


}

void SIMIT_IMP(bic_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	WRITE_REG(rd, READ_REG(rn) & ~((READ_REG(rm)>>1)|(C_FLAG<<31)));


}

void SIMIT_IMP(bic_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	WRITE_REG(rd, READ_REG(rn) & ~(val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm))));


}

void SIMIT_IMP(bic_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) & ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0));


}

void SIMIT_IMP(bic_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) & ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0));


}

void SIMIT_IMP(bic_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) & ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31)));


}

void SIMIT_IMP(bic_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;


	WRITE_REG(rd, READ_REG(rn) & ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));


}

void SIMIT_IMP(bics_lsz_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(carry=C_FLAG, READ_REG(rm));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_lsl_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(val32=READ_REG(rm), carry=BITn(val32, 32-shift_imm), val32<<shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_zero_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(carry=BIT31(READ_REG(rm)), 0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_lsr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), val32>>shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_sign_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(val32=READ_REG(rm), carry=BIT31(val32), BIT31(val32)?~0:0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_asr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (int32_t)val32>>shift_imm);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_rrx_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(val32=READ_REG(rm), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_ror_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (val32>>shift_imm)|(val32<<(32-shift_imm)));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_lsl_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_lsr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_asr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(bics_ror_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & ~(tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
    if (rd==15)
        WRITE_CPSR(SPSR);
    else
		ASGN_NZC(rslt32, carry);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(cmpi_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32  = imm8;
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmpi_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32  = ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))));
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (READ_REG(rm)<<shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = 0;
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (READ_REG(rm)>>shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (BIT31(READ_REG(rm))?~0:0);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = ((int32_t)(READ_REG(rm))>>shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0);
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31));
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmp_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	val32  = READ_REG(rn);
	rslt32 = val32 - tmp32;

	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmni_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = imm8;
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmni_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))));
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (READ_REG(rm)<<shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = 0;
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (READ_REG(rm)>>shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (BIT31(READ_REG(rm))?~0:0);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = ((int32_t)(READ_REG(rm))>>shift_imm);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_lsl_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32<<tmp32):0);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_lsr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (tmp32<32)?(val32>>tmp32):0);
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_asr_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (int32_t)val32>>(tmp32<32?tmp32:31));
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(cmn_ror_mode3_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	tmp32 = (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	val32  = READ_REG(rn);
	rslt32 = val32 + tmp32;
	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));


}

void SIMIT_IMP(tsti_imm_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (carry=C_FLAG,imm8);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tsti_rot_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (rslt32=(imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))), carry=BIT31(rslt32), rslt32);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_lsz_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (carry=C_FLAG, READ_REG(rm));
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_lsl_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BITn(val32, 32-shift_imm), val32<<shift_imm);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_zero_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (carry=BIT31(READ_REG(rm)), 0);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_lsr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), val32>>shift_imm);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_sign_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BIT31(val32), BIT31(val32)?~0:0);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_asr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (int32_t)val32>>shift_imm);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_rrx_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_ror_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_lsl_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_lsr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_asr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(tst_ror_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) & (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teqi_imm_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (carry=C_FLAG,imm8);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teqi_rot_mode1_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (rslt32=(imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))), carry=BIT31(rslt32), rslt32);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_lsz_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (carry=C_FLAG, READ_REG(rm));
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_lsl_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BITn(val32, 32-shift_imm), val32<<shift_imm);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_zero_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (carry=BIT31(READ_REG(rm)), 0);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_lsr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), val32>>shift_imm);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_sign_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BIT31(val32), BIT31(val32)?~0:0);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_asr_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (int32_t)val32>>shift_imm);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_rrx_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_ror_mode2_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (val32=READ_REG(rm), carry=BITn(val32, shift_imm-1), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_lsl_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_lsr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_asr_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(teq_ror_mode3_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;
	uint32_t carry;


	rslt32 = READ_REG(rn) ^ (tmp32=READ_REG(rs)&0xFF, val32=READ_REG(rm), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));
	ASGN_NZC(rslt32, carry);


}

void SIMIT_IMP(mla_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>12)&15;
	unsigned rd = (inst>>16)&15;



	WRITE_REG(rd, (uint32_t)((uint64_t)READ_REG(rm) * (uint64_t)READ_REG(rs) + READ_REG(rn)));


}

void SIMIT_IMP(mla_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>12)&15;
	unsigned rd = (inst>>16)&15;

	uint32_t rslt32;


	rslt32 = (uint32_t)((uint64_t)READ_REG(rm) * (uint64_t)READ_REG(rs) + READ_REG(rn));
	ASGN_NZ(rslt32);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(mul_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>16)&15;



	WRITE_REG(rd, (uint32_t)((uint64_t)READ_REG(rm) * (uint64_t)READ_REG(rs)));


}

void SIMIT_IMP(mul_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>16)&15;

	uint32_t rslt32;


	rslt32 = (uint32_t)((uint64_t)READ_REG(rm) * (uint64_t)READ_REG(rs));
	ASGN_NZ(rslt32);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(smull_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint64_t rslt64;


    rslt64 = (int64_t)READ_REG(rm) * (int64_t)READ_REG(rs);
    WRITE_REG(rn, (uint32_t)(rslt64>>32));
    WRITE_REG(rd, (uint32_t)rslt64);


}

void SIMIT_IMP(smull_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint64_t rslt64;


    rslt64 = (int64_t)READ_REG(rm) * (int64_t)READ_REG(rs);
	ASGN_NZ((uint32_t)rslt64);
    WRITE_REG(rn, (uint32_t)(rslt64>>32));
    WRITE_REG(rd, (uint32_t)rslt64);


}

void SIMIT_IMP(smlal_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint64_t rslt64;


	rslt64 = ((uint64_t)READ_REG(rn)<<32) + (uint64_t)READ_REG(rd);
    rslt64 = (int64_t)READ_REG(rm) * (int64_t)READ_REG(rs)+rslt64;
    WRITE_REG(rn, (uint32_t)(rslt64>>32));
    WRITE_REG(rd, (uint32_t)rslt64);


}

void SIMIT_IMP(smlal_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint64_t rslt64;


	rslt64 = ((uint64_t)READ_REG(rn)<<32) + (uint64_t)READ_REG(rd);
    rslt64 = (int64_t)READ_REG(rm) * (int64_t)READ_REG(rs)+rslt64;
    WRITE_REG(rn, (uint32_t)(rslt64>>32));
    WRITE_REG(rd, (uint32_t)rslt64);
	ASGN_NZ((uint32_t)rslt64);


}

void SIMIT_IMP(umull_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint64_t rslt64;


    rslt64 = (uint64_t)READ_REG(rm) * (uint64_t)READ_REG(rs);
    WRITE_REG(rn, (uint32_t)(rslt64>>32));
    WRITE_REG(rd, (uint32_t)rslt64);


}

void SIMIT_IMP(umull_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint64_t rslt64;


    rslt64 = (uint64_t)READ_REG(rm) * (uint64_t)READ_REG(rs);
    WRITE_REG(rn, (uint32_t)(rslt64>>32));
    WRITE_REG(rd, (uint32_t)rslt64);
	ASGN_NZ((uint32_t)rslt64);


}

void SIMIT_IMP(umlal_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint64_t rslt64;


	rslt64 = ((uint64_t)READ_REG(rn)<<32) + (uint64_t)READ_REG(rd);
    rslt64 = (uint64_t)READ_REG(rm) * (uint64_t)READ_REG(rs) + rslt64;
    WRITE_REG(rn, (uint32_t)(rslt64>>32));
    WRITE_REG(rd, (uint32_t)rslt64);


}

void SIMIT_IMP(umlal_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint64_t rslt64;


	rslt64 = ((uint64_t)READ_REG(rn)<<32) + (uint64_t)READ_REG(rd);
    rslt64 = (uint64_t)READ_REG(rm) * (uint64_t)READ_REG(rs) + rslt64;
    WRITE_REG(rn, (uint32_t)(rslt64>>32));
    WRITE_REG(rd, (uint32_t)rslt64);
	ASGN_NZ((uint32_t)rslt64);


}

void SIMIT_IMP(ldt_imm_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);
	
	MMU_READ_WORD(address,&rslt32);
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ldt_imm_b_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

			
	MMU_READ_BYTE(address,&rslt8);
	TRANSLATION_MODE(current_mode);

	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ldt_reg_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = (READ_REG(rm)<<shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_READ_WORD(address,&rslt32);
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ldt_reg_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = 0;
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_READ_WORD(address,&rslt32);
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ldt_reg_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = (READ_REG(rm)>>shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_READ_WORD(address,&rslt32);
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ldt_reg_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = (BIT31(READ_REG(rm))?~0:0);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_READ_WORD(address,&rslt32);
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ldt_reg_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_READ_WORD(address,&rslt32);
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ldt_reg_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_READ_WORD(address,&rslt32);
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ldt_reg_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_READ_WORD(address,&rslt32);
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ldt_reg_b_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = (READ_REG(rm)<<shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);
	
	MMU_READ_BYTE(address,&rslt8);
	TRANSLATION_MODE(current_mode);

	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ldt_reg_b_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = 0;
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);
	
	MMU_READ_BYTE(address,&rslt8);
	TRANSLATION_MODE(current_mode);

	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ldt_reg_b_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = (READ_REG(rm)>>shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);
	
	MMU_READ_BYTE(address,&rslt8);
	TRANSLATION_MODE(current_mode);

	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ldt_reg_b_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = (BIT31(READ_REG(rm))?~0:0);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);
	
	MMU_READ_BYTE(address,&rslt8);
	TRANSLATION_MODE(current_mode);

	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ldt_reg_b_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);
	
	MMU_READ_BYTE(address,&rslt8);
	TRANSLATION_MODE(current_mode);

	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ldt_reg_b_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);
	
	MMU_READ_BYTE(address,&rslt8);
	TRANSLATION_MODE(current_mode);

	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ldt_reg_b_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);
	
	MMU_READ_BYTE(address,&rslt8);
	TRANSLATION_MODE(current_mode);

	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_imm_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	mmu_fault_t fault;


	address = READ_REG(rn);
	MMU_READ_WORD(address,&rslt32);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_imm_p_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	mmu_fault_t fault;


	address = ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12;
	MMU_READ_WORD(address,&rslt32);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_imm_b_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	mmu_fault_t fault;


	address = READ_REG(rn);
	MMU_READ_BYTE(address,&rslt8);

	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_imm_pb_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	mmu_fault_t fault;


	address = ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12;
	MMU_READ_BYTE(address,&rslt8);

	if(fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (READ_REG(rm)<<shift_imm);
	address = READ_REG(rn);
	MMU_READ_WORD(address,&rslt32);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = 0;
	address = READ_REG(rn);
	MMU_READ_WORD(address,&rslt32);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (READ_REG(rm)>>shift_imm);
	address = READ_REG(rn);
	MMU_READ_WORD(address,&rslt32);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (BIT31(READ_REG(rm))?~0:0);
	address = READ_REG(rn);
	MMU_READ_WORD(address,&rslt32);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = READ_REG(rn);
	MMU_READ_WORD(address,&rslt32);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = READ_REG(rn);
	MMU_READ_WORD(address,&rslt32);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = READ_REG(rn);
	MMU_READ_WORD(address,&rslt32);

	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_p_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (READ_REG(rm)<<shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;
	MMU_READ_WORD(address,&rslt32);
	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_p_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = 0;
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;
	MMU_READ_WORD(address,&rslt32);
	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_p_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (READ_REG(rm)>>shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;
	MMU_READ_WORD(address,&rslt32);
	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_p_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (BIT31(READ_REG(rm))?~0:0);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;
	MMU_READ_WORD(address,&rslt32);
	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_p_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;
	MMU_READ_WORD(address,&rslt32);
	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_p_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;
	MMU_READ_WORD(address,&rslt32);
	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_p_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;
	MMU_READ_WORD(address,&rslt32);
	if (fault){
		ABORT( DataAbortV );
	}

	/*if (address&0x3)
		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/

	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt32);


}

void SIMIT_IMP(ld1_reg_b_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (READ_REG(rm)<<shift_imm);
	address = READ_REG(rn);
	
	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_b_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = 0;
	address = READ_REG(rn);
	
	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_b_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (READ_REG(rm)>>shift_imm);
	address = READ_REG(rn);
	
	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_b_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (BIT31(READ_REG(rm))?~0:0);
	address = READ_REG(rn);
	
	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_b_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = READ_REG(rn);
	
	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_b_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = READ_REG(rn);
	
	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_b_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = READ_REG(rn);
	
	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_pb_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (READ_REG(rm)<<shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_pb_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = 0;
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_pb_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (READ_REG(rm)>>shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_pb_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (BIT31(READ_REG(rm))?~0:0);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_pb_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_pb_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld1_reg_pb_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt8);


}

void SIMIT_IMP(ld2_imm_h_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint16_t rslt16;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (imm4_1 << 4) | imm4_2;

	address = READ_REG(rn);

	MMU_READ_HALF_WORD(address,&rslt16);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt16);


}

void SIMIT_IMP(ld2_imm_ph_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint16_t rslt16;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (imm4_1 << 4) | imm4_2;

	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_HALF_WORD(address,&rslt16);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
			WRITE_REG(rn, address);
	WRITE_REG(rd, rslt16);


}

void SIMIT_IMP(ld2_imm_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (imm4_1 << 4) | imm4_2;

	address = READ_REG(rn);
	
	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, (int8_t)rslt8);


}

void SIMIT_IMP(ld2_imm_ps_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (imm4_1 << 4) | imm4_2;

	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);

	WRITE_REG(rd, (int8_t)rslt8);


}

void SIMIT_IMP(ld2_imm_hs_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint16_t rslt16;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (imm4_1 << 4) | imm4_2;

	address = READ_REG(rn);

	MMU_READ_HALF_WORD(address,&rslt16);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, (int16_t)rslt16);



}

void SIMIT_IMP(ld2_imm_phs_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint16_t rslt16;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (imm4_1 << 4) | imm4_2;

	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_HALF_WORD(address,&rslt16);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, (int16_t)rslt16);


}

void SIMIT_IMP(ld2_reg_h_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint16_t rslt16;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = READ_REG(rm);
	address = READ_REG(rn);

	MMU_READ_HALF_WORD(address,&rslt16);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, rslt16);


}

void SIMIT_IMP(ld2_reg_ph_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint16_t rslt16;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = READ_REG(rm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;


	MMU_READ_HALF_WORD(address,&rslt16);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, rslt16);


}

void SIMIT_IMP(ld2_reg_s_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = READ_REG(rm);
	address = READ_REG(rn);
	
	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, (int8_t)rslt8);


}

void SIMIT_IMP(ld2_reg_ps_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint8_t rslt8;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = READ_REG(rm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, (int8_t)rslt8);


}

void SIMIT_IMP(ld2_reg_hs_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint16_t rslt16;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = READ_REG(rm);
	address = READ_REG(rn);

	MMU_READ_HALF_WORD(address,&rslt16);
	if(fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	WRITE_REG(rd, (int16_t)rslt16);


}

void SIMIT_IMP(ld2_reg_phs_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint16_t rslt16;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = READ_REG(rm);

	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_READ_HALF_WORD(address,&rslt16);
	if(fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);
	WRITE_REG(rd, (int16_t)rslt16);


}

void SIMIT_IMP(stt_imm_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_WORD(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12);


}

void SIMIT_IMP(stt_imm_b_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);
	
	MMU_WRITE_BYTE(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12);


}

void SIMIT_IMP(stt_reg_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = (READ_REG(rm)<<shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_WORD(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = 0;
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_WORD(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = (READ_REG(rm)>>shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_WORD(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = (BIT31(READ_REG(rm))?~0:0);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_WORD(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_WORD(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_WORD(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_WORD(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_b_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = (READ_REG(rm)<<shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_b_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = 0;
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_b_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = (READ_REG(rm)>>shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_b_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = (BIT31(READ_REG(rm))?~0:0);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_b_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_b_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(stt_reg_b_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint32_t address;
	uint32_t offset;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


    offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = READ_REG(rn);
	current_mode = TRANSLATION_MODE(USR_MODE);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	TRANSLATION_MODE(current_mode);

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_imm_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	mmu_fault_t fault;


	address = READ_REG(rn);
	MMU_WRITE_WORD(address, READ_REG(rd));

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12);


}

void SIMIT_IMP(st1_imm_p_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	mmu_fault_t fault;


	address = ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12;
	MMU_WRITE_WORD(address, READ_REG(rd));

	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_imm_b_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	mmu_fault_t fault;


	address = READ_REG(rn);
	MMU_WRITE_BYTE(address, READ_REG(rd));

	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12);


}

void SIMIT_IMP(st1_imm_pb_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm12 = (inst>>0)&4095;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	mmu_fault_t fault;


	address = ufld?READ_REG(rn)+imm12:READ_REG(rn)-imm12;
	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (READ_REG(rm)<<shift_imm);
	address = READ_REG(rn);

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = 0;
	address = READ_REG(rn);

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (READ_REG(rm)>>shift_imm);
	address = READ_REG(rn);

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (BIT31(READ_REG(rm))?~0:0);
	address = READ_REG(rn);

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = READ_REG(rn);

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = READ_REG(rn);

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = READ_REG(rn);

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_p_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (READ_REG(rm)<<shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_p_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = 0;
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_p_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (READ_REG(rm)>>shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_p_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (BIT31(READ_REG(rm))?~0:0);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_p_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_p_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_p_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_WORD(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_b_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (READ_REG(rm)<<shift_imm);
	address = READ_REG(rn);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_b_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = 0;
	address = READ_REG(rn);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_b_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (READ_REG(rm)>>shift_imm);
	address = READ_REG(rn);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_b_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (BIT31(READ_REG(rm))?~0:0);
	address = READ_REG(rn);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_b_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = READ_REG(rn);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_b_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = READ_REG(rn);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_b_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = READ_REG(rn);

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st1_reg_pb_lsl_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (READ_REG(rm)<<shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_pb_zero_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = 0;
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_pb_lsr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (READ_REG(rm)>>shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_pb_sign_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (BIT31(READ_REG(rm))?~0:0);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_pb_asr_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = ((int32_t)(READ_REG(rm))>>shift_imm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_pb_rrx_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = ((READ_REG(rm)>>1)|(C_FLAG<<31));
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st1_reg_pb_ror_mode2_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned shift_imm = (inst>>7)&31;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t val32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


    offset = (val32=READ_REG(rm), (val32>>shift_imm)|(val32<<(32-shift_imm)));
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_BYTE(address, READ_REG(rd));
	if (fault){
		ABORT( DataAbortV );
	}
	if(wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st2_imm_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (imm4_1 << 4) | imm4_2;
	address = READ_REG(rn);

	MMU_WRITE_HALF_WORD(address, READ_REG(rd));
	
	if (fault){
		ABORT( DataAbortV );
	}

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st2_imm_p_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = (imm4_1 << 4) | imm4_2;
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_HALF_WORD(address, READ_REG(rd));

	if (fault){
		ABORT( DataAbortV );
	}
	if (wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(st2_reg_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = READ_REG(rm);
	address = READ_REG(rn);

	MMU_WRITE_HALF_WORD(address, READ_REG(rd));
	
	if (fault){
		ABORT( DataAbortV );
	}

	WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);


}

void SIMIT_IMP(st2_reg_p_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	offset = READ_REG(rm);
	address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

	MMU_WRITE_HALF_WORD(address, READ_REG(rd));

	if (fault){
		ABORT( DataAbortV );
	}

	if (wfld)
		WRITE_REG(rn, address);


}

void SIMIT_IMP(swap_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	mmu_fault_t fault;


	address = READ_REG(rn);
	MMU_READ_WORD(address,&rslt32);
	if (fault){
		ABORT( DataAbortV );
	}
	MMU_WRITE_WORD(address, READ_REG(rm));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rd,rslt32);


}

void SIMIT_IMP(swap_byte_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint8_t rslt8;
	uint32_t address;
	mmu_fault_t fault;


	address = READ_REG(rn);
	MMU_READ_BYTE(address,&rslt8);
	if(fault){
		ABORT( DataAbortV );
	}
	MMU_WRITE_BYTE(address, (uint8_t)READ_REG(rm));
	if (fault){
		ABORT( DataAbortV );
	}
	WRITE_REG(rd, (uint32_t)rslt8);


}

void SIMIT_IMP(ldm1_ldm1_000_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;
	
	
	base_addr = READ_REG(rn);
	start_addr = base_addr - pcount + 4;

	
	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_READ_WORD(address, &val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(ldm1_ldm1_001_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;
	
	
	base_addr = READ_REG(rn);
	start_addr = base_addr - pcount + 4;
	WRITE_REG(rn, base_addr - pcount);

	
	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_READ_WORD(address, &val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(ldm1_ldm1_010_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;
	
	
	base_addr = READ_REG(rn);
	start_addr = base_addr;

	
	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_READ_WORD(address, &val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(ldm1_ldm1_011_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;
	
	
	base_addr = READ_REG(rn);
	start_addr = base_addr;
	WRITE_REG(rn, base_addr + pcount);

	
	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_READ_WORD(address, &val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(ldm1_ldm1_100_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;
	
	
	base_addr = READ_REG(rn);
	start_addr = base_addr - pcount;

	
	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_READ_WORD(address, &val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(ldm1_ldm1_101_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;
	
	
	base_addr = READ_REG(rn);
	start_addr = base_addr - pcount;
	WRITE_REG(rn, base_addr - pcount);

	
	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_READ_WORD(address, &val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(ldm1_ldm1_110_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;
	
	
	base_addr = READ_REG(rn);
	start_addr = base_addr + 4;

	
	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_READ_WORD(address, &val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(ldm1_ldm1_111_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;
	
	
	base_addr = READ_REG(rn);
	start_addr = base_addr + 4;
	WRITE_REG(rn, base_addr + pcount);

	
	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_READ_WORD(address, &val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(ldm2_ldm2_000_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask2)*4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr - pcount + 4;


	address = start_addr-(start_addr&0x3);
	current_mode = CPU_MODE;
	SWITCH_BANK(current_mode,USR_MODE);	

	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				SWITCH_BANK(USR_MODE,current_mode);
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	SWITCH_BANK(USR_MODE,current_mode);	



}

void SIMIT_IMP(ldm2_ldm2_010_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask2)*4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr;


	address = start_addr-(start_addr&0x3);
	current_mode = CPU_MODE;
	SWITCH_BANK(current_mode,USR_MODE);	

	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				SWITCH_BANK(USR_MODE,current_mode);
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	SWITCH_BANK(USR_MODE,current_mode);	



}

void SIMIT_IMP(ldm2_ldm2_100_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask2)*4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr - pcount;


	address = start_addr-(start_addr&0x3);
	current_mode = CPU_MODE;
	SWITCH_BANK(current_mode,USR_MODE);	

	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				SWITCH_BANK(USR_MODE,current_mode);
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	SWITCH_BANK(USR_MODE,current_mode);	



}

void SIMIT_IMP(ldm2_ldm2_110_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask2)*4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr + 4;


	address = start_addr-(start_addr&0x3);
	current_mode = CPU_MODE;
	SWITCH_BANK(current_mode,USR_MODE);	

	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				SWITCH_BANK(USR_MODE,current_mode);
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	SWITCH_BANK(USR_MODE,current_mode);	



}

void SIMIT_IMP(ldm3_ldm3_000_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;



	pcount = popcount16(reg_mask2)*4 + 4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr - pcount + 4;


	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	WRITE_CPSR(SPSR);
	MMU_READ_WORD(address,&val32);
	if(fault){
		WRITE_REG((inst>>16)&0xF, base_addr);
		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
		ABORT( DataAbortV );
	}

	WRITE_REG(15, val32);




}

void SIMIT_IMP(ldm3_ldm3_001_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;



	pcount = popcount16(reg_mask2)*4 + 4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr - pcount + 4;
	WRITE_REG(rn, base_addr - pcount);


	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	WRITE_CPSR(SPSR);
	MMU_READ_WORD(address,&val32);
	if(fault){
		WRITE_REG((inst>>16)&0xF, base_addr);
		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
		ABORT( DataAbortV );
	}

	WRITE_REG(15, val32);




}

void SIMIT_IMP(ldm3_ldm3_010_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;



	pcount = popcount16(reg_mask2)*4 + 4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr;


	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	WRITE_CPSR(SPSR);
	MMU_READ_WORD(address,&val32);
	if(fault){
		WRITE_REG((inst>>16)&0xF, base_addr);
		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
		ABORT( DataAbortV );
	}

	WRITE_REG(15, val32);




}

void SIMIT_IMP(ldm3_ldm3_011_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;



	pcount = popcount16(reg_mask2)*4 + 4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr;
	WRITE_REG(rn, base_addr + pcount);


	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	WRITE_CPSR(SPSR);
	MMU_READ_WORD(address,&val32);
	if(fault){
		WRITE_REG((inst>>16)&0xF, base_addr);
		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
		ABORT( DataAbortV );
	}

	WRITE_REG(15, val32);




}

void SIMIT_IMP(ldm3_ldm3_100_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;



	pcount = popcount16(reg_mask2)*4 + 4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr - pcount;


	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	WRITE_CPSR(SPSR);
	MMU_READ_WORD(address,&val32);
	if(fault){
		WRITE_REG((inst>>16)&0xF, base_addr);
		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
		ABORT( DataAbortV );
	}

	WRITE_REG(15, val32);




}

void SIMIT_IMP(ldm3_ldm3_101_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;



	pcount = popcount16(reg_mask2)*4 + 4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr - pcount;
	WRITE_REG(rn, base_addr - pcount);


	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	WRITE_CPSR(SPSR);
	MMU_READ_WORD(address,&val32);
	if(fault){
		WRITE_REG((inst>>16)&0xF, base_addr);
		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
		ABORT( DataAbortV );
	}

	WRITE_REG(15, val32);




}

void SIMIT_IMP(ldm3_ldm3_110_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;



	pcount = popcount16(reg_mask2)*4 + 4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr + 4;


	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	WRITE_CPSR(SPSR);
	MMU_READ_WORD(address,&val32);
	if(fault){
		WRITE_REG((inst>>16)&0xF, base_addr);
		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
		ABORT( DataAbortV );
	}

	WRITE_REG(15, val32);




}

void SIMIT_IMP(ldm3_ldm3_111_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask2 = (inst>>0)&32767;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;



	pcount = popcount16(reg_mask2)*4 + 4;
	
	base_addr = READ_REG(rn);	
	start_addr = base_addr + 4;
	WRITE_REG(rn, base_addr + pcount);


	address = start_addr-(start_addr&0x3);
	iterator = rmo16(reg_mask2);
	while (iterator<15) {
		if (reg_mask2&(1<<iterator)) {
			MMU_READ_WORD(address,&val32);
			if(fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			WRITE_REG(iterator, val32);
			address += 4;
		}
		iterator++;
	}

	WRITE_CPSR(SPSR);
	MMU_READ_WORD(address,&val32);
	if(fault){
		WRITE_REG((inst>>16)&0xF, base_addr);
		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
		ABORT( DataAbortV );
	}

	WRITE_REG(15, val32);




}

void SIMIT_IMP(stm1_stm1_000_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr - pcount + 4;


	address = start_addr - (start_addr&0x3);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(stm1_stm1_001_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr - pcount + 4;
	WRITE_REG(rn, base_addr - pcount);


	address = start_addr - (start_addr&0x3);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(stm1_stm1_010_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr;


	address = start_addr - (start_addr&0x3);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(stm1_stm1_011_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr;
	WRITE_REG(rn, base_addr + pcount);


	address = start_addr - (start_addr&0x3);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(stm1_stm1_100_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr - pcount;


	address = start_addr - (start_addr&0x3);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(stm1_stm1_101_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr - pcount;
	WRITE_REG(rn, base_addr - pcount);


	address = start_addr - (start_addr&0x3);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(stm1_stm1_110_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr + 4;


	address = start_addr - (start_addr&0x3);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(stm1_stm1_111_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr + 4;
	WRITE_REG(rn, base_addr + pcount);


	address = start_addr - (start_addr&0x3);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}



}

void SIMIT_IMP(stm2_stm2_000_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr - pcount + 4;


	address = start_addr - (start_addr&0x3);
	current_mode = CPU_MODE;
	SWITCH_BANK(current_mode,USR_MODE);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				SWITCH_BANK(USR_MODE,current_mode);
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}
	SWITCH_BANK(USR_MODE,current_mode);



}

void SIMIT_IMP(stm2_stm2_010_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr;


	address = start_addr - (start_addr&0x3);
	current_mode = CPU_MODE;
	SWITCH_BANK(current_mode,USR_MODE);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				SWITCH_BANK(USR_MODE,current_mode);
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}
	SWITCH_BANK(USR_MODE,current_mode);



}

void SIMIT_IMP(stm2_stm2_100_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr - pcount;


	address = start_addr - (start_addr&0x3);
	current_mode = CPU_MODE;
	SWITCH_BANK(current_mode,USR_MODE);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				SWITCH_BANK(USR_MODE,current_mode);
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}
	SWITCH_BANK(USR_MODE,current_mode);



}

void SIMIT_IMP(stm2_stm2_110_) (emulator_t *emu, target_inst_t inst)
{

	unsigned reg_mask = (inst>>0)&65535;
	unsigned rn = (inst>>16)&15;

	uint32_t address;
	uint32_t start_addr;
	uint32_t base_addr;
	uint32_t pcount;
	uint32_t iterator;
	cpu_mode_t current_mode;
	mmu_fault_t fault;


	pcount = popcount16(reg_mask)*4;

	
	base_addr = READ_REG(rn);
	start_addr = base_addr + 4;


	address = start_addr - (start_addr&0x3);
	current_mode = CPU_MODE;
	SWITCH_BANK(current_mode,USR_MODE);

	/* This is not perfect exception here since
	   write may have been committed before exception occurs. */
	iterator = rmo16(reg_mask);
	while (iterator<16) {
		if (reg_mask&(1<<iterator)) {
			MMU_WRITE_WORD(address, READ_REG(iterator));
			if (fault){
				SWITCH_BANK(USR_MODE,current_mode);
				WRITE_REG((inst>>16)&0xF, base_addr);
				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);
				ABORT( DataAbortV );
			}
			address += 4;
		}
		iterator++;
	}
	SWITCH_BANK(USR_MODE,current_mode);



}

void SIMIT_IMP(branch_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm24 = (inst>>0)&16777215;


		SET_PC(GET_PC()+((int32_t)(imm24<<8)>>6));

}

void SIMIT_IMP(branch_exchange_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;



	if (READ_REG(rm) & 1){
		ABORT2(UndefinedInstrV);
	}
	WRITE_REG(15, READ_REG(rm) & 0xFFFFFFFC);


}

void SIMIT_IMP(branch_link_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm24 = (inst>>0)&16777215;



	WRITE_REG(LRIND, GET_PC()-4);

		SET_PC(GET_PC()+((int32_t)(imm24<<8)>>6));

}

void SIMIT_IMP(mrs_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;



    WRITE_REG(rd, CPSR);


}

void SIMIT_IMP(mrs_r_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;



    WRITE_REG(rd, SPSR);


}

void SIMIT_IMP(msri_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned field_mask = (inst>>16)&15;

	uint32_t rslt32;


    rslt32 = imm8;
    UPDATE_CPSR(rslt32, field_mask);


}

void SIMIT_IMP(msri_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned field_mask = (inst>>16)&15;

	uint32_t rslt32;


    rslt32 = ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))));
    UPDATE_CPSR(rslt32, field_mask);


}

void SIMIT_IMP(msri_r_imm_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned field_mask = (inst>>16)&15;

	uint32_t rslt32;


    rslt32 = imm8;
    UPDATE_SPSR(rslt32, field_mask);


}

void SIMIT_IMP(msri_r_rot_mode1_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned rotate = (inst>>8)&15;
	unsigned field_mask = (inst>>16)&15;

	uint32_t rslt32;


    rslt32 = ((imm8>>(rotate<<1))|(imm8<<(32-(rotate<<1))));
    UPDATE_SPSR(rslt32, field_mask);


}

void SIMIT_IMP(msr_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned field_mask = (inst>>16)&15;



    UPDATE_CPSR(READ_REG(rm), field_mask);


}

void SIMIT_IMP(msr_r_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned field_mask = (inst>>16)&15;



    UPDATE_SPSR(READ_REG(rm), field_mask);


}

void SIMIT_IMP(sc_) (emulator_t *emu, target_inst_t inst)
{




	if (inst == ABORTWORD && emu->abort_addr == (GET_PC()-8) ) {
		/* A prefetch abort.  */
		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_MMU_EXCPT, GET_PC()-8);
		ABORT2(PrefetchAbortV);
	}

	DO_SWI(inst);


}

void SIMIT_IMP(cdp_) (emulator_t *emu, target_inst_t inst)
{

	unsigned cp_num = (inst>>8)&15;

	uint32_t cpab;


		
	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}

	
	cpab = emu->copro[cp_num]->CDP(ARMul_FIRST, inst);
	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->CDP(ARMul_INTERRUPT, inst);
		}
		else
			cpab = emu->copro[cp_num]->CDP(ARMul_BUSY, inst);
	}


	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}



}

void SIMIT_IMP(mcr_) (emulator_t *emu, target_inst_t inst)
{

	unsigned cp_num = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t cpab;



	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->MCR(ARMul_FIRST, inst, READ_REG(rd));

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->MCR(ARMul_INTERRUPT, inst, READ_REG(rd));
		}
		else
			cpab = emu->copro[cp_num]->MCR(ARMul_BUSY, inst, READ_REG(rd));
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}


}

void SIMIT_IMP(mrc_) (emulator_t *emu, target_inst_t inst)
{

	unsigned cp_num = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t cpab;

	
	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->MRC(ARMul_FIRST, inst, &val32);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->MRC(ARMul_INTERRUPT, inst, &val32);
		}
		else
			cpab = emu->copro[cp_num]->MRC(ARMul_BUSY, inst, &val32);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}
	
	if (rd  == 15) {
		ASGN_N ((val32 & (1<<31)) != 0);
		ASGN_Z ((val32 & (1<<30)) != 0);
		ASGN_C ((val32 & (1<<29)) != 0);
		ASGN_V ((val32 & (1<<28)) != 0);
	}
	else{
		WRITE_REG(rd,val32);
	}


}

void SIMIT_IMP(ldc1_ldc1_01_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	
	address = READ_REG(rn);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->LDC(ARMul_FIRST, inst, 0);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->LDC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->LDC(ARMul_BUSY, inst, 0);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}

	cpab = emu->copro[cp_num]->LDC(ARMul_TRANSFER, inst, 0);
	MMU_READ_WORD(address,&rslt32);
	if(fault){
		ABORT( DataAbortV );
	}

	cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);

	while (cpab == ARMul_INC) {
		address += 4;
		MMU_READ_WORD(address,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);
	}


}

void SIMIT_IMP(ldc1_ldc1_10_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	
	address = READ_REG(rn) - (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->LDC(ARMul_FIRST, inst, 0);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->LDC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->LDC(ARMul_BUSY, inst, 0);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}

	cpab = emu->copro[cp_num]->LDC(ARMul_TRANSFER, inst, 0);
	MMU_READ_WORD(address,&rslt32);
	if(fault){
		ABORT( DataAbortV );
	}

	cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);

	while (cpab == ARMul_INC) {
		address += 4;
		MMU_READ_WORD(address,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);
	}


}

void SIMIT_IMP(ldc1_ldc1_11_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	
	address = READ_REG(rn) + (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->LDC(ARMul_FIRST, inst, 0);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->LDC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->LDC(ARMul_BUSY, inst, 0);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}

	cpab = emu->copro[cp_num]->LDC(ARMul_TRANSFER, inst, 0);
	MMU_READ_WORD(address,&rslt32);
	if(fault){
		ABORT( DataAbortV );
	}

	cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);

	while (cpab == ARMul_INC) {
		address += 4;
		MMU_READ_WORD(address,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);
	}


}

void SIMIT_IMP(ldc2_ldc2_00_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	uint32_t start_addr;
	uint32_t base_addr;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	base_addr =  READ_REG(rn);
	
	start_addr = base_addr;
	address = base_addr - (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->LDC(ARMul_FIRST, inst, 0);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->LDC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->LDC(ARMul_BUSY, inst, 0);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}

	WRITE_REG(rn, address);
	cpab = emu->copro[cp_num]->LDC(ARMul_TRANSFER, inst, 0);
	MMU_READ_WORD(address,&rslt32);
	if(fault){
		WRITE_REG(rn, base_addr);
		ABORT( DataAbortV );
	}
	
	address = start_addr;
	cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);

	while (cpab == ARMul_INC) {
		address += 4;
		MMU_READ_WORD(address,&rslt32);
		if(fault){
			WRITE_REG(rn, base_addr);
			ABORT( DataAbortV );
		}
		cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);
	}


}

void SIMIT_IMP(ldc2_ldc2_01_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	uint32_t start_addr;
	uint32_t base_addr;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	base_addr =  READ_REG(rn);
	
	start_addr = base_addr;
	address = base_addr + (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->LDC(ARMul_FIRST, inst, 0);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->LDC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->LDC(ARMul_BUSY, inst, 0);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}

	WRITE_REG(rn, address);
	cpab = emu->copro[cp_num]->LDC(ARMul_TRANSFER, inst, 0);
	MMU_READ_WORD(address,&rslt32);
	if(fault){
		WRITE_REG(rn, base_addr);
		ABORT( DataAbortV );
	}
	
	address = start_addr;
	cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);

	while (cpab == ARMul_INC) {
		address += 4;
		MMU_READ_WORD(address,&rslt32);
		if(fault){
			WRITE_REG(rn, base_addr);
			ABORT( DataAbortV );
		}
		cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);
	}


}

void SIMIT_IMP(ldc2_ldc2_10_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	uint32_t start_addr;
	uint32_t base_addr;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	base_addr =  READ_REG(rn);
	
	start_addr = address = base_addr - (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->LDC(ARMul_FIRST, inst, 0);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->LDC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->LDC(ARMul_BUSY, inst, 0);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}

	WRITE_REG(rn, address);
	cpab = emu->copro[cp_num]->LDC(ARMul_TRANSFER, inst, 0);
	MMU_READ_WORD(address,&rslt32);
	if(fault){
		WRITE_REG(rn, base_addr);
		ABORT( DataAbortV );
	}
	
	address = start_addr;
	cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);

	while (cpab == ARMul_INC) {
		address += 4;
		MMU_READ_WORD(address,&rslt32);
		if(fault){
			WRITE_REG(rn, base_addr);
			ABORT( DataAbortV );
		}
		cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);
	}


}

void SIMIT_IMP(ldc2_ldc2_11_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	uint32_t start_addr;
	uint32_t base_addr;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	base_addr =  READ_REG(rn);
	
	start_addr = address = base_addr + (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->LDC(ARMul_FIRST, inst, 0);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->LDC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->LDC(ARMul_BUSY, inst, 0);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}

	WRITE_REG(rn, address);
	cpab = emu->copro[cp_num]->LDC(ARMul_TRANSFER, inst, 0);
	MMU_READ_WORD(address,&rslt32);
	if(fault){
		WRITE_REG(rn, base_addr);
		ABORT( DataAbortV );
	}
	
	address = start_addr;
	cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);

	while (cpab == ARMul_INC) {
		address += 4;
		MMU_READ_WORD(address,&rslt32);
		if(fault){
			WRITE_REG(rn, base_addr);
			ABORT( DataAbortV );
		}
		cpab = emu->copro[cp_num]->LDC(ARMul_DATA, inst, rslt32);
	}


}

void SIMIT_IMP(stc1_stc1_01_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	
	address = READ_REG(rn);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->STC(ARMul_FIRST, inst, &rslt32);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->STC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->STC(ARMul_BUSY, inst, &rslt32);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}

	cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
	MMU_WRITE_WORD(address,rslt32);
	if(fault){
		ABORT( DataAbortV );
	}
	while (cpab == ARMul_INC) {
		address += 4;
		cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
		MMU_WRITE_WORD(address,rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
	}


}

void SIMIT_IMP(stc1_stc1_10_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	
	address = READ_REG(rn) - (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->STC(ARMul_FIRST, inst, &rslt32);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->STC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->STC(ARMul_BUSY, inst, &rslt32);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}

	cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
	MMU_WRITE_WORD(address,rslt32);
	if(fault){
		ABORT( DataAbortV );
	}
	while (cpab == ARMul_INC) {
		address += 4;
		cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
		MMU_WRITE_WORD(address,rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
	}


}

void SIMIT_IMP(stc1_stc1_11_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	
	address = READ_REG(rn) + (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->STC(ARMul_FIRST, inst, &rslt32);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->STC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->STC(ARMul_BUSY, inst, &rslt32);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}

	cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
	MMU_WRITE_WORD(address,rslt32);
	if(fault){
		ABORT( DataAbortV );
	}
	while (cpab == ARMul_INC) {
		address += 4;
		cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
		MMU_WRITE_WORD(address,rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
	}


}

void SIMIT_IMP(stc2_stc2_00_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	uint32_t start_addr;
	uint32_t base_addr;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	base_addr =  READ_REG(rn);
	
	start_addr = base_addr;
	address = base_addr - (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->STC(ARMul_FIRST, inst, &rslt32);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->STC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->STC(ARMul_BUSY, inst, &rslt32);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}
	
	WRITE_REG(rn, address);

	address = start_addr;
	cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
	MMU_WRITE_WORD(address,rslt32);
	if(fault){
		WRITE_REG(rn, base_addr);
		ABORT( DataAbortV );
	}
	while (cpab == ARMul_INC) {
		address += 4;
		cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
		MMU_WRITE_WORD(address,rslt32);
		if(fault){
			WRITE_REG(rn, base_addr);
			ABORT( DataAbortV );
		}
	}


}

void SIMIT_IMP(stc2_stc2_01_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	uint32_t start_addr;
	uint32_t base_addr;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	base_addr =  READ_REG(rn);
	
	start_addr = base_addr;
	address = base_addr + (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->STC(ARMul_FIRST, inst, &rslt32);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->STC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->STC(ARMul_BUSY, inst, &rslt32);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}
	
	WRITE_REG(rn, address);

	address = start_addr;
	cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
	MMU_WRITE_WORD(address,rslt32);
	if(fault){
		WRITE_REG(rn, base_addr);
		ABORT( DataAbortV );
	}
	while (cpab == ARMul_INC) {
		address += 4;
		cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
		MMU_WRITE_WORD(address,rslt32);
		if(fault){
			WRITE_REG(rn, base_addr);
			ABORT( DataAbortV );
		}
	}


}

void SIMIT_IMP(stc2_stc2_10_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	uint32_t start_addr;
	uint32_t base_addr;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	base_addr =  READ_REG(rn);
	
	start_addr = address = base_addr - (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->STC(ARMul_FIRST, inst, &rslt32);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->STC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->STC(ARMul_BUSY, inst, &rslt32);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}
	
	WRITE_REG(rn, address);

	address = start_addr;
	cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
	MMU_WRITE_WORD(address,rslt32);
	if(fault){
		WRITE_REG(rn, base_addr);
		ABORT( DataAbortV );
	}
	while (cpab == ARMul_INC) {
		address += 4;
		cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
		MMU_WRITE_WORD(address,rslt32);
		if(fault){
			WRITE_REG(rn, base_addr);
			ABORT( DataAbortV );
		}
	}


}

void SIMIT_IMP(stc2_stc2_11_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm8 = (inst>>0)&255;
	unsigned cp_num = (inst>>8)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	uint32_t start_addr;
	uint32_t base_addr;
	mmu_fault_t fault;
	uint32_t cpab;


	offset = imm8;
	base_addr =  READ_REG(rn);
	
	start_addr = address = base_addr + (offset << 2);
;

	if (emu->copro[cp_num] == NULL){
		ABORT2(UndefinedInstrV);
	}

	if (!CP_ACCESS_ALLOWED (cp_num)) {
		ABORT2(UndefinedInstrV);
	}
	
	cpab = emu->copro[cp_num]->STC(ARMul_FIRST, inst, &rslt32);

	while (cpab == ARMul_BUSY) {
		if (emu->int_pending ()) {
			cpab = emu->copro[cp_num]->STC(ARMul_INTERRUPT, inst, 0);
		}
		else
			cpab = emu->copro[cp_num]->STC(ARMul_BUSY, inst, &rslt32);
	}

	if (cpab == ARMul_CANT){
		ABORT2(UndefinedInstrV);
	}
	
	WRITE_REG(rn, address);

	address = start_addr;
	cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
	MMU_WRITE_WORD(address,rslt32);
	if(fault){
		WRITE_REG(rn, base_addr);
		ABORT( DataAbortV );
	}
	while (cpab == ARMul_INC) {
		address += 4;
		cpab = emu->copro[cp_num]->STC(ARMul_DATA, inst, &rslt32);
		MMU_WRITE_WORD(address,rslt32);
		if(fault){
			WRITE_REG(rn, base_addr);
			ABORT( DataAbortV );
		}
	}


}

void SIMIT_IMP(mra_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;


	if (emu->is_xscale()) {
	/* XScale MRA insn.  Move accumulator into two registers.  */
		val32 = (emu->accumulator >> 32) & 255;
		if (val32 & 128)
			val32 -= 256;
		
		WRITE_REG(rd,emu->accumulator);
		WRITE_REG(rn,val32);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(mar_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;



	if (emu->is_xscale()) {
	/* XScale MAR insn.  Move two registers into accumulator.  */
		emu->accumulator = READ_REG(rd);
		emu->accumulator += (uint64_t)READ_REG(rn)<<32;
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(clz_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;

	uint32_t val32;
	uint32_t rslt32;


	
	if (emu->is_xscale()) {
		val32=READ_REG(rm);
		if(val32==0){
			WRITE_REG(rd,32);
		}
		else{
			for(rslt32=0;(val32 & 0x80000000) == 0; val32 <<=1)
				rslt32++;
			WRITE_REG(rd,rslt32);
		}
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(ldd_imm_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	if (emu->is_xscale() ) {
		if(rd & 1)
			ABORT2(UndefinedInstrV);
		
		offset = (imm4_1 << 4) | imm4_2;

		address = READ_REG(rn);
	
		fault = NO_FAULT;

		if(address & 7)/* Alignment violation.  */
			ABORT( DataAbortV );

		MMU_READ_WORD(address,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		WRITE_REG(rd, rslt32);
		
		MMU_READ_WORD(address+4,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		WRITE_REG(rd+1, rslt32);
		
		WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(ldd_imm_p_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	if (emu->is_xscale() ) {
		if(rd & 1)
			ABORT2(UndefinedInstrV);
		
		offset = (imm4_1 << 4) | imm4_2;
	
		address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;
		
		fault = NO_FAULT;

		if(address & 7)/* Alignment violation.  */
			ABORT( DataAbortV );

		MMU_READ_WORD(address,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		WRITE_REG(rd, rslt32);
		
		MMU_READ_WORD(address+4,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		WRITE_REG(rd+1, rslt32);

		if (wfld)
			WRITE_REG(rn, address);
	
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(ldd_reg_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	if (emu->is_xscale() ) {
		if(rd & 1)
			ABORT2(UndefinedInstrV);

		offset = READ_REG(rm);
		address = READ_REG(rn);
	
		fault = NO_FAULT;

		if(address & 7)/* Alignment violation.  */
			ABORT( DataAbortV );

		MMU_READ_WORD(address,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		WRITE_REG(rd, rslt32);
		
		MMU_READ_WORD(address+4,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		WRITE_REG(rd+1, rslt32);

		WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(ldd_reg_p_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t rslt32;
	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	if (emu->is_xscale() ) {
		if(rd & 1)
			ABORT2(UndefinedInstrV);
	
		offset = READ_REG(rm);
		address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

		fault = NO_FAULT;

		if(address & 7)/* Alignment violation.  */
			ABORT( DataAbortV );

		MMU_READ_WORD(address,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		WRITE_REG(rd, rslt32);
		
		MMU_READ_WORD(address+4,&rslt32);
		if(fault){
			ABORT( DataAbortV );
		}
		WRITE_REG(rd+1, rslt32);

		if (wfld)
			WRITE_REG(rn, address);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(std_imm_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	if (emu->is_xscale() ) {
		if(rd & 1)
			ABORT2(UndefinedInstrV);
	
		offset = (imm4_1 << 4) | imm4_2;
		address = READ_REG(rn);

		fault = NO_FAULT;

		if(address & 7)/* Alignment violation.  */
			ABORT( DataAbortV );

		MMU_WRITE_WORD(address, READ_REG(rd));
		if (fault){
			ABORT( DataAbortV );
		}
		MMU_WRITE_WORD(address+4, READ_REG(rd+1));
		if (fault){
			ABORT( DataAbortV );
		}

		WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(std_imm_p_) (emulator_t *emu, target_inst_t inst)
{

	unsigned imm4_2 = (inst>>0)&15;
	unsigned imm4_1 = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	if (emu->is_xscale() ) {
		if(rd & 1)
			ABORT2(UndefinedInstrV);
	
		offset = (imm4_1 << 4) | imm4_2;
		address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

		fault = NO_FAULT;

		if(address & 7)/* Alignment violation.  */
			ABORT( DataAbortV );

		MMU_WRITE_WORD(address, READ_REG(rd));
		if (fault){
			ABORT( DataAbortV );
		}
		MMU_WRITE_WORD(address+4, READ_REG(rd+1));
		if (fault){
			ABORT( DataAbortV );
		}

		if (wfld)
			WRITE_REG(rn, address);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(std_reg_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	if (emu->is_xscale() ) {
		if(rd & 1)
			ABORT2(UndefinedInstrV);
	
		offset = READ_REG(rm);
		address = READ_REG(rn);

		fault = NO_FAULT;

		if(address & 7)/* Alignment violation.  */
			ABORT( DataAbortV );

		MMU_WRITE_WORD(address, READ_REG(rd));
		if (fault){
			ABORT( DataAbortV );
		}
		MMU_WRITE_WORD(address+4, READ_REG(rd+1));
		if (fault){
			ABORT( DataAbortV );
		}

		WRITE_REG(rn, ufld?READ_REG(rn)+offset:READ_REG(rn)-offset);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(std_reg_p_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;
	unsigned wfld = (inst>>21)&1;
	unsigned ufld = (inst>>23)&1;

	uint32_t address;
	uint32_t offset;
	mmu_fault_t fault;


	if (emu->is_xscale() ) {
		if(rd & 1)
			ABORT2(UndefinedInstrV);

		offset = READ_REG(rm);
		address = ufld?READ_REG(rn)+offset:READ_REG(rn)-offset;

		fault = NO_FAULT;

		if(address & 7)/* Alignment violation.  */
			ABORT( DataAbortV );

		MMU_WRITE_WORD(address, READ_REG(rd));
		if (fault){
			ABORT( DataAbortV );
		}
		MMU_WRITE_WORD(address+4, READ_REG(rd+1));
		if (fault){
			ABORT( DataAbortV );
		}

		if (wfld)
			WRITE_REG(rn, address);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(smla_xy_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned x_bit = (inst>>5)&1;
	unsigned y_bit = (inst>>6)&1;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>12)&15;
	unsigned rd = (inst>>16)&15;

	uint32_t op1;
	uint32_t op2;
	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	if (emu->is_xscale() ) {
	/*  SMLAxy insn.  */
		
		val32 = READ_REG(rn);
		
		op1 = READ_REG(rm); 
		op2 = READ_REG(rs);

		op1 = x_bit ? op1>>16 : op1&0xFFFF;
		op2 = y_bit ? op2>>16 : op2&0xFFFF;
						
		if (op1 & 0x8000)
			op1 -= 65536;
		if (op2 & 0x8000)
			op2 -= 65536;
		
		tmp32 = op1 * op2;
		rslt32 = val32 + tmp32;
		
		WRITE_REG(rd, rslt32);

		ASGN_Q((val32^tmp32^-1) & (val32^rslt32));

	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(smlal_xy_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned x_bit = (inst>>5)&1;
	unsigned y_bit = (inst>>6)&1;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t op1;
	uint32_t op2;
	uint64_t rslt64;


	if (emu->is_xscale() ) {
	/*  SMLALxy insn.  */

		op1 = READ_REG(rm); 
		op2 = READ_REG(rs);

		op1 = x_bit ? op1>>16 : op1&0xFFFF;
		op2 = y_bit ? op2>>16 : op2&0xFFFF;
						
		if (op1 & 0x8000)
			op1 -= 65536;
		if (op2 & 0x8000)
			op2 -= 65536;

		rslt64 = ((uint64_t)READ_REG(rn)<<32) + (uint64_t)READ_REG(rd);
		rslt64 = (int64_t)op1 * (int64_t)op2 +rslt64;

		WRITE_REG(rn, (uint32_t)(rslt64>>32));
		WRITE_REG(rd, (uint32_t)rslt64);

	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(smlaw_y_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned y_bit = (inst>>6)&1;
	unsigned rs = (inst>>8)&15;
	unsigned rn = (inst>>12)&15;
	unsigned rd = (inst>>16)&15;

	uint32_t op2;
	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	if (emu->is_xscale() ) {
	/*  SMLAWy insn.  */
		
		val32  =  READ_REG(rn);

		op2 = READ_REG(rs);
		op2 = y_bit ? op2>>16 : op2&0xFFFF;
						
		if (op2 & 0x8000)
			op2 -= 65536;

		tmp32  = (uint32_t)( (int64_t)READ_REG(rm)*op2  >> 16);
		rslt32 = val32 + tmp32;

		WRITE_REG(rd,rslt32);

		ASGN_Q((val32^tmp32^-1) & (val32^rslt32));
	
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(smul_xy_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned x_bit = (inst>>5)&1;
	unsigned y_bit = (inst>>6)&1;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>16)&15;

	uint32_t op1;
	uint32_t op2;


	if (emu->is_xscale() ) {
	/*  SMULxy insn.  */

		op1 = READ_REG(rm); 
		op2 = READ_REG(rs);

		op1 = x_bit ? op1>>16 : op1&0xFFFF;
		op2 = y_bit ? op2>>16 : op2&0xFFFF;

		if (op1 & 0x8000)
			op1 -= 65536;
		if (op2 & 0x8000)
			op2 -= 65536;

		WRITE_REG(rd,op1 * op2);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(smulw_y_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned y_bit = (inst>>6)&1;
	unsigned rs = (inst>>8)&15;
	unsigned rd = (inst>>16)&15;

	uint32_t op2;
	uint32_t rslt32;


	if (emu->is_xscale() ) {
	/* SMULWy insn.  */

		op2 = READ_REG(rs);
		op2 = y_bit ? op2>>16 : op2&0xFFFF;
						
		if (op2 & 0x8000)
			op2 -= 65536;

		rslt32  = (uint32_t)( (int64_t)READ_REG(rm)*op2  >> 16); 

		WRITE_REG(rd,rslt32);
	
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(qadd_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	if (emu->is_xscale() ) {
	/*   QADD insn.  */

		val32 = READ_REG(rm); 
		tmp32 = READ_REG(rn);
		rslt32 = val32 + tmp32;


		if ((val32^tmp32^-1) & (val32^rslt32)){
			rslt32 = (rslt32 & 0x80000000)? 0x7fffffff:0x80000000;
			SET_Q;
		}
		
		WRITE_REG(rd,rslt32);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(qdadd_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	if (emu->is_xscale() ) {
	/*   QDADD insn.  */

		val32 = READ_REG(rm);
		tmp32 = READ_REG(rn); 


		if ((tmp32^tmp32^-1) & (tmp32^(tmp32<<1))){
			tmp32 = ((tmp32<<2) & 0x80000000)? 0x7fffffff:0x80000000;
			SET_Q;
		}
		else
			tmp32<<=1;
		
		rslt32 = val32 + tmp32;
		
		if ((val32^tmp32^-1) & (val32^rslt32)){
			rslt32 = (rslt32 & 0x80000000)? 0x7fffffff:0x80000000;
			SET_Q;
		}
		
		WRITE_REG(rd,rslt32);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(qsub_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	if (emu->is_xscale() ) {
	/*   QSUB insn.  */

		val32 = READ_REG(rm); 
		tmp32 = READ_REG(rn);
		rslt32 = val32 - tmp32;


		if ((val32^tmp32) & (val32^rslt32)){
			rslt32 = (rslt32 & 0x80000000)? 0x7fffffff:0x80000000;
			SET_Q;
		}
		
		WRITE_REG(rd,rslt32);
	}
	else
		ABORT2(UndefinedInstrV);


}

void SIMIT_IMP(qdsub_) (emulator_t *emu, target_inst_t inst)
{

	unsigned rm = (inst>>0)&15;
	unsigned rd = (inst>>12)&15;
	unsigned rn = (inst>>16)&15;

	uint32_t val32;
	uint32_t tmp32;
	uint32_t rslt32;


	if (emu->is_xscale() ) {
	/*   QDSUB insn.  */

		val32 = READ_REG(rm);
		tmp32 = READ_REG(rn); 


		if ((tmp32^tmp32^-1) & (tmp32^(tmp32<<1))){
			tmp32 = ((tmp32<<2) & 0x80000000)? 0x7fffffff:0x80000000;
			SET_Q;
		}
		else
			tmp32<<=1;
		
		rslt32 = val32 - tmp32;
		
		if ((val32^tmp32) & (val32^rslt32)){
			rslt32 = (rslt32 & 0x80000000)? 0x7fffffff:0x80000000;
			SET_Q;
		}
		
		WRITE_REG(rd,rslt32);
	}
	else
		ABORT2(UndefinedInstrV);


}

