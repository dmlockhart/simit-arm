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

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "armemul.h"

#define disasm_mov_1  disasm_unop_1		/*0x3A*/
#define disasm_mov_1s disasm_unop_1		/*0x3B*/
#define disasm_mov_2  disasm_unop_2 	/*0x3A*/
#define disasm_mov_2s disasm_unop_2		/*0x3B*/
#define disasm_mov_3  disasm_unop_3 	/*0x3A*/
#define disasm_mov_3s disasm_unop_3		/*0x3B*/

#define disasm_mvn_1  disasm_unop_1		/*0x3E*/
#define disasm_mvn_1s disasm_unop_1		/*0x3F*/
#define disasm_mvn_2  disasm_unop_2		/*0x3E*/
#define disasm_mvn_2s disasm_unop_2		/*0x3F*/
#define disasm_mvn_3  disasm_unop_3		/*0x3E*/
#define disasm_mvn_3s disasm_unop_3		/*0x3F*/

#define disasm_add_1  disasm_binop_1		/*0x28*/
#define disasm_add_1s disasm_binop_1		/*0x29*/
#define disasm_add_2  disasm_binop_2		/*0x28*/
#define disasm_add_2s disasm_binop_2		/*0x29*/
#define disasm_add_3  disasm_binop_3		/*0x28*/
#define disasm_add_3s disasm_binop_3		/*0x29*/

#define disasm_adc_1  disasm_binop_1		/*0x2A*/
#define disasm_adc_1s disasm_binop_1		/*0x2B*/
#define disasm_adc_2  disasm_binop_2		/*0x2A*/
#define disasm_adc_2s disasm_binop_2		/*0x2B*/
#define disasm_adc_3  disasm_binop_3		/*0x2A*/
#define disasm_adc_3s disasm_binop_3		/*0x2B*/

#define disasm_sub_1  disasm_binop_1		/*0x24*/
#define disasm_sub_1s disasm_binop_1		/*0x25*/
#define disasm_sub_2  disasm_binop_2		/*0x24*/
#define disasm_sub_2s disasm_binop_2		/*0x25*/
#define disasm_sub_3  disasm_binop_3		/*0x24*/
#define disasm_sub_3s disasm_binop_3		/*0x25*/

#define disasm_sbc_1  disasm_binop_1		/*0x2C*/
#define disasm_sbc_1s disasm_binop_1		/*0x2D*/
#define disasm_sbc_2  disasm_binop_2		/*0x2C*/
#define disasm_sbc_2s disasm_binop_2		/*0x2D*/
#define disasm_sbc_3  disasm_binop_3		/*0x2C*/
#define disasm_sbc_3s disasm_binop_3		/*0x2D*/

#define disasm_rsb_1  disasm_binop_1		/*0x26*/
#define disasm_rsb_1s disasm_binop_1		/*0x27*/
#define disasm_rsb_2  disasm_binop_2		/*0x26*/
#define disasm_rsb_2s disasm_binop_2		/*0x27*/
#define disasm_rsb_3  disasm_binop_3		/*0x26*/
#define disasm_rsb_3s disasm_binop_3		/*0x27*/

#define disasm_rsc_1  disasm_binop_1		/*0x2E*/
#define disasm_rsc_1s disasm_binop_1		/*0x2F*/
#define disasm_rsc_2  disasm_binop_2		/*0x2E*/
#define disasm_rsc_2s disasm_binop_2		/*0x2F*/
#define disasm_rsc_3  disasm_binop_3		/*0x2E*/
#define disasm_rsc_3s disasm_binop_3		/*0x2F*/

#define disasm_and_1  disasm_binop_1		/*0x20*/
#define disasm_and_1s disasm_binop_1		/*0x21*/
#define disasm_and_2  disasm_binop_2		/*0x20*/
#define disasm_and_2s disasm_binop_2		/*0x21*/
#define disasm_and_3  disasm_binop_3		/*0x20*/
#define disasm_and_3s disasm_binop_3		/*0x21*/

#define disasm_eor_1  disasm_binop_1		/*0x22*/
#define disasm_eor_1s disasm_binop_1		/*0x23*/
#define disasm_eor_2  disasm_binop_2		/*0x22*/
#define disasm_eor_2s disasm_binop_2		/*0x23*/
#define disasm_eor_3  disasm_binop_3		/*0x22*/
#define disasm_eor_3s disasm_binop_3		/*0x23*/

