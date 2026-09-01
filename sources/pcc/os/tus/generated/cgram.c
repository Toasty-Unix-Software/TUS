/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 151 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"

# include "pass1.h"
# include <stdarg.h>
# include <string.h>
# include <stdlib.h>

#undef n_type
#define n_type ptype
#undef n_qual
#define n_qual pqual
#undef n_df
#define n_df pdf

int fun_inline;	/* Reading an inline function */
int oldstyle;	/* Current function being defined */
static struct symtab *xnf;
extern int enummer, tvaloff, inattr;
extern struct rstack *rpole;
static int alwinl;
P1ND *cftnod;
static int attrwarn = 1;

#define	NORETYP	SNOCREAT /* no return type, save in unused field in symtab */

struct genlist {
	struct genlist *next;
	P1ND *p;
	TWORD t;
};

       P1ND *bdty(int op, ...);
static void fend(void);
static void fundef(P1ND *tp, P1ND *p);
static void olddecl(P1ND *p, P1ND *a);
static struct symtab *init_declarator(P1ND *tn, P1ND *p, int assign, P1ND *a,
	char *as);
static void resetbc(int mask);
static void swend(void);
static void addcase(P1ND *p);
#ifdef GCC_COMPAT
static void gcccase(P1ND *p, P1ND *);
#endif
static struct attr *gcc_attr_wrapper(P1ND *p);
static void adddef(void);
static void savebc(void);
static void swstart(int, TWORD);
static void genswitch(int, TWORD, struct swents **, int);
static char *mkpstr(char *str);
static struct initctx *clbrace(P1ND *);
static P1ND *cmop(P1ND *l, P1ND *r);
static P1ND *xcmop(P1ND *out, P1ND *in, P1ND *str);
static void mkxasm(char *str, P1ND *p);
static P1ND *xasmop(char *str, P1ND *p);
static P1ND *biop(int op, P1ND *l, P1ND *r);
static void flend(void);
static P1ND *gccexpr(int bn, P1ND *q);
static char * simname(char *s);
static P1ND *tyof(P1ND *);	/* COMPAT_GCC */
static P1ND *voidcon(void);
static P1ND *funargs(P1ND *p);
static void oldargs(P1ND *p);
static void uawarn(P1ND *p, char *s);
static int con_e(P1ND *p);
static void dainit(struct initctx *, P1ND *d, P1ND *a);
static P1ND *tymfix(P1ND *p);
static P1ND *namekill(P1ND *p, int clr);
static P1ND *aryfix(P1ND *p);
static P1ND *dogen(struct genlist *g, P1ND *e);
static struct genlist *newgen(P1ND *p, P1ND *q);
static struct genlist *addgen(struct genlist *g, struct genlist *h);

static void savlab(int);
static void xcbranch(P1ND *, int);
extern int *mkclabs(void);

#define	TYMFIX(inp) { \
	P1ND *pp = inp; \
	inp = tymerge(pp->n_left, pp->n_right); \
	p1nfree(pp->n_left); p1nfree(pp); }

struct xalloc;
extern struct xalloc *bkpole, *sapole;
extern int cbkp, cstp;
extern int usdnodes;
struct bks {
	struct xalloc *ptr;
	int off;
};

/*
 * State for saving current switch state (when nested switches).
 */
struct savbc {
	struct savbc *next;
	int brklab;
	int contlab;
	int flostat;
	int swx;
	struct xalloc *bkptr;
	int bkoff;
	struct xalloc *stptr;
	int stoff;
	int numnode;
} *savbc, *savctx;


