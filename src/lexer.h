/* 
 * Copyright (c) 2021-2022 Alistair Bell <alistair@alistairbell.xyz>
 *
 * HolyC compiler is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https:www.gnu.org/licenses/>.
 */

#ifndef HOLYC_LEXER_H
#define HOLYC_LEXER_H

#include "inc.h"
#include "util.h"

/* What category it falls into. */
enum token_category {
	TOKEN_UNKNOWN,
	TOKEN_KEYWORD,
	TOKEN_MODIFIER,
	TOKEN_SYMBOL,
	TOKEN_TYPE,
	TOKEN_OPERATOR,
	TOKEN_ASSIGNMENT,
	TOKEN_SCOPER,
	TOKEN_TERMINATOR,
	TOKEN_CONSTANT,
};
/* Types. */
enum token_type {
	TYPE_U0,
	TYPE_U8,
	TYPE_U16,
	TYPE_U32,
	TYPE_U64,
	TYPE_I8,
	TYPE_I16,
	TYPE_I32,
	TYPE_I64,
	TYPE_F64,
	TYPE_BOOLEAN,
};
/* Types of constants: 0.0f, 10, "hello" etc. */
enum token_constant {
	CONSTANT_STRING,
	CONSTANT_INTEGER,
	CONSTANT_DECIMAL,
};
enum token_modifier {
	/* Declaration modifiers. */
	MODIFIER_const,
	MODIFIER_static,
	MODIFIER_extern,
};
enum token_operator {
	OPERATOR_ADD,
	OPERATOR_SUB,
	OPERATOR_MUL,
	OPERATOR_DIV,
	OPERATOR_MOD,
	OPERATOR_AND,
	OPERATOR_OR,
	OPERATOR_XOR,
	OPERATOR_OC,
	OPERATOR_GT,
	OPERATOR_LT,
};
enum token_keyword {
	KEYWORD_break,
	KEYWORD_case,
	KEYWORD_class,
	KEYWORD_continue,
	KEYWORD_default,
	KEYWORD_do,
	KEYWORD_else,
	KEYWORD_enum,
	KEYWORD_for,
	KEYWORD_goto,
	KEYWORD_if,
	KEYWORD_return,
	KEYWORD_struct,
	KEYWORD_switch,
	KEYWORD_union,
	KEYWORD_while,
};

struct token {
	unsigned short category;
	unsigned short type;
	unsigned int  diff;
	unsigned int  offset;
	unsigned int  line;
	/* Some tokens may leave this field empty. */
	hash_t        specific;
};

/* Turns the chars into tokens, populates the structures */
int lex_chars(char *, struct token **, unsigned int *);
int lex_decipher(struct token *, char *, char *, unsigned int);
unsigned int lex_get_line(const struct token *, char *);

#endif
