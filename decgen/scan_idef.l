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

#define YY_NO_UNPUT
#define YY_DECL int yylex()
#define yyin dec_in

#include "string.h"
#include "parse_idef.h"
extern FILE *dec_in;
int mylineno;

%}

%option noyywrap

D       [0-9]
L       [a-zA-Z_]
E       [Ee][+-]?{D}+
DECNUM  [1-9][0-9]*|0
HEXNUM  0[xX][0-9a-fA-F]+

WHITES  [ \t]*
IDENT   [a-zA-Z_][a-zA-Z_0-9]*

%x comment
%x directive

%%

"IDEF"		return IDEF;

{IDENT}		yylval.strval = strdup(yytext); return IDENT;
{HEXNUM}	yylval.strval = strdup(yytext); return HCONST;
{DECNUM}	yylval.strval = strdup(yytext); return DCONST;
{D}+{E}             yylval.fltval=strtod(yytext, NULL); return FCONST;
{D}*"."{D}+({E})?   yylval.fltval=strtod(yytext, NULL); return FCONST;
{D}+"."{D}*({E})?   yylval.fltval=strtod(yytext, NULL); return FCONST;


"("		return LBRAC;
")"		return RBRAC;
","		return COMMA;

"/*"			BEGIN(comment);
<comment>"\n"	mylineno++;
<comment>.		/*ignore*/
<comment>"*/"	BEGIN(INITIAL);

"#"				BEGIN(directive);
<directive>.	/*ignore*/
<directive>"\n"	mylineno++; BEGIN(INITIAL);

"\n"			mylineno++;
{WHITES}		/*ignore*/
<<EOF>>			return 0;

.				/*ignore*/

%%