#line 178 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    C_STRING = 258,                /* C_STRING  */
    C_ICON = 259,                  /* C_ICON  */
    C_FCON = 260,                  /* C_FCON  */
    C_NAME = 261,                  /* C_NAME  */
    C_TYPENAME = 262,              /* C_TYPENAME  */
    C_ANDAND = 263,                /* C_ANDAND  */
    C_OROR = 264,                  /* C_OROR  */
    C_GOTO = 265,                  /* C_GOTO  */
    C_RETURN = 266,                /* C_RETURN  */
    C_TYPE = 267,                  /* C_TYPE  */
    C_CLASS = 268,                 /* C_CLASS  */
    C_ASOP = 269,                  /* C_ASOP  */
    C_RELOP = 270,                 /* C_RELOP  */
    C_EQUOP = 271,                 /* C_EQUOP  */
    C_DIVOP = 272,                 /* C_DIVOP  */
    C_SHIFTOP = 273,               /* C_SHIFTOP  */
    C_INCOP = 274,                 /* C_INCOP  */
    C_UNOP = 275,                  /* C_UNOP  */
    C_STROP = 276,                 /* C_STROP  */
    C_STRUCT = 277,                /* C_STRUCT  */
    C_QUALIFIER = 278,             /* C_QUALIFIER  */
    C_FUNSPEC = 279,               /* C_FUNSPEC  */
    C_IF = 280,                    /* C_IF  */
    C_ELSE = 281,                  /* C_ELSE  */
    C_SWITCH = 282,                /* C_SWITCH  */
    C_BREAK = 283,                 /* C_BREAK  */
    C_CONTINUE = 284,              /* C_CONTINUE  */
    C_WHILE = 285,                 /* C_WHILE  */
    C_DO = 286,                    /* C_DO  */
    C_FOR = 287,                   /* C_FOR  */
    C_DEFAULT = 288,               /* C_DEFAULT  */
    C_CASE = 289,                  /* C_CASE  */
    C_SIZEOF = 290,                /* C_SIZEOF  */
    C_ENUM = 291,                  /* C_ENUM  */
    C_ELLIPSIS = 292,              /* C_ELLIPSIS  */
    C_ASM = 293,                   /* C_ASM  */
    NOMATCH = 294,                 /* NOMATCH  */
    C_TYPEOF = 295,                /* C_TYPEOF  */
    C_ATTRIBUTE = 296,             /* C_ATTRIBUTE  */
    PCC_OFFSETOF = 297,            /* PCC_OFFSETOF  */
    GCC_DESIG = 298,               /* GCC_DESIG  */
    C_STATICASSERT = 299,          /* C_STATICASSERT  */
    C_ALIGNAS = 300,               /* C_ALIGNAS  */
    C_ALIGNOF = 301,               /* C_ALIGNOF  */
    C_GENERIC = 302,               /* C_GENERIC  */
    C_ATOMIC = 303                 /* C_ATOMIC  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define C_STRING 258
#define C_ICON 259
#define C_FCON 260
#define C_NAME 261
#define C_TYPENAME 262
#define C_ANDAND 263
#define C_OROR 264
#define C_GOTO 265
#define C_RETURN 266
#define C_TYPE 267
#define C_CLASS 268
#define C_ASOP 269
#define C_RELOP 270
#define C_EQUOP 271
#define C_DIVOP 272
#define C_SHIFTOP 273
#define C_INCOP 274
#define C_UNOP 275
#define C_STROP 276
#define C_STRUCT 277
#define C_QUALIFIER 278
#define C_FUNSPEC 279
#define C_IF 280
#define C_ELSE 281
#define C_SWITCH 282
#define C_BREAK 283
#define C_CONTINUE 284
#define C_WHILE 285
#define C_DO 286
#define C_FOR 287
#define C_DEFAULT 288
#define C_CASE 289
#define C_SIZEOF 290
#define C_ENUM 291
#define C_ELLIPSIS 292
#define C_ASM 293
#define NOMATCH 294
#define C_TYPEOF 295
#define C_ATTRIBUTE 296
#define PCC_OFFSETOF 297
#define GCC_DESIG 298
#define C_STATICASSERT 299
#define C_ALIGNAS 300
#define C_ALIGNOF 301
#define C_GENERIC 302
#define C_ATOMIC 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 258 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"

	TWORD type;
	int intval;
	P1ND *nodep;
	struct symtab *symp;
	struct rstack *rp;
	char *strp;
	struct bks *bkp;
	struct flt flt;
	struct lexint li;
	struct genlist *g;
	struct initctx *ctx;

#line 341 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_C_STRING = 3,                   /* C_STRING  */
  YYSYMBOL_C_ICON = 4,                     /* C_ICON  */
  YYSYMBOL_C_FCON = 5,                     /* C_FCON  */
  YYSYMBOL_C_NAME = 6,                     /* C_NAME  */
  YYSYMBOL_C_TYPENAME = 7,                 /* C_TYPENAME  */
  YYSYMBOL_C_ANDAND = 8,                   /* C_ANDAND  */
  YYSYMBOL_C_OROR = 9,                     /* C_OROR  */
  YYSYMBOL_C_GOTO = 10,                    /* C_GOTO  */
  YYSYMBOL_C_RETURN = 11,                  /* C_RETURN  */
  YYSYMBOL_C_TYPE = 12,                    /* C_TYPE  */
  YYSYMBOL_C_CLASS = 13,                   /* C_CLASS  */
  YYSYMBOL_C_ASOP = 14,                    /* C_ASOP  */
  YYSYMBOL_C_RELOP = 15,                   /* C_RELOP  */
  YYSYMBOL_C_EQUOP = 16,                   /* C_EQUOP  */
  YYSYMBOL_C_DIVOP = 17,                   /* C_DIVOP  */
  YYSYMBOL_C_SHIFTOP = 18,                 /* C_SHIFTOP  */
  YYSYMBOL_C_INCOP = 19,                   /* C_INCOP  */
  YYSYMBOL_C_UNOP = 20,                    /* C_UNOP  */
  YYSYMBOL_C_STROP = 21,                   /* C_STROP  */
  YYSYMBOL_C_STRUCT = 22,                  /* C_STRUCT  */
  YYSYMBOL_C_QUALIFIER = 23,               /* C_QUALIFIER  */
  YYSYMBOL_C_FUNSPEC = 24,                 /* C_FUNSPEC  */
  YYSYMBOL_C_IF = 25,                      /* C_IF  */
  YYSYMBOL_C_ELSE = 26,                    /* C_ELSE  */
  YYSYMBOL_C_SWITCH = 27,                  /* C_SWITCH  */
  YYSYMBOL_C_BREAK = 28,                   /* C_BREAK  */
  YYSYMBOL_C_CONTINUE = 29,                /* C_CONTINUE  */
  YYSYMBOL_C_WHILE = 30,                   /* C_WHILE  */
  YYSYMBOL_C_DO = 31,                      /* C_DO  */
  YYSYMBOL_C_FOR = 32,                     /* C_FOR  */
  YYSYMBOL_C_DEFAULT = 33,                 /* C_DEFAULT  */
  YYSYMBOL_C_CASE = 34,                    /* C_CASE  */
  YYSYMBOL_C_SIZEOF = 35,                  /* C_SIZEOF  */
  YYSYMBOL_C_ENUM = 36,                    /* C_ENUM  */
  YYSYMBOL_C_ELLIPSIS = 37,                /* C_ELLIPSIS  */
  YYSYMBOL_C_ASM = 38,                     /* C_ASM  */
  YYSYMBOL_NOMATCH = 39,                   /* NOMATCH  */
  YYSYMBOL_C_TYPEOF = 40,                  /* C_TYPEOF  */
  YYSYMBOL_C_ATTRIBUTE = 41,               /* C_ATTRIBUTE  */
  YYSYMBOL_PCC_OFFSETOF = 42,              /* PCC_OFFSETOF  */
  YYSYMBOL_GCC_DESIG = 43,                 /* GCC_DESIG  */
  YYSYMBOL_C_STATICASSERT = 44,            /* C_STATICASSERT  */
  YYSYMBOL_C_ALIGNAS = 45,                 /* C_ALIGNAS  */
  YYSYMBOL_C_ALIGNOF = 46,                 /* C_ALIGNOF  */
  YYSYMBOL_C_GENERIC = 47,                 /* C_GENERIC  */
  YYSYMBOL_C_ATOMIC = 48,                  /* C_ATOMIC  */
  YYSYMBOL_49_ = 49,                       /* ','  */
  YYSYMBOL_50_ = 50,                       /* '='  */
  YYSYMBOL_51_ = 51,                       /* '?'  */
  YYSYMBOL_52_ = 52,                       /* ':'  */
  YYSYMBOL_53_ = 53,                       /* '|'  */
  YYSYMBOL_54_ = 54,                       /* '^'  */
  YYSYMBOL_55_ = 55,                       /* '&'  */
  YYSYMBOL_56_ = 56,                       /* '+'  */
  YYSYMBOL_57_ = 57,                       /* '-'  */
  YYSYMBOL_58_ = 58,                       /* '*'  */
  YYSYMBOL_59_ = 59,                       /* '['  */
  YYSYMBOL_60_ = 60,                       /* '('  */
  YYSYMBOL_61_ = 61,                       /* ';'  */
  YYSYMBOL_62_ = 62,                       /* ')'  */
  YYSYMBOL_63_ = 63,                       /* ']'  */
  YYSYMBOL_64_ = 64,                       /* '{'  */
  YYSYMBOL_65_ = 65,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 66,                  /* $accept  */
  YYSYMBOL_ext_def_list = 67,              /* ext_def_list  */
  YYSYMBOL_external_def = 68,              /* external_def  */
  YYSYMBOL_funtype = 69,                   /* funtype  */
  YYSYMBOL_kr_args = 70,                   /* kr_args  */
  YYSYMBOL_declaration_specifiers = 71,    /* declaration_specifiers  */
  YYSYMBOL_merge_attribs = 72,             /* merge_attribs  */
  YYSYMBOL_type_sq = 73,                   /* type_sq  */
  YYSYMBOL_cf_spec = 74,                   /* cf_spec  */
  YYSYMBOL_typeof = 75,                    /* typeof  */
  YYSYMBOL_attribute_specifier = 76,       /* attribute_specifier  */
  YYSYMBOL_attribute_list = 77,            /* attribute_list  */
  YYSYMBOL_attribute = 78,                 /* attribute  */
  YYSYMBOL_declarator = 79,                /* declarator  */
  YYSYMBOL_ecq = 80,                       /* ecq  */
  YYSYMBOL_r = 81,                         /* r  */
  YYSYMBOL_c = 82,                         /* c  */
  YYSYMBOL_type_qualifier_list = 83,       /* type_qualifier_list  */
  YYSYMBOL_identifier_list = 84,           /* identifier_list  */
  YYSYMBOL_parameter_type_list = 85,       /* parameter_type_list  */
  YYSYMBOL_parameter_list = 86,            /* parameter_list  */
  YYSYMBOL_parameter_declaration = 87,     /* parameter_declaration  */
  YYSYMBOL_abstract_declarator = 88,       /* abstract_declarator  */
  YYSYMBOL_ib2 = 89,                       /* ib2  */
  YYSYMBOL_maybe_r = 90,                   /* maybe_r  */
  YYSYMBOL_arg_dcl_list = 91,              /* arg_dcl_list  */
  YYSYMBOL_arg_declaration = 92,           /* arg_declaration  */
  YYSYMBOL_arg_param_list = 93,            /* arg_param_list  */
  YYSYMBOL_block_item_list = 94,           /* block_item_list  */
  YYSYMBOL_block_item = 95,                /* block_item  */
  YYSYMBOL_declaration = 96,               /* declaration  */
  YYSYMBOL_init_declarator_list = 97,      /* init_declarator_list  */
  YYSYMBOL_98_1 = 98,                      /* @1  */
  YYSYMBOL_enum_dcl = 99,                  /* enum_dcl  */
  YYSYMBOL_enum_head = 100,                /* enum_head  */
  YYSYMBOL_moe_list = 101,                 /* moe_list  */
  YYSYMBOL_moe = 102,                      /* moe  */
  YYSYMBOL_struct_dcl = 103,               /* struct_dcl  */
  YYSYMBOL_attr_var = 104,                 /* attr_var  */
  YYSYMBOL_attr_spec_list = 105,           /* attr_spec_list  */
  YYSYMBOL_str_head = 106,                 /* str_head  */
  YYSYMBOL_struct_dcl_list = 107,          /* struct_dcl_list  */
  YYSYMBOL_struct_declaration = 108,       /* struct_declaration  */
  YYSYMBOL_optsemi = 109,                  /* optsemi  */
  YYSYMBOL_specifier_qualifier_list = 110, /* specifier_qualifier_list  */
  YYSYMBOL_merge_specifiers = 111,         /* merge_specifiers  */
  YYSYMBOL_struct_declarator_list = 112,   /* struct_declarator_list  */
  YYSYMBOL_113_2 = 113,                    /* @2  */
  YYSYMBOL_struct_declarator = 114,        /* struct_declarator  */
  YYSYMBOL_xnfdeclarator = 115,            /* xnfdeclarator  */
  YYSYMBOL_init_declarator = 116,          /* init_declarator  */
  YYSYMBOL_begbr = 117,                    /* begbr  */
  YYSYMBOL_initializer = 118,              /* initializer  */
  YYSYMBOL_119_3 = 119,                    /* @3  */
  YYSYMBOL_init_list = 120,                /* init_list  */
  YYSYMBOL_121_4 = 121,                    /* @4  */
  YYSYMBOL_designation = 122,              /* designation  */
  YYSYMBOL_designator_list = 123,          /* designator_list  */
  YYSYMBOL_designator = 124,               /* designator  */
  YYSYMBOL_optcomma = 125,                 /* optcomma  */
  YYSYMBOL_ibrace = 126,                   /* ibrace  */
  YYSYMBOL_compoundstmt = 127,             /* compoundstmt  */
  YYSYMBOL_begin = 128,                    /* begin  */
  YYSYMBOL_statement = 129,                /* statement  */
  YYSYMBOL_asmstatement = 130,             /* asmstatement  */
  YYSYMBOL_svstr = 131,                    /* svstr  */
  YYSYMBOL_mvol = 132,                     /* mvol  */
  YYSYMBOL_xasm = 133,                     /* xasm  */
  YYSYMBOL_oplist = 134,                   /* oplist  */
  YYSYMBOL_oper = 135,                     /* oper  */
  YYSYMBOL_cnstr = 136,                    /* cnstr  */
  YYSYMBOL_label = 137,                    /* label  */
  YYSYMBOL_doprefix = 138,                 /* doprefix  */
  YYSYMBOL_ifprefix = 139,                 /* ifprefix  */
  YYSYMBOL_ifelprefix = 140,               /* ifelprefix  */
  YYSYMBOL_whprefix = 141,                 /* whprefix  */
  YYSYMBOL_forprefix = 142,                /* forprefix  */
  YYSYMBOL_143_5 = 143,                    /* $@5  */
  YYSYMBOL_switchpart = 144,               /* switchpart  */
  YYSYMBOL_145_e = 145,                    /* .e  */
  YYSYMBOL_elist = 146,                    /* elist  */
  YYSYMBOL_e2 = 147,                       /* e2  */
  YYSYMBOL_e = 148,                        /* e  */
  YYSYMBOL_xbegin = 149,                   /* xbegin  */
  YYSYMBOL_term = 150,                     /* term  */
  YYSYMBOL_gen_ass_list = 151,             /* gen_ass_list  */
  YYSYMBOL_gen_assoc = 152,                /* gen_assoc  */
  YYSYMBOL_xa = 153,                       /* xa  */
  YYSYMBOL_clbrace = 154,                  /* clbrace  */
  YYSYMBOL_string = 155,                   /* string  */
  YYSYMBOL_cast_type = 156                 /* cast_type  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2235

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  66
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  91
/* YYNRULES -- Number of rules.  */
#define YYNRULES  276
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  520

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    55,     2,
      60,    62,    58,    56,    49,    57,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    52,    61,
       2,    50,     2,    51,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    59,     2,    63,    54,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    64,    53,    65,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   296,   296,   297,   300,   301,   302,   303,   304,   307,
     311,   314,   315,   324,   327,   328,   329,   330,   333,   334,
     343,   344,   345,   346,   347,   351,   357,   358,   361,   364,
     365,   368,   369,   373,   376,   377,   380,   385,   386,   394,
     395,   399,   400,   404,   405,   406,   409,   413,   416,   417,
     418,   419,   420,   421,   422,   425,   431,   438,   439,   443,
     447,   452,   453,   463,   464,   474,   475,   484,   490,   494,
     500,   501,   505,   506,   510,   511,   515,   519,   523,   527,
     531,   535,   541,   544,   545,   551,   552,   556,   561,   565,
     574,   575,   578,   579,   589,   590,   594,   606,   607,   607,
     613,   614,   617,   618,   621,   622,   625,   626,   627,   628,
     631,   640,   644,   647,   659,   662,   663,   666,   667,   670,
     671,   675,   680,   681,   685,   688,   689,   693,   694,   694,
     698,   708,   714,   726,   741,   753,   756,   765,   768,   771,
     779,   783,   786,   789,   790,   790,   792,   795,   796,   796,
     800,   801,   802,   803,   806,   807,   810,   818,   823,   830,
     831,   834,   839,   840,   843,   874,   875,   876,   877,   883,
     892,   905,   917,   925,   933,   941,   952,   995,   996,   997,
     998,   999,  1000,  1001,  1004,  1005,  1008,  1011,  1012,  1015,
    1016,  1017,  1020,  1021,  1024,  1025,  1030,  1031,  1034,  1035,
    1036,  1037,  1042,  1045,  1053,  1058,  1068,  1082,  1096,  1096,
    1109,  1134,  1135,  1138,  1139,  1142,  1143,  1144,  1149,  1155,
    1156,  1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,
    1166,  1167,  1168,  1169,  1170,  1171,  1172,  1175,  1181,  1182,
    1183,  1184,  1185,  1186,  1187,  1190,  1191,  1195,  1199,  1207,
    1211,  1215,  1216,  1219,  1220,  1221,  1222,  1223,  1240,  1241,
    1242,  1243,  1244,  1245,  1248,  1251,  1260,  1263,  1264,  1267,
    1268,  1271,  1274,  1277,  1278,  1281,  1284
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "C_STRING", "C_ICON",
  "C_FCON", "C_NAME", "C_TYPENAME", "C_ANDAND", "C_OROR", "C_GOTO",
  "C_RETURN", "C_TYPE", "C_CLASS", "C_ASOP", "C_RELOP", "C_EQUOP",
  "C_DIVOP", "C_SHIFTOP", "C_INCOP", "C_UNOP", "C_STROP", "C_STRUCT",
  "C_QUALIFIER", "C_FUNSPEC", "C_IF", "C_ELSE", "C_SWITCH", "C_BREAK",
  "C_CONTINUE", "C_WHILE", "C_DO", "C_FOR", "C_DEFAULT", "C_CASE",
  "C_SIZEOF", "C_ENUM", "C_ELLIPSIS", "C_ASM", "NOMATCH", "C_TYPEOF",
  "C_ATTRIBUTE", "PCC_OFFSETOF", "GCC_DESIG", "C_STATICASSERT",
  "C_ALIGNAS", "C_ALIGNOF", "C_GENERIC", "C_ATOMIC", "','", "'='", "'?'",
  "':'", "'|'", "'^'", "'&'", "'+'", "'-'", "'*'", "'['", "'('", "';'",
  "')'", "']'", "'{'", "'}'", "$accept", "ext_def_list", "external_def",
  "funtype", "kr_args", "declaration_specifiers", "merge_attribs",
  "type_sq", "cf_spec", "typeof", "attribute_specifier", "attribute_list",
  "attribute", "declarator", "ecq", "r", "c", "type_qualifier_list",
  "identifier_list", "parameter_type_list", "parameter_list",
  "parameter_declaration", "abstract_declarator", "ib2", "maybe_r",
  "arg_dcl_list", "arg_declaration", "arg_param_list", "block_item_list",
  "block_item", "declaration", "init_declarator_list", "@1", "enum_dcl",
  "enum_head", "moe_list", "moe", "struct_dcl", "attr_var",
  "attr_spec_list", "str_head", "struct_dcl_list", "struct_declaration",
  "optsemi", "specifier_qualifier_list", "merge_specifiers",
  "struct_declarator_list", "@2", "struct_declarator", "xnfdeclarator",
  "init_declarator", "begbr", "initializer", "@3", "init_list", "@4",
  "designation", "designator_list", "designator", "optcomma", "ibrace",
  "compoundstmt", "begin", "statement", "asmstatement", "svstr", "mvol",
  "xasm", "oplist", "oper", "cnstr", "label", "doprefix", "ifprefix",
  "ifelprefix", "whprefix", "forprefix", "$@5", "switchpart", ".e",
  "elist", "e2", "e", "xbegin", "term", "gen_ass_list", "gen_assoc", "xa",
  "clbrace", "string", "cast_type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-356)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-213)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -356,   863,  -356,  -356,  -356,  -356,  -356,  -356,    29,  -356,
    -356,    80,   101,    51,    69,    72,    91,    95,   268,   122,
    -356,  -356,  2167,    42,  -356,  2167,  2167,  -356,  -356,   136,
    -356,  -356,   127,  -356,   129,   143,  -356,   200,    29,   145,
    -356,   151,  1150,   166,  1334,  1150,  1123,  -356,  -356,   136,
     295,   141,   122,   177,   125,  2167,  -356,  -356,   830,   -12,
     196,  -356,  -356,  -356,  1196,  1405,   255,   351,  -356,   186,
    -356,   253,  -356,  -356,  -356,   198,   265,  1334,  1334,  -356,
     222,  -356,   226,  1334,  1334,  1334,  1334,   802,  1123,   178,
    -356,  -356,  1539,   238,   287,   232,   301,  1992,  1590,   262,
     270,  -356,  -356,   136,  -356,   206,  -356,  -356,   483,   338,
      44,  -356,   275,   288,    29,  -356,   890,  -356,   279,  1334,
     282,  1220,   324,  -356,  2003,  -356,  -356,    58,   -11,   289,
     307,  -356,   304,   330,   333,  -356,  -356,  1045,  -356,   102,
     117,  1150,  -356,   238,   238,  1379,  1123,   334,  1334,   238,
     238,   238,   238,  -356,  1604,   678,   328,  -356,   308,  1196,
     401,   257,  1334,  1334,  1334,  1334,  1334,  1334,  1334,  1334,
    1334,  1265,  1334,  1334,  1334,  1334,  1334,  1334,  -356,  -356,
     382,  1334,  1150,  -356,  -356,   343,   344,   363,  1334,  -356,
    -356,  -356,  -356,   -20,    28,   362,    23,   974,   357,   360,
     366,   371,   369,  -356,   377,   386,  1334,  -356,  -356,    42,
     548,  -356,  -356,  -356,  -356,   384,   740,   740,   740,   740,
     740,  1334,   740,  1776,  -356,   125,  -356,   253,  -356,  -356,
       3,   990,  -356,  1334,  1334,  2003,  -356,  1334,   161,    37,
     338,   257,   440,  -356,  -356,  2117,  1334,  1334,   255,   385,
    -356,  -356,  1334,   305,    49,  -356,   253,  -356,   389,   400,
     398,   990,  -356,   802,   238,   408,  1123,  2054,  -356,   613,
    1827,   909,   311,   257,   403,    29,   263,  2167,  1310,   405,
    1126,   368,   990,     9,   171,  -356,    19,   990,   990,  1334,
    1930,  1232,  1345,   197,    16,    16,  -356,  -356,  -356,  1455,
     406,  1150,   410,   301,    17,  -356,  -356,    29,    29,   412,
    1334,  -356,  1840,  1334,  1334,  -356,  -356,  1334,  1019,  -356,
    1919,   184,  -356,  -356,  -356,   362,  -356,   441,   444,  -356,
    -356,   413,  2003,  -356,  -356,   338,   414,   125,   395,  -356,
    1334,  -356,   425,   955,   291,  -356,  2003,  2003,   246,  -356,
    -356,  -356,  -356,   990,   990,  -356,  -356,   990,  1334,  -356,
    -356,  -356,   416,   418,   427,   432,  -356,  -356,  1150,   420,
    1334,   423,  1092,   430,  1853,   434,  -356,   238,    48,   257,
      29,  -356,  -356,   435,   437,  1466,    29,  2167,  2065,  1334,
    -356,  -356,   439,  -356,  -356,   443,  -356,  -356,  -356,  1904,
    -356,  1618,  1669,  1683,  2137,   448,  1334,  -356,   460,  -356,
     740,  -356,   149,  -356,  -356,  -356,  1440,   461,   467,  -356,
    -356,   468,   990,  -356,  1334,  -356,   776,   102,  -356,  1334,
     253,   253,   990,  -356,   273,   394,  -356,   482,    -2,  -356,
     484,  -356,   470,   475,  -356,   425,  -356,    29,    29,    29,
    -356,   480,  2065,  -356,  -356,  -356,  -356,  -356,  -356,  1334,
    1334,  1981,  1334,  -356,  -356,  1334,  -356,   135,  -356,  -356,
     135,  1517,    29,  -356,  1697,   493,   486,  -356,  1334,  2187,
    -356,  1334,   488,  -356,   492,  -356,  -356,  -356,    29,   501,
     502,  -356,  1748,  1528,   955,   425,  -356,   253,  1334,   990,
    -356,   990,  -356,  -356,  -356,  -356,  -356,   503,   515,  -356,
     504,  -356,   517,  1762,  -356,  -356,  -356,   253,  -356,  -356
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     1,     8,    41,    19,    18,    29,   113,    22,
      30,   102,   187,     0,     0,     0,     0,    26,     0,     0,
       7,     2,    11,     0,    13,    14,    16,    28,    23,     9,
       5,    21,     0,    20,     0,     0,   115,   117,   114,   101,
     188,     0,     0,     0,     0,     0,     0,    57,    59,    39,
       0,     0,     0,     0,     0,    12,    85,    94,    10,     0,
       0,    97,    15,    17,    83,     0,     0,     0,     6,   111,
     116,     0,   273,   258,   259,   256,     0,     0,     0,   271,
       0,   271,     0,     0,     0,     0,     0,     0,   126,   275,
     124,   260,     0,   236,   186,     0,    36,     0,     0,     0,
       0,    58,    60,    40,    43,     0,   164,     4,     0,   113,
       0,    86,     0,   137,   113,    95,     0,    56,    55,    53,
       0,     0,    83,    48,    49,    61,    47,    69,     0,     0,
      63,    65,   106,   107,   159,   104,   112,     0,   119,   134,
       0,   213,   265,   244,   243,     0,     0,     0,     0,   240,
     242,   241,   239,   237,     0,     0,     0,   125,    70,    83,
      82,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,   238,
       0,     0,   213,   274,    32,    37,     0,    34,     0,    24,
      25,    27,    42,     0,   256,    19,     0,     0,     0,     0,
       0,     0,     0,   203,     0,     0,     0,   180,   163,     0,
       0,    90,    92,   166,    93,     0,     0,     0,     0,     0,
       0,   212,     0,     0,    88,     0,    87,     0,    98,   142,
     153,   139,    44,    54,     0,    50,    84,     0,    70,    82,
     113,    68,     0,    46,    45,     0,     0,     0,   160,     0,
     110,   120,     0,   113,     0,   127,   192,   184,     0,     0,
     214,   215,   218,     0,   245,     0,     0,     0,   261,     0,
       0,     0,    71,    72,     0,   113,     0,     0,    83,    82,
     225,   224,   221,   230,   229,   234,   231,   219,   220,     0,
       0,   226,   227,   228,   232,   233,   235,   254,   255,     0,
       0,   213,     0,    36,   186,   181,   182,   113,   113,     0,
       0,   175,     0,     0,     0,   173,   174,     0,   208,   202,
       0,   113,   162,    91,   179,     0,   183,     0,   167,   168,
     169,     0,   211,   172,   165,   113,     0,     0,     0,   151,
       0,   141,   159,     0,     0,   154,    52,    51,    71,    67,
      62,    64,    66,   108,   109,   105,   100,   131,     0,   130,
     128,   122,   121,     0,   189,   193,   185,   252,     0,     0,
       0,     0,     0,     0,     0,   165,   272,   246,   153,    73,
     113,    78,    74,     0,     0,     0,   113,     0,   223,     0,
     251,   253,     0,    33,    35,     0,   198,   199,   177,     0,
     176,     0,     0,     0,     0,     0,     0,   200,     0,   205,
       0,    89,   113,    99,   158,   157,     0,   148,     0,   161,
     147,   144,   143,   150,     0,   155,   132,   134,   123,     0,
     192,     0,   216,   217,   247,     0,   248,     0,     0,   267,
       0,   264,   165,     0,   250,   159,    75,   113,   113,   113,
      80,     0,   222,    38,    96,   178,   204,   210,   206,   212,
     212,     0,     0,   171,   138,     0,   156,   153,   140,   146,
     153,     0,   133,   129,     0,   190,     0,   257,     0,     0,
     266,     0,     0,   262,     0,    79,    76,    77,   113,     0,
       0,   201,     0,     0,     0,   159,   194,     0,     0,   270,
     268,   269,   263,   249,    81,   209,   207,     0,     0,   149,
       0,   196,   191,     0,   170,   152,   145,     0,   195,   197
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -356,  -356,  -356,  -356,  -356,    18,   409,     6,  -356,  -356,
      -8,   271,  -356,    71,   426,  -356,   466,  -149,  -356,  -238,
    -356,   346,   -75,   318,  -121,  -356,   543,  -356,   445,  -185,
       5,  -356,  -356,  -356,  -356,  -356,   353,  -356,    -5,   -15,
    -356,  -356,   462,  -356,   -55,   514,  -356,  -356,   180,  -356,
     274,  -356,   116,  -356,  -355,  -356,   148,  -356,   278,  -325,
    -356,   574,   -82,  -163,   627,   -69,  -356,  -356,   199,  -356,
    -356,  -356,  -356,  -356,  -356,  -356,  -356,  -356,  -356,  -284,
    -174,  -356,   -29,  -356,    88,  -356,   152,   549,  -356,   446,
     -24
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    21,    22,    53,   209,    24,    88,    26,    27,
      28,   186,   187,    49,   120,   121,   122,    50,   128,   129,
     130,   131,   273,   277,   123,    55,    56,   110,   210,   211,
     212,    59,   337,    31,    32,   134,   135,    33,   113,    38,
      34,   137,   138,   362,    89,    90,   254,   427,   255,    60,
      61,   230,   420,   470,   342,   467,   343,   344,   345,   249,
     421,   213,   108,   214,   215,    91,    41,   258,   364,   365,
     512,   216,   217,   218,   219,   220,   221,   404,   222,   331,
     259,   260,   223,   155,    93,   438,   439,   145,   378,    94,
     262
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,   237,   140,    37,    52,   153,    30,    25,   300,   272,
      48,    36,   139,    92,   161,    97,    98,   418,    95,    23,
     183,    99,   100,   445,   338,   323,   167,   168,    25,   309,
      70,    25,    25,   167,   405,   124,   167,   114,   242,   383,
      54,   305,   102,     4,    70,   306,   339,   479,     4,   115,
      36,   243,   241,   326,   327,   328,   329,   330,   154,   333,
     480,    25,   340,   156,     4,   175,   176,   177,   341,   338,
      14,    25,    29,    54,   177,   175,   176,   177,    14,   395,
     307,   310,   139,   127,   323,   276,    39,   231,   141,   348,
      51,   339,   235,   225,    58,   238,   159,   239,   360,   275,
      18,    36,    19,    57,   224,   226,    36,   340,     4,   228,
     361,    42,   261,   444,    25,   495,   238,   159,   239,   267,
     484,   103,   265,   105,    40,   109,   270,   392,     4,    43,
     124,     4,    44,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   290,   291,   292,   293,   294,   295,   296,   451,
      48,    45,   299,   261,   252,    46,   338,   384,   336,   287,
      18,    25,    19,    14,   276,   143,   144,     4,   312,   256,
     510,   149,   150,   151,   152,   489,   490,   320,   339,   257,
      18,   153,    19,    18,    47,    19,   165,   363,   167,   168,
      14,    66,   332,    67,   340,    64,    65,   379,   240,  -136,
      64,    65,    14,   104,    68,   346,    69,   152,   347,  -103,
     253,    71,   165,   166,   167,   168,    25,   353,   354,   238,
     159,   239,   112,   357,    52,    14,    96,   175,   176,   177,
      48,    36,    36,   264,   154,   349,   158,   159,   160,   369,
     374,   106,   371,    64,    65,    36,   116,   463,   359,   385,
    -118,    25,     4,   175,   176,   177,    72,   179,   141,   180,
     388,   132,   133,   127,   102,    64,    65,    36,   192,   101,
     381,   142,   261,   379,     4,    25,    72,    73,    74,    75,
     321,   399,   146,    25,   401,   402,   148,    14,   403,   332,
     183,    47,    77,    78,   184,   127,   335,   181,   182,    36,
      36,     4,   396,   397,   238,   159,   239,   185,    79,    14,
      51,   416,   338,    36,   422,    80,   278,   279,   101,    81,
      82,   152,   278,   279,   190,   382,    18,    36,    19,   426,
     411,    47,   191,    87,   101,   227,    14,   376,  -135,   432,
     102,   423,   -84,   287,   433,   232,    14,   236,   440,    14,
     424,   244,    14,    18,   246,    19,   245,   358,     5,   377,
     452,   363,   476,     6,    64,    65,   158,   159,   160,   158,
     159,   160,    36,     8,     9,   446,   162,   461,    36,    14,
     247,   450,   248,   165,   166,   167,   168,    11,   297,   298,
     271,    13,    14,    25,   266,   471,    16,    64,    65,    17,
     474,   414,   415,   301,    36,   127,   302,   464,   321,   459,
      25,   472,   303,   179,   308,   180,   136,   313,    36,   103,
     314,   172,   173,   174,   175,   176,   177,   315,   511,   317,
     332,   332,   316,   492,    62,    63,   493,   318,   319,    36,
      36,    36,   485,   486,   487,   324,   350,   368,   519,   499,
     356,   366,   501,   181,   182,   440,   477,   370,   435,   158,
     159,   160,   367,   275,    70,   422,   380,   386,   391,   513,
     409,   408,   393,   398,   417,   410,   412,   428,   429,   430,
      36,   431,   434,   504,   193,   436,    72,    73,    74,   194,
     195,    76,   441,   196,   197,     6,     7,   447,   253,   443,
     448,   453,    77,    78,   454,     8,     9,    10,   198,   460,
     199,   200,   201,   202,   203,   204,   205,   206,    79,    11,
     462,    12,   377,    13,    14,    80,  -160,    15,    16,    81,
      82,    17,   468,   469,   478,   482,   481,   483,    83,    84,
      85,    86,   488,    87,   207,   497,   498,   106,   208,   193,
     502,    72,    73,    74,   194,   195,    76,   503,   196,   197,
       6,     7,   505,   506,   514,   515,   517,    77,    78,   516,
       8,     9,    10,   198,   394,   199,   200,   201,   202,   203,
     204,   205,   206,    79,    11,   274,    12,   234,    13,    14,
      80,   352,    15,    16,    81,    82,    17,   387,   111,   251,
     269,   355,   157,    83,    84,    85,    86,   473,    87,   207,
     509,   413,   106,   322,   193,   494,    72,    73,    74,   194,
     195,    76,   425,   196,   197,     6,     7,   107,    35,   475,
     147,   500,    77,    78,   304,     8,     9,    10,   198,     0,
     199,   200,   201,   202,   203,   204,   205,   206,    79,    11,
       0,    12,     0,    13,    14,    80,     0,    15,    16,    81,
      82,    17,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,    87,   207,     0,     0,   106,   373,   193,
       0,    72,    73,    74,   194,   195,    76,     0,   196,   197,
       6,     7,     0,     0,     0,     0,     0,    77,    78,     0,
       8,     9,    10,   198,     0,   199,   200,   201,   202,   203,
     204,   205,   206,    79,    11,     0,    12,     0,    13,    14,
      80,     0,    15,    16,    81,    82,    17,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,    87,   207,
       0,   193,   106,    72,    73,    74,   194,   325,    76,     0,
     196,   197,     0,     0,     0,     0,     0,     0,     0,    77,
      78,     0,     0,     0,     0,   198,     0,   199,   200,   201,
     202,   203,   204,   205,   206,    79,     0,     0,    12,     0,
       0,     0,    80,     0,   162,   163,    81,    82,     0,     0,
     164,   165,   166,   167,   168,    83,    84,    85,    86,     0,
      87,   207,     0,     0,   106,    72,    73,    74,    75,     5,
      76,     0,     0,     0,     6,     0,     0,    14,     0,     0,
       0,    77,    78,     0,     8,     9,   170,   171,     0,   172,
     173,   174,   175,   176,   177,     0,     0,    79,    11,     0,
       0,     0,    13,    14,    80,     0,     0,    16,    81,    82,
      17,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,    87,     2,     3,     0,   106,     0,   112,     4,
       5,    14,     0,     0,     0,     6,     7,     0,     0,  -113,
    -113,     0,     0,     0,     0,     8,     9,    10,     0,    64,
      65,  -113,     0,    72,    73,    74,    75,     0,    76,    11,
       0,    12,     0,    13,    14,     0,     0,    15,    16,    77,
      78,    17,    72,    73,    74,    75,     0,    76,     0,     0,
       0,    18,     0,    19,    20,    79,     0,     0,    77,    78,
       0,     0,    80,     0,     0,     0,    81,    82,     0,     0,
       0,     0,     0,     0,    79,    83,    84,    85,    86,     0,
      87,    80,     0,     0,   229,    81,    82,     0,    72,    73,
      74,    75,     0,    76,    83,    84,    85,    86,     0,    87,
       0,     0,     0,   376,    77,    78,     0,    72,    73,    74,
      75,     0,    76,     0,     0,     0,     0,     0,     0,     0,
      79,     0,     0,    77,    78,     0,     0,    80,   162,   163,
       0,    81,    82,     0,   164,   165,   166,   167,   168,    79,
      83,    84,    85,    86,     0,    87,    80,     0,     0,   419,
      81,    82,    72,    73,    74,    75,     0,    76,     0,    83,
      84,    85,    86,     0,    87,   311,     0,     0,    77,    78,
     170,   171,     0,   172,   173,   174,   175,   176,   177,     0,
       0,     0,     5,     0,    79,     0,     0,     6,     0,     0,
       0,    80,     0,     0,     0,    81,    82,     8,     9,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,    87,
    -212,    11,     0,     0,     0,    13,    14,     0,     0,     0,
      16,     0,     0,    17,     0,    72,    73,    74,    75,     5,
      76,     0,     0,     0,     6,     0,     0,     0,     0,     0,
     250,    77,    78,     0,     8,     9,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,     0,    79,    11,     0,
       5,     0,    13,    14,    80,     6,     0,    16,    81,    82,
      17,   165,   166,   167,   168,     8,     9,    83,    84,    85,
      86,     0,    87,    72,    73,    74,    75,     5,    76,    11,
       0,     0,     6,    13,    14,     0,     0,     0,    16,    77,
      78,    17,     8,     9,     0,     0,     0,     0,     0,   172,
     173,   174,   175,   176,   177,    79,    11,     0,     0,     0,
      13,    14,    80,     0,     0,    16,    81,    82,    17,    72,
      73,    74,    75,     0,    76,    83,    84,    85,    86,   117,
      87,     0,     0,     0,     0,    77,    78,     0,     0,   118,
       0,     0,     0,    72,    73,    74,    75,     0,    76,     0,
       0,    79,     0,   117,     0,     0,     0,     0,    80,    77,
      78,     0,    81,    82,     0,     0,     0,   165,   166,   167,
     168,    83,    84,    85,   119,    79,    87,     0,     0,     0,
       0,     0,    80,     0,     0,     0,    81,    82,    72,    73,
      74,    75,     0,    76,     0,    83,    84,    85,   233,     0,
      87,     0,     0,     0,    77,    78,   173,   174,   175,   176,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,    80,     0,     0,
       0,    81,    82,    72,    73,    74,    75,   289,    76,     0,
      83,    84,    85,    86,     0,    87,     0,     0,     0,    77,
      78,     0,     0,   236,     0,     0,     0,    72,    73,    74,
      75,     0,    76,     0,     0,    79,     0,     0,     0,     0,
       0,     0,    80,    77,    78,     0,    81,    82,     0,     0,
     165,   166,   167,   168,     0,    83,    84,    85,    86,    79,
      87,     0,     0,     0,     0,     0,    80,     0,     0,     0,
      81,    82,    72,    73,    74,    75,     0,    76,     0,    83,
      84,    85,    86,     0,    87,     0,     0,     0,    77,    78,
     174,   175,   176,   177,     0,     0,     0,     0,     0,     0,
       0,   125,     5,     0,    79,     0,     0,     6,     7,     0,
       0,    80,     0,     0,     0,    81,    82,     8,     9,    10,
       0,     0,     0,     0,    83,    84,    85,    86,     0,   263,
       0,    11,     0,     0,     0,    13,    14,     0,   162,   163,
      16,     0,     0,    17,   164,   165,   166,   167,   168,     0,
       0,     0,     0,   162,   163,     0,     0,   126,     0,   164,
     165,   166,   167,   168,   162,   163,     0,   465,     0,     0,
     164,   165,   166,   167,   168,     0,     0,     0,     0,   169,
     170,   171,     0,   172,   173,   174,   175,   176,   177,     0,
       0,     0,     0,   466,   169,   170,   171,     0,   172,   173,
     174,   175,   176,   177,     0,   169,   170,   171,   390,   172,
     173,   174,   175,   176,   177,   162,   163,     0,     0,   449,
       0,   164,   165,   166,   167,   168,   162,   163,     0,     0,
       0,     0,   164,   165,   166,   167,   168,   162,   163,     0,
       0,     0,     0,   164,   165,   166,   167,   168,     0,     0,
       0,     0,     0,     0,     0,     0,   169,   170,   171,     0,
     172,   173,   174,   175,   176,   177,     0,   169,   170,   171,
     466,   172,   173,   174,   175,   176,   177,     0,   169,   170,
     171,   508,   172,   173,   174,   175,   176,   177,   162,   163,
       0,   178,     0,     0,   164,   165,   166,   167,   168,     0,
       0,     0,   162,   163,     0,     0,     0,     0,   164,   165,
     166,   167,   168,     0,     0,     0,   162,   163,     0,     0,
       0,     0,   164,   165,   166,   167,   168,     0,     0,   169,
     170,   171,     0,   172,   173,   174,   175,   176,   177,     0,
       0,     0,   189,   169,   170,   171,     0,   172,   173,   174,
     175,   176,   177,     0,     0,     0,   268,   169,   170,   171,
       0,   172,   173,   174,   175,   176,   177,   162,   163,     0,
     456,     0,     0,   164,   165,   166,   167,   168,     0,     0,
       0,   162,   163,     0,     0,     0,     0,   164,   165,   166,
     167,   168,     0,     0,     0,   162,   163,     0,     0,     0,
       0,   164,   165,   166,   167,   168,     0,     0,   169,   170,
     171,     0,   172,   173,   174,   175,   176,   177,     0,     0,
       0,   457,   169,   170,   171,     0,   172,   173,   174,   175,
     176,   177,     0,     0,     0,   458,   169,   170,   171,     0,
     172,   173,   174,   175,   176,   177,   162,   163,     0,   496,
       0,     0,   164,   165,   166,   167,   168,     0,     0,     0,
     162,   163,     0,     0,     0,     0,   164,   165,   166,   167,
     168,     0,     0,     0,   162,   163,     0,     0,     0,     0,
     164,   165,   166,   167,   168,     0,     0,   169,   170,   171,
       0,   172,   173,   174,   175,   176,   177,     0,     0,     0,
     507,   169,   170,   171,     0,   172,   173,   174,   175,   176,
     177,     0,     0,     0,   518,   169,   170,   171,     0,   172,
     173,   174,   175,   176,   177,   162,   163,   334,     0,     0,
       0,   164,   165,   166,   167,   168,     0,     0,   162,   163,
       0,     0,     0,     0,   164,   165,   166,   167,   168,     0,
       0,   162,   163,     0,     0,     0,     0,   164,   165,   166,
     167,   168,     0,     0,     0,     0,   169,   170,   171,     0,
     172,   173,   174,   175,   176,   177,     0,     0,   375,   169,
     170,   171,     0,   172,   173,   174,   175,   176,   177,     0,
       0,   400,   169,   170,   171,     0,   172,   173,   174,   175,
     176,   177,   162,   163,   442,     0,     0,     0,   164,   165,
     166,   167,   168,     0,     0,     0,     0,   162,   163,     0,
       0,     0,     0,   164,   165,   166,   167,   168,   162,   163,
       0,     0,     0,     0,   164,   165,   166,   167,   168,     0,
       0,     0,     0,   169,   170,   171,   406,   172,   173,   174,
     175,   176,   177,     0,     0,   455,     0,     0,   169,   170,
     171,   407,   172,   173,   174,   175,   176,   177,     0,   169,
     170,   171,   389,   172,   173,   174,   175,   176,   177,   162,
     163,     0,     0,     0,     0,   164,   165,   166,   167,   168,
     162,   163,     0,     0,     0,     0,   164,   165,   166,   167,
     168,   162,   163,     0,     0,     0,     0,   164,   165,   166,
     167,   168,     0,     0,     0,     0,     0,     0,     0,     0,
     169,   170,   171,   491,   172,   173,   174,   175,   176,   177,
       0,   188,   170,   171,     0,   172,   173,   174,   175,   176,
     177,     0,   169,   170,   171,     0,   172,   173,   174,   175,
     176,   177,   162,   163,     0,     0,     0,     0,   164,   165,
     166,   167,   168,   162,   163,     0,     0,     0,     0,     0,
     165,   166,   167,   168,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   372,   170,   171,     0,   172,   173,   174,
     175,   176,   177,     0,     0,     0,   171,     0,   172,   173,
     174,   175,   176,   177,     5,     0,     0,     0,     0,     6,
       7,     0,     0,     0,     0,     0,     0,     0,     0,     8,
       9,    10,     0,     0,     5,     0,     0,     0,     0,     6,
       7,     0,     0,    11,   351,     0,     0,    13,    14,     8,
       9,    10,    16,     0,     0,    17,     0,     0,     0,     0,
       0,     0,     0,    11,     5,     0,     0,    13,    14,     6,
       7,    15,    16,     0,     0,    17,     0,     0,     0,     8,
       9,    10,     0,     0,     5,     0,     0,     0,     0,     6,
       0,     0,     0,    11,     0,     0,     0,    13,    14,     8,
       9,     0,    16,     0,     0,    17,     0,     0,     0,     0,
     437,     0,     0,    11,     0,     0,     0,    13,    14,     0,
       0,     0,    16,     0,     0,    17
};

