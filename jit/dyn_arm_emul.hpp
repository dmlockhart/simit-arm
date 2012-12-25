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
#ifndef SIMIT_DYN_ARM_EMUL_H
#define SIMIT_DYN_ARM_EMUL_H

#include <pthread.h>
#include <armemul.h>

/* forward declaration so that do not need to include list */
namespace simit {

#define SIMIT_MAX_SERVER 32

class dyn_arm_emulator: public arm_emulator {


  public:

	/* constructor
	 * verbose    -- print simulation messages
	 * showmsg    -- print compilation messages
	 * user_level -- perform user level simulation
	 * threshold  -- threshold to trigger compilation
	 * blkbits    -- log2 of block byte size 
	 */
	dyn_arm_emulator(bool verbose, bool showmsg, bool user_level,
			uint32_t threshold, uint32_t blkbits);

	/* destructor */
	~dyn_arm_emulator();

	/* to run a program
	 */
	uint64_t run();

	/********************************************************************
	  Communication thread to interact with servers
	********************************************************************/

	/* the thread function, using socket */
	void com_thread();

	/* the thread function, using another pthread/processor */
	void com_thread2();

	/* the thread function, using another pthread/processor */
	void com_cleanup();

	/* read the sever configuration file */
	int read_servers(const char* fname);

	/* get the number of connected servers */
	int get_server_count() const {return srv_count;}

	/* get the number of compiled files by a server */
	int get_server_compile_count(int ind) const {return srv_dir[ind].count;}



	/********************************************************************
	  Some methods to be called by compiled code
	********************************************************************/

	/* check if should return from compiled code due to signal or I/O */
	bool check_return() const {return (pcount<=0) || SigSet;}


	/* check whether write invalidates compiled region
	 * pa -- physical address for system-level, virtual address for user level
	 */
	bool check_write(arm_addr_t addr) const
	{
		unsigned ind = addr >> shiftval;
		return blk_info[ind].dll_fptr || blk_info[ind].count == -1u;
	}

	bool check_write_range(arm_addr_t addr, unsigned size) const
	{
		unsigned ind = addr >> shiftval, end = (addr + size) >> shiftval;

		for (; ind < end; ind++)
			if (blk_info[ind].dll_fptr || blk_info[ind].count == -1u)
				return true;

		return false;
	}

	/* set the flag to invalidate */
	void set_invalidate_flag(arm_addr_t pa)
	{
		inval_addr = pa;
		inval_flag = true;
	}



	/********************************************************************
	  methods related to compiled simulation
	********************************************************************/


	/* get the total count of compilation requests */
	unsigned get_num_reqs() const {return req_num;}

	/* get the total count of compilation requests that hits in cache */
	unsigned get_num_hits() const {return hit_num;}

	/* load cached DLLs for the program, 
	 * return false on failure to access/create the cache dir
	 */
	bool setup_cache(const char *progname);

  private:

	/********************************************************************
	  methods that shadows those in armemul, not very OOP
	********************************************************************/

	void execute_user(arm_inst_t inst, arm_addr_t);
	void execute_system(arm_inst_t inst, arm_addr_t);

	void step_user();
	void step_system();

	uint64_t run_user();
	uint64_t run_system();


	/* pull from the compilation queue a blk to compile, blocking */
	void get_block_to_compile(unsigned *dll_index, unsigned *pblk);

	/* read dict.dat file, or create one if none exists */
	bool read_or_create_dll_dict();

	/* look up dictionary (cache) to see if a matching block exists,
	 * return the index of the matching DLL, -1 if none
	 * crc is the set to the checksum
	 */
	unsigned lookup_dict(byte_t *buf, unsigned *crc = NULL);

	/* load a lib if already in dictionary, otherwise build it */
	void build_andor_load_lib(unsigned pblk);

	/* compile a block, called by the above function, and com_thread2 */
	bool compile_block(const byte_t *buf, unsigned ind);

	/* link a library, .o -> .so, ind is the index of the DLL file */
	bool ld_lib(unsigned ind);

	/* load a library file,
	   pblk is the index of the physical block,
	   ind is the index of the file */
	bool load_lib(unsigned pblk, unsigned ind);
	bool unload_lib(unsigned pblk);


	/* read dict.dat file */
	void read_dll_dict();

	/* update the cache with the newly compiled DLL,
	   ind is the index of the file
	   crc is the check sum of the dll
	 */
	void update_dll_dict(unsigned blk, unsigned crc);


	uint32_t threshold;
	uint32_t shiftval;
	const bool showmsg;

	bool inval_flag;
	arm_addr_t inval_addr;


	unsigned req_num;		// total number of requests
	unsigned hit_num;		// total number that hit in the DLL cache
	unsigned dll_ind;		// dll file name index, always grows

	char *cache_dir;	

	/* use void * insteand of std::list so that <list> does not need
	 * to be included here. Including list will slow down the compilation
	 * speed of the compiled functions, I guess. The same reason applies
	 * to the following several void *'s for the dictionary (cache).
	 */
	void *pid_list;			// list of physical indexes to compile


	/* type of the DLLs */
	typedef void (*fptr_t)(arm_emulator *, arm_addr_t);

	typedef struct
	{
		unsigned count;		// access count, -1u means in compilation
		fptr_t dll_fptr;	// pointer to the function
	} blk_info_t;

	blk_info_t *blk_info;


	/************************************************************************
      cache information
	************************************************************************/

	typedef struct
	{
		unsigned rcount;	// reference count
		unsigned index;		// filename index
		void *handle;		// dll handle
		fptr_t dll_fptr;	// pointer to the function
	} dll_info_t;

	void *dll_info;		// map from fptr to dll_info_t, for loaded DLLs
	void *dll_info2;	// map from file index to dll_info_t, for loaded DLLs
	void *dll_dict;		// multimap from CRC to name, for all cached DLLs


	/************************************************************************
      compilation server information
	************************************************************************/

	/* compilation server information */
	int srv_count;
	int srv_index;
	bool com_done;

	struct srv_info_t {
		char ipaddr[64];
		int portno;
		int count;
		int socket;
		byte_t *buf;
	} srv_dir[SIMIT_MAX_SERVER];

	/* pid_mut is used to protect the pid list */
	pthread_mutex_t pid_mut;
	pthread_cond_t  pid_cond;

	/* a big lock to protect everything except the pid list */
	pthread_mutex_t srv_mut;

	/* temporary buffers used by the compilation threads, need protection */
	byte_t *temp_buf1;
	byte_t *temp_buf2;
};
    
/* translate a block, buf is holds the instructions
 * ind is the index of the DLL file.
 * this function is made public so that the translation server
 * and the static compiled simulator builder can use it.
 */
void decomp_block(FILE *outfile, const byte_t *buf,
	unsigned size, bool user_level, unsigned ind);


}


#endif