#define disasm_orr_1  disasm_binop_1		/*0x38*/
#define disasm_orr_1s disasm_binop_1		/*0x39*/
#define disasm_orr_2  disasm_binop_2		/*0x38*/
#define disasm_orr_2s disasm_binop_2		/*0x39*/
#define disasm_orr_3  disasm_binop_3		/*0x38*/
#define disasm_orr_3s disasm_binop_3		/*0x39*/

#define disasm_bic_1  disasm_binop_1		/*0x3C*/
#define disasm_bic_1s disasm_binop_1		/*0x3D*/
#define disasm_bic_2  disasm_binop_2		/*0x3C*/
#define disasm_bic_2s disasm_binop_2		/*0x3D*/
#define disasm_bic_3  disasm_binop_3		/*0x3C*/
#define disasm_bic_3s disasm_binop_3		/*0x3D*/

#define disasm_cmp_1s disasm_tst_1		/*0x35*/
#define disasm_cmp_2s disasm_tst_2		/*0x35*/
#define disasm_cmp_3s disasm_tst_3		/*0x35*/

#define disasm_cmn_1s disasm_tst_1		/*0x37*/
#define disasm_cmn_2s disasm_tst_2		/*0x37*/
#define disasm_cmn_3s disasm_tst_3		/*0x37*/

#define disasm_tst_1s disasm_tst_1		/*0x31*/
#define disasm_tst_2s disasm_tst_2		/*0x31*/
#define disasm_tst_3s disasm_tst_3		/*0x31*/

#define disasm_teq_1s disasm_tst_1		/*0x33*/
#define disasm_teq_2s disasm_tst_2		/*0x33*/
#define disasm_teq_3s disasm_tst_3		/*0x33*/


#define disasm_ld1_imm disasm_ldst1_imm	
#define disasm_ld1_imm_p disasm_ldst1_imm	
#define disasm_ld1_imm_b disasm_ldst1_imm
#define disasm_ld1_imm_pb disasm_ldst1_imm

#define disasm_ld1_reg disasm_ldst1_reg	
#define disasm_ld1_reg_p disasm_ldst1_reg
#define disasm_ld1_reg_b disasm_ldst1_reg
#define disasm_ld1_reg_pb disasm_ldst1_reg

#define disasm_ld2_imm disasm_ldst2_imm	
#define disasm_ld2_reg disasm_ldst2_reg

#define disasm_st1_imm disasm_ldst1_imm
#define disasm_st1_imm_p disasm_ldst1_imm
#define disasm_st1_imm_b disasm_ldst1_imm
#define disasm_st1_imm_pb disasm_ldst1_imm

#define disasm_st1_reg disasm_ldst1_reg	
#define disasm_st1_reg_p disasm_ldst1_reg
#define disasm_st1_reg_b disasm_ldst1_reg
#define disasm_st1_reg_pb disasm_ldst1_reg

#define disasm_st2_imm disasm_ldst2_imm
#define disasm_st2_reg disasm_ldst2_reg

#define disasm_ldm disasm_ldstm
#define disasm_stm disasm_ldstm

#define disasm_mla_s   disasm_mla
#define disasm_mul_s   disasm_mul
#define disasm_smull_s disasm_smull
#define disasm_smlal_s disasm_smlal
#define disasm_umull_s disasm_umull
#define disasm_umlal_s disasm_umlal

#define disasm_ldd_imm disasm_ldstd_imm	
#define disasm_ldd_reg disasm_ldstd_reg

#define disasm_std_imm disasm_ldstd_imm
#define disasm_std_reg disasm_ldstd_reg


using namespace simit;

const char *arm_conditional[] =
{"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc",
 "hi", "ls", "ge", "lt", "gt", "le", "", "nv"};

