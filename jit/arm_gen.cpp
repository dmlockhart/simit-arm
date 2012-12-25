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
#include "arch_gen.hpp"
char *emitc_movi_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, %u);\n"
	"", (inst>>12)&15, (inst>>0)&255);
	return buf;

}

char *emitc_movi_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ((%u>>(%u<<1))|(%u<<(32-(%u<<1)))));\n"
	"", (inst>>12)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_movis_imm_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (carry=C_FLAG,%u);\n"
	"	if (%u==15)\n"
	"		WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movis_rot_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (rslt32=(%u>>(%u<<1))|(%u<<(32-(%u<<1))), carry=BIT31(rslt32), rslt32);\n"
	"	if (%u==15)\n"
	"		WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mov_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (READ_REG(%u)<<%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_mov_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, 0);\n"
	"", (inst>>12)&15);
	return buf;

}

char *emitc_mov_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (READ_REG(%u)>>%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_mov_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (BIT31(READ_REG(%u))?~0:0));\n"
	"", (inst>>12)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mov_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ((int32_t)(READ_REG(%u))>>%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_mov_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ((READ_REG(%u)>>1)|(C_FLAG<<31)));\n"
	"", (inst>>12)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mov_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_mov_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mov_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mov_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mov_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_movs_lsz_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (carry=C_FLAG, READ_REG(%u));\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_lsl_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (val32=READ_REG(%u), carry=BITn(val32, 32-%u), val32<<%u);\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_zero_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (carry=BIT31(READ_REG(%u)), 0);\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_lsr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (val32=READ_REG(%u), carry=BITn(val32, %u-1), val32>>%u);\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_sign_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (val32=READ_REG(%u), carry=BIT31(val32), BIT31(val32)?~0:0);\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_asr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (val32=READ_REG(%u), carry=BITn(val32, %u-1), (int32_t)val32>>%u);\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_rrx_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (val32=READ_REG(%u), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_ror_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (val32=READ_REG(%u), carry=BITn(val32, %u-1), (val32>>%u)|(val32<<(32-%u)));\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_lsl_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_lsr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_asr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_movs_ror_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	if (%u==15)\n"
	"    	WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvni_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~%u);\n"
	"", (inst>>12)&15, (inst>>0)&255);
	return buf;

}

char *emitc_mvni_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~((%u>>(%u<<1))|(%u<<(32-(%u<<1)))));\n"
	"", (inst>>12)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_mvnis_imm_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(carry=C_FLAG,%u);\n"
	"	if (%u==15)\n"
	"		WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvnis_rot_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(rslt32=(%u>>(%u<<1))|(%u<<(32-(%u<<1))), carry=BIT31(rslt32), rslt32);\n"
	"	if (%u==15)\n"
	"		WRITE_CPSR(SPSR);\n"
	"	else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvn_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~(READ_REG(%u)<<%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_mvn_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~0);\n"
	"", (inst>>12)&15);
	return buf;

}

