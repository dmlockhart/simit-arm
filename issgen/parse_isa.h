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

#ifndef YY_ISA_HOME_VMM_DEV_PROJECTS_SIMIT_ARM_ISSGEN_PARSE_ISA_H_INCLUDED
# define YY_ISA_HOME_VMM_DEV_PROJECTS_SIMIT_ARM_ISSGEN_PARSE_ISA_H_INCLUDED
/* Enabling traces.  */
#ifndef ISA_DEBUG
# if defined YYDEBUG
#  if YYDEBUG
#   define ISA_DEBUG 1
#  else
#   define ISA_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define ISA_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined ISA_DEBUG */
#if ISA_DEBUG
extern int isa_debug;
#endif

/* Tokens.  */
#ifndef ISA_TOKENTYPE
# define ISA_TOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum isa_tokentype {
     TK_OP = 258,
     TK_GRP = 259,
     TK_VAR = 260,
     TK_FIELD = 261,
     TK_IDENT = 262,
     TK_NUM = 263,
     TK_PAT = 264,
     TK_STR = 265
   };
#endif


#if ! defined ISA_STYPE && ! defined ISA_STYPE_IS_DECLARED
typedef union ISA_STYPE
{
/* Line 2053 of yacc.c  */
#line 45 "/home/vmm/dev/projects/SimIt-ARM/issgen/parse_isa.y"


	char *strval;

	unsigned uint;
	Symbol sym_t;

	VAR_ENV *venv_t;
	FIELD_ENV *fenv_t;

	OP_CON_T *op_con_t;
	OP_CON_ENV *op_con_env_t;

	class oper_base *oper_t;
	std::vector<class oper_base *> *oper_list_t;

	std::vector<bin_code *> *code_list_t;


/* Line 2053 of yacc.c  */
#line 95 "/home/vmm/dev/projects/SimIt-ARM/issgen/parse_isa.h"
} ISA_STYPE;
# define ISA_STYPE_IS_TRIVIAL 1
# define isa_stype ISA_STYPE /* obsolescent; will be withdrawn */
# define ISA_STYPE_IS_DECLARED 1
#endif

extern ISA_STYPE isa_lval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int isa_parse (void *YYPARSE_PARAM);
#else
int isa_parse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int isa_parse (void);
#else
int isa_parse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_ISA_HOME_VMM_DEV_PROJECTS_SIMIT_ARM_ISSGEN_PARSE_ISA_H_INCLUDED  */
