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

#ifndef SIMIT_CONF_H
#define SIMIT_CONF_H

/* header file for system configuration information */

#include <cstdio>
#include <cstdlib>
#include <vector>

namespace simit 
{

const unsigned int MEMBANK_READABLE = 0x1;
const unsigned int MEMBANK_WRITABLE = 0x2;
const unsigned int MEMBANK_REMAPPED = 0x4;

struct membank_info_t
{
	unsigned int flag;
	unsigned int addr;
	unsigned int size;
};



struct preload_info_t
{
	enum {
		PRE_VAL,
		PRE_IMG,
		PRE_ELF,
	} type;
	
	void *path;	// either char * or vector<char> *;
	unsigned int addr;
};

struct reginit_info_t
{
	int id;
	unsigned int val;
};

class processor_info_t 
{

  public:

	/* constructor */
	processor_info_t() : cpu(NULL), mach(NULL) {}

	/* destructor */
	~processor_info_t();

	/* set cpu */
	void set_cpu_name(char *c);

	/* set mach */
	void set_mach_name(char *m);

	/* get cpu */
	const char *get_cpu_name() const {return cpu;}

	/* get mach */
	const char *get_mach_name() const {return mach;}


	/* add membank information */
	void add_membank(struct membank_info_t *);

	/* add mem bank */
	void add_preload(struct preload_info_t *);

	/* add mem bank */
	void add_reginit(struct reginit_info_t *);

	/* get n-th memory bank*/
	const std::vector<struct membank_info_t *>& get_membanks() const
	{
		return mems;
	}

	/* get preload information */
	const std::vector<struct preload_info_t *>& get_preloads() const
	{
		return imgs;
	}

	/* get preload information */
	const std::vector<struct reginit_info_t *>& get_reginits() const
	{
		return regs;
	}

	/* verify the provided information is consistent, true on success */
	bool verify();

  private:

	char *cpu;
	char *mach;
	std::vector<membank_info_t *> mems;
	std::vector<preload_info_t *> imgs;
	std::vector<reginit_info_t *> regs;
};


} /* namespace simit */


#endif