char *emitc_mvn_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~(READ_REG(%u)>>%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_mvn_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~(BIT31(READ_REG(%u))?~0:0));\n"
	"", (inst>>12)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mvn_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~((int32_t)(READ_REG(%u))>>%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_mvn_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~((READ_REG(%u)>>1)|(C_FLAG<<31)));\n"
	"", (inst>>12)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mvn_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~(val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_mvn_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mvn_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mvn_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mvn_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mvns_lsz_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(carry=C_FLAG, READ_REG(%u));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_lsl_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(val32=READ_REG(%u), carry=BITn(val32, 32-%u), val32<<%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_zero_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(carry=BIT31(READ_REG(%u)), 0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_lsr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(val32=READ_REG(%u), carry=BITn(val32, %u-1), val32>>%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_sign_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(val32=READ_REG(%u), carry=BIT31(val32), BIT31(val32)?~0:0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_asr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(val32=READ_REG(%u), carry=BITn(val32, %u-1), (int32_t)val32>>%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_rrx_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(val32=READ_REG(%u), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_ror_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(val32=READ_REG(%u), carry=BITn(val32, %u-1), (val32>>%u)|(val32<<(32-%u)));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_lsl_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_lsr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_asr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mvns_ror_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_addi_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + %u);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255);
	return buf;

}

char *emitc_addi_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + ((%u>>(%u<<1))|(%u<<(32-(%u<<1)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_addis_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = %u;\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_addis_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((%u>>(%u<<1))|(%u<<(32-(%u<<1))));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_add_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (READ_REG(%u)<<%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_add_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + 0);\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_add_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (READ_REG(%u)>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_add_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (BIT31(READ_REG(%u))?~0:0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_add_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + ((int32_t)(READ_REG(%u))>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_add_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + ((READ_REG(%u)>>1)|(C_FLAG<<31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_add_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_add_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_add_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_add_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_add_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_adds_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (READ_REG(%u)<<%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adds_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = 0;\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adds_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (READ_REG(%u)>>%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adds_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (BIT31(READ_REG(%u))?~0:0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adds_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adds_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adds_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adds_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adds_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adds_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adds_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, rslt32<val32, (val32^tmp32^-1) & (val32^rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adci_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + %u + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255);
	return buf;

}

char *emitc_adci_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + ((%u>>(%u<<1))|(%u<<(32-(%u<<1)))) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_adcis_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = %u;\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcis_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = ((%u>>(%u<<1))|(%u<<(32-(%u<<1))));\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adc_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (READ_REG(%u)<<%u) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_adc_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + 0 + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_adc_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (READ_REG(%u)>>%u) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_adc_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (BIT31(READ_REG(%u))?~0:0) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_adc_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + ((int32_t)(READ_REG(%u))>>%u) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_adc_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + ((READ_REG(%u)>>1)|(C_FLAG<<31)) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_adc_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_adc_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_adc_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_adc_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_adc_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) + (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))) + C_FLAG);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_adcs_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = (READ_REG(%u)<<%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcs_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = 0;\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcs_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = (READ_REG(%u)>>%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcs_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = (BIT31(READ_REG(%u))?~0:0);\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcs_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcs_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcs_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcs_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0);\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcs_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0);\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcs_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_adcs_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	val32  = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = tmp32+val32+1;\n"
	"		carry = rslt32 <= val32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = tmp32+val32;\n"
	"		carry = rslt32 < val32;\n"
	"	}\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subi_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - %u);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255);
	return buf;

}

char *emitc_subi_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - ((%u>>(%u<<1))|(%u<<(32-(%u<<1)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_subis_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = %u;\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subis_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = ((%u>>(%u<<1))|(%u<<(32-(%u<<1))));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sub_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (READ_REG(%u)<<%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_sub_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - 0);\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_sub_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (READ_REG(%u)>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_sub_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (BIT31(READ_REG(%u))?~0:0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sub_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - ((int32_t)(READ_REG(%u))>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_sub_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - ((READ_REG(%u)>>1)|(C_FLAG<<31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sub_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_sub_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sub_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sub_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sub_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_subs_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (READ_REG(%u)<<%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subs_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = 0;\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subs_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (READ_REG(%u)>>%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subs_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (BIT31(READ_REG(%u))?~0:0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subs_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subs_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subs_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subs_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subs_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subs_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_subs_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbci_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - %u - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255);
	return buf;

}

char *emitc_sbci_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - ((%u>>(%u<<1))|(%u<<(32-(%u<<1)))) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_sbcis_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = %u;\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcis_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((%u>>(%u<<1))|(%u<<(32-(%u<<1))));\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbc_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (READ_REG(%u)<<%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_sbc_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - 0 - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_sbc_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (READ_REG(%u)>>%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_sbc_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (BIT31(READ_REG(%u))?~0:0) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sbc_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - ((int32_t)(READ_REG(%u))>>%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_sbc_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - ((READ_REG(%u)>>1)|(C_FLAG<<31)) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sbc_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_sbc_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sbc_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sbc_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sbc_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) - (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_sbcs_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (READ_REG(%u)<<%u);\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcs_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = 0;\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcs_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (READ_REG(%u)>>%u);\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcs_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (BIT31(READ_REG(%u))?~0:0);\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcs_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcs_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcs_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcs_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0);\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcs_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0);\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcs_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_sbcs_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	val32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbi_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, %u - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>0)&255, (inst>>16)&15);
	return buf;

}

char *emitc_rsbi_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ((%u>>(%u<<1))|(%u<<(32-(%u<<1)))) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsbis_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = %u;\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbis_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = ((%u>>(%u<<1))|(%u<<(32-(%u<<1))));\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsb_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (READ_REG(%u)<<%u) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_rsb_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, 0 - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsb_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (READ_REG(%u)>>%u) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_rsb_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (BIT31(READ_REG(%u))?~0:0) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsb_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ((int32_t)(READ_REG(%u))>>%u) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_rsb_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ((READ_REG(%u)>>1)|(C_FLAG<<31)) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsb_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_rsb_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsb_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsb_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsb_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))) - READ_REG(%u));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsbs_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (READ_REG(%u)<<%u);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbs_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = 0;\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbs_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (READ_REG(%u)>>%u);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbs_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (BIT31(READ_REG(%u))?~0:0);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbs_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbs_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbs_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbs_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbs_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbs_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsbs_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	tmp32 = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, val32>=tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsci_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, %u - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>0)&255, (inst>>16)&15);
	return buf;

}

char *emitc_rsci_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ((%u>>(%u<<1))|(%u<<(32-(%u<<1)))) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rscis_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = %u;\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscis_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = ((%u>>(%u<<1))|(%u<<(32-(%u<<1))));\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rsc_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (READ_REG(%u)<<%u) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_rsc_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, 0 - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsc_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (READ_REG(%u)>>%u) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_rsc_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (BIT31(READ_REG(%u))?~0:0) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsc_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ((int32_t)(READ_REG(%u))>>%u) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_rsc_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, ((READ_REG(%u)>>1)|(C_FLAG<<31)) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsc_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_rsc_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsc_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsc_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rsc_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))) - READ_REG(%u) - (1-C_FLAG));\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_rscs_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (READ_REG(%u)<<%u);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscs_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = 0;\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscs_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (READ_REG(%u)>>%u);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscs_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (BIT31(READ_REG(%u))?~0:0);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscs_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscs_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscs_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscs_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscs_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0);\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscs_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_rscs_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	val32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	tmp32 = READ_REG(%u);\n"
	"	if (C_FLAG) {\n"
	"		rslt32 = val32 - tmp32;\n"
	"		carry = val32 >= tmp32;\n"
	"	}\n"
	"	else {\n"
	"		rslt32 = val32 - tmp32 - 1;\n"
	"		carry = val32 > tmp32;\n"
	"	}\n"
	"\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZCV(rslt32, carry, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_andi_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & %u);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255);
	return buf;

}

char *emitc_andi_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ((%u>>(%u<<1))|(%u<<(32-(%u<<1)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_andis_imm_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (carry=C_FLAG,%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&255, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_andis_rot_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (rslt32=(%u>>(%u<<1))|(%u<<(32-(%u<<1))), carry=BIT31(rslt32), rslt32);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_and_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & (READ_REG(%u)<<%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_and_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & 0);\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_and_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & (READ_REG(%u)>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_and_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & (BIT31(READ_REG(%u))?~0:0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_and_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ((int32_t)(READ_REG(%u))>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_and_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ((READ_REG(%u)>>1)|(C_FLAG<<31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_and_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_and_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_and_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_and_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_and_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_ands_lsz_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (carry=C_FLAG, READ_REG(%u));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_lsl_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BITn(val32, 32-%u), val32<<%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_zero_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (carry=BIT31(READ_REG(%u)), 0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_lsr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BITn(val32, %u-1), val32>>%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_sign_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BIT31(val32), BIT31(val32)?~0:0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_asr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BITn(val32, %u-1), (int32_t)val32>>%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_rrx_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_ror_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BITn(val32, %u-1), (val32>>%u)|(val32<<(32-%u)));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_lsl_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_lsr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_asr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ands_ror_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eori_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ %u);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255);
	return buf;

}

char *emitc_eori_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ ((%u>>(%u<<1))|(%u<<(32-(%u<<1)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_eoris_imm_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (carry=C_FLAG,%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&255, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eoris_rot_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (rslt32=(%u>>(%u<<1))|(%u<<(32-(%u<<1))), carry=BIT31(rslt32), rslt32);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eor_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ (READ_REG(%u)<<%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_eor_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ 0);\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_eor_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ (READ_REG(%u)>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_eor_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ (BIT31(READ_REG(%u))?~0:0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_eor_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ ((int32_t)(READ_REG(%u))>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_eor_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ ((READ_REG(%u)>>1)|(C_FLAG<<31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_eor_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_eor_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_eor_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_eor_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_eor_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_eors_lsz_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (carry=C_FLAG, READ_REG(%u));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_lsl_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BITn(val32, 32-%u), val32<<%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_zero_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (carry=BIT31(READ_REG(%u)), 0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_lsr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BITn(val32, %u-1), val32>>%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_sign_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BIT31(val32), BIT31(val32)?~0:0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_asr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BITn(val32, %u-1), (int32_t)val32>>%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_rrx_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_ror_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BITn(val32, %u-1), (val32>>%u)|(val32<<(32-%u)));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_lsl_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_lsr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_asr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_eors_ror_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orri_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | %u);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255);
	return buf;

}

char *emitc_orri_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | ((%u>>(%u<<1))|(%u<<(32-(%u<<1)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_orris_imm_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (carry=C_FLAG,%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&255, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orris_rot_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (rslt32=(%u>>(%u<<1))|(%u<<(32-(%u<<1))), carry=BIT31(rslt32), rslt32);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orr_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | (READ_REG(%u)<<%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_orr_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | 0);\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_orr_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | (READ_REG(%u)>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_orr_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | (BIT31(READ_REG(%u))?~0:0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_orr_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | ((int32_t)(READ_REG(%u))>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_orr_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | ((READ_REG(%u)>>1)|(C_FLAG<<31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_orr_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_orr_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_orr_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_orr_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_orr_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) | (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_orrs_lsz_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (carry=C_FLAG, READ_REG(%u));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_lsl_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (val32=READ_REG(%u), carry=BITn(val32, 32-%u), val32<<%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_zero_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (carry=BIT31(READ_REG(%u)), 0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_lsr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (val32=READ_REG(%u), carry=BITn(val32, %u-1), val32>>%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_sign_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (val32=READ_REG(%u), carry=BIT31(val32), BIT31(val32)?~0:0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_asr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (val32=READ_REG(%u), carry=BITn(val32, %u-1), (int32_t)val32>>%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_rrx_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (val32=READ_REG(%u), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_ror_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (val32=READ_REG(%u), carry=BITn(val32, %u-1), (val32>>%u)|(val32<<(32-%u)));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_lsl_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_lsr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_asr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_orrs_ror_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) | (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bici_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~%u);\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255);
	return buf;

}

char *emitc_bici_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~((%u>>(%u<<1))|(%u<<(32-(%u<<1)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_bicis_imm_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(carry=C_FLAG,%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&255, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bicis_rot_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(rslt32=(%u>>(%u<<1))|(%u<<(32-(%u<<1))), carry=BIT31(rslt32), rslt32);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bic_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~(READ_REG(%u)<<%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_bic_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~0);\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_bic_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~(READ_REG(%u)>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_bic_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~(BIT31(READ_REG(%u))?~0:0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_bic_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~((int32_t)(READ_REG(%u))>>%u));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31);
	return buf;

}

char *emitc_bic_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~((READ_REG(%u)>>1)|(C_FLAG<<31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_bic_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~(val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_bic_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_bic_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_bic_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31)));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_bic_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, READ_REG(%u) & ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f)))));\n"
	"", (inst>>12)&15, (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_bics_lsz_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(carry=C_FLAG, READ_REG(%u));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_lsl_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(val32=READ_REG(%u), carry=BITn(val32, 32-%u), val32<<%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_zero_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(carry=BIT31(READ_REG(%u)), 0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_lsr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(val32=READ_REG(%u), carry=BITn(val32, %u-1), val32>>%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_sign_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(val32=READ_REG(%u), carry=BIT31(val32), BIT31(val32)?~0:0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_asr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(val32=READ_REG(%u), carry=BITn(val32, %u-1), (int32_t)val32>>%u);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_rrx_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(val32=READ_REG(%u), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_ror_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(val32=READ_REG(%u), carry=BITn(val32, %u-1), (val32>>%u)|(val32<<(32-%u)));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>7)&31, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_lsl_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_lsr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_asr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_bics_ror_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & ~(tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"    if (%u==15)\n"
	"        WRITE_CPSR(SPSR);\n"
	"    else\n"
	"		ASGN_NZC(rslt32, carry);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_cmpi_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = %u;\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>0)&255, (inst>>16)&15);
	return buf;

}

char *emitc_cmpi_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32  = ((%u>>(%u<<1))|(%u<<(32-(%u<<1))));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmp_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (READ_REG(%u)<<%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_cmp_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = 0;\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>16)&15);
	return buf;

}

char *emitc_cmp_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (READ_REG(%u)>>%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_cmp_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (BIT31(READ_REG(%u))?~0:0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmp_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_cmp_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmp_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_cmp_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmp_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmp_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmp_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 - tmp32;\n"
	"\n"
	"	ASGN_NZCV(rslt32, val32 >= tmp32, (val32 ^ tmp32) & (val32 ^ rslt32));\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmni_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = %u;\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>0)&255, (inst>>16)&15);
	return buf;

}

char *emitc_cmni_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((%u>>(%u<<1))|(%u<<(32-(%u<<1))));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmn_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (READ_REG(%u)<<%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_cmn_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = 0;\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>16)&15);
	return buf;

}

char *emitc_cmn_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (READ_REG(%u)>>%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_cmn_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (BIT31(READ_REG(%u))?~0:0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmn_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_cmn_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmn_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15);
	return buf;

}

char *emitc_cmn_lsl_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32<<tmp32):0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmn_lsr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (tmp32<32)?(val32>>tmp32):0);\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmn_asr_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_cmn_ror_mode3_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	tmp32 = (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	val32  = READ_REG(%u);\n"
	"	rslt32 = val32 + tmp32;\n"
	"	ASGN_NZCV(rslt32, rslt32<val32, (val32 ^ tmp32 ^ -1) & (val32 ^ rslt32));\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_tsti_imm_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (carry=C_FLAG,%u);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&255);
	return buf;

}

char *emitc_tsti_rot_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (rslt32=(%u>>(%u<<1))|(%u<<(32-(%u<<1))), carry=BIT31(rslt32), rslt32);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_tst_lsz_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (carry=C_FLAG, READ_REG(%u));\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_tst_lsl_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BITn(val32, 32-%u), val32<<%u);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_tst_zero_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (carry=BIT31(READ_REG(%u)), 0);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_tst_lsr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BITn(val32, %u-1), val32>>%u);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_tst_sign_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BIT31(val32), BIT31(val32)?~0:0);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_tst_asr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BITn(val32, %u-1), (int32_t)val32>>%u);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_tst_rrx_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_tst_ror_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (val32=READ_REG(%u), carry=BITn(val32, %u-1), (val32>>%u)|(val32<<(32-%u)));\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_tst_lsl_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_tst_lsr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_tst_asr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_tst_ror_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) & (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_teqi_imm_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (carry=C_FLAG,%u);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&255);
	return buf;

}

char *emitc_teqi_rot_mode1_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (rslt32=(%u>>(%u<<1))|(%u<<(32-(%u<<1))), carry=BIT31(rslt32), rslt32);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15);
	return buf;

}

char *emitc_teq_lsz_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (carry=C_FLAG, READ_REG(%u));\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_teq_lsl_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BITn(val32, 32-%u), val32<<%u);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_teq_zero_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (carry=BIT31(READ_REG(%u)), 0);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_teq_lsr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BITn(val32, %u-1), val32>>%u);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_teq_sign_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BIT31(val32), BIT31(val32)?~0:0);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_teq_asr_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BITn(val32, %u-1), (int32_t)val32>>%u);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_teq_rrx_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BIT0(val32), (val32>>1)|(C_FLAG<<31));\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15);
	return buf;

}

char *emitc_teq_ror_mode2_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (val32=READ_REG(%u), carry=BITn(val32, %u-1), (val32>>%u)|(val32<<(32-%u)));\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>7)&31);
	return buf;

}

char *emitc_teq_lsl_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,32-val32)), (tmp32<32)?(val32<<tmp32):0);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_teq_lsr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>32)?0:BITn(val32,tmp32-1)), (tmp32<32)?(val32>>tmp32):0);\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_teq_asr_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32>31)?BIT31(val32):BITn(val32,tmp32-1)), (int32_t)val32>>(tmp32<32?tmp32:31));\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_teq_ror_mode3_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t carry;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = READ_REG(%u) ^ (tmp32=READ_REG(%u)&0xFF, val32=READ_REG(%u), carry=(tmp32==0)?C_FLAG:((tmp32&0x1f==0)?BIT31(val32):BITn(val32,(tmp32&0x1f)-1)), (val32>>(tmp32&0x1f))|(val32<<(32-(tmp32&0x1f))));\n"
	"	ASGN_NZC(rslt32, carry);\n"
	"", (inst>>16)&15, (inst>>8)&15, (inst>>0)&15);
	return buf;

}

