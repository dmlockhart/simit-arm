
#include <read_elf.h>
#include <stdlib.h>
#include <string.h>

#define SwapHalf(a) (((a & 0x00ff) << 8) | ((a & 0xff00) >> 8))
#define SwapWord(a) (((a & 0xff000000) >> 24) | ((a & 0x00ff0000) >> 8) | ((a & 0x0000ff00) << 8) | ((a & 0x000000ff) << 24))
#define SwapAddr(a) SwapWord(a)
#define SwapOff(a) SwapWord(a)

int LittleEndian()
{
	Elf32_Word a = 0x01020304;
	return *(char *) &a == 0x04;
}

void SwapElfHeader(Elf32_Ehdr *hdr)
{
	hdr->e_type = SwapHalf(hdr->e_type);
	hdr->e_machine = SwapHalf(hdr->e_machine);
	hdr->e_version = SwapWord(hdr->e_version);
	hdr->e_entry = SwapAddr(hdr->e_entry);
	hdr->e_phoff = SwapOff(hdr->e_phoff);
	hdr->e_shoff = SwapOff(hdr->e_shoff);
	hdr->e_flags = SwapWord(hdr->e_flags);
	hdr->e_ehsize = SwapHalf(hdr->e_ehsize);
	hdr->e_phentsize = SwapHalf(hdr->e_phentsize);
	hdr->e_phnum = SwapHalf(hdr->e_phnum);
	hdr->e_shentsize = SwapHalf(hdr->e_shentsize);
	hdr->e_shnum = SwapHalf(hdr->e_shnum);
	hdr->e_shstrndx = SwapHalf(hdr->e_shstrndx);
}

void SwapProgramHeader(Elf32_Phdr *phdr)
{
	phdr->p_type = SwapWord(phdr->p_type);
	phdr->p_offset = SwapOff(phdr->p_offset);
	phdr->p_vaddr = SwapAddr(phdr->p_vaddr);
	phdr->p_paddr = SwapAddr(phdr->p_paddr);
	phdr->p_filesz = SwapWord(phdr->p_filesz);
	phdr->p_memsz = SwapWord(phdr->p_memsz);
	phdr->p_flags = SwapWord(phdr->p_flags);
	phdr->p_align = SwapWord(phdr->p_align);
}

void SwapSectionHeader(Elf32_Shdr *shdr)
{
	shdr->sh_name = SwapWord(shdr->sh_name);
	shdr->sh_type = SwapWord(shdr->sh_type);
	shdr->sh_flags = SwapWord(shdr->sh_flags);
	shdr->sh_addr = SwapAddr(shdr->sh_addr);
	shdr->sh_offset = SwapOff(shdr->sh_offset);
	shdr->sh_size = SwapWord(shdr->sh_size);
	shdr->sh_link = SwapWord(shdr->sh_link);
	shdr->sh_info = SwapWord(shdr->sh_info);
	shdr->sh_addralign = SwapWord(shdr->sh_addralign);
	shdr->sh_entsize = SwapWord(shdr->sh_entsize);
}

void AdjustElfHeader(Elf32_Ehdr *hdr)
{
	switch(hdr->e_ident[EI_DATA])
	{
		case ELFDATA2LSB:
			if(!LittleEndian())
				SwapElfHeader(hdr);
			break;
		case ELFDATA2MSB:
			if(LittleEndian())
				SwapElfHeader(hdr);
			break;
	}
}

void AdjustProgramHeader(Elf32_Ehdr *hdr, Elf32_Phdr *phdr)
{
	switch(hdr->e_ident[EI_DATA])
	{
		case ELFDATA2LSB:
			if(!LittleEndian())
				SwapProgramHeader(phdr);
			break;
		case ELFDATA2MSB:
			if(LittleEndian())
				SwapProgramHeader(phdr);
			break;
	}
}
void AdjustSectionHeader(Elf32_Ehdr *hdr, Elf32_Shdr *shdr)
{
	switch(hdr->e_ident[EI_DATA])
	{
		case ELFDATA2LSB:
			if(!LittleEndian())
				SwapSectionHeader(shdr);
			break;
		case ELFDATA2MSB:
			if(LittleEndian())
				SwapSectionHeader(shdr);
			break;
	}
}

