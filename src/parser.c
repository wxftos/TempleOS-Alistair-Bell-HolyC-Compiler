#include "parser.h"

struct holyc_parse_update_data {
	struct holyc_token **tokens;
	uint32_t *token_count;
};

typedef void (*holyc_parse_function)(const char, const char, struct holyc_parse_pinsor *, char *, void *, void **, struct holyc_parse_update_data *);

static void
holyc_parse_type_default(const char current, const char previous, struct holyc_parse_pinsor *pinsor, char *chars, void *baton, void **func, struct holyc_parse_update_data *d)
{
	switch (current) {
		/* Essentially whitespace junk, probaly really stupid code. */
		case '\t'... ' ': {

			/* Add a token if next is not a whitespace char. */
			if (previous > ' ') {
				/* Now add the token */
				char tmp[64] = { 0 };
				strncpy(tmp, chars + pinsor->left, pinsor->right - pinsor->left);
				printf("%s\n", tmp);
			}
			/* Equalise the claws and incriment the left claw. */
			pinsor->left = pinsor->right;
			pinsor->left++;
			break;
		}
	}
}


int8_t 
holyc_parse_chars(char *chars, uint32_t char_count, struct holyc_token **tokens, uint32_t *token_count)
{
	/* Pinsor used like a claw for grabbing tokens. */
	struct holyc_parse_pinsor p = { 0 };

	/* Start in default mode. */
	holyc_parse_function callback = holyc_parse_type_default;

	/* Always have a non incrimented buffer. */
	char *chars_copy = chars;
	/* Stores the previous and current character being inspected. */
	char current_char, previous_char = '#';

	/* Prepare the memory. */
	*tokens = (struct holyc_token *)calloc(1, sizeof(*tokens));

	/* Baton data that gives extra unique data. */
	void *baton = malloc(sizeof(*baton));
	
	struct holyc_parse_update_data data = {
		.tokens = tokens,
		.token_count = token_count,
	};
	
	/* Fixes stupid bug. */
	if ('\b' <  *chars && *chars < '!') {
		p.left++;
	}

	/* Loop througth the chars. */
	while ((current_char = *(chars_copy++))) {
		
		callback(current_char, previous_char, &p, chars, baton, (void **)&callback, &data);

		p.right++;	
		previous_char = current_char;
	}

	return 0;
}
