/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_CONF_HOME_VMM_DEV_PROJECTS_SIMIT_ARM_EMULATOR_PARSE_CONF_H_INCLUDED
# define YY_CONF_HOME_VMM_DEV_PROJECTS_SIMIT_ARM_EMULATOR_PARSE_CONF_H_INCLUDED
/* Enabling traces.  */
#ifndef CONF_DEBUG
# if defined YYDEBUG
#  if YYDEBUG
#   define CONF_DEBUG 1
#  else
#   define CONF_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define CONF_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined CONF_DEBUG */
#if CONF_DEBUG
extern int conf_debug;
#endif

/* Tokens.  */
#ifndef CONF_TOKENTYPE
# define CONF_TOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum conf_tokentype {
     SEMI = 258,
     COLON = 259,
     COMMA = 260,
     EQUAL = 261,
     STR = 262,
     PATH = 263,
     IDENT = 264,
     HCONST = 265,
     DCONST = 266,
     ELF = 267,
     IMG = 268,
     VAL = 269,
     PRE = 270,
     REG = 271,
     ADDR = 272,
     CPU = 273,
     MACH = 274,
     MAP = 275,
     MEM = 276,
     TYPE = 277,
     SIZE = 278
   };
#endif


#if ! defined CONF_STYPE && ! defined CONF_STYPE_IS_DECLARED
typedef union CONF_STYPE
{
/* Line 2053 of yacc.c  */
#line 45 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"

	unsigned uval;
	char *strval;
	std::vector<char>     *nnstr;
	struct membank_info_t *mem_t;
	struct preload_info_t *pre_t;
	struct reginit_info_t *reg_t;


/* Line 2053 of yacc.c  */
#line 98 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.h"
} CONF_STYPE;
# define CONF_STYPE_IS_TRIVIAL 1
# define conf_stype CONF_STYPE /* obsolescent; will be withdrawn */
# define CONF_STYPE_IS_DECLARED 1
#endif

extern CONF_STYPE conf_lval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int conf_parse (void *YYPARSE_PARAM);
#else
int conf_parse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int conf_parse (void);
#else
int conf_parse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_CONF_HOME_VMM_DEV_PROJECTS_SIMIT_ARM_EMULATOR_PARSE_CONF_H_INCLUDED  */