Elf32_Ehdr *ReadElfHeader(FILE *f)
{
	Elf32_Ehdr *hdr = (Elf32_Ehdr *) malloc(sizeof(Elf32_Ehdr));

	if(fseek(f, 0, SEEK_SET) != 0)
	{
		free(hdr);
		return NULL;
	}
	if(fread(hdr, sizeof(Elf32_Ehdr), 1, f) != 1)
	{
		free(hdr);
		return NULL;
	}
	
	if(hdr->e_ident[EI_MAG0] != 0x7f ||
	   hdr->e_ident[EI_MAG1] != 'E' ||
	   hdr->e_ident[EI_MAG2] != 'L' ||
	   hdr->e_ident[EI_MAG3] != 'F')
	{
		free(hdr);
		return NULL;
	}
	
	hdr = (Elf32_Ehdr *) realloc(hdr, hdr->e_ehsize);

	AdjustElfHeader(hdr);	
	return hdr;
}

Elf32_Phdr *ReadProgramHeaders(Elf32_Ehdr *hdr, FILE *f)
{
	int i;
	unsigned long sz = hdr->e_phnum * hdr->e_phentsize;
	Elf32_Phdr *phdr = (Elf32_Phdr *) malloc(sz);
	
	fseek(f, hdr->e_phoff, SEEK_SET);
	fread(phdr, sz, 1, f);
	
	for(i = 0; i < hdr->e_phnum; i++)
		AdjustProgramHeader(hdr, phdr + i);
	
	return phdr;
}

Elf32_Shdr *ReadSectionHeaders(Elf32_Ehdr *hdr, FILE *f)
{
	int i;
	unsigned long sz = hdr->e_shnum * hdr->e_shentsize;
	Elf32_Shdr *shdr = (Elf32_Shdr *) malloc(sz);
	
	if(fseek(f, hdr->e_shoff, SEEK_SET) != 0)
	{
		free(shdr);
		return NULL;
	}
	if(fread(shdr, sz, 1, f) != 1)
	{
		free(shdr);
		return NULL;
	}

	for(i = 0; i < hdr->e_shnum; i++)
		AdjustSectionHeader(hdr, shdr + i);
	
	return shdr;
}


char *LoadStringTable(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, FILE *f)
{
	Elf32_Shdr *shdr = shdr_table + hdr->e_shstrndx;
	char *string_table;

	string_table = (char *) malloc(shdr->sh_size);
	fseek(f, shdr->sh_offset, SEEK_SET);
	fread(string_table, shdr->sh_size, 1, f);
	
	return string_table;
}

