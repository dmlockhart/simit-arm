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
#include <armemul.h>
#include "config.h"

#include "armmmu.h"

#if HAVE_SYS_RESOURCE_H && HAVE_SYS_TIME_H
#include <sys/time.h>
#include <sys/resource.h>
#endif


using namespace simit;


static void usage(char *fname)
{
	char *cp;
	if ((cp = strrchr(fname, '/'))!=NULL) cp++;
	else cp = fname;
	fprintf(stderr, 
		"**** " PACKAGE " Version " VERSION
		" (C) 2002-2007 Wei Qin ****\n"
		"usage : %s [-h] [-d] [-v] [-m num] [-s config] "
		" [<file name> <args...>]\n"
		"  -h : print this message and quit\n"
		"  -d : debuging mode\n"
		"  -v : verbose mode\n"
		"  -m num : maximum number of instructions to simulate, in million\n"
		"  -s config : path to config file for system-level simulation\n"
		"  file name : the user-level ELF program to simulate\n"
		"  args      : arguments to the program\n"
		"Note: Only one of -s and <file name> should be given.\n", cp);
}


static arm_emulator *ema = NULL;

static void sig_handler(int signum)
{
	/* do nothing if the simulator does not exist */
	if (ema==NULL) exit(0);

	if (signum==SIGINT || signum==SIGTERM) {
		ema->interrupt();
	}
#if 0
	else if (signum==SIGUSR1) {
		ema->seg_fault();

		fprintf(stderr, "Error accessing memory, address = %08x\n",
			ema->mem->get_fault_addr(), ema->mem->get_fault_code());

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


int main(int argc, char *argv[], char *envp[])
{
	int i;
	char *bin_name;
	char *prog_name = NULL;
	char *conf_name = NULL;
	bool verbose = false;
	bool debuging = false;
	bool usr_level = false;
	uint64_t max_inum = (uint64_t)-1;

	for(i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-d") == 0) debuging = true; else
		if(strcmp(argv[i], "-v") == 0) verbose = true; else
		if(strcmp(argv[i], "-m") == 0) max_inum = ato_uint64(argv[++i]); else
		if(strcmp(argv[i], "-h") == 0) {usage(argv[0]); return 0;} else
		if(strcmp(argv[i], "-s") == 0) conf_name = argv[++i]; else
		if(!prog_name)
		{
			prog_name = argv[i];
			break;
		}	
	}

	usr_level = conf_name==NULL;
	
	/* get the basename of myself, for printing messages */
	if ((bin_name = strrchr(argv[0], '/'))!=NULL) bin_name++;
	else bin_name = argv[0];

	/* emulator instance */
	if ((usr_level && prog_name!=NULL) || (!usr_level && prog_name==NULL))
	{
		/* if isimage, then do not emulate system calls */
		ema = new arm_emulator(verbose, usr_level);

		/* problem loading configuration file */
		if (!usr_level)
		{
			if (!ema->load_config(conf_name))
			{
				delete ema;
				return 1;
			}
			if (!ema->is_pc_set())
				fprintf(stderr, "Warning: PC not given, starting from 0x0.\n");

		}
		else /* user_level */
		{
			/* load_program does not return error code, it exits on error */
			ema->load_program(prog_name, argc - i, argv + i, envp);
		}

		setup_signals();

		if (debuging)
		{
			ema->debug();
		}
		else
		{
			fprintf(stderr, "%s: Simulation starts ...\n", bin_name);

#if HAVE_SYS_RESOURCE_H && HAVE_SYS_TIME_H
	
			struct timeval begin_u, end_u, begin_s, end_s;
			struct rusage usg;
			float user_time, sys_time;
			getrusage(RUSAGE_SELF, &usg);
			begin_u = usg.ru_utime;
			begin_s = usg.ru_stime;
#endif

			uint64_t icount;

			if (max_inum != (uint64_t)-1)
				ema->set_max_count(max_inum << 20);

			if (!usr_level) term_init();

			icount = ema->run();
			
#if HAVE_SYS_RESOURCE_H && HAVE_SYS_TIME_H
			getrusage(RUSAGE_SELF, &usg);
			end_u = usg.ru_utime;
			end_s = usg.ru_stime;
			user_time = (end_u.tv_sec+end_u.tv_usec/1000000.0)-
				(begin_u.tv_sec+begin_u.tv_usec/1000000.0);
			sys_time = (end_s.tv_sec+end_s.tv_usec/1000000.0)-
				(begin_s.tv_sec+begin_s.tv_usec/1000000.0);

#endif

			if (ema->is_interrupted())
			{
				fprintf(stderr, "%s: Simulation stopped by user.\n", bin_name);
			}
			else if (usr_level)
			{
				if (ema->get_retcode()==0)
					fprintf(stderr, "%s: Program exited normally.\n",
						bin_name);
				else
					fprintf(stderr, "%s: Program exited with code %d.\n",
						bin_name, ema->get_retcode());
			}
			else
			{
				term_exit();
			}


#if HAVE_SYS_RESOURCE_H && HAVE_SYS_TIME_H
			fprintf(stderr, "Total user time  : %.3f sec.\n"
				"Total system time: %.3f sec.\n"
				"Simulation speed : %.3e inst/sec.\n",
				user_time, sys_time, icount/(user_time+sys_time));
#endif

			if (verbose)
				fprintf(stderr, "Total %dK memory pages allocated : %d\n",
					MEMORY_PAGE_SIZE>>10, ema->mem->get_page_count());

			ema->dump_instruction_counters(stderr);


		}

		clear_signals();

		delete ema;
	}
	else usage(argv[0]);

	return 0;
}
