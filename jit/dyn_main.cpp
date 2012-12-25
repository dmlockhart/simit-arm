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
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <config.h>

#include "build_dll.hpp"
#include "dyn_arm_emul.hpp"

#if HAVE_SYS_RESOURCE_H && HAVE_SYS_TIME_H
#include <sys/time.h>
#include <sys/resource.h>
#endif

using namespace simit;

static uint32_t ilog2(uint32_t num)
{
	uint32_t power  = 0;
	uint32_t logval = 1;

	while(logval < num)
	{
		logval*=2;
		power++;
	}

	return power;
}

static bool check_install()
{
	FILE *fp;
	fp = fopen(PREFIX "/include/arch_jit.hpp", "rt");
	if (fp == NULL) return false;

	fclose(fp);
	return true;
}

static void usage(char *fname)
{
	char *cp;
	if ((cp = strrchr(fname, '/'))!=NULL) cp++;
	else cp = fname;
	fprintf(stderr,
        "**** " PACKAGE " Version " VERSION
        " (C) 2002-2007 Wei Qin ****\n"
		"usage : %s [-h] [-v] [-m num] [-f fname] [-j num] [-s config]\n"
		"                [<file name> <args...>]\n"
		"  -h : print this message and quit\n"
		"  -v : verbose simulator\n"
		"  -m num : maximum number of instructions to simulate, in million\n"
#if 0 /* the following is hidden, for developer's use only */
		"  -w : verbose communication\n"
#endif
		"  -f fname  : translation server configuration file\n"
		"  -j num    : additional translation threads to create\n"
		"  -s config : system-level simulation, path to configuration file\n"
#if 0 /* the following two are hidden, for developer's use only */
		"  -l num    : instructions per translation block, "
			"default to 256 for system level, 512 for user level\n"
		"  -t num    : translation threshold in million, "
			"default to 8M for system level, 32M for user level\n"
#endif
		"  file name : ARM ELF32 program to simulate\n"
		"  args      : arguments to the program, ignored when -s is given\n"
		"Note: Only one of -s and <file name> should be given.\n", cp);

}

static dyn_arm_emulator *mltr = NULL;

static void sig_handler(int signum)
{
	/* do nothing if the simulator does not exist */
	if (mltr==NULL) exit(0);

	if (signum==SIGINT || signum==SIGTERM) {
		mltr->interrupt();
	}
#if 0
    else if (signum==SIGUSR1) {
        mltr->seg_fault();                                     
                                                              
        fprintf(stderr, "Error accessing memory, address = %08x, code = %d\n",
            mltr->mem->get_last_faddr(), mltr->mem->get_last_fault());
                                                              
        exit(1);                                              
    }
#endif
}

static void setup_signals()
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));                             
	act.sa_handler = sig_handler;                        
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
}

static void clear_signals()
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));                             
	act.sa_handler = SIG_DFL;                        
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
}

void *compile_thread(void *param)
{
	dyn_arm_emulator *emul = (dyn_arm_emulator*)param;

	emul->com_thread();

	return NULL;
}

void *compile_thread2(void *param)
{
	dyn_arm_emulator *emul = (dyn_arm_emulator*)param;

	emul->com_thread2();

	return NULL;
}


