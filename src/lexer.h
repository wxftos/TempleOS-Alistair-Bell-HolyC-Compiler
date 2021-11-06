#ifndef HOLYC_LEXER_H
#define HOLYC_LEXER_H

#include "inc.h"
#include "util.h"

struct token {
	hash_t hash;
};

/* Turns the chars into tokens, populates the structures */
int8_t lex_chars(char *, struct token **, uint32_t *);

#endif
