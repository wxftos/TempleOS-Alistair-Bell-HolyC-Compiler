#include "parser.h"

/* 
 * This is a function used by the parser to determine whether the char is special and requires it's own token.
 * If a token changes the interpreter mode (str or comment) then the callback is changed to account for this.
 * Params:
 *		1: current character being analyased.
 *      2: index along the buffer (right claw of the pinsor).
 *		3: the entire source (if previous lookup required).
 *		4: pointer to the next function to be used for analyses, updated if analysing requires it.
 */
typedef uint8_t (*holyc_parse_mode_func_t)(char, uint64_t, char *, void **);

/* 
 * For analysing the stream it each char is analysed for special chars or whitespace / newlines.
 * The right pinsor always advances until no more chars.
 * When a special char is found the substr of the buffer is retrieved using offsets calculated by the pinsor.
 * Then when the token is added the left is set to the right pinsor repeating until no more.
 */
struct holyc_parse_pinsor {
	uint64_t left;
	uint64_t right;
};

int8_t 
holyc_parse_stream(char *buffer, uint32_t char_count, struct holyc_token **tokens, uint32_t *token_count)
{

	/* Pinsor used for analysis. */
	struct holyc_parse_pinsor p = { 0 };

	/* Reference to the current character being inspected. */
	char current;
	while ((current = *buffer++)) {


		/* Incriment the right pinsor at the end of the loop. */
		++p.right;
	}

	return 0;
}
