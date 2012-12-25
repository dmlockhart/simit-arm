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

#ifndef SIMIT_ARMEMUL_H
#define SIMIT_ARMEMUL_H

#include <cstdio>
#include <misc.h> // search follows the path of -I's
#include "emumem.h"

namespace simit
{

/* some types for the arm architecture */

typedef enum
{
	SA1100 = 0x4401a100,
	PXA250 = 0x69052100,
	PXA270 = 0x69054110,
} cpu_id_t;


typedef enum
{
	USR_MODE = 16L,
	FIQ_MODE = 17L,
	IRQ_MODE = 18L,
	SVC_MODE = 19L,
	ABT_MODE = 23L,
	UND_MODE = 27L,
	SYS_MODE = 31L,
} cpu_mode_t;

typedef enum
{
	USR_BANK = 0,
	FIQ_BANK = 1,
	IRQ_BANK = 2,
	SVC_BANK = 3,
	ABT_BANK = 4,
	UND_BANK = 5,
	DUM_BANK = 6,
	SYS_BANK = USR_BANK,
} reg_bank_t;

typedef enum
{
	ResetV          = 0L,
	UndefinedInstrV = 4L,
	SWIV            = 8L,
	PrefetchAbortV  = 12L,
	DataAbortV      = 16L,
	AddrExceptnV    = 20L,
	IRQV            = 24L,
	FIQV            = 28L,
} ex_vector_t;


const word_t ABORTWORD = 0xefffffff;   /* SWI -1 */


/* forward declaration of some member classes */
class arm_mmu;
class arm_io;	
class arm_copro;

class arm_emulator 
{

  public:

	/* constructor */
	arm_emulator(bool verbose, bool user_level);

	/* destructor */
	~arm_emulator();

	/* reset everything */
	void reset();

	/* if verbose flag set */
	bool is_verbose() const {return verbose;}

	/* if syscall need to be emulated */
	bool is_user_level() const {return user_level;}



	/* load an ELF user-level program, set up stack/heap/mmap */
	void load_program(const char *, int argc, char *argv[], char *env[]);

	/* load an ELF OS image, no need to set up stack/heap */
	void load_osimage(const char *);

	/* load a binary image to address */
	void load_binary(const char *, arm_addr_t);

	/* load a configuration file that sets up cpu and memory */
	bool load_config(const char *);

	/* get the cpu_id value */
	cpu_id_t get_cpu_id() const {return cpu_val;}

	/* if this is xscale */
	bool is_xscale() const {return cpu_val==PXA250 || cpu_val==PXA270;}



	/* run the program till exit*/
	uint64_t run();

	/* debug a program */
	void debug();




	/* user interruption, can be called from CTRL-C handler */
	void interrupt();
	bool is_interrupted() const {return status==ST_SIGINT;}

	/* brk instruction */
	void break_point();
	bool is_broken() const {return status==ST_BREAK;}

	/* error */
	void seg_fault();
	bool is_fault() const {return status==ST_ERROR;}

	/* check status */
	bool is_running()   const {return status==ST_RUNNING;}
	bool is_debugging() const {return status==ST_DEBUGGING;}
	void set_running()  {status=ST_RUNNING;}

	/* program exit, used in user-level simulation */
	void prog_exit(int);
	bool is_done() const {return status==ST_EXIT;}

	/* get the exit code of the program, used in user-level simulation */
	int get_retcode() const {return retcode;}

	/* if pc is set by the loader or the configuration file */
	bool is_pc_set() const {return pc_set;}




	/******************************************************************
	   cpu mode
	 *****************************************************************/
	/* switch mode and change bank, return the old mode */
	cpu_mode_t switch_cpu_mode(cpu_mode_t new_mode)
	{
		switch_reg_bank(mode, new_mode);
		return translation_mode(new_mode);
	}

	/* get the mode */
	cpu_mode_t get_cpu_mode() const {return mode;}


	/* check if in one exception mode */
	bool is_exception_mode () const {return mode_exception;}

	/* check if in one privilege mode */
	bool is_privilege_mode () const {return mode_privilege;}

	/* switch the mode, returns the old mode, but does not change reg bank */
	cpu_mode_t translation_mode(cpu_mode_t new_mode);

	/* change register bank, and returns the old mode */
	void switch_reg_bank(cpu_mode_t old_mode, cpu_mode_t new_mode);


	/* raise an exception, vector is the address to jump to */
	void raise_exception (ex_vector_t vector);

	/******************************************************************
	   register access interfaces 
	 *****************************************************************/
	word_t get_pc() const {return my_regs.gpr[PC_REAL_IND];}