char *emitc_mla_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (uint32_t)((uint64_t)READ_REG(%u) * (uint64_t)READ_REG(%u) + READ_REG(%u)));\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>8)&15, (inst>>12)&15);
	return buf;

}

char *emitc_mla_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (uint32_t)((uint64_t)READ_REG(%u) * (uint64_t)READ_REG(%u) + READ_REG(%u));\n"
	"	ASGN_NZ(rslt32);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>8)&15, (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_mul_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(%u, (uint32_t)((uint64_t)READ_REG(%u) * (uint64_t)READ_REG(%u)));\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>8)&15);
	return buf;

}

char *emitc_mul_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	rslt32 = (uint32_t)((uint64_t)READ_REG(%u) * (uint64_t)READ_REG(%u));\n"
	"	ASGN_NZ(rslt32);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>8)&15, (inst>>16)&15);
	return buf;

}

char *emitc_smull_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint64_t rslt64;\n");

	buf += sprintf(buf, "\n"
	"    rslt64 = (int64_t)READ_REG(%u) * (int64_t)READ_REG(%u);\n"
	"    WRITE_REG(%u, (uint32_t)(rslt64>>32));\n"
	"    WRITE_REG(%u, (uint32_t)rslt64);\n"
	"", (inst>>0)&15, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_smull_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint64_t rslt64;\n");

	buf += sprintf(buf, "\n"
	"    rslt64 = (int64_t)READ_REG(%u) * (int64_t)READ_REG(%u);\n"
	"	ASGN_NZ((uint32_t)rslt64);\n"
	"    WRITE_REG(%u, (uint32_t)(rslt64>>32));\n"
	"    WRITE_REG(%u, (uint32_t)rslt64);\n"
	"", (inst>>0)&15, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_smlal_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint64_t rslt64;\n");

	buf += sprintf(buf, "\n"
	"	rslt64 = ((uint64_t)READ_REG(%u)<<32) + (uint64_t)READ_REG(%u);\n"
	"    rslt64 = (int64_t)READ_REG(%u) * (int64_t)READ_REG(%u)+rslt64;\n"
	"    WRITE_REG(%u, (uint32_t)(rslt64>>32));\n"
	"    WRITE_REG(%u, (uint32_t)rslt64);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>0)&15, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_smlal_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint64_t rslt64;\n");

	buf += sprintf(buf, "\n"
	"	rslt64 = ((uint64_t)READ_REG(%u)<<32) + (uint64_t)READ_REG(%u);\n"
	"    rslt64 = (int64_t)READ_REG(%u) * (int64_t)READ_REG(%u)+rslt64;\n"
	"    WRITE_REG(%u, (uint32_t)(rslt64>>32));\n"
	"    WRITE_REG(%u, (uint32_t)rslt64);\n"
	"	ASGN_NZ((uint32_t)rslt64);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>0)&15, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_umull_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint64_t rslt64;\n");

	buf += sprintf(buf, "\n"
	"    rslt64 = (uint64_t)READ_REG(%u) * (uint64_t)READ_REG(%u);\n"
	"    WRITE_REG(%u, (uint32_t)(rslt64>>32));\n"
	"    WRITE_REG(%u, (uint32_t)rslt64);\n"
	"", (inst>>0)&15, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_umull_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint64_t rslt64;\n");

	buf += sprintf(buf, "\n"
	"    rslt64 = (uint64_t)READ_REG(%u) * (uint64_t)READ_REG(%u);\n"
	"    WRITE_REG(%u, (uint32_t)(rslt64>>32));\n"
	"    WRITE_REG(%u, (uint32_t)rslt64);\n"
	"	ASGN_NZ((uint32_t)rslt64);\n"
	"", (inst>>0)&15, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_umlal_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint64_t rslt64;\n");

	buf += sprintf(buf, "\n"
	"	rslt64 = ((uint64_t)READ_REG(%u)<<32) + (uint64_t)READ_REG(%u);\n"
	"    rslt64 = (uint64_t)READ_REG(%u) * (uint64_t)READ_REG(%u) + rslt64;\n"
	"    WRITE_REG(%u, (uint32_t)(rslt64>>32));\n"
	"    WRITE_REG(%u, (uint32_t)rslt64);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>0)&15, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_umlal_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint64_t rslt64;\n");

	buf += sprintf(buf, "\n"
	"	rslt64 = ((uint64_t)READ_REG(%u)<<32) + (uint64_t)READ_REG(%u);\n"
	"    rslt64 = (uint64_t)READ_REG(%u) * (uint64_t)READ_REG(%u) + rslt64;\n"
	"    WRITE_REG(%u, (uint32_t)(rslt64>>32));\n"
	"    WRITE_REG(%u, (uint32_t)rslt64);\n"
	"	ASGN_NZ((uint32_t)rslt64);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>0)&15, (inst>>8)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_imm_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"	\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+%u:READ_REG(%u)-%u);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_imm_b_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"			\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+%u:READ_REG(%u)-%u);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)<<%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = 0;\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_b_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)<<%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_b_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = 0;\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_b_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_b_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_b_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_b_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldt_reg_b_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_imm_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+%u:READ_REG(%u)-%u);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_imm_p_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = %u?READ_REG(%u)+%u:READ_REG(%u)-%u;\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_imm_b_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+%u:READ_REG(%u)-%u);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_imm_pb_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = %u?READ_REG(%u)+%u:READ_REG(%u)-%u;\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)<<%u);\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = 0;\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_p_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)<<%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_p_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = 0;\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_p_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)>>%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_p_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_p_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_p_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_p_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	/*if (address&0x3)\n"
	"		rslt32 = rotate_right(rslt32, (address&0x3)<<3);*/\n"
	"\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt32);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_b_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)<<%u);\n"
	"	address = READ_REG(%u);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_b_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = 0;\n"
	"	address = READ_REG(%u);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_b_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_b_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = READ_REG(%u);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_b_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_b_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = READ_REG(%u);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_b_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = READ_REG(%u);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_pb_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)<<%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_pb_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = 0;\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_pb_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (READ_REG(%u)>>%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_pb_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_pb_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_pb_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld1_reg_pb_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt8);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_imm_h_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint16_t rslt16;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (%u << 4) | %u;\n"
	"\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_READ_HALF_WORD(address,&rslt16);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt16);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_imm_ph_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint16_t rslt16;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (%u << 4) | %u;\n"
	"\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_HALF_WORD(address,&rslt16);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"			WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt16);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_imm_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (%u << 4) | %u;\n"
	"\n"
	"	address = READ_REG(%u);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, (int8_t)rslt8);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_imm_ps_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (%u << 4) | %u;\n"
	"\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"\n"
	"	WRITE_REG(%u, (int8_t)rslt8);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_imm_hs_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint16_t rslt16;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (%u << 4) | %u;\n"
	"\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_READ_HALF_WORD(address,&rslt16);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, (int16_t)rslt16);\n"
	"\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_imm_phs_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint16_t rslt16;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (%u << 4) | %u;\n"
	"\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_HALF_WORD(address,&rslt16);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, (int16_t)rslt16);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_reg_h_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint16_t rslt16;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = READ_REG(%u);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_READ_HALF_WORD(address,&rslt16);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, rslt16);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_reg_ph_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint16_t rslt16;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = READ_REG(%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"\n"
	"	MMU_READ_HALF_WORD(address,&rslt16);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, rslt16);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_reg_s_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = READ_REG(%u);\n"
	"	address = READ_REG(%u);\n"
	"	\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, (int8_t)rslt8);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_reg_ps_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = READ_REG(%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, (int8_t)rslt8);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_reg_hs_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint16_t rslt16;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = READ_REG(%u);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_READ_HALF_WORD(address,&rslt16);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	WRITE_REG(%u, (int16_t)rslt16);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ld2_reg_phs_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint16_t rslt16;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = READ_REG(%u);\n"
	"\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_READ_HALF_WORD(address,&rslt16);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"	WRITE_REG(%u, (int16_t)rslt16);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>21)&1, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_stt_imm_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+%u:READ_REG(%u)-%u);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095);
	return buf;

}

