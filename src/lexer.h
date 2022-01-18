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

enum tokens {
	TOKEN_UNDEFINED = 0,
	/* Predefined system types. */
	TOKEN_U0,
	TOKEN_U8, TOKEN_I8,
	TOKEN_U16, TOKEN_I16,
	TOKEN_U32, TOKEN_I32,
	TOKEN_U64, TOKEN_I64,
	TOKEN_F64,
	TOKEN_BOOLEAN,
	/* Language keywowrds. */
	TOKEN_BREAK,
	TOKEN_CASE,
	TOKEN_CLASS,
	TOKEN_CONST,
	TOKEN_CONTINUE,
	TOKEN_DEFAULT,
	TOKEN_DO,
	TOKEN_ELSE,
	TOKEN_ENUM,
	TOKEN_EXTERN,
	TOKEN_FOR,
	TOKEN_GOTO,
	TOKEN_IF,
	TOKEN_RETURN,
	TOKEN_STATIC,
	TOKEN_STRUCT,
	TOKEN_SWITCH,
	TOKEN_UNION,
	TOKEN_WHILE,
	/* Scopers. */
	TOKEN_LPARAM, TOKEN_RPARAM,
	TOKEN_LINDEX, TOKEN_RINDEX,
	TOKEN_LCURLY, TOKEN_RCURLY,
	/* Terminators and assignments. */
	TOKEN_EQUALS,
	TOKEN_TERMINATOR,
	/* Math operators. */
	TOKEN_ADD, TOKEN_SUB, TOKEN_MUL, TOKEN_DIV,
	/* Bitwise. */
	TOKEN_LSHIFT,
	TOKEN_RSHIFT,
	TOKEN_XOR,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_1_COMP,
	/* Comparitives.  */
	TOKEN_LTHAN,
	TOKEN_GTHAN,
	TOKEN_NOT,
	/* Runtime generated stuff. */
	TOKEN_SYMBOL,
	TOKEN_CONSTANT,
	/* Dummy. */
	TOKEN_MAX,
};

enum token_constant {
	CONSTANT_STRING,
	CONSTANT_INTEGER,
	CONSTANT_DECIMAL,
};

enum token_category {
	CATEGORY_KEYWORD,
	CATEGORY_OPERATOR,
	CATEGORY_TYPE,
	CATEGORY_COMPARISON,
	CATEGORY_SCOPER,
	CATEGORY_MISC,
};

struct token {
	unsigned short type;
	unsigned short category;
	unsigned int   diff;
	unsigned int   offset;
	unsigned int   line;
	/* Some tokens may leave this field empty. */
	hash_t         specific;
};

/* Turns the chars into tokens, populates the structures */
int lex_chars(char *, struct token **, unsigned int *);
int lex_decipher(struct token *, char *);

#endif
