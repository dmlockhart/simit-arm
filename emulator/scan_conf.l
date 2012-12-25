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

#include <cstring>
#include <cstdio>
#include <cctype>
#include <vector>
#include "parse_conf.hpp"

int mylineno;

static std::vector<char> *lex_str(char *val, unsigned int len);

#define YY_NO_UNPUT
#define yyin conf_in

%}

%option noyywrap

D       [0-9]
L       [a-zA-Z_]
E       [Ee][+-]?{D}+
DECNUM  [1-9][0-9]*|0
HEXNUM  0[xX][0-9a-fA-F]+
WHITES  [ \t]*
IDENT   [a-zA-Z_][a-zA-Z_0-9]*
PATH	[a-zA-Z_0-9/\.\-]+
STR		\"(\\.|[^\\"])*\"

%x comment
%x directive

%%

"cpu"				return CPU;
"mach"				return MACH;
"mem_bank"			return MEM;
"map"				return MAP;
"type"				return TYPE;
"address"			return ADDR;
"addr"				return ADDR;
"size"				return SIZE;
"image"				return IMG;
"elf32"				return ELF;
"value"				return VAL;
"preload"			return PRE;
"reginit"			return REG;

":"		return COLON;
";"		return SEMI;
","		return COMMA;
"="		return EQUAL;

{STR}		conf_lval.nnstr  = lex_str(yytext, yyleng); return STR;
{IDENT}		conf_lval.strval = strdup(yytext); return IDENT;
{HEXNUM}	conf_lval.strval = strdup(yytext); return HCONST;
{DECNUM}	conf_lval.strval = strdup(yytext); return DCONST;
{PATH}		conf_lval.strval = strdup(yytext); return PATH;

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

static char tohex(char val)
{
	if (val>='0' && val<='9') return val - '0';

	return val - 'a' + 10;
}

using std::vector;

static vector<char> *lex_str(char *val, unsigned int len)
{
	char tmp;
	unsigned pos = 0;
	vector<char> *ret = new vector<char>;

	/* skip the quotes */
	pos++;
	len--;

	while (pos < len)
	{
		/* escape sequence */
		if (val[pos] == '\\')
		{
			if (++pos >= len)
				goto error;

			switch (val[pos])
			{
				case 'a': tmp = '\a'; break;
				case 'b': tmp = '\b'; break;
				case 'f': tmp = '\f'; break;
				case 'n': tmp = '\n'; break;
				case 'r': tmp = '\r'; break;
				case 't': tmp = '\t'; break;
				case 'v': tmp = '\v'; break;
				case '\\': tmp = '\\'; break;
				case '?' : tmp = '\?'; break;
				case '\'': tmp = '\''; break;
				case '\"': tmp = '\"'; break;
				case 'x': 
				{
					if (pos + 2 >= len ||
						!isxdigit(val[pos + 1]) || !isxdigit(val[pos + 2]))
						goto error;

					tmp = (tohex(val[pos + 1]) << 4) |
							tohex(val[pos + 2]);

					pos++;
					break;
				}
				default:
				{
					if (pos + 2 >= len ||
						val[pos] < '0' || val[pos] > '3' ||
						val[pos+1] < '0' || val[pos+1] > '7' ||
						val[pos+2] < '0' || val[pos+2] > '7')
						goto error;

					tmp =   ((val[pos] - '0') << 6) |
							((val[pos+1] - '0') << 3)  |
							(val[pos+2] - '0');

					pos += 2;
					break;
				}
			}

			ret->push_back(tmp);
			pos++;
		}
		else
			ret->push_back(val[pos]);

		pos++;
	}

	return ret;

error:
	fprintf(stderr, "Error parsing ASCII string, character %c, at %d:%d\n",
		val[pos], mylineno + 1, pos);

	delete ret;
	return NULL;

}
