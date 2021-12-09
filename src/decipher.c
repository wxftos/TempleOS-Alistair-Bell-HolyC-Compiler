#include "lexer.h"

static enum token_type
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
	if (diff < 2) {
		if ((out->type = char_type(*in)) != TOKEN_UNKNOWN) {
			/* Pack our data into the 64 bit value, there is not a need for their to be a hash. */
			out->data = (hash_t)*in;
			return 0;
		} else {
			/* No keyword or predefined type is 1 char wide, this means that it must be a new symbol. */
			out->type = TOKEN_SYMBOL;
			out->data = hash_chars(in);
		}
	}


	return 0;
}