char *emitc_stt_imm_b_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"	\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+%u:READ_REG(%u)-%u);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095);
	return buf;

}

char *emitc_stt_reg_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)<<%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = 0;\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_b_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)<<%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_b_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = 0;\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_b_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_b_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_b_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_b_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_stt_reg_b_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = READ_REG(%u);\n"
	"	current_mode = TRANSLATION_MODE(USR_MODE);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	TRANSLATION_MODE(current_mode);\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_imm_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = READ_REG(%u);\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+%u:READ_REG(%u)-%u);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095);
	return buf;

}

char *emitc_st1_imm_p_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = %u?READ_REG(%u)+%u:READ_REG(%u)-%u;\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_imm_b_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = READ_REG(%u);\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+%u:READ_REG(%u)-%u);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095);
	return buf;

}

char *emitc_st1_imm_pb_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = %u?READ_REG(%u)+%u:READ_REG(%u)-%u;\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>23)&1, (inst>>16)&15, (inst>>0)&4095, (inst>>16)&15, (inst>>0)&4095, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)<<%u);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = 0;\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)>>%u);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_p_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)<<%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_p_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = 0;\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_p_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)>>%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_p_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_p_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_p_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_p_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_b_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)<<%u);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_b_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = 0;\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_b_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)>>%u);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_b_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_b_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_b_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_b_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_pb_lsl_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)<<%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_pb_zero_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = 0;\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_pb_lsr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (READ_REG(%u)>>%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_pb_sign_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (BIT31(READ_REG(%u))?~0:0);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_pb_asr_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((int32_t)(READ_REG(%u))>>%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_pb_rrx_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = ((READ_REG(%u)>>1)|(C_FLAG<<31));\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st1_reg_pb_ror_mode2_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"    offset = (val32=READ_REG(%u), (val32>>%u)|(val32<<(32-%u)));\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_BYTE(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if(%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>7)&31, (inst>>7)&31, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st2_imm_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (%u << 4) | %u;\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_HALF_WORD(address, READ_REG(%u));\n"
	"	\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st2_imm_p_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = (%u << 4) | %u;\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_HALF_WORD(address, READ_REG(%u));\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>8)&15, (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_st2_reg_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = READ_REG(%u);\n"
	"	address = READ_REG(%u);\n"
	"\n"
	"	MMU_WRITE_HALF_WORD(address, READ_REG(%u));\n"
	"	\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_st2_reg_p_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	offset = READ_REG(%u);\n"
	"	address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"	MMU_WRITE_HALF_WORD(address, READ_REG(%u));\n"
	"\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	if (%u)\n"
	"		WRITE_REG(%u, address);\n"
	"", (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_swap_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u,rslt32);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15);
	return buf;

}

char *emitc_swap_byte_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint8_t rslt8;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	address = READ_REG(%u);\n"
	"	MMU_READ_BYTE(address,&rslt8);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	MMU_WRITE_BYTE(address, (uint8_t)READ_REG(%u));\n"
	"	if (fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	WRITE_REG(%u, (uint32_t)rslt8);\n"
	"", (inst>>16)&15, (inst>>0)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldm1_ldm1_000_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr - pcount + 4;\n"
	"\n"
	"	\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address, &val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_ldm1_ldm1_001_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr - pcount + 4;\n"
	"	WRITE_REG(%u, base_addr - pcount);\n"
	"\n"
	"	\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address, &val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_ldm1_ldm1_010_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr;\n"
	"\n"
	"	\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address, &val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_ldm1_ldm1_011_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr;\n"
	"	WRITE_REG(%u, base_addr + pcount);\n"
	"\n"
	"	\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address, &val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_ldm1_ldm1_100_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr - pcount;\n"
	"\n"
	"	\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address, &val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_ldm1_ldm1_101_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr - pcount;\n"
	"	WRITE_REG(%u, base_addr - pcount);\n"
	"\n"
	"	\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address, &val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_ldm1_ldm1_110_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr + 4;\n"
	"\n"
	"	\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address, &val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_ldm1_ldm1_111_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr + 4;\n"
	"	WRITE_REG(%u, base_addr + pcount);\n"
	"\n"
	"	\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address, &val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_ldm2_ldm2_000_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr - pcount + 4;\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	current_mode = CPU_MODE;\n"
	"	SWITCH_BANK(current_mode,USR_MODE);	\n"
	"\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				SWITCH_BANK(USR_MODE,current_mode);\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	SWITCH_BANK(USR_MODE,current_mode);	\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst);
	return buf;

}

char *emitc_ldm2_ldm2_010_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr;\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	current_mode = CPU_MODE;\n"
	"	SWITCH_BANK(current_mode,USR_MODE);	\n"
	"\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				SWITCH_BANK(USR_MODE,current_mode);\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	SWITCH_BANK(USR_MODE,current_mode);	\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst);
	return buf;

}

char *emitc_ldm2_ldm2_100_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr - pcount;\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	current_mode = CPU_MODE;\n"
	"	SWITCH_BANK(current_mode,USR_MODE);	\n"
	"\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				SWITCH_BANK(USR_MODE,current_mode);\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	SWITCH_BANK(USR_MODE,current_mode);	\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst);
	return buf;

}

char *emitc_ldm2_ldm2_110_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr + 4;\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	current_mode = CPU_MODE;\n"
	"	SWITCH_BANK(current_mode,USR_MODE);	\n"
	"\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				SWITCH_BANK(USR_MODE,current_mode);\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	SWITCH_BANK(USR_MODE,current_mode);	\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst);
	return buf;

}