static const yytype_int16 yycheck[] =
{
       8,   122,    71,     8,    19,    87,     1,     1,   182,   158,
      18,    19,    67,    42,    89,    44,    45,   342,    42,     1,
       3,    45,    46,   378,    21,   210,    17,    18,    22,     6,
      38,    25,    26,    17,   318,    64,    17,    49,    49,   277,
      22,    61,    50,     6,    52,    65,    43,    49,     6,    61,
      58,    62,   127,   216,   217,   218,   219,   220,    87,   222,
      62,    55,    59,    87,     6,    56,    57,    58,    65,    21,
      41,    65,     1,    55,    58,    56,    57,    58,    41,    62,
      52,    58,   137,    65,   269,   160,     6,   116,    60,   238,
      19,    43,   121,    49,    23,    58,    59,    60,    49,    62,
      58,   109,    60,    61,   109,    61,   114,    59,     6,   114,
      61,    60,   141,    65,   108,   470,    58,    59,    60,   148,
     445,    50,   146,    52,    23,    54,   155,   301,     6,    60,
     159,     6,    60,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   387,
     158,    60,   181,   182,    52,    60,    21,   278,   227,   188,
      58,   155,    60,    41,   239,    77,    78,     6,   197,    52,
     495,    83,    84,    85,    86,   459,   460,   206,    43,    62,
      58,   263,    60,    58,    23,    60,    15,   256,    17,    18,
      41,    64,   221,    64,    59,    59,    60,   272,   127,    50,
      59,    60,    41,    62,    61,   234,     6,   119,   237,    64,
     139,    60,    15,    16,    17,    18,   210,   246,   247,    58,
      59,    60,    38,   252,   239,    41,    60,    56,    57,    58,
     238,   239,   240,   145,   263,   240,    58,    59,    60,   263,
     269,    64,   266,    59,    60,   253,    50,   410,   253,   278,
      64,   245,     6,    56,    57,    58,     3,    19,    60,    21,
     289,     6,     7,   245,   272,    59,    60,   275,    62,    23,
     275,     6,   301,   348,     6,   269,     3,     4,     5,     6,
     209,   310,    60,   277,   313,   314,    60,    41,   317,   318,
       3,    23,    19,    20,    62,   277,   225,    59,    60,   307,
     308,     6,   307,   308,    58,    59,    60,     6,    35,    41,
     239,   340,    21,   321,   343,    42,    59,    60,    23,    46,
      47,   233,    59,    60,    62,    62,    58,   335,    60,   358,
     335,    23,    62,    60,    23,    60,    41,    64,    50,   368,
     348,    50,    63,   372,   368,    63,    41,    23,   372,    41,
      59,    62,    41,    58,    50,    60,    49,    52,     7,   271,
     389,   430,   431,    12,    59,    60,    58,    59,    60,    58,
      59,    60,   380,    22,    23,   380,     8,   406,   386,    41,
      50,   386,    49,    15,    16,    17,    18,    36,     6,     7,
      62,    40,    41,   387,    60,   424,    45,    59,    60,    48,
     429,     6,     7,    60,   412,   387,    62,   412,   337,   404,
     404,   426,    49,    19,    52,    21,    65,    60,   426,   348,
      60,    53,    54,    55,    56,    57,    58,    61,   497,    60,
     459,   460,    61,   462,    25,    26,   465,    60,    52,   447,
     448,   449,   447,   448,   449,    61,     6,    49,   517,   478,
      65,    62,   481,    59,    60,   479,    62,    49,   370,    58,
      59,    60,    62,    62,   472,   494,    63,    62,    62,   498,
      26,    30,    62,    61,    49,    62,    62,    61,    60,    52,
     488,    49,    62,   488,     1,    62,     3,     4,     5,     6,
       7,     8,    62,    10,    11,    12,    13,    62,   427,    65,
      63,    62,    19,    20,    61,    22,    23,    24,    25,    61,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      60,    38,   434,    40,    41,    42,    65,    44,    45,    46,
      47,    48,    65,    65,    52,    65,    52,    62,    55,    56,
      57,    58,    62,    60,    61,    52,    60,    64,    65,     1,
      62,     3,     4,     5,     6,     7,     8,    65,    10,    11,
      12,    13,    61,    61,    61,    50,    49,    19,    20,    65,
      22,    23,    24,    25,   303,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,   159,    38,   121,    40,    41,
      42,   245,    44,    45,    46,    47,    48,   279,    55,   137,
     155,   248,    88,    55,    56,    57,    58,   427,    60,    61,
     494,   337,    64,    65,     1,   467,     3,     4,     5,     6,
       7,     8,   344,    10,    11,    12,    13,    53,     1,   430,
      81,   479,    19,    20,   188,    22,    23,    24,    25,    -1,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    -1,    40,    41,    42,    -1,    44,    45,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
      57,    58,    -1,    60,    61,    -1,    -1,    64,    65,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    10,    11,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,
      22,    23,    24,    25,    -1,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    -1,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    -1,    60,    61,
      -1,     1,    64,     3,     4,     5,     6,     7,     8,    -1,
      10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    38,    -1,
      -1,    -1,    42,    -1,     8,     9,    46,    47,    -1,    -1,
      14,    15,    16,    17,    18,    55,    56,    57,    58,    -1,
      60,    61,    -1,    -1,    64,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    41,    -1,    -1,
      -1,    19,    20,    -1,    22,    23,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,    -1,    -1,    35,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    47,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    -1,    60,     0,     1,    -1,    64,    -1,    38,     6,
       7,    41,    -1,    -1,    -1,    12,    13,    -1,    -1,    49,
      50,    -1,    -1,    -1,    -1,    22,    23,    24,    -1,    59,
      60,    61,    -1,     3,     4,     5,     6,    -1,     8,    36,
      -1,    38,    -1,    40,    41,    -1,    -1,    44,    45,    19,
      20,    48,     3,     4,     5,     6,    -1,     8,    -1,    -1,
      -1,    58,    -1,    60,    61,    35,    -1,    -1,    19,    20,
      -1,    -1,    42,    -1,    -1,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    55,    56,    57,    58,    -1,
      60,    42,    -1,    -1,    64,    46,    47,    -1,     3,     4,
       5,     6,    -1,     8,    55,    56,    57,    58,    -1,    60,
      -1,    -1,    -1,    64,    19,    20,    -1,     3,     4,     5,
       6,    -1,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    19,    20,    -1,    -1,    42,     8,     9,
      -1,    46,    47,    -1,    14,    15,    16,    17,    18,    35,
      55,    56,    57,    58,    -1,    60,    42,    -1,    -1,    64,
      46,    47,     3,     4,     5,     6,    -1,     8,    -1,    55,
      56,    57,    58,    -1,    60,    61,    -1,    -1,    19,    20,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      -1,    -1,     7,    -1,    35,    -1,    -1,    12,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    46,    47,    22,    23,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,    60,
      61,    36,    -1,    -1,    -1,    40,    41,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      65,    19,    20,    -1,    22,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    36,    -1,
       7,    -1,    40,    41,    42,    12,    -1,    45,    46,    47,
      48,    15,    16,    17,    18,    22,    23,    55,    56,    57,
      58,    -1,    60,     3,     4,     5,     6,     7,     8,    36,
      -1,    -1,    12,    40,    41,    -1,    -1,    -1,    45,    19,
      20,    48,    22,    23,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    35,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    47,    48,     3,
       4,     5,     6,    -1,     8,    55,    56,    57,    58,    13,
      60,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    23,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,     8,    -1,
      -1,    35,    -1,    13,    -1,    -1,    -1,    -1,    42,    19,
      20,    -1,    46,    47,    -1,    -1,    -1,    15,    16,    17,
      18,    55,    56,    57,    58,    35,    60,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    46,    47,     3,     4,
       5,     6,    -1,     8,    -1,    55,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    19,    20,    54,    55,    56,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    46,    47,     3,     4,     5,     6,    52,     8,    -1,
      55,    56,    57,    58,    -1,    60,    -1,    -1,    -1,    19,
      20,    -1,    -1,    23,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,     8,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    19,    20,    -1,    46,    47,    -1,    -1,
      15,    16,    17,    18,    -1,    55,    56,    57,    58,    35,
      60,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      46,    47,     3,     4,     5,     6,    -1,     8,    -1,    55,
      56,    57,    58,    -1,    60,    -1,    -1,    -1,    19,    20,
      55,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     6,     7,    -1,    35,    -1,    -1,    12,    13,    -1,
      -1,    42,    -1,    -1,    -1,    46,    47,    22,    23,    24,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    -1,    60,
      -1,    36,    -1,    -1,    -1,    40,    41,    -1,     8,     9,
      45,    -1,    -1,    48,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,     8,     9,    -1,    -1,    62,    -1,    14,
      15,    16,    17,    18,     8,     9,    -1,    37,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      -1,    -1,    -1,    63,    49,    50,    51,    -1,    53,    54,
      55,    56,    57,    58,    -1,    49,    50,    51,    63,    53,
      54,    55,    56,    57,    58,     8,     9,    -1,    -1,    63,
      -1,    14,    15,    16,    17,    18,     8,     9,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,     8,     9,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    -1,    49,    50,    51,
      63,    53,    54,    55,    56,    57,    58,    -1,    49,    50,
      51,    63,    53,    54,    55,    56,    57,    58,     8,     9,
      -1,    62,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,     8,     9,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,     8,     9,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      -1,    -1,    62,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    -1,    -1,    62,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,     8,     9,    -1,
      62,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,     8,     9,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,     8,     9,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    49,    50,
      51,    -1,    53,    54,    55,    56,    57,    58,    -1,    -1,
      -1,    62,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    58,    -1,    -1,    -1,    62,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,     8,     9,    -1,    62,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
       8,     9,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,     8,     9,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    -1,    -1,    -1,
      62,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    -1,    -1,    62,    49,    50,    51,    -1,    53,
      54,    55,    56,    57,    58,     8,     9,    61,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,     8,     9,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,     8,     9,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    -1,    -1,    61,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    -1,
      -1,    61,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    58,     8,     9,    61,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,     8,     9,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,     8,     9,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    49,    50,    51,    37,    53,    54,    55,
      56,    57,    58,    -1,    -1,    61,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,     8,
       9,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
       8,     9,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,     8,     9,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      -1,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    58,     8,     9,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,     8,     9,    -1,    -1,    -1,    -1,    -1,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,    -1,    53,    54,    55,
      56,    57,    58,    -1,    -1,    -1,    51,    -1,    53,    54,
      55,    56,    57,    58,     7,    -1,    -1,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    -1,    -1,     7,    -1,    -1,    -1,    -1,    12,
      13,    -1,    -1,    36,    37,    -1,    -1,    40,    41,    22,
      23,    24,    45,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,     7,    -1,    -1,    40,    41,    12,
      13,    44,    45,    -1,    -1,    48,    -1,    -1,    -1,    22,
      23,    24,    -1,    -1,     7,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    40,    41,    22,
      23,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    36,    -1,    -1,    -1,    40,    41,    -1,
      -1,    -1,    45,    -1,    -1,    48
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    67,     0,     1,     6,     7,    12,    13,    22,    23,
      24,    36,    38,    40,    41,    44,    45,    48,    58,    60,
      61,    68,    69,    71,    72,    73,    74,    75,    76,    79,
      96,    99,   100,   103,   106,   130,    76,   104,   105,     6,
      23,   132,    60,    60,    60,    60,    60,    23,    76,    79,
      83,    79,   105,    70,    71,    91,    92,    61,    79,    97,
     115,   116,    72,    72,    59,    60,    64,    64,    61,     6,
      76,    60,     3,     4,     5,     6,     8,    19,    20,    35,
      42,    46,    47,    55,    56,    57,    58,    60,    73,   110,
     111,   131,   148,   150,   155,   156,    60,   148,   148,   156,
     156,    23,    76,    79,    62,    79,    64,   127,   128,    79,
      93,    92,    38,   104,    49,    61,    50,    13,    23,    58,
      80,    81,    82,    90,   148,     6,    62,    71,    84,    85,
      86,    87,     6,     7,   101,   102,    65,   107,   108,   110,
     131,    60,     6,   150,   150,   153,    60,   153,    60,   150,
     150,   150,   150,   128,   148,   149,   156,   111,    58,    59,
      60,    88,     8,     9,    14,    15,    16,    17,    18,    49,
      50,    51,    53,    54,    55,    56,    57,    58,    62,    19,
      21,    59,    60,     3,    62,     6,    77,    78,    49,    62,
      62,    62,    62,     1,     6,     7,    10,    11,    25,    27,
      28,    29,    30,    31,    32,    33,    34,    61,    65,    71,
      94,    95,    96,   127,   129,   130,   137,   138,   139,   140,
     141,   142,   144,   148,   104,    49,    61,    60,   104,    64,
     117,   148,    63,    58,    82,   148,    23,    90,    58,    60,
      79,    88,    49,    62,    62,    49,    50,    50,    49,   125,
      65,   108,    52,    79,   112,   114,    52,    62,   133,   146,
     147,   148,   156,    60,   150,   156,    60,   148,    62,    94,
     148,    62,    83,    88,    80,    62,    88,    89,    59,    60,
     148,   148,   148,   148,   148,   148,   148,   148,   148,    52,
     148,   148,   148,   148,   148,   148,   148,     6,     7,   148,
     146,    60,    62,    49,   155,    61,    65,    52,    52,     6,
      58,    61,   148,    60,    60,    61,    61,    60,    60,    52,
     148,    79,    65,    95,    61,     7,   129,   129,   129,   129,
     129,   145,   148,   129,    61,    79,   131,    98,    21,    43,
      59,    65,   120,   122,   123,   124,   148,   148,    83,   104,
       6,    37,    87,   148,   148,   102,    65,   148,    52,   104,
      49,    61,   109,   131,   134,   135,    62,    62,    49,   156,
      49,   156,    49,    65,   148,    61,    64,   150,   154,    88,
      63,   104,    62,    85,    90,   148,    62,    89,   148,    52,
      63,    62,   146,    62,    77,    62,   104,   104,    61,   148,
      61,   148,   148,   148,   143,   145,    37,    52,    30,    26,
      62,   104,    62,   116,     6,     7,   148,    49,   125,    64,
     118,   126,   148,    50,    59,   124,   148,   113,    61,    60,
      52,    49,   148,   156,    62,   150,    62,    33,   151,   152,
     156,    62,    61,    65,    65,   120,   104,    62,    63,    63,
     104,    85,   148,    62,    61,    61,    62,    62,    62,    96,
      61,   148,    60,   129,   104,    37,    63,   121,    65,    65,
     119,   148,   105,   114,   148,   134,   131,    62,    52,    49,
      62,    52,    65,    62,   125,   104,   104,   104,    62,   145,
     145,    52,   148,   148,   122,   120,    62,    52,    60,   148,
     152,   148,    62,    65,   104,    61,    61,    62,    63,   118,
     125,   131,   136,   148,    61,    50,    65,    49,    62,   131
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    66,    67,    67,    68,    68,    68,    68,    68,    69,
      69,    70,    70,    71,    72,    72,    72,    72,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    74,
      74,    75,    75,    76,    77,    77,    78,    78,    78,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    80,    80,
      80,    80,    80,    80,    80,    81,    82,    83,    83,    83,
      83,    84,    84,    85,    85,    86,    86,    87,    87,    87,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    89,    90,    90,    91,    91,    92,    93,    93,
      94,    94,    95,    95,    96,    96,    96,    97,    98,    97,
      99,    99,   100,   100,   101,   101,   102,   102,   102,   102,
     103,   103,   103,   104,   104,   105,   105,   106,   106,   107,
     107,   108,   109,   109,   110,   111,   111,   112,   113,   112,
     114,   114,   114,   114,   114,   115,   115,   116,   116,   116,
     116,   116,   117,   118,   119,   118,   118,   120,   121,   120,
     122,   122,   122,   122,   123,   123,   124,   124,   124,   125,
     125,   126,   127,   127,   128,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   130,   130,   131,   132,   132,   133,
     133,   133,   134,   134,   135,   135,   136,   136,   137,   137,
     137,   137,   137,   138,   139,   140,   141,   142,   143,   142,
     144,   145,   145,   146,   146,   147,   147,   147,   147,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   149,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   151,   151,   152,
     152,   153,   154,   155,   155,   156,   156
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     3,     1,     2,     1,     1,     1,
       2,     0,     1,     1,     1,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     4,     4,     1,     4,     1,     1,
       1,     4,     4,     6,     1,     3,     0,     1,     4,     2,
       3,     1,     4,     3,     4,     4,     4,     3,     1,     1,
       2,     3,     3,     1,     2,     1,     1,     1,     2,     1,
       2,     1,     3,     1,     3,     1,     3,     3,     2,     1,
       1,     2,     2,     3,     3,     4,     5,     5,     3,     5,
       4,     6,     0,     0,     1,     1,     2,     3,     2,     4,
       1,     2,     1,     1,     2,     3,     7,     1,     0,     5,
       5,     2,     1,     2,     1,     3,     1,     1,     3,     3,
       4,     3,     3,     0,     1,     1,     2,     2,     3,     1,
       2,     3,     1,     2,     1,     2,     1,     1,     0,     4,
       2,     2,     3,     4,     0,     2,     5,     2,     6,     3,
       6,     4,     1,     1,     0,     5,     2,     2,     0,     5,
       2,     1,     6,     0,     1,     2,     3,     2,     2,     0,
       1,     1,     3,     2,     1,     2,     1,     2,     2,     2,
       7,     4,     2,     2,     2,     2,     3,     3,     4,     2,
       1,     2,     2,     2,     5,     6,     1,     0,     1,     2,
       4,     6,     0,     1,     4,     6,     1,     3,     3,     3,
       3,     5,     2,     1,     4,     3,     4,     6,     0,     6,
       4,     1,     0,     0,     1,     1,     3,     3,     1,     3,
       3,     3,     5,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     3,     4,     5,     5,     7,
       5,     4,     4,     4,     3,     3,     1,     6,     1,     1,
       1,     3,     6,     7,     5,     2,     6,     1,     3,     3,
       3,     0,     1,     1,     2,     1,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* ext_def_list: %empty  */
#line 297 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                  { ftnend(); }
#line 2192 "y.tab.c"
    break;

  case 4: /* external_def: funtype kr_args compoundstmt  */
#line 300 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                { fend(); }
#line 2198 "y.tab.c"
    break;

  case 5: /* external_def: declaration  */
#line 301 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                { blevel = 0; symclear(0); }
#line 2204 "y.tab.c"
    break;

  case 8: /* external_def: error  */
#line 304 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                         { blevel = 0; }
#line 2210 "y.tab.c"
    break;

  case 9: /* funtype: declarator  */
#line 307 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                 {
		    fundef(mkty(INT, 0, 0), (yyvsp[0].nodep));
		    cftnsp->sflags |= NORETYP;
		}
