#ifndef HOLYC_PARSER_H
#define HOLYC_PARSER_H

#include "inc.h"
#include "util.h"

enum token_type {
	TOKEN_TYPE_NULL,
	TOKEN_TYPE_REG,
	TOKEN_TYPE_TYPE,
	TOKEN_TYPE_SYMBOL,
	TOKEN_TYPE_OPER,
	TOKEN_TYPE_ASSIGNMENT,
	TOKEN_TYPE_PARAMETER,
	TOKEN_TYPE_DQUOTE,
	TOKEN_TYPE_SQUOTE,
};

struct token {
	enum token_type type;
	/* Specific flags based upon the token type, each type has its own simple set of flags.  */
	uint8_t type_flags;
	/* Length of the string. */
	uint16_t string_length;
	/* Start position within the chars. */
	uint32_t start_position;
	/* Hash of the chars, used for comparisons.  */
	hash_t hash;
};

struct pinsor {
	uint32_t left;
	uint32_t right;
};

/* Turns the chars into tokens, populates the structures */
int8_t parse_chars(char *, uint32_t, struct token **, uint32_t *);

#endif
