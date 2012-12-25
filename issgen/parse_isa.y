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
#include <string>
#include "cstdlib"
#include "issgen.hpp"
#include <FlexLexer.h>

#define YYERROR_VERBOSE

using namespace issgen;

using std::vector;
using std::string;

/* globals shared across parsers */
extern int isa_lex();
extern void isa_error(char *);

static void warn(char *, Symbol);

/* some globals for parsing isa program */
extern isa_prog *isa_parser_result;

%}

%union {

	char *strval;

	unsigned uint;
	Symbol sym_t;

	VAR_ENV *venv_t;
	FIELD_ENV *fenv_t;

	OP_CON_T *op_con_t;
	OP_CON_ENV *op_con_env_t;

	class oper_base *oper_t;
	vector<class oper_base *> *oper_list_t;

	vector<bin_code *> *code_list_t;
}

%token TK_OP		"op"
%token TK_GRP		"group"
%token TK_VAR		"var"
%token TK_FIELD		"field"

%token <sym_t>  TK_IDENT	"identifier"
%token <uint>   TK_NUM		"integer"
%token <strval> TK_PAT		"boolean pattern"
%token <strval> TK_STR		"string literal"

%type <fenv_t> fieldsec;
%type <fenv_t> field_list;
%type <venv_t> varsec;
%type <venv_t> var_list;
%type <oper_t> op;
%type <oper_list_t> op_list;
%type <code_list_t> pattern;
%type <op_con_env_t> opfld_env;
%type <op_con_t> opfld;



%start prog

%%

prog:		fieldsec varsec op_list {
				isa_parser_result = new isa_prog($1, $2, $3);
			}
			;

fieldsec:	TK_FIELD '{' field_list '}' {
				$$ = $3;
			}
			;

field_list:	TK_IDENT ':' TK_NUM ';' {
				$$ = new FIELD_ENV;
				$$->insert(FIELD_T($1, $3));
			}
	|		field_list TK_IDENT ':' TK_NUM ';' {
				$$ = $1;
				if ($$->find($2)!=$$->end()) {
					warn("Ignoring redeclaration of field ", $2);
				}
				else {
					$$->insert(FIELD_T($2, $4));
				}
			}
			;

varsec:		TK_VAR '{' var_list '}' {
				$$ = $3;
			}
	|		TK_VAR '{' '}' {
				$$ = new VAR_ENV;
			}
			;

var_list:	TK_IDENT ':' TK_IDENT ';' {
				$$ = new VAR_ENV;
				$$->insert(VAR_T($1, $3));
			}
	|		var_list TK_IDENT ':' TK_IDENT ';' {
				$$ = $1;
				if ($$->find($2)!=$$->end()) {
					warn("Ignoring redeclaration of variable ", $2);
				}
				else {
					$$->insert(VAR_T($2, $4));
				}
			}
			;

op_list:	op {
				$$ = new vector<oper_base *>;
				$$->push_back($1);
			}
	|		op_list op {
				$$ = $1;
				$$->push_back($2);
			}
			;

op:			TK_OP TK_IDENT '(' pattern ')' '{' opfld_env '}' {
				$$ = new oper($2, $4, $7);
			}
	|		TK_GRP TK_IDENT '{' op_list '}' {
				$$ = new group($2, $4);
			}
			;


pattern:	TK_PAT {
				$$ = new vector<bin_code *>;
				$$->push_back(new bin_literal($1));
				free($1);
			}
	|		TK_IDENT {
				$$ = new vector<bin_code *>;
				$$->push_back(new bin_ident($1));
			}
	|		pattern ':' TK_PAT {
				$$ = $1;
				$$->push_back(new bin_literal($3));
				free($3);
			}
	|		pattern ':' TK_IDENT {
				$$ = $1;
				$$->push_back(new bin_ident($3));
			}
			;

opfld_env:	opfld {
				$$ = new OP_CON_ENV;	
				$$->insert(*($1));
				delete $1;
			}
	|		opfld_env opfld {
				$$ = $1;
				if ($$->find($2->first)!=$$->end()) {
					warn("Ignoring redeclaration of ", $2->first);
				}
				else {
					$$->insert(*($2));
				}
				delete $2;
			}
			;

opfld:		TK_IDENT '=' TK_STR {
				$$ = new OP_CON_T($1, $3); 
			}
			;
%%

static void warn(char *msg, Symbol ident) {
	fprintf(stderr, "%s %s\n", msg, symbol_get_string(ident).c_str());
}