#line 2219 "y.tab.c"
    break;

  case 10: /* funtype: declaration_specifiers declarator  */
#line 311 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                    { fundef((yyvsp[-1].nodep),(yyvsp[0].nodep)); }
#line 2225 "y.tab.c"
    break;

  case 13: /* declaration_specifiers: merge_attribs  */
#line 324 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                 { (yyval.nodep) = typenode((yyvsp[0].nodep)); }
#line 2231 "y.tab.c"
    break;

  case 14: /* merge_attribs: type_sq  */
#line 327 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2237 "y.tab.c"
    break;

  case 15: /* merge_attribs: type_sq merge_attribs  */
#line 328 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                         { (yyval.nodep) = cmop((yyvsp[0].nodep), (yyvsp[-1].nodep)); }
#line 2243 "y.tab.c"
    break;

  case 16: /* merge_attribs: cf_spec  */
#line 329 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2249 "y.tab.c"
    break;

  case 17: /* merge_attribs: cf_spec merge_attribs  */
#line 330 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                         { (yyval.nodep) = cmop((yyvsp[0].nodep), (yyvsp[-1].nodep)); }
#line 2255 "y.tab.c"
    break;

  case 18: /* type_sq: C_TYPE  */
#line 333 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                          { (yyval.nodep) = mkty((yyvsp[0].type), 0, 0); }
#line 2261 "y.tab.c"
    break;

  case 19: /* type_sq: C_TYPENAME  */
#line 334 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                              { 
			struct symtab *sp = lookup((yyvsp[0].strp), 0);
			if (sp->stype == ENUMTY) {
				sp->stype = strmemb(sp->td->ss)->stype;
			}
			(yyval.nodep) = mkty(sp->stype, sp->sdf, sp->sss);
			(yyval.nodep)->n_sp = sp;
			(yyval.nodep)->n_ap = sp->sap;
		}
#line 2275 "y.tab.c"
    break;

  case 20: /* type_sq: struct_dcl  */
#line 343 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                              { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2281 "y.tab.c"
    break;

  case 21: /* type_sq: enum_dcl  */
#line 344 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                            { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2287 "y.tab.c"
    break;

  case 22: /* type_sq: C_QUALIFIER  */
#line 345 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               { (yyval.nodep) = block(QUALIFIER, NULL, NULL, 0, 0, 0); (yyval.nodep)->n_qual = (yyvsp[0].type); }
#line 2293 "y.tab.c"
    break;

  case 23: /* type_sq: attribute_specifier  */
#line 346 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       { (yyval.nodep) = biop(ATTRIB, (yyvsp[0].nodep), 0); }
#line 2299 "y.tab.c"
    break;

  case 24: /* type_sq: C_ALIGNAS '(' e ')'  */
#line 347 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       { 
			(yyval.nodep) = biop(ALIGN, NULL, NULL);
			slval((yyval.nodep), con_e((yyvsp[-1].nodep)));
		}
#line 2308 "y.tab.c"
    break;

  case 25: /* type_sq: C_ALIGNAS '(' cast_type ')'  */
#line 351 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                               {
			TYMFIX((yyvsp[-1].nodep));
			(yyval.nodep) = biop(ALIGN, NULL, NULL);
			slval((yyval.nodep), talign((yyvsp[-1].nodep)->n_type, (yyvsp[-1].nodep)->pss)/SZCHAR);
			p1tfree((yyvsp[-1].nodep));
		}
#line 2319 "y.tab.c"
    break;

  case 26: /* type_sq: C_ATOMIC  */
#line 357 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                            { uerror("_Atomic not supported"); (yyval.nodep) = bcon(0); }
#line 2325 "y.tab.c"
    break;

  case 27: /* type_sq: C_ATOMIC '(' cast_type ')'  */
#line 358 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                              {
			uerror("_Atomic not supported"); (yyval.nodep) = (yyvsp[-1].nodep);
		}
#line 2333 "y.tab.c"
    break;

  case 28: /* type_sq: typeof  */
#line 361 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                          { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2339 "y.tab.c"
    break;

  case 29: /* cf_spec: C_CLASS  */
#line 364 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = block(CLASS, NULL, NULL, (yyvsp[0].type), 0, 0); }
#line 2345 "y.tab.c"
    break;

  case 30: /* cf_spec: C_FUNSPEC  */
#line 365 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                             { (yyval.nodep) = block(FUNSPEC, NULL, NULL, (yyvsp[0].type), 0, 0); }
#line 2351 "y.tab.c"
    break;

  case 31: /* typeof: C_TYPEOF '(' e ')'  */
#line 368 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                      { (yyval.nodep) = tyof(eve((yyvsp[-1].nodep))); }
#line 2357 "y.tab.c"
    break;

  case 32: /* typeof: C_TYPEOF '(' cast_type ')'  */
#line 369 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                              { TYMFIX((yyvsp[-1].nodep)); (yyval.nodep) = tyof((yyvsp[-1].nodep)); }
#line 2363 "y.tab.c"
    break;

  case 33: /* attribute_specifier: C_ATTRIBUTE '(' '(' attribute_list ')' ')'  */
#line 373 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                              { (yyval.nodep) = (yyvsp[-2].nodep); }
#line 2369 "y.tab.c"
    break;

  case 35: /* attribute_list: attribute ',' attribute_list  */
#line 377 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                { (yyval.nodep) = cmop((yyvsp[0].nodep), (yyvsp[-2].nodep)); }
#line 2375 "y.tab.c"
    break;

  case 36: /* attribute: %empty  */
#line 380 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                   {
#ifdef GCC_COMPAT
			 (yyval.nodep) = voidcon();
#endif
		}
#line 2385 "y.tab.c"
    break;

  case 37: /* attribute: C_NAME  */
#line 385 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                          { (yyval.nodep) = bdty(NAME, (yyvsp[0].strp)); }
#line 2391 "y.tab.c"
    break;

  case 38: /* attribute: C_NAME '(' elist ')'  */
#line 386 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                        {
			(yyval.nodep) = bdty((yyvsp[-1].nodep) == NULL ? UCALL : CALL, bdty(NAME, (yyvsp[-3].strp)), (yyvsp[-1].nodep));
		}
#line 2399 "y.tab.c"
    break;

  case 39: /* declarator: '*' declarator  */
#line 394 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                  { (yyval.nodep) = bdty(UMUL, (yyvsp[0].nodep)); }
#line 2405 "y.tab.c"
    break;

  case 40: /* declarator: '*' type_qualifier_list declarator  */
#line 395 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                      {
			(yyval.nodep) = (yyvsp[-1].nodep);
			(yyval.nodep)->n_left = (yyvsp[0].nodep);
		}
#line 2414 "y.tab.c"
    break;

  case 41: /* declarator: C_NAME  */
#line 399 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                          { (yyval.nodep) = bdty(NAME, (yyvsp[0].strp)); }
#line 2420 "y.tab.c"
    break;

  case 42: /* declarator: '(' attr_spec_list declarator ')'  */
#line 400 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                     {
			(yyval.nodep) = (yyvsp[-1].nodep);
			(yyval.nodep)->n_ap = attr_add((yyval.nodep)->n_ap, gcc_attr_wrapper((yyvsp[-2].nodep)));
		}
#line 2429 "y.tab.c"
    break;

  case 43: /* declarator: '(' declarator ')'  */
#line 404 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                      { (yyval.nodep) = (yyvsp[-1].nodep); }
#line 2435 "y.tab.c"
    break;

  case 44: /* declarator: declarator '[' ecq ']'  */
#line 405 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                          { (yyval.nodep) = biop(LB, (yyvsp[-3].nodep), (yyvsp[-1].nodep)); }
#line 2441 "y.tab.c"
    break;

  case 45: /* declarator: declarator '(' parameter_type_list ')'  */
#line 406 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                          {
			(yyval.nodep) = bdty(CALL, (yyvsp[-3].nodep), (yyvsp[-1].nodep));
		}
#line 2449 "y.tab.c"
    break;

  case 46: /* declarator: declarator '(' identifier_list ')'  */
#line 409 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                      {
			(yyval.nodep) = bdty(CALL, (yyvsp[-3].nodep), (yyvsp[-1].nodep));
			oldstyle = 1;
		}
#line 2458 "y.tab.c"
    break;

  case 47: /* declarator: declarator '(' ')'  */
#line 413 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                      { (yyval.nodep) = bdty(UCALL, (yyvsp[-2].nodep)); }
#line 2464 "y.tab.c"
    break;

  case 48: /* ecq: maybe_r  */
#line 416 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = bcon(NOOFFSET); }
#line 2470 "y.tab.c"
    break;

  case 49: /* ecq: e  */
#line 417 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                      { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2476 "y.tab.c"
    break;

  case 50: /* ecq: r e  */
#line 418 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                       { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2482 "y.tab.c"
    break;

  case 51: /* ecq: c maybe_r e  */
#line 419 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2488 "y.tab.c"
    break;

  case 52: /* ecq: r c e  */
#line 420 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                         { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2494 "y.tab.c"
    break;

  case 53: /* ecq: '*'  */
#line 421 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                       { (yyval.nodep) = bcon(NOOFFSET); }
#line 2500 "y.tab.c"
    break;

  case 54: /* ecq: r '*'  */
#line 422 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                         { (yyval.nodep) = bcon(NOOFFSET); }
#line 2506 "y.tab.c"
    break;

  case 55: /* r: C_QUALIFIER  */
#line 425 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                              {
			if ((yyvsp[0].type) != 0)
				uerror("bad qualifier");
		}
#line 2515 "y.tab.c"
    break;

  case 56: /* c: C_CLASS  */
#line 431 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                          {
			if ((yyvsp[0].type) != STATIC)
				uerror("bad class keyword");
		}
#line 2524 "y.tab.c"
    break;

  case 57: /* type_qualifier_list: C_QUALIFIER  */
#line 438 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               { (yyval.nodep) = biop(UMUL, 0, 0); (yyval.nodep)->n_qual = (yyvsp[0].type); }
#line 2530 "y.tab.c"
    break;

  case 58: /* type_qualifier_list: type_qualifier_list C_QUALIFIER  */
#line 439 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                   {
			(yyval.nodep) = (yyvsp[-1].nodep);
			(yyval.nodep)->n_qual |= (yyvsp[0].type);
		}
#line 2539 "y.tab.c"
    break;

  case 59: /* type_qualifier_list: attribute_specifier  */
#line 443 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       {
			(yyval.nodep) = block(UMUL, NULL, NULL, 0, 0, 0);
			(yyval.nodep)->n_ap = gcc_attr_wrapper((yyvsp[0].nodep));
		}
#line 2548 "y.tab.c"
    break;

  case 60: /* type_qualifier_list: type_qualifier_list attribute_specifier  */
#line 447 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                           {
			(yyvsp[-1].nodep)->n_ap = attr_add((yyvsp[-1].nodep)->n_ap, gcc_attr_wrapper((yyvsp[0].nodep)));
		}
#line 2556 "y.tab.c"
    break;

  case 61: /* identifier_list: C_NAME  */
#line 452 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                          { (yyval.nodep) = bdty(NAME, (yyvsp[0].strp)); oldargs((yyval.nodep)); }
#line 2562 "y.tab.c"
    break;

  case 62: /* identifier_list: identifier_list ',' C_NAME  */
#line 453 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                              {
			(yyval.nodep) = cmop((yyvsp[-2].nodep), bdty(NAME, (yyvsp[0].strp)));
			oldargs((yyval.nodep)->n_right);
		}
#line 2571 "y.tab.c"
    break;

  case 63: /* parameter_type_list: parameter_list  */
#line 463 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                  { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2577 "y.tab.c"
    break;

  case 64: /* parameter_type_list: parameter_list ',' C_ELLIPSIS  */
#line 464 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                 {
			(yyval.nodep) = cmop((yyvsp[-2].nodep), biop(ELLIPSIS, NULL, NULL));
		}
#line 2585 "y.tab.c"
    break;

  case 65: /* parameter_list: parameter_declaration  */
#line 474 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                         { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2591 "y.tab.c"
    break;

  case 66: /* parameter_list: parameter_list ',' parameter_declaration  */
#line 475 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                            {
			(yyval.nodep) = cmop((yyvsp[-2].nodep), (yyvsp[0].nodep));
		}
#line 2599 "y.tab.c"
    break;

  case 67: /* parameter_declaration: declaration_specifiers declarator attr_var  */
#line 484 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                              {
			if (glval((yyvsp[-2].nodep)) != SNULL && glval((yyvsp[-2].nodep)) != REGISTER)
				uerror("illegal parameter class");
			(yyval.nodep) = block(TYMERGE, (yyvsp[-2].nodep), (yyvsp[-1].nodep), INT, 0, 0);
			(yyval.nodep)->n_ap = gcc_attr_wrapper((yyvsp[0].nodep));
		}
#line 2610 "y.tab.c"
    break;

  case 68: /* parameter_declaration: declaration_specifiers abstract_declarator  */
#line 490 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                              { 
			(yyvsp[-1].nodep)->n_ap = attr_add((yyvsp[-1].nodep)->n_ap, (yyvsp[0].nodep)->n_ap);
			(yyval.nodep) = block(TYMERGE, (yyvsp[-1].nodep), (yyvsp[0].nodep), INT, 0, 0);
		}
#line 2619 "y.tab.c"
    break;

  case 69: /* parameter_declaration: declaration_specifiers  */
#line 494 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                          {
			(yyval.nodep) = block(TYMERGE, (yyvsp[0].nodep), bdty(NAME, NULL), INT, 0, 0);
		}
#line 2627 "y.tab.c"
    break;

  case 70: /* abstract_declarator: '*'  */
#line 500 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                       { (yyval.nodep) = bdty(UMUL, bdty(NAME, NULL)); }
#line 2633 "y.tab.c"
    break;

  case 71: /* abstract_declarator: '*' type_qualifier_list  */
#line 501 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                           {
			(yyval.nodep) = (yyvsp[0].nodep);
			(yyval.nodep)->n_left = bdty(NAME, NULL);
		}
#line 2642 "y.tab.c"
    break;

  case 72: /* abstract_declarator: '*' abstract_declarator  */
#line 505 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                           { (yyval.nodep) = bdty(UMUL, (yyvsp[0].nodep)); }
#line 2648 "y.tab.c"
    break;

  case 73: /* abstract_declarator: '*' type_qualifier_list abstract_declarator  */
#line 506 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                               {
			(yyval.nodep) = (yyvsp[-1].nodep);
			(yyval.nodep)->n_left = (yyvsp[0].nodep);
		}
#line 2657 "y.tab.c"
    break;

  case 74: /* abstract_declarator: '(' abstract_declarator ')'  */