const char *arm_regnames_simple[] =
{"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
 "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};

const char *arm_regnames[] =
{"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
 "r8", "r9", "sl", "fp", "ip", "sp", "lr", "pc"};

const char *arm_shift[] =
{"lsl", "lsr", "asr", "ror"};

const char *msr_fields[] = 
{"", "c", "x", "cx", "s", "cs", "xs", "cxs",
 "f", "cf", "xf", "cxf", "sf", "csf", "xsf", "cxsf"};

/*table to compress 4 bits into 2 bits
   00 01 11 10
00  *  x  x  *
01  *  x  x  *
11  *  o  o  *
10  *  v  o  *

* -- 0
x -- 1
o -- 2	signature of ld/st mode2
v -- 3	signature of swap/mult
*/

void impl_default(simit::arm_emulator *emu, arm_inst_t inst)
{
	fprintf(stderr, "Warning: Unimplemented instruction 0x%08x:"
		"0x%08x ignored.\n", PC-8, inst);

	if (!emu->is_user_level())
		emu->raise_exception(UndefinedInstrV);
}

char *disasm_default(arm_inst_t inst, arm_addr_t pc, char *buf)
{
	buf += sprintf(buf, "????\n");
	return buf;
}

static const char *arm_dpinames[] = 
{"and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc",
 "tst", "teq", "cmp", "cmn", "orr", "mov", "bic", "mvn"};


/*utilities for alu operations*/
static inline uint32_t rotate_right(uint32_t val, uint32_t imm)
{
	return (val >> imm ) | (val << (32 - imm));
}


/*data processing instructions*/
static uint32_t disasm_shifter_operand1(arm_inst_t inst, char *buf)
{
	uint32_t imm = inst & 0xff;
	uint32_t shift = (inst>>7) & 0x1e;
	uint32_t result;

	result = rotate_right(imm, shift);
	return sprintf(buf, "#%u", result);
}

static uint32_t disasm_shifter_operand2(arm_inst_t inst, char *buf)
{
	uint32_t shift_imm = (inst>>7) & 0x01f;
	uint8_t type = (inst>>5) & 0x03;

	if (shift_imm==0 && type==3)
		return sprintf(buf, "%s, rrx", arm_regnames[RMFLD]);
	else if (shift_imm==0 && type==0)
		return sprintf(buf, "%s", arm_regnames[RMFLD]);
	else
		return sprintf(buf, "%s, %s #%u",
			arm_regnames[RMFLD], arm_shift[type], shift_imm);
}

static uint32_t disasm_shifter_operand3(arm_inst_t inst, char *buf)
{
	uint8_t type = (inst>>5) & 0x03;

	return sprintf(buf, "%s, %s %s",
		arm_regnames[RMFLD], arm_shift[type], arm_regnames[RSFLD]);
}

static char *disasm_unop_1(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s%s %s, ",
		arm_dpinames[DOPC], arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RDFLD]);
	buf += disasm_shifter_operand1(inst, buf);
	buf += sprintf(buf, ";\n");
	return buf;
} 

static char *disasm_unop_2(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s%s %s, ",
		arm_dpinames[DOPC], arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RDFLD]);
	buf += disasm_shifter_operand2(inst, buf);
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_unop_3(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s%s %s, ",
		arm_dpinames[DOPC], arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RDFLD]);
	buf += disasm_shifter_operand3(inst, buf);
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_binop_1(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s%s %s, %s, ",
		arm_dpinames[DOPC], arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RDFLD], arm_regnames[RNFLD]);
	buf += disasm_shifter_operand1(inst, buf);
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_binop_2(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s%s %s, %s, ",
		arm_dpinames[DOPC], arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RDFLD], arm_regnames[RNFLD]);
	buf += disasm_shifter_operand2(inst, buf);
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_binop_3(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s%s %s, %s, ",
		arm_dpinames[DOPC], arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RDFLD], arm_regnames[RNFLD]);
	buf += disasm_shifter_operand3(inst, buf);
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_tst_1(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s %s, ",
		arm_dpinames[DOPC], arm_conditional[COND],
		arm_regnames[RNFLD]);
	buf += disasm_shifter_operand1(inst, buf);
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_tst_2(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s %s, ",
		arm_dpinames[DOPC], arm_conditional[COND],
		arm_regnames[RNFLD]);
	buf += disasm_shifter_operand2(inst, buf);
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_tst_3(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s %s, ",
		arm_dpinames[DOPC], arm_conditional[COND],
		arm_regnames[RNFLD]);
	buf += disasm_shifter_operand3(inst, buf);
	buf += sprintf(buf, ";\n");
	return buf;
}

