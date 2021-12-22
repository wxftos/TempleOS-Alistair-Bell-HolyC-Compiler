#include "parser.h"
#include "defs.h"

static const char *category_str[] = {
	[TOKEN_UNKNOWN] = "unknown",
	[TOKEN_KEYWORD] = "keyword",
	[TOKEN_MODIFIER] = "modifier",
	[TOKEN_SYMBOL] = "symbol",
	[TOKEN_TYPE] = "type",
	[TOKEN_OPERATOR] = "operator",
	[TOKEN_ASSIGNMENT] = "assignment",
	[TOKEN_SCOPER] = "scoper",
	[TOKEN_TERMINATOR] = "terminator",
	[TOKEN_CONSTANT] = "constant",
};
static const char *modifier_str[] = {
	[MODIFIER_extern] = "extern",
	[MODIFIER_static] = "static",
	[MODIFIER_const] = "const",
};


int
parse_tokens(struct token *tokens, unsigned int count, char *chars)
{
	struct token *i;
	/* Loop through our tokens. */
	for (i = tokens; i < tokens + count; ++i) {
		if (i->category == TOKEN_MODIFIER) {
			fprintf(stdout, "tokens[%2d].type=%s.\n", i - tokens, modifier_str[i->type]);
		} else if (i->category == TOKEN_CONSTANT) {
			if (i->type == CONSTANT_INTEGER) {
				fprintf(stdout, "tokens[%2d].specific=%ld.\n", i - tokens, (signed long long)i->specific);
			} else if (i->type == CONSTANT_DECIMAL) {
				fprintf(stdout, "tokens[%2d].specific=%.3ff.\n", i - tokens, (double)i->specific);
			}
		} else {
			fprintf(stdout, "tokens[%2d].category=%s.\n", i - tokens, category_str[i->category]);
		}
	}
	return 0;
}