#line 510 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                               { (yyval.nodep) = (yyvsp[-1].nodep); }
#line 2663 "y.tab.c"
    break;

  case 75: /* abstract_declarator: '[' ecq ']' attr_var  */
#line 511 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                        {
			(yyval.nodep) = block(LB, bdty(NAME, NULL), (yyvsp[-2].nodep), INT, 0, 0);
			(yyval.nodep)->n_ap = gcc_attr_wrapper((yyvsp[0].nodep));
		}
#line 2672 "y.tab.c"
    break;

  case 76: /* abstract_declarator: abstract_declarator '[' maybe_r ']' attr_var  */
#line 515 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                                {
			(yyval.nodep) = block(LB, (yyvsp[-4].nodep), bcon(NOOFFSET), INT, 0, 0);
			(yyval.nodep)->n_ap = gcc_attr_wrapper((yyvsp[0].nodep));
		}
#line 2681 "y.tab.c"
    break;

  case 77: /* abstract_declarator: abstract_declarator '[' e ']' attr_var  */
#line 519 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                          {
			(yyval.nodep) = block(LB, (yyvsp[-4].nodep), (yyvsp[-2].nodep), INT, 0, 0);
			(yyval.nodep)->n_ap = gcc_attr_wrapper((yyvsp[0].nodep));
		}
#line 2690 "y.tab.c"
    break;

  case 78: /* abstract_declarator: '(' ')' attr_var  */
#line 523 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                    {
			(yyval.nodep) = bdty(UCALL, bdty(NAME, NULL));
			(yyval.nodep)->n_ap = gcc_attr_wrapper((yyvsp[0].nodep));
		}
#line 2699 "y.tab.c"
    break;

  case 79: /* abstract_declarator: '(' ib2 parameter_type_list ')' attr_var  */
#line 527 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                            {
			(yyval.nodep) = block(CALL, bdty(NAME, NULL), (yyvsp[-2].nodep), INT, 0, 0);
			(yyval.nodep)->n_ap = gcc_attr_wrapper((yyvsp[0].nodep));
		}
#line 2708 "y.tab.c"
    break;

  case 80: /* abstract_declarator: abstract_declarator '(' ')' attr_var  */
#line 531 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                        {
			(yyval.nodep) = block(UCALL, (yyvsp[-3].nodep), NULL, INT, 0, 0);
			(yyval.nodep)->n_ap = gcc_attr_wrapper((yyvsp[0].nodep));
		}
#line 2717 "y.tab.c"
    break;

  case 81: /* abstract_declarator: abstract_declarator '(' ib2 parameter_type_list ')' attr_var  */
#line 535 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                                                {
			(yyval.nodep) = block(CALL, (yyvsp[-5].nodep), (yyvsp[-2].nodep), INT, 0, 0);
			(yyval.nodep)->n_ap = gcc_attr_wrapper((yyvsp[0].nodep));
		}
#line 2726 "y.tab.c"
    break;

  case 82: /* ib2: %empty  */
#line 541 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                   { }
#line 2732 "y.tab.c"
    break;

  case 83: /* maybe_r: %empty  */
#line 544 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                   { }
#line 2738 "y.tab.c"
    break;

  case 84: /* maybe_r: C_QUALIFIER  */
#line 545 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               { }
#line 2744 "y.tab.c"
    break;

  case 87: /* arg_declaration: declaration_specifiers arg_param_list ';'  */
#line 556 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                             {
			p1nfree((yyvsp[-2].nodep));
		}
#line 2752 "y.tab.c"
    break;

  case 88: /* arg_param_list: declarator attr_var  */
#line 561 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       {
			olddecl(block(TYMERGE, p1tcopy((yyvsp[-2].nodep)), (yyvsp[-1].nodep),
			    INT, 0, 0), (yyvsp[0].nodep));
		}
#line 2761 "y.tab.c"
    break;

  case 89: /* arg_param_list: arg_param_list ',' declarator attr_var  */
#line 565 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                          {
			olddecl(block(TYMERGE, p1tcopy((yyvsp[-4].nodep)), (yyvsp[-1].nodep),
			    INT, 0, 0), (yyvsp[0].nodep));
		}
#line 2770 "y.tab.c"
    break;

  case 93: /* block_item: statement  */
#line 579 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                             { stmtfree(); }
#line 2776 "y.tab.c"
    break;

  case 94: /* declaration: declaration_specifiers ';'  */
#line 589 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                              { p1tfree((yyvsp[-1].nodep)); fun_inline = 0; }
#line 2782 "y.tab.c"
    break;

  case 95: /* declaration: declaration_specifiers init_declarator_list ';'  */
#line 590 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                                   {
			p1tfree((yyvsp[-2].nodep));
			fun_inline = 0;
		}
#line 2791 "y.tab.c"
    break;

  case 96: /* declaration: C_STATICASSERT '(' e ',' string ')' ';'  */
#line 594 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                           {
			int r = con_e((yyvsp[-4].nodep));
			if (r == 0) /* false */
				uerror((yyvsp[-2].strp));
		}
#line 2801 "y.tab.c"
    break;

  case 97: /* init_declarator_list: init_declarator  */
#line 606 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                   { symclear(blevel); }
#line 2807 "y.tab.c"
    break;

  case 98: /* @1: %empty  */
#line 607 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                     { (yyval.nodep) = (yyvsp[-3].nodep); }
#line 2813 "y.tab.c"
    break;

  case 99: /* init_declarator_list: init_declarator_list ',' attr_var @1 init_declarator  */
#line 607 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                                                                {
			uawarn((yyvsp[-2].nodep), "init_declarator");
			symclear(blevel);
		}
#line 2822 "y.tab.c"
    break;

  case 100: /* enum_dcl: enum_head '{' moe_list optcomma '}'  */
#line 613 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                       { (yyval.nodep) = enumdcl((yyvsp[-4].symp)); }
#line 2828 "y.tab.c"
    break;

  case 101: /* enum_dcl: C_ENUM C_NAME  */
#line 614 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                 {  (yyval.nodep) = enumref((yyvsp[0].strp)); }
#line 2834 "y.tab.c"
    break;

  case 102: /* enum_head: C_ENUM  */
#line 617 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                          { (yyval.symp) = enumhd(NULL); }
#line 2840 "y.tab.c"
    break;

  case 103: /* enum_head: C_ENUM C_NAME  */
#line 618 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                 {  (yyval.symp) = enumhd((yyvsp[0].strp)); }
#line 2846 "y.tab.c"
    break;

  case 104: /* moe_list: moe  */
#line 621 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                       { moedef((yyvsp[0].strp), enummer++); }
#line 2852 "y.tab.c"
    break;

  case 105: /* moe_list: moe_list ',' moe  */
#line 622 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                    { moedef((yyvsp[0].strp), enummer++); }
#line 2858 "y.tab.c"
    break;

  case 108: /* moe: C_NAME '=' e  */
#line 627 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                { enummer = con_e((yyvsp[0].nodep)); (yyval.strp) = (yyvsp[-2].strp); }
#line 2864 "y.tab.c"
    break;

  case 109: /* moe: C_TYPENAME '=' e  */
#line 628 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                    { enummer = con_e((yyvsp[0].nodep)); (yyval.strp) = (yyvsp[-2].strp); }
#line 2870 "y.tab.c"
    break;

  case 110: /* struct_dcl: str_head '{' struct_dcl_list '}'  */
#line 631 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                    {
			P1ND *p;

			(yyval.nodep) = dclstruct((yyvsp[-3].rp));
			if (pragma_allpacked) {
				p = bdty(CALL, bdty(NAME, "packed"),
				    bcon(pragma_allpacked));
				(yyval.nodep)->n_ap = attr_add((yyval.nodep)->n_ap,gcc_attr_wrapper(p)); }
		}
#line 2884 "y.tab.c"
    break;

  case 111: /* struct_dcl: C_STRUCT attr_var C_NAME  */
#line 640 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                            { 
			(yyval.nodep) = rstruct((yyvsp[0].strp),(yyvsp[-2].intval));
			uawarn((yyvsp[-1].nodep), "struct_dcl");
		}
#line 2893 "y.tab.c"
    break;

  case 112: /* struct_dcl: str_head '{' '}'  */
#line 644 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                    { (yyval.nodep) = dclstruct((yyvsp[-2].rp)); }
#line 2899 "y.tab.c"
    break;

  case 113: /* attr_var: %empty  */
#line 647 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                   {	
			P1ND *q, *p;

			p = pragma_aligned ? bdty(CALL, bdty(NAME, "aligned"),
			    bcon(pragma_aligned)) : NULL;
			if (pragma_packed) {
				q = bdty(NAME, "packed");
				p = (p == NULL ? q : cmop(p, q));
			}
			pragma_aligned = pragma_packed = 0;
			(yyval.nodep) = p;
		}
#line 2916 "y.tab.c"
    break;

  case 116: /* attr_spec_list: attr_spec_list attribute_specifier  */
#line 663 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                      { (yyval.nodep) = cmop((yyvsp[-1].nodep), (yyvsp[0].nodep)); }
#line 2922 "y.tab.c"
    break;

  case 117: /* str_head: C_STRUCT attr_var  */
#line 666 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                     {  (yyval.rp) = bstruct(NULL, (yyvsp[-1].intval), (yyvsp[0].nodep));  }
#line 2928 "y.tab.c"
    break;

  case 118: /* str_head: C_STRUCT attr_var C_NAME  */
#line 667 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                            {  (yyval.rp) = bstruct((yyvsp[0].strp), (yyvsp[-2].intval), (yyvsp[-1].nodep));  }
#line 2934 "y.tab.c"
    break;

  case 121: /* struct_declaration: specifier_qualifier_list struct_declarator_list optsemi  */
#line 675 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                                           {
			p1tfree((yyvsp[-2].nodep));
		}
#line 2942 "y.tab.c"
    break;

  case 122: /* optsemi: ';'  */
#line 680 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                       { }
#line 2948 "y.tab.c"
    break;

  case 123: /* optsemi: optsemi ';'  */
#line 681 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               { werror("extra ; in struct"); }
#line 2954 "y.tab.c"
    break;

  case 124: /* specifier_qualifier_list: merge_specifiers  */
#line 685 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                    { (yyval.nodep) = typenode((yyvsp[0].nodep)); }
#line 2960 "y.tab.c"
    break;

  case 125: /* merge_specifiers: type_sq merge_specifiers  */
#line 688 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                            { (yyval.nodep) = cmop((yyvsp[0].nodep), (yyvsp[-1].nodep)); }
#line 2966 "y.tab.c"
    break;

  case 126: /* merge_specifiers: type_sq  */
#line 689 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = (yyvsp[0].nodep); }
#line 2972 "y.tab.c"
    break;

  case 127: /* struct_declarator_list: struct_declarator  */
#line 693 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                     { symclear(blevel); }
#line 2978 "y.tab.c"
    break;

  case 128: /* @2: %empty  */
#line 694 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                              { (yyval.nodep)=(yyvsp[-2].nodep); }
#line 2984 "y.tab.c"
    break;

  case 129: /* struct_declarator_list: struct_declarator_list ',' @2 struct_declarator  */
#line 695 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                          { symclear(blevel); }
#line 2990 "y.tab.c"
    break;

  case 130: /* struct_declarator: declarator attr_var  */
#line 698 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       {
			P1ND *p;

			(yyvsp[-1].nodep) = aryfix((yyvsp[-1].nodep));
			p = tymerge((yyvsp[-2].nodep), tymfix((yyvsp[-1].nodep)));
			if ((yyvsp[0].nodep))
				p->n_ap = attr_add(p->n_ap, gcc_attr_wrapper((yyvsp[0].nodep)));
			soumemb(p, (char *)(yyvsp[-1].nodep)->n_sp, 0);
			p1tfree(p);
		}
#line 3005 "y.tab.c"
    break;

  case 131: /* struct_declarator: ':' e  */
#line 708 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                         {
			int ie = con_e((yyvsp[0].nodep));
			if (fldchk(ie))
				ie = 1;
			falloc(NULL, ie, (yyvsp[-2].nodep));
		}
#line 3016 "y.tab.c"
    break;

  case 132: /* struct_declarator: declarator ':' e  */
#line 714 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                    {
			int ie = con_e((yyvsp[0].nodep));
			if (fldchk(ie))
				ie = 1;
			if ((yyvsp[-2].nodep)->n_op == NAME) {
				/* XXX - tymfix() may alter $1 */
				tymerge((yyvsp[-3].nodep), tymfix((yyvsp[-2].nodep)));
				soumemb((yyvsp[-2].nodep), (char *)(yyvsp[-2].nodep)->n_sp, FIELD | ie);
				p1nfree((yyvsp[-2].nodep));
			} else
				uerror("illegal declarator");
		}
#line 3033 "y.tab.c"
    break;

  case 133: /* struct_declarator: declarator ':' e attr_spec_list  */
#line 726 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                   {
			int ie = con_e((yyvsp[-1].nodep));
			if (fldchk(ie))
				ie = 1;
			if ((yyvsp[-3].nodep)->n_op == NAME) {
				/* XXX - tymfix() may alter $1 */
				tymerge((yyvsp[-4].nodep), tymfix((yyvsp[-3].nodep)));
				if ((yyvsp[0].nodep))
					(yyvsp[-3].nodep)->n_ap = attr_add((yyvsp[-3].nodep)->n_ap,
					    gcc_attr_wrapper((yyvsp[0].nodep)));
				soumemb((yyvsp[-3].nodep), (char *)(yyvsp[-3].nodep)->n_sp, FIELD | ie);
				p1nfree((yyvsp[-3].nodep));
			} else
				uerror("illegal declarator");
		}
#line 3053 "y.tab.c"
    break;

  case 134: /* struct_declarator: %empty  */
#line 741 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       {
			P1ND *p = (yyvsp[0].nodep);
			char *c = permalloc(10);

			if (p->n_type != STRTY && p->n_type != UNIONTY)
				uerror("bad unnamed member type");
			snprintf(c, 10, "*%dFAKE", getlab());
			soumemb(p, c, 0);
		}
#line 3067 "y.tab.c"
    break;

  case 135: /* xnfdeclarator: declarator attr_var  */
#line 753 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       {
			(yyval.symp) = xnf = init_declarator((yyvsp[-2].nodep), (yyvsp[-1].nodep), 1, (yyvsp[0].nodep), 0);
		}
#line 3075 "y.tab.c"
    break;

  case 136: /* xnfdeclarator: declarator C_ASM '(' svstr ')'  */
#line 756 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                  {
			(yyval.symp) = xnf = init_declarator((yyvsp[-5].nodep), (yyvsp[-4].nodep), 1, NULL, (yyvsp[-1].strp));
		}
#line 3083 "y.tab.c"
    break;

  case 137: /* init_declarator: declarator attr_var  */
#line 765 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       {
			init_declarator((yyvsp[-2].nodep), (yyvsp[-1].nodep), 0, (yyvsp[0].nodep), 0);
		}
#line 3091 "y.tab.c"
    break;

  case 138: /* init_declarator: declarator C_ASM '(' svstr ')' attr_var  */
#line 768 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                           {
			init_declarator((yyvsp[-6].nodep), (yyvsp[-5].nodep), 0, (yyvsp[0].nodep), (yyvsp[-2].strp));
		}
#line 3099 "y.tab.c"
    break;

  case 139: /* init_declarator: xnfdeclarator '=' e  */
#line 771 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       { 
			if ((yyvsp[-2].symp)->sclass == STATIC || (yyvsp[-2].symp)->sclass == EXTDEF)
				statinit++;
			simpleinit((yyvsp[-2].symp), eve((yyvsp[0].nodep)));
			if ((yyvsp[-2].symp)->sclass == STATIC || (yyvsp[-2].symp)->sclass == EXTDEF)
				statinit--;
			xnf = NULL;
		}
#line 3112 "y.tab.c"
    break;

  case 140: /* init_declarator: xnfdeclarator '=' begbr init_list optcomma '}'  */
#line 779 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                                  {
			endinit((yyvsp[-3].ctx), 0);
			xnf = NULL;
		}
#line 3121 "y.tab.c"
    break;

  case 141: /* init_declarator: xnfdeclarator '=' begbr '}'  */
#line 783 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                               { endinit((yyvsp[-1].ctx), 0); xnf = NULL; }
#line 3127 "y.tab.c"
    break;

  case 142: /* begbr: '{'  */
#line 786 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                       { (yyval.ctx) = beginit((yyvsp[(-1) - (1)].symp)); }
#line 3133 "y.tab.c"
    break;

  case 143: /* initializer: e  */
#line 789 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               {  (yyval.nodep) = (yyvsp[0].nodep); }
#line 3139 "y.tab.c"
    break;

  case 144: /* @3: %empty  */
#line 790 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                          { (yyval.ctx) = (yyvsp[(-1) - (1)].ctx); }
#line 3145 "y.tab.c"
    break;

  case 145: /* initializer: ibrace @3 init_list optcomma '}'  */
#line 791 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                               { (yyval.nodep) = NULL; }
#line 3151 "y.tab.c"
    break;

  case 146: /* initializer: ibrace '}'  */
#line 792 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                              { asginit((yyvsp[(-1) - (2)].ctx), bcon(0)); (yyval.nodep) = NULL; }
#line 3157 "y.tab.c"
    break;

  case 147: /* init_list: designation initializer  */
#line 795 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                           { dainit((yyvsp[-2].ctx), (yyvsp[-1].nodep), (yyvsp[0].nodep)); }
#line 3163 "y.tab.c"
    break;

  case 148: /* @4: %empty  */
#line 796 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                 { (yyval.ctx) = (yyvsp[-2].ctx); }
#line 3169 "y.tab.c"
    break;

  case 149: /* init_list: init_list ',' @4 designation initializer  */
#line 797 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                { dainit((yyvsp[-5].ctx), (yyvsp[-1].nodep), (yyvsp[0].nodep)); }
#line 3175 "y.tab.c"
    break;

  case 150: /* designation: designator_list '='  */
#line 800 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       { desinit((yyvsp[-2].ctx), (yyvsp[-1].nodep)); (yyval.nodep) = NULL; }
#line 3181 "y.tab.c"
    break;

  case 151: /* designation: GCC_DESIG  */
#line 801 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                             { desinit((yyvsp[-1].ctx), bdty(NAME, (yyvsp[0].strp))); (yyval.nodep) = NULL; }
#line 3187 "y.tab.c"
    break;

  case 152: /* designation: '[' e C_ELLIPSIS e ']' '='  */
#line 802 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                              { (yyval.nodep) = biop(CM, (yyvsp[-4].nodep), (yyvsp[-2].nodep)); }
#line 3193 "y.tab.c"
    break;

  case 153: /* designation: %empty  */
#line 803 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                   { (yyval.nodep) = NULL; }
#line 3199 "y.tab.c"
    break;

  case 154: /* designator_list: designator  */
#line 806 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                              { (yyval.nodep) = (yyvsp[0].nodep); }
#line 3205 "y.tab.c"
    break;

  case 155: /* designator_list: designator_list designator  */
#line 807 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                              { (yyval.nodep) = (yyvsp[0].nodep); (yyval.nodep)->n_left = (yyvsp[-1].nodep); }
#line 3211 "y.tab.c"
    break;

  case 156: /* designator: '[' e ']'  */
#line 810 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                             {
			int ie = con_e((yyvsp[-1].nodep));
			if (ie < 0) {
				uerror("designator must be non-negative");
				ie = 0;
			}
			(yyval.nodep) = biop(LB, NULL, bcon(ie));
		}
#line 3224 "y.tab.c"
    break;

  case 157: /* designator: C_STROP C_TYPENAME  */
#line 818 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                      {
			if ((yyvsp[-1].intval) != DOT)
				uerror("invalid designator");
			(yyval.nodep) = bdty(NAME, (yyvsp[0].strp));
		}
#line 3234 "y.tab.c"
    break;

  case 158: /* designator: C_STROP C_NAME  */
#line 823 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                  {
			if ((yyvsp[-1].intval) != DOT)
				uerror("invalid designator");
			(yyval.nodep) = bdty(NAME, (yyvsp[0].strp));
		}
#line 3244 "y.tab.c"
    break;

  case 161: /* ibrace: '{'  */
#line 834 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                       {  ilbrace((yyvsp[(-1) - (1)].ctx)); }
#line 3250 "y.tab.c"
    break;

  case 162: /* compoundstmt: begin block_item_list '}'  */
#line 839 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                             { flend(); }
#line 3256 "y.tab.c"
    break;

  case 163: /* compoundstmt: begin '}'  */
#line 840 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                             { flend(); }
#line 3262 "y.tab.c"
    break;

  case 164: /* begin: '{'  */
#line 843 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                      {
			struct savbc *bc = malloc(sizeof(struct savbc));
			if (blevel == 1) {
#ifdef STABS
				if (gflag)
					stabs_line(lineno);
#endif
				dclargs();
			}
#ifdef STABS
			if (gflag && blevel > 1)
				stabs_lbrac(blevel+1);
