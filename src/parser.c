#include "parser.h"

static inline uint8_t
holyc_parse_is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}


int8_t 
holyc_parse_stream(char *chars, uint32_t char_count, struct holyc_token **tokens, uint32_t *token_count)
{
	char *chars_copy = chars;
	
	char current, previous = ' ';
	struct holyc_parse_pinsor p = { 0};

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

		previous = current;
	}

	return 0;
}
