#ifndef HOLYC_LEXER_H
#define HOLYC_LEXER_H

#include "inc.h"
#include "util.h"

enum token_type {
	TOKEN_KEYWORD,
	TOKEN_SYMBOL,
	TOKEN_OPERATOR,
	TOKEN_ASSIGNMENT,
	TOKEN_SCOPER,
	TOKEN_TERMINATOR,
	TOKEN_UNKNOWN,
};

struct token {
	enum token_type type;
	hash_t data;
};

/* Turns the chars into tokens, populates the structures */
int lex_chars(char *, struct token **, unsigned int *);
int lex_decipher(struct token *, char *, unsigned int);

#endif