	word_t read_gpr(int index) const {return my_regs.gpr[index];}

	void write_gpr(int index, word_t val)
	{
		my_regs.gpr[index==15?PC_REAL_IND:index] = val;
	}

	void write_gpr2(int index, word_t val)
	{
		my_regs.gpr[index] = val;
	}

	word_t read_cpsr() const
	{
		return my_regs.cpsr&0xfffffff | (my_regs.cc&0xf)<<28;
	}


	void write_cpsr(word_t val)
	{
		if (mode != (cpu_mode_t)(val&0x1f)){
			switch_cpu_mode((cpu_mode_t)(val&0x1f));
		}
		my_regs.cpsr=val;
		my_regs.cc=(val>>28)&0xf;
		my_regs.i_flag = (val>>7)& 1;
		my_regs.f_flag = (val>>6)& 1;
	}

	word_t read_spsr() const
	{
		if (bank) //not User Bank = has spsr = Exception Mode 
			return my_regs.spsr[bank];/*----add processor mode and bank----*/
		else
			return read_cpsr();
	}

	void write_spsr(word_t val)
	{
		if (bank)//not User Bank = has spsr = Exception Mode
			my_regs.spsr[bank]=val;/*----------------add processor mode------ */
	}

	word_t read_cc() const {return my_regs.cc;}
	void write_cc(word_t val) {my_regs.cc=val;}


	/* some convenience routine for interpretation */
	void update_CPSR(word_t val, word_t mask);
	void update_SPSR(word_t val, word_t mask);


	/******************************************************************
	   The CC access functions below are for compiled simulators  
	 *****************************************************************/
	word_t read_NZF() const {return my_regs.NZF;}
	word_t read_CF() const {return my_regs.CF;}
	word_t read_VF() const {return my_regs.VF;}

	void write_NZF(word_t val) {my_regs.NZF = val;}
	void write_CF(word_t val) {my_regs.CF = val;}
	void write_VF(word_t val) {my_regs.VF = val;}

	/* conversion from one set of NZCV to the other set */
	void cfs_to_cc()
	{
		my_regs.cc = ((my_regs.NZF>>28)&8) | (my_regs.CF<<1) |
					 ((my_regs.NZF==0)<<2) | (my_regs.VF>>31);
	}

	void cc_to_cfs()
	{
		my_regs.NZF = ((my_regs.cc&4)^4) | ((my_regs.cc&8)<<28);
		my_regs.CF = (my_regs.cc>>1)&1;
		my_regs.VF = (my_regs.cc)<<31;
	}


	/******************************************************************
	   The counter access functions
	 *****************************************************************/
	/* total instruction counter, compiled counter */
	uint64_t get_icount() const {return icount;}
	uint64_t get_ccount() const {return ccount;}

    /* instruction counters, used by the compiled simulator */
    void increment_compile_count(uint32_t val) {ccount += val;}
    void increment_interpret_count(uint32_t val) {icount += val;}
 
	/* stats output */
	void dump_instruction_counters(FILE *);

	/* stats initialization */
	void reset_instruction_counters();


	/* decrement the prescale counter */
	void decrement_pcount(int c) {pcount -= c;}

	/* set the maximum instructions to simulate, default is -1 */
	void set_max_count(uint64_t mcount) {max_count = mcount;}

	/* memory */
	memory *mem;

	/* mmu */
	arm_mmu	*mmu;


	/******************************************************************
	   The signal functions
	 *****************************************************************/
	void raise_reset_signal() {NresetSig = false; SigSet = true;}
	void raise_fiq_signal()   {NfiqSig = false;   SigSet = true;}
	void raise_irq_signal()   {NirqSig = false;   SigSet = true;}

	void clear_reset_signal() {NresetSig = true; SigSet = !(NfiqSig & NirqSig);}
	void clear_fiq_signal()   {NfiqSig = true; SigSet = !(NresetSig & NirqSig);}
	void clear_irq_signal()   {NirqSig = true; SigSet = !(NresetSig & NfiqSig);}


  protected:

	/* reset the content of the registers */
	void init_registers();

	/* fetch an instruction in system level*/
	arm_inst_t fetch_inst_system(arm_addr_t addr);

	/* fetch an instruction in user level */
	arm_inst_t fetch_inst_user(arm_addr_t addr) 
	{
		word_t inst;
		mem->read_word(addr, &inst);
		return inst;
	}


	/* execute one instruction in system level*/
	void execute_system(arm_inst_t inst, word_t addr);

	/* execute one instruction in user level */
	void execute_user(arm_inst_t inst, word_t addr);

	/* fetch and run one instruction in system level */
	void step_system();

