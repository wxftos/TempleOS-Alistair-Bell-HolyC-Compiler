#ifndef HOLYC_LEXER_H
#define HOLYC_LEXER_H

#include "inc.h"
#include "util.h"

/* What category it falls into. */
enum token_category {
	TOKEN_UNKNOWN = 0,
	TOKEN_KEYWORD,
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
	MODIFIER_CONST,
	MODIFIER_STATIC,
	MODIFIER_EXTERN,
	MODIFIER_SIGNED,
	MODIFIER_UNSIGNED,
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
	KEYWORD_const,
	KEYWORD_continue,
	KEYWORD_default,
	KEYWORD_do,
	KEYWORD_else,
	KEYWORD_enum,
	KEYWORD_extern,
	KEYWORD_for,
	KEYWORD_goto,
	KEYWORD_if,
	KEYWORD_return,
	KEYWORD_static,
	KEYWORD_struct,
	KEYWORD_switch,
	KEYWORD_union,
	KEYWORD_while,
};

struct token {
	unsigned char category;
	unsigned char type;
	hash_t        specific;
	char          *start;
};

/* Turns the chars into tokens, populates the structures */
int lex_chars(char *, struct token **, unsigned int *);
int lex_decipher(struct token *, char *, char *, unsigned int);

#endif