int main(int argc, char *argv[], char *envp[])
{
	int i;
	char *bin_name;
	char *prog_name = NULL;
	char *conf_name = NULL;	 // processor configuration file
	char *svr_config = NULL; // server configuration file
	bool verbose = false;
	bool showmsg = false;
	bool user_level = false;

	bool shift_set = false;
	bool threshold_set = false;
	uint32_t shift_val = 8;
	uint32_t threshold = 8<<20;
	uint64_t max_inum = (uint64_t)-1;

	int nserver = 0;	// number of remote compilation severs
	int nthread = 0;	// number of local compilation threads

	for(i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-s") == 0){
			conf_name = argv[++i]; 
		}
		else if(strcmp(argv[i], "-l") == 0) {
			shift_val = ilog2((uint32_t)atof(argv[++i]));
			shift_set = true;
		}
		else if(strcmp(argv[i], "-m") == 0){ 
			max_inum = ato_uint64(argv[++i]);
		}
		else if(strcmp(argv[i], "-t") == 0) {
			threshold = (uint32_t)(atof(argv[++i])*(1<<20));
			threshold_set = true;
		}
		else if(strcmp(argv[i], "-j") == 0) {
			nthread = atoi(argv[++i]);
		}
		else if(strcmp(argv[i], "-v") == 0) {
			verbose = true;
		}
		else if(strcmp(argv[i], "-w") == 0) {
			showmsg = true;
		}
		else if(strcmp(argv[i], "-f") == 0) {
			svr_config = argv[++i];
		}
		else if(strcmp(argv[i], "-h") == 0 || argv[i][0]=='-') {
			usage(argv[0]); return 0;
		}
		else if(!prog_name) {
			prog_name = argv[i];
			break;
		}
	}

	user_level = conf_name==NULL;

	if (!shift_set)
		shift_val = user_level?9:8;
	else if (shift_val<7)
	/* 2^7 = 128 instructions/blk is the smallest size
	   otherwise the block info table will be too big */
		shift_val = 7;

	if (!threshold_set)
		threshold = user_level?16<<20:8<<20;

	if ((bin_name = strrchr(argv[0], '/'))!=NULL) bin_name++;
	else bin_name = argv[0];

	/* emulator instance */
	if ((user_level && prog_name!=NULL) || (!user_level && prog_name==NULL))
	{

		if (!check_install())
		{
			fprintf(stderr, "Error: cannot find header files. "
							"Did you \"make install\"?\n");
			return 1;
		}

		mltr = new dyn_arm_emulator(verbose, showmsg, user_level,
				threshold, shift_val+2);

		/* problem loading configuration file */
		if (!user_level)
		{
			if (!mltr->load_config(conf_name))
			{
				delete mltr;
				return 1;
			}
			if (!mltr->is_pc_set())
				fprintf(stderr, "Warning: PC not given, starting from 0x0.\n");

			term_init();
		}
		else /* user_level */
		{
			/* load_program does not return error code, it exits on error */
			mltr->load_program(prog_name, argc - i, argv + i, envp);
		}

		nserver = mltr->read_servers(svr_config);

		// create a new simulator object
		signal(SIGPIPE, SIG_IGN);
		signal(SIGUSR1, SIG_IGN);
		setup_signals();
	
		/* tralsnation cache */
		if (!mltr->setup_cache(prog_name))
			exit(1);

		pthread_t tid[nserver + nthread];

		for (i=0; i<nserver; i++)
			pthread_create(tid + i, NULL, &compile_thread, mltr);

		for (i=0; i<nthread; i++)
			pthread_create(tid + nserver + i, NULL, &compile_thread2, mltr);

#if HAVE_SYS_RESOURCE_H && HAVE_SYS_TIME_H
	
		struct timeval begin_u, end_u, begin_s, end_s,
				cbegin_u, cend_u, cbegin_s, cend_s;
		struct rusage usg, cusg;
		float sim_user_time, sim_sys_time, child_user_time, child_sys_time;
		getrusage(RUSAGE_SELF, &usg);
		getrusage(RUSAGE_CHILDREN, &cusg);
		begin_u = usg.ru_utime;
		cbegin_u = cusg.ru_utime;
		begin_s = usg.ru_stime;
		cbegin_s = cusg.ru_stime;
#endif

		if (max_inum != (uint64_t)-1)
			if (user_level)
				fprintf(stderr,
					"Warning: -m ignored for user level jit simulation.\n");
			else
				mltr->set_max_count(max_inum << 20);
		
		uint64_t icount;

		icount = mltr->run();

		clear_signals();

#if HAVE_SYS_RESOURCE_H && HAVE_SYS_TIME_H
		getrusage(RUSAGE_SELF, &usg);
		getrusage(RUSAGE_CHILDREN, &cusg);
		end_u = usg.ru_utime;
		cend_u = cusg.ru_utime;
		end_s = usg.ru_stime;
		cend_s = cusg.ru_stime;
		sim_user_time = (end_u.tv_sec+end_u.tv_usec/1000000.0)-
			(begin_u.tv_sec+begin_u.tv_usec/1000000.0);
		sim_sys_time = (end_s.tv_sec+end_s.tv_usec/1000000.0)-
			(begin_s.tv_sec+begin_s.tv_usec/1000000.0);
		child_user_time = (cend_u.tv_sec+cend_u.tv_usec/1000000.0)-
			(cbegin_u.tv_sec+cbegin_u.tv_usec/1000000.0);
		child_sys_time = (cend_s.tv_sec+cend_s.tv_usec/1000000.0)-
			(cbegin_s.tv_sec+cbegin_s.tv_usec/1000000.0);

		fprintf(stderr, "Total user time  : %.3f sec.\n"
			"Total system time: %.3f sec.\n",
			child_user_time+sim_user_time, child_sys_time+sim_sys_time);
		fprintf(stderr, "Simulation user time  : %.3f sec.\n"
			"Simulation system time: %.3f sec.\n",
			sim_user_time, sim_sys_time);
		fprintf(stderr, "Caching user time  : %.3f sec.\n"
			"Caching system time: %.3f sec.\n",
			child_user_time, child_sys_time);
#endif

		fprintf(stderr, "Simulation speed : %.3e inst/sec.\n",
			icount/(child_user_time+sim_user_time+
					child_sys_time+sim_sys_time));
		mltr->dump_instruction_counters(stderr);



		fprintf(stderr, "Requested blocks  : %d.\n",
			mltr->get_num_reqs());
		fprintf(stderr, "Cache-hit count : %d.\n",
			mltr->get_num_hits());

		if (nserver > 0 || nthread > 0) {

			fprintf(stderr, "Connected servers/threads : %d of %d.\n",
				mltr->get_server_count(), nserver + nthread);
			fprintf(stderr, "Compilation count :\n");

			for (int ii=0; ii<nserver; ii++)
				pthread_join(tid[ii], NULL);

			for (int ii=0; ii<nserver; ii++)
				fprintf(stderr, "  server %d: %d\n", ii,
					mltr->get_server_compile_count(ii));

			for (int ii=0; ii<nthread; ii++)
				pthread_join(tid[ii + nserver], NULL);

			for (int ii=0; ii<nthread; ii++)
				fprintf(stderr, "  thread %d: %d\n", ii,
					mltr->get_server_compile_count(ii + nserver));

		}
		
		if (!user_level)term_exit();
	}
	else usage(argv[0]);

	delete mltr;

	return 0;
}

