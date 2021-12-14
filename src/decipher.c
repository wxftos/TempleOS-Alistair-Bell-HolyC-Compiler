#include "lexer.h"

static enum token_category
char_type(const char in)
{
	switch (in) {
		case '+':
		case '-':
		case '/':
		case '*':
			/* TODO add the bitwise operators. */
			return TOKEN_OPERATOR;
		case '=':
			return TOKEN_ASSIGNMENT;
		case ')':
		case '(':
		case '{':
		case '}':
		case '[':
		case ']':
			return TOKEN_SCOPER;
		case ';':
			return TOKEN_TERMINATOR;
	}
	return TOKEN_UNKNOWN;
}
int
lex_decipher(struct token *out, char *in, unsigned int diff)
{

	return 0;
}
