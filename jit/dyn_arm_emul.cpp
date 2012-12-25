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
#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cassert>

#include <list>
#include <map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>

#include "dyn_arm_emul.hpp"
#include "armmmu.h"
#include "arm_io.h"
#include "armcopro.h"

#include "build_dll.hpp"

using std::list;
using std::map;
using std::multimap;
using std::pair;
using namespace simit;

typedef arm_emulator emulator_t;
typedef arm_inst_t target_inst_t;
typedef arm_addr_t target_addr_t;

#define PID_LIST  reinterpret_cast<list<unsigned> *>(pid_list)
#define DLL_INFO  reinterpret_cast<map<fptr_t, dll_info_t *> *>(dll_info)
#define DLL_INFO2 reinterpret_cast<map<unsigned, dll_info_t *> *>(dll_info2)
#define DLL_DICT  reinterpret_cast<multimap<unsigned, unsigned> *>(dll_dict)

/* Two types of simulation are implemented, interpretation and translation.

   The user level interpretation routines here are identical to those
   in the emulator directory. However, the system-level interpretation
   routines are slightly different. They must take into account
   self-modifying code. If a MEM_WRITE_* modifies a compiled address,
   the corresponding DLL must be unloaded. 

   The translated code is very different from the interpretation code.
   For user-level translation, care must be taken on WRITE_REG. If
   an instruction writes to register 15, then it is a jump instruction.
   The WRITE_REG should be translated into a goto or a return, depending on
   whether the target address is within the same translation block.
   Therefore, the WRITE_REG macro must be redefined to check if the
   destination is 15. If so, it will jump immediately. A prerequisite
   for this to work is that an instruction can only modify the PC in
   its last statement. Otherwise some effects of the instruction will
   be lost. Fortunately, the requirement can be satisfied for all ARM
   instructions. See arm.isa file for details.

   For system-level translation, additional care is given to memory write
   instructions due to the existence of self-modifying code. If a
   write modifies a compiled block. Then the compiled block should
   be invalidated immediately. Since in theory an instruction may modify
   itself, the invalidation cannot be done by the instruction itself.
   (I don't think a DLL can unload itself.) Therefore, the instruction
   must abort to the main execution loop, which will unload the compiled
   DLL, and then restart the instruction. A prerequisite for this to
   work is that all memory write instructions can restart. Fortunately,
   the requirement can be satisfied under the base-register restore
   abort model. See arm.isa for details.

*/

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

static bool write_check(void *e, arm_addr_t addr)
{
	dyn_arm_emulator *emu = reinterpret_cast<dyn_arm_emulator *>(e);

	return emu->check_write_range(addr, ~MMU_HASH_MASK + 1);
}

static bool write_alarm(void *e, arm_addr_t addr)
{
	dyn_arm_emulator *emu = reinterpret_cast<dyn_arm_emulator *>(e);

	if (emu->check_write(addr))
	{
		emu->set_invalidate_flag(addr);
		return true;
	}

	return false;
}

/* constructor */
dyn_arm_emulator::dyn_arm_emulator(bool verbose, bool showmsg,
	bool user_level, uint32_t thr, uint32_t blkbits) :
	arm_emulator(verbose, user_level), threshold(thr),
	shiftval(blkbits), showmsg(showmsg)
{
	/* limit threshold so that blk count never reaches -1u, -1u
	 * is a special value meaning that the block is awaiting compilation
	 */
	if (threshold > -3u) threshold = -3u;

	/* limit shiftval so that the blk_info table is not too big */
	if (shiftval < 9) shiftval = 9;

	/* for system level, the blk size should be no bigger than 1K, the
	   size of a tiny page for ARM.
	 */
	if (!user_level && shiftval > 10) shiftval = 10;

	inval_flag = false;
	inval_addr = 0;

	hit_num = 0;
	req_num = 0;
	dll_ind = 0;

	cache_dir = NULL;

	pid_list = reinterpret_cast<void *>(new list<unsigned>);

	blk_info = (blk_info_t *)calloc(1 << (32 - shiftval), sizeof(blk_info_t));

	dll_info = reinterpret_cast<void *>(new map<fptr_t, dll_info_t *>);
	dll_info2= reinterpret_cast<void *>(new map<unsigned, dll_info_t *>);
	dll_dict = reinterpret_cast<void *>(new multimap<unsigned, unsigned>);

	srv_count = 0;
	srv_index = 0;
	com_done = false;

	memset(srv_dir, 0, sizeof(srv_dir));

	pthread_cond_init(&pid_cond, NULL);
	pthread_mutex_init(&pid_mut, NULL);
	pthread_mutex_init(&srv_mut, NULL);

	temp_buf1 = (byte_t*)malloc(1 << shiftval);
	temp_buf2 = (byte_t*)malloc(1 << shiftval);

	if (blk_info==NULL || dll_info==NULL || dll_info2==NULL ||
		dll_dict==NULL || temp_buf1==NULL || temp_buf2==NULL)
	{
		fprintf(stderr, "Error: Insufficient memory.\n");
		exit(1);
	}

	if (!user_level)
		mmu->register_write_checker(this, write_check, write_alarm);

}

/* destructor */
dyn_arm_emulator::~dyn_arm_emulator()
{

	// close all dll libraries
	map<fptr_t, dll_info_t *>::iterator mit;
	for (mit = DLL_INFO->begin(); mit != DLL_INFO->end(); mit++)
	{
		dll_info_t *pdll = (*mit).second;
		dlclose(pdll->handle);
		free(pdll);
	}

	delete DLL_INFO;
	delete DLL_INFO2;
	delete DLL_DICT;
	delete PID_LIST;

	free(blk_info);
	free(temp_buf1);
	free(temp_buf2);

	if (cache_dir) free(cache_dir);

	com_cleanup();
}


/* The impl_xxxx routines for system level simulation is duplicated
 * in jit directory (in arm_iss_jit.cpp). When linking, ld will choose
 * those in arm_iss_jit.o instead of those in libarmemu.a since arm_iss_jit
 * appears first. The difference between the two is check_write.
 */

#define  SIMIT_SYSTEM_LEVEL
#include "auto_impl.h" 
#include "arm_iss.hpp"
#include "arm_dec.h"

