/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7.12-4996"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         CONF_STYPE
/* Substitute the variable and function names.  */
#define yyparse         conf_parse
#define yylex           conf_lex
#define yyerror         conf_error
#define yylval          conf_lval
#define yychar          conf_char
#define yydebug         conf_debug
#define yynerrs         conf_nerrs

/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 16 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"


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


/* Line 371 of yacc.c  */
#line 103 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parse_conf.h".  */
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
/* Line 387 of yacc.c  */
#line 45 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"

	unsigned uval;
	char *strval;
	std::vector<char>     *nnstr;
	struct membank_info_t *mem_t;
	struct preload_info_t *pre_t;
	struct reginit_info_t *reg_t;


/* Line 387 of yacc.c  */
#line 187 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.cpp"
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 215 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined CONF_STYPE_IS_TRIVIAL && CONF_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   69

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  24
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  33
/* YYNRULES -- Number of states.  */
#define YYNSTATES  79

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   278

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23
};

#if CONF_DEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      23,    28,    39,    43,    47,    51,    55,    59,    63,    70,
      77,    82,    86,    90,    94,    98,   102,   104,   106,   108,
     111,   114,   117,   124
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      25,     0,    -1,    26,    -1,    25,    26,    -1,    27,    -1,
      28,    -1,    29,    -1,    34,    -1,    39,    -1,    18,     4,
       9,     3,    -1,    19,     4,     9,     3,    -1,    21,     4,
      30,     5,    31,     5,    32,     5,    33,     3,    -1,    20,
       6,     9,    -1,    22,     6,     9,    -1,    17,     6,    10,
      -1,    17,     6,    11,    -1,    23,     6,    10,    -1,    23,
       6,    11,    -1,    15,     4,    35,     5,    32,     3,    -1,
      15,     4,    37,     5,    32,     3,    -1,    15,     4,    36,
       3,    -1,    13,     6,     8,    -1,    13,     6,     9,    -1,
      12,     6,     8,    -1,    12,     6,     9,    -1,    14,     6,
      38,    -1,     7,    -1,    10,    -1,    11,    -1,    38,     7,
      -1,    38,    10,    -1,    38,    11,    -1,    16,     4,     9,
       6,    10,     3,    -1,    16,     4,     9,     6,    11,     3,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    92,    92,    94,    97,   100,   103,   106,   109,   114,
     119,   123,   131,   137,   145,   149,   155,   159,   165,   171,
     177,   185,   188,   193,   196,   201,   206,   209,   213,   217,
     222,   228,   237,   249
};
#endif