/*memory opcode fields*/
#define BFLD ((inst>>22)&1)
#define UFLD ((inst>>23)&1)
#define PFLD ((inst>>24)&1)
#define WFLD ((inst>>21)&1)
#define SIGN ((inst>>6)&1)
#define HFLD ((inst>>5)&1)
#define LFLD ((inst>>20)&1)

static char *disasm_ldst1_imm(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s%s%s %s, ",
		LFLD?"ldr":"str", arm_conditional[COND], BFLD?"b":"",
		(!PFLD&&WFLD)?"t":"", arm_regnames[RDFLD]);

	/*immediate offset*/
	if (PFLD) {
		buf += sprintf(buf, "[%s, #%s%u]",
			arm_regnames[RNFLD], UFLD?"":"-", inst&0xFFF);
		if (WFLD)
			buf += sprintf(buf, "!");
	}
	else {
		buf += sprintf(buf, "[%s], #%s%u",
			arm_regnames[RNFLD], UFLD?"":"-", inst&0xFFF);
	}
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_ldst1_reg(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	uint32_t shift_imm = (inst>>7) & 0x01f;
	uint8_t type = (inst>>5) & 0x03;

	buf += sprintf(buf, "%s%s%s%s %s, ",
		LFLD?"ldr":"str", arm_conditional[COND], BFLD?"b":"",
		(!PFLD&&WFLD)?"t":"", arm_regnames[RDFLD]);

	/*register offset*/
	if (PFLD) {
		buf += sprintf(buf, "[%s, %s%s",
			arm_regnames[RNFLD], UFLD?"":"-", arm_regnames[RMFLD]);

		if (shift_imm==0 && type==0) 
			buf += sprintf(buf, "]");
		else if (shift_imm==0 && type==3)
			buf += sprintf(buf, ", rrx]");
		else
			buf += sprintf(buf, ", %s #%u]", arm_shift[type], shift_imm); 

		if (WFLD)
			buf += sprintf(buf, "!");
	}
	else {
		buf += sprintf(buf, "[%s], %s%s",
			arm_regnames[RNFLD], UFLD?"":"-", arm_regnames[RMFLD]);

		if (shift_imm==0 && type==3)
			buf += sprintf(buf, ", rrx");
		else if (shift_imm || type) 
			buf += sprintf(buf, ", %s #%u", arm_shift[type], shift_imm); 
	}

	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_ldst2_imm(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	uint32_t offset = ((inst>>4) & 0xF0) | (inst & 0xF);

	buf += sprintf(buf, "%s%s%s%s %s, ",
		LFLD?"ldr":"str", arm_conditional[COND], SIGN?"s":"", HFLD?"h":"b",
		arm_regnames[RDFLD]);

	/*immediate offset*/
	if (PFLD) {
		buf += sprintf(buf, "[%s, #%s%u]",
			arm_regnames[RNFLD], UFLD?"":"-", offset);
		if (WFLD)
			buf += sprintf(buf, "!");
	}
	else {
		buf += sprintf(buf, "[%s], #%s%u",
			arm_regnames[RNFLD], UFLD?"":"-", offset);
	}
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_ldst2_reg(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s%s%s %s, ",
		LFLD?"ldr":"str", arm_conditional[COND], SIGN?"s":"", HFLD?"h":"b",
		arm_regnames[RDFLD]);

	/*immediate offset*/
	if (PFLD) {
		buf += sprintf(buf, "[%s, %s%s]",
			arm_regnames[RNFLD], UFLD?"":"-", arm_regnames[RMFLD]);
		if (WFLD)
			buf += sprintf(buf, "!");
	}
	else {
		buf += sprintf(buf, "[%s], %s%s",
			arm_regnames[RNFLD], UFLD?"":"-", arm_regnames[RMFLD]);
	}
	buf += sprintf(buf, ";\n");
	return buf;
}
	
static char *disasm_swap(arm_inst_t inst, arm_addr_t addr, char *buf)
{
    buf += sprintf(buf, "swp%s %s, %s, [%s];\n",
		arm_conditional[COND],
		arm_regnames[RDFLD], arm_regnames[RMFLD], arm_regnames[RNFLD]);
    return buf;
}

static char *disasm_swap_byte(arm_inst_t inst, arm_addr_t addr, char *buf)
{
    buf += sprintf(buf, "swp%sb %s, %s, [%s];\n",
		arm_conditional[COND],
		arm_regnames[RDFLD], arm_regnames[RMFLD], arm_regnames[RNFLD]);
    return buf;
}

static char *disasm_ldstm(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	uint32_t i, first = 1;
	buf += sprintf(buf, "%s%s%s%s %s%s, {",
		LFLD?"ldm":"stm", arm_conditional[COND], UFLD?"i":"d", PFLD?"b":"a",
		arm_regnames[RNFLD], WFLD?"!":"");

	for (i = 0; i<16; i++) {
		if ((1<<i) & inst) {
			buf += sprintf(buf, "%s%s", first?"":", ", arm_regnames[i]);
			first = 0;
		}
	}
	buf += sprintf(buf, "}");

	if (BITn(inst,22))
		buf += sprintf(buf, "^");

	buf += sprintf(buf, ";\n");
	return buf;
}

/* multiplications */

static char *disasm_mla(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "mla%s%s %s, %s, %s, %s;\n",
		arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RNFLD], arm_regnames[RMFLD],
		arm_regnames[RSFLD], arm_regnames[RDFLD]); 
	return buf;
}