void dyn_arm_emulator::execute_system(arm_inst_t inst, word_t addr)
{
	/* set default next PC; operation can override */
	write_gpr2(PC_REAL_IND, addr+4);
	if (COND == 0xe || eval_cond(this, inst)) {

		/*operation sees PC+8*/
		write_gpr2(PC_AHEAD_IND, addr+8);

		decode_main_system(this, inst);
	}
}

/* The uimpl_xxxx routines are those in libarmemu.a
 */
#undef   SIMIT_SYSTEM_LEVEL
#include "auto_impl.h" 
#include "arm_iss.hpp"
#include "arm_dec.h"

void dyn_arm_emulator::execute_user(arm_inst_t inst, word_t addr)
{
	/* set default next PC; operation can override */
	write_gpr2(PC_REAL_IND, addr+4);
	if (COND == 0xe || eval_cond(this, inst)) {

		/*operation sees PC+8*/
		write_gpr2(PC_AHEAD_IND, addr+8);

		decode_main_user(this, inst);
	}
}

void dyn_arm_emulator::step_system()
{
	arm_addr_t phy_addr;
	arm_inst_t  inst;
	byte_t *ptr;

	//  if (EventSet)
	//	  EnvokeEvent();

	if (SigSet)
	{
		/* Any exceptions ?  */
		if (!NresetSig)
		{
			raise_exception(ResetV);
			return;
		}
		else if ((!NfiqSig) && (!my_regs.f_flag))
		{
			raise_exception(FIQV);
			return;
		}
		else if ((!NirqSig) && (!my_regs.i_flag))
		{
			raise_exception(IRQV);
			return;
		}
	}

	if (pcount <= 0)
	{
		/* the compile_count includes interpret_count,
		   adjust at the end of the function */
		io->do_cycle();
		icount += prescale - pcount;
		pcount = prescale;
	
		if (max_count_set && icount >= max_count)
			status = ST_EXIT; 
	}

#ifdef SINGLESTEP
if (icount > (30*(1<<20) + 585300) && icount < (30*(1<<20) + 585360))
{
unsigned sum = 0;
for (int ii=0; ii<15; ii++) sum += read_gpr(ii);
fprintf(stderr, "=%x %x %x\n", get_pc(), sum, read_cpsr());
}
#endif

	/* see if the current PC can be loaded */
	if (mmu->translate_instr_addr(get_pc(), &phy_addr, &ptr)) 
	{
		abort_addr = get_pc();
		inst = ABORTWORD;
		execute_system(inst, get_pc()); 
		pcount--;
	}
	else 
	{
		unsigned ind = phy_addr >> shiftval;

#ifdef SINGLESTEP
L1:
#endif
		if (blk_info[ind].dll_fptr)
		{
			int oldpcount = pcount;
			cc_to_cfs();
			(*(blk_info[ind].dll_fptr))(this, get_pc());
			cfs_to_cc();
#ifdef SINGLESTEP //debug
			pcount--;
#endif
			increment_compile_count(oldpcount - pcount);
		}
		else 
		{
			if (blk_info[ind].count > threshold)
			{
				if (blk_info[ind].count != -1u) 
				{
					req_num++;
					build_andor_load_lib(ind);
#ifdef SINGLESTEP
					goto L1;
#endif
					return;
				}
			}
			else
				blk_info[ind].count++;

			if (ptr)
				DIRECT_READ_WORD(ptr,inst);
			else
				mem->read_word(phy_addr, &inst);

			execute_system(inst, get_pc()); 
			pcount--;
		}
	}


	/* if invalidation occurs */
	if (inval_flag)
	{
		unload_lib(inval_addr >> shiftval);
		if (showmsg) fprintf(stderr, "invalidate %x\n", inval_addr);
		inval_flag = false;
	}
}

void dyn_arm_emulator::step_user()
{
	arm_inst_t  inst;
	unsigned ind;

	ind = get_pc() >> shiftval;

	if (blk_info[ind].dll_fptr)
	{
		cc_to_cfs();
		while (blk_info[ind].dll_fptr && status==ST_RUNNING)
		{
			(*(blk_info[ind].dll_fptr))(this, get_pc());
			ind = get_pc() >> shiftval;
		}
		cfs_to_cc();
		return;
	}
	else if(blk_info[ind].count > threshold) 
	{
		if (blk_info[ind].count != -1u) 
		{
			req_num++;
			build_andor_load_lib(ind);
			return;
		}
	}
	else
		blk_info[ind].count++;

	inst = fetch_inst_user(get_pc());
	execute_user(inst, get_pc()); 
	icount++;
}

uint64_t dyn_arm_emulator::run_system()
{
	icount = 0;
	ccount = 0;
	pcount = prescale;
	status = ST_RUNNING;

	while (status==ST_RUNNING)
		step_system();

	com_done = true;
	pthread_cond_broadcast(&pid_cond);

	increment_interpret_count(prescale - pcount);
	icount -= ccount;	// icount includes ccount;
	return icount + ccount;
}

uint64_t dyn_arm_emulator::run_user()
{
	icount = 0;
	ccount = 0;
	status = ST_RUNNING;

	while(status==ST_RUNNING)
		step_user();

	com_done = true;
	pthread_cond_broadcast(&pid_cond);

	return icount + ccount;
}

uint64_t dyn_arm_emulator::run()
{
	max_count_set = (max_count != (uint64_t)-1);

	if (user_level) 
		return run_user();
	else 
		return run_system();
}


/* this is called by the communication thread */
void dyn_arm_emulator::get_block_to_compile(unsigned *dll, unsigned *pblk)
{
	pthread_mutex_lock(&pid_mut);
	while (PID_LIST->empty() && !com_done)
		pthread_cond_wait(&pid_cond, &pid_mut);

	// thr_inds acts as a FIFO, can use stack style by popping the back
	if (!com_done)
	{
		*pblk = PID_LIST->front();
		PID_LIST->pop_front();

		*dll = dll_ind++;
	}
	pthread_mutex_unlock(&pid_mut);
}