	/* fetch and run one instruction in user level */
	void step_user();

	/* run in system level */
	uint64_t run_system();

	/* run in user level */
	uint64_t run_user();
	uint64_t run_user_count();

	/* debug routines */
	void debug_trace(int count);
	void debug_go_to(arm_addr_t addr);
	void debug_dump_registers(FILE *stream);
	void debug_disasm(FILE *stream, arm_addr_t addr);
	void debug_dump(FILE *stream, arm_addr_t addr);

	/* convert mode to bank */
	reg_bank_t mode_to_bank(cpu_mode_t m) const;


	/* prescale counter for periodical IO update */
	int pcount;

	/******************************************************************
	   icount -- total instruction counter,
	   ccount -- compiled counter, only used in compiled simulation mode
	 *****************************************************************/
	uint64_t icount, ccount;

	bool max_count_set;
	uint64_t max_count;

	/* registers */
	struct regs_t {

		word_t gpr[NUM_GPR];
		word_t cpsr;
		word_t spsr[7];
		

		/* extra copy of NCZV flags, used to accelerate interpretation */
		word_t cc;

		/* another copy of NCZV flags, used to accelerate compiled ISS */
		word_t NZF; // the actual result, bit 31 is N
		word_t CF;	// bit 0 is the carry
		word_t VF;	// bit 31 is overflow

		/* extra copy of cpsr bits, used to accelerate interpretation */
		word_t i_flag; // bit 7 of cpsr: Disables IRQ interrupts when set
		word_t f_flag; // bit 6 of cpsr: Disables FIQ interrupts when set
	
		/* Big member stays in the back so that the compiled can use
		*  small offsets for all previous members. Small offsets reduce
		*  code size on x86.
		*/
		word_t reg_bank[7][16];

	} my_regs;


	bool NresetSig;	/* reset the processor */
	bool NfiqSig;
	bool NirqSig;

	bool SigSet;	/* a signal is set, !NresetSig | !NfigSig | !NirqSig*/




	/******************************************************************
       miscellaneous simulation status
	 *****************************************************************/
	int retcode;	// valid on ST_EXIT

	enum status_t 
	{
		ST_RESET,		// just reset, empty
		ST_IDLE,		// program loaded
		ST_RUNNING,
		ST_DEBUGGING,
		ST_ERROR,		// memory protection violation
		ST_EXIT,		// program exit normally
		ST_SIGINT,		// user interrupted
		ST_BREAK,		// brk instruction
		ST_ELSE,
	} status;


	reg_bank_t bank;
	cpu_mode_t mode;
	bool   mode_exception;
	bool   mode_privilege;


	/******************************************************************
       configuration parameters
	 *****************************************************************/
	const bool verbose;
	const bool user_level;
	const int  prescale;

	cpu_id_t cpu_val;



	/******************************************************************
	   some loader related functions and variables
	 *****************************************************************/
	void load_elf32(const char *);
	bool cpu_config(const char *);

	// some variables for the ELF loader
	uint32_t data_base;
	uint32_t data_size;
	uint32_t prog_base;

	// variable for configuration file, pc set in configuration file
	bool pc_set;


  public:

	/******************************************************************
	   some less frequently used members 
	 *****************************************************************/

	/* io */
	arm_io  *io;

	/* coprocessors */
	arm_copro *copro[16];


	/******************************************************************
	   finally some system call emulation related stuff, user-level only
	 *****************************************************************/

	arm_addr_t syscall_get_brk() const {return brk_point;}
	arm_addr_t syscall_get_mmap_brk() const {return mmap_brk_point;}

	/* set brk point for syscall emulation, mmap is actually not used */
	void syscall_set_brk(arm_addr_t addr) {brk_point = addr;}
	void syscall_set_mmap_brk(arm_addr_t addr) {mmap_brk_point = addr;}

	void syscall_alloc_buf(unsigned size);
	void syscall_realloc_buf(unsigned size);

	/* some variables for system call states */
    int syscall_phase;
    int syscall_iret;
    void *syscall_buf;

	unsigned syscall_mem_size;
    int syscall_open_len;
    int syscall_open_addr;
    int syscall_times_ret;

  private:
	target_addr_t brk_point;
	target_addr_t mmap_brk_point;


  public:

	word_t val_register[16]; /*Validation Suite Coprocessor's Registers*/
	uint64_t accumulator;   /*Xscale coprocessor0 acc0 */	


	bool int_pending(void);

	/* This returns the number of clock ticks since the last reset.  */
	uint64_t time(void) const {return icount + ccount;}

	unsigned abort_addr; /* For Prefetch Abort */

  };

} /* namespace */

#endif
