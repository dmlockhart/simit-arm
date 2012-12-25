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
#ifndef SIMIT_IO_H
#define SIMIT_IO_H

#include "armemul.h"

namespace simit{ 


class arm_io : public memory_ext_interface
{
	public:

		arm_io(arm_emulator *emu) : emu(emu) {};
		virtual ~arm_io() {};

		virtual void do_cycle() = 0;

		/* disable unused interface functions */
		memory_fault_t read_byte(target_addr_t addr, byte_t *p) 
		{
			return MEM_NO_FAULT;
		}

		memory_fault_t write_byte(target_addr_t addr, byte_t v)
		{
			return MEM_NO_FAULT;
		}
                                                              
		memory_fault_t read_hword(target_addr_t addr, hword_t *p)
		{
			return MEM_NO_FAULT;
		}

		memory_fault_t write_hword(target_addr_t addr, hword_t v)
		{
			return MEM_NO_FAULT;
		}

		memory_fault_t read_dword(target_addr_t addr, dword_t *p)
		{
			return MEM_NO_FAULT;
		}

		memory_fault_t write_dword(target_addr_t addr, dword_t v)
		{
			return MEM_NO_FAULT;
		}

	protected:

		arm_emulator *emu;
};


#define IO_DIVISOR 50 // handle IO every 50 cycles

}

#endif //SIMIT_IO_H
