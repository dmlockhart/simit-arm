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
						  loader.c  -  description
							 -------------------
	begin				: Wed Sep 26 2001
	copyright			: (C) 2001 CEA and Université Paris XI Orsay
	author				: Gilles Mouchard
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

#include "read_elf.h"
#include "loader.h"
#include "simconf.h"
#include "arm_io.h"

#include "armemul.h"

#include <cstdlib>
#include <cstring>
#include <vector>
#include <csignal>

using namespace simit;

/* for signal handling */
static const memory *my_mem;
static const char *my_path;

/* trap when memory fault occurs */
static void load_config_trap(int signum)
{
	fprintf(stderr, "Error: failed to load %s at %08x.\n"
					"	   Address %s.\n",
					my_path,
					my_mem->get_last_faddr(),
					my_mem->get_last_fault() == MEM_WRITE_PERM_FAULT ?
					"not writable" : "bad or remapped");
	exit(1);
}

/* set up memory signal so that mem traps whenever a bad write occurs */
static void setup_trap(const char *path, memory *mem)
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = load_config_trap;

	my_mem = mem;
	my_path = path;
	mem->set_signal(SIGUSR1);
	sigaction(SIGUSR1, &act, NULL);
}

static void clear_trap(memory *mem)
{
	struct sigaction act;

	mem->set_signal(-1);
	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_DFL;

	sigaction(SIGUSR1, &act, NULL);
}

/* stack grows to smaller address, mmap area grows toward bigger address */
/* 0xd0000000 to MMAP_BASE is used by floating point emulation, see nwfpe.h */
#define STACK_BASE 0xc0000000
#define MMAP_BASE  0x40000000
#define MAX_ENVIRON (16 * 1024)
#define STACK_SIZE (1024 * 1024)

#define FPE_ENTRY 0xd0000000
#define FPE_FPA11 0xd1000000
#define FPE_STACK 0xd2000000

void arm_emulator::load_elf32(const char *filename)
{
	Elf32_Ehdr *hdr;
	Elf32_Phdr *phdr;
	Elf32_Shdr *shdr;
	char *string_table;
	Elf32_Shdr *shdr_new_section;
	Elf32_Word new_section_size, new_section_type, new_section_flags;
	void *new_section;
	Elf32_Addr new_section_addr;
	FILE *fobj;
	int i;	

	data_base = 0;
	data_size = 0;


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
		if ((new_section_type == SHT_PROGBITS) ||
			(new_section_type == SHT_NOBITS))
		{
			new_section_size =
				shdr_new_section ? GetSectionSize(shdr_new_section) : 0;
			new_section_addr = GetSectionAddr(shdr_new_section);

			if (new_section_size && (new_section_flags & SHF_ALLOC))
			{
				if (verbose)
				fprintf(stderr, "Loading %s (%u bytes) at address 0x%08x\n",
					GetSymbolName(shdr[i].sh_name, string_table),
					new_section_size, new_section_addr);

				new_section = malloc(new_section_size);
				LoadSection(shdr_new_section, new_section, fobj);

				/* unitialized section => write 0s */
				if (new_section_type == SHT_NOBITS) {
					mem->set_block(new_section_addr, 0, new_section_size);
				}
				else {
					/* initialized section => copy from objfile */
					mem->write_block(new_section, new_section_addr,
							new_section_size);
				}

#if 0
				if (new_section_flags&SHF_EXECINSTR) {
					mem->set_region_permission(new_section_addr,
						new_section_size,
						MEMORY_PAGE_READABLE | MEMORY_PAGE_EXECUTABLE);
	
					if (verbose)
						fprintf(stderr, "Mark memory address %x len %x "
							"as text segment\n", new_section_addr,
							new_section_size);
				}
#endif

				free(new_section);
			}
		}
		else if (new_section_type == SHT_DYNAMIC ||
			new_section_type == SHT_DYNSYM) {
			fprintf(stderr, "File is dynamically linked,"
				" compile with `-static'.\n");
			exit(1);
		}
	}
	free(string_table);
	free(phdr);
	free(shdr);
	free(hdr);

	fclose(fobj);
}