unsigned dyn_arm_emulator::lookup_dict(byte_t *buf, unsigned *pcrc)
{
	/* compute checksum first */
	unsigned crc = 0;
	for (unsigned curpc = 0; curpc < (1u << shiftval); curpc+=4)
	{
		target_inst_t inst;
		DIRECT_READ_WORD(buf + curpc, inst);
		crc += inst;
	}

	if (pcrc) *pcrc = crc;

	char fname[1024];


	/* need to lock this region since temp_buf2 is shared. Also
	 * DLL_DICT may not be thread safe, some one may be updating it */
	pthread_mutex_lock(&srv_mut);

	/* we compute check sum first to see if this is in cache */
	if (DLL_DICT->find(crc) != DLL_DICT->end())
	{
		pair<multimap<unsigned, unsigned>::iterator,
			 multimap<unsigned, unsigned>::iterator> ii;

		ii = DLL_DICT->equal_range(crc);

		/* lets compare the content of the files */
		multimap<unsigned, unsigned>::iterator nit;
		for (nit=ii.first; nit!=ii.second; nit++)
		{
			sprintf(fname,"%s/Xcompiled_%u.dat", cache_dir, (*nit).second);
			FILE *fp = fopen(fname, "rb");
			if (fp == NULL) /* possibly corrupted cache */
			{
				DLL_DICT->erase(nit);
				continue;
			}

			/* something is wrong */
			if (fread(temp_buf2, 1, 1 << shiftval, fp) != (1u << shiftval))
			{
				DLL_DICT->erase(nit);
				fclose(fp);
			}
			else if (memcmp(buf, temp_buf2, 1 << shiftval) == 0)
			{
				pthread_mutex_unlock(&srv_mut);

				if (showmsg)
					fprintf(stderr, "Found DLL in cache %s.\n", fname);

				hit_num++;

				fclose(fp);
				return (*nit).second;
			}
		}
	}

	pthread_mutex_unlock(&srv_mut);
	return -1u;
}


void dyn_arm_emulator::build_andor_load_lib(unsigned pblk)
{
	/* mark the block as in compilation */
	blk_info[pblk].count = -1u;

	/* remove mmu translation cache entry*/
	mmu->invalidate_write_cache(get_pc());

	/* if threads are ready, send the task for communication threads */
	if (srv_count > 0)
	{
		pthread_mutex_lock(&pid_mut);
		PID_LIST->push_back(pblk);
		pthread_cond_signal(&pid_cond);
		pthread_mutex_unlock(&pid_mut);
		return;
	}


	unsigned crc, dres, ind;

	/* still we need a lock since threads may now start to work after
	 * the main one has runs for a while */
	pthread_mutex_lock(&pid_mut);
	ind = dll_ind++;
	pthread_mutex_unlock(&pid_mut);

	mem->read_block(temp_buf1, pblk << shiftval, 1 << shiftval);

	if ((dres = lookup_dict(temp_buf1, &crc)) != -1u)
	{
		load_lib(pblk, dres);
		return;
	}

	if (compile_block(temp_buf1, ind))
	{

		pthread_mutex_lock(&srv_mut);
		update_dll_dict(ind, crc);
		pthread_mutex_unlock(&srv_mut);
		load_lib(pblk, ind);
	}

}


bool dyn_arm_emulator::compile_block(const byte_t *buf, unsigned ind)
{

	char compilestring[1024];
	FILE* outfile;

	/*create cpps*/
	sprintf(compilestring, "%s/Xcompiled_%u.cpp", cache_dir, ind);
	outfile = fopen(compilestring, "w");
	if(outfile == NULL) {
		fprintf(stderr, "Can't open %s to write", compilestring);
		exit(1);
	}

	fprintf(outfile, "#include <arch_jit.hpp>\n");
	fprintf(outfile, "#include <dyn_arm_emul.hpp>\n");

	if (user_level)
		fprintf(outfile, "typedef arm_emulator arm_emul_t;\n");
	else
		fprintf(outfile, "typedef dyn_arm_emulator arm_emul_t;\n");

	decomp_block(outfile, buf, 1 << shiftval, user_level, ind);

	fclose(outfile);

	sprintf(compilestring, 
		CXX MORE_OPTION " %s/Xcompiled_%u.cpp"
		" -Wl,-soname,libXcompiled_%u.so"
		" -o %s/libXcompiled_%u.so"
		" -I%s/include -DSIMIT_COMPILED_ISS %s",
		cache_dir, ind, ind, cache_dir, ind, PREFIX,
		user_level?"":"-DSIMIT_SYSTEM_LEVEL");

	if (showmsg)
		fprintf(stderr,"%s\n", compilestring);

	if (system(compilestring)==0) {
		if (showmsg)
			fprintf(stderr,"...done!\n");
	}
	else {
		if (showmsg)
			fprintf(stderr,"...failed!\n");

		// something is wrong, stop simulation
		if (status==ST_RUNNING)
			status = ST_ELSE;

		return false;
	}

	sprintf(compilestring,"%s/Xcompiled_%u.dat", cache_dir, ind);
	outfile = fopen(compilestring, "wb");
	if(outfile == NULL) {
		fprintf(stderr, "Can't open %s to write", compilestring);
		exit(1);
	}

	fwrite(buf, 1, 1 << shiftval, outfile);
	fclose(outfile);

	return true;
}


char *emitc_default(char *buf,
	unsigned curpc, target_inst_t inst, unsigned size)
{
	buf += sprintf(buf, "ABORT2(UndefinedInstrV);\n");
	return buf;
}

static inline uint32_t popcount16(uint32_t x)
{
	x = (x & 0x5555) + ((x >> 1) & 0x5555);
	x = (x & 0x3333) + ((x >> 2) & 0x3333);
	x = (x & 0x0F0F) + ((x >> 4) & 0x0F0F);
	x = (x & 0x00FF) + ((x >> 8) & 0x00FF);
	return x;
}

typedef arm_emulator emulator_t;
typedef arm_inst_t target_inst_t;
typedef arm_addr_t target_addr_t;


#include "auto_emitc.h"
#include "arm_gen.hpp"
#include "arm_dec.h"

#define UFLD ((inst>>23)&1)
#define PFLD ((inst>>24)&1)
#define WFLD ((inst>>21)&1)
#define sIGN ((inst>>6)&1)
#define HFLD ((inst>>5)&1)
#define LFLD ((inst>>20)&1)