char *emitc_ldm3_ldm3_000_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"\n"
	"	pcount = popcount16(%u)*4 + 4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr - pcount + 4;\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	WRITE_CPSR(SPSR);\n"
	"	MMU_READ_WORD(address,&val32);\n"
	"	if(fault){\n"
	"		WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	WRITE_REG(15, val32);\n"
	"\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst, inst);
	return buf;

}

char *emitc_ldm3_ldm3_001_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"\n"
	"	pcount = popcount16(%u)*4 + 4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr - pcount + 4;\n"
	"	WRITE_REG(%u, base_addr - pcount);\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	WRITE_CPSR(SPSR);\n"
	"	MMU_READ_WORD(address,&val32);\n"
	"	if(fault){\n"
	"		WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	WRITE_REG(15, val32);\n"
	"\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst, inst);
	return buf;

}

char *emitc_ldm3_ldm3_010_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"\n"
	"	pcount = popcount16(%u)*4 + 4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr;\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	WRITE_CPSR(SPSR);\n"
	"	MMU_READ_WORD(address,&val32);\n"
	"	if(fault){\n"
	"		WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	WRITE_REG(15, val32);\n"
	"\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst, inst);
	return buf;

}

char *emitc_ldm3_ldm3_011_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"\n"
	"	pcount = popcount16(%u)*4 + 4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr;\n"
	"	WRITE_REG(%u, base_addr + pcount);\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	WRITE_CPSR(SPSR);\n"
	"	MMU_READ_WORD(address,&val32);\n"
	"	if(fault){\n"
	"		WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	WRITE_REG(15, val32);\n"
	"\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst, inst);
	return buf;

}

char *emitc_ldm3_ldm3_100_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"\n"
	"	pcount = popcount16(%u)*4 + 4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr - pcount;\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	WRITE_CPSR(SPSR);\n"
	"	MMU_READ_WORD(address,&val32);\n"
	"	if(fault){\n"
	"		WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	WRITE_REG(15, val32);\n"
	"\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst, inst);
	return buf;

}

char *emitc_ldm3_ldm3_101_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"\n"
	"	pcount = popcount16(%u)*4 + 4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr - pcount;\n"
	"	WRITE_REG(%u, base_addr - pcount);\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	WRITE_CPSR(SPSR);\n"
	"	MMU_READ_WORD(address,&val32);\n"
	"	if(fault){\n"
	"		WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	WRITE_REG(15, val32);\n"
	"\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst, inst);
	return buf;

}

char *emitc_ldm3_ldm3_110_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"\n"
	"	pcount = popcount16(%u)*4 + 4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr + 4;\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	WRITE_CPSR(SPSR);\n"
	"	MMU_READ_WORD(address,&val32);\n"
	"	if(fault){\n"
	"		WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	WRITE_REG(15, val32);\n"
	"\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst, inst);
	return buf;

}

char *emitc_ldm3_ldm3_111_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"\n"
	"	pcount = popcount16(%u)*4 + 4;\n"
	"	\n"
	"	base_addr = READ_REG(%u);	\n"
	"	start_addr = base_addr + 4;\n"
	"	WRITE_REG(%u, base_addr + pcount);\n"
	"\n"
	"\n"
	"	address = start_addr-(start_addr&0x3);\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<15) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_READ_WORD(address,&val32);\n"
	"			if(fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			WRITE_REG(iterator, val32);\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"	WRITE_CPSR(SPSR);\n"
	"	MMU_READ_WORD(address,&val32);\n"
	"	if(fault){\n"
	"		WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	WRITE_REG(15, val32);\n"
	"\n"
	"\n"
	"", (inst>>0)&32767, (inst>>16)&15, (inst>>16)&15, (inst>>0)&32767, (inst>>0)&32767, inst, inst);
	return buf;

}

char *emitc_stm1_stm1_000_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr - pcount + 4;\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm1_stm1_001_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr - pcount + 4;\n"
	"	WRITE_REG(%u, base_addr - pcount);\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm1_stm1_010_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr;\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm1_stm1_011_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr;\n"
	"	WRITE_REG(%u, base_addr + pcount);\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm1_stm1_100_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr - pcount;\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm1_stm1_101_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr - pcount;\n"
	"	WRITE_REG(%u, base_addr - pcount);\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm1_stm1_110_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr + 4;\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm1_stm1_111_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr + 4;\n"
	"	WRITE_REG(%u, base_addr + pcount);\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm2_stm2_000_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr - pcount + 4;\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"	current_mode = CPU_MODE;\n"
	"	SWITCH_BANK(current_mode,USR_MODE);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				SWITCH_BANK(USR_MODE,current_mode);\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"	SWITCH_BANK(USR_MODE,current_mode);\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm2_stm2_010_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr;\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"	current_mode = CPU_MODE;\n"
	"	SWITCH_BANK(current_mode,USR_MODE);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				SWITCH_BANK(USR_MODE,current_mode);\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"	SWITCH_BANK(USR_MODE,current_mode);\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm2_stm2_100_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr - pcount;\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"	current_mode = CPU_MODE;\n"
	"	SWITCH_BANK(current_mode,USR_MODE);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				SWITCH_BANK(USR_MODE,current_mode);\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"	SWITCH_BANK(USR_MODE,current_mode);\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_stm2_stm2_110_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tuint32_t pcount;\n");
	buf += sprintf(buf, "\tuint32_t iterator;\n");
	buf += sprintf(buf, "\tcpu_mode_t current_mode;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	pcount = popcount16(%u)*4;\n"
	"\n"
	"	\n"
	"	base_addr = READ_REG(%u);\n"
	"	start_addr = base_addr + 4;\n"
	"\n"
	"\n"
	"	address = start_addr - (start_addr&0x3);\n"
	"	current_mode = CPU_MODE;\n"
	"	SWITCH_BANK(current_mode,USR_MODE);\n"
	"\n"
	"	/* This is not perfect exception here since\n"
	"	   write may have been committed before exception occurs. */\n"
	"	iterator = rmo16(%u);\n"
	"	while (iterator<16) {\n"
	"		if (%u&(1<<iterator)) {\n"
	"			MMU_WRITE_WORD(address, READ_REG(iterator));\n"
	"			if (fault){\n"
	"				SWITCH_BANK(USR_MODE,current_mode);\n"
	"				WRITE_REG((%uU>>16)&0xF, base_addr);\n"
	"				XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN,address);\n"
	"				ABORT( DataAbortV );\n"
	"			}\n"
	"			address += 4;\n"
	"		}\n"
	"		iterator++;\n"
	"	}\n"
	"	SWITCH_BANK(USR_MODE,current_mode);\n"
	"\n"
	"", (inst>>0)&65535, (inst>>16)&15, (inst>>0)&65535, (inst>>0)&65535, inst);
	return buf;

}

char *emitc_branch_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	EMIT_DELAY_SLOT;
	target_addr_t jpc = CURPC+((int32_t)(((inst>>0)&16777215)<<8)>>6);
	if (jpc >= size)
		buf += sprintf(buf, "\tLONG_JUMP(%uU, %uU);\n", curpc, jpc);
	else
		buf += sprintf(buf, "\tJUMP(%uU, %uU, L%x);\n", curpc, jpc, jpc);
	return buf;

}

char *emitc_branch_exchange_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	if (READ_REG(%u) & 1){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	WRITE_REG(15, READ_REG(%u) & 0xFFFFFFFC);\n"
	"", (inst>>0)&15, (inst>>0)&15);
	return buf;

}

char *emitc_branch_link_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	WRITE_REG(LRIND, %s-4);\n"
	"", "PC");
	EMIT_DELAY_SLOT;
	target_addr_t jpc = CURPC+((int32_t)(((inst>>0)&16777215)<<8)>>6);
	if (jpc >= size)
		buf += sprintf(buf, "\tLONG_JUMP(%uU, %uU);\n", curpc, jpc);
	else
		buf += sprintf(buf, "\tJUMP(%uU, %uU, L%x);\n", curpc, jpc, jpc);
	return buf;

}

char *emitc_mrs_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"    WRITE_REG(%u, CPSR);\n"
	"", (inst>>12)&15);
	return buf;

}

char *emitc_mrs_r_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"    WRITE_REG(%u, SPSR);\n"
	"", (inst>>12)&15);
	return buf;

}

char *emitc_msri_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"    rslt32 = %u;\n"
	"    UPDATE_CPSR(rslt32, %u);\n"
	"", (inst>>0)&255, (inst>>16)&15);
	return buf;

}

char *emitc_msri_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"    rslt32 = ((%u>>(%u<<1))|(%u<<(32-(%u<<1))));\n"
	"    UPDATE_CPSR(rslt32, %u);\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15);
	return buf;

}

