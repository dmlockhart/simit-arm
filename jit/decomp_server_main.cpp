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
#include <decomp_server.hpp>
#include <config.h>
#include <setjmp.h>

#include <unistd.h>
#if HAVE_SYS_RESOURCE_H && HAVE_SYS_TIME_H
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#endif

//these are new to the dynamic compiler

using namespace simit;

static void usage(char *fname)
{
  char *cp;
	if ((cp = strrchr(fname, '/'))!=NULL) cp++;
	else cp = fname;
	fprintf(stderr, 
		"usage : %s [-v] [-p portnum] [-h]\n"
		"  -v : verbose mode\n"
		"  -p : port number\n"
		"  -n : no linking\n"
		"  -h : print this message and quit\n", cp);
 
}

static char temp_path[] = "/tmp/dcmpXXXXXX";
static jmp_buf jbuf;

static void sig_handler(int signum)
{
	char fname[128];

	// remove the temporary files
	strcpy(fname, temp_path);
	strcat(fname, "/a.cpp");
	//unlink(fname);

	strcpy(fname, temp_path);
	strcat(fname, "/c.o");
	//unlink(fname);

	//rmdir(temp_path);
	longjmp(jbuf, 1);
}

int main(int argc, char *argv[], char *envp[])
{
	int i;
	int portnum = 55555;
	bool verbose = false;
	bool linking = true;
	char *bin_name;

	for(i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-v") == 0) verbose = true; else
		if(strcmp(argv[i], "-n") == 0) linking = false; else
		if(strcmp(argv[i], "-p") == 0) portnum = atoi(argv[++i]); else
		{usage(argv[0]); return 0;}
	}

	if ((bin_name = strrchr(argv[0], '/'))!=NULL) bin_name++; 
	else bin_name = argv[0];

	// create a new simulator object
	decomp_server *ds = new decomp_server(verbose, portnum, linking);
       
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

	if (mkdtemp(temp_path)==NULL) {
		perror("Cannot open temporary folder");
		return 0;
	}

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, sig_handler);

	uint32_t comp_count;

	if (setjmp(jbuf)==0) {
		comp_count = ds->run(temp_path);
	}

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
#endif
	fprintf(stderr, "Total compiled   : %d.\n", ds->get_count());
	delete ds;

	return 0;
}