void DumpElfHeader(Elf32_Ehdr *hdr)
{
	printf("--- Elf Header ---");
	printf("\nClass : ");
	switch(hdr->e_ident[EI_CLASS])
	{
		case ELFCLASSNONE : printf("Invalid class"); break;
		case ELFCLASS32 : printf("32-bit objects"); break;
		case ELFCLASS64 : printf("64-bit objects"); break;
		default : printf("?"); break;
	}
	
	printf("\nData encoding : ");
	switch(hdr->e_ident[EI_DATA])
	{
		case ELFDATANONE : printf("Invalid data encoding"); break;
		case ELFDATA2LSB : printf("2's complement, little endian"); break;
		case ELFDATA2MSB : printf("2's complement, big endian"); break;
	}
	
	printf("\nVersion : %u", (unsigned int) hdr->e_ident[EI_VERSION]);	
	
/* 	printf("\nOS ABI identification : "); */
/* 	switch(hdr->e_ident[EI_OSABI]) */
/* 	{ */
/* 		case ELFOSABI_SYSV : printf("UNIX System V ABI"); break; */
/* 		case ELFOSABI_HPUX : printf("HP-UX"); break; */
/* 		case ELFOSABI_ARM : printf("ARM"); break; */
/* 		case ELFOSABI_STANDALONE : printf("Standalone (embedded) application"); */
/* 		default : printf("?"); */
/* 	} */
	
	printf("\nObject File Type : ");
	
	switch(hdr->e_type)
	{
		case ET_NONE : printf("No file type"); break;
		case ET_REL : printf("Relocatable file"); break;
		case ET_EXEC : printf("Executable file"); break;
		case ET_DYN : printf("Shared object file"); break;
		case ET_CORE : printf("Core file"); break;
		case ET_NUM : printf("Number of defined types"); break;
		case ET_LOPROC : printf("Processor-specific"); break;
		case ET_HIPROC : printf("Processor-specific"); break;
		default : printf("?");
	}
	
	printf("\nTarget machine : ");
	
/* 	switch(hdr->e_machine) */
/* 	{ */
/* 		case EM_NONE : printf("No machine"); break; */
/* 		case EM_M32 : printf("AT&T WE 32100"); break; */
/* 		case EM_SPARC : printf("SUN SPARC"); break; */
/* 		case EM_386 : printf("Intel 80386"); break; */
/* 		case EM_68K : printf("Motorola m68k family"); break; */
/* 		case EM_88K : printf("Motorola m88k family"); break; */
/* 		case EM_486 : printf("Intel 80486"); break; */
/* 		case EM_860 : printf("Intel 80860"); break; */
/* 		case EM_MIPS : printf("MIPS R3000 big-endian"); break; */
/* 		case EM_S370 : printf("Amdahl"); break; */
/* 		case EM_MIPS_RS4_BE : printf("MIPS R4000 big-endian"); break; */
/* 		case EM_RS6000 : printf("RS6000"); break; */

/* 		case EM_PARISC : printf("HPPA"); break; */
/* 		case EM_nCUBE : printf("nCUBE"); break; */
/* 		case EM_VPP500 : printf("Fujitsu VPP500"); break; */
/* 		case EM_SPARC32PLUS : printf("Sun v8plus"); break; */
/* 		case EM_960 : printf("Intel 80960"); break; */
/* 		case EM_PPC : printf("PowerPC"); break; */

/* 		case EM_V800 : printf("NEC V800 series"); break; */
/* 		case EM_FR20 : printf("Fujitsu FR20"); break; */
/* 		case EM_RH32 : printf("TRW RH32"); break; */
/* 		case EM_MMA : printf("Fujitsu MMA"); break; */
/* 		case EM_ARM : printf("ARM"); break; */
/* 		case EM_FAKE_ALPHA : printf("Digital Alpha"); break; */
/* 		case EM_SH : printf("Hitachi SH"); break; */
/* 		case EM_SPARCV9 : printf("SPARC v9 64-bit"); break; */
/* 		case EM_TRICORE : printf("Siemens Tricore"); break; */
/* 		case EM_ARC : printf("Argonaut RISC Core"); break; */
/* 		case EM_H8_300 : printf("Hitachi H8/300"); break; */
/* 		case EM_H8_300H : printf("Hitachi H8/300H"); break; */
/* 		case EM_H8S : printf("Hitachi H8S"); break; */
/* 		case EM_H8_500 : printf("Hitachi H8/500"); break; */
/* 		case EM_IA_64 : printf("Intel Merced"); break; */
/* 		case EM_MIPS_X : printf("Stanford MIPS-X"); break; */
/* 		case EM_COLDFIRE : printf("Motorola Coldfire"); break; */
/* 		case EM_68HC12 : printf("Motorola M68HC12"); break; */
/* 		default: printf("?"); */
/* 	} */
	
	printf("\nNumber of program headers : %u", hdr->e_phnum);
	
	printf("\nNumber of section headers : %u", hdr->e_shnum);
	
	printf("\n");
}

