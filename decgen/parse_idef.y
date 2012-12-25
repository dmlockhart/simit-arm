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

#include "decode.h"

#define YYERROR_VERBOSE

extern "C" {
int yylex();
}

void yyerror(char *);
extern void push_entry(char *, char *, char *, double);

extern int mylineno;
extern std::vector<DecodeEntry> entries;

%}

%union {
	double fltval;
	char *strval;
}

%token IDEF LBRAC RBRAC COMMA

%token <strval> IDENT  "identifier"
%token <strval> HCONST "heximal constant"
%token <strval> DCONST "decimal constant"
%token <fltval> FCONST "floating point constant"

%start prog

%%

prog: stmt {}
	| prog stmt {};

stmt:	IDEF LBRAC IDENT COMMA HCONST COMMA HCONST COMMA FCONST RBRAC {
		push_entry($3,$5,$7,$9);
	}
	| 	IDEF LBRAC IDENT COMMA HCONST COMMA HCONST COMMA DCONST RBRAC {
		push_entry($3,$5,$7,strtod($9,NULL));
	};

%%

void yyerror (char *s)
{
	std::cerr << s << " near line " << mylineno+1 << std::endl;
}

void push_entry(char *id, char *mask, char *sign, double prob)
{
	DecodeEntry tmp(mask,sign,id,prob);
	entries.push_back(tmp);
	free(id),free(mask),free(sign); /* free the memory */
}