static char *disasm_mul(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "mul%s%s %s, %s, %s;\n",
		arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RNFLD], arm_regnames[RMFLD], arm_regnames[RSFLD]); 
	return buf;
}

static char *disasm_smull(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "smull%s%s %s, %s, %s, %s;\n",
		arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RDFLD], arm_regnames[RNFLD],
		arm_regnames[RMFLD], arm_regnames[RSFLD]); 
	return buf;
}

static char *disasm_smlal(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "smlal%s%s %s, %s, %s, %s;\n",
		arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RDFLD], arm_regnames[RNFLD],
		arm_regnames[RMFLD], arm_regnames[RSFLD]); 
	return buf;
}

static char *disasm_umull(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "umull%s%s %s, %s, %s, %s;\n",
		arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RDFLD], arm_regnames[RNFLD],
		arm_regnames[RMFLD], arm_regnames[RSFLD]); 
	return buf;
}

static char *disasm_umlal(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "umlal%s%s %s, %s, %s, %s;\n",
		arm_conditional[COND], SFLD?"s":"",
		arm_regnames[RDFLD], arm_regnames[RNFLD],
		arm_regnames[RMFLD], arm_regnames[RSFLD]); 
	return buf;
}

/* branches */

static char *disasm_branch(arm_inst_t inst, arm_addr_t pc, char *buf)
{
	int32_t offset = (int32_t)(inst<<8)>>6;
	buf += sprintf(buf, "b%s 0x%x;\n",
		arm_conditional[COND], pc+8+offset);
	return buf;
}

static char *disasm_branch_link(arm_inst_t inst, arm_addr_t pc, char *buf)
{
	int32_t offset = (int32_t)(inst<<8)>>6;
	buf += sprintf(buf, "bl%s 0x%x;\n",
		arm_conditional[COND], pc+8+offset);
	return buf;
}


/* status registers */

static char *disasm_mrs(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "mrs%s %s, CPSR;\n",
		arm_conditional[COND], arm_regnames[RDFLD]);
	return buf;
}

static char *disasm_mrs_r(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "mrs%s %s, SPSR;\n",
		arm_conditional[COND], arm_regnames[RDFLD]);
	return buf;
}

/*0x32*/
static char *disasm_msr_1(arm_inst_t inst, arm_addr_t addr, char *buf)
{
    uint32_t operand;
    uint32_t imm = inst & 0xff;
    uint32_t shift = (inst>>7) & 0x1e;
	operand = (imm >> shift) | (imm << (32 - shift));
	buf += sprintf(buf, "msr%s CPSR_%s, #%u;\n",
		arm_conditional[COND], msr_fields[RNFLD], operand);
	return buf;
}

/*0x36*/
static char *disasm_msr_1r(arm_inst_t inst, arm_addr_t addr, char *buf)
{
    uint32_t operand;
    uint32_t imm = inst & 0xff;
    uint32_t shift = (inst>>7) & 0x1e;
	operand = (imm >> shift) | (imm << (32 - shift));
	buf += sprintf(buf, "msr%s SPSR_%s, #%u;\n",
		arm_conditional[COND], msr_fields[RNFLD], operand);
	return buf;
}

