#include "lexer.h"
#include "defs.h"


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
	['='] = { TOKEN_ASSIGNMENT, 0 }, 
	[';'] = { TOKEN_TERMINATOR, 0 }, 
	/* Scoper. */
	['.'] = { TOKEN_SCOPER, '{' },
	['{'] = { TOKEN_SCOPER, '{' },
	['}'] = { TOKEN_SCOPER, '}' },
	['['] = { TOKEN_SCOPER, '[' },
	[']'] = { TOKEN_SCOPER, ']' },
	[')'] = { TOKEN_SCOPER, ')' },
	['('] = { TOKEN_SCOPER, '(' },
	/* Numerical constants. */
	['0'] = { TOKEN_CONSTANT, 0 },
	['1'] = { TOKEN_CONSTANT, 0 },
	['2'] = { TOKEN_CONSTANT, 0 },
	['3'] = { TOKEN_CONSTANT, 0 },
	['4'] = { TOKEN_CONSTANT, 0 },
	['5'] = { TOKEN_CONSTANT, 0 },
	['6'] = { TOKEN_CONSTANT, 0 },
	['7'] = { TOKEN_CONSTANT, 0 },
	['8'] = { TOKEN_CONSTANT, 0 },
	['9'] = { TOKEN_CONSTANT, 0 },
};

#define CSET(h, c, t)            \
	case h: {                    \
 		out->category = c;       \
		out->type = t;           \
		out->specific = hash_in; \
		return 0;                \
	} 

int
lex_decipher(struct token *out, char *in, char *raw_start, unsigned int diff)
{
	if (diff == 1) {
		if (single_lookup[(int)*in][0] != 0) {
			/* Special character. */			
			*out = (struct token) {
				.category = single_lookup[(int)*in][0],
				.type     = single_lookup[(int)*in][1],
				.specific = (hash_t)*in,
			};
			return 0;
		}
		*out = (struct token) {
			.category = TOKEN_SYMBOL,
			.specific = *in,
		};
		return 0;
	} 
	/* Wider than a single character. */
	if (*in == '\'' || *in == '"') {
		/* String constant. */
		*out = (struct token) {
			.category = TOKEN_CONSTANT,
			.type     = CONSTANT_STRING,
		};
		return 0;
	}
	/* Check for numerical constant. */
	if (isdigit(*in)) {
		errno = 0;
		out->category = TOKEN_CONSTANT;
		out->specific = (hash_t)validate_numerical_constant(in, (unsigned int *)&out->type, out->line);
		return (errno == 0) - 1;
	}

	/* If the string is not a numerical or string constant then it may be a predefined symbol or type. */
	hash_t hash_in = hash_chars(in);
	switch (hash_in) {
		CSET(HASH_TYPES_U0, TOKEN_TYPE, TYPE_U0);
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
		CSET(HASH_KEYWORDS_const, TOKEN_MODIFIER, MODIFIER_const);
		CSET(HASH_KEYWORDS_continue, TOKEN_KEYWORD, KEYWORD_continue);
		CSET(HASH_KEYWORDS_default, TOKEN_KEYWORD, KEYWORD_default);
		CSET(HASH_KEYWORDS_do, TOKEN_KEYWORD, KEYWORD_do);
		CSET(HASH_KEYWORDS_else, TOKEN_KEYWORD, KEYWORD_else);
		CSET(HASH_KEYWORDS_enum, TOKEN_KEYWORD, KEYWORD_enum);
		CSET(HASH_KEYWORDS_extern, TOKEN_MODIFIER, MODIFIER_extern);
		CSET(HASH_KEYWORDS_for, TOKEN_KEYWORD, KEYWORD_for);
		CSET(HASH_KEYWORDS_goto, TOKEN_KEYWORD, KEYWORD_goto);
		CSET(HASH_KEYWORDS_if, TOKEN_KEYWORD, KEYWORD_if);
		CSET(HASH_KEYWORDS_return, TOKEN_KEYWORD, KEYWORD_return);
		CSET(HASH_KEYWORDS_static, TOKEN_MODIFIER, MODIFIER_static);
		CSET(HASH_KEYWORDS_struct, TOKEN_KEYWORD, KEYWORD_struct);
		CSET(HASH_KEYWORDS_switch, TOKEN_KEYWORD, KEYWORD_switch);
		CSET(HASH_KEYWORDS_union, TOKEN_KEYWORD, KEYWORD_union);
		CSET(HASH_KEYWORDS_while, TOKEN_KEYWORD, KEYWORD_while);
	}
	/* New symbol. */
	out->category = TOKEN_SYMBOL;
	out->specific = hash_in;
	return 0;
}
