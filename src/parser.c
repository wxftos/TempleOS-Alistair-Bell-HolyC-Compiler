#include "parser.h"

struct holyc_parse_update_data {
	struct holyc_token **tokens;
	uint32_t *token_count;
	uint32_t alloc_count;
	char **construction;
};

typedef void (*holyc_parse_function)(const char, const char, struct holyc_parse_pinsor *, char *, void *, void **, struct holyc_parse_update_data *);

static void
holyc_parse_add_token(char *chars, struct holyc_parse_pinsor *pinsor, struct holyc_parse_update_data *data)
{
	/* Clean the previous junk that the construction had. */
	memset(*data->construction, 0, HOLYC_UNDER_CONSTRUCTION_SIZE);
	/* Copy the data to the buffer. */
	strncpy(*data->construction, chars + pinsor->left, pinsor->right - pinsor->left);

	/* Check for another batch token increase. */
	if (data->alloc_count <= *data->token_count + 1) {
		fprintf(stdout, "holyc: new token batch reallocation, count %d\n", data->alloc_count);
		*data->tokens = (struct holyc_token *)realloc(*data->tokens, sizeof(struct holyc_token) * (data->alloc_count + 20));
		data->alloc_count += 20;
	}


	struct holyc_token tmp_token = {
		.hash = holyc_hash_chars(*data->construction),
		.start_char_index = pinsor->right,
	};

	struct holyc_token *insertion = &((*data->tokens)[(*data->token_count)]);

	++(*data->token_count);
	memcpy(insertion, &tmp_token, sizeof(tmp_token));
}

static void
holyc_parse_type_default(const char current, const char previous, struct holyc_parse_pinsor *pinsor, char *chars, void *baton, void **func, struct holyc_parse_update_data *d)
{
	switch (current) {
		/* Essentially whitespace junk, probaly really stupid code. */
		case '\t'... ' ': {

			/* Add a token if next is not a whitespace char. */
			if (previous > ' ') {
				/* Now add the token */
				holyc_parse_add_token(chars, pinsor, d);
			}
			/* Equalise the claws and incriment the left claw. */
			pinsor->left = pinsor->right;
			pinsor->left++;
			break;
		}
		
		/* 
		 * Ascii table has special grammer chars all spread out so a few fallthrough switches are needed.
		 * this language won't support unicode, only ascii and extended.
		 */
		
		case '!' ... '/': {
			/* FALLTHROUGH */
		}
		case ':' ... '@': {
			/* FALLTHROUGH */
		}

		case '[' ... '^': {
			/* FALLTHROUGH */
		}
		
		case '{' ... '~': {
			/* Firstly add the previous token. */
			break;
		}
		default: {
			return;
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

	/* 
	 * Prepare the memory. 
	 * In this situation the program gueses by the char count to how many to tokens to pre allocate, saves constant reallocation.
	 */
	*tokens = (struct holyc_token *)calloc((char_count / 4) + 5, sizeof(*(*tokens)));

	/* Baton data that gives extra unique data. */
	void *baton = malloc(sizeof(*baton));

	/* Buffer to parse to the hasher where a token is stored. */
	char *under_construction = (char *)malloc(HOLYC_UNDER_CONSTRUCTION_SIZE);
	
	struct holyc_parse_update_data data = {
		.tokens = tokens,
		.alloc_count = (char_count / 4) + 5,
		.token_count = token_count,
		.construction = &under_construction,
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

	free(under_construction);
	return 0;
}