void arm_emulator::load_program(const char *filename, 
		int argc, char *argv[], char *envp[])
{
	int ii;
	uint32_t envAddr, argAddr;
	uint32_t stack_base, stack_ptr;

	setup_trap(filename, mem);

	stack_base = STACK_BASE;
	if (stack_base > MEMORY_MAX_ADDR)
		stack_base = MEMORY_MAX_ADDR;
	stack_base -= stack_base % 4096;

	/* mark memory space available 
	 * from address 0x00000000 to STACK_BASE
	 */
	mem->mark_region_available(0x0, stack_base,
		MEMORY_PAGE_READABLE | MEMORY_PAGE_WRITABLE);

	load_elf32(filename);

	write_gpr(PC_AHEAD_IND, prog_base/*0x8300*/);
	pc_set = true;

	mem->set_block(stack_base - STACK_SIZE, 0, STACK_SIZE);

	stack_ptr = stack_base - MAX_ENVIRON;
	write_gpr(SPIND, stack_ptr);

	/*write argc to stack*/
	mem->write_word(stack_ptr, argc);
	write_gpr(1, argc);
	stack_ptr += 4;

	/*skip stack_ptr past argv pointer array*/
	argAddr = stack_ptr;
	write_gpr(2, argAddr);
	stack_ptr += (argc+1)*4;

	/*skip env pointer array*/
	envAddr = stack_ptr;
	for (ii=0; envp[ii]; ii++)
		stack_ptr += 4;
	stack_ptr += 4;

	/*write argv to stack*/ 
	for (ii=0; ii<argc; ii++) {
		mem->write_word(argAddr+ii*4, stack_ptr);
		mem->write_block(argv[ii], stack_ptr, strlen(argv[ii]));
		/*0 already at the end of the string as done by initialization*/
		stack_ptr += strlen(argv[ii])+1;
	}

	/*0 already at the end argv pointer array*/

	/*write env to stack*/
	for (ii=0; envp[ii]; ii++) {
		mem->write_word(envAddr+ii*4, stack_ptr);
		mem->write_block(envp[ii], stack_ptr, strlen(envp[ii]));
		/*0 already at the end of the string as done by initialization*/
		stack_ptr += strlen(envp[ii])+1;
	}

	/*0 already at the end argv pointer array*/

	clear_trap(mem);

	/*stack overflow*/
	if (stack_ptr+4>=stack_base) {
		fprintf(stderr,
			"Environment overflow. Need to increase MAX_ENVIRON.\n");
		exit(1);
	}

	/* initialize brk point to 4k byte boundary */
	target_addr_t abrk = data_base+data_size+4096;
	abrk -= abrk % 4096;
	
	syscall_set_brk(abrk);
	syscall_set_mmap_brk(MMAP_BASE);
}


/* load an elf OS image, no need to set up stacks */
void arm_emulator::load_osimage(const char *filename)
{
	setup_trap(filename, mem);

	load_elf32(filename);
	/* user may still set the pc in the configuraiton file */
	write_gpr(PC_AHEAD_IND, prog_base);
	pc_set = true;

	clear_trap(mem);
}

/* load a binary file to address addr */
void arm_emulator::load_binary(const char *file, arm_addr_t addr)
{
	setup_trap(file, mem);

	FILE *bif = fopen(file, "rb");
	if (bif==NULL) {
		fprintf(stderr,
			"Error: failed to open image file %s.\n", file);
		exit(1);	
	}

	if (verbose) {
		fprintf(stderr,
			"Loading binary file %s to 0x%x... ", file, addr);
	}

	char buf[2048];
	unsigned int paddr = addr;
	while (!feof(bif)) {
		size_t nbyte = fread(buf, 1, 1024, bif);
		mem->write_block(buf, paddr, nbyte);
		paddr += nbyte;
	}

	if (verbose) {
		fprintf(stderr, "Done. %d bytes saved.\n", paddr - addr);
	}

	fclose(bif);

	clear_trap(mem);
}



/* to load configuration information */

/* parsing interface */
processor_info_t *proc_info;
const  char *conf_path;
extern FILE *conf_in;
extern int conf_parse();

using std::vector;


