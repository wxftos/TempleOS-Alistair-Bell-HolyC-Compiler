#include "parser.h"

static inline uint8_t
holyc_parse_is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}


int8_t 
holyc_parse_stream(char *chars, uint32_t char_count, struct holyc_token **tokens, uint32_t *token_count)
{
    /* Create a copy buffer that will be incrimented. */
	char *chars_copy = chars;
	
    /* Stores the current character and the previous one. */
	char current, previous;

    /* Pinsor used to claw around tokens and extract them. */
	struct holyc_parse_pinsor p = { 0};

    /* 
     * Fixes an annoying bug.
     * Having a single whitespace char at the start of the chars will make the first token have rubbish at the start.
     * Probaly an flaw in this program but simple enougth fix.
     */
	if (holyc_parse_is_whitespace(*chars)) {
		++p.left;
	}

	while ((current = *(++chars_copy))) {
		p.right++;

		if (holyc_parse_is_whitespace(current)) { 
			if (!holyc_parse_is_whitespace(previous)) {
				char tmp[64] = { 0 };
				strncpy(tmp, chars + p.left,  p.right - p.left);
				printf("[%2ld %2ld][%s]\n", p.left, p.right, tmp);
			}
			p.left = p.right;
			++p.left;
		}

        /* Set the current to the previous before the current char is updated. */
		previous = current;
	}

	return 0;
}