#if CONF_DEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\";\"", "\":\"", "\",\"", "\"=\"",
  "\"ascii string\"", "\"filename\"", "\"identifier\"",
  "\"heximal constant\"", "\"decimal constant\"", "\"elf32\"", "\"image\"",
  "\"value\"", "\"preload\"", "\"reginit\"", "\"address\"", "\"cpu\"",
  "\"machine\"", "\"map\"", "\"mem_bank\"", "\"type\"", "\"size\"",
  "$accept", "proc", "elem", "cpu", "mach", "mem", "map", "type", "addr",
  "size", "pre", "path", "path2", "value", "valist", "reg", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    24,    25,    25,    26,    26,    26,    26,    26,    27,
      28,    29,    30,    31,    32,    32,    33,    33,    34,    34,
      34,    35,    35,    36,    36,    37,    38,    38,    38,    38,
      38,    38,    39,    39
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     4,
       4,    10,     3,     3,     3,     3,     3,     3,     6,     6,
       4,     3,     3,     3,     3,     3,     1,     1,     1,     2,
       2,     2,     6,     6
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     2,     4,     5,
       6,     7,     8,     0,     0,     0,     0,     0,     1,     3,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    20,     0,     0,     9,    10,
       0,     0,    23,    24,    21,    22,    26,    27,    28,    25,
       0,     0,     0,     0,     0,    12,     0,     0,    29,    30,
      31,     0,    18,    19,    32,    33,     0,     0,    14,    15,
      13,     0,     0,     0,     0,     0,    11,    16,    17
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     6,     7,     8,     9,    10,    30,    57,    51,    74,
      11,    23,    24,    25,    49,    12
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -37
static const yytype_int8 yypact[] =
{
     -13,    14,    17,    32,    33,    34,     1,   -37,   -37,   -37,
     -37,   -37,   -37,    11,    -5,    30,    31,     8,   -37,   -37,
      35,    36,    37,    39,    42,    41,    43,    44,    45,    46,
      48,     6,    18,     0,    38,   -37,    38,    19,   -37,   -37,
      47,    28,   -37,   -37,   -37,   -37,   -37,   -37,   -37,     2,
      51,    55,    56,    57,    58,   -37,    59,    49,   -37,   -37,
     -37,    22,   -37,   -37,   -37,   -37,    53,    38,   -37,   -37,
     -37,    61,    40,    62,    64,    24,   -37,   -37,   -37
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -37,   -37,    63,   -37,   -37,   -37,   -37,   -37,   -36,   -37,
     -37,   -37,   -37,   -37,   -37,   -37
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      52,    18,     1,     2,    26,     3,     4,    46,     5,    58,
      47,    48,    59,    60,    42,    43,     1,     2,    13,     3,
       4,    14,     5,    20,    21,    22,    44,    45,    29,    53,
      54,    71,    68,    69,    77,    78,    15,    16,    17,    27,
      28,    31,    32,    33,    34,    35,    36,    38,    39,    37,
      56,     0,    40,    41,    67,    50,    55,    61,    62,    63,
      64,    65,    70,    73,     0,    66,    72,    76,    75,    19
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-37)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
      36,     0,    15,    16,     9,    18,    19,     7,    21,     7,
      10,    11,    10,    11,     8,     9,    15,    16,     4,    18,
      19,     4,    21,    12,    13,    14,     8,     9,    20,    10,
      11,    67,    10,    11,    10,    11,     4,     4,     4,     9,
       9,     6,     6,     6,     5,     3,     5,     3,     3,     6,
      22,    -1,     6,     5,     5,    17,     9,     6,     3,     3,
       3,     3,     9,    23,    -1,     6,     5,     3,     6,     6
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    15,    16,    18,    19,    21,    25,    26,    27,    28,
      29,    34,    39,     4,     4,     4,     4,     4,     0,    26,
      12,    13,    14,    35,    36,    37,     9,     9,     9,    20,
      30,     6,     6,     6,     5,     3,     5,     6,     3,     3,
       6,     5,     8,     9,     8,     9,     7,    10,    11,    38,
      17,    32,    32,    10,    11,     9,    22,    31,     7,    10,
      11,     6,     3,     3,     3,     3,     6,     5,    10,    11,
       9,    32,     5,    23,    33,     6,     3,    10,    11
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if CONF_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !CONF_DEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !CONF_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YYUSE (yytype);
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1787 of yacc.c  */
#line 92 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
	}
    break;

  case 3:
/* Line 1787 of yacc.c  */
#line 94 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
	}
    break;

  case 4:
/* Line 1787 of yacc.c  */
#line 97 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		proc_info->set_cpu_name((yyvsp[(1) - (1)].strval));
	}
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 100 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		proc_info->set_mach_name((yyvsp[(1) - (1)].strval));
	}
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 103 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		proc_info->add_membank((yyvsp[(1) - (1)].mem_t));
	}
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 106 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		proc_info->add_preload((yyvsp[(1) - (1)].pre_t));
	}
    break;

  case 8:
/* Line 1787 of yacc.c  */
#line 109 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		proc_info->add_reginit((yyvsp[(1) - (1)].reg_t));
	}
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 114 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.strval) = (yyvsp[(3) - (4)].strval);
	}
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 119 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.strval) = (yyvsp[(3) - (4)].strval);
	}
    break;

  case 11:
/* Line 1787 of yacc.c  */
#line 123 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.mem_t) = new membank_info_t;
		(yyval.mem_t)->flag = (yyvsp[(3) - (10)].uval) | (yyvsp[(5) - (10)].uval);
		(yyval.mem_t)->addr = (yyvsp[(7) - (10)].uval);
		(yyval.mem_t)->size = (yyvsp[(9) - (10)].uval);
	}
    break;

  case 12:
/* Line 1787 of yacc.c  */
#line 131 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		if ((yyvsp[(3) - (3)].strval)[0]=='I' || (yyvsp[(3) - (3)].strval)[0]=='i') (yyval.uval) = MEMBANK_REMAPPED; else (yyval.uval) = 0;
		free((yyvsp[(3) - (3)].strval));
	}
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 137 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.uval) = 0;
		if (strchr((yyvsp[(3) - (3)].strval), 'R') || strchr((yyvsp[(3) - (3)].strval), 'r')) (yyval.uval) |= MEMBANK_READABLE;
		if (strchr((yyvsp[(3) - (3)].strval), 'W') || strchr((yyvsp[(3) - (3)].strval), 'w')) (yyval.uval) |= MEMBANK_WRITABLE;
		free((yyvsp[(3) - (3)].strval));
	}
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 145 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.uval) = strtoul((yyvsp[(3) - (3)].strval), NULL, 16);
		free((yyvsp[(3) - (3)].strval));
	}
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 149 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.uval) = strtoul((yyvsp[(3) - (3)].strval), NULL, 10);
		free((yyvsp[(3) - (3)].strval));
	}
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 155 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.uval) = strtoul((yyvsp[(3) - (3)].strval), NULL, 16);
		free((yyvsp[(3) - (3)].strval));
	}
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 159 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.uval) = strtoul((yyvsp[(3) - (3)].strval), NULL, 10);
		free((yyvsp[(3) - (3)].strval));
	}
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 165 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.pre_t) = new preload_info_t;
		(yyval.pre_t)->path = (yyvsp[(3) - (6)].strval);
		(yyval.pre_t)->addr = (yyvsp[(5) - (6)].uval);
		(yyval.pre_t)->type = preload_info_t::PRE_IMG;
	}
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 171 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.pre_t) = new preload_info_t;
		(yyval.pre_t)->path = (yyvsp[(3) - (6)].nnstr);
		(yyval.pre_t)->addr = (yyvsp[(5) - (6)].uval);
		(yyval.pre_t)->type = preload_info_t::PRE_VAL;
	}
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 177 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.pre_t) = new preload_info_t;
		(yyval.pre_t)->path = (yyvsp[(3) - (4)].strval);
		(yyval.pre_t)->addr = 0;
		(yyval.pre_t)->type = preload_info_t::PRE_ELF;
	}
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 185 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.strval) = (yyvsp[(3) - (3)].strval);
	}
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 188 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.strval) = (yyvsp[(3) - (3)].strval);
	}
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 193 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.strval) = (yyvsp[(3) - (3)].strval);
	}
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 196 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.strval) = (yyvsp[(3) - (3)].strval);
	}
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 201 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.nnstr) = (yyvsp[(3) - (3)].nnstr);
	}
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 206 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.nnstr) = (yyvsp[(1) - (1)].nnstr);
	}
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 209 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.nnstr) = convert_int_to_vchar(strtoul((yyvsp[(1) - (1)].strval), NULL, 16));
		free((yyvsp[(1) - (1)].strval));
	}
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 213 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.nnstr) = convert_int_to_vchar(strtoul((yyvsp[(1) - (1)].strval), NULL, 10));
		free((yyvsp[(1) - (1)].strval));
	}
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 217 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.nnstr) = (yyvsp[(1) - (2)].nnstr);
		(yyval.nnstr)->insert((yyval.nnstr)->end(), (yyvsp[(2) - (2)].nnstr)->begin(), (yyvsp[(2) - (2)].nnstr)->end());
		delete (yyvsp[(2) - (2)].nnstr);
	}
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 222 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.nnstr) = convert_int_to_vchar(strtoul((yyvsp[(2) - (2)].strval), NULL, 16));
		(yyval.nnstr)->insert((yyval.nnstr)->begin(), (yyvsp[(1) - (2)].nnstr)->begin(), (yyvsp[(1) - (2)].nnstr)->end());
		free((yyvsp[(2) - (2)].strval));
		delete (yyvsp[(1) - (2)].nnstr);
	}
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 228 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.nnstr) = convert_int_to_vchar(strtoul((yyvsp[(2) - (2)].strval), NULL, 10));
		(yyval.nnstr)->insert((yyval.nnstr)->begin(), (yyvsp[(1) - (2)].nnstr)->begin(), (yyvsp[(1) - (2)].nnstr)->end());
		free((yyvsp[(2) - (2)].strval));
		delete (yyvsp[(1) - (2)].nnstr);
	}
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 237 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.reg_t) = new reginit_info_t;
		(yyval.reg_t)->id = arm_decode_regname((yyvsp[(3) - (6)].strval));
		if ((yyval.reg_t)->id==-1)
		{
			conf_error("Unknown register");
			YYABORT;
		}
		(yyval.reg_t)->val = strtoul((yyvsp[(5) - (6)].strval), NULL, 16);
		free((yyvsp[(3) - (6)].strval));
		free((yyvsp[(5) - (6)].strval));
	}
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 249 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"
    {
		(yyval.reg_t) = new reginit_info_t;
		(yyval.reg_t)->id = arm_decode_regname((yyvsp[(3) - (6)].strval));
		if ((yyval.reg_t)->id==-1)
		{
			conf_error("Unknown register");
			YYABORT;
		}
		(yyval.reg_t)->val = strtoul((yyvsp[(5) - (6)].strval), NULL, 10);
		free((yyvsp[(3) - (6)].strval));
		free((yyvsp[(5) - (6)].strval));
	}
    break;


/* Line 1787 of yacc.c  */
#line 1761 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2050 of yacc.c  */
#line 264 "/home/vmm/dev/projects/SimIt-ARM/emulator/parse_conf.y"


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