/*0x12*/
static char *disasm_msr_2(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "msr%s CPSR_%s, %s;\n",
		arm_conditional[COND], msr_fields[RNFLD], arm_regnames[RMFLD]);
	return buf;
}

/*0x16*/
static char *disasm_msr_2r(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "msr%s SPSR_%s, %s;\n",
		arm_conditional[COND], msr_fields[RNFLD], arm_regnames[RMFLD]);
	return buf;
}


/* coprocessor instruction */

static char *disasm_cdp(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "cdp p%u, %u, cr%u, cr%u, cr%u, %u\n",
			RSFLD, (inst>>20)&15, RDFLD, RNFLD, RMFLD, (inst>>5)&7);
	return buf;
}

static char *disasm_mcr(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "mcr p%u, %u, %s, cr%u, cr%u, %u\n",
			RSFLD, (inst>>21)&7, arm_regnames[RDFLD],
			RNFLD, RMFLD, (inst>>5)&7);
	return buf;
}

static char *disasm_mrc(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "mrc p%u, %u, %s, cr%u, cr%u, %u\n",
			RSFLD, (inst>>21)&7, arm_regnames[RDFLD],
			RNFLD, RMFLD, (inst>>5)&7);
	return buf;
}

static char *disasm_ldc(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "ldc p%u, cr%u, ", RSFLD, RDFLD);

	if (PFLD) {
		buf += sprintf(buf, "[%s, #%s%u]",
			arm_regnames[RNFLD], UFLD?"":"-", (inst&0xFF)<<2);
		if (WFLD)
			buf += sprintf(buf, "!");
	}
	else {
		if (WFLD)
			buf += sprintf(buf, "[%s], #%s%u",
				arm_regnames[RNFLD], UFLD?"":"-", (inst&0xFF)<<2);
		else
			buf += sprintf(buf, "[%s], {%u}",
				arm_regnames[RNFLD], inst&0xFF);
	}
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_stc(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "stc p%u, cr%u, ", RSFLD, RDFLD);

	if (PFLD) {
		buf += sprintf(buf, "[%s, #%s%u]",
			arm_regnames[RNFLD], UFLD?"":"-", (inst&0xFF)<<2);
		if (WFLD)
			buf += sprintf(buf, "!");
	}
	else {
		if (WFLD)
			buf += sprintf(buf, "[%s], #%s%u",
				arm_regnames[RNFLD], UFLD?"":"-", (inst&0xFF)<<2);
		else
			buf += sprintf(buf, "[%s], {%u}",
				arm_regnames[RNFLD], inst&0xFF);
	}
	buf += sprintf(buf, ";\n");
	return buf;
}

/* syscall */
static char *disasm_sc(arm_inst_t inst, target_addr_t pc, char *buf)
{
	buf += sprintf(buf, "swi %x;\n", (inst<<8)>>8);
	return buf;
}

static char *disasm_clz(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "clz%s %s, %s\n",
		arm_conditional[COND], arm_regnames[RDFLD] ,arm_regnames[RMFLD]);
	return buf;
}

/* XScale instruction (MAR MRA) */
static char *disasm_mar(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "mar%s acc0, %s, %s\n",
		arm_conditional[COND], arm_regnames[RDFLD] ,arm_regnames[RNFLD]);
	return buf;
}

static char *disasm_mra(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "mra%s %s, %s, acc0\n",
		arm_conditional[COND], arm_regnames[RDFLD] ,arm_regnames[RNFLD]);
	return buf;
}