bool arm_emulator::load_config(const char *filename)
{
	/* open the configuration file */
	if (verbose) {
		fprintf(stderr,
			"Openning configuration file %s to read.\n", filename);
	}

	conf_path = filename;

	conf_in = fopen(filename, "rt");
	if (conf_in==NULL) {
		fprintf(stderr,
			"Error: failed to open file %s.\n", filename);
		return false;
	}

	proc_info = new processor_info_t();

	if (conf_parse() || !proc_info->verify()) {
		fclose(conf_in);
		delete proc_info;
		return false;
	}

	fclose(conf_in);


	if (!cpu_config(proc_info->get_cpu_name())) {
		fprintf(stderr,
			"Error: unknown cpu name %s.\n", proc_info->get_cpu_name());
		delete proc_info;
		return false;
	}


	const vector<membank_info_t *>& mems = proc_info->get_membanks();
	const vector<preload_info_t *>& imgs = proc_info->get_preloads();
	const vector<reginit_info_t *>& regs = proc_info->get_reginits();

	/* setting up the permission for physical memory banks */
	vector<membank_info_t *>::const_iterator mit;
	for (mit = mems.begin(); mit != mems.end(); mit++)
	{
		unsigned int perm = 0;
		const membank_info_t *minfo = *mit;

		if (minfo->flag & MEMBANK_REMAPPED) continue;

		/* set up the permission */
		if (minfo->flag & MEMBANK_READABLE) perm |= MEMORY_PAGE_READABLE;
		if (minfo->flag & MEMBANK_WRITABLE) perm |= MEMORY_PAGE_WRITABLE;

		mem->mark_region_available(minfo->addr, minfo->size, perm);

		if (verbose) {
			fprintf(stderr,
				"Setting up memory bank, map=M, addr=0x%08x, "
				"size=0x%08x, %c%c\n",
				minfo->addr, minfo->size,
				minfo->flag & MEMBANK_READABLE?'R':'-',
				minfo->flag & MEMBANK_WRITABLE?'W':'-');
		}
	}


	/* loading the images */
	vector<preload_info_t *>::const_iterator pit;
	for (pit = imgs.begin(); pit != imgs.end(); pit++)
	{
		if ((*pit)->type == preload_info_t::PRE_IMG)
		{
			/* load the file into the memory bank */
			load_binary((char *)((*pit)->path), (*pit)->addr);
		}
		else if ((*pit)->type == preload_info_t::PRE_ELF)
		{
			load_osimage((char *)((*pit)->path));
		}
		else
		{
			setup_trap("data", mem);

			vector<char> *str = reinterpret_cast<vector<char> *>((*pit)->path); 

			unsigned int addr = (*pit)->addr;
			vector<char>::const_iterator strit;
			for (strit = str->begin(); strit != str->end(); strit++)
				mem->write_byte(addr++, *strit);
			
			if (verbose) {
				fprintf(stderr,
					"Saved %d bytes into memory at 0x%x.\n",
						str->size(), (*pit)->addr);
			}

			clear_trap(mem);
		}
	}

	/* setting up the permission for IO mapped memory banks */
	for (mit = mems.begin(); mit != mems.end(); mit++)
	{
		unsigned int perm = 0;
		const membank_info_t *minfo = *mit;

		if ((minfo->flag & MEMBANK_REMAPPED) == 0) continue;

		/* set up the permission */
		if (minfo->flag & MEMBANK_READABLE) perm |= MEMORY_PAGE_READABLE;
		if (minfo->flag & MEMBANK_WRITABLE) perm |= MEMORY_PAGE_WRITABLE;

		/* set up remapping information */
		mem->remap_region(minfo->addr, minfo->size, io, perm);

		if (verbose) {
			fprintf(stderr,
				"Setting up memory bank, map=I, addr=0x%08x, "
				"size=0x%08x, %c%c\n",
				minfo->addr, minfo->size,
				minfo->flag & MEMBANK_READABLE?'R':'-',
				minfo->flag & MEMBANK_WRITABLE?'W':'-');
		}
	}


	/* loading the images */
	/* set the registers */
	vector<reginit_info_t *>::const_iterator rit;
	for (rit = regs.begin(); rit != regs.end(); rit++)
	{
		write_gpr((*rit)->id, (*rit)->val);
		if ((*rit)->id == 15) pc_set = true;

		if (verbose) {
			fprintf(stderr,
				"Initializing register r%d = 0x%x\n",
				(*rit)->id, (*rit)->val);
		}
	}

	delete proc_info;
	return true;
}
