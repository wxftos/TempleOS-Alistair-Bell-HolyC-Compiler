#include "lexer.h"
#include "defs.h"

int
lex_is_special(const char c)
{
	switch (c) {
	}
}

static const unsigned char single_lookup[][2] = {
	/* Operators. */
	['+'] = { TOKEN_OPERATOR, OPERATOR_ADD },
	['-'] = { TOKEN_OPERATOR, OPERATOR_SUB },
	['*'] = { TOKEN_OPERATOR, OPERATOR_MUL },
	['/'] = { TOKEN_OPERATOR, OPERATOR_DIV },
	['%'] = { TOKEN_OPERATOR, OPERATOR_MOD },
	['&'] = { TOKEN_OPERATOR, OPERATOR_ADD },
	['|'] = { TOKEN_OPERATOR, OPERATOR_OR  },
	['^'] = { TOKEN_OPERATOR, OPERATOR_XOR },
	['>'] = { TOKEN_OPERATOR, OPERATOR_GT  },
	['<'] = { TOKEN_OPERATOR, OPERATOR_LT  },
	/* Misc. */
	['='] = { TOKEN_ASSIGNMENT, '=' },
	[';'] = { TOKEN_TERMINATOR, ';' },
	/* Scoper. */
	['.'] = { TOKEN_SCOPER, '{' },
	['{'] = { TOKEN_SCOPER, '{' },
	['}'] = { TOKEN_SCOPER, '}' },
	['['] = { TOKEN_SCOPER, '[' },
	[']'] = { TOKEN_SCOPER, ']' },
	[')'] = { TOKEN_SCOPER, ')' },
	['('] = { TOKEN_SCOPER, '(' },
};

static int
single_char(const char in, char *category, char *type)
{
	if (isdigit(in) != 0) {
		*category = TOKEN_CONSTANT;
		*type = CONSTANT_INTEGER;
	} else {
		
	}
}
int
lex_decipher(struct token *out, char *in, char *raw_start, unsigned int diff)
{
	if (diff == 1) {
		if (single_lookup[*in][0] != 0) {
			out->category = single_lookup[*in][0];
			out->type     = single_lookup[*in][1];
			out->specific = (hash_t)*in;
		} else {
			if (isdigit(*in)) {
				out->category = TOKEN_CONSTANT;
				out->type     = CONSTANT_INTEGER;
				/* Cast this to an integer, because the ascii values are sequential subtraction can ensure. */
				out->specific = ((int) *in - 48);
			} else {
				out->category = TOKEN_SYMBOL;
				out->specific = (char)*in;
			}
		}
	} else {

	}
	out->start = raw_start;
}