void DumpProgramHeader(Elf32_Phdr *phdr)
{
	printf("--- Program Header ---");
	printf("\nSegment type : ");
	switch(phdr->p_type)
	{	
		case PT_NULL: printf("Program header table entry unused"); break;
		case PT_LOAD: printf("Loadable program segment"); break;
		case PT_DYNAMIC: printf("Dynamic linking information"); break;
		case PT_INTERP: printf("Program interpreter"); break;
		case PT_NOTE: printf("Auxiliary information"); break;
		case PT_SHLIB: printf("Reserved"); break;
		case PT_PHDR: printf("Entry for header table itself"); break;
/* 		case PT_LOOS: printf("Start of OS-specific"); break; */
/* 		case PT_HIOS: printf("End of OS-specific"); break; */
		case PT_LOPROC: printf("Start of processor-specific"); break;
		case PT_HIPROC: printf("End of processor-specific"); break;
	}
	
	printf("\nFile offset : 0x%lx", (unsigned long) phdr->p_offset);
	printf("\nVirtual Address : 0x%lx", (unsigned long) phdr->p_vaddr);
	printf("\nPhysical Address : 0x%lx", (unsigned long) phdr->p_paddr);
	printf("\nFile size : %lu", (unsigned long) phdr->p_filesz);
	printf("\nMemory size : %lu", (unsigned long) phdr->p_memsz);
	printf("\nSegment flags (rwx) : ");
	printf(phdr->p_flags & PF_R ? "r" : "-");
	printf(phdr->p_flags & PF_W ? "w" : "-");
	printf(phdr->p_flags & PF_X ? "x" : "-");
	printf(phdr->p_flags & PF_MASKPROC ? " and some Processor-specific flags" : "");

	printf("\nMemory/File alignment : %u", phdr->p_align);

	printf("\n");
}



void DumpSectionHeader(Elf32_Shdr *shdr, char *string_table)
{
	printf("--- Section header ---");
	printf("\nSection name : %s", string_table + shdr->sh_name);
	printf("\nSection type : ");
	
	switch(shdr->sh_type)
	{
		case SHT_NULL: printf("Section header table entry unused"); break;
		case SHT_PROGBITS: printf("Program data"); break;
		case SHT_SYMTAB: printf("Symbol table"); break;
		case SHT_STRTAB: printf("String table"); break;
		case SHT_RELA: printf("Relocation entries with addends"); break;
		case SHT_HASH: printf("Symbol hash table"); break;
		case SHT_DYNAMIC: printf("Dynamic linking information"); break;
		case SHT_NOTE: printf("Notes"); break;
		case SHT_NOBITS: printf("Program space with no data (bss)"); break;
		case SHT_REL: printf("Relocation entries, no addends"); break;
		case SHT_SHLIB: printf("Reserved"); break;
		case SHT_DYNSYM: printf("Dynamic linker symbol table"); break;
		case SHT_NUM: printf("Number of defined types. "); break;
/* 		case SHT_LOOS: printf("Start OS-specific"); break; */
		case SHT_LOSUNW: printf("Sun-specific low bound. "); break;
		case SHT_SUNW_syminfo: printf("sun ?"); break;
/* 		case SHT_GNU_verdef: printf("Version definition section."); break; */
/* 		case SHT_GNU_verneed: printf("Version needs section."); break; */
/* 		case SHT_GNU_versym: printf("Version symbol table."); break; */
		default : printf("?");
	}
	
	printf("\nSection flags : ");
	
	if(shdr->sh_flags & SHF_WRITE) printf("\n - Writable");
	if(shdr->sh_flags & SHF_ALLOC) printf("\n - Occupies memory during execution");
	if(shdr->sh_flags & SHF_EXECINSTR) printf("\n - Executable");
	if(shdr->sh_flags & SHF_MASKPROC) printf("\n - Processor-specific");
	
	printf("\nFile offset : 0x%x", shdr->sh_offset);
	printf("\nVirtual Address : 0x%x", shdr->sh_addr);

	printf("\n");
}

Elf32_Shdr *GetTextSection(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char
*string_table)
{
	Elf32_Shdr *shdr = shdr_table;
	int i;
	
	for(i = 0; i < hdr->e_shnum; i++)
	{
		if(shdr[i].sh_type == SHT_PROGBITS &&
		   (shdr[i].sh_flags & SHF_ALLOC) != 0 &&
		   (shdr[i].sh_flags & SHF_EXECINSTR) != 0 &&
		   strcmp(GetSymbolName(shdr[i].sh_name, string_table), ".text") == 0)
		{
			return &shdr[i];
		}
	}
	return NULL;
}

