#include "lexer.h"
#include "defs.h"

#define CSET(h, t, c)            \
	case h: {                    \
		out->type = t;           \
 		out->category = c;       \
		return 0;                \
	} 

static unsigned int
isfloat(char *src)
{
	char *ptr = &src[0];
	unsigned int dec_count = 0;
	while (*(++ptr) != '\0') {
		dec_count += *ptr == '.';
	}
	return dec_count;
}

static hash_t
validate_numerical_constant(char *in, unsigned short *category, unsigned int line)
{
	/* Check whether it is an floating point constant. */
	int res = isfloat(in);
	if (res > 1) {
		fprintf(stderr, "error: floating point value \'%s\' has too many decimal points, line %u!\n", in, line);
		errno = -1;
		return 0;
	} else if (res) {
		*category = CONSTANT_DECIMAL;
		return (hash_t)atof(in);
	}
	*category = CONSTANT_INTEGER;
	char *eptr;
	hash_t ret;

#ifdef _DEFAULT_SOURCE
	ret = (hash_t)strtoll(in, &eptr, 0);
#else 
	ret = (hash_t)strtol(in, &eptr, 0);
#endif
	/* Whilst `strtol(3)` does concatinate the garbage at the end of the string this will throw the error. */
	if (*eptr != '\0') {
		errno = -1;
		fprintf(stderr, "error: trailing junk in numerical constant \'%s\', \'%c\' onwards, line %u!\n", in, *eptr, line);
		return errno;
	}
	if (errno != 0) {
		register int snap = errno;
		fprintf(stderr, "error: numerical constant conversion failed, %s, \'%s\', line %u!\n", strerror(errno), in, line);
		return snap;
	}
	return ret;
}

/* Stores mapped token along with the category it resides in. */
static const char single_lookup[][2] = {

	['('] = { CATEGORY_SCOPER, TOKEN_LPARAM },
	[')'] = { CATEGORY_SCOPER, TOKEN_RPARAM },
	['['] = { CATEGORY_SCOPER, TOKEN_LINDEX },
	[']'] = { CATEGORY_SCOPER, TOKEN_RINDEX },
	['{'] = { CATEGORY_SCOPER, TOKEN_RCURLY },
	['}'] = { CATEGORY_SCOPER, TOKEN_LCURLY },

	['='] = { CATEGORY_OPERATOR, TOKEN_EQUALS },
	['<'] = { CATEGORY_OPERATOR, TOKEN_LTHAN },
	['>'] = { CATEGORY_OPERATOR, TOKEN_GTHAN },
	['+'] = { CATEGORY_OPERATOR, TOKEN_ADD },
	['-'] = { CATEGORY_OPERATOR, TOKEN_SUB },
	['*'] = { CATEGORY_OPERATOR, TOKEN_MUL },
	['/'] = { CATEGORY_OPERATOR, TOKEN_DIV },

	['^'] = { CATEGORY_OPERATOR, TOKEN_XOR },
	['&'] = { CATEGORY_OPERATOR, TOKEN_AND },
	['~'] = { CATEGORY_OPERATOR, TOKEN_1_COMP },
	['|'] = { CATEGORY_OPERATOR, TOKEN_OR },
	[';'] = { CATEGORY_MISC, TOKEN_TERMINATOR },
};

int
lex_decipher(struct token *out, char *in)
{
	if (out->diff == 1) {
		/* Check for single value within our lookup table. */
		char res[2] = {
			single_lookup[(in[0])][0],
			single_lookup[(in[0])][1],
		};
		/* If the lookup was unsuccesful the sum of the results should sum 0. */
		if (res[0] + res[1] != 0) {
			out->type     = res[1];
			out->category = res[0];
			return 0;
		}
		/* If the only digit is numerical, it's a constant. */
		if (isdigit(in[0]) != 0) {
			out->type     = TOKEN_CONSTANT;
			out->category = CONSTANT_INTEGER;
			out->specific = (hash_t)(in - 48);
			return 0;
		}

		/* If the lookup failed then it is safe to assume that this is a new symbol. */
		out->type         = TOKEN_SYMBOL,
		out->category     = 0;
		out->specific     = (hash_t)in[0];
		return 0;
	}

	if (in[0] == '\'' || in[0] == '"') {
		/* Since the lexer has not spliced our string, simply populate the members. */
		out->type         = TOKEN_CONSTANT;
		out->category     = CONSTANT_STRING;
		return 0;
	}
	if (isdigit(in[0])) {
		/* We need to validate that this constant make sense. */
		out->specific     = TOKEN_CONSTANT;
		return validate_numerical_constant(in, &out->category, out->line);
	}

	/* 
	 * If all the previous checks fail:
	 * Token a language keyword.
	 * New token not before seen.
	 */

	switch ((out->specific = hash_chars(in))) {
		CSET(HASH_TYPES_U0, TOKEN_U0, CATEGORY_TYPE);
		CSET(HASH_TYPES_U8, TOKEN_U8, CATEGORY_TYPE);
		CSET(HASH_TYPES_I8, TOKEN_I8, CATEGORY_TYPE);
		CSET(HASH_TYPES_U16, TOKEN_U16, CATEGORY_TYPE);
		CSET(HASH_TYPES_I16, TOKEN_I16, CATEGORY_TYPE);
		CSET(HASH_TYPES_U32, TOKEN_U32, CATEGORY_TYPE);
		CSET(HASH_TYPES_I32, TOKEN_I32, CATEGORY_TYPE);
		CSET(HASH_TYPES_U64, TOKEN_U64, CATEGORY_TYPE);
		CSET(HASH_TYPES_I64, TOKEN_I64, CATEGORY_TYPE);
		CSET(HASH_TYPES_F64, TOKEN_F64, CATEGORY_TYPE);
		CSET(HASH_TYPES_Boolean, TOKEN_BOOLEAN, CATEGORY_TYPE);

		CSET(HASH_KEYWORDS_break, TOKEN_BREAK, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_case, TOKEN_CASE, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_class, TOKEN_CLASS, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_const, TOKEN_CONST, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_continue, TOKEN_CONTINUE, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_default, TOKEN_DEFAULT, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_do, TOKEN_DO, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_else, TOKEN_ELSE, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_enum, TOKEN_ENUM, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_extern, TOKEN_EXTERN, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_for, TOKEN_FOR, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_goto, TOKEN_GOTO, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_if, TOKEN_IF, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_return, TOKEN_RETURN, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_static, TOKEN_STATIC, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_struct, TOKEN_STRUCT, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_switch, TOKEN_SWITCH, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_union, TOKEN_UNION, CATEGORY_KEYWORD);
		CSET(HASH_KEYWORDS_while, TOKEN_WHILE, CATEGORY_KEYWORD);
	}
	/* Must be new token. */
	out->type     = TOKEN_SYMBOL;
	return 0;
}