static void emit_ldm_hack(char *buf, arm_inst_t inst)
{
	int ii, count;
	int pcount = popcount16(inst & 0xFFFF) << 2;

	buf += sprintf(buf,
		"\tuint32_t base_addr;\n"
		"\tuint32_t address;\n"
		"\tuint32_t start_addr;\n"
		"\tuint32_t val32;\n"
		"\tuint32_t cnt;\n"
		"\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\tstatic word_t ind[] = {");
	for (ii=0, count=0; ii<16; ii++) {
		if (inst&(1<<ii)) {
			buf += sprintf(buf, "%d, ", ii);
		}
	}
	buf += sprintf(buf, "};\n");

	buf += sprintf(buf,
		"\tbase_addr = READ_REG(%d);\n", RNFLD);

	if (!PFLD && UFLD) {
		buf += sprintf(buf,
			"\tstart_addr = READ_REG(%d);\n", RNFLD);
		if (WFLD)
			buf += sprintf(buf,
			"\tWRITE_REG(%d, READ_REG(%d) + %d);\n", RNFLD, RNFLD, pcount);
	}
	else if (PFLD && UFLD) {
		buf += sprintf(buf,
			"\tstart_addr = READ_REG(%d) + 4;\n", RNFLD);
		if (WFLD)
			buf += sprintf(buf,
			"\tWRITE_REG(%d, READ_REG(%d) + %d);\n", RNFLD, RNFLD, pcount);
	}
	else if (!PFLD && !UFLD) {
		buf += sprintf(buf,
			"\tstart_addr = READ_REG(%d) - %d + 4;\n", RNFLD, pcount);
		if (WFLD)
			buf += sprintf(buf,
			"\tWRITE_REG(%d, READ_REG(%d) - %d);\n", RNFLD, RNFLD, pcount); 
	}														 
	else {													
		buf += sprintf(buf,
			"\tstart_addr = READ_REG(%d) - %d;\n", RNFLD, pcount);
		if (WFLD)
			buf += sprintf(buf,							   
			"\tWRITE_REG(%d, READ_REG(%d) - %d);\n", RNFLD, RNFLD, pcount); 
	}
															  
	//if (BITn(inst, 15) & BITn(inst, 22))					  
	//	buf += sprintf(buf, "\tWRITE_CPSR(SPSR);\n");		 
															  
	/*ignore the last two bits of the address*/			   
	buf += sprintf(buf, "\taddress = start_addr & 0xfffffffc;\n");	  

	buf += sprintf(buf,
	"\tfor (cnt = 0; cnt < %d; cnt++)\n"
	"\t{\n"
	"\t\tMMU_READ_WORD(address, &val32);\n"
	"\t\tif (fault)\n"
	"\t\t{\n"
	"\t\t\tWRITE_REG(%d, base_addr);\n"
	"\t\t\tXSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN, address);\n"
	"\t\t\tABORT( DataAbortV );\n"
	"\t\t}\n"
	"\t\tWRITE_REG(ind[cnt], val32);\n"
	"\t\taddress += 4;\n"
	"\t}\n", pcount >> 2, RNFLD);

}

static void emit_stm_hack(char *buf, arm_inst_t inst)
{
	int ii, count;
	int pcount = popcount16(inst & 0xFFFF) << 2;

	buf += sprintf(buf,
		"\tuint32_t base_addr;\n"
		"\tuint32_t address;\n"
		"\tuint32_t start_addr;\n"
		"\tuint32_t val32;\n"
		"\tuint32_t cnt;\n"
		"\tmmu_fault_t fault;\n");

	buf += sprintf(buf, "\tstatic word_t ind[] = {");
	for (ii=0, count=0; ii<16; ii++) {
		if (inst&(1<<ii)) {
			buf += sprintf(buf, "%d, ", ii);
		}
	}
	buf += sprintf(buf, "};\n");

	buf += sprintf(buf,
		"\tbase_addr = READ_REG(%d);\n", RNFLD);

	if (!PFLD && UFLD) {
		buf += sprintf(buf,
			"\tstart_addr = READ_REG(%d);\n", RNFLD);
		if (WFLD)
			buf += sprintf(buf,
			"\tWRITE_REG(%d, READ_REG(%d) + %d);\n", RNFLD, RNFLD, pcount);
	}
	else if (PFLD && UFLD) {
		buf += sprintf(buf,
			"\tstart_addr = READ_REG(%d) + 4;\n", RNFLD);
		if (WFLD)
			buf += sprintf(buf,
			"\tWRITE_REG(%d, READ_REG(%d) + %d);\n", RNFLD, RNFLD, pcount);
	}
	else if (!PFLD && !UFLD) {
		buf += sprintf(buf,
			"\tstart_addr = READ_REG(%d) - %d + 4;\n", RNFLD, pcount);
		if (WFLD)
			buf += sprintf(buf,
			"\tWRITE_REG(%d, READ_REG(%d) - %d);\n", RNFLD, RNFLD, pcount); 
	}														 
	else {
		buf += sprintf(buf,
			"\tstart_addr = READ_REG(%d) - %d;\n", RNFLD, pcount);
		if (WFLD)
			buf += sprintf(buf,
			"\tWRITE_REG(%d, READ_REG(%d) - %d);\n", RNFLD, RNFLD, pcount); 
	}

	//if (BITn(inst, 15) & BITn(inst, 22))
	//	buf += sprintf(buf, "\tWRITE_CPSR(SPSR);\n");		 
															  
	/*ignore the last two bits of the address*/
	buf += sprintf(buf, "\taddress = start_addr & 0xfffffffc;\n");	  

	/* translate the address */
	buf += sprintf(buf,
	"\tfor (cnt = 0; cnt < %d; cnt++)\n"
	"\t{\n"
	"\t\tMMU_WRITE_WORD(address, READ_REG(ind[cnt]));\n"
	"\t\tif (fault)\n"
	"\t\t{\n"
	"\t\t\tWRITE_REG(%d, base_addr);\n"
	"\t\t\tXSCALE_UPDATE_FSR_FAR(ARMul_CP15_R5_ST_ALIGN, address);\n"
	"\t\t\tABORT( DataAbortV );\n"
	"\t\t}\n"
	"\t\taddress += 4;\n"
	"\t}\n", pcount >> 2, RNFLD);

}