Elf32_Shdr *GetDataSection(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table)
{
	Elf32_Shdr *shdr = shdr_table;
	int i;
	
	for(i = 0; i < hdr->e_shnum; i++)
	{
		if(shdr[i].sh_type == SHT_PROGBITS &&
		   (shdr[i].sh_flags & SHF_ALLOC) != 0 &&
		   (shdr[i].sh_flags & SHF_WRITE) != 0 &&
		   strcmp(GetSymbolName(shdr[i].sh_name, string_table), ".data") == 0)
		{
			return &shdr[i];
		}
	}
	return NULL;
}

Elf32_Shdr *GetBSSSection(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table)
{
	Elf32_Shdr *shdr = shdr_table;
	int i;
	
	for(i = 0; i < hdr->e_shnum; i++)
	{
		if(shdr[i].sh_type == SHT_NOBITS &&
		   (shdr[i].sh_flags & SHF_ALLOC) != 0 &&
		   (shdr[i].sh_flags & SHF_WRITE) != 0 &&
		   strcmp(GetSymbolName(shdr[i].sh_name, string_table), ".bss") == 0)
		{
			return &shdr[i];
		}
	}
	return NULL;
}

Elf32_Word GetSectionSize(Elf32_Shdr *shdr)
{
	return shdr->sh_size;
}

Elf32_Addr GetSectionAddr(Elf32_Shdr *shdr)
{
	return shdr->sh_addr;
}

Elf32_Word GetSectionType(Elf32_Shdr *shdr)
{
    return shdr->sh_type;
}

Elf32_Word GetSectionFlags(Elf32_Shdr *shdr)
{
	return shdr->sh_flags;
}


int LoadSection(Elf32_Shdr *shdr, void *buffer, FILE *f)
{
	if(fseek(f, shdr->sh_offset, SEEK_SET) != 0) return -1;
	if(fread(buffer, shdr->sh_size, 1, f) != 1) return -1;
	return 0;
}

char *GetSymbolName(Elf32_Half index, char *string_table)
{
	return string_table + index;
}

Elf32_Shdr *GetSymbolTableSection(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table)
{
	Elf32_Shdr *shdr = shdr_table;
	int i;
	
	for(i = 0; i < hdr->e_shnum; i++)
	{
		if(shdr[i].sh_type == SHT_SYMTAB &&
		   strcmp(GetSymbolName(shdr[i].sh_name, string_table), ".symtab") == 0)
		{
			return &shdr[i];
		}
	}
	return NULL;
}

Elf32_Shdr *GetSymbolStringTableSection(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table)
{
	Elf32_Shdr *shdr = shdr_table;
	int i;
	
	for(i = 0; i < hdr->e_shnum; i++)
	{
		if(shdr[i].sh_type == SHT_STRTAB &&
		   strcmp(GetSymbolName(shdr[i].sh_name, string_table), ".strtab") == 0)
		{
			return &shdr[i];
		}
	}
	return NULL;
}

Elf32_Sym *LoadSymbolTable(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table, FILE *f)
{
/*	Elf32_Shdr *shdr = GetSymbolTableSection(hdr, shdr_table, string_table);*/
	Elf32_Word size = GetSectionSize(shdr_table);
	
	Elf32_Sym *symbol_table = (Elf32_Sym *) malloc(size);
	
	return LoadSection(shdr_table, symbol_table, f) ? NULL : symbol_table;
}

Elf32_Sym *GetSymbol(Elf32_Sym *symbol_table, Elf32_Shdr *shdr_symbol_table, char *name, char
*string_table)
{
	unsigned int i;
	
	for(i = 0; i < shdr_symbol_table->sh_size / shdr_symbol_table->sh_entsize; i++)
	{
		if(strcmp(name, GetSymbolName(symbol_table[i].st_name, string_table)) == 0)
		{
			return &symbol_table[i];
		}
	}
	return NULL;
}

Elf32_Addr GetSymbolAddr(Elf32_Sym *symbol, Elf32_Shdr *shdr_table)
{
	switch(symbol->st_shndx)
	{
		case SHN_ABS:
			return symbol->st_value;
	}
	return shdr_table[symbol->st_shndx].sh_addr + symbol->st_value;
}
