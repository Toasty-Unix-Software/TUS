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
#line 1 "ld_script_parser.y"

/*-
 * Copyright (c) 2010-2013 Kai Wang
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "ld.h"
#include "ld_arch.h"
#include "ld_options.h"
#include "ld_output.h"
#include "ld_script.h"
#include "ld_file.h"
#include "ld_path.h"
#include "ld_exp.h"

ELFTC_VCSID("$Id$");

struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;

#ifndef	YY_BUF_SIZE
#define	YY_BUF_SIZE 16384
#endif

extern int yylex(void);
extern int yyparse(void);
extern YY_BUFFER_STATE yy_create_buffer(FILE *file, int size);
extern YY_BUFFER_STATE yy_scan_string(char *yy_str);
extern void yy_switch_to_buffer(YY_BUFFER_STATE b);
extern void yy_delete_buffer(YY_BUFFER_STATE b);
extern int lineno;
extern FILE *yyin;
extern struct ld *ld;

static void yyerror(const char *s);
static void _init_script(void);
static struct ld_script_cmd_head ldss_c, ldso_c;


#line 132 "ld_script_parser.c"

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

#include "ld_script_parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_ABSOLUTE = 3,                 /* T_ABSOLUTE  */
  YYSYMBOL_T_ADDR = 4,                     /* T_ADDR  */
  YYSYMBOL_T_ALIGN = 5,                    /* T_ALIGN  */
  YYSYMBOL_T_ALIGNOF = 6,                  /* T_ALIGNOF  */
  YYSYMBOL_T_ASSERT = 7,                   /* T_ASSERT  */
  YYSYMBOL_T_AS_NEEDED = 8,                /* T_AS_NEEDED  */
  YYSYMBOL_T_AT = 9,                       /* T_AT  */
  YYSYMBOL_T_BIND = 10,                    /* T_BIND  */
  YYSYMBOL_T_BLOCK = 11,                   /* T_BLOCK  */
  YYSYMBOL_T_BYTE = 12,                    /* T_BYTE  */
  YYSYMBOL_T_CONSTANT = 13,                /* T_CONSTANT  */
  YYSYMBOL_T_CONSTRUCTORS = 14,            /* T_CONSTRUCTORS  */
  YYSYMBOL_T_CREATE_OBJECT_SYMBOLS = 15,   /* T_CREATE_OBJECT_SYMBOLS  */
  YYSYMBOL_T_DATA_SEGMENT_ALIGN = 16,      /* T_DATA_SEGMENT_ALIGN  */
  YYSYMBOL_T_DATA_SEGMENT_END = 17,        /* T_DATA_SEGMENT_END  */
  YYSYMBOL_T_DATA_SEGMENT_RELRO_END = 18,  /* T_DATA_SEGMENT_RELRO_END  */
  YYSYMBOL_T_DEFINED = 19,                 /* T_DEFINED  */
  YYSYMBOL_T_ENTRY = 20,                   /* T_ENTRY  */
  YYSYMBOL_T_EXCLUDE_FILE = 21,            /* T_EXCLUDE_FILE  */
  YYSYMBOL_T_EXTERN = 22,                  /* T_EXTERN  */
  YYSYMBOL_T_FILEHDR = 23,                 /* T_FILEHDR  */
  YYSYMBOL_T_FILL = 24,                    /* T_FILL  */
  YYSYMBOL_T_FLAGS = 25,                   /* T_FLAGS  */
  YYSYMBOL_T_FLOAT = 26,                   /* T_FLOAT  */
  YYSYMBOL_T_FORCE_COMMON_ALLOCATION = 27, /* T_FORCE_COMMON_ALLOCATION  */
  YYSYMBOL_T_GROUP = 28,                   /* T_GROUP  */
  YYSYMBOL_T_HLL = 29,                     /* T_HLL  */
  YYSYMBOL_T_INCLUDE = 30,                 /* T_INCLUDE  */
  YYSYMBOL_T_INHIBIT_COMMON_ALLOCATION = 31, /* T_INHIBIT_COMMON_ALLOCATION  */
  YYSYMBOL_T_INPUT = 32,                   /* T_INPUT  */
  YYSYMBOL_T_KEEP = 33,                    /* T_KEEP  */
  YYSYMBOL_T_LENGTH = 34,                  /* T_LENGTH  */
  YYSYMBOL_T_LOADADDR = 35,                /* T_LOADADDR  */
  YYSYMBOL_T_LONG = 36,                    /* T_LONG  */
  YYSYMBOL_T_MAP = 37,                     /* T_MAP  */
  YYSYMBOL_T_MAX = 38,                     /* T_MAX  */
  YYSYMBOL_T_MEMORY = 39,                  /* T_MEMORY  */
  YYSYMBOL_T_MIN = 40,                     /* T_MIN  */
  YYSYMBOL_T_NEXT = 41,                    /* T_NEXT  */
  YYSYMBOL_T_NOCROSSREFS = 42,             /* T_NOCROSSREFS  */
  YYSYMBOL_T_NOFLOAT = 43,                 /* T_NOFLOAT  */
  YYSYMBOL_T_OPTION = 44,                  /* T_OPTION  */
  YYSYMBOL_T_ORIGIN = 45,                  /* T_ORIGIN  */
  YYSYMBOL_T_OUTPUT = 46,                  /* T_OUTPUT  */
  YYSYMBOL_T_OUTPUT_ARCH = 47,             /* T_OUTPUT_ARCH  */
  YYSYMBOL_T_OUTPUT_FORMAT = 48,           /* T_OUTPUT_FORMAT  */
  YYSYMBOL_T_PHDRS = 49,                   /* T_PHDRS  */
  YYSYMBOL_T_PROVIDE = 50,                 /* T_PROVIDE  */
  YYSYMBOL_T_PROVIDE_HIDDEN = 51,          /* T_PROVIDE_HIDDEN  */
  YYSYMBOL_T_QUAD = 52,                    /* T_QUAD  */
  YYSYMBOL_T_REGION_ALIAS = 53,            /* T_REGION_ALIAS  */
  YYSYMBOL_T_SEARCH_DIR = 54,              /* T_SEARCH_DIR  */
  YYSYMBOL_T_SECTIONS = 55,                /* T_SECTIONS  */
  YYSYMBOL_T_SEGMENT_START = 56,           /* T_SEGMENT_START  */
  YYSYMBOL_T_SHORT = 57,                   /* T_SHORT  */
  YYSYMBOL_T_SIZEOF = 58,                  /* T_SIZEOF  */
  YYSYMBOL_T_SIZEOF_HEADERS = 59,          /* T_SIZEOF_HEADERS  */
  YYSYMBOL_T_SORT_BY_ALIGNMENT = 60,       /* T_SORT_BY_ALIGNMENT  */
  YYSYMBOL_T_SORT_BY_NAME = 61,            /* T_SORT_BY_NAME  */
  YYSYMBOL_T_SPECIAL = 62,                 /* T_SPECIAL  */
  YYSYMBOL_T_SQUAD = 63,                   /* T_SQUAD  */
  YYSYMBOL_T_STARTUP = 64,                 /* T_STARTUP  */
  YYSYMBOL_T_SUBALIGN = 65,                /* T_SUBALIGN  */
  YYSYMBOL_T_SYSLIB = 66,                  /* T_SYSLIB  */
  YYSYMBOL_T_TARGET = 67,                  /* T_TARGET  */
  YYSYMBOL_T_TRUNCATE = 68,                /* T_TRUNCATE  */
  YYSYMBOL_T_VER_EXTERN = 69,              /* T_VER_EXTERN  */
  YYSYMBOL_T_VER_GLOBAL = 70,              /* T_VER_GLOBAL  */
  YYSYMBOL_T_VER_LOCAL = 71,               /* T_VER_LOCAL  */
  YYSYMBOL_T_LSHIFT_E = 72,                /* T_LSHIFT_E  */
  YYSYMBOL_T_RSHIFT_E = 73,                /* T_RSHIFT_E  */
  YYSYMBOL_T_LSHIFT = 74,                  /* T_LSHIFT  */
  YYSYMBOL_T_RSHIFT = 75,                  /* T_RSHIFT  */
  YYSYMBOL_T_EQ = 76,                      /* T_EQ  */
  YYSYMBOL_T_NE = 77,                      /* T_NE  */
  YYSYMBOL_T_GE = 78,                      /* T_GE  */
  YYSYMBOL_T_LE = 79,                      /* T_LE  */
  YYSYMBOL_T_ADD_E = 80,                   /* T_ADD_E  */
  YYSYMBOL_T_SUB_E = 81,                   /* T_SUB_E  */
  YYSYMBOL_T_MUL_E = 82,                   /* T_MUL_E  */
  YYSYMBOL_T_DIV_E = 83,                   /* T_DIV_E  */
  YYSYMBOL_T_AND_E = 84,                   /* T_AND_E  */
  YYSYMBOL_T_OR_E = 85,                    /* T_OR_E  */
  YYSYMBOL_T_LOGICAL_AND = 86,             /* T_LOGICAL_AND  */
  YYSYMBOL_T_LOGICAL_OR = 87,              /* T_LOGICAL_OR  */
  YYSYMBOL_88_ = 88,                       /* '='  */
  YYSYMBOL_89_ = 89,                       /* '?'  */
  YYSYMBOL_90_ = 90,                       /* ':'  */
  YYSYMBOL_91_ = 91,                       /* '|'  */
  YYSYMBOL_92_ = 92,                       /* '&'  */
  YYSYMBOL_93_ = 93,                       /* '>'  */
  YYSYMBOL_94_ = 94,                       /* '<'  */
  YYSYMBOL_95_ = 95,                       /* '+'  */
  YYSYMBOL_96_ = 96,                       /* '-'  */
  YYSYMBOL_97_ = 97,                       /* '*'  */
  YYSYMBOL_98_ = 98,                       /* '/'  */
  YYSYMBOL_99_ = 99,                       /* '%'  */
  YYSYMBOL_UNARY = 100,                    /* UNARY  */
  YYSYMBOL_T_NUM = 101,                    /* T_NUM  */
  YYSYMBOL_T_COMMONPAGESIZE = 102,         /* T_COMMONPAGESIZE  */
  YYSYMBOL_T_COPY = 103,                   /* T_COPY  */
  YYSYMBOL_T_DSECT = 104,                  /* T_DSECT  */
  YYSYMBOL_T_IDENT = 105,                  /* T_IDENT  */
  YYSYMBOL_T_INFO = 106,                   /* T_INFO  */
  YYSYMBOL_T_MAXPAGESIZE = 107,            /* T_MAXPAGESIZE  */
  YYSYMBOL_T_MEMORY_ATTR = 108,            /* T_MEMORY_ATTR  */
  YYSYMBOL_T_NOLOAD = 109,                 /* T_NOLOAD  */
  YYSYMBOL_T_ONLY_IF_RO = 110,             /* T_ONLY_IF_RO  */
  YYSYMBOL_T_ONLY_IF_RW = 111,             /* T_ONLY_IF_RW  */
  YYSYMBOL_T_OVERLAY = 112,                /* T_OVERLAY  */
  YYSYMBOL_T_STRING = 113,                 /* T_STRING  */
  YYSYMBOL_T_WILDCARD = 114,               /* T_WILDCARD  */
  YYSYMBOL_115_ = 115,                     /* '!'  */
  YYSYMBOL_116_ = 116,                     /* '~'  */
  YYSYMBOL_117_ = 117,                     /* '('  */
  YYSYMBOL_118_ = 118,                     /* ')'  */
  YYSYMBOL_119_ = 119,                     /* ','  */
  YYSYMBOL_120_ = 120,                     /* ';'  */
  YYSYMBOL_121_ = 121,                     /* '{'  */
  YYSYMBOL_122_ = 122,                     /* '}'  */
  YYSYMBOL_123_ = 123,                     /* '.'  */
  YYSYMBOL_YYACCEPT = 124,                 /* $accept  */
  YYSYMBOL_script = 125,                   /* script  */
  YYSYMBOL_ldscript = 126,                 /* ldscript  */
  YYSYMBOL_expression = 127,               /* expression  */
  YYSYMBOL_function = 128,                 /* function  */
  YYSYMBOL_absolute_function = 129,        /* absolute_function  */
  YYSYMBOL_addr_function = 130,            /* addr_function  */
  YYSYMBOL_align_function = 131,           /* align_function  */
  YYSYMBOL_alignof_function = 132,         /* alignof_function  */
  YYSYMBOL_block_function = 133,           /* block_function  */
  YYSYMBOL_data_segment_align_function = 134, /* data_segment_align_function  */
  YYSYMBOL_data_segment_end_function = 135, /* data_segment_end_function  */
  YYSYMBOL_data_segment_relro_end_function = 136, /* data_segment_relro_end_function  */
  YYSYMBOL_defined_function = 137,         /* defined_function  */
  YYSYMBOL_length_function = 138,          /* length_function  */
  YYSYMBOL_loadaddr_function = 139,        /* loadaddr_function  */
  YYSYMBOL_max_function = 140,             /* max_function  */
  YYSYMBOL_min_function = 141,             /* min_function  */
  YYSYMBOL_next_function = 142,            /* next_function  */
  YYSYMBOL_origin_function = 143,          /* origin_function  */
  YYSYMBOL_segment_start_function = 144,   /* segment_start_function  */
  YYSYMBOL_sizeof_function = 145,          /* sizeof_function  */
  YYSYMBOL_sizeof_headers_function = 146,  /* sizeof_headers_function  */
  YYSYMBOL_constant = 147,                 /* constant  */
  YYSYMBOL_symbolic_constant = 148,        /* symbolic_constant  */
  YYSYMBOL_ldscript_command = 149,         /* ldscript_command  */
  YYSYMBOL_assignment = 150,               /* assignment  */
  YYSYMBOL_simple_assignment = 151,        /* simple_assignment  */
  YYSYMBOL_provide_assignment = 152,       /* provide_assignment  */
  YYSYMBOL_provide_hidden_assignment = 153, /* provide_hidden_assignment  */
  YYSYMBOL_assign_op = 154,                /* assign_op  */
  YYSYMBOL_assert_command = 155,           /* assert_command  */
  YYSYMBOL_entry_command = 156,            /* entry_command  */
  YYSYMBOL_extern_command = 157,           /* extern_command  */
  YYSYMBOL_force_common_allocation_command = 158, /* force_common_allocation_command  */
  YYSYMBOL_group_command = 159,            /* group_command  */
  YYSYMBOL_inhibit_common_allocation_command = 160, /* inhibit_common_allocation_command  */
  YYSYMBOL_input_command = 161,            /* input_command  */
  YYSYMBOL_memory_command = 162,           /* memory_command  */
  YYSYMBOL_memory_region_list = 163,       /* memory_region_list  */
  YYSYMBOL_memory_region = 164,            /* memory_region  */
  YYSYMBOL_memory_attr = 165,              /* memory_attr  */
  YYSYMBOL_nocrossrefs_command = 166,      /* nocrossrefs_command  */
  YYSYMBOL_output_command = 167,           /* output_command  */
  YYSYMBOL_output_arch_command = 168,      /* output_arch_command  */
  YYSYMBOL_output_format_command = 169,    /* output_format_command  */
  YYSYMBOL_phdrs_command = 170,            /* phdrs_command  */
  YYSYMBOL_phdr_list = 171,                /* phdr_list  */
  YYSYMBOL_phdr = 172,                     /* phdr  */
  YYSYMBOL_phdr_filehdr = 173,             /* phdr_filehdr  */
  YYSYMBOL_phdr_phdrs = 174,               /* phdr_phdrs  */
  YYSYMBOL_phdr_at = 175,                  /* phdr_at  */
  YYSYMBOL_phdr_flags = 176,               /* phdr_flags  */
  YYSYMBOL_region_alias_command = 177,     /* region_alias_command  */
  YYSYMBOL_search_dir_command = 178,       /* search_dir_command  */
  YYSYMBOL_sections_command = 179,         /* sections_command  */
  YYSYMBOL_sections_command_list = 180,    /* sections_command_list  */
  YYSYMBOL_sections_sub_command = 181,     /* sections_sub_command  */
  YYSYMBOL_output_sections_desc = 182,     /* output_sections_desc  */
  YYSYMBOL_183_1 = 183,                    /* $@1  */
  YYSYMBOL_output_section_addr_and_type = 184, /* output_section_addr_and_type  */
  YYSYMBOL_output_section_addr = 185,      /* output_section_addr  */
  YYSYMBOL_output_section_type = 186,      /* output_section_type  */
  YYSYMBOL_output_section_type_keyword = 187, /* output_section_type_keyword  */
  YYSYMBOL_output_section_lma = 188,       /* output_section_lma  */
  YYSYMBOL_output_section_align = 189,     /* output_section_align  */
  YYSYMBOL_output_section_subalign = 190,  /* output_section_subalign  */
  YYSYMBOL_output_section_constraint = 191, /* output_section_constraint  */
  YYSYMBOL_output_section_command_list = 192, /* output_section_command_list  */
  YYSYMBOL_output_section_command = 193,   /* output_section_command  */
  YYSYMBOL_input_section_desc = 194,       /* input_section_desc  */
  YYSYMBOL_input_section_desc_no_keep = 195, /* input_section_desc_no_keep  */
  YYSYMBOL_input_section = 196,            /* input_section  */
  YYSYMBOL_output_section_data = 197,      /* output_section_data  */
  YYSYMBOL_data_type = 198,                /* data_type  */
  YYSYMBOL_output_section_keywords = 199,  /* output_section_keywords  */
  YYSYMBOL_output_section_region = 200,    /* output_section_region  */
  YYSYMBOL_output_section_lma_region = 201, /* output_section_lma_region  */
  YYSYMBOL_output_section_phdr = 202,      /* output_section_phdr  */
  YYSYMBOL_output_section_fillexp = 203,   /* output_section_fillexp  */
  YYSYMBOL_overlay_desc = 204,             /* overlay_desc  */
  YYSYMBOL_overlay_vma = 205,              /* overlay_vma  */
  YYSYMBOL_overlay_nocref = 206,           /* overlay_nocref  */
  YYSYMBOL_overlay_section_list = 207,     /* overlay_section_list  */
  YYSYMBOL_overlay_section = 208,          /* overlay_section  */
  YYSYMBOL_startup_command = 209,          /* startup_command  */
  YYSYMBOL_target_command = 210,           /* target_command  */
  YYSYMBOL_version_script_node = 211,      /* version_script_node  */
  YYSYMBOL_extern_block = 212,             /* extern_block  */
  YYSYMBOL_version_block = 213,            /* version_block  */
  YYSYMBOL_version_entry_list = 214,       /* version_entry_list  */
  YYSYMBOL_version_entry = 215,            /* version_entry  */
  YYSYMBOL_version_dependency = 216,       /* version_dependency  */
  YYSYMBOL_ident = 217,                    /* ident  */
  YYSYMBOL_variable = 218,                 /* variable  */
  YYSYMBOL_wildcard = 219,                 /* wildcard  */
  YYSYMBOL_wildcard_sort = 220,            /* wildcard_sort  */
  YYSYMBOL_ident_list = 221,               /* ident_list  */
  YYSYMBOL_ident_list_nosep = 222,         /* ident_list_nosep  */
  YYSYMBOL_input_file_list = 223,          /* input_file_list  */
  YYSYMBOL_input_file = 224,               /* input_file  */
  YYSYMBOL_as_needed_list = 225,           /* as_needed_list  */
  YYSYMBOL_wildcard_list = 226,            /* wildcard_list  */
  YYSYMBOL_separator = 227                 /* separator  */
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
#define YYFINAL  86
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1725

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  124
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  104
/* YYNRULES -- Number of rules.  */
#define YYNRULES  253
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  525

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   357


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
       2,     2,     2,   115,     2,     2,     2,    99,    92,     2,
     117,   118,    97,    95,   119,    96,   123,    98,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    90,   120,
      94,    88,    93,    89,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   121,    91,   122,   116,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   279,   279,   280,   284,   288,   295,   298,   301,   304,
     307,   310,   313,   316,   319,   322,   325,   328,   331,   334,
     337,   340,   343,   346,   349,   352,   355,   358,   361,   362,
     363,   364,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     389,   395,   401,   404,   410,   416,   422,   428,   434,   440,
     446,   452,   458,   464,   470,   476,   482,   488,   494,   500,
     503,   509,   510,   514,   515,   521,   522,   523,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   543,   544,   545,   549,   555,   561,
     567,   568,   569,   570,   571,   572,   573,   574,   575,   579,
     585,   591,   595,   599,   605,   609,   615,   619,   622,   628,
     635,   636,   640,   646,   655,   662,   665,   671,   675,   678,
     683,   689,   690,   694,   695,   699,   700,   704,   705,   709,
     715,   722,   734,   738,   745,   746,   749,   752,   755,   759,
     759,   794,   798,   805,   809,   810,   811,   815,   816,   817,
     818,   819,   823,   824,   828,   829,   833,   834,   838,   839,
     840,   844,   848,   855,   858,   861,   864,   868,   872,   876,
     883,   888,   896,   903,   913,   923,   924,   925,   926,   927,
     928,   932,   935,   938,   944,   945,   949,   950,   954,   957,
     962,   963,   967,   989,   990,   994,   995,   999,  1002,  1008,
    1025,  1032,  1036,  1039,  1042,  1045,  1051,  1058,  1065,  1068,
    1074,  1078,  1082,  1085,  1091,  1092,  1096,  1097,  1101,  1102,
    1106,  1107,  1108,  1109,  1113,  1118,  1123,  1128,  1133,  1138,
    1143,  1151,  1152,  1156,  1157,  1161,  1162,  1166,  1167,  1171,
    1175,  1176,  1180,  1181
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
  "\"end of file\"", "error", "\"invalid token\"", "T_ABSOLUTE", "T_ADDR",
  "T_ALIGN", "T_ALIGNOF", "T_ASSERT", "T_AS_NEEDED", "T_AT", "T_BIND",
  "T_BLOCK", "T_BYTE", "T_CONSTANT", "T_CONSTRUCTORS",
  "T_CREATE_OBJECT_SYMBOLS", "T_DATA_SEGMENT_ALIGN", "T_DATA_SEGMENT_END",
  "T_DATA_SEGMENT_RELRO_END", "T_DEFINED", "T_ENTRY", "T_EXCLUDE_FILE",
  "T_EXTERN", "T_FILEHDR", "T_FILL", "T_FLAGS", "T_FLOAT",
  "T_FORCE_COMMON_ALLOCATION", "T_GROUP", "T_HLL", "T_INCLUDE",
  "T_INHIBIT_COMMON_ALLOCATION", "T_INPUT", "T_KEEP", "T_LENGTH",
  "T_LOADADDR", "T_LONG", "T_MAP", "T_MAX", "T_MEMORY", "T_MIN", "T_NEXT",
  "T_NOCROSSREFS", "T_NOFLOAT", "T_OPTION", "T_ORIGIN", "T_OUTPUT",
  "T_OUTPUT_ARCH", "T_OUTPUT_FORMAT", "T_PHDRS", "T_PROVIDE",
  "T_PROVIDE_HIDDEN", "T_QUAD", "T_REGION_ALIAS", "T_SEARCH_DIR",
  "T_SECTIONS", "T_SEGMENT_START", "T_SHORT", "T_SIZEOF",
  "T_SIZEOF_HEADERS", "T_SORT_BY_ALIGNMENT", "T_SORT_BY_NAME", "T_SPECIAL",
  "T_SQUAD", "T_STARTUP", "T_SUBALIGN", "T_SYSLIB", "T_TARGET",
  "T_TRUNCATE", "T_VER_EXTERN", "T_VER_GLOBAL", "T_VER_LOCAL",
  "T_LSHIFT_E", "T_RSHIFT_E", "T_LSHIFT", "T_RSHIFT", "T_EQ", "T_NE",
  "T_GE", "T_LE", "T_ADD_E", "T_SUB_E", "T_MUL_E", "T_DIV_E", "T_AND_E",
  "T_OR_E", "T_LOGICAL_AND", "T_LOGICAL_OR", "'='", "'?'", "':'", "'|'",
  "'&'", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "'%'", "UNARY", "T_NUM",
  "T_COMMONPAGESIZE", "T_COPY", "T_DSECT", "T_IDENT", "T_INFO",
  "T_MAXPAGESIZE", "T_MEMORY_ATTR", "T_NOLOAD", "T_ONLY_IF_RO",
  "T_ONLY_IF_RW", "T_OVERLAY", "T_STRING", "T_WILDCARD", "'!'", "'~'",
  "'('", "')'", "','", "';'", "'{'", "'}'", "'.'", "$accept", "script",
  "ldscript", "expression", "function", "absolute_function",
  "addr_function", "align_function", "alignof_function", "block_function",
  "data_segment_align_function", "data_segment_end_function",
  "data_segment_relro_end_function", "defined_function", "length_function",
  "loadaddr_function", "max_function", "min_function", "next_function",
  "origin_function", "segment_start_function", "sizeof_function",
  "sizeof_headers_function", "constant", "symbolic_constant",
  "ldscript_command", "assignment", "simple_assignment",
  "provide_assignment", "provide_hidden_assignment", "assign_op",
  "assert_command", "entry_command", "extern_command",
  "force_common_allocation_command", "group_command",
  "inhibit_common_allocation_command", "input_command", "memory_command",
  "memory_region_list", "memory_region", "memory_attr",
  "nocrossrefs_command", "output_command", "output_arch_command",
  "output_format_command", "phdrs_command", "phdr_list", "phdr",
  "phdr_filehdr", "phdr_phdrs", "phdr_at", "phdr_flags",
  "region_alias_command", "search_dir_command", "sections_command",
  "sections_command_list", "sections_sub_command", "output_sections_desc",
  "$@1", "output_section_addr_and_type", "output_section_addr",
  "output_section_type", "output_section_type_keyword",
  "output_section_lma", "output_section_align", "output_section_subalign",
  "output_section_constraint", "output_section_command_list",
  "output_section_command", "input_section_desc",
  "input_section_desc_no_keep", "input_section", "output_section_data",
  "data_type", "output_section_keywords", "output_section_region",
  "output_section_lma_region", "output_section_phdr",
  "output_section_fillexp", "overlay_desc", "overlay_vma",
  "overlay_nocref", "overlay_section_list", "overlay_section",
  "startup_command", "target_command", "version_script_node",
  "extern_block", "version_block", "version_entry_list", "version_entry",
  "version_dependency", "ident", "variable", "wildcard", "wildcard_sort",
  "ident_list", "ident_list_nosep", "input_file_list", "input_file",
  "as_needed_list", "wildcard_list", "separator", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-479)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-231)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     267,   -91,   -70,   -68,  -479,   -61,  -479,   -37,   -35,   -33,
     -12,     3,     8,    27,    10,    13,    34,    38,    41,    52,
      54,    65,  -479,  -479,  -479,   399,  -479,   180,   267,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,   -15,   -15,   -66,   492,   578,   -15,   -15,
      -6,    -6,   -15,   -15,   -15,   -15,   -15,   -15,   -75,   -75,
     -15,   -15,    37,   -15,   -15,    60,  -479,  -479,  -479,  -479,
    -479,    62,   533,  -479,  -479,    67,  -479,  -479,    70,  -479,
      73,   -15,   -15,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,  -479,   578,    68,    77,    81,    83,    85,    86,    87,
      91,    93,   101,   103,   115,   120,   121,   122,   125,   128,
     129,  -479,   578,  -479,   578,   578,   578,   565,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,   492,   136,  -479,   -89,   130,  -479,   -99,  -479,  -479,
     -18,   -90,  -479,   147,    -9,   148,   149,    14,   -16,  -479,
     -15,   107,   182,   152,   154,   578,  -479,  -479,  -479,   -10,
    -479,  -479,  -479,   245,   157,   158,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,   164,   171,  1566,   578,   -15,   578,   -15,
     578,   -85,   578,   578,   578,   -15,   -15,   -15,   578,   578,
     578,   -15,   -15,   -15,  -479,  -479,  -479,  1034,   578,   578,
     578,   578,   578,   578,   578,   578,   578,   578,   578,   578,
     578,   578,   578,   578,   578,   578,   165,  -479,  -479,  -479,
     -15,  -479,  -479,    -6,  -479,  -479,  -479,  -479,   187,  -479,
    -479,  -479,  -479,   -15,  -479,  -479,   265,   578,   578,   -15,
    -479,  1566,   202,  -479,  -479,   118,  1566,   203,   179,  -479,
    -479,  -479,  -479,  -479,  1062,   184,   855,   189,  1090,   190,
     194,   889,  1118,   918,   201,   205,   208,   947,   976,  1146,
     210,   178,   211,  -479,    48,    48,    19,    19,    19,    19,
    1618,  1592,  1540,  1626,   632,    19,    19,   -47,   -47,  -479,
    -479,  -479,   219,  -479,    22,  -479,   255,   186,  -479,   289,
    1174,  1202,   222,   300,  -479,  -479,  -479,  -479,  -479,  -479,
     225,  -479,   221,  -479,  -479,  -479,  -479,   578,  -479,  -479,
    -479,  -479,   578,  -479,   578,  -479,  -479,  -479,   578,   578,
    -479,  -479,   578,  -479,   578,  -479,  -479,   -15,   259,   -15,
    -479,   340,  -479,  -479,  -479,  -479,   342,  -479,   342,  1230,
    1258,  1286,  1314,  1342,  1370,  1566,  -479,   578,   234,   236,
     344,   254,   253,   370,  -479,  -479,  -479,  -479,  -479,  -479,
    1005,  -479,   578,   260,   256,   578,   -15,   261,   314,   348,
    1398,   283,  -479,  1426,     6,  -479,   271,   578,   269,    55,
     312,  -479,   276,  -479,   313,  -479,   805,  1454,   578,  -479,
    -479,   293,   578,  -479,   -15,  -479,  -479,  -479,  -479,  -479,
     288,  -479,  -479,  -479,   298,   305,  -479,  -479,  -479,   688,
    -479,  -479,  -479,  -479,   309,  -479,   -86,  -479,   -72,  -479,
    1482,   805,  1566,  -479,    -5,   155,   304,   112,  -479,  -479,
     578,   155,   168,  -479,  -479,   729,   578,   -15,  -479,   310,
     292,   311,   315,   316,   317,   319,   323,   326,    -5,  1510,
     330,   332,  -479,    78,   313,  1566,  -479,   306,  -479,   324,
     324,  -479,  -479,   324,   324,  -479,  -479,  -479,  -479,   155,
    -479,  -479,   422,   327,   336,   341,   345,   284,   371,  -479,
     353,   359,   360,   363,   155,   -15,    -5,  -479,  -479,  -479,
    -479,   294,  -479,  -479,  -479
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     0,   112,     0,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   226,   227,    93,     0,   229,     0,     2,     4,
      74,    94,    95,    96,    73,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,   225,   225,   228,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   220,   221,   233,   232,
     231,     0,     0,   218,   230,     0,     1,     5,     0,   224,
       0,   225,   225,   100,   101,   102,   103,   104,   105,   106,
     107,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,     0,     0,     0,    28,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    29,    70,    27,
     228,    30,     0,   243,     0,     0,   247,   253,   245,   248,
     253,     0,   117,   121,     0,     0,     0,     0,     0,   128,
       0,     0,     0,     0,     0,   204,   148,   145,   144,     0,
     142,   146,   147,   228,     0,     0,   216,   223,   217,   219,
     222,   214,   215,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    23,    25,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,   111,   244,
       0,   113,   252,     0,   115,   116,   118,   120,     0,   122,
     123,   124,   125,     0,   127,   129,   132,     0,     0,     0,
     140,   203,     0,   141,   143,     0,   153,     0,   156,   152,
     210,   211,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    19,    20,    15,    16,    17,    18,
      21,    22,     0,    12,    11,    13,    14,     6,     7,     8,
       9,    10,     0,   241,   253,   246,     0,     0,   131,   134,
       0,     0,     0,   206,   157,   158,   159,   160,   161,   155,
       0,   149,     0,   151,    50,    51,    52,     0,    54,    55,
      71,    72,     0,    57,     0,    59,    60,    61,     0,     0,
      64,    65,     0,    67,     0,   109,   249,     0,     0,     0,
     133,   136,    98,    99,   139,   205,   163,   154,   163,     0,
       0,     0,     0,     0,     0,    26,   242,     0,     0,     0,
     138,     0,     0,   165,    53,    56,    58,    62,    63,    66,
       0,   126,     0,     0,     0,     0,     0,     0,   167,     0,
       0,     0,   130,     0,     0,   207,     0,     0,     0,   170,
       0,   135,     0,   162,   195,   208,     0,     0,     0,   168,
     169,     0,     0,   137,     0,   199,   185,   192,   191,   190,
       0,   187,   188,   186,     0,     0,   189,   177,   173,     0,
     171,   174,   178,   175,     0,   176,   228,   234,     0,   164,
       0,     0,   119,   194,   201,     0,     0,     0,   199,   172,
       0,     0,     0,   180,   166,     0,     0,     0,   202,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   201,     0,
       0,     0,   250,     0,   195,   200,   198,     0,   179,     0,
       0,   238,   193,     0,     0,   235,   209,   184,   181,     0,
     183,   251,   197,     0,     0,     0,     0,     0,     0,   199,
       0,     0,     0,     0,     0,     0,   201,   240,   239,   237,
     236,     0,   196,   150,   182
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -479,  -479,  -479,   328,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,   405,     5,     9,  -479,  -479,
    -479,  -479,   -71,  -479,  -479,  -479,  -479,  -479,  -479,  -479,
     280,  -479,  -479,  -479,  -479,  -479,  -479,  -479,   318,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,   287,  -479,  -479,
    -479,  -479,   214,  -479,   116,  -479,  -479,  -479,    40,  -426,
    -479,    30,    12,  -479,  -479,  -479,    11,  -479,  -431,  -470,
    -479,  -479,  -479,  -479,    94,  -479,  -479,  -479,   -13,   -41,
    -479,   415,   -48,     0,     7,   -14,  -151,  -479,   437,   440,
     262,  -479,  -478,   188
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    27,    28,   217,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    29,   438,   149,    32,    33,
     102,    34,    35,    36,    37,    38,    39,    40,    41,   161,
     162,   248,    42,    43,    44,    45,    46,   168,   169,   319,
     361,   380,   394,    47,    48,    49,   179,   180,   181,   368,
     267,   268,   269,   330,   382,   398,   409,   421,   439,   440,
     441,   442,   463,   443,   444,   445,   425,   509,   454,   468,
     182,   262,   366,   404,   405,    50,    51,    52,    53,    54,
      82,    83,    88,   150,   151,   447,   448,   314,   154,   157,
     158,   159,   483,   243
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   178,   155,    21,  -230,    30,    90,    56,   496,    31,
       2,    85,    81,   459,    92,    22,    22,   279,   461,   241,
     242,   507,   280,    23,    23,    84,    57,   478,    55,   238,
      22,  -230,   245,    30,   186,    56,   521,    31,    23,   459,
      14,    15,    91,   193,   194,   462,   523,    58,    26,    59,
     233,   234,   235,    89,    89,    25,    60,     2,   152,   153,
     156,   156,   163,   153,   165,   166,   167,   170,    85,    81,
     173,   174,   183,   184,   185,   171,   172,   177,   516,    56,
      61,    31,    84,   466,    63,   467,    62,    14,    15,    22,
      22,    89,    89,   218,   219,    22,    22,    23,    23,    22,
     244,   242,   175,    23,    23,    64,   254,    23,   178,   249,
     176,    22,   263,    26,   231,   232,   233,   234,   235,    23,
      65,   103,   104,   105,   106,    66,   474,    68,   414,   107,
      69,   108,   252,   253,   109,   110,   111,   112,   434,   469,
     356,   242,    22,   231,   232,   233,   234,   235,    67,   175,
      23,    70,   113,   114,   239,    71,   115,   176,   116,   117,
      26,   163,    72,   118,   239,   419,   420,    78,   170,    73,
     256,    74,   475,   476,   119,    79,   120,   121,    75,   183,
      86,    25,   187,    22,   177,   196,    56,   190,    31,   481,
     191,    23,    80,   192,   197,   257,   500,   275,   198,   277,
     199,    78,   200,   201,   202,   284,   285,   286,   203,    79,
     204,   290,   291,   292,   122,   434,   469,    22,   205,   123,
     206,   324,   325,    22,   326,    23,    80,   327,   434,   469,
     328,    23,   207,   124,   125,   126,   329,   208,   209,   210,
     313,    26,   211,   156,    78,   212,   213,   240,   103,   104,
     105,   106,    79,   317,   237,   247,   107,    78,   108,   322,
      22,   109,   110,   111,   112,    79,   250,   251,    23,    80,
     258,   259,   260,    22,     1,   270,   271,   316,   312,   113,
     114,    23,    80,   115,   272,   116,   117,     2,   318,     3,
     118,   273,   323,   331,     4,     5,   332,   352,     6,     7,
     358,   119,   335,   120,   121,   359,     8,   338,   340,     9,
     480,   482,   341,    10,    11,    12,    13,    14,    15,   345,
      16,    17,    18,   346,   324,   325,   347,   326,   351,   353,
     327,    19,   501,   328,    20,  -156,    21,   355,   360,   329,
     364,   122,   365,   367,   434,   469,   123,   377,   482,   379,
      22,   381,   391,   392,   434,   469,   501,   376,    23,   378,
     124,   125,   265,   482,   471,   472,   475,   476,    26,   393,
     501,   395,    22,    78,   396,   397,   402,   401,   407,   408,
      23,    79,   410,    78,   412,   127,   418,    24,    25,    22,
      26,    79,   416,    78,   423,    78,   406,    23,    80,    22,
     422,    79,   514,    79,   406,   455,   424,    23,    80,    22,
     488,    22,   524,    78,   451,   456,   446,    23,    80,    23,
      80,    79,   457,    56,   453,    31,   460,   487,   489,    22,
     195,   508,   490,    87,   491,   492,   493,    23,    80,   446,
     494,   246,   473,   477,   495,   510,    56,   462,    31,   499,
     214,   446,   215,   216,   511,    84,    84,    84,    56,   512,
      31,    84,    84,   513,   515,   446,   264,   486,    21,    76,
      77,   517,    56,   477,    31,   503,   504,   518,   519,   505,
     506,   520,   333,    84,   383,   470,   255,    84,    78,    84,
      84,   465,   498,    84,    84,   502,    79,   189,   415,    84,
     164,   160,   357,   261,    22,   315,     0,    84,     0,     0,
       0,   266,    23,    80,    84,   522,     0,     0,     0,     0,
       0,    84,     0,     0,   274,     0,   276,     0,   278,     0,
     281,   282,   283,     0,     0,     0,   287,   288,   289,     0,
       0,     0,     0,     0,     0,     0,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,    93,    94,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,   100,     0,     0,
     101,   103,   104,   105,   106,   320,   321,     0,     0,   107,
       0,   108,     0,     0,   109,   110,   111,   112,     0,     0,
       0,     0,    21,    76,    77,     0,     0,     0,     0,     0,
       0,     0,   113,   114,     0,     0,   115,     0,   116,   117,
       0,     0,    78,   118,     0,     0,     0,     0,     0,     0,
      79,     0,     0,     0,   119,     0,   120,   121,    22,   218,
     219,   220,   221,   222,   223,     0,    23,    80,     0,     0,
       0,   224,   225,     0,   226,   188,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   369,     0,     0,     0,     0,
     370,     0,   371,     0,   122,     0,   372,   373,     0,   123,
     374,     0,   375,    22,   236,     0,     0,     0,     0,     0,
       0,    23,     0,   124,   125,   126,     0,     0,     0,     0,
     426,    26,   427,   428,     0,   390,   218,   219,   220,   221,
     222,   223,   429,     0,     0,     0,     0,     0,     0,     0,
     400,   430,     0,   403,   431,   229,   230,   231,   232,   233,
     234,   235,     0,     0,     0,   417,     0,     0,    14,    15,
     432,   426,     0,   427,   428,   433,   450,     0,   434,   435,
     452,   436,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     0,    14,
      15,   432,     0,     0,     0,    79,   433,     0,   479,   434,
     435,     0,   436,    22,   485,     0,     0,     0,     0,     0,
       0,    23,    80,     0,     0,     0,     0,     0,   437,     0,
     458,    26,     0,     0,     0,     0,     0,   426,    78,   427,
     428,     0,     0,     0,     0,     0,    79,     0,     0,   429,
       0,     0,     0,     0,    22,     0,     0,     0,   430,     0,
       0,   431,    23,    80,     0,     0,     0,     0,     0,   437,
       0,   484,    26,     0,     0,    14,    15,   432,     0,     0,
       0,     0,   433,     0,     0,   434,   435,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,    79,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    23,    80,
       0,     0,     0,     0,     0,   437,     0,     0,    26,   218,
     219,   220,   221,   222,   223,     0,     0,     0,     0,     0,
       0,   224,   225,     0,   226,     0,   227,   228,   229,   230,
     231,   232,   233,   234,   235,     0,     0,     0,     0,     0,
       0,     0,     0,   218,   219,   220,   221,   222,   223,     0,
       0,     0,     0,   336,   337,   224,   225,     0,   226,     0,
     227,   228,   229,   230,   231,   232,   233,   234,   235,     0,
       0,     0,   218,   219,   220,   221,   222,   223,     0,     0,
       0,     0,     0,     0,   224,   225,     0,   226,   342,   227,
     228,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,   218,   219,   220,   221,   222,   223,     0,     0,     0,
       0,     0,     0,   224,   225,     0,   226,   344,   227,   228,
     229,   230,   231,   232,   233,   234,   235,     0,     0,     0,
     218,   219,   220,   221,   222,   223,     0,     0,     0,     0,
       0,     0,   224,   225,     0,   226,   348,   227,   228,   229,
     230,   231,   232,   233,   234,   235,     0,     0,     0,   218,
     219,   220,   221,   222,   223,     0,     0,     0,     0,     0,
       0,   224,   225,     0,   226,   349,   227,   228,   229,   230,
     231,   232,   233,   234,   235,     0,     0,     0,   218,   219,
     220,   221,   222,   223,     0,     0,     0,     0,     0,     0,
     224,   225,     0,   226,   399,   227,   228,   229,   230,   231,
     232,   233,   234,   235,     0,     0,   218,   219,   220,   221,
     222,   223,     0,     0,     0,     0,     0,     0,   224,   225,
       0,   226,   293,   227,   228,   229,   230,   231,   232,   233,
     234,   235,     0,     0,   218,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,     0,   224,   225,     0,   226,
     334,   227,   228,   229,   230,   231,   232,   233,   234,   235,
       0,     0,   218,   219,   220,   221,   222,   223,     0,     0,
       0,     0,     0,     0,   224,   225,     0,   226,   339,   227,
     228,   229,   230,   231,   232,   233,   234,   235,     0,     0,
     218,   219,   220,   221,   222,   223,     0,     0,     0,     0,
       0,     0,   224,   225,     0,   226,   343,   227,   228,   229,
     230,   231,   232,   233,   234,   235,     0,     0,   218,   219,
     220,   221,   222,   223,     0,     0,     0,     0,     0,     0,
     224,   225,     0,   226,   350,   227,   228,   229,   230,   231,
     232,   233,   234,   235,     0,     0,   218,   219,   220,   221,
     222,   223,     0,     0,     0,     0,     0,     0,   224,   225,
       0,   226,   362,   227,   228,   229,   230,   231,   232,   233,
     234,   235,     0,     0,   218,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,     0,   224,   225,     0,   226,
     363,   227,   228,   229,   230,   231,   232,   233,   234,   235,
       0,     0,   218,   219,   220,   221,   222,   223,     0,     0,
       0,     0,     0,     0,   224,   225,     0,   226,   384,   227,
     228,   229,   230,   231,   232,   233,   234,   235,     0,     0,
     218,   219,   220,   221,   222,   223,     0,     0,     0,     0,
       0,     0,   224,   225,     0,   226,   385,   227,   228,   229,
     230,   231,   232,   233,   234,   235,     0,     0,   218,   219,
     220,   221,   222,   223,     0,     0,     0,     0,     0,     0,
     224,   225,     0,   226,   386,   227,   228,   229,   230,   231,
     232,   233,   234,   235,     0,     0,   218,   219,   220,   221,
     222,   223,     0,     0,     0,     0,     0,     0,   224,   225,
       0,   226,   387,   227,   228,   229,   230,   231,   232,   233,
     234,   235,     0,     0,   218,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,     0,   224,   225,     0,   226,
     388,   227,   228,   229,   230,   231,   232,   233,   234,   235,
       0,     0,   218,   219,   220,   221,   222,   223,     0,     0,
       0,     0,     0,     0,   224,   225,     0,   226,   389,   227,
     228,   229,   230,   231,   232,   233,   234,   235,     0,     0,
     218,   219,   220,   221,   222,   223,     0,     0,     0,     0,
       0,     0,   224,   225,     0,   226,   411,   227,   228,   229,
     230,   231,   232,   233,   234,   235,     0,     0,   218,   219,
     220,   221,   222,   223,     0,     0,     0,     0,     0,     0,
     224,   225,     0,   226,   413,   227,   228,   229,   230,   231,
     232,   233,   234,   235,     0,     0,   218,   219,   220,   221,
     222,   223,     0,     0,     0,     0,     0,     0,   224,   225,
       0,   226,   449,   227,   228,   229,   230,   231,   232,   233,
     234,   235,     0,     0,   218,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,     0,   224,   225,     0,   226,
     464,   227,   228,   229,   230,   231,   232,   233,   234,   235,
       0,     0,     0,     0,   218,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,     0,   224,   225,   497,   226,
     354,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     218,   219,   220,   221,   222,   223,     0,     0,     0,     0,
       0,     0,   224,   225,     0,   226,     0,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   218,   219,   220,   221,
     222,   223,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   218,   219,   220,   221,   222,   223,     0,     0,
     218,   219,   220,   221,   222,   223,     0,     0,     0,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   228,   229,
     230,   231,   232,   233,   234,   235
};

