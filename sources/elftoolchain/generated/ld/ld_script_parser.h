/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_LD_SCRIPT_PARSER_H_INCLUDED
# define YY_YY_LD_SCRIPT_PARSER_H_INCLUDED
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
    T_ABSOLUTE = 258,              /* T_ABSOLUTE  */
    T_ADDR = 259,                  /* T_ADDR  */
    T_ALIGN = 260,                 /* T_ALIGN  */
    T_ALIGNOF = 261,               /* T_ALIGNOF  */
    T_ASSERT = 262,                /* T_ASSERT  */
    T_AS_NEEDED = 263,             /* T_AS_NEEDED  */
    T_AT = 264,                    /* T_AT  */
    T_BIND = 265,                  /* T_BIND  */
    T_BLOCK = 266,                 /* T_BLOCK  */
    T_BYTE = 267,                  /* T_BYTE  */
    T_CONSTANT = 268,              /* T_CONSTANT  */
    T_CONSTRUCTORS = 269,          /* T_CONSTRUCTORS  */
    T_CREATE_OBJECT_SYMBOLS = 270, /* T_CREATE_OBJECT_SYMBOLS  */
    T_DATA_SEGMENT_ALIGN = 271,    /* T_DATA_SEGMENT_ALIGN  */
    T_DATA_SEGMENT_END = 272,      /* T_DATA_SEGMENT_END  */
    T_DATA_SEGMENT_RELRO_END = 273, /* T_DATA_SEGMENT_RELRO_END  */
    T_DEFINED = 274,               /* T_DEFINED  */
    T_ENTRY = 275,                 /* T_ENTRY  */
    T_EXCLUDE_FILE = 276,          /* T_EXCLUDE_FILE  */
    T_EXTERN = 277,                /* T_EXTERN  */
    T_FILEHDR = 278,               /* T_FILEHDR  */
    T_FILL = 279,                  /* T_FILL  */
    T_FLAGS = 280,                 /* T_FLAGS  */
    T_FLOAT = 281,                 /* T_FLOAT  */
    T_FORCE_COMMON_ALLOCATION = 282, /* T_FORCE_COMMON_ALLOCATION  */
    T_GROUP = 283,                 /* T_GROUP  */
    T_HLL = 284,                   /* T_HLL  */
    T_INCLUDE = 285,               /* T_INCLUDE  */
    T_INHIBIT_COMMON_ALLOCATION = 286, /* T_INHIBIT_COMMON_ALLOCATION  */
    T_INPUT = 287,                 /* T_INPUT  */
    T_KEEP = 288,                  /* T_KEEP  */
    T_LENGTH = 289,                /* T_LENGTH  */
    T_LOADADDR = 290,              /* T_LOADADDR  */
    T_LONG = 291,                  /* T_LONG  */
    T_MAP = 292,                   /* T_MAP  */
    T_MAX = 293,                   /* T_MAX  */
    T_MEMORY = 294,                /* T_MEMORY  */
    T_MIN = 295,                   /* T_MIN  */
    T_NEXT = 296,                  /* T_NEXT  */
    T_NOCROSSREFS = 297,           /* T_NOCROSSREFS  */
    T_NOFLOAT = 298,               /* T_NOFLOAT  */
    T_OPTION = 299,                /* T_OPTION  */
    T_ORIGIN = 300,                /* T_ORIGIN  */
    T_OUTPUT = 301,                /* T_OUTPUT  */
    T_OUTPUT_ARCH = 302,           /* T_OUTPUT_ARCH  */
    T_OUTPUT_FORMAT = 303,         /* T_OUTPUT_FORMAT  */
    T_PHDRS = 304,                 /* T_PHDRS  */
    T_PROVIDE = 305,               /* T_PROVIDE  */
    T_PROVIDE_HIDDEN = 306,        /* T_PROVIDE_HIDDEN  */
    T_QUAD = 307,                  /* T_QUAD  */
    T_REGION_ALIAS = 308,          /* T_REGION_ALIAS  */
    T_SEARCH_DIR = 309,            /* T_SEARCH_DIR  */
    T_SECTIONS = 310,              /* T_SECTIONS  */
    T_SEGMENT_START = 311,         /* T_SEGMENT_START  */
    T_SHORT = 312,                 /* T_SHORT  */
    T_SIZEOF = 313,                /* T_SIZEOF  */
    T_SIZEOF_HEADERS = 314,        /* T_SIZEOF_HEADERS  */
    T_SORT_BY_ALIGNMENT = 315,     /* T_SORT_BY_ALIGNMENT  */
    T_SORT_BY_NAME = 316,          /* T_SORT_BY_NAME  */
    T_SPECIAL = 317,               /* T_SPECIAL  */
    T_SQUAD = 318,                 /* T_SQUAD  */
    T_STARTUP = 319,               /* T_STARTUP  */
    T_SUBALIGN = 320,              /* T_SUBALIGN  */
    T_SYSLIB = 321,                /* T_SYSLIB  */
    T_TARGET = 322,                /* T_TARGET  */
    T_TRUNCATE = 323,              /* T_TRUNCATE  */
    T_VER_EXTERN = 324,            /* T_VER_EXTERN  */
    T_VER_GLOBAL = 325,            /* T_VER_GLOBAL  */
    T_VER_LOCAL = 326,             /* T_VER_LOCAL  */
    T_LSHIFT_E = 327,              /* T_LSHIFT_E  */
    T_RSHIFT_E = 328,              /* T_RSHIFT_E  */
    T_LSHIFT = 329,                /* T_LSHIFT  */
    T_RSHIFT = 330,                /* T_RSHIFT  */
    T_EQ = 331,                    /* T_EQ  */
    T_NE = 332,                    /* T_NE  */
    T_GE = 333,                    /* T_GE  */
    T_LE = 334,                    /* T_LE  */
    T_ADD_E = 335,                 /* T_ADD_E  */
    T_SUB_E = 336,                 /* T_SUB_E  */
    T_MUL_E = 337,                 /* T_MUL_E  */
    T_DIV_E = 338,                 /* T_DIV_E  */
    T_AND_E = 339,                 /* T_AND_E  */
    T_OR_E = 340,                  /* T_OR_E  */
    T_LOGICAL_AND = 341,           /* T_LOGICAL_AND  */
    T_LOGICAL_OR = 342,            /* T_LOGICAL_OR  */
    UNARY = 343,                   /* UNARY  */
    T_NUM = 344,                   /* T_NUM  */
    T_COMMONPAGESIZE = 345,        /* T_COMMONPAGESIZE  */
    T_COPY = 346,                  /* T_COPY  */
    T_DSECT = 347,                 /* T_DSECT  */
    T_IDENT = 348,                 /* T_IDENT  */
    T_INFO = 349,                  /* T_INFO  */
    T_MAXPAGESIZE = 350,           /* T_MAXPAGESIZE  */
    T_MEMORY_ATTR = 351,           /* T_MEMORY_ATTR  */
    T_NOLOAD = 352,                /* T_NOLOAD  */
    T_ONLY_IF_RO = 353,            /* T_ONLY_IF_RO  */
    T_ONLY_IF_RW = 354,            /* T_ONLY_IF_RW  */
    T_OVERLAY = 355,               /* T_OVERLAY  */
    T_STRING = 356,                /* T_STRING  */
    T_WILDCARD = 357               /* T_WILDCARD  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define T_ABSOLUTE 258
#define T_ADDR 259
#define T_ALIGN 260
#define T_ALIGNOF 261
#define T_ASSERT 262
#define T_AS_NEEDED 263
#define T_AT 264
#define T_BIND 265
#define T_BLOCK 266
#define T_BYTE 267
#define T_CONSTANT 268
#define T_CONSTRUCTORS 269
#define T_CREATE_OBJECT_SYMBOLS 270
#define T_DATA_SEGMENT_ALIGN 271
#define T_DATA_SEGMENT_END 272
#define T_DATA_SEGMENT_RELRO_END 273
#define T_DEFINED 274
#define T_ENTRY 275
#define T_EXCLUDE_FILE 276
#define T_EXTERN 277
#define T_FILEHDR 278
#define T_FILL 279
#define T_FLAGS 280
#define T_FLOAT 281
#define T_FORCE_COMMON_ALLOCATION 282
#define T_GROUP 283
#define T_HLL 284
#define T_INCLUDE 285
#define T_INHIBIT_COMMON_ALLOCATION 286
#define T_INPUT 287
#define T_KEEP 288
#define T_LENGTH 289
#define T_LOADADDR 290
#define T_LONG 291
#define T_MAP 292
#define T_MAX 293
#define T_MEMORY 294
#define T_MIN 295
#define T_NEXT 296
#define T_NOCROSSREFS 297
#define T_NOFLOAT 298
#define T_OPTION 299
#define T_ORIGIN 300
#define T_OUTPUT 301
#define T_OUTPUT_ARCH 302
#define T_OUTPUT_FORMAT 303
#define T_PHDRS 304
#define T_PROVIDE 305
#define T_PROVIDE_HIDDEN 306
#define T_QUAD 307
#define T_REGION_ALIAS 308
#define T_SEARCH_DIR 309
#define T_SECTIONS 310
#define T_SEGMENT_START 311
#define T_SHORT 312
#define T_SIZEOF 313
#define T_SIZEOF_HEADERS 314
#define T_SORT_BY_ALIGNMENT 315
#define T_SORT_BY_NAME 316
#define T_SPECIAL 317
#define T_SQUAD 318
#define T_STARTUP 319
#define T_SUBALIGN 320
#define T_SYSLIB 321
#define T_TARGET 322
#define T_TRUNCATE 323
#define T_VER_EXTERN 324
#define T_VER_GLOBAL 325
#define T_VER_LOCAL 326
#define T_LSHIFT_E 327
#define T_RSHIFT_E 328
#define T_LSHIFT 329
#define T_RSHIFT 330
#define T_EQ 331
#define T_NE 332
#define T_GE 333
#define T_LE 334
#define T_ADD_E 335
#define T_SUB_E 336
#define T_MUL_E 337
#define T_DIV_E 338
#define T_AND_E 339
#define T_OR_E 340
#define T_LOGICAL_AND 341
#define T_LOGICAL_OR 342
#define UNARY 343
#define T_NUM 344
#define T_COMMONPAGESIZE 345
#define T_COPY 346
#define T_DSECT 347
#define T_IDENT 348
#define T_INFO 349
#define T_MAXPAGESIZE 350
#define T_MEMORY_ATTR 351
#define T_NOLOAD 352
#define T_ONLY_IF_RO 353
#define T_ONLY_IF_RW 354
#define T_OVERLAY 355
#define T_STRING 356
#define T_WILDCARD 357

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 256 "ld_script_parser.y"

	struct ld_exp *exp;
	struct ld_script_assign *assign;
	struct ld_script_cmd *cmd;
	struct ld_script_list *list;
	struct ld_script_input_file *input_file;
	struct ld_script_phdr *phdr;
	struct ld_script_region *region;
	struct ld_script_sections_output *output_desc;
	struct ld_script_sections_output_data *output_data;
	struct ld_script_sections_output_input *input_section;
	struct ld_script_sections_overlay *overlay_desc;
	struct ld_script_sections_overlay_section *overlay_section;
	struct ld_script_version_entry *version_entry;
	struct ld_script_version_entry_head *version_entry_head;
	struct ld_wildcard *wildcard;
	char *str;
	int64_t num;

#line 291 "ld_script_parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_LD_SCRIPT_PARSER_H_INCLUDED  */