/* return the CRC of the block */
void simit::decomp_block(FILE *outfile, const byte_t *pblk,
	unsigned size, bool user_level, unsigned ind)
{
	char buf[4096];

	/*print preamble of a function*/
	fprintf(outfile,
	"extern \"C\" void compiled_%u(arm_emul_t *emu, "
		"arm_inst_t _start_addr) {\n", ind);

#if 0
	fprintf(outfile, "\tif (!(_start_addr >= 0x%x && _start_addr < 0x%x)) {\n"
			"\t\tfprintf(stderr, \"pc=%%x\\n\", _start_addr);\n\t}\n", 
			fstart, fend);
	fprintf(outfile, "\tassert(_start_addr >= 0x%x && _start_addr < 0x%x);\n",
			fstart, fend);

	fprintf(outfile, "#undef CISS_FSTART\n");
	fprintf(outfile, "#undef CISS_FEND\n");
	fprintf(outfile, "#define CISS_FSTART 0x%x\n", fstart);
	fprintf(outfile, "#define CISS_FEND 0x%x\n", fend);
#endif

	/*define variables*/
	fprintf(outfile,
		"word_t _virt_blk_ind;\n"
		"word_t _lpc;\n");
	
	/*label table*/
	fprintf(outfile,
		"static void *labels[%u] = {\n\t&&L%x", size/4, 0);

	for (target_addr_t kk = 4; kk < size; kk += 4) {
		fprintf(outfile, ", &&L%x", kk);
		if ((kk)%16==0) fprintf(outfile, "\n\t");
	}
   
	fprintf(outfile, "};\n");

	fprintf(outfile, "#define OFFSET_MASK 0x%x\n", (size - 1));
	fprintf(outfile, "#define INDEX_MASK 0x%x\n", ~(size - 1));
	
	//fprintf(outfile,
	//	"#undef _virt_blk_ind\n"
	//	"#define _virt_blk_ind 0x%x\n", fstart);

	/*goto start*/
	fprintf(outfile,
		"_virt_blk_ind = _start_addr & INDEX_MASK;\n"
		"_lpc = (_start_addr & OFFSET_MASK) >> 2;\n"
		"goto *labels[_lpc];\n");

	for (unsigned offset_pc = 0; offset_pc < size; offset_pc+=4 ) {

		/*labels*/
		fprintf(outfile, "L%x:\n", offset_pc);

		target_inst_t inst;

		DIRECT_READ_WORD(pblk + offset_pc, inst);

		/* if this is nop */
		if ((inst << 4) == 0 || inst == 0xe1a00000) 
		{
#ifdef SINGLESTEP // DEBUG
			fprintf(outfile, "  LONG_JUMP(%uU, %uU);\n",
					offset_pc, offset_pc+4);
			fprintf(outfile, "  return;\n");
#endif
			continue;
		}
		
		fprintf(outfile, "#undef CISS_PC\n");
		fprintf(outfile, "#define CISS_PC 0x%x\n", offset_pc);
		fprintf(outfile, "#undef CISS_INST\n");
		fprintf(outfile, "#define CISS_INST 0x%x\n", inst);

		/* deal with ldm1/stm1 only, for now */
		if ((inst & 0xE400000) == 0x08000000) {
			if (BITn(inst, 20))
				emit_ldm_hack(buf, inst);
			else
				emit_stm_hack(buf, inst);
		}
		else
			emit_code(buf, offset_pc, inst, size);

		/* predicates */
		switch (COND) {
			case 0: //EQ
				fprintf(outfile, "  if (Z_FLAG) {\n");
				break;
			case 1:	//NE
				fprintf(outfile, "  if (!Z_FLAG) {\n");
				break;
			case 2: //CS
				fprintf(outfile, "  if (C_FLAG) {\n");
				break;
			case 3: //CC
				fprintf(outfile, "  if (!C_FLAG) {\n");
				break;
			case 4: //MI
				fprintf(outfile, "  if (N_FLAG) {\n");
				break;
			case 5: //PL
				fprintf(outfile, "  if (!N_FLAG) {\n");
				break;
			case 6: //VS
				fprintf(outfile, "  if (V_FLAG) {\n");
				break;
			case 7: //VC
				fprintf(outfile, "  if (!V_FLAG) {\n");
				break;
			case 8: //HI
				fprintf(outfile, "  if (C_FLAG & !Z_FLAG) {\n");
				break;
			case 9: //LS
				fprintf(outfile, "  if (!C_FLAG | Z_FLAG) {\n");
				break;
			case 10: //GE
				fprintf(outfile, "  if (N_FLAG==V_FLAG) {\n");
				break;
			case 11: //LT
				fprintf(outfile, "  if (N_FLAG!=V_FLAG) {\n");
				break;
			case 12: //GT
				fprintf(outfile, "  if (!(Z_FLAG | (N_FLAG^V_FLAG))) {\n");
				break;
			case 13: //LE
				fprintf(outfile, "  if (Z_FLAG | (N_FLAG^V_FLAG)) {\n");
				break;
			case 14: //AL
				fprintf(outfile, "  {\n");
				break;
			case 15: //NV
			default:
				break;
		}

		if (COND!=15) {
			fprintf(outfile, "%s", buf);
			fprintf(outfile, "  }\n");
		}
#ifdef SINGLESTEP // DEBUG
		fprintf(outfile, "  LONG_JUMP(%uU, %uU);\n", offset_pc, offset_pc+4);
		fprintf(outfile, "  return;\n");
#endif
		/* end of instruction */
	}

	/* print the tail of a function 
	   print CISS_PC again in case the last instruction is a nop
	*/
	fprintf(outfile, "#undef CISS_PC\n");
	fprintf(outfile, "#define CISS_PC 0x%x\n", size-4);
	fprintf(outfile, "LONG_JUMP(%uU, %uU);\n}\n", size-4, size);
}

bool dyn_arm_emulator::ld_lib(unsigned ind)
{
	char ldstring[1024];
	sprintf(ldstring,
		CXX MORE_OPTION " %s/Xcompiled_%u.o"
		" -o %s/libXcompiled_%u.so",
		cache_dir, ind, cache_dir, ind);

	if (showmsg)
		fprintf(stderr, "%s\n", ldstring);

	if (system(ldstring)==0) {
		if (showmsg)
			fprintf(stderr, "...done!\n");
		return true;
	}
	if (showmsg)
		fprintf(stderr, "...failed!\n");
	return false;
}