char *emitc_msri_r_imm_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"    rslt32 = %u;\n"
	"    UPDATE_SPSR(rslt32, %u);\n"
	"", (inst>>0)&255, (inst>>16)&15);
	return buf;

}

char *emitc_msri_r_rot_mode1_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"    rslt32 = ((%u>>(%u<<1))|(%u<<(32-(%u<<1))));\n"
	"    UPDATE_SPSR(rslt32, %u);\n"
	"", (inst>>0)&255, (inst>>8)&15, (inst>>0)&255, (inst>>8)&15, (inst>>16)&15);
	return buf;

}

char *emitc_msr_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"    UPDATE_CPSR(READ_REG(%u), %u);\n"
	"", (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_msr_r_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"    UPDATE_SPSR(READ_REG(%u), %u);\n"
	"", (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_sc_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	if (%uU == ABORTWORD && emu->abort_addr == (%s-8) ) {\n"
	"		/* A prefetch abort.  */\n"
	"		XSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_MMU_EXCPT, %s-8);\n"
	"		ABORT2(PrefetchAbortV);\n"
	"	}\n"
	"\n"
	"	DO_SWI(%uU);\n"
	"", inst, "PC", "PC", inst);
	return buf;

}

char *emitc_cdp_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"		\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	\n"
	"	cpab = emu->copro[%u]->CDP(ARMul_FIRST, %uU);\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->CDP(ARMul_INTERRUPT, %uU);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->CDP(ARMul_BUSY, %uU);\n"
	"	}\n"
	"\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"", (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst);
	return buf;

}

char *emitc_mcr_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->MCR(ARMul_FIRST, %uU, READ_REG(%u));\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->MCR(ARMul_INTERRUPT, %uU, READ_REG(%u));\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->MCR(ARMul_BUSY, %uU, READ_REG(%u));\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"", (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>12)&15, (inst>>8)&15, inst, (inst>>12)&15, (inst>>8)&15, inst, (inst>>12)&15);
	return buf;

}

char *emitc_mrc_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "	\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->MRC(ARMul_FIRST, %uU, &val32);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->MRC(ARMul_INTERRUPT, %uU, &val32);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->MRC(ARMul_BUSY, %uU, &val32);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	if (%u  == 15) {\n"
	"		ASGN_N ((val32 & (1<<31)) != 0);\n"
	"		ASGN_Z ((val32 & (1<<30)) != 0);\n"
	"		ASGN_C ((val32 & (1<<29)) != 0);\n"
	"		ASGN_V ((val32 & (1<<28)) != 0);\n"
	"	}\n"
	"	else{\n"
	"		WRITE_REG(%u,val32);\n"
	"	}\n"
	"", (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldc1_ldc1_01_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	\n"
	"	address = READ_REG(%u);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_FIRST, %uU, 0);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_BUSY, %uU, 0);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_TRANSFER, %uU, 0);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst);
	return buf;

}

char *emitc_ldc1_ldc1_10_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	\n"
	"	address = READ_REG(%u) - (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_FIRST, %uU, 0);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_BUSY, %uU, 0);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_TRANSFER, %uU, 0);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst);
	return buf;

}

char *emitc_ldc1_ldc1_11_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	\n"
	"	address = READ_REG(%u) + (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_FIRST, %uU, 0);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_BUSY, %uU, 0);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_TRANSFER, %uU, 0);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst);
	return buf;

}

char *emitc_ldc2_ldc2_00_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	base_addr =  READ_REG(%u);\n"
	"	\n"
	"	start_addr = base_addr;\n"
	"	address = base_addr - (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_FIRST, %uU, 0);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_BUSY, %uU, 0);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	WRITE_REG(%u, address);\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_TRANSFER, %uU, 0);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if(fault){\n"
	"		WRITE_REG(%u, base_addr);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	\n"
	"	address = start_addr;\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			WRITE_REG(%u, base_addr);\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst);
	return buf;

}

char *emitc_ldc2_ldc2_01_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	base_addr =  READ_REG(%u);\n"
	"	\n"
	"	start_addr = base_addr;\n"
	"	address = base_addr + (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_FIRST, %uU, 0);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_BUSY, %uU, 0);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	WRITE_REG(%u, address);\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_TRANSFER, %uU, 0);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if(fault){\n"
	"		WRITE_REG(%u, base_addr);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	\n"
	"	address = start_addr;\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			WRITE_REG(%u, base_addr);\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst);
	return buf;

}

char *emitc_ldc2_ldc2_10_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	base_addr =  READ_REG(%u);\n"
	"	\n"
	"	start_addr = address = base_addr - (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_FIRST, %uU, 0);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_BUSY, %uU, 0);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	WRITE_REG(%u, address);\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_TRANSFER, %uU, 0);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if(fault){\n"
	"		WRITE_REG(%u, base_addr);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	\n"
	"	address = start_addr;\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			WRITE_REG(%u, base_addr);\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst);
	return buf;

}

char *emitc_ldc2_ldc2_11_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	base_addr =  READ_REG(%u);\n"
	"	\n"
	"	start_addr = address = base_addr + (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_FIRST, %uU, 0);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->LDC(ARMul_BUSY, %uU, 0);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	WRITE_REG(%u, address);\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_TRANSFER, %uU, 0);\n"
	"	MMU_READ_WORD(address,&rslt32);\n"
	"	if(fault){\n"
	"		WRITE_REG(%u, base_addr);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	\n"
	"	address = start_addr;\n"
	"	cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			WRITE_REG(%u, base_addr);\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		cpab = emu->copro[%u]->LDC(ARMul_DATA, %uU, rslt32);\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst);
	return buf;

}

char *emitc_stc1_stc1_01_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	\n"
	"	address = READ_REG(%u);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->STC(ARMul_FIRST, %uU, &rslt32);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->STC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->STC(ARMul_BUSY, %uU, &rslt32);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"	MMU_WRITE_WORD(address,rslt32);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"		MMU_WRITE_WORD(address,rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst);
	return buf;

}

char *emitc_stc1_stc1_10_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	\n"
	"	address = READ_REG(%u) - (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->STC(ARMul_FIRST, %uU, &rslt32);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->STC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->STC(ARMul_BUSY, %uU, &rslt32);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"	MMU_WRITE_WORD(address,rslt32);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"		MMU_WRITE_WORD(address,rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst);
	return buf;

}

char *emitc_stc1_stc1_11_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	\n"
	"	address = READ_REG(%u) + (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->STC(ARMul_FIRST, %uU, &rslt32);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->STC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->STC(ARMul_BUSY, %uU, &rslt32);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"	MMU_WRITE_WORD(address,rslt32);\n"
	"	if(fault){\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"		MMU_WRITE_WORD(address,rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst);
	return buf;

}

char *emitc_stc2_stc2_00_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	base_addr =  READ_REG(%u);\n"
	"	\n"
	"	start_addr = base_addr;\n"
	"	address = base_addr - (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->STC(ARMul_FIRST, %uU, &rslt32);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->STC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->STC(ARMul_BUSY, %uU, &rslt32);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	WRITE_REG(%u, address);\n"
	"\n"
	"	address = start_addr;\n"
	"	cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"	MMU_WRITE_WORD(address,rslt32);\n"
	"	if(fault){\n"
	"		WRITE_REG(%u, base_addr);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"		MMU_WRITE_WORD(address,rslt32);\n"
	"		if(fault){\n"
	"			WRITE_REG(%u, base_addr);\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15);
	return buf;

}

char *emitc_stc2_stc2_01_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	base_addr =  READ_REG(%u);\n"
	"	\n"
	"	start_addr = base_addr;\n"
	"	address = base_addr + (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->STC(ARMul_FIRST, %uU, &rslt32);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->STC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->STC(ARMul_BUSY, %uU, &rslt32);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	WRITE_REG(%u, address);\n"
	"\n"
	"	address = start_addr;\n"
	"	cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"	MMU_WRITE_WORD(address,rslt32);\n"
	"	if(fault){\n"
	"		WRITE_REG(%u, base_addr);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"		MMU_WRITE_WORD(address,rslt32);\n"
	"		if(fault){\n"
	"			WRITE_REG(%u, base_addr);\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15);
	return buf;

}

char *emitc_stc2_stc2_10_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	base_addr =  READ_REG(%u);\n"
	"	\n"
	"	start_addr = address = base_addr - (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->STC(ARMul_FIRST, %uU, &rslt32);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->STC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->STC(ARMul_BUSY, %uU, &rslt32);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	WRITE_REG(%u, address);\n"
	"\n"
	"	address = start_addr;\n"
	"	cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"	MMU_WRITE_WORD(address,rslt32);\n"
	"	if(fault){\n"
	"		WRITE_REG(%u, base_addr);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"		MMU_WRITE_WORD(address,rslt32);\n"
	"		if(fault){\n"
	"			WRITE_REG(%u, base_addr);\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15);
	return buf;

}

