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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <errno.h>

#include <sys/stat.h>

#include "dyn_arm_emul.hpp"
#include "decomp_server.hpp"
#include "armmmu.h"
#include "build_dll.hpp"

using namespace simit;

static ssize_t sendall(int fd, const void *buf, ssize_t nbyte)
{
	ssize_t nwrite = 0, n;

	do {
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
	while (nwrite < nbyte);

	return nwrite;
}

static ssize_t recvall(int fd, void *buf, ssize_t nbyte)
{
	ssize_t nread = 0, n;

	do {
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
	while (nread < nbyte);

	return nread;
}

//constructor
decomp_server::decomp_server(bool verbose, uint32_t portnum, bool tolink) :
	verbose(verbose), portnum(portnum), tolink(tolink)
{
}

//destructor
decomp_server::~decomp_server()
{
}

uint32_t decomp_server::run(const char *temp_path)
{ 
	char obj_path[128];

	unsigned pblk_size = 4096;
	byte_t *pblk = (byte_t *)malloc(pblk_size);
	if (pblk == NULL) {
		fprintf(stderr, "Error: Insufficient memory.\n");
		return 0;	
	}

	decomp_count = 0;
	strcpy(obj_path, temp_path);
	strcat(obj_path, "/c.o");

	int create_socket, new_socket;
	struct sockaddr_in address;
	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0))>0 && verbose)
		if (verbose)
			 fprintf(stderr, "The socket was successfully created.\n");

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(portnum);
	if (bind(create_socket, (struct sockaddr *)&address,
		   sizeof(address))==0)

	if (verbose)
		fprintf(stderr, "Binding Socket, port num %d\n", portnum);

	listen(create_socket, 3);


	while (true) {

		socklen_t addrlen = sizeof(struct sockaddr_in);
		new_socket = accept(create_socket, (struct sockaddr*)&address,&addrlen);
		if (new_socket>0 && verbose)									 
			 fprintf(stderr, "The Client %s is connected...\n",
				inet_ntoa(address.sin_addr));
	
		int response = 1;
		if (verbose) fprintf(stderr, "waiting for response...\n");
	
		while (response>0)
		{
			unsigned bsize, ind;
			bool user;

			response = recvall(new_socket, &bsize, sizeof(bsize));
			if (response>0)
			{
				if (verbose) {
					fprintf(stderr, "Response received...\n");
					fprintf(stderr, "bufsize = %d\n", bsize);
				}
	
				recvall(new_socket, &ind, sizeof(unsigned));
				recvall(new_socket, &user, sizeof(bool));
	
				if (verbose) {
					fprintf(stderr, "index   = %d\n", ind);
					fprintf(stderr, "user level = %d\n", user);
				}

				/* increase buffer size in case */
				if (pblk_size < bsize) {
					pblk_size = bsize;
					pblk = (byte_t *)realloc(pblk, bsize);
					if (pblk == NULL) {
						fprintf(stderr, "Error: Insufficient memory, wanted %u bytes.\n", bsize);
						close(new_socket);
						close(create_socket);
						return decomp_count;
					}
				}

				char buf[1024];
				unsigned cnt = 0;
				while (cnt < bsize)
				{
					ssize_t res = (bsize-cnt)>1024?1024:bsize-cnt;
					if ((res=recvall(new_socket, buf, res))>0)
					{
						memcpy(pblk + cnt, buf, res);
					}
					cnt += res;
				}

				if (verbose)
					fprintf(stderr, "Code received...\n");
	
				if (compile_block(pblk, bsize, ind, user, temp_path)) {
	
					struct stat stat_stru;
					stat(obj_path, &stat_stru);
					int objsize = stat_stru.st_size; // 32bit size
					int linking = tolink;
					sendall(new_socket, &objsize, sizeof(int));
					sendall(new_socket, &linking, sizeof(int));
	
					FILE* fp = fopen(obj_path, "r");
					unsigned nread;
					while ((nread=fread(buf, 1, 1024, fp))>0)
					{
						sendall(new_socket, buf, nread);
					}
					if (verbose)
						fprintf(stderr, "Sent obj code, size %d...\n", objsize);
	
					fclose(fp);
				}
				else {
					// else send 0 back to indicate error
					int objsize = 0; // 32bit size
					int linking = 0;
					sendall(new_socket, &objsize, sizeof(int));
					sendall(new_socket, &linking, sizeof(int));
				}
			}
			decomp_count++;
		}
	
		close(new_socket);
	}

	free (pblk);
	close(create_socket);

	return decomp_count;
}


bool decomp_server::compile_block(const byte_t *pblk,
	uint32_t size, unsigned ind, bool user_level, const char *temp_path)
{

	char fname[128];
	FILE* outfile;

	strcpy(fname, temp_path);
	strcat(fname, "/a.cpp");

	/*create cpps*/
	outfile = fopen(fname, "w");
	if(outfile == NULL) {
		fprintf(stderr, "Can't open file to write");
		exit(1);
	}

	fprintf(outfile, "#include <arch_jit.hpp>\n");
	fprintf(outfile, "#include <dyn_arm_emul.hpp>\n");
	fprintf(outfile, "using namespace simit;\n");


	if (user_level)
		fprintf(outfile, "typedef arm_emulator arm_emul_t;\n");
	else
		fprintf(outfile, "typedef dyn_arm_emulator arm_emul_t;\n");

	simit::decomp_block(outfile, pblk, size, user_level, ind);

	fclose(outfile);

	char compilestring[1024];

	if (tolink)
		sprintf(compilestring, 
			CXX MORE_OPTION " %s/a.cpp"
			" -Wl,-soname,libXcompiled_%u.so"
			" -o %s/c.o"
			" -I%s/include -DSIMIT_COMPILED_ISS %s",
			temp_path, ind, temp_path, PREFIX,
			user_level?"":"-DSIMIT_SYSTEM_LEVEL");
	else
		sprintf(compilestring, 
			CXX MORE_OPTION " %s/a.cpp"
			" -c -o %s/c.o"
			" -I%s/include -DSIMIT_COMPILED_ISS %s",
			temp_path, temp_path, PREFIX,
			user_level?"":"-DSIMIT_SYSTEM_LEVEL");

	if (verbose)
		fprintf(stderr, "%s\n", compilestring);

	if (system(compilestring)==0) {
		if (verbose) fprintf(stderr,"...done!\n");
		return true;
	}

	if (verbose) fprintf(stderr,"...failed!\n");
	return false;
}