static const yytype_int16 yycheck[] =
{
       0,    72,     8,    69,    90,     0,    54,     0,   478,     0,
      20,    25,    25,   439,    55,   105,   105,   102,    90,   118,
     119,   499,   107,   113,   113,    25,   117,   458,    28,   118,
     105,   117,   122,    28,    75,    28,   514,    28,   113,   465,
      50,    51,    55,    91,    92,   117,   516,   117,   123,   117,
      97,    98,    99,    53,    54,   121,   117,    20,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    82,    82,
      70,    71,    72,    73,    74,    68,    69,    72,   509,    72,
     117,    72,    82,    88,   117,    90,   121,    50,    51,   105,
     105,    91,    92,    74,    75,   105,   105,   113,   113,   105,
     118,   119,   112,   113,   113,   117,   122,   113,   179,   118,
     120,   105,   122,   123,    95,    96,    97,    98,    99,   113,
     117,     3,     4,     5,     6,   117,    14,   117,   122,    11,
     117,    13,   118,   119,    16,    17,    18,    19,    60,    61,
     118,   119,   105,    95,    96,    97,    98,    99,   121,   112,
     113,   117,    34,    35,   154,   117,    38,   120,    40,    41,
     123,   161,   121,    45,   164,   110,   111,    89,   168,   117,
     170,   117,    60,    61,    56,    97,    58,    59,   113,   179,
       0,   121,   120,   105,   179,   117,   179,   120,   179,    21,
     120,   113,   114,   120,   117,    88,   118,   197,   117,   199,
     117,    89,   117,   117,   117,   205,   206,   207,   117,    97,
     117,   211,   212,   213,    96,    60,    61,   105,   117,   101,
     117,   103,   104,   105,   106,   113,   114,   109,    60,    61,
     112,   113,   117,   115,   116,   117,   118,   117,   117,   117,
     240,   123,   117,   243,    89,   117,   117,   117,     3,     4,
       5,     6,    97,   253,   118,   108,    11,    89,    13,   259,
     105,    16,    17,    18,    19,    97,   118,   118,   113,   114,
      88,   119,   118,   105,     7,   118,   118,    90,   113,    34,
      35,   113,   114,    38,   120,    40,    41,    20,    23,    22,
      45,   120,    90,    90,    27,    28,   117,   119,    31,    32,
      45,    56,   118,    58,    59,   119,    39,   118,   118,    42,
     461,   462,   118,    46,    47,    48,    49,    50,    51,   118,
      53,    54,    55,   118,   103,   104,   118,   106,   118,   118,
     109,    64,   483,   112,    67,    90,    69,   118,    49,   118,
     118,    96,    42,   118,    60,    61,   101,    88,   499,     9,
     105,     9,   118,   117,    60,    61,   507,   357,   113,   359,
     115,   116,   117,   514,    60,    61,    60,    61,   123,    25,
     521,   117,   105,    89,   121,     5,   120,   117,   117,    65,
     113,    97,    34,    89,   101,    57,   117,   120,   121,   105,
     123,    97,   121,    89,   118,    89,   396,   113,   114,   105,
      88,    97,   118,    97,   404,   117,    93,   113,   114,   105,
     118,   105,   118,    89,   121,   117,   416,   113,   114,   113,
     114,    97,   117,   416,   424,   416,   117,   117,   117,   105,
     102,     9,   117,    28,   118,   118,   117,   113,   114,   439,
     117,   161,   456,   457,   118,   118,   439,   117,   439,   117,
     122,   451,   124,   125,   118,   455,   456,   457,   451,   118,
     451,   461,   462,   118,    93,   465,   179,   467,    69,    70,
      71,   118,   465,   487,   465,   489,   490,   118,   118,   493,
     494,   118,   268,   483,   368,   455,   168,   487,    89,   489,
     490,   451,   480,   493,   494,   484,    97,    82,   404,   499,
      63,    61,   314,   175,   105,   243,    -1,   507,    -1,    -1,
      -1,   183,   113,   114,   514,   515,    -1,    -1,    -1,    -1,
      -1,   521,    -1,    -1,   196,    -1,   198,    -1,   200,    -1,
     202,   203,   204,    -1,    -1,    -1,   208,   209,   210,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    85,    -1,    -1,
      88,     3,     4,     5,     6,   257,   258,    -1,    -1,    11,
      -1,    13,    -1,    -1,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    35,    -1,    -1,    38,    -1,    40,    41,
      -1,    -1,    89,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    56,    -1,    58,    59,   105,    74,
      75,    76,    77,    78,    79,    -1,   113,   114,    -1,    -1,
      -1,    86,    87,    -1,    89,   122,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   337,    -1,    -1,    -1,    -1,
     342,    -1,   344,    -1,    96,    -1,   348,   349,    -1,   101,
     352,    -1,   354,   105,   119,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,   115,   116,   117,    -1,    -1,    -1,    -1,
      12,   123,    14,    15,    -1,   377,    74,    75,    76,    77,
      78,    79,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     392,    33,    -1,   395,    36,    93,    94,    95,    96,    97,
      98,    99,    -1,    -1,    -1,   407,    -1,    -1,    50,    51,
      52,    12,    -1,    14,    15,    57,   418,    -1,    60,    61,
     422,    63,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    50,
      51,    52,    -1,    -1,    -1,    97,    57,    -1,   460,    60,
      61,    -1,    63,   105,   466,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,   123,    -1,    -1,    -1,    -1,    -1,    12,    89,    14,
      15,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    24,
      -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,    33,    -1,
      -1,    36,   113,   114,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,   123,    -1,    -1,    50,    51,    52,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    60,    61,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,   123,    74,
      75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    -1,    89,    -1,    91,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    75,    76,    77,    78,    79,    -1,
      -1,    -1,    -1,   118,   119,    86,    87,    -1,    89,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    -1,    89,   119,    91,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    87,    -1,    89,   119,    91,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    89,   119,    91,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,    -1,    -1,    74,
      75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    -1,    89,   119,    91,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,    -1,    -1,    74,    75,
      76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    89,   119,    91,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,    -1,    74,    75,    76,    77,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      -1,    89,   118,    91,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,    -1,    74,    75,    76,    77,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,
     118,    91,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,    -1,    74,    75,    76,    77,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    -1,    89,   118,    91,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,    -1,
      74,    75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    89,   118,    91,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,    -1,    74,    75,
      76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    89,   118,    91,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,    -1,    74,    75,    76,    77,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      -1,    89,   118,    91,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,    -1,    74,    75,    76,    77,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,
     118,    91,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,    -1,    74,    75,    76,    77,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    -1,    89,   118,    91,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,    -1,
      74,    75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    89,   118,    91,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,    -1,    74,    75,
      76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    89,   118,    91,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,    -1,    74,    75,    76,    77,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      -1,    89,   118,    91,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,    -1,    74,    75,    76,    77,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,
     118,    91,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,    -1,    74,    75,    76,    77,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    87,    -1,    89,   118,    91,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,    -1,
      74,    75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    89,   118,    91,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,    -1,    74,    75,
      76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    89,   118,    91,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,    -1,    74,    75,    76,    77,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      -1,    89,   118,    91,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,    -1,    74,    75,    76,    77,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,
     118,    91,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,   118,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
      74,    75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    89,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    99,    74,    75,    76,    77,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    99,    74,    75,    76,    77,    78,    79,    -1,    -1,
      74,    75,    76,    77,    78,    79,    -1,    -1,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    99,    92,    93,
      94,    95,    96,    97,    98,    99
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,    20,    22,    27,    28,    31,    32,    39,    42,
      46,    47,    48,    49,    50,    51,    53,    54,    55,    64,
      67,    69,   105,   113,   120,   121,   123,   125,   126,   149,
     150,   151,   152,   153,   155,   156,   157,   158,   159,   160,
     161,   162,   166,   167,   168,   169,   170,   177,   178,   179,
     209,   210,   211,   212,   213,   217,   218,   117,   117,   117,
     117,   117,   121,   117,   117,   117,   117,   121,   117,   117,
     117,   117,   121,   117,   117,   113,    70,    71,    89,    97,
     114,   212,   214,   215,   217,   219,     0,   149,   216,   217,
     216,   212,   213,    72,    73,    80,    81,    82,    83,    84,
      85,    88,   154,     3,     4,     5,     6,    11,    13,    16,
      17,    18,    19,    34,    35,    38,    40,    41,    45,    56,
      58,    59,    96,   101,   115,   116,   117,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   151,
     217,   218,   217,   217,   222,     8,   217,   223,   224,   225,
     223,   163,   164,   217,   222,   217,   217,   217,   171,   172,
     217,   218,   218,   217,   217,   112,   120,   150,   156,   180,
     181,   182,   204,   217,   217,   217,   213,   120,   122,   215,
     120,   120,   120,   216,   216,   127,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   127,   127,   127,   127,    74,    75,
      76,    77,    78,    79,    86,    87,    89,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   119,   118,   118,   217,
     117,   118,   119,   227,   118,   122,   164,   108,   165,   118,
     118,   118,   118,   119,   122,   172,   217,    88,    88,   119,
     118,   127,   205,   122,   181,   117,   127,   184,   185,   186,
     118,   118,   120,   120,   127,   217,   127,   217,   127,   102,
     107,   127,   127,   127,   217,   217,   217,   127,   127,   127,
     217,   217,   217,   118,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   113,   217,   221,   224,    90,   217,    23,   173,
     127,   127,   217,    90,   103,   104,   106,   109,   112,   118,
     187,    90,   117,   186,   118,   118,   118,   119,   118,   118,
     118,   118,   119,   118,   119,   118,   118,   118,   119,   119,
     118,   118,   119,   118,    90,   118,   118,   227,    45,   119,
      49,   174,   118,   118,   118,    42,   206,   118,   183,   127,
     127,   127,   127,   127,   127,   127,   217,    88,   217,     9,
     175,     9,   188,   188,   118,   118,   118,   118,   118,   118,
     127,   118,   117,    25,   176,   117,   121,     5,   189,   119,
     127,   117,   120,   127,   207,   208,   217,   117,    65,   190,
      34,   118,   101,   118,   122,   208,   121,   127,   117,   110,
     111,   191,    88,   118,    93,   200,    12,    14,    15,    24,
      33,    36,    52,    57,    60,    61,    63,   120,   150,   192,
     193,   194,   195,   197,   198,   199,   217,   219,   220,   118,
     127,   121,   127,   217,   202,   117,   117,   117,   122,   193,
     117,    90,   117,   196,   118,   192,    88,    90,   203,    61,
     195,    60,    61,   219,    14,    60,    61,   219,   202,   127,
     220,    21,   220,   226,   122,   127,   217,   117,   118,   117,
     117,   118,   118,   117,   117,   118,   203,   118,   196,   117,
     118,   220,   200,   219,   219,   219,   219,   226,     9,   201,
     118,   118,   118,   118,   118,    93,   202,   118,   118,   118,
     118,   226,   217,   203,   118
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   124,   125,   125,   126,   126,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     129,   130,   131,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     147,   148,   148,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   150,   150,   150,   151,   152,   153,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   163,   164,
     165,   165,   166,   167,   168,   169,   169,   170,   171,   171,
     172,   173,   173,   174,   174,   175,   175,   176,   176,   177,
     178,   179,   180,   180,   181,   181,   181,   181,   181,   183,
     182,   184,   184,   185,   186,   186,   186,   187,   187,   187,
     187,   187,   188,   188,   189,   189,   190,   190,   191,   191,
     191,   192,   192,   193,   193,   193,   193,   193,   194,   194,
     195,   195,   196,   196,   197,   198,   198,   198,   198,   198,
     198,   199,   199,   199,   200,   200,   201,   201,   202,   202,
     203,   203,   204,   205,   205,   206,   206,   207,   207,   208,
     209,   210,   211,   211,   211,   211,   212,   213,   214,   214,
     215,   215,   215,   215,   216,   216,   217,   217,   218,   218,
     219,   219,   219,   219,   220,   220,   220,   220,   220,   220,
     220,   221,   221,   222,   222,   223,   223,   224,   224,   225,
     226,   226,   227,   227
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     1,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     5,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     6,     4,     4,     6,     4,     6,     4,
       4,     4,     6,     6,     4,     4,     6,     4,     1,     1,
       1,     4,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     6,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       4,     4,     1,     4,     1,     4,     4,     1,     2,    10,
       1,     0,     4,     4,     4,     4,     8,     4,     1,     2,
       7,     1,     0,     1,     0,     4,     0,     4,     0,     6,
       4,     4,     1,     2,     1,     1,     1,     1,     1,     0,
      15,     2,     1,     1,     3,     2,     0,     1,     1,     1,
       1,     1,     4,     0,     4,     0,     4,     0,     1,     1,
       0,     1,     2,     1,     1,     1,     1,     1,     1,     4,
       2,     4,     7,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     2,     0,     3,     0,     3,     0,
       2,     0,    11,     1,     0,     1,     0,     1,     2,     6,
       4,     4,     4,     4,     3,     3,     3,     3,     1,     2,
       1,     1,     2,     2,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     7,     7,     4,     7,
       7,     1,     3,     1,     2,     1,     3,     1,     1,     4,
       1,     2,     1,     0
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
  case 4: /* ldscript: ldscript_command  */
#line 284 "ld_script_parser.y"
                           {
		if ((yyvsp[0].cmd) != NULL)
			ld_script_cmd_insert(&ld->ld_scp->lds_c, (yyvsp[0].cmd));
	}
#line 1995 "ld_script_parser.c"
    break;

  case 5: /* ldscript: ldscript ldscript_command  */
#line 288 "ld_script_parser.y"
                                    {
		if ((yyvsp[0].cmd) != NULL)
			ld_script_cmd_insert(&ld->ld_scp->lds_c, (yyvsp[0].cmd));
	}
#line 2004 "ld_script_parser.c"
    break;

  case 6: /* expression: expression '+' expression  */
#line 295 "ld_script_parser.y"
                                    {
		(yyval.exp) = ld_exp_binary(ld, LEOP_ADD, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2012 "ld_script_parser.c"
    break;

  case 7: /* expression: expression '-' expression  */
#line 298 "ld_script_parser.y"
                                    {
		(yyval.exp) = ld_exp_binary(ld, LEOP_SUBSTRACT, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2020 "ld_script_parser.c"
    break;

  case 8: /* expression: expression '*' expression  */
#line 301 "ld_script_parser.y"
                                    {
		(yyval.exp) = ld_exp_binary(ld, LEOP_MUL, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2028 "ld_script_parser.c"
    break;

  case 9: /* expression: expression '/' expression  */
#line 304 "ld_script_parser.y"
                                    {
		(yyval.exp) = ld_exp_binary(ld, LEOP_DIV, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2036 "ld_script_parser.c"
    break;

  case 10: /* expression: expression '%' expression  */
#line 307 "ld_script_parser.y"
                                    {
		(yyval.exp) = ld_exp_binary(ld, LEOP_MOD, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2044 "ld_script_parser.c"
    break;

  case 11: /* expression: expression '&' expression  */
#line 310 "ld_script_parser.y"
                                    {
		(yyval.exp) = ld_exp_binary(ld, LEOP_AND, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2052 "ld_script_parser.c"
    break;

  case 12: /* expression: expression '|' expression  */
#line 313 "ld_script_parser.y"
                                    {
		(yyval.exp) = ld_exp_binary(ld, LEOP_OR, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2060 "ld_script_parser.c"
    break;

  case 13: /* expression: expression '>' expression  */
#line 316 "ld_script_parser.y"
                                    {
		(yyval.exp) = ld_exp_binary(ld, LEOP_GREATER, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2068 "ld_script_parser.c"
    break;

  case 14: /* expression: expression '<' expression  */
#line 319 "ld_script_parser.y"
                                    {
		(yyval.exp) = ld_exp_binary(ld, LEOP_LESSER, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2076 "ld_script_parser.c"
    break;

  case 15: /* expression: expression T_EQ expression  */
#line 322 "ld_script_parser.y"
                                     {
		(yyval.exp) = ld_exp_binary(ld, LEOP_EQUAL, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2084 "ld_script_parser.c"
    break;

  case 16: /* expression: expression T_NE expression  */
#line 325 "ld_script_parser.y"
                                     {
		(yyval.exp) = ld_exp_binary(ld, LEOP_NE, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2092 "ld_script_parser.c"
    break;

  case 17: /* expression: expression T_GE expression  */
#line 328 "ld_script_parser.y"
                                     {
		(yyval.exp) = ld_exp_binary(ld, LEOP_GE, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2100 "ld_script_parser.c"
    break;

  case 18: /* expression: expression T_LE expression  */
#line 331 "ld_script_parser.y"
                                     {
		(yyval.exp) = ld_exp_binary(ld, LEOP_LE, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2108 "ld_script_parser.c"
    break;

  case 19: /* expression: expression T_LSHIFT expression  */
#line 334 "ld_script_parser.y"
                                         {
		(yyval.exp) = ld_exp_binary(ld, LEOP_LSHIFT, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2116 "ld_script_parser.c"
    break;

  case 20: /* expression: expression T_RSHIFT expression  */
#line 337 "ld_script_parser.y"
                                         {
		(yyval.exp) = ld_exp_binary(ld, LEOP_RSHIFT, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2124 "ld_script_parser.c"
    break;

  case 21: /* expression: expression T_LOGICAL_AND expression  */
#line 340 "ld_script_parser.y"
                                              {
		(yyval.exp) = ld_exp_binary(ld, LEOP_LOGICAL_AND, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2132 "ld_script_parser.c"
    break;

  case 22: /* expression: expression T_LOGICAL_OR expression  */
#line 343 "ld_script_parser.y"
                                             {
		(yyval.exp) = ld_exp_binary(ld, LEOP_LOGICAL_OR, (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2140 "ld_script_parser.c"
    break;

  case 23: /* expression: '!' expression  */
#line 346 "ld_script_parser.y"
                                     {
		(yyval.exp) = ld_exp_unary(ld, LEOP_NOT, (yyvsp[0].exp));
	}
#line 2148 "ld_script_parser.c"
    break;

  case 24: /* expression: '-' expression  */
#line 349 "ld_script_parser.y"
                                     {
		(yyval.exp) = ld_exp_unary(ld, LEOP_MINUS, (yyvsp[0].exp));
	}
#line 2156 "ld_script_parser.c"
    break;

  case 25: /* expression: '~' expression  */
#line 352 "ld_script_parser.y"
                                     {
		(yyval.exp) = ld_exp_unary(ld, LEOP_NEGATION, (yyvsp[0].exp));
	}
#line 2164 "ld_script_parser.c"
    break;

  case 26: /* expression: expression '?' expression ':' expression  */
#line 355 "ld_script_parser.y"
                                                   {
		(yyval.exp) = ld_exp_trinary(ld, (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2172 "ld_script_parser.c"
    break;

  case 27: /* expression: simple_assignment  */
#line 358 "ld_script_parser.y"
                            {
		(yyval.exp) = ld_exp_assign(ld, (yyvsp[0].assign));
	}
#line 2180 "ld_script_parser.c"
    break;

  case 31: /* expression: '(' expression ')'  */
#line 364 "ld_script_parser.y"
                             { (yyval.exp) = (yyvsp[-1].exp);	(yyval.exp)->le_par = 1; }
#line 2186 "ld_script_parser.c"
    break;

  case 50: /* absolute_function: T_ABSOLUTE '(' expression ')'  */
#line 389 "ld_script_parser.y"
                                        {
		(yyval.exp) = ld_exp_unary(ld, LEOP_ABS, (yyvsp[-1].exp));
	}
#line 2194 "ld_script_parser.c"
    break;

  case 51: /* addr_function: T_ADDR '(' ident ')'  */
#line 395 "ld_script_parser.y"
                               {
		(yyval.exp) = ld_exp_unary(ld, LEOP_ADDR, ld_exp_name(ld, (yyvsp[-1].str)));
	}
#line 2202 "ld_script_parser.c"
    break;

  case 52: /* align_function: T_ALIGN '(' expression ')'  */
#line 401 "ld_script_parser.y"
                                     {
		(yyval.exp) = ld_exp_unary(ld, LEOP_ALIGN, (yyvsp[-1].exp));
	}
#line 2210 "ld_script_parser.c"
    break;

  case 53: /* align_function: T_ALIGN '(' expression ',' expression ')'  */
#line 404 "ld_script_parser.y"
                                                    {
		(yyval.exp) = ld_exp_binary(ld, LEOP_ALIGN, (yyvsp[-3].exp), (yyvsp[-1].exp));
	}
#line 2218 "ld_script_parser.c"
    break;

  case 54: /* alignof_function: T_ALIGNOF '(' ident ')'  */
#line 410 "ld_script_parser.y"
                                  {
		(yyval.exp) = ld_exp_unary(ld, LEOP_ALIGNOF, ld_exp_name(ld, (yyvsp[-1].str)));
	}
#line 2226 "ld_script_parser.c"
    break;

  case 55: /* block_function: T_BLOCK '(' expression ')'  */
#line 416 "ld_script_parser.y"
                                     {
		(yyval.exp) = ld_exp_unary(ld, LEOP_BLOCK, (yyvsp[-1].exp));
	}
#line 2234 "ld_script_parser.c"
    break;

  case 56: /* data_segment_align_function: T_DATA_SEGMENT_ALIGN '(' expression ',' expression ')'  */
#line 422 "ld_script_parser.y"
                                                                 {
		(yyval.exp) = ld_exp_binary(ld, LEOP_DSA, (yyvsp[-3].exp), (yyvsp[-1].exp));
	}
#line 2242 "ld_script_parser.c"
    break;

  case 57: /* data_segment_end_function: T_DATA_SEGMENT_END '(' expression ')'  */
#line 428 "ld_script_parser.y"
                                                {
		(yyval.exp) = ld_exp_unary(ld, LEOP_DSE, (yyvsp[-1].exp));
	}
#line 2250 "ld_script_parser.c"
    break;

  case 58: /* data_segment_relro_end_function: T_DATA_SEGMENT_RELRO_END '(' expression ',' expression ')'  */
#line 434 "ld_script_parser.y"
                                                                     {
		(yyval.exp) = ld_exp_binary(ld, LEOP_DSRE, (yyvsp[-3].exp), (yyvsp[-1].exp));
	}
#line 2258 "ld_script_parser.c"
    break;

  case 59: /* defined_function: T_DEFINED '(' ident ')'  */
#line 440 "ld_script_parser.y"
                                  {
		(yyval.exp) = ld_exp_unary(ld, LEOP_DEFINED, ld_exp_symbol(ld, (yyvsp[-1].str)));
	}
#line 2266 "ld_script_parser.c"
    break;

  case 60: /* length_function: T_LENGTH '(' ident ')'  */
#line 446 "ld_script_parser.y"
                                 {
		(yyval.exp) = ld_exp_unary(ld, LEOP_LENGTH, ld_exp_name(ld, (yyvsp[-1].str)));
	}
#line 2274 "ld_script_parser.c"
    break;

  case 61: /* loadaddr_function: T_LOADADDR '(' ident ')'  */
#line 452 "ld_script_parser.y"
                                   {
		(yyval.exp) = ld_exp_unary(ld, LEOP_LOADADDR, ld_exp_name(ld, (yyvsp[-1].str)));
	}
#line 2282 "ld_script_parser.c"
    break;

  case 62: /* max_function: T_MAX '(' expression ',' expression ')'  */
#line 458 "ld_script_parser.y"
                                                  {
		(yyval.exp) = ld_exp_binary(ld, LEOP_MAX, (yyvsp[-3].exp), (yyvsp[-1].exp));
	}
#line 2290 "ld_script_parser.c"
    break;

  case 63: /* min_function: T_MIN '(' expression ',' expression ')'  */
#line 464 "ld_script_parser.y"
                                                  {
		(yyval.exp) = ld_exp_binary(ld, LEOP_MIN, (yyvsp[-3].exp), (yyvsp[-1].exp));
	}
#line 2298 "ld_script_parser.c"
    break;

  case 64: /* next_function: T_NEXT '(' expression ')'  */
#line 470 "ld_script_parser.y"
                                    {
		(yyval.exp) = ld_exp_unary(ld, LEOP_NEXT, (yyvsp[-1].exp));
	}
#line 2306 "ld_script_parser.c"
    break;

  case 65: /* origin_function: T_ORIGIN '(' ident ')'  */
#line 476 "ld_script_parser.y"
                                 {
		(yyval.exp) = ld_exp_unary(ld, LEOP_ORIGIN, ld_exp_name(ld, (yyvsp[-1].str)));
	}
#line 2314 "ld_script_parser.c"
    break;

  case 66: /* segment_start_function: T_SEGMENT_START '(' ident ',' expression ')'  */
#line 482 "ld_script_parser.y"
                                                       {
		(yyval.exp) = ld_exp_binary(ld, LEOP_MIN, ld_exp_name(ld, (yyvsp[-3].str)), (yyvsp[-1].exp));
	}
#line 2322 "ld_script_parser.c"
    break;

  case 67: /* sizeof_function: T_SIZEOF '(' ident ')'  */
#line 488 "ld_script_parser.y"
                                 {
		(yyval.exp) = ld_exp_unary(ld, LEOP_SIZEOF, ld_exp_name(ld, (yyvsp[-1].str)));
	}
#line 2330 "ld_script_parser.c"
    break;

  case 68: /* sizeof_headers_function: T_SIZEOF_HEADERS  */
#line 494 "ld_script_parser.y"
                           {
		(yyval.exp) = ld_exp_sizeof_headers(ld);
	}
#line 2338 "ld_script_parser.c"
    break;

  case 69: /* constant: T_NUM  */
#line 500 "ld_script_parser.y"
                {
		(yyval.exp) = ld_exp_constant(ld, (yyvsp[0].num));
	}
#line 2346 "ld_script_parser.c"
    break;

  case 70: /* constant: symbolic_constant  */
#line 503 "ld_script_parser.y"
                            {
		(yyval.exp) = ld_exp_symbolic_constant(ld, (yyvsp[0].str));
	}
#line 2354 "ld_script_parser.c"
    break;

  case 71: /* symbolic_constant: T_CONSTANT '(' T_COMMONPAGESIZE ')'  */
#line 509 "ld_script_parser.y"
                                              { (yyval.str) = (yyvsp[-1].str); }
#line 2360 "ld_script_parser.c"
    break;

  case 72: /* symbolic_constant: T_CONSTANT '(' T_MAXPAGESIZE ')'  */
#line 510 "ld_script_parser.y"
                                           { (yyval.str) = (yyvsp[-1].str); }
#line 2366 "ld_script_parser.c"
    break;

  case 74: /* ldscript_command: assignment  */
#line 515 "ld_script_parser.y"
                     {
		if (*(yyvsp[0].assign)->lda_var->le_name == '.')
			ld_fatal(ld, "variable . can only be used inside"
			    " SECTIONS command");
		(yyval.cmd) = ld_script_cmd(ld, LSC_ASSIGN, (yyvsp[0].assign));
	}
#line 2377 "ld_script_parser.c"
    break;

  case 76: /* ldscript_command: extern_command  */
#line 522 "ld_script_parser.y"
                         { (yyval.cmd) = NULL; }
#line 2383 "ld_script_parser.c"
    break;

  case 77: /* ldscript_command: force_common_allocation_command  */
#line 523 "ld_script_parser.y"
                                          { (yyval.cmd) = NULL; }
#line 2389 "ld_script_parser.c"
    break;

  case 78: /* ldscript_command: group_command  */
#line 524 "ld_script_parser.y"
                        { (yyval.cmd) = NULL; }
#line 2395 "ld_script_parser.c"
    break;

  case 79: /* ldscript_command: inhibit_common_allocation_command  */
#line 525 "ld_script_parser.y"
                                            { (yyval.cmd) = NULL; }
#line 2401 "ld_script_parser.c"
    break;

  case 80: /* ldscript_command: input_command  */
#line 526 "ld_script_parser.y"
                        { (yyval.cmd) = NULL; }
#line 2407 "ld_script_parser.c"
    break;

  case 81: /* ldscript_command: memory_command  */
#line 527 "ld_script_parser.y"
                         { (yyval.cmd) = NULL; }
#line 2413 "ld_script_parser.c"
    break;

  case 82: /* ldscript_command: nocrossrefs_command  */
#line 528 "ld_script_parser.y"
                              { (yyval.cmd) = NULL; }
#line 2419 "ld_script_parser.c"
    break;

  case 83: /* ldscript_command: output_command  */
#line 529 "ld_script_parser.y"
                         { (yyval.cmd) = NULL; }
#line 2425 "ld_script_parser.c"
    break;

  case 84: /* ldscript_command: output_arch_command  */
#line 530 "ld_script_parser.y"
                              { (yyval.cmd) = NULL; }
#line 2431 "ld_script_parser.c"
    break;

  case 85: /* ldscript_command: output_format_command  */
#line 531 "ld_script_parser.y"
                                { (yyval.cmd) = NULL; }
#line 2437 "ld_script_parser.c"
    break;

  case 86: /* ldscript_command: phdrs_command  */
#line 532 "ld_script_parser.y"
                        { (yyval.cmd) = NULL; }
#line 2443 "ld_script_parser.c"
    break;

  case 87: /* ldscript_command: region_alias_command  */
#line 533 "ld_script_parser.y"
                               { (yyval.cmd) = NULL; }
#line 2449 "ld_script_parser.c"
    break;

  case 88: /* ldscript_command: search_dir_command  */
#line 534 "ld_script_parser.y"
                             { (yyval.cmd) = NULL; }
#line 2455 "ld_script_parser.c"
    break;

  case 90: /* ldscript_command: startup_command  */
#line 536 "ld_script_parser.y"
                          { (yyval.cmd) = NULL; }
#line 2461 "ld_script_parser.c"
    break;

  case 91: /* ldscript_command: target_command  */
#line 537 "ld_script_parser.y"
                         { (yyval.cmd) = NULL; }
#line 2467 "ld_script_parser.c"
    break;

  case 92: /* ldscript_command: version_script_node  */
#line 538 "ld_script_parser.y"
                              { (yyval.cmd) = NULL; }
#line 2473 "ld_script_parser.c"
    break;

  case 93: /* ldscript_command: ';'  */
#line 539 "ld_script_parser.y"
              { (yyval.cmd) = NULL; }
#line 2479 "ld_script_parser.c"
    break;

  case 97: /* simple_assignment: variable assign_op expression  */
#line 549 "ld_script_parser.y"
                                                  {
		(yyval.assign) = ld_script_assign(ld, (yyvsp[-2].exp), (yyvsp[-1].num), (yyvsp[0].exp), 0, 0);
	}
#line 2487 "ld_script_parser.c"
    break;

  case 98: /* provide_assignment: T_PROVIDE '(' variable '=' expression ')'  */
#line 555 "ld_script_parser.y"
                                                    {
		(yyval.assign) = ld_script_assign(ld, (yyvsp[-3].exp), LSAOP_E, (yyvsp[-1].exp), 1, 0);
	}
#line 2495 "ld_script_parser.c"
    break;

  case 99: /* provide_hidden_assignment: T_PROVIDE_HIDDEN '(' variable '=' expression ')'  */
#line 561 "ld_script_parser.y"
                                                           {
		(yyval.assign) = ld_script_assign(ld, (yyvsp[-3].exp), LSAOP_E, (yyvsp[-1].exp), 1, 1);
	}
#line 2503 "ld_script_parser.c"
    break;

  case 100: /* assign_op: T_LSHIFT_E  */
#line 567 "ld_script_parser.y"
                     { (yyval.num) = LSAOP_LSHIFT_E; }
#line 2509 "ld_script_parser.c"
    break;

  case 101: /* assign_op: T_RSHIFT_E  */
#line 568 "ld_script_parser.y"
                     { (yyval.num) = LSAOP_RSHIFT_E; }
#line 2515 "ld_script_parser.c"
    break;

  case 102: /* assign_op: T_ADD_E  */
#line 569 "ld_script_parser.y"
                  { (yyval.num) = LSAOP_ADD_E; }
#line 2521 "ld_script_parser.c"
    break;

  case 103: /* assign_op: T_SUB_E  */
#line 570 "ld_script_parser.y"
                  { (yyval.num) = LSAOP_SUB_E; }
#line 2527 "ld_script_parser.c"
    break;

  case 104: /* assign_op: T_MUL_E  */
#line 571 "ld_script_parser.y"
                  { (yyval.num) = LSAOP_MUL_E; }
#line 2533 "ld_script_parser.c"
    break;

  case 105: /* assign_op: T_DIV_E  */
#line 572 "ld_script_parser.y"
                  { (yyval.num) = LSAOP_DIV_E; }
#line 2539 "ld_script_parser.c"
    break;

  case 106: /* assign_op: T_AND_E  */
#line 573 "ld_script_parser.y"
                  { (yyval.num) = LSAOP_AND_E; }
#line 2545 "ld_script_parser.c"
    break;

  case 107: /* assign_op: T_OR_E  */
#line 574 "ld_script_parser.y"
                 { (yyval.num) = LSAOP_OR_E; }
#line 2551 "ld_script_parser.c"
    break;

  case 108: /* assign_op: '='  */
#line 575 "ld_script_parser.y"
              { (yyval.num) = LSAOP_E; }
#line 2557 "ld_script_parser.c"
    break;

  case 109: /* assert_command: T_ASSERT '(' expression ',' T_STRING ')'  */
#line 579 "ld_script_parser.y"
                                                   {
		(yyval.cmd) = ld_script_assert(ld, (yyvsp[-3].exp), (yyvsp[-1].str));
	}
#line 2565 "ld_script_parser.c"
    break;

  case 110: /* entry_command: T_ENTRY '(' ident ')'  */
#line 585 "ld_script_parser.y"
                                {
		(yyval.cmd) = ld_script_cmd(ld, LSC_ENTRY, (yyvsp[-1].str));
	}
#line 2573 "ld_script_parser.c"
    break;

  case 111: /* extern_command: T_EXTERN '(' ident_list_nosep ')'  */
#line 591 "ld_script_parser.y"
                                            { ld_script_extern(ld, (yyvsp[-1].list)); }
#line 2579 "ld_script_parser.c"
    break;

  case 112: /* force_common_allocation_command: T_FORCE_COMMON_ALLOCATION  */
#line 595 "ld_script_parser.y"
                                    { ld->ld_common_alloc = 1; }
#line 2585 "ld_script_parser.c"
    break;

  case 113: /* group_command: T_GROUP '(' input_file_list ')'  */
#line 599 "ld_script_parser.y"
                                          {
		 ld_script_group(ld, ld_script_list_reverse((yyvsp[-1].list)));
	}
#line 2593 "ld_script_parser.c"
    break;

  case 114: /* inhibit_common_allocation_command: T_INHIBIT_COMMON_ALLOCATION  */
#line 605 "ld_script_parser.y"
                                      { ld->ld_common_no_alloc = 1; }
#line 2599 "ld_script_parser.c"
    break;

  case 115: /* input_command: T_INPUT '(' input_file_list ')'  */
#line 609 "ld_script_parser.y"
                                          {
		ld_script_input(ld, ld_script_list_reverse((yyvsp[-1].list)));
	}
#line 2607 "ld_script_parser.c"
    break;

  case 117: /* memory_region_list: memory_region  */
#line 619 "ld_script_parser.y"
                        {
		STAILQ_INSERT_TAIL(&ld->ld_scp->lds_r, (yyvsp[0].region), ldsr_next);
	}
#line 2615 "ld_script_parser.c"
    break;

  case 118: /* memory_region_list: memory_region_list memory_region  */
#line 622 "ld_script_parser.y"
                                           {
		STAILQ_INSERT_TAIL(&ld->ld_scp->lds_r, (yyvsp[0].region), ldsr_next);
	}
#line 2623 "ld_script_parser.c"
    break;

  case 119: /* memory_region: ident memory_attr ':' T_ORIGIN '=' expression ',' T_LENGTH '=' expression  */
#line 629 "ld_script_parser.y"
                   {
		ld_script_region(ld, (yyvsp[-9].str), (yyvsp[-8].str), (yyvsp[-4].exp), (yyvsp[0].exp));
	}
#line 2631 "ld_script_parser.c"
    break;

  case 121: /* memory_attr: %empty  */
#line 636 "ld_script_parser.y"
          { (yyval.str) = NULL; }
#line 2637 "ld_script_parser.c"
    break;

  case 122: /* nocrossrefs_command: T_NOCROSSREFS '(' ident_list_nosep ')'  */
#line 640 "ld_script_parser.y"
                                                 {
		ld_script_nocrossrefs(ld, (yyvsp[-1].list));
	}
#line 2645 "ld_script_parser.c"
    break;

  case 123: /* output_command: T_OUTPUT '(' ident ')'  */
#line 646 "ld_script_parser.y"
                                 {
		if (ld->ld_output == NULL)
			ld->ld_output_file = (yyvsp[-1].str);
		else
			free((yyvsp[-1].str));
	}
#line 2656 "ld_script_parser.c"
    break;

  case 124: /* output_arch_command: T_OUTPUT_ARCH '(' ident ')'  */
#line 655 "ld_script_parser.y"
                                      {
		ld_arch_set(ld, (yyvsp[-1].str));
		free((yyvsp[-1].str));
	}
#line 2665 "ld_script_parser.c"
    break;

  case 125: /* output_format_command: T_OUTPUT_FORMAT '(' ident ')'  */
#line 662 "ld_script_parser.y"
                                        {
		ld_output_format(ld, (yyvsp[-1].str), (yyvsp[-1].str), (yyvsp[-1].str));
	}
#line 2673 "ld_script_parser.c"
    break;

  case 126: /* output_format_command: T_OUTPUT_FORMAT '(' ident ',' ident ',' ident ')'  */
#line 665 "ld_script_parser.y"
                                                            {
		ld_output_format(ld, (yyvsp[-5].str), (yyvsp[-3].str), (yyvsp[-1].str));
	}
#line 2681 "ld_script_parser.c"
    break;

  case 128: /* phdr_list: phdr  */
#line 675 "ld_script_parser.y"
               {
		STAILQ_INSERT_TAIL(&ld->ld_scp->lds_p, (yyvsp[0].phdr), ldsp_next);
	}
#line 2689 "ld_script_parser.c"
    break;

  case 129: /* phdr_list: phdr_list phdr  */
#line 678 "ld_script_parser.y"
                         {
		STAILQ_INSERT_TAIL(&ld->ld_scp->lds_p, (yyvsp[0].phdr), ldsp_next);
	}
#line 2697 "ld_script_parser.c"
    break;

  case 130: /* phdr: ident ident phdr_filehdr phdr_phdrs phdr_at phdr_flags ';'  */
#line 683 "ld_script_parser.y"
                                                                     {
		(yyval.phdr) = ld_script_phdr(ld, (yyvsp[-6].str), (yyvsp[-5].str), (yyvsp[-4].num), (yyvsp[-3].num), (yyvsp[-2].exp), (yyvsp[-1].num));
	}
#line 2705 "ld_script_parser.c"
    break;

  case 131: /* phdr_filehdr: T_FILEHDR  */
#line 689 "ld_script_parser.y"
                    { (yyval.num) = 1; }
#line 2711 "ld_script_parser.c"
    break;

  case 132: /* phdr_filehdr: %empty  */
#line 690 "ld_script_parser.y"
          { (yyval.num) = 0; }
#line 2717 "ld_script_parser.c"
    break;

  case 133: /* phdr_phdrs: T_PHDRS  */
#line 694 "ld_script_parser.y"
                  { (yyval.num) = 1; }
#line 2723 "ld_script_parser.c"
    break;

  case 134: /* phdr_phdrs: %empty  */
#line 695 "ld_script_parser.y"
          { (yyval.num) = 0; }
#line 2729 "ld_script_parser.c"
    break;

  case 135: /* phdr_at: T_AT '(' expression ')'  */
#line 699 "ld_script_parser.y"
                                  { (yyval.exp) = (yyvsp[-1].exp); }
#line 2735 "ld_script_parser.c"
    break;

  case 136: /* phdr_at: %empty  */
#line 700 "ld_script_parser.y"
          { (yyval.exp) = NULL; }
#line 2741 "ld_script_parser.c"
    break;

  case 137: /* phdr_flags: T_FLAGS '(' T_NUM ')'  */
#line 704 "ld_script_parser.y"
                                { (yyval.num) = (yyvsp[-1].num); }
#line 2747 "ld_script_parser.c"
    break;

  case 138: /* phdr_flags: %empty  */
#line 705 "ld_script_parser.y"
          { (yyval.num) = 0; }
#line 2753 "ld_script_parser.c"
    break;

  case 139: /* region_alias_command: T_REGION_ALIAS '(' ident ',' ident ')'  */
#line 709 "ld_script_parser.y"
                                                 {
		ld_script_region_alias(ld, (yyvsp[-3].str), (yyvsp[-1].str));
	}
#line 2761 "ld_script_parser.c"
    break;

  case 140: /* search_dir_command: T_SEARCH_DIR '(' ident ')'  */
#line 715 "ld_script_parser.y"
                                     {
		ld_path_add(ld, (yyvsp[-1].str), LPT_L);
		free((yyvsp[-1].str));
	}
#line 2770 "ld_script_parser.c"
    break;

  case 141: /* sections_command: T_SECTIONS '{' sections_command_list '}'  */
#line 722 "ld_script_parser.y"
                                                   {
		struct ld_script_sections *ldss;
		ldss = malloc(sizeof(struct ld_script_sections));
		if (ldss == NULL)
			ld_fatal_std(ld, "malloc");
		memcpy(&ldss->ldss_c, &ldss_c, sizeof(ldss_c));
		(yyval.cmd) = ld_script_cmd(ld, LSC_SECTIONS, ldss);
		STAILQ_INIT(&ldss_c);
	}
#line 2784 "ld_script_parser.c"
    break;

  case 142: /* sections_command_list: sections_sub_command  */
#line 734 "ld_script_parser.y"
                               {
		if ((yyvsp[0].cmd) != NULL)
			ld_script_cmd_insert(&ldss_c, (yyvsp[0].cmd));
	}
#line 2793 "ld_script_parser.c"
    break;

  case 143: /* sections_command_list: sections_command_list sections_sub_command  */
#line 738 "ld_script_parser.y"
                                                     {
		if ((yyvsp[0].cmd) != NULL)
			ld_script_cmd_insert(&ldss_c, (yyvsp[0].cmd));
	}
#line 2802 "ld_script_parser.c"
    break;

  case 145: /* sections_sub_command: assignment  */
#line 746 "ld_script_parser.y"
                     {
		(yyval.cmd) = ld_script_cmd(ld, LSC_ASSIGN, (yyvsp[0].assign));
	}
#line 2810 "ld_script_parser.c"
    break;

  case 146: /* sections_sub_command: output_sections_desc  */
#line 749 "ld_script_parser.y"
                               {
		(yyval.cmd) = ld_script_cmd(ld, LSC_SECTIONS_OUTPUT, (yyvsp[0].output_desc));
	}
#line 2818 "ld_script_parser.c"
    break;

  case 147: /* sections_sub_command: overlay_desc  */
#line 752 "ld_script_parser.y"
                       {
		(yyval.cmd) = ld_script_cmd(ld, LSC_SECTIONS_OVERLAY, (yyvsp[0].overlay_desc));
	}
#line 2826 "ld_script_parser.c"
    break;

  case 148: /* sections_sub_command: ';'  */
#line 755 "ld_script_parser.y"
              { (yyval.cmd) = NULL; }
#line 2832 "ld_script_parser.c"
    break;

  case 149: /* $@1: %empty  */
#line 759 "ld_script_parser.y"
                                                 {
		/* Remember the name of last output section, needed later for assignment. */
		ld->ld_scp->lds_base_os_name = (yyvsp[-2].str);
	}
#line 2841 "ld_script_parser.c"
    break;

  case 150: /* output_sections_desc: ident output_section_addr_and_type ':' $@1 output_section_lma output_section_align output_section_subalign output_section_constraint '{' output_section_command_list '}' output_section_region output_section_lma_region output_section_phdr output_section_fillexp  */
#line 771 "ld_script_parser.y"
                               {
		(yyval.output_desc) = calloc(1, sizeof(struct ld_script_sections_output));
		if ((yyval.output_desc) == NULL)
			ld_fatal_std(ld, "calloc");
		(yyval.output_desc)->ldso_name = (yyvsp[-14].str);
		(yyval.output_desc)->ldso_vma = (yyvsp[-13].list)->ldl_entry;
		(yyval.output_desc)->ldso_type = (yyvsp[-13].list)->ldl_next->ldl_entry;
		(yyval.output_desc)->ldso_lma = (yyvsp[-10].exp);
		(yyval.output_desc)->ldso_align = (yyvsp[-9].exp);
		(yyval.output_desc)->ldso_subalign = (yyvsp[-8].exp);
		(yyval.output_desc)->ldso_constraint = (yyvsp[-7].str);
		memcpy(&(yyval.output_desc)->ldso_c, &ldso_c, sizeof(ldso_c));
		(yyval.output_desc)->ldso_region = (yyvsp[-3].str);
		(yyval.output_desc)->ldso_lma_region = (yyvsp[-2].str);
		(yyval.output_desc)->ldso_phdr = ld_script_list_reverse((yyvsp[-1].list));
		(yyval.output_desc)->ldso_fill = (yyvsp[0].exp);
		STAILQ_INIT(&ldso_c);
		ld->ld_scp->lds_base_os_name = 0;
		ld->ld_scp->lds_last_os_name = (yyvsp[-14].str);
	}
#line 2866 "ld_script_parser.c"
    break;

  case 151: /* output_section_addr_and_type: output_section_addr output_section_type  */
#line 794 "ld_script_parser.y"
                                                  {
		(yyval.list) = ld_script_list(ld, NULL, (yyvsp[0].str));
		(yyval.list) = ld_script_list(ld, (yyval.list), (yyvsp[-1].exp));
	}
#line 2875 "ld_script_parser.c"
    break;

  case 152: /* output_section_addr_and_type: output_section_type  */
#line 798 "ld_script_parser.y"
                              {
		(yyval.list) = ld_script_list(ld, NULL, NULL);
		(yyval.list) = ld_script_list(ld, (yyval.list), (yyvsp[0].str));
	}
#line 2884 "ld_script_parser.c"
    break;

  case 154: /* output_section_type: '(' output_section_type_keyword ')'  */
#line 809 "ld_script_parser.y"
                                              { (yyval.str) = (yyvsp[-1].str); }
#line 2890 "ld_script_parser.c"
    break;

  case 155: /* output_section_type: '(' ')'  */
#line 810 "ld_script_parser.y"
                  { (yyval.str) = NULL; }
#line 2896 "ld_script_parser.c"
    break;

  case 156: /* output_section_type: %empty  */
#line 811 "ld_script_parser.y"
          { (yyval.str) = NULL; }
#line 2902 "ld_script_parser.c"
    break;

  case 162: /* output_section_lma: T_AT '(' expression ')'  */
#line 823 "ld_script_parser.y"
                                  { (yyval.exp) = (yyvsp[-1].exp); }
#line 2908 "ld_script_parser.c"
    break;

  case 163: /* output_section_lma: %empty  */
#line 824 "ld_script_parser.y"
          { (yyval.exp) = NULL; }
#line 2914 "ld_script_parser.c"
    break;

  case 164: /* output_section_align: T_ALIGN '(' expression ')'  */
#line 828 "ld_script_parser.y"
                                     { (yyval.exp) = (yyvsp[-1].exp); }
#line 2920 "ld_script_parser.c"
    break;

  case 165: /* output_section_align: %empty  */
#line 829 "ld_script_parser.y"
          { (yyval.exp) = NULL; }
#line 2926 "ld_script_parser.c"
    break;

  case 166: /* output_section_subalign: T_SUBALIGN '(' expression ')'  */
#line 833 "ld_script_parser.y"
                                        { (yyval.exp) = (yyvsp[-1].exp); }
#line 2932 "ld_script_parser.c"
    break;

  case 167: /* output_section_subalign: %empty  */
#line 834 "ld_script_parser.y"
          { (yyval.exp) = NULL; }
#line 2938 "ld_script_parser.c"
    break;

  case 170: /* output_section_constraint: %empty  */
#line 840 "ld_script_parser.y"
          { (yyval.str) = NULL; }
#line 2944 "ld_script_parser.c"
    break;

  case 171: /* output_section_command_list: output_section_command  */
#line 844 "ld_script_parser.y"
                                 {
		if ((yyvsp[0].cmd) != NULL)
			ld_script_cmd_insert(&ldso_c, (yyvsp[0].cmd));
	}
#line 2953 "ld_script_parser.c"
    break;

  case 172: /* output_section_command_list: output_section_command_list output_section_command  */
#line 848 "ld_script_parser.y"
                                                             {
		if ((yyvsp[0].cmd) != NULL)
			ld_script_cmd_insert(&ldso_c, (yyvsp[0].cmd));
	}
#line 2962 "ld_script_parser.c"
    break;

  case 173: /* output_section_command: assignment  */
#line 855 "ld_script_parser.y"
                     {
		(yyval.cmd) = ld_script_cmd(ld, LSC_ASSIGN, (yyvsp[0].assign));
	}
#line 2970 "ld_script_parser.c"
    break;

  case 174: /* output_section_command: input_section_desc  */
#line 858 "ld_script_parser.y"
                             {
		(yyval.cmd) = ld_script_cmd(ld, LSC_SECTIONS_OUTPUT_INPUT, (yyvsp[0].input_section));
	}
#line 2978 "ld_script_parser.c"
    break;

  case 175: /* output_section_command: output_section_data  */
#line 861 "ld_script_parser.y"
                              {
		(yyval.cmd) = ld_script_cmd(ld, LSC_SECTIONS_OUTPUT_DATA, (yyvsp[0].output_data));
	}
#line 2986 "ld_script_parser.c"
    break;

  case 176: /* output_section_command: output_section_keywords  */
#line 864 "ld_script_parser.y"
                                  {
		(yyval.cmd) = ld_script_cmd(ld, LSC_SECTIONS_OUTPUT_KEYWORD,
		    (void *) (uintptr_t) (yyvsp[0].num));
	}
#line 2995 "ld_script_parser.c"
    break;

  case 177: /* output_section_command: ';'  */
#line 868 "ld_script_parser.y"
              { (yyval.cmd) = NULL; }
#line 3001 "ld_script_parser.c"
    break;

  case 178: /* input_section_desc: input_section_desc_no_keep  */
#line 872 "ld_script_parser.y"
                                     {
		(yyvsp[0].input_section)->ldoi_keep = 0;
		(yyval.input_section) = (yyvsp[0].input_section);
	}
#line 3010 "ld_script_parser.c"
    break;

  case 179: /* input_section_desc: T_KEEP '(' input_section_desc_no_keep ')'  */
#line 876 "ld_script_parser.y"
                                                    {
		(yyvsp[-1].input_section)->ldoi_keep = 0;
		(yyval.input_section) = (yyvsp[-1].input_section);
	}
#line 3019 "ld_script_parser.c"
    break;

  case 180: /* input_section_desc_no_keep: wildcard_sort input_section  */
#line 883 "ld_script_parser.y"
                                      {
		(yyvsp[0].input_section)->ldoi_ar = NULL;
		(yyvsp[0].input_section)->ldoi_file = (yyvsp[-1].wildcard);
		(yyval.input_section) = (yyvsp[0].input_section);
	}
#line 3029 "ld_script_parser.c"
    break;

  case 181: /* input_section_desc_no_keep: wildcard_sort ':' wildcard_sort input_section  */
#line 888 "ld_script_parser.y"
                                                        {
		(yyvsp[0].input_section)->ldoi_ar = (yyvsp[-3].wildcard);
		(yyvsp[0].input_section)->ldoi_ar = (yyvsp[-1].wildcard);
		(yyval.input_section) = (yyvsp[0].input_section);
	}
#line 3039 "ld_script_parser.c"
    break;

  case 182: /* input_section: '(' T_EXCLUDE_FILE '(' wildcard_list ')' wildcard_list ')'  */
#line 896 "ld_script_parser.y"
                                                                     {
		(yyval.input_section) = calloc(1, sizeof(struct ld_script_sections_output_input));
		if ((yyval.input_section) == NULL)
			ld_fatal_std(ld, "calloc");
		(yyval.input_section)->ldoi_exclude = ld_script_list_reverse((yyvsp[-3].list));
		(yyval.input_section)->ldoi_sec = ld_script_list_reverse((yyvsp[-1].list));
	}
#line 3051 "ld_script_parser.c"
    break;

  case 183: /* input_section: '(' wildcard_list ')'  */
#line 903 "ld_script_parser.y"
                                {
		(yyval.input_section) = calloc(1, sizeof(struct ld_script_sections_output_input));
		if ((yyval.input_section) == NULL)
			ld_fatal_std(ld, "calloc");
		(yyval.input_section)->ldoi_exclude = NULL;
		(yyval.input_section)->ldoi_sec = ld_script_list_reverse((yyvsp[-1].list));
	}
#line 3063 "ld_script_parser.c"
    break;

  case 184: /* output_section_data: data_type '(' expression ')'  */
#line 913 "ld_script_parser.y"
                                       {
		(yyval.output_data) = calloc(1, sizeof(struct ld_script_sections_output_data));
		if ((yyval.output_data) == NULL)
			ld_fatal_std(ld, "calloc");
		(yyval.output_data)->ldod_type = (yyvsp[-3].num);
		(yyval.output_data)->ldod_exp = (yyvsp[-1].exp);
	}
#line 3075 "ld_script_parser.c"
    break;

  case 185: /* data_type: T_BYTE  */
#line 923 "ld_script_parser.y"
                 { (yyval.num) = LSODT_BYTE; }
#line 3081 "ld_script_parser.c"
    break;

  case 186: /* data_type: T_SHORT  */
#line 924 "ld_script_parser.y"
                  { (yyval.num) = LSODT_SHORT; }
#line 3087 "ld_script_parser.c"
    break;

  case 187: /* data_type: T_LONG  */
#line 925 "ld_script_parser.y"
                 { (yyval.num) = LSODT_LONG; }
#line 3093 "ld_script_parser.c"
    break;

  case 188: /* data_type: T_QUAD  */
#line 926 "ld_script_parser.y"
                 { (yyval.num) = LSODT_QUAD; }
#line 3099 "ld_script_parser.c"
    break;

  case 189: /* data_type: T_SQUAD  */
#line 927 "ld_script_parser.y"
                  { (yyval.num) = LSODT_SQUAD; }
#line 3105 "ld_script_parser.c"
    break;

  case 190: /* data_type: T_FILL  */
#line 928 "ld_script_parser.y"
                 { (yyval.num) = LSODT_FILL; }
#line 3111 "ld_script_parser.c"
    break;

  case 191: /* output_section_keywords: T_CREATE_OBJECT_SYMBOLS  */
#line 932 "ld_script_parser.y"
                                  {
		(yyval.num) = LSOK_CREATE_OBJECT_SYMBOLS;
	}
#line 3119 "ld_script_parser.c"
    break;

  case 192: /* output_section_keywords: T_CONSTRUCTORS  */
#line 935 "ld_script_parser.y"
                         {
		(yyval.num) = LSOK_CONSTRUCTORS;
	}
#line 3127 "ld_script_parser.c"
    break;

  case 193: /* output_section_keywords: T_SORT_BY_NAME '(' T_CONSTRUCTORS ')'  */
#line 938 "ld_script_parser.y"
                                                {
		(yyval.num) = LSOK_CONSTRUCTORS_SORT_BY_NAME;
	}
#line 3135 "ld_script_parser.c"
    break;

  case 194: /* output_section_region: '>' ident  */
#line 944 "ld_script_parser.y"
                    { (yyval.str) = (yyvsp[0].str); }
#line 3141 "ld_script_parser.c"
    break;

  case 195: /* output_section_region: %empty  */
#line 945 "ld_script_parser.y"
          { (yyval.str) = NULL; }
#line 3147 "ld_script_parser.c"
    break;

  case 196: /* output_section_lma_region: T_AT '>' ident  */
#line 949 "ld_script_parser.y"
                         { (yyval.str) = (yyvsp[0].str); }
#line 3153 "ld_script_parser.c"
    break;

  case 197: /* output_section_lma_region: %empty  */
#line 950 "ld_script_parser.y"
          { (yyval.str) = NULL; }
#line 3159 "ld_script_parser.c"
    break;

  case 198: /* output_section_phdr: output_section_phdr ':' ident  */
#line 954 "ld_script_parser.y"
                                        {
		(yyval.list) = ld_script_list(ld, (yyval.list), (yyvsp[0].str));
	}
#line 3167 "ld_script_parser.c"
    break;

  case 199: /* output_section_phdr: %empty  */
#line 957 "ld_script_parser.y"
          { (yyval.list) = NULL; }
#line 3173 "ld_script_parser.c"
    break;

  case 200: /* output_section_fillexp: '=' expression  */
#line 962 "ld_script_parser.y"
                         { (yyval.exp) = (yyvsp[0].exp); }
#line 3179 "ld_script_parser.c"
    break;

  case 201: /* output_section_fillexp: %empty  */
#line 963 "ld_script_parser.y"
          { (yyval.exp) = NULL; }
#line 3185 "ld_script_parser.c"
    break;

  case 202: /* overlay_desc: T_OVERLAY overlay_vma ':' overlay_nocref output_section_lma '{' overlay_section_list '}' output_section_region output_section_phdr output_section_fillexp  */
#line 974 "ld_script_parser.y"
                               {
		(yyval.overlay_desc) = calloc(1, sizeof(struct ld_script_sections_overlay));
		if ((yyval.overlay_desc) == NULL)
			ld_fatal_std(ld, "calloc");
		(yyval.overlay_desc)->ldso_vma = (yyvsp[-9].exp);
		(yyval.overlay_desc)->ldso_nocrossref = !!(yyvsp[-7].num);
		(yyval.overlay_desc)->ldso_lma = (yyvsp[-6].exp);
		(yyval.overlay_desc)->ldso_s = (yyvsp[-4].list);
		(yyval.overlay_desc)->ldso_region = (yyvsp[-2].str);
		(yyval.overlay_desc)->ldso_phdr = (yyvsp[-1].list);
		(yyval.overlay_desc)->ldso_fill = (yyvsp[0].exp);
	}
#line 3202 "ld_script_parser.c"
    break;

  case 204: /* overlay_vma: %empty  */
#line 990 "ld_script_parser.y"
          { (yyval.exp) = NULL; }
#line 3208 "ld_script_parser.c"
    break;

  case 205: /* overlay_nocref: T_NOCROSSREFS  */
#line 994 "ld_script_parser.y"
                        { (yyval.num) = 1; }
#line 3214 "ld_script_parser.c"
    break;

  case 206: /* overlay_nocref: %empty  */
#line 995 "ld_script_parser.y"
          { (yyval.num) = 0; }
#line 3220 "ld_script_parser.c"
    break;

  case 207: /* overlay_section_list: overlay_section  */
#line 999 "ld_script_parser.y"
                          {
		(yyval.list) = ld_script_list(ld, NULL, (yyvsp[0].overlay_section));
	}
#line 3228 "ld_script_parser.c"
    break;

  case 208: /* overlay_section_list: overlay_section_list overlay_section  */
#line 1002 "ld_script_parser.y"
                                               {
		(yyval.list) = ld_script_list(ld, (yyvsp[-1].list), (yyvsp[0].overlay_section));
	}
#line 3236 "ld_script_parser.c"
    break;

  case 209: /* overlay_section: ident '{' output_section_command_list '}' output_section_phdr output_section_fillexp  */
#line 1011 "ld_script_parser.y"
                               {
		(yyval.overlay_section) = calloc(1,
		    sizeof(struct ld_script_sections_overlay_section));
		if ((yyval.overlay_section) == NULL)
			ld_fatal_std(ld, "calloc");
		(yyval.overlay_section)->ldos_name = (yyvsp[-5].str);
		memcpy(&(yyval.overlay_section)->ldos_c, &ldso_c, sizeof(ldso_c));
		(yyval.overlay_section)->ldos_phdr = (yyvsp[-1].list);
		(yyval.overlay_section)->ldos_fill = (yyvsp[0].exp);
		STAILQ_INIT(&ldso_c);
	}
#line 3252 "ld_script_parser.c"
    break;

  case 210: /* startup_command: T_STARTUP '(' ident ')'  */
#line 1025 "ld_script_parser.y"
                                  {
		ld_file_add_first(ld, (yyvsp[-1].str), LFT_UNKNOWN);
		free((yyvsp[-1].str));
	}
#line 3261 "ld_script_parser.c"
    break;

  case 212: /* version_script_node: ident extern_block version_dependency ';'  */
#line 1036 "ld_script_parser.y"
                                                    {
		ld_script_version_add_node(ld, (yyvsp[-3].str), (yyvsp[-2].version_entry_head), (yyvsp[-1].str));
	}
#line 3269 "ld_script_parser.c"
    break;

  case 213: /* version_script_node: ident version_block version_dependency ';'  */
#line 1039 "ld_script_parser.y"
                                                     {
		ld_script_version_add_node(ld, (yyvsp[-3].str), (yyvsp[-2].version_entry_head), (yyvsp[-1].str));
	}
#line 3277 "ld_script_parser.c"
    break;

  case 214: /* version_script_node: extern_block version_dependency ';'  */
#line 1042 "ld_script_parser.y"
                                              {
		ld_script_version_add_node(ld, NULL, (yyvsp[-2].version_entry_head), (yyvsp[-1].str));
	}
#line 3285 "ld_script_parser.c"
    break;

  case 215: /* version_script_node: version_block version_dependency ';'  */
#line 1045 "ld_script_parser.y"
                                               {
		ld_script_version_add_node(ld, NULL, (yyvsp[-2].version_entry_head), (yyvsp[-1].str));
	}
#line 3293 "ld_script_parser.c"
    break;

  case 216: /* extern_block: T_VER_EXTERN T_STRING version_block  */
#line 1051 "ld_script_parser.y"
                                              {
		ld_script_version_set_lang(ld, (yyvsp[0].version_entry_head), (yyvsp[-1].str));
		(yyval.version_entry_head) = (yyvsp[0].version_entry_head);
	}
#line 3302 "ld_script_parser.c"
    break;

  case 217: /* version_block: '{' version_entry_list '}'  */
#line 1058 "ld_script_parser.y"
                                     {
		(yyval.version_entry_head) = (yyvsp[-1].version_entry_head);
		ld->ld_state.ls_version_local = 0;
	}
#line 3311 "ld_script_parser.c"
    break;

  case 218: /* version_entry_list: version_entry  */
#line 1065 "ld_script_parser.y"
                        {
		(yyval.version_entry_head) = ld_script_version_link_entry(ld, NULL, (yyvsp[0].version_entry));
	}
#line 3319 "ld_script_parser.c"
    break;

  case 219: /* version_entry_list: version_entry_list version_entry  */
#line 1068 "ld_script_parser.y"
                                           {
		(yyval.version_entry_head) = ld_script_version_link_entry(ld, (yyvsp[-1].version_entry_head), (yyvsp[0].version_entry));
	}
#line 3327 "ld_script_parser.c"
    break;

  case 220: /* version_entry: T_VER_GLOBAL  */
#line 1074 "ld_script_parser.y"
                       {
		ld->ld_state.ls_version_local = 0;
		(yyval.version_entry) = NULL;
	}
#line 3336 "ld_script_parser.c"
    break;

  case 221: /* version_entry: T_VER_LOCAL  */
#line 1078 "ld_script_parser.y"
                      {
		ld->ld_state.ls_version_local = 1;
		(yyval.version_entry) = NULL;
	}
#line 3345 "ld_script_parser.c"
    break;

  case 222: /* version_entry: wildcard ';'  */
#line 1082 "ld_script_parser.y"
                       {
		(yyval.version_entry) = ld_script_version_alloc_entry(ld, (yyvsp[-1].str), NULL);
	}
#line 3353 "ld_script_parser.c"
    break;

  case 223: /* version_entry: extern_block ';'  */
#line 1085 "ld_script_parser.y"
                           {
		(yyval.version_entry) = ld_script_version_alloc_entry(ld, NULL, (yyvsp[-1].version_entry_head));
	}
#line 3361 "ld_script_parser.c"
    break;

  case 225: /* version_dependency: %empty  */
#line 1092 "ld_script_parser.y"
          { (yyval.str) = NULL; }
#line 3367 "ld_script_parser.c"
    break;

  case 228: /* variable: ident  */
#line 1101 "ld_script_parser.y"
                { (yyval.exp) = ld_exp_symbol(ld, (yyvsp[0].str)); }
#line 3373 "ld_script_parser.c"
    break;

  case 229: /* variable: '.'  */
#line 1102 "ld_script_parser.y"
               { (yyval.exp) = ld_exp_symbol(ld, "."); }
#line 3379 "ld_script_parser.c"
    break;

  case 232: /* wildcard: '*'  */
#line 1108 "ld_script_parser.y"
              { (yyval.str) = strdup("*"); }
#line 3385 "ld_script_parser.c"
    break;

  case 233: /* wildcard: '?'  */
#line 1109 "ld_script_parser.y"
              { (yyval.str) = strdup("?"); }
#line 3391 "ld_script_parser.c"
    break;

  case 234: /* wildcard_sort: wildcard  */
#line 1113 "ld_script_parser.y"
                   {
		(yyval.wildcard) = ld_wildcard_alloc(ld);
		(yyval.wildcard)->lw_name = (yyvsp[0].str);
		(yyval.wildcard)->lw_sort = LWS_NONE;
	}
#line 3401 "ld_script_parser.c"
    break;

  case 235: /* wildcard_sort: T_SORT_BY_NAME '(' wildcard ')'  */
#line 1118 "ld_script_parser.y"
                                          {
		(yyval.wildcard) = ld_wildcard_alloc(ld);
		(yyval.wildcard)->lw_name = (yyvsp[-1].str);
		(yyval.wildcard)->lw_sort = LWS_NAME;
	}
#line 3411 "ld_script_parser.c"
    break;

  case 236: /* wildcard_sort: T_SORT_BY_NAME '(' T_SORT_BY_NAME '(' wildcard ')' ')'  */
#line 1123 "ld_script_parser.y"
                                                                 {
		(yyval.wildcard) = ld_wildcard_alloc(ld);
		(yyval.wildcard)->lw_name = (yyvsp[-2].str);
		(yyval.wildcard)->lw_sort = LWS_NAME;
	}
#line 3421 "ld_script_parser.c"
    break;

  case 237: /* wildcard_sort: T_SORT_BY_NAME '(' T_SORT_BY_ALIGNMENT '(' wildcard ')' ')'  */
#line 1128 "ld_script_parser.y"
                                                                      {
		(yyval.wildcard) = ld_wildcard_alloc(ld);
		(yyval.wildcard)->lw_name = (yyvsp[-2].str);
		(yyval.wildcard)->lw_sort = LWS_NAME_ALIGN;
	}
#line 3431 "ld_script_parser.c"
    break;

  case 238: /* wildcard_sort: T_SORT_BY_ALIGNMENT '(' wildcard ')'  */
#line 1133 "ld_script_parser.y"
                                               {
		(yyval.wildcard) = ld_wildcard_alloc(ld);
		(yyval.wildcard)->lw_name = (yyvsp[-1].str);
		(yyval.wildcard)->lw_sort = LWS_ALIGN;
	}
#line 3441 "ld_script_parser.c"
    break;

  case 239: /* wildcard_sort: T_SORT_BY_ALIGNMENT '(' T_SORT_BY_NAME '(' wildcard ')' ')'  */
#line 1138 "ld_script_parser.y"
                                                                      {
		(yyval.wildcard) = ld_wildcard_alloc(ld);
		(yyval.wildcard)->lw_name = (yyvsp[-2].str);
		(yyval.wildcard)->lw_sort = LWS_ALIGN_NAME;
	}
#line 3451 "ld_script_parser.c"
    break;

  case 240: /* wildcard_sort: T_SORT_BY_ALIGNMENT '(' T_SORT_BY_ALIGNMENT '(' wildcard ')' ')'  */
#line 1143 "ld_script_parser.y"
                                                                           {
		(yyval.wildcard) = ld_wildcard_alloc(ld);
		(yyval.wildcard)->lw_name = (yyvsp[-2].str);
		(yyval.wildcard)->lw_sort = LWS_ALIGN;
	}
#line 3461 "ld_script_parser.c"
    break;

  case 241: /* ident_list: ident  */
#line 1151 "ld_script_parser.y"
                { (yyval.list) = ld_script_list(ld, NULL, (yyvsp[0].str)); }
#line 3467 "ld_script_parser.c"
    break;

  case 242: /* ident_list: ident_list separator ident  */
#line 1152 "ld_script_parser.y"
                                     { (yyval.list) = ld_script_list(ld, (yyvsp[-2].list), (yyvsp[0].str)); }
#line 3473 "ld_script_parser.c"
    break;

  case 243: /* ident_list_nosep: ident  */
#line 1156 "ld_script_parser.y"
                { (yyval.list) = ld_script_list(ld, NULL, (yyvsp[0].str)); }
#line 3479 "ld_script_parser.c"
    break;

  case 244: /* ident_list_nosep: ident_list_nosep ident  */
#line 1157 "ld_script_parser.y"
                                 { (yyval.list) = ld_script_list(ld, (yyvsp[-1].list), (yyvsp[0].str)); }
#line 3485 "ld_script_parser.c"
    break;

  case 245: /* input_file_list: input_file  */
#line 1161 "ld_script_parser.y"
                     { (yyval.list) = ld_script_list(ld, NULL, (yyvsp[0].input_file)); }
#line 3491 "ld_script_parser.c"
    break;

  case 246: /* input_file_list: input_file_list separator input_file  */
#line 1162 "ld_script_parser.y"
                                               { (yyval.list) = ld_script_list(ld, (yyvsp[-2].list), (yyvsp[0].input_file)); }
#line 3497 "ld_script_parser.c"
    break;

  case 247: /* input_file: ident  */
#line 1166 "ld_script_parser.y"
                { (yyval.input_file) = ld_script_input_file(ld, 0, (yyvsp[0].str)); }
#line 3503 "ld_script_parser.c"
    break;

  case 248: /* input_file: as_needed_list  */
#line 1167 "ld_script_parser.y"
                         { (yyval.input_file) = ld_script_input_file(ld, 1, (yyvsp[0].list)); }
#line 3509 "ld_script_parser.c"
    break;

  case 249: /* as_needed_list: T_AS_NEEDED '(' ident_list ')'  */
#line 1171 "ld_script_parser.y"
                                         { (yyval.list) = (yyvsp[-1].list); }
#line 3515 "ld_script_parser.c"
    break;

  case 250: /* wildcard_list: wildcard_sort  */
#line 1175 "ld_script_parser.y"
                        { (yyval.list) = ld_script_list(ld, NULL, (yyvsp[0].wildcard)); }
#line 3521 "ld_script_parser.c"
    break;

  case 251: /* wildcard_list: wildcard_list wildcard_sort  */
#line 1176 "ld_script_parser.y"
                                      { (yyval.list) = ld_script_list(ld, (yyvsp[-1].list), (yyvsp[0].wildcard)); }
#line 3527 "ld_script_parser.c"
    break;


#line 3531 "ld_script_parser.c"

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

#line 1184 "ld_script_parser.y"


/* ARGSUSED */
static void
yyerror(const char *s)
{

	(void) s;
	errx(1, "Syntax error in ld script, line %d\n", lineno);
}

static void
_init_script(void)
{

	STAILQ_INIT(&ldss_c);
	STAILQ_INIT(&ldso_c);
}

void
ld_script_parse(const char *name)
{
	YY_BUFFER_STATE b;

	_init_script();

	if ((yyin = fopen(name, "r")) == NULL)
		ld_fatal_std(ld, "fopen %s name failed", name);
	b = yy_create_buffer(yyin, YY_BUF_SIZE);
	yy_switch_to_buffer(b);
	if (yyparse() < 0)
		ld_fatal(ld, "unable to parse linker script %s", name);
	yy_delete_buffer(b);
}

void
ld_script_parse_internal(void)
{
	YY_BUFFER_STATE b;

	_init_script();

	assert(ld->ld_arch != NULL && ld->ld_arch->script != NULL);
	b = yy_scan_string(ld->ld_arch->script);
	yy_switch_to_buffer(b);
	if (yyparse() < 0)
		ld_fatal(ld, "unable to parse internal linker script");
	yy_delete_buffer(b);
}