bool dyn_arm_emulator::read_or_create_dll_dict()
{
	char l1dir[1024];
	char l2dir[1024];
	bool ret = false;

	strcpy(l1dir, getenv("HOME"));
	strcat(l1dir, "/.ema/");

	if (!access(l1dir, W_OK) || !mkdir(l1dir, 0770))
	{
		sprintf(l2dir, "%s%c%d/", l1dir,
			user_level?'u':'s', 1 << (shiftval - 2));
		if (!access(l2dir, W_OK) || !mkdir(l2dir, 0770))
		{
			cache_dir = strdup(l2dir);
			ret = true;
		}
	}

	if (ret)
	{
		sprintf(l2dir, "%s/dict.dat", cache_dir);
		if (!access(l2dir, R_OK | W_OK))
		{
			read_dll_dict();
		}
		else
		{
			int fd = creat(l2dir, 0660);
			if (fd==-1)
				ret = false;
			else
				close(fd);
		}
	}
	
	if (!ret)
		fprintf(stderr,
			"Cannot create dictionary to insufficient permission!\n");

	return ret;
}

void dyn_arm_emulator::read_dll_dict()
{
	FILE* datin;
	char fullpath[1024];
	unsigned val1, val2;

	/* clear the dictionary */
	DLL_DICT->clear();

	sprintf(fullpath,"%s/dict.dat",cache_dir);
	datin = fopen(fullpath, "r");
	if(datin == NULL)
	{
		fprintf(stderr, "Can't open dictionary at %s to read", fullpath);
		return;
	}

	/* lines contain dll_index crc */
	while(fscanf(datin, "%u %u", &val1, &val2) != EOF)
	{
		DLL_DICT->insert(pair<unsigned, unsigned>(val1, val2));
		if (val2 >= dll_ind) dll_ind = val2 + 1; 
	}

	fclose(datin);
}

void dyn_arm_emulator::update_dll_dict(unsigned ind, unsigned crc)
{
	FILE *outfile;

	/*addr backup data*/
	char fullpath[1024];
	sprintf(fullpath,"%s/dict.dat",cache_dir);
	outfile = fopen(fullpath, "a");
	if(outfile == NULL)
	{
		fprintf(stderr, "Can't open dictionary at %s to append", fullpath);
		return;
	}

	fprintf(outfile, "%u %u\n", crc, ind); 
	fclose(outfile);
}


bool dyn_arm_emulator::load_lib(unsigned pblk, unsigned ind)
{
	char* error;
	char filename[1024];
	char funname[1024];

	sprintf(filename, "%s/libXcompiled_%u.so", cache_dir, ind);
	sprintf(funname, "compiled_%u", ind);

	if (showmsg)
		fprintf(stderr, "Loading %d for address %x.\n", ind, pblk << shiftval);

	/* not sure if the many libc functions below are thread-safe.
	 * to save the trouble, use the big lock, many compilation
	 * threads can call load_lib, so need to safe guard
	 */
	pthread_mutex_lock(&srv_mut);

	/* in the parallel version, load_lib is called by comm. threads,
	 * need to guard with mutex since the main thread may call unload_lib
	 * at the same time
	 * if the program is not yet running, then it must be setting up cache
	 */
	if (blk_info[pblk].count != -1u && status != ST_RESET) {
		pthread_mutex_unlock(&srv_mut);
		return false;
	}

	/* check if it is already loaded */
	map<unsigned, dll_info_t *>::iterator mit = DLL_INFO2->find(ind);
	if (mit != DLL_INFO2->end())
	{
		(*mit).second->rcount++;
		blk_info[pblk].dll_fptr = (*mit).second->dll_fptr;
		blk_info[pblk].count = 0;
		pthread_mutex_unlock(&srv_mut);
		return true;
	}

	void *handle = dlopen(filename, RTLD_LAZY);
	if (!handle) {
		fputs(dlerror(), stderr);
		pthread_mutex_unlock(&srv_mut);
		return false;
	}

	fptr_t fptr = (fptr_t)dlsym(handle, funname);
	if ((error = dlerror())!= NULL) {
		fputs(error, stderr);								
		pthread_mutex_unlock(&srv_mut);
		return false;
	}

	dll_info_t *ndll = (dll_info_t *)malloc(sizeof(dll_info_t));
	if (ndll==NULL) {
		fputs("Out of memory\n", stderr);
		exit(1);
	}

	ndll->rcount = 1;
	ndll->handle = handle;
	ndll->index  = ind;
	ndll->dll_fptr = fptr;

	DLL_INFO->insert(pair<fptr_t, dll_info_t *>(fptr, ndll));
	DLL_INFO2->insert(pair<unsigned, dll_info_t *>(ind, ndll));

	blk_info[pblk].dll_fptr = fptr;

	/* clear the count so whatever happens, it can still be interpreted */
	blk_info[pblk].count = 0;

	pthread_mutex_unlock(&srv_mut);
	return true;
}

bool dyn_arm_emulator::unload_lib(unsigned pblk)
{
	/* clear the block */
	pthread_mutex_lock(&srv_mut);

	/* the block is still in compilation */
	if (blk_info[pblk].count == -1u)
	{
		blk_info[pblk].count = 0;
		pthread_mutex_unlock(&srv_mut);
		return true;
	}

	map<fptr_t, dll_info_t *>::iterator mit =
		DLL_INFO->find(blk_info[pblk].dll_fptr);

	assert(mit != (DLL_INFO->end()));

	/* decrement the reference count */
	if (--((*mit).second->rcount) == 0)
	{
		dlclose((*mit).second->handle);
		DLL_INFO2->erase((*mit).second->index);
		free((*mit).second);
		DLL_INFO->erase(mit);
	}

	blk_info[pblk].dll_fptr = NULL;
	blk_info[pblk].count = 0;
	pthread_mutex_unlock(&srv_mut);

	return true;
}


/************************************************************************
	convenient routines for socket communication
************************************************************************/

/* send nbytes to socket */
static ssize_t sendall(int fd, const void *buf, size_t nbyte)
{
	ssize_t nwrite = 0, n;

	do
	{
		if ((n = send(fd, (const char *)buf + nwrite, nbyte - nwrite, 0)) ==
			(ssize_t)-1)
		{
			if (errno == EINTR)
				continue;
			else
				return -1;
		}
		nwrite += n;
	}
	while (nwrite < (ssize_t)nbyte);

	return nwrite;
}