char *emitc_stc2_stc2_11_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tuint32_t start_addr;\n");
	buf += sprintf(buf, "\tuint32_t base_addr;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");
	buf += sprintf(buf, "\tuint32_t cpab;\n");

	buf += sprintf(buf, "\n"
	"	offset = %u;\n"
	"	base_addr =  READ_REG(%u);\n"
	"	\n"
	"	start_addr = address = base_addr + (offset << 2);\n"
	";\n"
	"\n"
	"	if (emu->copro[%u] == NULL){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"\n"
	"	if (!CP_ACCESS_ALLOWED (%u)) {\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	cpab = emu->copro[%u]->STC(ARMul_FIRST, %uU, &rslt32);\n"
	"\n"
	"	while (cpab == ARMul_BUSY) {\n"
	"		if (emu->int_pending ()) {\n"
	"			cpab = emu->copro[%u]->STC(ARMul_INTERRUPT, %uU, 0);\n"
	"		}\n"
	"		else\n"
	"			cpab = emu->copro[%u]->STC(ARMul_BUSY, %uU, &rslt32);\n"
	"	}\n"
	"\n"
	"	if (cpab == ARMul_CANT){\n"
	"		ABORT2(UndefinedInstrV);\n"
	"	}\n"
	"	\n"
	"	WRITE_REG(%u, address);\n"
	"\n"
	"	address = start_addr;\n"
	"	cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"	MMU_WRITE_WORD(address,rslt32);\n"
	"	if(fault){\n"
	"		WRITE_REG(%u, base_addr);\n"
	"		ABORT( DataAbortV );\n"
	"	}\n"
	"	while (cpab == ARMul_INC) {\n"
	"		address += 4;\n"
	"		cpab = emu->copro[%u]->STC(ARMul_DATA, %uU, &rslt32);\n"
	"		MMU_WRITE_WORD(address,rslt32);\n"
	"		if(fault){\n"
	"			WRITE_REG(%u, base_addr);\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"	}\n"
	"", (inst>>0)&255, (inst>>16)&15, (inst>>8)&15, (inst>>8)&15, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15, (inst>>8)&15, inst, (inst>>16)&15);
	return buf;

}

char *emitc_mra_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale()) {\n"
	"	/* XScale MRA insn.  Move accumulator into two registers.  */\n"
	"		val32 = (emu->accumulator >> 32) & 255;\n"
	"		if (val32 & 128)\n"
	"			val32 -= 256;\n"
	"		\n"
	"		WRITE_REG(%u,emu->accumulator);\n"
	"		WRITE_REG(%u,val32);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_mar_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale()) {\n"
	"	/* XScale MAR insn.  Move two registers into accumulator.  */\n"
	"		emu->accumulator = READ_REG(%u);\n"
	"		emu->accumulator += (uint64_t)READ_REG(%u)<<32;\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>16)&15);
	return buf;

}

char *emitc_clz_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	\n"
	"	if (emu->is_xscale()) {\n"
	"		val32=READ_REG(%u);\n"
	"		if(val32==0){\n"
	"			WRITE_REG(%u,32);\n"
	"		}\n"
	"		else{\n"
	"			for(rslt32=0;(val32 & 0x80000000) == 0; val32 <<=1)\n"
	"				rslt32++;\n"
	"			WRITE_REG(%u,rslt32);\n"
	"		}\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>0)&15, (inst>>12)&15, (inst>>12)&15);
	return buf;

}

char *emitc_ldd_imm_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"		if(%u & 1)\n"
	"			ABORT2(UndefinedInstrV);\n"
	"		\n"
	"		offset = (%u << 4) | %u;\n"
	"\n"
	"		address = READ_REG(%u);\n"
	"	\n"
	"		fault = NO_FAULT;\n"
	"\n"
	"		if(address & 7)/* Alignment violation.  */\n"
	"			ABORT( DataAbortV );\n"
	"\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		WRITE_REG(%u, rslt32);\n"
	"		\n"
	"		MMU_READ_WORD(address+4,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		WRITE_REG(%u+1, rslt32);\n"
	"		\n"
	"		WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_ldd_imm_p_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"		if(%u & 1)\n"
	"			ABORT2(UndefinedInstrV);\n"
	"		\n"
	"		offset = (%u << 4) | %u;\n"
	"	\n"
	"		address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"		\n"
	"		fault = NO_FAULT;\n"
	"\n"
	"		if(address & 7)/* Alignment violation.  */\n"
	"			ABORT( DataAbortV );\n"
	"\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		WRITE_REG(%u, rslt32);\n"
	"		\n"
	"		MMU_READ_WORD(address+4,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		WRITE_REG(%u+1, rslt32);\n"
	"\n"
	"		if (%u)\n"
	"			WRITE_REG(%u, address);\n"
	"	\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_ldd_reg_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"		if(%u & 1)\n"
	"			ABORT2(UndefinedInstrV);\n"
	"\n"
	"		offset = READ_REG(%u);\n"
	"		address = READ_REG(%u);\n"
	"	\n"
	"		fault = NO_FAULT;\n"
	"\n"
	"		if(address & 7)/* Alignment violation.  */\n"
	"			ABORT( DataAbortV );\n"
	"\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		WRITE_REG(%u, rslt32);\n"
	"		\n"
	"		MMU_READ_WORD(address+4,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		WRITE_REG(%u+1, rslt32);\n"
	"\n"
	"		WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_ldd_reg_p_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t rslt32;\n");
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"		if(%u & 1)\n"
	"			ABORT2(UndefinedInstrV);\n"
	"	\n"
	"		offset = READ_REG(%u);\n"
	"		address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"		fault = NO_FAULT;\n"
	"\n"
	"		if(address & 7)/* Alignment violation.  */\n"
	"			ABORT( DataAbortV );\n"
	"\n"
	"		MMU_READ_WORD(address,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		WRITE_REG(%u, rslt32);\n"
	"		\n"
	"		MMU_READ_WORD(address+4,&rslt32);\n"
	"		if(fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		WRITE_REG(%u+1, rslt32);\n"
	"\n"
	"		if (%u)\n"
	"			WRITE_REG(%u, address);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_std_imm_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"		if(%u & 1)\n"
	"			ABORT2(UndefinedInstrV);\n"
	"	\n"
	"		offset = (%u << 4) | %u;\n"
	"		address = READ_REG(%u);\n"
	"\n"
	"		fault = NO_FAULT;\n"
	"\n"
	"		if(address & 7)/* Alignment violation.  */\n"
	"			ABORT( DataAbortV );\n"
	"\n"
	"		MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"		if (fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		MMU_WRITE_WORD(address+4, READ_REG(%u+1));\n"
	"		if (fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"\n"
	"		WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_std_imm_p_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"		if(%u & 1)\n"
	"			ABORT2(UndefinedInstrV);\n"
	"	\n"
	"		offset = (%u << 4) | %u;\n"
	"		address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"		fault = NO_FAULT;\n"
	"\n"
	"		if(address & 7)/* Alignment violation.  */\n"
	"			ABORT( DataAbortV );\n"
	"\n"
	"		MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"		if (fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		MMU_WRITE_WORD(address+4, READ_REG(%u+1));\n"
	"		if (fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"\n"
	"		if (%u)\n"
	"			WRITE_REG(%u, address);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_std_reg_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"		if(%u & 1)\n"
	"			ABORT2(UndefinedInstrV);\n"
	"	\n"
	"		offset = READ_REG(%u);\n"
	"		address = READ_REG(%u);\n"
	"\n"
	"		fault = NO_FAULT;\n"
	"\n"
	"		if(address & 7)/* Alignment violation.  */\n"
	"			ABORT( DataAbortV );\n"
	"\n"
	"		MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"		if (fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		MMU_WRITE_WORD(address+4, READ_REG(%u+1));\n"
	"		if (fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"\n"
	"		WRITE_REG(%u, %u?READ_REG(%u)+offset:READ_REG(%u)-offset);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15, (inst>>16)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15);
	return buf;

}

char *emitc_std_reg_p_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t address;\n");
	buf += sprintf(buf, "\tuint32_t offset;\n");
	buf += sprintf(buf, "\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"		if(%u & 1)\n"
	"			ABORT2(UndefinedInstrV);\n"
	"\n"
	"		offset = READ_REG(%u);\n"
	"		address = %u?READ_REG(%u)+offset:READ_REG(%u)-offset;\n"
	"\n"
	"		fault = NO_FAULT;\n"
	"\n"
	"		if(address & 7)/* Alignment violation.  */\n"
	"			ABORT( DataAbortV );\n"
	"\n"
	"		MMU_WRITE_WORD(address, READ_REG(%u));\n"
	"		if (fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"		MMU_WRITE_WORD(address+4, READ_REG(%u+1));\n"
	"		if (fault){\n"
	"			ABORT( DataAbortV );\n"
	"		}\n"
	"\n"
	"		if (%u)\n"
	"			WRITE_REG(%u, address);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>23)&1, (inst>>16)&15, (inst>>16)&15, (inst>>12)&15, (inst>>12)&15, (inst>>21)&1, (inst>>16)&15);
	return buf;

}

char *emitc_smla_xy_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t op1;\n");
	buf += sprintf(buf, "\tuint32_t op2;\n");
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"	/*  SMLAxy insn.  */\n"
	"		\n"
	"		val32 = READ_REG(%u);\n"
	"		\n"
	"		op1 = READ_REG(%u); \n"
	"		op2 = READ_REG(%u);\n"
	"\n"
	"		op1 = %u ? op1>>16 : op1&0xFFFF;\n"
	"		op2 = %u ? op2>>16 : op2&0xFFFF;\n"
	"						\n"
	"		if (op1 & 0x8000)\n"
	"			op1 -= 65536;\n"
	"		if (op2 & 0x8000)\n"
	"			op2 -= 65536;\n"
	"		\n"
	"		tmp32 = op1 * op2;\n"
	"		rslt32 = val32 + tmp32;\n"
	"		\n"
	"		WRITE_REG(%u, rslt32);\n"
	"\n"
	"		ASGN_Q((val32^tmp32^-1) & (val32^rslt32));\n"
	"\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>0)&15, (inst>>8)&15, (inst>>5)&1, (inst>>6)&1, (inst>>16)&15);
	return buf;

}

char *emitc_smlal_xy_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t op1;\n");
	buf += sprintf(buf, "\tuint32_t op2;\n");
	buf += sprintf(buf, "\tuint64_t rslt64;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"	/*  SMLALxy insn.  */\n"
	"\n"
	"		op1 = READ_REG(%u); \n"
	"		op2 = READ_REG(%u);\n"
	"\n"
	"		op1 = %u ? op1>>16 : op1&0xFFFF;\n"
	"		op2 = %u ? op2>>16 : op2&0xFFFF;\n"
	"						\n"
	"		if (op1 & 0x8000)\n"
	"			op1 -= 65536;\n"
	"		if (op2 & 0x8000)\n"
	"			op2 -= 65536;\n"
	"\n"
	"		rslt64 = ((uint64_t)READ_REG(%u)<<32) + (uint64_t)READ_REG(%u);\n"
	"		rslt64 = (int64_t)op1 * (int64_t)op2 +rslt64;\n"
	"\n"
	"		WRITE_REG(%u, (uint32_t)(rslt64>>32));\n"
	"		WRITE_REG(%u, (uint32_t)rslt64);\n"
	"\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>0)&15, (inst>>8)&15, (inst>>5)&1, (inst>>6)&1, (inst>>16)&15, (inst>>12)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_smlaw_y_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t op2;\n");
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"	/*  SMLAWy insn.  */\n"
	"		\n"
	"		val32  =  READ_REG(%u);\n"
	"\n"
	"		op2 = READ_REG(%u);\n"
	"		op2 = %u ? op2>>16 : op2&0xFFFF;\n"
	"						\n"
	"		if (op2 & 0x8000)\n"
	"			op2 -= 65536;\n"
	"\n"
	"		tmp32  = (uint32_t)( (int64_t)READ_REG(%u)*op2  >> 16);\n"
	"		rslt32 = val32 + tmp32;\n"
	"\n"
	"		WRITE_REG(%u,rslt32);\n"
	"\n"
	"		ASGN_Q((val32^tmp32^-1) & (val32^rslt32));\n"
	"	\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>12)&15, (inst>>8)&15, (inst>>6)&1, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_smul_xy_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t op1;\n");
	buf += sprintf(buf, "\tuint32_t op2;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"	/*  SMULxy insn.  */\n"
	"\n"
	"		op1 = READ_REG(%u); \n"
	"		op2 = READ_REG(%u);\n"
	"\n"
	"		op1 = %u ? op1>>16 : op1&0xFFFF;\n"
	"		op2 = %u ? op2>>16 : op2&0xFFFF;\n"
	"\n"
	"		if (op1 & 0x8000)\n"
	"			op1 -= 65536;\n"
	"		if (op2 & 0x8000)\n"
	"			op2 -= 65536;\n"
	"\n"
	"		WRITE_REG(%u,op1 * op2);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>0)&15, (inst>>8)&15, (inst>>5)&1, (inst>>6)&1, (inst>>16)&15);
	return buf;

}

