/*
 * This file is part of cparser.
 * Copyright (C) 2007-2009 Matthias Braun <matze@braunis.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */
#ifndef TOKEN_T_H
#define TOKEN_T_H

#include <stdio.h>
#include "string_rep.h"
#include "symbol.h"
#include "symbol_table.h"
#include "type.h"

typedef enum token_kind_t {
	T_NULL  =  0,
	T_EOF   = '\x04', // EOT
#define T(mode,x,str,val) T_##x val,
#define TS(x,str,val) T_##x val,
#include "tokens.inc"
#undef TS
#undef T
	T_LAST_TOKEN
} token_kind_t;

typedef enum preprocessor_token_kind_t {
	TP_NULL  = T_NULL,
	TP_EOF   = T_EOF,
#define T(mode,x,str,val) TP_##x val,
#define TS(x,str,val) TP_##x val,
#include "tokens_preprocessor.inc"
#undef TS
#undef T
	TP_LAST_TOKEN
} preprocessor_token_kind_t;

typedef struct source_position_t source_position_t;
struct source_position_t {
	const char *input_name;
	unsigned    lineno;
	unsigned    colno            : 31;
	unsigned    is_system_header : 1;
};

/* position used for "builtin" declarations/types */
extern const source_position_t builtin_source_position;

typedef struct token_base_t     token_base_t;
typedef struct string_literal_t string_literal_t;
typedef struct number_literal_t number_literal_t;
typedef union  token_t          token_t;

struct token_base_t {
	unsigned          kind;
	source_position_t source_position;
	symbol_t         *symbol;
};

enum string_encoding_t {
	STRING_ENCODING_CHAR,
	STRING_ENCODING_WIDE
};
typedef enum string_encoding_t string_encoding_t;

struct string_literal_t {
	token_base_t      base;
	string_encoding_t encoding;
	string_t          string;
};

struct number_literal_t {
	token_base_t  base;
	string_t      number;
	string_t      suffix;
};

union token_t {
	unsigned          kind;
	token_base_t      base;
	string_literal_t  string;
	number_literal_t  number;
};

char const *get_string_encoding_prefix(string_encoding_t);

void init_tokens(void);
void exit_tokens(void);
void print_token_kind(FILE *out, token_kind_t token_kind);
void print_token(FILE *out, const token_t *token);

void print_pp_token_kind(FILE *out, int kind);
void print_pp_token(FILE *out, const token_t *token);

/**
 * returns true if pasting 2 preprocessing tokens next to each other
 * without a space in between would generate (an)other preprocessing token(s)
 */
bool tokens_would_paste(preprocessor_token_kind_t token1,
                        preprocessor_token_kind_t token2);

#endif
