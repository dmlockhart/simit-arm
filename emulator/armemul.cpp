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

#include <csignal>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include "armemul.h"
#include "armmmu.h"
#include "arm_io.h" 
#include "sa_io.h" 
#include "pxa_io.h" 
#include "armcopro.h" 
#include "decode.h"

using namespace simit;

typedef arm_emulator emulator_t;
typedef arm_inst_t target_inst_t;
typedef arm_addr_t target_addr_t;



static const uint16_t cond_table[] =
{0xf0f0, 0x0f0f, 0xcccc, 0x3333,
 0xff00, 0x00ff, 0xaaaa, 0x5555,
 0x0c0c, 0xf3f3, 0xaa55, 0x55aa,
 0x0a05, 0xf5fa, 0xffff, 0x0000};


static inline uint32_t eval_cond(arm_emulator *emu, arm_inst_t inst)
{
	uint32_t val;
	val = (cond_table[COND] >> CC) & 1;
#if 0
	switch (COND) {
		case 0: val = Z_FLAG; break;
		case 1: val = !Z_FLAG; break;
		case 2: val = C_FLAG; break;
		case 3: val = !C_FLAG; break;
		case 4: val = N_FLAG; break;
		case 5: val = !N_FLAG; break;
		case 6: val = V_FLAG; break;
		case 7: val = !V_FLAG; break;
		case 8: val = C_FLAG & ~Z_FLAG; break;
		case 9: val = !(C_FLAG & ~Z_FLAG); break;
		case 10: val = N_FLAG == V_FLAG; break;
		case 11: val = N_FLAG ^ V_FLAG; break;
		case 12: val = !(Z_FLAG | (N_FLAG ^ V_FLAG)); break;
		case 13: val = Z_FLAG | (N_FLAG ^ V_FLAG); break;
		case 14: val = 1; break;
		default: val = 0; break;
	}
#endif
	return val;
}

arm_emulator::arm_emulator(bool verbose, bool user_level) :
	verbose(verbose), user_level(user_level), prescale(IO_DIVISOR)
{
	mem = new memory(0);
	mmu = new arm_mmu(this);

	for(int ii=1; ii<16; ii++) copro[ii] = NULL;

	io = NULL;

	syscall_buf = NULL;

	reset();

	pc_set = false;

	max_count_set = false;
	max_count = (uint64_t)-1;

}

arm_emulator::~arm_emulator()
{
	delete mem;
	delete mmu;
	delete io;

	for(int ii=1; ii<16; ii++) delete copro[ii];

	if (syscall_buf) {
		free(syscall_buf);
	}
	
}

void arm_emulator::reset()
{
	mem->reset();

	clear_reset_signal();
	clear_irq_signal();
	clear_fiq_signal();

	for(int ii=1; ii<16; ii++) val_register[ii] = 0;

	init_registers();
	reset_instruction_counters();

	status = ST_RESET;
	
	abort_addr = 1;

	if (syscall_buf) {
		free(syscall_buf);
		syscall_buf = NULL;
	}

}

bool arm_emulator::cpu_config(const char *cpu_name)
{
//	copro[1] =	new copro1(this);
//	copro[2] =	new copro2(this);
//	copro[4] =	new copro4(this);
//	copro[5] =	new copro5(this);
	copro[15]=	new copro15(this);	//mmu
	if (strcasecmp(cpu_name, "SA1100") == 0)
	{ 
		io		 =	new sa_io(this);
		cpu_val  =	SA1100;
		my_regs.gpr[1] = 109;	/*adsbitsy machine id. */
	}
	else if (strncasecmp(cpu_name, "PXA25", 5) == 0)
	{
		copro[14]=	new copro14(this);
		io		 =	new pxa_io(this);
		cpu_val =	PXA250;
		my_regs.gpr[1] = 89;    /*lubbock machine id.*/
		mmu->set_xscale();
	}
	else if	(strncasecmp(cpu_name, "PXA27", 5) == 0) 
	{
		copro[6] =	new copro6(this);
		copro[14]=	new copro14(this);
		io		 =	new pxa_io(this);
		cpu_val = PXA270;
		my_regs.gpr[1] = 406;	/*mainstone machine id. */
		mmu->set_xscale();
	}
	else{ 
		return false;
	}
	return true;
}