#endif
			++blevel;
			oldstyle = 0;
			bc->contlab = autooff;
			bc->next = savctx;
			bc->bkptr = bkpole;
			bc->bkoff = cbkp;
			bc->stptr = sapole;
			bc->stoff = cstp;
			bc->numnode = usdnodes;
			usdnodes = 0;
			bkpole = sapole = NULL;
			cbkp = cstp = 0;
			savctx = bc;
			if (!isinlining && sspflag && blevel == 2)
				sspstart();
		}
#line 3296 "y.tab.c"
    break;

  case 165: /* statement: e ';'  */
#line 874 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                         { ecomp(eve((yyvsp[-1].nodep))); symclear(blevel); }
#line 3302 "y.tab.c"
    break;

  case 167: /* statement: ifprefix statement  */
#line 876 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                      { plabel((yyvsp[-1].intval)); reached = 1; }
#line 3308 "y.tab.c"
    break;

  case 168: /* statement: ifelprefix statement  */
#line 877 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                        {
			if ((yyvsp[-1].intval) != NOLAB) {
				plabel( (yyvsp[-1].intval));
				reached = 1;
			}
		}
#line 3319 "y.tab.c"
    break;

  case 169: /* statement: whprefix statement  */
#line 883 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                      {
			branch(contlab);
			plabel( brklab );
			if( (flostat&FBRK) || !(flostat&FLOOP))
				reached = 1;
			else
				reached = 0;
			resetbc(0);
		}
#line 3333 "y.tab.c"
    break;

  case 170: /* statement: doprefix statement C_WHILE '(' e ')' ';'  */
#line 892 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                            {
			plabel(contlab);
			if (flostat & FCONT)
				reached = 1;
			if (reached)
				cbranch(buildtree(NE, eve((yyvsp[-2].nodep)), bcon(0)),
				    bcon((yyvsp[-6].intval)));
			else
				p1tfree(eve((yyvsp[-2].nodep)));
			plabel( brklab);
			reached = 1;
			resetbc(0);
		}
#line 3351 "y.tab.c"
    break;

  case 171: /* statement: forprefix .e ')' statement  */
#line 906 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                        {  plabel( contlab );
			    if( flostat&FCONT ) reached = 1;
			    if( (yyvsp[-2].nodep) ) ecomp( (yyvsp[-2].nodep) );
			    branch((yyvsp[-3].intval));
			    plabel( brklab );
			    if( (flostat&FBRK) || !(flostat&FLOOP) ) reached = 1;
			    else reached = 0;
			    resetbc(0);
			    blevel--;
			    symclear(blevel);
			    }
#line 3367 "y.tab.c"
    break;

  case 172: /* statement: switchpart statement  */
#line 918 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                        { if( reached ) branch( brklab );
			    plabel( (yyvsp[-1].intval) );
			    swend();
			    plabel( brklab);
			    if( (flostat&FBRK) || !(flostat&FDEF) ) reached = 1;
			    resetbc(FCONT);
			    }
#line 3379 "y.tab.c"
    break;

  case 173: /* statement: C_BREAK ';'  */
#line 925 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                {
			if (brklab == NOLAB)
				uerror("illegal break");
			else if (reached)
				branch(brklab);
			flostat |= FBRK;
			reached = 0;
		}
#line 3392 "y.tab.c"
    break;

  case 174: /* statement: C_CONTINUE ';'  */
#line 933 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                   {
			if (contlab == NOLAB)
				uerror("illegal continue");
			else
				branch(contlab);
			flostat |= FCONT;
			goto rch;
		}
#line 3405 "y.tab.c"
    break;

  case 175: /* statement: C_RETURN ';'  */
#line 941 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                 {
			branch(retlab);
			if (cftnsp->stype != VOID && 
			    (cftnsp->sflags & NORETYP) == 0 &&
			    cftnsp->stype != VOID+FTN)
				uerror("return value required");
			rch:
			if (!reached)
				warner(Wunreachable_code);
			reached = 0;
		}
#line 3421 "y.tab.c"
    break;

  case 176: /* statement: C_RETURN e ';'  */
#line 952 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                   {
			P1ND *p, *q;

			p = nametree(cftnsp);
			p->n_type = DECREF(p->n_type);
			q = eve((yyvsp[-1].nodep));
#ifdef TARGET_TIMODE  
			{
			P1ND *r;
			if ((r = gcc_eval_ticast(RETURN, p, q)) != NULL)
				q = r;
			}
#endif
#ifndef NO_COMPLEX
			if (ANYCX(q) || ANYCX(p))
				q = cxret(q, p);
			else if (ISITY(p->n_type) || ISITY(q->n_type)) {
				q = imret(q, p);
				if (ISITY(p->n_type))
					p->n_type -= (FIMAG-FLOAT);
				if (ISITY(q->n_type))
					q->n_type -= (FIMAG-FLOAT);
			}
#endif
			p = buildtree(RETURN, p, q);
			if (p->n_type == VOID) {
				ecomp(p->n_right);
			} else {
				if (cftnod == NULL) {
					P1ND *r = tempnode(0, p->n_type,
					    p->n_df, p->pss);
					cftnod = tmpalloc(sizeof(P1ND));
					*cftnod = *r;
					p1tfree(r);
				}
				ecomp(buildtree(ASSIGN,
				    p1tcopy(cftnod), p->n_right));
			}
			p1tfree(p->n_left);
			p1nfree(p);
			branch(retlab);
			reached = 0;
		}
#line 3469 "y.tab.c"
    break;

  case 177: /* statement: C_GOTO C_NAME ';'  */
#line 995 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                     { gotolabel((yyvsp[-1].strp)); goto rch; }
#line 3475 "y.tab.c"
    break;

  case 178: /* statement: C_GOTO '*' e ';'  */
#line 996 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                    { ecomp(biop(GOTO, eve((yyvsp[-1].nodep)), NULL)); }
#line 3481 "y.tab.c"
    break;

  case 184: /* asmstatement: C_ASM mvol '(' svstr ')'  */
#line 1004 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                            { send_passt(IP_ASM, mkpstr((yyvsp[-1].strp))); }
#line 3487 "y.tab.c"
    break;

  case 185: /* asmstatement: C_ASM mvol '(' svstr xasm ')'  */
#line 1005 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                 { mkxasm((yyvsp[-2].strp), (yyvsp[-1].nodep)); }
#line 3493 "y.tab.c"
    break;

  case 186: /* svstr: string  */
#line 1008 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                         { (yyval.strp) = addstring((yyvsp[0].strp)); }
#line 3499 "y.tab.c"
    break;

  case 188: /* mvol: C_QUALIFIER  */
#line 1012 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               { }
#line 3505 "y.tab.c"
    break;

  case 189: /* xasm: ':' oplist  */
#line 1015 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                              { (yyval.nodep) = xcmop((yyvsp[0].nodep), NULL, NULL); }
#line 3511 "y.tab.c"
    break;

  case 190: /* xasm: ':' oplist ':' oplist  */
#line 1016 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                         { (yyval.nodep) = xcmop((yyvsp[-2].nodep), (yyvsp[0].nodep), NULL); }
#line 3517 "y.tab.c"
    break;

  case 191: /* xasm: ':' oplist ':' oplist ':' cnstr  */
#line 1017 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                   { (yyval.nodep) = xcmop((yyvsp[-4].nodep), (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3523 "y.tab.c"
    break;

  case 192: /* oplist: %empty  */
#line 1020 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                 { (yyval.nodep) = NULL; }
#line 3529 "y.tab.c"
    break;

  case 193: /* oplist: oper  */
#line 1021 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                        { (yyval.nodep) = (yyvsp[0].nodep); }
#line 3535 "y.tab.c"
    break;

  case 194: /* oper: svstr '(' e ')'  */
#line 1024 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                   { (yyval.nodep) = xasmop((yyvsp[-3].strp), pconvert(eve((yyvsp[-1].nodep)))); }
#line 3541 "y.tab.c"
    break;

  case 195: /* oper: oper ',' svstr '(' e ')'  */
#line 1025 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                            {
			(yyval.nodep) = cmop((yyvsp[-5].nodep), xasmop((yyvsp[-3].strp), pconvert(eve((yyvsp[-1].nodep)))));
		}
#line 3549 "y.tab.c"
    break;

  case 196: /* cnstr: svstr  */
#line 1030 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                         { (yyval.nodep) = xasmop((yyvsp[0].strp), bcon(0)); }
#line 3555 "y.tab.c"
    break;

  case 197: /* cnstr: cnstr ',' svstr  */
#line 1031 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                   { (yyval.nodep) = cmop((yyvsp[-2].nodep), xasmop((yyvsp[0].strp), bcon(0))); }
#line 3561 "y.tab.c"
    break;

  case 198: /* label: C_NAME ':' attr_var  */
#line 1034 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       { deflabel((yyvsp[-2].strp), (yyvsp[0].nodep)); reached = 1; }
#line 3567 "y.tab.c"
    break;

  case 199: /* label: C_TYPENAME ':' attr_var  */
#line 1035 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                           { deflabel((yyvsp[-2].strp), (yyvsp[0].nodep)); reached = 1; }
#line 3573 "y.tab.c"
    break;

  case 200: /* label: C_CASE e ':'  */
#line 1036 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                { addcase(eve((yyvsp[-1].nodep))); reached = 1; }
#line 3579 "y.tab.c"
    break;

  case 201: /* label: C_CASE e C_ELLIPSIS e ':'  */
#line 1037 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                             {
#ifdef GCC_COMPAT
			gcccase(eve((yyvsp[-3].nodep)), eve((yyvsp[-1].nodep))); reached = 1;
#endif
		}
#line 3589 "y.tab.c"
    break;

  case 202: /* label: C_DEFAULT ':'  */
#line 1042 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                 { reached = 1; adddef(); flostat |= FDEF; }
#line 3595 "y.tab.c"
    break;

  case 203: /* doprefix: C_DO  */
#line 1045 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                     {
			savebc();
			brklab = getlab();
			contlab = getlab();
			plabel(  (yyval.intval) = getlab());
			reached = 1;
		}
#line 3607 "y.tab.c"
    break;

  case 204: /* ifprefix: C_IF '(' e ')'  */
#line 1053 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               {
			xcbranch(eve((yyvsp[-1].nodep)), (yyval.intval) = getlab());
			reached = 1;
		}
#line 3616 "y.tab.c"
    break;

  case 205: /* ifelprefix: ifprefix statement C_ELSE  */
#line 1058 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                            {
			if (reached)
				branch((yyval.intval) = getlab());
			else
				(yyval.intval) = NOLAB;
			plabel( (yyvsp[-2].intval));
			reached = 1;
		}
#line 3629 "y.tab.c"
    break;

  case 206: /* whprefix: C_WHILE '(' e ')'  */
#line 1068 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       {
			savebc();
			(yyvsp[-1].nodep) = eve((yyvsp[-1].nodep));
			if ((yyvsp[-1].nodep)->n_op == ICON && glval((yyvsp[-1].nodep)) != 0)
				flostat = FLOOP | (flostat & FP_CONTR_CBR);
			plabel( contlab = getlab());
			reached = 1;
			brklab = getlab();
			if (flostat & FLOOP)
				p1tfree((yyvsp[-1].nodep));
			else
				xcbranch((yyvsp[-1].nodep), brklab);
		}
#line 3647 "y.tab.c"
    break;

  case 207: /* forprefix: C_FOR '(' .e ';' .e ';'  */
#line 1082 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                              {
			++blevel;
			if ((yyvsp[-3].nodep))
				ecomp((yyvsp[-3].nodep));
			savebc();
			contlab = getlab();
			brklab = getlab();
			plabel( (yyval.intval) = getlab());
			reached = 1;
			if ((yyvsp[-1].nodep))
				xcbranch((yyvsp[-1].nodep), brklab);
			else
				flostat |= FLOOP;
		}
#line 3666 "y.tab.c"
    break;

  case 208: /* $@5: %empty  */
#line 1096 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                             { ++blevel; }
#line 3672 "y.tab.c"
    break;

  case 209: /* forprefix: C_FOR '(' $@5 declaration .e ';'  */
#line 1096 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                              {
			savebc();
			contlab = getlab();
			brklab = getlab();
			plabel( (yyval.intval) = getlab());
			reached = 1;
			if ((yyvsp[-1].nodep))
				xcbranch((yyvsp[-1].nodep), brklab);
			else
				flostat |= FLOOP;
		}
#line 3688 "y.tab.c"
    break;

  case 210: /* switchpart: C_SWITCH '(' e ')'  */
#line 1109 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                        {
			P1ND *p;
			int num;
			TWORD t;

			savebc();
			brklab = getlab();
			(yyvsp[-1].nodep) = eve((yyvsp[-1].nodep));
			if (!ISINTEGER((yyvsp[-1].nodep)->n_type)) {
				uerror("switch expression must have integer "
				       "type");
				t = INT;
			} else {
				(yyvsp[-1].nodep) = intprom((yyvsp[-1].nodep));
				t = (yyvsp[-1].nodep)->n_type;
			}
			p = tempnode(0, t, 0, 0);
			num = regno(p);
			ecomp(buildtree(ASSIGN, p, (yyvsp[-1].nodep)));
			branch( (yyval.intval) = getlab());
			swstart(num, t);
			reached = 0;
		}
#line 3716 "y.tab.c"
    break;

  case 211: /* .e: e  */
#line 1134 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                     { (yyval.nodep) = eve((yyvsp[0].nodep)); }
#line 3722 "y.tab.c"
    break;

  case 212: /* .e: %empty  */
#line 1135 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                        { (yyval.nodep)=0; }
#line 3728 "y.tab.c"
    break;

  case 213: /* elist: %empty  */
#line 1138 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                   { (yyval.nodep) = NULL; }
#line 3734 "y.tab.c"
    break;

  case 214: /* elist: e2  */
#line 1139 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                      { (yyval.nodep) = (yyvsp[0].nodep); }
#line 3740 "y.tab.c"
    break;

  case 216: /* e2: e2 ',' e  */
#line 1143 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                              { (yyval.nodep) = biop(CM, (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3746 "y.tab.c"
    break;

  case 217: /* e2: e2 ',' cast_type  */
#line 1144 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                      { /* hack for stdarg */
			TYMFIX((yyvsp[0].nodep));
			(yyvsp[0].nodep)->n_op = TYPE;
			(yyval.nodep) = biop(CM, (yyvsp[-2].nodep), (yyvsp[0].nodep));
		}
#line 3756 "y.tab.c"
    break;

  case 218: /* e2: cast_type  */
#line 1149 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                             { TYMFIX((yyvsp[0].nodep)); (yyvsp[0].nodep)->n_op = TYPE; (yyval.nodep) = (yyvsp[0].nodep); }
#line 3762 "y.tab.c"
    break;

  case 219: /* e: e ',' e  */
#line 1155 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = biop(COMOP, (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3768 "y.tab.c"
    break;

  case 220: /* e: e '=' e  */
#line 1156 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           {  (yyval.nodep) = biop(ASSIGN, (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3774 "y.tab.c"
    break;

  case 221: /* e: e C_ASOP e  */
#line 1157 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                              {  (yyval.nodep) = biop((yyvsp[-1].intval), (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3780 "y.tab.c"
    break;

  case 222: /* e: e '?' e ':' e  */
#line 1158 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                 { (yyval.nodep)=biop(QUEST, (yyvsp[-4].nodep), biop(COLON, (yyvsp[-2].nodep), (yyvsp[0].nodep))); }
#line 3786 "y.tab.c"
    break;

  case 223: /* e: e '?' ':' e  */
#line 1159 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               { (yyval.nodep) = biop(BIQUEST, (yyvsp[-3].nodep), (yyvsp[0].nodep)); }
#line 3792 "y.tab.c"
    break;

  case 224: /* e: e C_OROR e  */
#line 1160 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                              { (yyval.nodep) = biop((yyvsp[-1].intval), (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3798 "y.tab.c"
    break;

  case 225: /* e: e C_ANDAND e  */
#line 1161 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                { (yyval.nodep) = biop((yyvsp[-1].intval), (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3804 "y.tab.c"
    break;

  case 226: /* e: e '|' e  */
#line 1162 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = biop(OR, (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3810 "y.tab.c"
    break;

  case 227: /* e: e '^' e  */
#line 1163 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = biop(ER, (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3816 "y.tab.c"
    break;

  case 228: /* e: e '&' e  */
#line 1164 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = biop(AND, (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3822 "y.tab.c"
    break;

  case 229: /* e: e C_EQUOP e  */
#line 1165 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                { (yyval.nodep) = biop((yyvsp[-1].intval), (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3828 "y.tab.c"
    break;

  case 230: /* e: e C_RELOP e  */
#line 1166 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               { (yyval.nodep) = biop((yyvsp[-1].intval), (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3834 "y.tab.c"
    break;

  case 231: /* e: e C_SHIFTOP e  */
#line 1167 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                 { (yyval.nodep) = biop((yyvsp[-1].intval), (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3840 "y.tab.c"
    break;

  case 232: /* e: e '+' e  */
#line 1168 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = biop(PLUS, (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3846 "y.tab.c"
    break;

  case 233: /* e: e '-' e  */
#line 1169 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = biop(MINUS, (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3852 "y.tab.c"
    break;

  case 234: /* e: e C_DIVOP e  */
#line 1170 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               { (yyval.nodep) = biop((yyvsp[-1].intval), (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3858 "y.tab.c"
    break;

  case 235: /* e: e '*' e  */
#line 1171 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                           { (yyval.nodep) = biop(MUL, (yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 3864 "y.tab.c"
    break;

  case 237: /* xbegin: begin  */
#line 1175 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                         {
			(yyval.intval) = getlab(); getlab(); getlab();
			branch((yyval.intval)); plabel(((yyval.intval))+2);
		}
#line 3873 "y.tab.c"
    break;

  case 238: /* term: term C_INCOP  */
#line 1181 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                {  (yyval.nodep) = biop((yyvsp[0].intval), (yyvsp[-1].nodep), bcon(1)); }
#line 3879 "y.tab.c"
    break;

  case 239: /* term: '*' term  */
#line 1182 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                            { (yyval.nodep) = biop(UMUL, (yyvsp[0].nodep), NULL); }
#line 3885 "y.tab.c"
    break;

  case 240: /* term: '&' term  */
#line 1183 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                            { (yyval.nodep) = biop(ADDROF, (yyvsp[0].nodep), NULL); }
#line 3891 "y.tab.c"
    break;

  case 241: /* term: '-' term  */
#line 1184 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                            { (yyval.nodep) = biop(UMINUS, (yyvsp[0].nodep), NULL ); }
#line 3897 "y.tab.c"
    break;

  case 242: /* term: '+' term  */
#line 1185 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                            { (yyval.nodep) = biop(UPLUS, (yyvsp[0].nodep), NULL ); }
#line 3903 "y.tab.c"
    break;

  case 243: /* term: C_UNOP term  */
#line 1186 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                               { (yyval.nodep) = biop((yyvsp[-1].intval), (yyvsp[0].nodep), NULL); }
#line 3909 "y.tab.c"
    break;

  case 244: /* term: C_INCOP term  */
#line 1187 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                {
			(yyval.nodep) = biop((yyvsp[-1].intval) == INCR ? PLUSEQ : MINUSEQ, (yyvsp[0].nodep), bcon(1));
		}
#line 3917 "y.tab.c"
    break;

  case 245: /* term: C_SIZEOF xa term  */
#line 1190 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                    { (yyval.nodep) = biop(SZOF, (yyvsp[0].nodep), bcon(0)); inattr = (yyvsp[-1].intval); }
#line 3923 "y.tab.c"
    break;

  case 246: /* term: '(' cast_type ')' term  */
#line 1191 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                         {
			TYMFIX((yyvsp[-2].nodep));
			(yyval.nodep) = biop(CAST, (yyvsp[-2].nodep), (yyvsp[0].nodep));
		}
#line 3932 "y.tab.c"
    break;

  case 247: /* term: C_SIZEOF xa '(' cast_type ')'  */
#line 1195 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                                 {
			(yyval.nodep) = biop(SZOF, (yyvsp[-1].nodep), bcon(1));
			inattr = (yyvsp[-3].intval);
		}
#line 3941 "y.tab.c"
    break;

  case 248: /* term: C_ALIGNOF xa '(' cast_type ')'  */
#line 1199 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                  {
			int al;
			TYMFIX((yyvsp[-1].nodep));
			al = talign((yyvsp[-1].nodep)->n_type, (yyvsp[-1].nodep)->pss);
			(yyval.nodep) = bcon(al/SZCHAR);
			inattr = (yyvsp[-3].intval);
			p1tfree((yyvsp[-1].nodep));
		}
#line 3954 "y.tab.c"
    break;

  case 249: /* term: '(' cast_type ')' clbrace init_list optcomma '}'  */
#line 1207 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                                   {
			(yyval.nodep) = bdty(NAME, endinit((yyvsp[-3].ctx), 0));
			(yyval.nodep)->n_op = CLOP;
		}
#line 3963 "y.tab.c"
    break;

  case 250: /* term: '(' cast_type ')' clbrace '}'  */
