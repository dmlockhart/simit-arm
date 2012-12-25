/***************************************************************************
                          read_elf.h  -  description
                             -------------------
    begin                : Wed Sep 26 2001
    copyright            : (C) 2001 CEA and Université Paris XI Orsay
    author               : Gilles Mouchard
    email                : gilles.mouchard@lri.fr, gilles.mouchard@cea.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __READELF_H__
#define __READELF_H__
#include "config.h"
#include "bittypes.h"
#include "elf.h"
#include <stdio.h>

extern Elf32_Ehdr *ReadElfHeader(FILE *f);
extern Elf32_Phdr *ReadProgramHeaders(Elf32_Ehdr *hdr, FILE *f);
extern Elf32_Shdr *ReadSectionHeaders(Elf32_Ehdr *hdr, FILE *f);
extern char *LoadStringTable(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, FILE *f);
extern void DumpElfHeader(Elf32_Ehdr *hdr);
extern void DumpProgramHeader(Elf32_Phdr *phdr);
extern void DumpSectionHeader(Elf32_Shdr *shdr, char *string_table);
extern Elf32_Shdr *GetTextSection(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table);
extern Elf32_Shdr *GetDataSection(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table);
extern Elf32_Shdr *GetBSSSection(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table);
extern Elf32_Shdr *GetSymbolTableSection(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table);
extern char *GetSymbolName(Elf32_Half index, char *string_table);
extern Elf32_Word GetSectionSize(Elf32_Shdr *shdr);
extern int LoadSection(Elf32_Shdr *shdr, void *buffer, FILE *f);
extern Elf32_Addr GetSectionAddr(Elf32_Shdr *shdr);
extern Elf32_Shdr *GetSymbolTableSection(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table);
extern Elf32_Sym *LoadSymbolTable(Elf32_Ehdr *hdr, Elf32_Shdr *shdr_table, char *string_table, FILE *f);
extern Elf32_Sym *GetSymbol(Elf32_Sym *symbol_table, Elf32_Shdr *shdr_symbol_table, char *name, char *string_table);
extern Elf32_Addr GetSymbolAddr(Elf32_Sym *symbol, Elf32_Shdr *shdr_table);
extern Elf32_Word GetSectionType(Elf32_Shdr *shdr);
extern Elf32_Word GetSectionFlags(Elf32_Shdr *shdr);

#endif