void arm_emulator::prog_exit(int ret)
{
	status = ST_EXIT;
	retcode = ret;
}

void arm_emulator::interrupt()
{
	if (status==ST_RUNNING || status==ST_DEBUGGING)
		status = ST_SIGINT;
	else
		exit(0);
}

void arm_emulator::break_point()
{
	if (status==ST_RUNNING || status==ST_DEBUGGING)
		status = ST_BREAK;
}

void arm_emulator::seg_fault()
{
	status = ST_ERROR;
	retcode = 1;
}

/* Two sets of interpretation routines,
   one performs system-level simulation, one does user-level.
   The difference is the macros in arch.hpp.
*/
#define  SIMIT_SYSTEM_LEVEL
#include "auto_impl.h"
#include "arm_iss.hpp"
#include "arm_dec.h"

void arm_emulator::execute_system(arm_inst_t inst, word_t addr)
{

#if 0
	arm_disassemble_to_stream(inst, get_pc(), stderr);
	fprintf(stderr, "0x%08x : 0x%08x\n", addr, inst);
#endif

	/* set default next PC; operation can override */
	write_gpr2(PC_REAL_IND, addr+4);
	if (COND == 0xe || eval_cond(this, inst)) {

		/*operation sees PC+8*/
		write_gpr2(PC_AHEAD_IND, addr+8);

		decode_main_system(this, inst);
	}
}

#undef   SIMIT_SYSTEM_LEVEL
#include "auto_impl.h"
#include "arm_iss.hpp"
#include "arm_dec.h"

void arm_emulator::execute_user(arm_inst_t inst, word_t addr)
{

#if 0
	arm_disassemble_to_stream(inst, get_pc(), stderr);
	fprintf(stderr, "0x%08x : 0x%08x\n", addr, inst);
#endif

	/* set default next PC; operation can override */
	write_gpr2(PC_REAL_IND, addr+4);
	if (COND == 0xe || eval_cond(this, inst)) {

		/*operation sees PC+8*/
		write_gpr2(PC_AHEAD_IND, addr+8);

		decode_main_user(this, inst);
	}
}



arm_inst_t arm_emulator::fetch_inst_system(arm_addr_t addr)
{
	mmu_fault_t fault;
	word_t inst;
	
	fault =  mmu->load_instr(addr, &inst);
	if (fault){
		abort_addr = addr ;
		return ABORTWORD;
	}
	return inst;
}

void arm_emulator::step_system()
{
	arm_inst_t inst;

	if (SigSet) {

		/* Any exceptions ?  */
		if (!NresetSig) {
			raise_exception (ResetV);
			return;
		}
		else if ((!NfiqSig) && (!my_regs.f_flag)) {
			raise_exception (FIQV);
			return;
		}
		else if ((!NirqSig) && (!my_regs.i_flag)) {
			raise_exception (IRQV);
			return;
		}
	}

	if(pcount <= 0){
		io->do_cycle();
		icount += prescale - pcount;
		pcount = prescale;
		if (max_count_set && icount >= max_count)
			status = ST_EXIT;
	}

	pcount--;

	inst = fetch_inst_system(get_pc()); 
	execute_system(inst, get_pc());

}


void arm_emulator::step_user()
{
	arm_inst_t inst;
	inst = fetch_inst_user(get_pc());
	execute_user(inst, get_pc());
	icount++;
}

uint64_t arm_emulator::run_user()
{
	icount = 0;
	status = ST_RUNNING;

	while(status==ST_RUNNING)
		step_user();

	return icount;
}

uint64_t arm_emulator::run_user_count()
{
	icount = 0;
	status = ST_RUNNING;

	while(status==ST_RUNNING && icount < max_count)
		step_user();

	return icount;
}

uint64_t arm_emulator::run_system()
{
	icount = 0;
	status = ST_RUNNING;
	pcount = prescale;

	while(status==ST_RUNNING)
		step_system();

	icount += prescale - pcount;
	return icount;
}

