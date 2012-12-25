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


#include <fstream>
#include "issgen.hpp"

using namespace issgen;

using std::ifstream;
using std::string;

#define YYLTYPE issgen::yyltype_t
#include "parse_isa.hpp"

#define YY_NO_UNPUT

isa_prog *isa_parser_result;

extern int isa_parse();

const char *isa_filename;
isa_FlexLexer *isa_lexer;

static void isa_lexerror(char c) 
{
	fprintf(stderr, "Unknown character %c at line %d\n",
		c, isa_lexer->lineno());
}


%}

%option c++
%option yylineno
%option noyywrap

BINPAT  [01-]+
DECNUM  [0-9]+

EXP     [Ee][+-]?{DIG}+

STR		\"(\\.|[^\\"])*\"

WHITES  [ \t]*
IDENT   [a-zA-Z_][a-zA-Z_0-9]*


%x opdec
%x comment
%x mcomment

%%

{STR}		{
			isa_lval.strval = strdup(YYText()+1);
			isa_lval.strval[YYLeng()-2]=0;
			return TK_STR;
			}

"group"		return TK_GRP;
"op"		BEGIN(opdec); return TK_OP;

<opdec>{IDENT}	{
			isa_lval.sym_t = symbol_find_or_insert(YYText());
			return TK_IDENT;
			}

<opdec>":"	return ':';
<opdec>"("	return '(';
<opdec>")"	BEGIN(INITIAL); return ')';
<opdec>{BINPAT} {
			isa_lval.strval = strdup(YYText());
			return TK_PAT;
			}
<opdec>{WHITES}	
<opdec>"\n"+		

"var"		return TK_VAR;
"field"		return TK_FIELD;

{IDENT}		isa_lval.sym_t = symbol_find_or_insert(YYText()); return TK_IDENT;
{DECNUM}    isa_lval.uint = atoi(YYText()); return TK_NUM;

"//"			BEGIN(comment);
<comment>"\n"	BEGIN(INITIAL);
<comment>.		/*ignore*/

"/*"			BEGIN(mcomment);
<mcomment>"*/"	BEGIN(INITIAL);
<mcomment>.		/*ignore*/

"{"		return '{';
"}"		return '}';
":"		return ':';
"="		return '=';
";"		return ';';

{WHITES}	
"\n"+		
<<EOF>>		return 0;

.			{
			 isa_lexerror(*YYText());
			}

%%

void isa_error (char *s)
{
	fprintf(stderr, "%s near line %d\n", s, isa_lexer->lineno());
	exit(1);
}

int isa_lex()
{
    return isa_lexer->yylex();                                    
}

isa_prog *isa_parse_wrapper(ifstream *infile, const char *fname)
{
	isa_filename = fname;
	isa_lexer = new isa_FlexLexer(infile);

	isa_parse();

	delete isa_lexer;
	return isa_parser_result;
}