/* receive up to nbytes from socket */
static ssize_t recvall(int fd, void *buf, size_t nbyte)
{
	ssize_t nread = 0, n;

	do
	{
		if ((n = recv(fd, (char *)buf + nread, nbyte - nread, 0)) ==
			(ssize_t)-1)
		{
			if (errno == EINTR)
				continue;
			else
				return -1;
		}
		if (n == 0)
			return nread;
		nread += n;
	}
	while (nread < (ssize_t)nbyte);

	return nread;
}

void dyn_arm_emulator::com_thread()
{
	int my_index = SIMIT_MAX_SERVER;
 
	unsigned bufsize = 1 << shiftval;
	byte_t *buf = (byte_t *)malloc(bufsize);
	if (buf == NULL)
	{
		fprintf(stderr, "Error: Insufficient memory, thread terminates\n");
		return;
	}

	pthread_mutex_lock(&srv_mut);
	/* find one spot that is not in use */
	for (int ii=0; ii<SIMIT_MAX_SERVER; ii++)
	{
		if (srv_dir[ii].buf == NULL) 
		{
			my_index = ii;
			srv_dir[ii].buf = buf;
			/* don't increment srv_count yet since connection may fail */
			break;
		}
	}
	pthread_mutex_unlock(&srv_mut);

	/* too many threads, silently quit */
	if (my_index == SIMIT_MAX_SERVER) return;


	struct sockaddr_in address;
	int my_socket;

	if ((my_socket = socket(AF_INET,SOCK_STREAM,0))>0)
		if (showmsg)
		 fprintf(stderr, "The socket was created %d\n",my_socket);

 
	address.sin_family= AF_INET;
	address.sin_port = htons(srv_dir[my_index].portno);
	inet_pton(AF_INET, srv_dir[my_index].ipaddr, &address.sin_addr);

	if (showmsg)
		fprintf(stderr, "Trying to connect with server %s:%d...\n",
			srv_dir[my_index].ipaddr, srv_dir[my_index].portno);

	if (connect(my_socket, (struct sockaddr*)&address, sizeof(address))==0) {
		if (showmsg)
			fprintf(stderr, "Connection was accepted by server %s:%d.\n",
				srv_dir[my_index].ipaddr, srv_dir[my_index].portno);
	}
	else {
		fprintf(stderr, "Connection was rejected by server %s:%d.\n",
			srv_dir[my_index].ipaddr, srv_dir[my_index].portno);
		pthread_exit(NULL);
	}

	srv_dir[my_index].socket = my_socket;
	pthread_mutex_lock(&srv_mut);
	srv_count++;
	pthread_mutex_unlock(&srv_mut);

	while (true)
	{
		unsigned ind; // DLL number
		unsigned pblk; // physical address index

		get_block_to_compile(&ind, &pblk);

		// simulation has finished?
		if (com_done) break;

		if (showmsg)
			fprintf(stderr,"start to compile ...\n");	

		mem->read_block(buf, pblk << shiftval, bufsize);

		unsigned crc;
		unsigned dres = lookup_dict(buf, &crc);

		if (dres != -1u)
		{
			load_lib(pblk, dres);
			continue;
		}

		if (showmsg)
			fprintf(stderr, "sending the bufsize %d...\n", bufsize);

		/* unrecoverable connection error */
		if (sendall(my_socket, &bufsize, sizeof(bufsize)) < 0) break;

		if (showmsg)
			fprintf(stderr, "sending the dll_ind %d...\n", ind);

		if (sendall(my_socket, &ind, sizeof(ind)) < 0) break;

		if (showmsg)
			fprintf(stderr, "sending the user_level %d...\n", user_level);

		if (sendall(my_socket, &user_level, sizeof(user_level)) < 0) break;

		if (showmsg)
			fprintf(stderr, "sending block of size %d...\n", bufsize);

		if (sendall(my_socket, buf, bufsize) < 0) break;

		int objsize, islinked;

		if (recvall(my_socket, &objsize, sizeof(int)) < 0) break;
		if (recvall(my_socket, &islinked, sizeof(int)) < 0) break;

		/* 0 means error, but still connected */
		if (objsize == 0)
		{
			if (showmsg)
				fprintf(stderr, "server error encountered...\n");
			continue;
		}
			
		srv_dir[my_index].count++;

		char pkt[1024];
		char fname[1024];

		if (islinked)
			sprintf(fname, "%s/libXcompiled_%u.so", cache_dir, ind);
		else
			sprintf(fname, "%s/Xcompiled_%u.o", cache_dir, ind);

		FILE* fp = fopen(fname, "w");
		int rem = objsize, recd = 1;
		while (recd > 0 && rem > 0)
		{
			recd = recvall(my_socket, pkt, rem<1024?rem:1024);
			if (recd > 0)
			{
				fwrite(pkt, 1, recd, fp);
				rem -= recd;
			}
		}
		fclose(fp);

		if (recd < 0) break;

		if (showmsg)
			fprintf(stderr,"%d Compiled code received of size %d\n",
				my_index, objsize);

		sprintf(fname, "%s/Xcompiled_%u.dat", cache_dir, ind);
		fp = fopen(fname, "wb");
		fwrite(buf, 1, 1 << shiftval, fp);
		fclose(fp);

		/* if code is not linked, link it */
		if (islinked || ld_lib(ind))
		{
			pthread_mutex_lock(&srv_mut);
			update_dll_dict(ind, crc);
			pthread_mutex_unlock(&srv_mut);
			if (!com_done && load_lib(pblk, ind))
			{
				if (showmsg)
					fprintf(stderr, "File successfully loaded...\n");
			}
		}
	}

	if (!com_done)
	{
		srv_count--;
 		if (showmsg)
			fprintf(stderr, "Connection reset, thread %d exits.\n", my_index);
	}

	pthread_exit(NULL);
}