uint64_t arm_emulator::run()
{
	max_count_set = (max_count != (uint64_t)-1);

	if (user_level)
		return max_count_set?run_user_count():run_user();
	else 
		return run_system();
}


void arm_emulator::init_registers()
{
	int ii, jj;

	for (ii = 0; ii < NUM_GPR; ii++)
		my_regs.gpr[ii] = 0;
	
	for (ii = 0; ii < 7; ii++)
		for (jj = 0; jj < 16; jj++)
			my_regs.reg_bank[ii][jj] = 0;
	
	mode = SVC_MODE;
	mode_exception = true;
	mode_privilege = true;
	bank = SVC_BANK;

	my_regs.cpsr = 0xc0 | mode; //skyeye INTBITS
	my_regs.spsr[bank] = 0;
	my_regs.cc = 0;
	my_regs.i_flag = 1;
	my_regs.f_flag = 1;
}


	/* switch the mode, returns the old mode, but does not change reg bank */
cpu_mode_t arm_emulator::translation_mode(cpu_mode_t new_mode)
{
	cpu_mode_t old_mode = mode;
	mode = new_mode;
	/* mode_exception as a cache for fast checking */
	mode_exception = ((mode != USR_MODE) && (mode != SYS_MODE));
	mode_privilege = (mode != USR_MODE);
	
	/*switch all pages' access permision */
	mmu->evaluate_access_all();
	return old_mode;
}

void arm_emulator::reset_instruction_counters()
{
	icount = 0;
	ccount = 0;
}


void arm_emulator::dump_instruction_counters(FILE *fp)
{
	fputs("Total instructions : ", fp);
	dump_int64(icount+ccount, fp);
	if (icount+ccount > 1000000) {
		fputs(" (", fp);
		dump_int64_smart(icount+ccount, fp);
		fputs(")", fp);
	}

	/* in the compiled simulator */
	if (ccount) {
		fprintf(fp, ", including %.2lf%% compiled",
			100.0*ccount/(icount+ccount));
	}

	fputs("\n", fp);
}

void arm_emulator::syscall_realloc_buf(unsigned size)
{
	assert(syscall_buf!=NULL);
	syscall_buf = realloc(syscall_buf, size);
	if (syscall_buf==NULL) {
        fprintf(stderr, "Error: Out of host memory when allocating %d"
            " bytes for a system call.\n", size);
        exit(-1);
	}
}

void arm_emulator::syscall_alloc_buf(unsigned size)
{
	if (syscall_buf!=NULL)
		free(syscall_buf);

	syscall_buf = malloc(size);
	if (syscall_buf==NULL) {
        fprintf(stderr, "Error: Out of host memory when reallocating %d"
            " bytes for a system call.\n", size);
        exit(-1);
	}
}

void arm_emulator::update_CPSR(word_t val, word_t mask)
{
    uint32_t result;

    result = read_cpsr();
    if (!user_level && mode != USR_MODE)
	{
        if (mask&1)
			result = (result & 0xFFFFFF00) | (val & 0x000000FF);
		
		if (mask&2)
			result = (result & 0xFFFF00FF) | (val & 0x0000FF00);
		
		if (mask&4)
			result = (result & 0xFF00FFFF) | (val & 0x00FF0000);
    }

	if (mask&8)
		result = (result & 0x00FFFFFF) | (val & 0xFF000000);

    write_cpsr(result);
}

void arm_emulator::update_SPSR(word_t val, word_t mask)
{
    uint32_t result;

    if (!user_level && mode_exception) 
	{
    	result = read_spsr();

        if (mask&1)
			result = (result & 0xFFFFFF00) | (val & 0x000000FF);
		
		if (mask&2)
			result = (result & 0xFFFF00FF) | (val & 0x0000FF00);
		
		if (mask&4)
			result = (result & 0xFF00FFFF) | (val & 0x00FF0000);
		
		if (mask&8)
			result = (result & 0x00FFFFFF) | (val & 0xFF000000);

		write_spsr(result);
    }
} 
