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

%{

#include <vector>

#include "misc.h"
#include "simconf.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

#define YYERROR_VERBOSE

extern int conf_lex();
extern int mylineno;
extern int arm_decode_regname(const char *);

using namespace simit;
using std::vector;

extern processor_info_t *proc_info;
extern const char *conf_path;

void conf_error(const char *);
static vector<char> *convert_int_to_vchar(uint32_t);

%}

%union {
	unsigned uval;
	char *strval;
	std::vector<char>     *nnstr;
	struct membank_info_t *mem_t;
	struct preload_info_t *pre_t;
	struct reginit_info_t *reg_t;
}

/*%token SEMI COLON COMMA EQUAL*/
/*%token CPU MACH MEM MAP TYPE ADDR SIZE PRE FLE IMG ELF VAL REG*/

%token SEMI    ";"
%token COLON   ":"
%token COMMA   ","
%token EQUAL   "="

%token <nnstr>  STR    "ascii string"
%token <strval> PATH   "filename"
%token <strval> IDENT  "identifier"
%token <strval> HCONST "heximal constant"
%token <strval> DCONST "decimal constant"
%token <strval> ELF    "elf32"
%token <strval> IMG    "image"
%token <strval> VAL    "value"
%token <strval> PRE    "preload"
%token <strval> REG    "reginit"
%token <strval> ADDR   "address"
%token <strval> CPU    "cpu"
%token <strval> MACH   "machine"
%token <strval> MAP    "map"
%token <strval> MEM    "mem_bank"
%token <strval> TYPE   "type"
%token <strval> SIZE   "size"


%type  <uval> map type addr size
%type  <strval> path path2 cpu mach
%type  <nnstr> value valist
%type  <mem_t> mem
%type  <pre_t> pre
%type  <reg_t> reg

%start proc

%%

proc: elem {
	}
	| proc elem {
	};

elem: cpu {
		proc_info->set_cpu_name($1);
	}
	| mach {
		proc_info->set_mach_name($1);
	}
	| mem {
		proc_info->add_membank($1);
	}
	| pre {
		proc_info->add_preload($1);
	}
	| reg {
		proc_info->add_reginit($1);
	}
	;

cpu: CPU COLON IDENT SEMI {
		$$ = $3;
	}
	;

mach: MACH COLON IDENT SEMI {
		$$ = $3;
	}

mem: MEM COLON map COMMA type COMMA addr COMMA size SEMI {
		$$ = new membank_info_t;
		$$->flag = $3 | $5;
		$$->addr = $7;
		$$->size = $9;
	}
	;

map: MAP EQUAL IDENT {
		if ($3[0]=='I' || $3[0]=='i') $$ = MEMBANK_REMAPPED; else $$ = 0;
		free($3);
	}
	;

type: TYPE EQUAL IDENT {
		$$ = 0;
		if (strchr($3, 'R') || strchr($3, 'r')) $$ |= MEMBANK_READABLE;
		if (strchr($3, 'W') || strchr($3, 'w')) $$ |= MEMBANK_WRITABLE;
		free($3);
	}
	;

addr: ADDR EQUAL HCONST {
		$$ = strtoul($3, NULL, 16);
		free($3);
	}
	| ADDR EQUAL DCONST {
		$$ = strtoul($3, NULL, 10);
		free($3);
	}
	;

size: SIZE EQUAL HCONST {
		$$ = strtoul($3, NULL, 16);
		free($3);
	}
	| SIZE EQUAL DCONST {
		$$ = strtoul($3, NULL, 10);
		free($3);
	}
	;

pre: PRE COLON path COMMA addr SEMI {
		$$ = new preload_info_t;
		$$->path = $3;
		$$->addr = $5;
		$$->type = preload_info_t::PRE_IMG;
	}
	| PRE COLON value COMMA addr SEMI {
		$$ = new preload_info_t;
		$$->path = $3;
		$$->addr = $5;
		$$->type = preload_info_t::PRE_VAL;
	}
	| PRE COLON path2 SEMI {
		$$ = new preload_info_t;
		$$->path = $3;
		$$->addr = 0;
		$$->type = preload_info_t::PRE_ELF;
	}
	;

path: IMG EQUAL PATH {
		$$ = $3;
	}
	| IMG EQUAL IDENT {
		$$ = $3;
	}
	;

path2: ELF EQUAL PATH {
		$$ = $3;
	}
	| ELF EQUAL IDENT {
		$$ = $3;
	}
	;

value: VAL EQUAL valist {
		$$ = $3;
	}
	;

valist: STR {
		$$ = $1;
	}
	| HCONST {
		$$ = convert_int_to_vchar(strtoul($1, NULL, 16));
		free($1);
	}
	| DCONST {
		$$ = convert_int_to_vchar(strtoul($1, NULL, 10));
		free($1);
	}
	| valist STR {
		$$ = $1;
		$$->insert($$->end(), $2->begin(), $2->end());
		delete $2;
	}
	| valist HCONST {
		$$ = convert_int_to_vchar(strtoul($2, NULL, 16));
		$$->insert($$->begin(), $1->begin(), $1->end());
		free($2);
		delete $1;
	}
	| valist DCONST {
		$$ = convert_int_to_vchar(strtoul($2, NULL, 10));
		$$->insert($$->begin(), $1->begin(), $1->end());
		free($2);
		delete $1;
	}
	;


reg: REG COLON IDENT EQUAL HCONST SEMI {
		$$ = new reginit_info_t;
		$$->id = arm_decode_regname($3);
		if ($$->id==-1)
		{
			conf_error("Unknown register");
			YYABORT;
		}
		$$->val = strtoul($5, NULL, 16);
		free($3);
		free($5);
	}
	| REG COLON IDENT EQUAL DCONST SEMI {
		$$ = new reginit_info_t;
		$$->id = arm_decode_regname($3);
		if ($$->id==-1)
		{
			conf_error("Unknown register");
			YYABORT;
		}
		$$->val = strtoul($5, NULL, 10);
		free($3);
		free($5);
	}
	;


%%

void conf_error (const char *s)
{
	fprintf(stderr, "Error parsing %s:\n", conf_path);
	fprintf(stderr, "      %s near line %d\n", s, mylineno+1);
	fprintf(stderr, "Correct the above error first.\n");
}

static vector<char> *convert_int_to_vchar(uint32_t ival)
{
	vector<char> *ret = new vector<char>;

#if TARGET_LITTLE_ENDIAN
	ret->push_back((char)ival);
	ret->push_back((char)(ival>>8));
	ret->push_back((char)(ival>>16));
	ret->push_back((char)(ival>>24));
#else
	ret->push_back((char)(ival>>24));
	ret->push_back((char)(ival>>16));
	ret->push_back((char)(ival>>8));
	ret->push_back((char)ival);
#endif
	return ret;
}