/* DSP instruction */
static char *disasm_ldstd_imm(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	uint32_t offset = ((inst>>4) & 0xF0) | (inst & 0xF);

	buf += sprintf(buf, "%s%s%s %s, ",
		HFLD?"str":"ldr", arm_conditional[COND], "d",
		arm_regnames[RDFLD]);

	/*immediate offset*/
	if (PFLD) {
		buf += sprintf(buf, "[%s, #%s%u]",
			arm_regnames[RNFLD], UFLD?"":"-", offset);
		if (WFLD)
			buf += sprintf(buf, "!");
	}
	else {
		buf += sprintf(buf, "[%s], #%s%u",
			arm_regnames[RNFLD], UFLD?"":"-", offset);
	}
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_ldstd_reg(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "%s%s%s %s, ",
		HFLD?"str":"ldr", arm_conditional[COND], "d",
		arm_regnames[RDFLD]);

	/*immediate offset*/
	if (PFLD) {
		buf += sprintf(buf, "[%s, %s%s]",
			arm_regnames[RNFLD], UFLD?"":"-", arm_regnames[RMFLD]);
		if (WFLD)
			buf += sprintf(buf, "!");
	}
	else {
		buf += sprintf(buf, "[%s], %s%s",
			arm_regnames[RNFLD], UFLD?"":"-", arm_regnames[RMFLD]);
	}
	buf += sprintf(buf, ";\n");
	return buf;
}

static char *disasm_qadd(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "qadd%s %s, %s, %s\n",
		arm_conditional[COND], arm_regnames[RDFLD],
		arm_regnames[RMFLD], arm_regnames[RNFLD]);
	return buf;
}

static char *disasm_qdadd(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "qdadd%s %s, %s, %s\n",
		arm_conditional[COND], arm_regnames[RDFLD],
		arm_regnames[RMFLD], arm_regnames[RNFLD]);
	return buf;
}

static char *disasm_qsub(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "qsub%s %s, %s, %s\n",
		arm_conditional[COND], arm_regnames[RDFLD],
		arm_regnames[RMFLD], arm_regnames[RNFLD]);
	return buf;
}

static char *disasm_qdsub(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "qdsub%s %s, %s, %s\n",
		arm_conditional[COND], arm_regnames[RDFLD],
		arm_regnames[RMFLD], arm_regnames[RNFLD]);
	return buf;
}

#define XFLD ((inst>>5)&1)
#define YFLD ((inst>>6)&1)

static char *disasm_smla_xy(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "smla%s%s%s %s, %s, %s, %s\n", XFLD?"t":"b",
		YFLD?"t":"b", arm_conditional[COND], arm_regnames[RNFLD],
		arm_regnames[RMFLD], arm_regnames[RSFLD], arm_regnames[RDFLD] );
	return buf;
}

static char *disasm_smlal_xy(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "smlal%s%s%s %s, %s, %s, %s\n", XFLD?"t":"b",
		YFLD?"t":"b", arm_conditional[COND], arm_regnames[RDFLD],
		arm_regnames[RNFLD], arm_regnames[RMFLD], arm_regnames[RSFLD] );
	return buf;
}

static char *disasm_smlaw_y(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "smlaw%s%s %s, %s, %s, %s\n", YFLD?"t":"b",
		arm_conditional[COND], arm_regnames[RNFLD],arm_regnames[RMFLD],
		arm_regnames[RSFLD], arm_regnames[RDFLD] );
	return buf;
}

static char *disasm_smul_xy(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "smul%s%s%s %s, %s, %s\n", XFLD?"t":"b",
		YFLD?"t":"b", arm_conditional[COND], arm_regnames[RNFLD],
		arm_regnames[RMFLD], arm_regnames[RSFLD] );
	return buf;
}

static char *disasm_smulw_y(arm_inst_t inst, arm_addr_t addr, char *buf)
{
	buf += sprintf(buf, "smulw%s%s %s, %s, %s\n", YFLD?"t":"b",
		arm_conditional[COND], arm_regnames[RNFLD],
		arm_regnames[RMFLD], arm_regnames[RSFLD] );
	return buf;
}


#include "auto_disasm.h"
#include "arm_dec2.h"


char *arm_disassemble(arm_inst_t inst, arm_addr_t pc, char *buf)
{
	return disasm_main(inst, pc, buf);
}

void arm_disassemble_to_stream(arm_inst_t inst, arm_addr_t pc, FILE *stream)
{
	char buf[256];
	arm_disassemble(inst, pc, buf);
	fprintf(stream, "0x%08x : 0x%08x    %s", pc, inst, buf);
}

int arm_decode_regname(const char *name)
{
	for (int ii = 0; ii < 16; ii++)
		if (strcasecmp(name, arm_regnames_simple[ii]) == 0) return ii;

	for (int ii = 10; ii < 16; ii++)
		if (strcasecmp(name, arm_regnames[ii]) == 0) return ii;

	return -1;
}


