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

#define CSET(h, c, t)            \
	case h: {                    \
 		out->category = c;       \
		out->type = t;           \
		out->specific = hash_in; \
		break;                   \
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
		if (*in == '\'' || *in == '"') {
			out->category = TOKEN_CONSTANT;
			out->type     = CONSTANT_STRING;
			out->specific = diff;
			out->start    = raw_start;
		} else if (isdigit(*in) != 0) {
			if (validate_constant(in, diff) < 0) {
				return -1;
			}
		}

		/* Compare the hash of the string coming in compared to our generated hashes from src/defs.h. */
		const hash_t hash_in = hash_chars(in);
		switch (hash_in) {
			CSET(HASH_TYPES_I8, TOKEN_TYPE, TYPE_I8);
			CSET(HASH_TYPES_U8, TOKEN_TYPE, TYPE_U8);
			CSET(HASH_TYPES_I16, TOKEN_TYPE, TYPE_I16);
			CSET(HASH_TYPES_U16, TOKEN_TYPE, TYPE_U16);
			CSET(HASH_TYPES_I32, TOKEN_TYPE, TYPE_I32);
			CSET(HASH_TYPES_U32, TOKEN_TYPE, TYPE_U32);
			CSET(HASH_TYPES_I64, TOKEN_TYPE, TYPE_I64);
			CSET(HASH_TYPES_U64, TOKEN_TYPE, TYPE_U64);
			CSET(HASH_TYPES_F64, TOKEN_TYPE, TYPE_F64);
			CSET(HASH_TYPES_Boolean, TOKEN_TYPE, TYPE_BOOLEAN);
			CSET(HASH_KEYWORDS_break, TOKEN_KEYWORD, KEYWORD_break);
			CSET(HASH_KEYWORDS_case, TOKEN_KEYWORD, KEYWORD_case);
			CSET(HASH_KEYWORDS_class, TOKEN_KEYWORD, KEYWORD_class);
			CSET(HASH_KEYWORDS_const, TOKEN_KEYWORD, KEYWORD_const);
			CSET(HASH_KEYWORDS_continue, TOKEN_KEYWORD, KEYWORD_continue);
			CSET(HASH_KEYWORDS_default, TOKEN_KEYWORD, KEYWORD_default);
			CSET(HASH_KEYWORDS_do, TOKEN_KEYWORD, KEYWORD_do);
			CSET(HASH_KEYWORDS_else, TOKEN_KEYWORD, KEYWORD_else);
			CSET(HASH_KEYWORDS_enum, TOKEN_KEYWORD, KEYWORD_enum);
			CSET(HASH_KEYWORDS_extern, TOKEN_KEYWORD, KEYWORD_extern);
			CSET(HASH_KEYWORDS_for, TOKEN_KEYWORD, KEYWORD_for);
			CSET(HASH_KEYWORDS_goto, TOKEN_KEYWORD, KEYWORD_goto);
			CSET(HASH_KEYWORDS_if, TOKEN_KEYWORD, KEYWORD_if);
			CSET(HASH_KEYWORDS_return, TOKEN_KEYWORD, KEYWORD_return);
			CSET(HASH_KEYWORDS_static, TOKEN_KEYWORD, KEYWORD_static);
			CSET(HASH_KEYWORDS_struct, TOKEN_KEYWORD, KEYWORD_struct);
			CSET(HASH_KEYWORDS_switch, TOKEN_KEYWORD, KEYWORD_switch);
			CSET(HASH_KEYWORDS_union, TOKEN_KEYWORD, KEYWORD_union);
			CSET(HASH_KEYWORDS_while, TOKEN_KEYWORD, KEYWORD_while);
			default: {
				/* Assume that it is an symbol, the string constant and numerical checks have failed us. */	
				out->category = TOKEN_SYMBOL;
				out->specific = hash_in;
			}
		}
	}
	out->start = raw_start;
}
