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
/***************************************************************************
						  debug.c  -  description
							 -------------------
	begin				: Wed Sep 26 2001
	copyright			: (C) 2001 CEA and Université Paris XI Orsay
	author			   : Gilles Mouchard
	email				: gilles.mouchard@lri.fr, gilles.mouchard@cea.fr
 ***************************************************************************/

/***************************************************************************
 *																		 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or	 *
 *   (at your option) any later version.								   *
 *																		 *
 ***************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include "armemul.h"
#include "arm_io.h"
#include "decode.h"
#include "armmmu.h"

using namespace simit;

extern char *arm_regnames[];
extern int arm_decode_regname(const char *);

void arm_emulator::debug_disasm(FILE *stream, arm_addr_t addr)
{
	int i;
	arm_inst_t inst;
	char disasm[256];
	
	for(i = 0; i < 10; i++, addr += 4)
	{
		inst = user_level?fetch_inst_user(addr):fetch_inst_system(addr);
		arm_disassemble(inst, addr, disasm);
		fprintf(stream, "0x%08x : 0x%08x    %s", addr, inst, disasm);
	}
}

void arm_emulator::debug_dump(FILE *stream, arm_addr_t addr)
{
	byte_t b;
	int i, j;
	
	fprintf(stream,
		"  address    0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
	for(i = 0; i < 16; i++)
	{
		fprintf(stream, "0x%08x: ", addr);
		for(j = 0; j < 16; j++, addr++)
		{
			if (user_level)
				mem->read_byte(addr, &b);
			else
				mmu->read_byte(addr, &b);
			fprintf(stream, "%02x", b);
			if (j < 15) fprintf(stream, " ");
		}
		fprintf(stream, "\n");
	}
}

void arm_emulator::debug_go_to(arm_addr_t addr)
{
	arm_inst_t inst;
	char disasm[256];
	
	status = ST_DEBUGGING;
	pcount = prescale;

	if (user_level) 
	{
		while(status==ST_DEBUGGING && get_pc() != addr)
			step_user();

		inst = fetch_inst_user(get_pc());
	}
	else
	{
		term_init();
		while(status==ST_DEBUGGING && get_pc() != addr)
			step_system();

		inst = fetch_inst_system(get_pc());
		term_exit();
	}

	if (is_done()) 
		printf("Program exited with code %d.\n", retcode);
	else
	{
		arm_disassemble(inst, get_pc(), disasm);
		printf("0x%08x : 0x%08x    %s", get_pc(), inst, disasm);
	}
}

void arm_emulator::debug_trace(int count)
{
	arm_inst_t inst;
	char disasm[256];

	status = ST_DEBUGGING;
	pcount = prescale;
	if (user_level)
	{
		while(status==ST_DEBUGGING && count > 0)
		{
			step_user();
			count--;
		}
		inst = fetch_inst_user(get_pc());
	}
	else 
	{
		term_init();
		while(status==ST_DEBUGGING && count > 0)
		{
			step_system();
			count--;
		}
		inst = fetch_inst_system(get_pc());
		term_exit();
	}
	
	arm_disassemble(inst, get_pc(), disasm);
	printf("0x%08x : 0x%08x    %s", get_pc(), inst, disasm);
}

void arm_emulator::debug_dump_registers(FILE *stream)
{
	int i, j, n;
	char s[256];
	
	for(n = 0, i = 0; i < 4;  i++)
	{
		for(j = 0; j < 4; j++, n++)
		{
			sprintf(s, "%s = 0x%08x", arm_regnames[n],
				read_gpr(n==PC_AHEAD_IND?PC_REAL_IND:n));
			fprintf(stream, "%17s", s);
			if (j < 3) fprintf(stream, "  ");
		}
		fprintf(stream, "\n");
	}
}

bool get_addr(char *s, arm_addr_t *addr)
{
	return sscanf(s, "%x", addr) == 1 || sscanf(s, "0x%x", addr) == 1;
}

char *my_gets(char *p, int size)
{
	char *ret = fgets(p, size, stdin);
	if (ret)
	{
		int l = strlen(p);
		if (l > 0) p[l - 1] = 0;
	}
	return ret;
}

static void debug_help(FILE *fp)
{
	fprintf(fp, " Debug commands:\n"
				"	h         print this message\n"
				"	u [addr]  disassemble 10 instructions\n"
				"	d [addr]  dump 256 bytes from memory\n"
				"	g [addr]  run until addr\n"
				"	s [num]   run until pc + num*4\n"
				"	t [num]   step num instruction(s)\n"
				"	r[n]      dump register value(s)\n"
				"	cpsr      dump cpsr value\n"
				"	spsr      dump spsr value\n"
				"	c         dump instruction counters\n"
				"	cc        reset instruction counters\n"
				"	q         quit\n");
}

void arm_emulator::debug()
{
	arm_addr_t disasm_addr = get_pc();
	arm_addr_t dump_addr = get_pc();
	int nparms;
	int count;
	char line[256];
	char last_line[256];
	char parm[8][32];
	
	
	strcpy(last_line, "");
	strcpy(line, "");
	
	do
	{	
	 	if (strcmp(line, "") == 0)
	 	{
	 		strcpy(line, last_line);
	 	}
		
		nparms = sscanf(line, "%s %s %s %s %s %s %s %s",
			parm[0], parm[1], parm[2], parm[3], parm[4], parm[5],
			parm[6], parm[7]);
		
		switch(nparms)
		{
			case 1:
				if (strcmp(parm[0], "h") == 0 || strcmp(parm[0], "?") == 0)
				{
					debug_help(stdout);
					break;
				}
				if (strcmp(parm[0], "u") == 0)
				{
					debug_disasm(stdout, disasm_addr);
					disasm_addr += 10 * 4;
					break;
				}
				else if (strcmp(parm[0], "d") == 0)
				{
					debug_dump(stdout, dump_addr);
					dump_addr += 16 * 16;
					break;
				}
				else if (strcmp(parm[0], "g") == 0)
				{
					debug_go_to(0);
					break;
				}
				else if (strcmp(parm[0], "cpsr") == 0)
				{
					printf("cpsr = 0x%08x\n", read_cpsr());
					break;
				}
				else if (strcmp(parm[0], "spsr") == 0)
				{
					printf("spsr = 0x%08x\n", read_spsr());
					break;
				}
				else if (strcmp(parm[0], "r") == 0)
				{
					debug_dump_registers(stdout);
					break;
				}
				else if (strcmp(parm[0], "s") == 0)
				{
					debug_go_to(get_pc() + 4);
					break;
				}
				else if (strcmp(parm[0], "t") == 0)
				{
					debug_trace(1);
					break;
				}
				else if (strcmp(parm[0], "q") == 0)
				{
					return;
				}
				else if (strcmp(parm[0], "c") == 0)
				{
					dump_instruction_counters(stdout);
					break;
				}
				else if (strcmp(parm[0], "cc") == 0)
				{
					reset_instruction_counters();
					break;
				}
				else
				{
					int id = arm_decode_regname(parm[0]);
					if (id >= 0)
						printf("%s = 0x%08x\n", parm[0],
							read_gpr(id==PC_AHEAD_IND?PC_REAL_IND:id));
					else
						printf("Unknown debugging command!\n");
				}
				break;
				
			case 2:
				/* maybe the second parm is a register */
				int id = arm_decode_regname(parm[1]);
				arm_addr_t parm_addr;
				if (id >= 0)
					parm_addr = read_gpr(id==PC_AHEAD_IND?PC_REAL_IND:id);

				if (strcmp(parm[0], "u") == 0 && 
					(id >=0 || get_addr(parm[1], &parm_addr)))
				{
					debug_disasm(stdout, parm_addr);
					disasm_addr = parm_addr + 10 * 4;
					break;
				}
				else if (strcmp(parm[0], "d") == 0 &&
					(id >=0 || get_addr(parm[1], &parm_addr)))
				{
					debug_dump(stdout, parm_addr);
					dump_addr = parm_addr + 16 * 16;
					break;
				}
				else if (strcmp(parm[0], "g") == 0 &&
					(id >=0 || get_addr(parm[1], &parm_addr)))
				{
					debug_go_to(parm_addr);
					disasm_addr = parm_addr;
					break;
				}
				else if (strcmp(parm[0], "s") == 0 &&
					sscanf(parm[1], "%d", &count) == 1)
				{
					debug_go_to(get_pc() + count * 4);
					break;
				}
				else if (strcmp(parm[0], "t") == 0 &&
					sscanf(parm[1], "%d", &count) == 1)
				{
					debug_trace(count);
					break;
				}
#if 0
				else if (strcmp(parm[0], "s") == 0)
				{
					arm_save(parm[1]);
					break;
				}
				else if (strcmp(parm[0], "l") == 0)
				{
					arm_restore(parm[1]);
					break;
				}
#endif
				else
				{
					printf("Unknown debugging command!\n");
				}
				break;
		}
	
		strcpy(last_line, line);
		
		printf("> ");

	} while (my_gets(line, sizeof(line)));

}