#line 1211 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                {
			(yyval.nodep) = bdty(NAME, endinit((yyvsp[-1].ctx), 0));
			(yyval.nodep)->n_op = CLOP;
		}
#line 3972 "y.tab.c"
    break;

  case 251: /* term: term '[' e ']'  */
#line 1215 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                  { (yyval.nodep) = biop(LB, (yyvsp[-3].nodep), (yyvsp[-1].nodep)); }
#line 3978 "y.tab.c"
    break;

  case 252: /* term: C_NAME '(' elist ')'  */
#line 1216 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                         {
			(yyval.nodep) = biop((yyvsp[-1].nodep) ? CALL : UCALL, bdty(NAME, (yyvsp[-3].strp)), (yyvsp[-1].nodep));
		}
#line 3986 "y.tab.c"
    break;

  case 253: /* term: term '(' elist ')'  */
#line 1219 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       { (yyval.nodep) = biop((yyvsp[-1].nodep) ? CALL : UCALL, (yyvsp[-3].nodep), (yyvsp[-1].nodep)); }
#line 3992 "y.tab.c"
    break;

  case 254: /* term: term C_STROP C_NAME  */
#line 1220 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                       { (yyval.nodep) = biop((yyvsp[-1].intval), (yyvsp[-2].nodep), bdty(NAME, (yyvsp[0].strp))); }
#line 3998 "y.tab.c"
    break;

  case 255: /* term: term C_STROP C_TYPENAME  */
#line 1221 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                           { (yyval.nodep) = biop((yyvsp[-1].intval), (yyvsp[-2].nodep), bdty(NAME, (yyvsp[0].strp)));}
#line 4004 "y.tab.c"
    break;

  case 256: /* term: C_NAME  */
#line 1222 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                      { (yyval.nodep) = bdty(NAME, (yyvsp[0].strp)); }
#line 4010 "y.tab.c"
    break;

  case 257: /* term: PCC_OFFSETOF '(' cast_type ',' term ')'  */
#line 1223 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                            {
			TYMFIX((yyvsp[-3].nodep));
			(yyvsp[-3].nodep)->n_type = INCREF((yyvsp[-3].nodep)->n_type);
			(yyvsp[-3].nodep) = biop(CAST, (yyvsp[-3].nodep), bcon(0));
			if ((yyvsp[-1].nodep)->n_op == NAME) {
				(yyval.nodep) = biop(STREF, (yyvsp[-3].nodep), (yyvsp[-1].nodep));
			} else {
				P1ND *p = (yyvsp[-1].nodep);
				while (p->n_left->n_op != NAME)
					p = p->n_left;
				p->n_left = biop(STREF, (yyvsp[-3].nodep), p->n_left);
				(yyval.nodep) = (yyvsp[-1].nodep);
			}
			(yyval.nodep) = biop(ADDROF, (yyval.nodep), NULL);
			(yyvsp[-3].nodep) = block(NAME, NULL, NULL, ENUNSIGN(INTPTR), 0, 0);
			(yyval.nodep) = biop(CAST, (yyvsp[-3].nodep), (yyval.nodep));
		}
#line 4032 "y.tab.c"
    break;

  case 258: /* term: C_ICON  */
#line 1240 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                          { (yyval.nodep) = bdty(ICON, &((yyvsp[0].li))); }
#line 4038 "y.tab.c"
    break;

  case 259: /* term: C_FCON  */
#line 1241 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                          { (yyval.nodep) = bdty(FCON, &((yyvsp[0].flt))); }
#line 4044 "y.tab.c"
    break;

  case 260: /* term: svstr  */
#line 1242 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                         { (yyval.nodep) = bdty(STRING, (yyvsp[0].strp), styp()); }
#line 4050 "y.tab.c"
    break;

  case 261: /* term: '(' e ')'  */
#line 1243 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                             { (yyval.nodep)=(yyvsp[-1].nodep); }
#line 4056 "y.tab.c"
    break;

  case 262: /* term: '(' xbegin e ';' '}' ')'  */
#line 1244 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                            { (yyval.nodep) = gccexpr((yyvsp[-4].intval), eve((yyvsp[-3].nodep))); }
#line 4062 "y.tab.c"
    break;

  case 263: /* term: '(' xbegin block_item_list e ';' '}' ')'  */
#line 1245 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                            {
			(yyval.nodep) = gccexpr((yyvsp[-5].intval), eve((yyvsp[-3].nodep)));
		}
#line 4070 "y.tab.c"
    break;

  case 264: /* term: '(' xbegin block_item_list '}' ')'  */
#line 1248 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                      { 
			(yyval.nodep) = gccexpr((yyvsp[-3].intval), voidcon());
		}
#line 4078 "y.tab.c"
    break;

  case 265: /* term: C_ANDAND C_NAME  */
#line 1251 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                  {
			struct symtab *s = lookup((yyvsp[0].strp), SLBLNAME|STEMP);
			if (s->soffset == 0) {
				s->soffset = -getlab();
				s->sclass = STATIC;
			}
			savlab(s->soffset);
			(yyval.nodep) = biop(ADDROF, bdty(GOTO, (yyvsp[0].strp)), NULL);
		}
#line 4092 "y.tab.c"
    break;

  case 266: /* term: C_GENERIC '(' e ',' gen_ass_list ')'  */
#line 1260 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                       { (yyval.nodep) = dogen((yyvsp[-1].g), (yyvsp[-3].nodep)); }
#line 4098 "y.tab.c"
    break;

  case 267: /* gen_ass_list: gen_assoc  */
#line 1263 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                            { (yyval.g) = (yyvsp[0].g); }
#line 4104 "y.tab.c"
    break;

  case 268: /* gen_ass_list: gen_ass_list ',' gen_assoc  */
#line 1264 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                             { (yyval.g) = addgen((yyvsp[-2].g), (yyvsp[0].g)); }
#line 4110 "y.tab.c"
    break;

  case 269: /* gen_assoc: cast_type ':' e  */
#line 1267 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                  { TYMFIX((yyvsp[-2].nodep)); (yyval.g) = newgen((yyvsp[-2].nodep), (yyvsp[0].nodep)); }
#line 4116 "y.tab.c"
    break;

  case 270: /* gen_assoc: C_DEFAULT ':' e  */
#line 1268 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                  { (yyval.g) = newgen(0, (yyvsp[0].nodep)); }
#line 4122 "y.tab.c"
    break;

  case 271: /* xa: %empty  */
#line 1271 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                  { (yyval.intval) = inattr; inattr = 0; }
#line 4128 "y.tab.c"
    break;

  case 272: /* clbrace: '{'  */
#line 1274 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                        { P1ND *q = (yyvsp[(-1) - (1)].nodep); TYMFIX(q); (yyval.ctx) = clbrace(q); }
#line 4134 "y.tab.c"
    break;

  case 273: /* string: C_STRING  */
#line 1277 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                            { (yyval.strp) = stradd(NULL, (yyvsp[0].strp)); }
#line 4140 "y.tab.c"
    break;

  case 274: /* string: string C_STRING  */
#line 1278 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                   { (yyval.strp) = stradd((yyvsp[-1].strp), (yyvsp[0].strp)); }
#line 4146 "y.tab.c"
    break;

  case 275: /* cast_type: specifier_qualifier_list  */
#line 1281 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                            {
			(yyval.nodep) = biop(TYMERGE, (yyvsp[0].nodep), bdty(NAME, NULL));
		}
#line 4154 "y.tab.c"
    break;

  case 276: /* cast_type: specifier_qualifier_list abstract_declarator  */
#line 1284 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"
                                                                {
			(yyval.nodep) = biop(TYMERGE, (yyvsp[-1].nodep), aryfix((yyvsp[0].nodep)));
		}
#line 4162 "y.tab.c"
    break;


#line 4166 "y.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1289 "/home/pi/projects/TUS/sources/pcc/cc/ccom/cgram.y"


P1ND *
mkty(TWORD t, union dimfun *d, struct ssdesc *ss)
{
	return block(TYPE, NULL, NULL, t, d, ss);
}

P1ND *
bdty(int op, ...)
{
	struct lexint *li;
	FLT *f2;
	CONSZ c;
	va_list ap;
	int val;
	register P1ND *q;

	va_start(ap, op);
	q = biop(op, NULL, NULL);

	switch (op) {
	case UMUL:
	case UCALL:
		q->n_left = va_arg(ap, P1ND *);
		q->n_rval = 0;
		break;

	case FCON:
		f2 = va_arg(ap, FLT *);
		q->n_scon = sfallo();
		*q->n_scon = f2->sf;
		q->n_type = f2->t;
		break;

	case ICON:
		li = va_arg(ap, struct lexint *);
		slval(q, li->val);
		q->n_type = li->t;
		break;

	case CALL:
		q->n_left = va_arg(ap, P1ND *);
		q->n_right = va_arg(ap, P1ND *);
		break;

	case LB:
		q->n_left = va_arg(ap, P1ND *);
		if ((val = va_arg(ap, int)) <= 0) {
			uerror("array size must be positive");
			val = 1;
		}
		q->n_right = bcon(val);
		break;

	case GOTO: /* for named labels */
		q->n_ap = attr_add(q->n_ap, attr_new(ATTR_P1LABELS, 1));
		/* FALLTHROUGH */
	case NAME:
		q->n_op = NAME;
		q->n_sp = va_arg(ap, struct symtab *); /* XXX survive tymerge */
		break;

	case STRING:
		q->n_type = PTR|CHAR;
		q->n_name = va_arg(ap, char *);
		c = va_arg(ap, TWORD);
		slval(q, c);
		break;

	default:
		cerror("bad bdty");
	}
	va_end(ap);

	return q;
}

static void
flend(void)
{
	struct savbc *sc;

	if (!isinlining && sspflag && blevel == 2)
		sspend();
#ifdef STABS
	if (gflag && blevel > 2)
		stabs_rbrac(blevel);
#endif
	--blevel;
	if( blevel == 1 )
		blevel = 0;
	symclear(blevel); /* Clean ut the symbol table */
	if (autooff > maxautooff)
		maxautooff = autooff;
	autooff = savctx->contlab;
	blkfree();
	stmtfree();
	bkpole = savctx->bkptr;
	cbkp = savctx->bkoff;
	sapole = savctx->stptr;
	cstp = savctx->stoff;
	usdnodes = savctx->numnode;
	sc = savctx->next;
	free(savctx);
	savctx = sc;
}

/*
 * XXX workaround routines for block level cleansing in gcc compat mode.
 * Temporary should be re reserved for this value before.
 */
static P1ND *
p1mcopy(P1ND *p)
{
	P1ND *q;

	q = xmalloc(sizeof(P1ND));
	*q = *p;

	switch (coptype(q->n_op)) {
	case BITYPE:
		q->n_right = p1mcopy(p->n_right);
		/* FALLTHROUGH */
	case UTYPE: 
		q->n_left = p1mcopy(p->n_left);
	}

	return(q);
}

static void
p1mfree(P1ND *p)
{
	int o = coptype(p->n_op);
	if (o == BITYPE)
		p1mfree(p->n_right);
	if (o != LTYPE)
		p1mfree(p->n_left);
	free(p);
}


static P1ND *
gccexpr(int bn, P1ND *q)
{
	P1ND *r, *p, *s;

	branch(bn+4);
	plabel(bn);
	r = buildtree(COMOP, biop(GOTO, bcon(bn+2), NULL), q);
	/* XXX hack to survive flend() */
	s = p1mcopy(r);
	p1tfree(r);
	flend();
	r = p1tcopy(s);
	p1mfree(s);
	q = r->n_right;
	/* XXX end hack */
	if (!(q->n_op == ICON && q->n_type == STRTY) && (r->n_type != VOID)) {
		p = tempnode(0, q->n_type, q->n_df, q->pss);
		r = buildtree(ASSIGN, p1tcopy(p), r);
		r = buildtree(COMOP, r, p);
	}
	return r;
}

static void
savebc(void)
{
	struct savbc *bc = malloc(sizeof(struct savbc));

	bc->brklab = brklab;
	bc->contlab = contlab;
	bc->flostat = flostat;
	bc->next = savbc;
	savbc = bc;
	flostat &= FP_CONTR_CBR;
}

static void
resetbc(int mask)
{
	struct savbc *bc;

	flostat = savbc->flostat | (flostat&mask);
	contlab = savbc->contlab;
	brklab = savbc->brklab;
	bc = savbc->next;
	free(savbc);
	savbc = bc;
}

struct swdef {
	struct swdef *next;	/* Next in list */
	int deflbl;		/* Label for "default" */
	struct swents *ents;	/* Linked sorted list of case entries */
	int nents;		/* # of entries in list */
	int num;		/* Node value will end up in */
	TWORD type;		/* Type of switch expression */
} *swpole;

/*
 * add case to switch
 */
static void
addcase(P1ND *p)
{
	struct swents **put, *w, *sw = malloc(sizeof(struct swents));
	CONSZ val;

	p = optloop(p);  /* change enum to ints */
	if (p->n_op != ICON || p->n_sp != NULL) {
		uerror( "non-constant case expression");
		return;
	}
	if (swpole == NULL) {
		uerror("case not in switch");
		return;
	}

	if (DEUNSIGN(swpole->type) != DEUNSIGN(p->n_type)) {
		val = glval(p);
		p = makety(p, mkqtyp(swpole->type));
		if (p->n_op != ICON)
			cerror("could not cast case value to type of switch "
			       "expression");
		if (glval(p) != val)
			werror("case expression truncated");
	}
	sw->sval = glval(p);
	p1tfree(p);
	put = &swpole->ents;
	if (ISUNSIGNED(swpole->type)) {
		for (w = swpole->ents;
		     w != NULL && (U_CONSZ)w->sval < (U_CONSZ)sw->sval;
		     w = w->next)
			put = &w->next;
	} else {
		for (w = swpole->ents; w != NULL && w->sval < sw->sval;
		     w = w->next)
			put = &w->next;
	}
	if (w != NULL && w->sval == sw->sval) {
		uerror("duplicate case in switch");
		return;
	}
	plabel(sw->slab = getlab());
	*put = sw;
	sw->next = w;
	swpole->nents++;
}

#ifdef GCC_COMPAT
void
gcccase(P1ND *ln, P1ND *hn)
{
	CONSZ i, l, h;

	l = icons(optim(ln));
	h = icons(optim(hn));

	if (h < l)
		i = l, l = h, h = i;

	for (i = l; i <= h; i++)
		addcase(xbcon(i, NULL, hn->n_type));
}
#endif

/*
 * add default case to switch
 */
static void
adddef(void)
{
	if (swpole == NULL)
		uerror("default not inside switch");
	else if (swpole->deflbl != 0)
		uerror("duplicate default in switch");
	else
		plabel( swpole->deflbl = getlab());
}

static void
swstart(int num, TWORD type)
{
	struct swdef *sw = malloc(sizeof(struct swdef));

	sw->deflbl = sw->nents = 0;
	sw->ents = NULL;
	sw->next = swpole;
	sw->num = num;
	sw->type = type;
	swpole = sw;
}

/*
 * end a switch block
 */
static void
swend(void)
{
	struct swents *sw, **swp;
	struct swdef *sp;
	int i;

	sw = FUNALLO(sizeof(struct swents));
	swp = FUNALLO(sizeof(struct swents *) * (swpole->nents+1));

	sw->slab = swpole->deflbl;
	swp[0] = sw;

	for (i = 1; i <= swpole->nents; i++) {
		swp[i] = swpole->ents;
		swpole->ents = swpole->ents->next;
	}
	genswitch(swpole->num, swpole->type, swp, swpole->nents);

	FUNFREE(sw);
	FUNFREE(swp);
	while (swpole->ents) {
		sw = swpole->ents;
		swpole->ents = sw->next;
		free(sw);
	}
	sp = swpole->next;
	free(swpole);
	swpole = sp;
}

/*
 * num: tempnode the value of the switch expression is in
 * type: type of the switch expression
 *
 * p points to an array of structures, each consisting
 * of a constant value and a label.
 * The first is >=0 if there is a default label;
 * its value is the label number
 * The entries p[1] to p[n] are the nontrivial cases
 * n is the number of case statements (length of list)
 */
static void
genswitch(int num, TWORD type, struct swents **p, int n)
{
	P1ND *r, *q;
	int i;

	if (mygenswitch(num, type, p, n))
		return;

	/* simple switch code */
	for (i = 1; i <= n; ++i) {
		/* already in 1 */
		r = tempnode(num, type, 0, 0);
		q = xbcon(p[i]->sval, NULL, type);
		r = buildtree(NE, r, clocal(q));
		xcbranch(r, p[i]->slab);
	}
	if (p[0]->slab > 0)
		branch(p[0]->slab);
}

/*
 * Declare a variable or prototype.
 */
static struct symtab *
init_declarator(P1ND *tn, P1ND *p, int assign, P1ND *a, char *as)
{
	int class = (int)glval(tn);
	struct symtab *sp;

	p = aryfix(p);
	p = tymerge(tn, p);
	if (a) {
		struct attr *ap = gcc_attr_wrapper(a);
		p->n_ap = attr_add(p->n_ap, ap);
	}

	p->n_sp = sp = lookup((char *)p->n_sp, 0); /* XXX */

	if (fun_inline && ISFTN(p->n_type))
		sp->sflags |= SINLINE;

	if (!ISFTN(p->n_type)) {
		if (assign) {
			defid2(p, class, as);
			sp = p->n_sp;
			sp->sflags |= SASG;
			if (sp->sflags & SDYNARRAY)
				uerror("can't initialize dynamic arrays");
			lcommdel(sp);
		} else
			nidcl2(p, class, as);
	} else {
		extern P1ND *parlink;
		if (assign)
			uerror("cannot initialise function");
		defid2(p, uclass(class), as);
		sp = p->n_sp;
		if (sp->sdf->dlst == 0 && !issyshdr)
			warner(Wstrict_prototypes);
		if (parlink) {
			/* dynamic sized arrays in prototypes */
			p1tfree(parlink); /* Free delayed tree */
			parlink = NULL;
		}
	}
	p1tfree(p);
	if (issyshdr)
		sp->sflags |= SINSYS; /* declared in system header */
	return sp;
}

/*
 * Declare old-stype function arguments.
 */
static void
oldargs(P1ND *p)
{
	blevel++;
	p->n_op = TYPE;
	p->n_type = FARG;
	p->n_sp = lookup((char *)p->n_sp, 0);/* XXX */
	defid(p, PARAM);
	blevel--;
}

/*
 * Set NAME nodes to a null name and index of LB nodes to NOOFFSET
 * unless clr is one, in that case preserve variable name.
 */
static P1ND *
namekill(P1ND *p, int clr)
{
	P1ND *q;
	int o = p->n_op;

	switch (coptype(o)) {
	case LTYPE:
		if (o == NAME) {
			if (clr)
				p->n_sp = NULL;
			else
				p->n_sp = lookup((char *)p->n_sp, 0);/* XXX */
		}
		break;

	case UTYPE:
		p->n_left = namekill(p->n_left, clr);
		break;

        case BITYPE:
                p->n_left = namekill(p->n_left, clr);
		if (o == LB) {
			if (clr) {
				p1tfree(p->n_right);
				p->n_right = bcon(NOOFFSET);
			} else
				p->n_right = eve(p->n_right);
		} else if (o == CALL)
			p->n_right = namekill(p->n_right, 1);
		else
			p->n_right = namekill(p->n_right, clr);
		if (o == TYMERGE) {
			q = tymerge(p->n_left, p->n_right);
			q->n_ap = attr_add(q->n_ap, p->n_ap);
			p1tfree(p->n_left);
			p1nfree(p);
			p = q;
		}
		break;
	}
	return p;
}

/*
 * Declare function arguments.
 */
static P1ND *
funargs(P1ND *p)
{
	extern P1ND *arrstk[10];

	if (p->n_op == ELLIPSIS)
		return p;

	p = namekill(p, 0);
	if (ISFTN(p->n_type))
		p->n_type = INCREF(p->n_type);
	if (ISARY(p->n_type)) {
		p->n_type += (PTR-ARY);
		if (p->n_df->ddim == -1)
			p1tfree(arrstk[0]), arrstk[0] = NULL;
		p->n_df++;
	}
	if (p->n_type == VOID && p->n_sp->sname == NULL)
		return p; /* sanitycheck later */
	else if (p->n_sp->sname == NULL)
		uerror("argument missing");
	else
		defid(p, PARAM);
	return p;
}

static P1ND *
listfw(P1ND *p, P1ND * (*f)(P1ND *))
{
        if (p->n_op == CM) {
                p->n_left = listfw(p->n_left, f);
                p->n_right = (*f)(p->n_right);
        } else
                p = (*f)(p);
	return p;
}


/*
 * Declare a function.
 */
