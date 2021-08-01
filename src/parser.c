/* 
 * Copyright (c) 2021 Alistair Bell <alistair@alistairbell.xyz>
 *
 * HolyC compiler is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "parser.h"

/* Data passed throught the functions responsible for adding new tokens and formatting them. */
struct holyc_parse_update_data {
	struct holyc_token **tokens;
	uint32_t *token_count;
	uint32_t alloc_count;
	char **construction;
};

/* 
 * Function that is called for each new char.
 * The running function can switch the next call based on the current char.
 * Paramas:
 *		all the chars,
 *      current char,
 *      last char type,
 *      pointer to source pinsor,
 *      pointer to function,
 *      pointer to add token data,
 */
typedef void (*holyc_parse_function)(char *, const char, enum holyc_parse_type *, struct holyc_parse_pinsor *, void **, struct holyc_parse_update_data *);

static void
holyc_parse_add_token(char *chars, struct holyc_parse_pinsor *pinsor, struct holyc_parse_update_data *data)
{
	/* Clean the previous junk that the construction had. */
	memset(*data->construction, 0, HOLYC_UNDER_CONSTRUCTION_SIZE);
	/* Copy the data to the buffer. */
	strncpy(*data->construction, chars + pinsor->left, pinsor->right - pinsor->left);

	/* Check for another batch token increase. */
	if (data->alloc_count <= *data->token_count + 1) {
		data->alloc_count += 20;
		fprintf(stdout, "holyc: new token batch reallocation, new count %d\n", data->alloc_count);
		*data->tokens = (struct holyc_token *)realloc(*data->tokens, sizeof(struct holyc_token) * data->alloc_count);
	}

	fprintf(stdout, "holyc: new token -> %s\n", *data->construction);

	struct holyc_token tmp_token = {
		.hash = holyc_hash_chars(*data->construction),
		.start_char_index = pinsor->right,
	};

	struct holyc_token *insertion = &((*data->tokens)[(*data->token_count)]);

	++(*data->token_count);
	memcpy(insertion, &tmp_token, sizeof(tmp_token));
}

static void
holyc_parse_type_default(char *chars, const char current_char, enum holyc_parse_type *last_type, struct holyc_parse_pinsor *pinsor, void **next_call, struct holyc_parse_update_data *data)
{
	switch (current_char) {
		/* Essentially whitespace junk, probaly really stupid code. */
		case '\t'... ' ': {

			/* Add a token if next is not a whitespace char. */
			if (*last_type != HOLYC_PARSE_TYPE_WHITESPACE) {
				/* Now add the token */
				holyc_parse_add_token(chars, pinsor, data);
			}
			/* Equalise the claws and incriment the left claw. */
			pinsor->left = pinsor->right;
			pinsor->left++;

			/* Update the last type. */
			*last_type = HOLYC_PARSE_TYPE_WHITESPACE;
			break;
		}
		
		/* 
		 * Ascii table has special grammer chars all spread out so a few fallthrough switches are needed.
		 * This language won't support unicode, only ascii and extended.
		 * All characters are treated as 1 byte wide.
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
			/* Firstly add the previous token if it was 'junk', prevents 2 consequtive specials being added too many times. */
			if (*last_type != HOLYC_PARSE_TYPE_WHITESPACE) {
				holyc_parse_add_token(chars, pinsor, data);
			}
			
		    pinsor->left = pinsor->right;

			*last_type = HOLYC_PARSE_TYPE_SPECIAL;
			break;
		}
		default: {
			
			if (*last_type == HOLYC_PARSE_TYPE_SPECIAL) {
				holyc_parse_add_token(chars, pinsor, data);
				pinsor->left = pinsor->right;
			}

			/* Fails special cases then return as junk. */
			*last_type = HOLYC_PARSE_TYPE_JUNK;
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
	/* Stores the current character being inspected. */
	char current_char;

	/* 
	 * Prepare the memory. 
	 * In this situation the program gueses by the char count to how many to tokens to pre allocate, saves constant reallocation.
	 */
	*tokens = (struct holyc_token *)calloc((char_count / 4) + 5, sizeof(*(*tokens)));

	/* Last type of char that was inspected. */
	enum holyc_parse_type last_type = HOLYC_PARSE_TYPE_WHITESPACE;

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
		callback(chars, current_char, &last_type, &p, (void **)&callback, &data);
		p.right++;	
	}

	free(under_construction);
	return 0;
}