char *emitc_smulw_y_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t op2;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"	/* SMULWy insn.  */\n"
	"\n"
	"		op2 = READ_REG(%u);\n"
	"		op2 = %u ? op2>>16 : op2&0xFFFF;\n"
	"						\n"
	"		if (op2 & 0x8000)\n"
	"			op2 -= 65536;\n"
	"\n"
	"		rslt32  = (uint32_t)( (int64_t)READ_REG(%u)*op2  >> 16); \n"
	"\n"
	"		WRITE_REG(%u,rslt32);\n"
	"	\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>8)&15, (inst>>6)&1, (inst>>0)&15, (inst>>16)&15);
	return buf;

}

char *emitc_qadd_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"	/*   QADD insn.  */\n"
	"\n"
	"		val32 = READ_REG(%u); \n"
	"		tmp32 = READ_REG(%u);\n"
	"		rslt32 = val32 + tmp32;\n"
	"\n"
	"\n"
	"		if ((val32^tmp32^-1) & (val32^rslt32)){\n"
	"			rslt32 = (rslt32 & 0x80000000)? 0x7fffffff:0x80000000;\n"
	"			SET_Q;\n"
	"		}\n"
	"		\n"
	"		WRITE_REG(%u,rslt32);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_qdadd_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"	/*   QDADD insn.  */\n"
	"\n"
	"		val32 = READ_REG(%u);\n"
	"		tmp32 = READ_REG(%u); \n"
	"\n"
	"\n"
	"		if ((tmp32^tmp32^-1) & (tmp32^(tmp32<<1))){\n"
	"			tmp32 = ((tmp32<<2) & 0x80000000)? 0x7fffffff:0x80000000;\n"
	"			SET_Q;\n"
	"		}\n"
	"		else\n"
	"			tmp32<<=1;\n"
	"		\n"
	"		rslt32 = val32 + tmp32;\n"
	"		\n"
	"		if ((val32^tmp32^-1) & (val32^rslt32)){\n"
	"			rslt32 = (rslt32 & 0x80000000)? 0x7fffffff:0x80000000;\n"
	"			SET_Q;\n"
	"		}\n"
	"		\n"
	"		WRITE_REG(%u,rslt32);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_qsub_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"	/*   QSUB insn.  */\n"
	"\n"
	"		val32 = READ_REG(%u); \n"
	"		tmp32 = READ_REG(%u);\n"
	"		rslt32 = val32 - tmp32;\n"
	"\n"
	"\n"
	"		if ((val32^tmp32) & (val32^rslt32)){\n"
	"			rslt32 = (rslt32 & 0x80000000)? 0x7fffffff:0x80000000;\n"
	"			SET_Q;\n"
	"		}\n"
	"		\n"
	"		WRITE_REG(%u,rslt32);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

char *emitc_qdsub_(char *buf, unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "\tuint32_t val32;\n");
	buf += sprintf(buf, "\tuint32_t tmp32;\n");
	buf += sprintf(buf, "\tuint32_t rslt32;\n");

	buf += sprintf(buf, "\n"
	"	if (emu->is_xscale() ) {\n"
	"	/*   QDSUB insn.  */\n"
	"\n"
	"		val32 = READ_REG(%u);\n"
	"		tmp32 = READ_REG(%u); \n"
	"\n"
	"\n"
	"		if ((tmp32^tmp32^-1) & (tmp32^(tmp32<<1))){\n"
	"			tmp32 = ((tmp32<<2) & 0x80000000)? 0x7fffffff:0x80000000;\n"
	"			SET_Q;\n"
	"		}\n"
	"		else\n"
	"			tmp32<<=1;\n"
	"		\n"
	"		rslt32 = val32 - tmp32;\n"
	"		\n"
	"		if ((val32^tmp32) & (val32^rslt32)){\n"
	"			rslt32 = (rslt32 & 0x80000000)? 0x7fffffff:0x80000000;\n"
	"			SET_Q;\n"
	"		}\n"
	"		\n"
	"		WRITE_REG(%u,rslt32);\n"
	"	}\n"
	"	else\n"
	"		ABORT2(UndefinedInstrV);\n"
	"", (inst>>0)&15, (inst>>16)&15, (inst>>12)&15);
	return buf;

}

