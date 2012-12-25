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

#include "simconf.h"

using namespace simit;
using std::vector;

processor_info_t::~processor_info_t()
{
	if (cpu) free(cpu);
	if (mach) free(mach);

	vector<membank_info_t *>::iterator mit;
	for (mit = mems.begin(); mit != mems.end(); mit++)
		delete *mit;

	vector<preload_info_t *>::iterator pit;
	for (pit = imgs.begin(); pit != imgs.end(); pit++)
	{
		if ((*pit)->type != preload_info_t::PRE_VAL)
			free((*pit)->path);
		else
			delete reinterpret_cast<vector<char> *>((*pit)->path);
		delete *pit;
	}

	vector<reginit_info_t *>::iterator rit;
	for (rit = regs.begin(); rit != regs.end(); rit++)
		delete *rit;
}

void processor_info_t::set_cpu_name(char *name)
{
	if (cpu) {
		fprintf(stderr, "Warning: cpu name %s overwritten by %s", cpu, name);
		free(cpu);
	}
	cpu = name;
}

void processor_info_t::set_mach_name(char *name)
{
	if (mach) {
		fprintf(stderr, "Warning: mach name %s overwritten by %s", mach, name);
		free(mach);
	}
	mach = name;
}


void processor_info_t::add_membank(membank_info_t *m) 
{
	mems.push_back(m);
}

void processor_info_t::add_preload(preload_info_t *p)
{
	imgs.push_back(p);
}

void processor_info_t::add_reginit(reginit_info_t *r)
{
	regs.push_back(r);
}

bool processor_info_t::verify()
{
	bool ret = true;

	vector<membank_info_t *>::const_iterator mit1, mit2;
	for (mit1 = mems.begin(); mit1 != mems.end(); mit1++) {

		/* check for useless memory banks */
		if (((*mit1)->flag & (MEMBANK_READABLE | MEMBANK_WRITABLE)) == 0) {
			fprintf(stderr, "Error: useless memory bank starting at %x, "
							"not readable or writable!\n", (*mit1)->addr);
			ret = false;
		}

		/* check for overlapping */
		for (mit2 = mit1, mit2++; mit2 != mems.end(); mit2++) {
			if ((*mit1)->addr <= (*mit2)->addr &&
				(*mit1)->addr+(*mit1)->size > (*mit2)->addr ||
				(*mit2)->addr <= (*mit1)->addr &&
				(*mit2)->addr+(*mit2)->size > (*mit1)->addr) {
				fprintf(stderr, "Error: overlapping memory banks "
							"starting at %x and %x!\n",
							(*mit1)->addr, (*mit2)->addr);
				ret = false;
			}
		}
	}

	return ret;
}