static void
fundef(P1ND *tp, P1ND *p)
{
	extern int prolab;
	struct symtab *s;
	P1ND *q, *typ;
	int class = (int)glval(tp), oclass, ctval;

	/*
	 * We discard all names except for those needed for
	 * parameter declaration. While doing that, also change
	 * non-constant array sizes to unknown.
	 */
	ctval = tvaloff;
	for (q = p; coptype(q->n_op) != LTYPE &&
	    q->n_left->n_op != NAME; q = q->n_left) {
		if (q->n_op == CALL)
			q->n_right = namekill(q->n_right, 1);
	}
	if (q->n_op != CALL && q->n_op != UCALL) {
		uerror("invalid function definition");
		p = bdty(UCALL, p);
	} else if (q->n_op == CALL) {
		blevel = 1;
		argoff = ARGINIT;
		if (oldstyle == 0)
			q->n_right = listfw(q->n_right, funargs);
		p1listf(q->n_right, argsave);
		blevel = 0;
	}

	p = typ = tymerge(tp, p);
#ifdef GCC_COMPAT
	/* gcc seems to discard __builtin_ when declaring functions */
	if (strncmp("__builtin_", (char *)typ->n_sp, 10) == 0)
		typ->n_sp = (struct symtab *)((char *)typ->n_sp + 10);
#endif
	s = typ->n_sp = lookup((char *)typ->n_sp, 0); /* XXX */

	oclass = s->sclass;
	if (class == STATIC && oclass == EXTERN)
		werror("%s was first declared extern, then static", s->sname);

	if (fun_inline) {
		/* special syntax for inline functions */
		if (! strcmp(s->sname,"main")) 
			uerror("cannot inline main()");

		s->sflags |= SINLINE;
		inline_start(s, class);
		if (class == EXTERN)
			class = EXTDEF;
	} else if (class == EXTERN)
		class = SNULL; /* same result */

	cftnsp = s;
	defid(p, class);
	if (s->sdf->dlst == 0 && !issyshdr)
		warner(Wstrict_prototypes);
#ifdef GCC_COMPAT
	if (attr_find(p->n_ap, GCC_ATYP_ALW_INL)) {
		/* Temporary turn on temps to make always_inline work */
		alwinl = 1;
		if (xtemps == 0) alwinl |= 2;
		xtemps = 1;
	}
#endif
	prolab = getlab();
	send_passt(IP_PROLOG, -1, getexname(cftnsp), cftnsp->stype,
	    cftnsp->sclass == EXTDEF, prolab, ctval);
	blevel++;
#ifdef STABS
	if (gflag)
		stabs_func(s);
#endif
	p1tfree(tp);
	p1tfree(p);

}

static void
fend(void)
{
	if (blevel)
		cerror("function level error");
	ftnend();
	fun_inline = 0;
	if (alwinl & 2) xtemps = 0;
	alwinl = 0;
	cftnsp = NULL;
}

P1ND *
structref(P1ND *p, int f, char *name)
{
	P1ND *r;

	if (f == DOT)
		p = buildtree(ADDROF, p, NULL);
	r = biop(NAME, NULL, NULL);
	r->n_name = name;
	r = buildtree(STREF, p, r);
	return r;
}

static void
olddecl(P1ND *p, P1ND *a)
{
	struct symtab *s;

	p = namekill(p, 0);
	s = p->n_sp;
	if (s->slevel != 1 || s->stype == UNDEF)
		uerror("parameter '%s' not defined", s->sname);
	else if (s->stype != FARG)
		uerror("parameter '%s' redefined", s->sname);

	s->stype = p->n_type;
	s->sdf = p->n_df;
	s->sap = p->n_ap;
	if (a)
		attr_add(s->sap, gcc_attr_wrapper(a));
	p1nfree(p);
}

void
branch(int lbl)
{
	int r = reached++;
	ecomp(biop(GOTO, bcon(lbl), NULL));
	reached = r;
}

/*
 * Create a printable string based on an encoded string.
 */
static char *
mkpstr(char *str)
{
	char *os, *s;
	size_t l = strlen(str) + 3; /* \t + \n + \0 */

	os = s = stmtalloc(l);
	*s++ = '\t';
	while (*str) {
		if (*str == '\\')
			*s++ = esccon(&str);
		else
			*s++ = *str++;
	}
	*s++ = '\n';
	*s = 0;

	return os;
}

/*
 * Fake a symtab entry for compound literals.
 */
static struct initctx *
clbrace(P1ND *p)
{
	struct initctx *ctx;
	struct symtab *sp;

	sp = getsymtab(simname("cl"), STEMP);
	sp->stype = p->n_type;
	sp->squal = p->n_qual;
	sp->sdf = p->n_df;
	sp->sss = p->pss;
	sp->sap = p->n_ap;
	p1tfree(p);
	if (blevel == 0 && xnf != NULL) {
		sp->sclass = STATIC;
		sp->slevel = 2;
		sp->soffset = getlab();
	} else {
		sp->sclass = blevel ? AUTO : STATIC;
		if (!ISARY(sp->stype) || sp->sdf->ddim != NOOFFSET) {
			sp->soffset = NOOFFSET;
			oalloc(sp, &autooff);
		}
	}
	ctx = beginit(sp);
	return ctx;
}

char *
simname(char *s)
{
	size_t len = strlen(s) + 10 + 1;
	char *w = tmpalloc(len); /* uncommon */

	snprintf(w, len, "%s%d", s, getlab());
	return w;
}

P1ND *
biop(int op, P1ND *l, P1ND *r)
{
	return block(op, l, r, INT, 0, 0);
}

static P1ND *
cmop(P1ND *l, P1ND *r)
{
	return biop(CM, l, r);
}

static P1ND *
voidcon(void)
{
	return block(ICON, NULL, NULL, STRTY, 0, 0);
}

/* Support for extended assembler a' la' gcc style follows below */

static P1ND *
xmrg(P1ND *out, P1ND *in)
{
	P1ND *p = in;

	if (p->n_op == XARG) {
		in = cmop(out, p);
	} else {
		while (p->n_left->n_op == CM)
			p = p->n_left;
		p->n_left = cmop(out, p->n_left);
	}
	return in;
}

/*
 * Put together in and out node lists in one list, and balance it with
 * the constraints on the right side of a CM node.
 */
static P1ND *
xcmop(P1ND *out, P1ND *in, P1ND *str)
{
	P1ND *p, *q;

	if (out) {
		/* D out-list sanity check */
		for (p = out; p->n_op == CM; p = p->n_left) {
			q = p->n_right;
			if (q->n_name[0] != '=' && q->n_name[0] != '+')
				uerror("output missing =");
		}
		if (p->n_name[0] != '=' && p->n_name[0] != '+')
			uerror("output missing =");
		if (in == NULL)
			p = out;
		else
			p = xmrg(out, in);
	} else if (in) {
		p = in;
	} else
		p = voidcon();

	if (str == NULL)
		str = voidcon();
	return cmop(p, str);
}

/*
 * Generate a XARG node based on a string and an expression.
 */
static P1ND *
xasmop(char *str, P1ND *p)
{

	p = biop(XARG, p, NULL);
	p->n_name = str;
	return p;
}

/*
 * Generate a XASM node based on a string and an expression.
 */
static void
mkxasm(char *str, P1ND *p)
{
	P1ND *q;

	q = biop(XASM, p->n_left, p->n_right);
	q->n_name = str;
	p1nfree(p);
	ecomp(optloop(q));
}

static struct attr *
gcc_attr_wrapper(P1ND *p)
{
#ifdef GCC_COMPAT
	return gcc_attr_parse(p);
#else
	if (p != NULL)
		uerror("gcc attribute used");
	return NULL;
#endif
}

#ifdef GCC_COMPAT
static P1ND *
tyof(P1ND *p)
{
	static struct symtab spp;
	P1ND *q = block(TYPE, NULL, NULL, p->n_type, p->n_df, p->pss);
	q->n_qual = p->n_qual;
	q->n_sp = &spp; /* for typenode */
	p1walkf(p, putjops, 0);
	p1tfree(p);
	return q;
}

#else
static P1ND *
tyof(P1ND *p)
{
	uerror("typeof gcc extension");
	return bcon(0);
}
#endif

/*
 * Traverse an unhandled expression tree bottom-up and call buildtree()
 * or equivalent as needed.
 */
P1ND *
eve(P1ND *p)
{
	struct symtab *sp;
	P1ND *r, *p1, *p2;
	int x;

	p1 = p->n_left;
	p2 = p->n_right;
	switch (p->n_op) {
	case NAME:
		sp = lookup((char *)p->n_sp,
		    attr_find(p->n_ap, ATTR_P1LABELS) ? SLBLNAME|STEMP : 0);
		if (sp->sflags & SINLINE)
			inline_ref(sp);
		r = nametree(sp);
		if (sp->sflags & SDYNARRAY)
			r = buildtree(UMUL, r, NULL);
#ifdef GCC_COMPAT
		if (attr_find(sp->sap, GCC_ATYP_DEPRECATED))
			warner(Wdeprecated_declarations, sp->sname);
#endif
		break;

	case DOT:
	case STREF:
		r = structref(eve(p1), p->n_op, (char *)p2->n_sp);
		p1nfree(p2);
		break;

	case CAST:
		p2 = eve(p2);
#ifndef NO_COMPLEX
		if (ANYCX(p1) || ANYCX(p2)) {
			r = cxcast(p1, p2);
			break;
		}
#endif
#ifdef TARGET_TIMODE
		if ((r = gcc_eval_ticast(CAST, p1, p2)) != NULL)
			break;
#endif
		p1 = buildtree(CAST, p1, p2);
		p1nfree(p1->n_left);
		r = p1->n_right;
		p1nfree(p1);
		break;


	case SZOF:
		x = xinline; xinline = 0; /* XXX hack */
		if (glval(p2) == 0)
			p1 = eve(p1);
		else
			TYMFIX(p1);
		p1nfree(p2);
		r = doszof(p1);
		xinline = x;
		break;

	case LB:
		p1 = eve(p1);
		p2 = eve(p2);
#ifdef TARGET_TIMODE
		if (isti(p2)) {
			P1ND *s = block(NAME, NULL, NULL, LONG, 0, 0);
			if ((r = gcc_eval_ticast(CAST, s, p2)) != NULL)
				p2 = r;
			p1nfree(s);
		}
#endif
		r = buildtree(UMUL, buildtree(PLUS, p1, p2), NULL);
		break;

	case COMPL:
#ifndef NO_COMPLEX
		p1 = eve(p1);
		if (ANYCX(p1))
			r = cxconj(p1);
		else
			r = buildtree(COMPL, p1, NULL);
		break;
#endif
	case UPLUS:
		r = eve(p1);
		if (r->n_op == FLD || r->n_type < INT)
			r = buildtree(PLUS, r, bcon(0)); /* must be size int */
		break;

	case UMINUS:
#ifndef NO_COMPLEX
		p1 = eve(p1);
		if (ANYCX(p1))
			r = cxop(UMINUS, p1, p1);
		else
			r = buildtree(UMINUS, p1, NULL);
		break;
#endif
	case NOT:
	case UMUL:
		p1 = eve(p1);
#ifdef TARGET_TIMODE
		if ((r = gcc_eval_tiuni(p->n_op, p1)) != NULL)
			break;
#endif
#ifndef NO_COMPLEX
		if (p->n_op == NOT && ANYCX(p1))
			p1 = cxop(NE, p1, bcon(0));
#endif
		r = buildtree(p->n_op, p1, NULL);
		break;

	case ADDROF:
		r = eve(p1);
		if (ISFTN(p->n_type)/* || ISARY(p->n_type) */){
#ifdef notdef
			werror( "& before array or function: ignored" );
#endif
		} else
			r = buildtree(ADDROF, r, NULL);
		break;

	case UCALL:
		p2 = NULL;
		/* FALLTHROUGH */
	case CALL:
		if (p1->n_op == NAME) {
			sp = lookup((char *)p1->n_sp, 0);
#ifndef NO_C_BUILTINS
			if (sp->sflags & SBUILTIN) {
				p1nfree(p1);
				r = builtin_check(sp, p2);
				break;
			}
#endif
			if (sp->stype == UNDEF) {
				p1->n_type = FTN|INT;
				p1->n_sp = sp;
				p1->n_ap = NULL;
				defid(p1, EXTERN);
			}
			p1nfree(p1);
#ifdef GCC_COMPAT
			if (attr_find(sp->sap, GCC_ATYP_DEPRECATED))
				warner(Wdeprecated_declarations, sp->sname);
#endif
			if (p->n_op == CALL)
				p2 = eve(p2);
			r = doacall(sp, nametree(sp), p2);
		} else {
			if (p->n_op == CALL)
				p2 = eve(p2);
			r = doacall(NULL, eve(p1), p2);
		}
		break;

#ifndef NO_COMPLEX
	case XREAL:
	case XIMAG:
		p1 = eve(p1);
		r = cxelem(p->n_op, p1);
		break;
#endif

	case COLON:
	case MUL:
	case DIV:
	case PLUS:
	case MINUS:
	case ASSIGN:
	case EQ:
	case NE:
	case OROR:
	case ANDAND:
#ifndef NO_COMPLEX
		p1 = eve(p1);
		p2 = eve(p2);
#ifdef TARGET_TIMODE
		if ((r = gcc_eval_timode(p->n_op, p1, p2)) != NULL)
			break;
#endif
		if (ANYCX(p1) || ANYCX(p2)) {
			r = cxop(p->n_op, p1, p2);
		} else if (ISITY(p1->n_type) || ISITY(p2->n_type)) {
			r = imop(p->n_op, p1, p2);
		} else
			r = buildtree(p->n_op, p1, p2);
		break;
#endif
	case C_GT:
	case C_LE:
	case MOD:
	case CM:
	case GT:
	case GE:
	case LT:
	case LE:
	case RS:
	case LS:
	case RSEQ:
	case LSEQ:
	case AND:
	case OR:
	case ER:
	case EREQ:
	case OREQ:
	case ANDEQ:
	case QUEST:
		p1 = eve(p1);
		p2 = eve(p2);
		if (p->n_op == C_GT || p->n_op == C_LE) {
			r = p1, p1 = p2, p2 = r;
			p->n_op = p->n_op == C_GT ? LT : GE;
		}
#ifdef TARGET_TIMODE
		if ((r = gcc_eval_timode(p->n_op, p1, p2)) != NULL)
			break;
#endif
		r = buildtree(p->n_op, p1, p2);
		break;

	case BIQUEST: /* gcc e ?: e op */
		p1 = eve(p1);
		r = tempnode(0, p1->n_type, p1->n_df, p1->pss);
		p2 = eve(biop(COLON, p1tcopy(r), p2));
		r = buildtree(QUEST, buildtree(ASSIGN, r, p1), p2);
		break;

	case INCR:
	case DECR:
	case MODEQ:
	case MINUSEQ:
	case PLUSEQ:
	case MULEQ:
	case DIVEQ:
		p1 = eve(p1);
		p2 = eve(p2);
#ifdef TARGET_TIMODE
		if ((r = gcc_eval_timode(p->n_op, p1, p2)) != NULL)
			break;
#endif
#ifndef NO_COMPLEX
		if (ANYCX(p1) || ANYCX(p2)) {
			r = cxop(UNASG p->n_op, p1tcopy(p1), p2);
			r = cxop(ASSIGN, p1, r);
			break;
		} else if (ISITY(p1->n_type) || ISITY(p2->n_type)) {
			r = imop(UNASG p->n_op, p1tcopy(p1), p2);
			r = cxop(ASSIGN, p1, r);
			break;
		}
		/* FALLTHROUGH */
#endif
		r = buildtree(p->n_op, p1, p2);
		break;

	case STRING:
		r = strend(p->n_name, (TWORD)glval(p));
		break;

	case COMOP:
		if (p1->n_op == GOTO) {
			/* inside ({ }), eve already called */
			r = buildtree(p->n_op, p1, p2);
		} else {
			p1 = eve(p1);
			r = buildtree(p->n_op, p1, eve(p2));
		}
		break;

	case TYPE:
	case ICON:
	case FCON:
	case TEMP:
		return p;

	case CLOP:
		r = nametree(p->n_sp);
		break;

	default:
#ifdef PCC_DEBUG
		p1fwalk(p, eprint, 0);
#endif
		cerror("eve");
		r = NULL;
	}
	p1nfree(p);
	return r;
}

int
con_e(P1ND *p)
{
	return (int)icons(optloop(eve(p)));
}

void
uawarn(P1ND *p, char *s)
{
	if (p == 0)
		return;
	if (attrwarn)
		werror("unhandled %s attribute", s);
	p1tfree(p);
}

static void
dainit(struct initctx *ctx, P1ND *d, P1ND *a)
{
	if (d == NULL) {
		asginit(ctx, a);
	} else if (d->n_op == CM) {
		int is = con_e(d->n_left);
		int ie = con_e(d->n_right);
		int i;

		p1nfree(d);
		if (ie < is)
			uerror("negative initializer range");
		desinit(ctx, biop(LB, NULL, bcon(is)));
		for (i = is; i < ie; i++)
			asginit(ctx, p1tcopy(a));
		asginit(ctx, a);
	} else {
		cerror("dainit");
	}
}

/*
 * Traverse down and tymerge() where appropriate.
 */
static P1ND *
tymfix(P1ND *p)
{
	P1ND *q;
	int o = coptype(p->n_op);

	switch (o) {
	case LTYPE:
		break;
	case UTYPE:
		p->n_left = tymfix(p->n_left);
		break;
	case BITYPE:
		p->n_left = tymfix(p->n_left);
		p->n_right = tymfix(p->n_right);
		if (p->n_op == TYMERGE) {
			q = tymerge(p->n_left, p->n_right);
			q->n_ap = attr_add(q->n_ap, p->n_ap);
			p1tfree(p->n_left);
			p1nfree(p);
			p = q;
		}
		break;
	}
	return p;
}

static P1ND *
aryfix(P1ND *p)
{
	P1ND *q;

	for (q = p; q->n_op != NAME; q = q->n_left) {
		if (q->n_op == LB) {
			q->n_right = optloop(eve(q->n_right));
			if ((blevel == 0 || rpole != NULL) &&
			    !nncon(q->n_right))
				uerror("array size not constant"); 
			/*
			 * Checks according to 6.7.5.2 clause 1:
			 * "...the expression shall have an integer type."
			 * "If the expression is a constant expression,	 
			 * it shall have a value greater than zero."
			 */
			if (!ISINTEGER(q->n_right->n_type))
				werror("array size is not an integer");
			else if (q->n_right->n_op == ICON &&
			    glval(q->n_right) < 0 &&
			    glval(q->n_right) != NOOFFSET) {
					uerror("array size cannot be negative");
					slval(q->n_right, 1);
			}
		} else if (q->n_op == CALL)
			q->n_right = namekill(q->n_right, 1);
	}
	return p;
}

struct labs {
	struct labs *next;
	int lab;
} *labp;

static void
savlab(int lab)
{
	struct labs *l = tmpalloc(sizeof(struct labs)); /* uncommon */
	l->lab = lab < 0 ? -lab : lab;
	l->next = labp;
	labp = l;
}

int *
mkclabs(void)
{
	struct labs *l;
	int i, *rv;

	for (i = 0, l = labp; l; l = l->next, i++)
		;
	rv = tmpalloc((i+1)*sizeof(int));	/* uncommon */
	for (i = 0, l = labp; l; l = l->next, i++)
		rv[i] = l->lab;
	rv[i] = 0;
	labp = 0;
	return rv;
}

void
xcbranch(P1ND *p, int lab)
{
#ifndef NO_COMPLEX
	if (ANYCX(p))
		p = cxop(NE, p, bcon(0));
#endif
	cbranch(buildtree(NOT, p, NULL), bcon(lab));
}

/*
 * New a case entry to genlist.
 * tn is type, e is expression.
 */
static struct genlist *
newgen(P1ND *tn, P1ND *e)
{
	struct genlist *ng;
	TWORD t;

	if (tn) {
		t = tn->n_type;
		p1tfree(tn);
	} else
		t = 0;

	/* add new entry */
	ng = malloc(sizeof(struct genlist));
	ng->next = NULL;
	ng->t = t;
	ng->p = e;
	return ng;
}

/*
 * Add a case entry to genlist.
 * g is list, ng is new entry.
 */
static struct genlist *
addgen(struct genlist *g, struct genlist *ng)
{
	struct genlist *w;

	/* search for duplicate type */
	for (w = g; w; w = w->next) {
		if (w->t == ng->t)
			uerror("duplicate type in _Generic");
	}
	ng->next = g;
	return ng;
}

static P1ND *
dogen(struct genlist *g, P1ND *e)
{
	struct genlist *ng;
	P1ND *w, *p;

	e = eve(e);

	/* search for direct match */
	for (ng = g, w = p = NULL; ng; ng = ng->next) {
		if (ng->t == 0)
			p = ng->p; /* save default */
		if (e->n_type == ng->t)
			w = ng->p;
	}

	/* if no match, use generic */
	if (w == NULL) {
		if (p == NULL) {
			uerror("_Generic: no default found");
			p = bcon(0);
		}
		w = p;
	}

	/* free tree */
	while (g) {
		if (g->p != w)
			p1tfree(g->p);
		ng = g->next;
		free(g);
		g = ng;
	}

	p1tfree(e);
	return w;
}