/* com_thread2 are local threads, we don't cancel, but join them on exit*/
void dyn_arm_emulator::com_thread2()
{
	int my_index = SIMIT_MAX_SERVER;
 
	unsigned bufsize = 1 << shiftval;
	byte_t *buf = (byte_t *)malloc(bufsize);
	if (buf == NULL)
	{
		fprintf(stderr, "Error: Insufficient memory, thread terminates\n");
		return;
	}

	pthread_mutex_lock(&srv_mut);
	/* find one spot that is not in use */
	for (int ii=0; ii<SIMIT_MAX_SERVER; ii++)
	{
		/* donot use entries reserved for remote server */
		if (srv_dir[ii].buf == NULL && srv_dir[ii].ipaddr[0] == 0) 
		{
			my_index = ii;
			srv_dir[ii].buf = buf;
			srv_count++;
			break;
		}
	}
	pthread_mutex_unlock(&srv_mut);

	/* too many threads, silently quit */
	if (my_index == SIMIT_MAX_SERVER) return;


	while (true)
	{
		unsigned ind; // DLL number
		unsigned pblk; // physical address index

		get_block_to_compile(&ind, &pblk);
		if (com_done) pthread_exit(NULL);

		if (showmsg)
			fprintf(stderr,"start to compile ...\n");	

		mem->read_block(buf, pblk << shiftval, bufsize);

		unsigned crc;
		unsigned dres = lookup_dict(buf, &crc);

		if (dres != -1u)
		{
			load_lib(pblk, dres);
		}
		else if (compile_block(buf, ind))
		{
			srv_dir[my_index].count++;

			pthread_mutex_lock(&srv_mut);
			update_dll_dict(ind, crc);
			pthread_mutex_unlock(&srv_mut);
			if (!com_done && load_lib(pblk, ind))
			{
				if (showmsg)
					fprintf(stderr, "File successfully loaded...\n");
			}
		}
	}
}


void dyn_arm_emulator::com_cleanup()
{
	for (int ii=0; ii<SIMIT_MAX_SERVER; ii++)
	{
		if (srv_dir[ii].buf) 
		{
			free(srv_dir[ii].buf);
			srv_dir[ii].buf = NULL;
		}
	}
}


/* this function reads the config file and populates the server database*/
int dyn_arm_emulator::read_servers(const char* fname)
{
	char buf1[64];
	int  ii = 0;
	
	if (fname==NULL) return 0;

	FILE* fp = fopen(fname,"r");
	if (!fp)
	{
		fprintf(stderr, "Could not open config file %s\n", fname);
		return 0;
	}

	// donot use scanf %s since it is not safe
	while (fgets(buf1, 64, fp)!=NULL)
	{
		char *ptr;
		if ((ptr = strtok(buf1, "\n\t "))==NULL) return ii;
		strncpy(srv_dir[ii].ipaddr, ptr, 63);

		if ((ptr = strtok(NULL, "\n\t "))==NULL) return ii;
		srv_dir[ii].portno = atoi(ptr);

		/* read as many as I can remember. */
		if (++ii==SIMIT_MAX_SERVER) break;
	}

	fclose(fp);
	return ii;
}



#include "read_elf.h"

bool dyn_arm_emulator::setup_cache(const char *filename)
{
	Elf32_Ehdr *hdr;
	Elf32_Phdr *phdr;
	Elf32_Shdr *shdr;
	char *string_table;
	Elf32_Shdr *shdr_new_section;
	Elf32_Word new_section_size, new_section_type, new_section_flags;
	Elf32_Addr new_section_addr;
	FILE *fobj;
	int i;

	if (!read_or_create_dll_dict()) return false;

	if (filename == NULL) return true;

	fobj = fopen(filename, "rb");
	if(fobj == NULL) {
		fprintf(stderr, "Can't open executable: %s\n", filename);
		exit(1);
	}

	hdr = ReadElfHeader(fobj);

	if(hdr == NULL) {
		fprintf(stderr, "Could not read ELF32 header from file: %s.\n",
			   	filename);
		exit(1);
	}

#ifndef EM_ARM
#define EM_ARM 40
#endif
	/* check if the file is for ARM */
	if (hdr->e_type != ET_EXEC ||
		hdr->e_machine != EM_ARM ||
		hdr->e_ident[EI_DATA] != ELFDATA2LSB) {
		fprintf(stderr, "File is not ARM LSB executable: %s.\n", filename);
		exit(1);
	}

	phdr = ReadProgramHeaders(hdr, fobj);

	for(i=0; i<hdr->e_phnum; i++) {
		if ( (phdr[i].p_type == PT_LOAD) /* Loadable Program Segment */ &&
		 ((phdr->p_flags & PF_X) != 0 /* not text segment => data segment */)) {
		data_base = phdr[i].p_vaddr;
		data_size = phdr[i].p_memsz;
		}
	}
	prog_base = hdr->e_entry;

	shdr = ReadSectionHeaders(hdr, fobj);
	
	if(shdr == NULL) {
		fprintf(stderr, "Can't read section headers from executable\n");
		exit(1);
	}
	string_table = LoadStringTable(hdr, shdr, fobj);

	for(i = 0; i < hdr->e_shnum; i++)
	{
		shdr_new_section = &shdr[i];

		new_section_type = GetSectionType(shdr_new_section);
		new_section_flags = GetSectionFlags(shdr_new_section);
		if ((new_section_type == SHT_PROGBITS) &&
			(new_section_flags & (SHF_EXECINSTR | SHF_ALLOC)))
		{
			new_section_size =
				shdr_new_section ? GetSectionSize(shdr_new_section) : 0;
			new_section_addr = GetSectionAddr(shdr_new_section);

			arm_addr_t new_section_end = new_section_addr + new_section_size;
			while (new_section_addr < new_section_end)
			{
				/* align memory address */
				arm_addr_t fstart = (new_section_addr >> shiftval) << shiftval;

				mem->read_block(temp_buf1, fstart, 1 << shiftval);

				/* if not loaded yet */
				if (blk_info[fstart >> shiftval].dll_fptr == NULL)
				{

					unsigned dres = lookup_dict(temp_buf1);

					if (dres != -1u)
					{
						req_num++;
						load_lib(fstart >> shiftval, dres);
					}
				}

				new_section_addr = fstart + (1 << shiftval);
			}
		}
	}
	free(string_table);
	free(phdr);
	free(shdr);
	free(hdr);

	fclose(fobj);

	return true;
}

