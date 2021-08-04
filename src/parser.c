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

static void
parser_add_token(char *chars, struct parser_pinsor *pinsor, struct parser_update_data *data)
{
	/* Clean the previous junk that the construction had. */
	memset(*data->construction, 0, HOLYC_UNDER_CONSTRUCTION_SIZE);
	/* Copy the data to the buffer. */
	strncpy(*data->construction, chars + pinsor->left, pinsor->right - pinsor->left);

	/* Check for another batch token increase. */
	if (data->alloc_count <= *data->token_count + 1) {
		data->alloc_count += 20;
		*data->tokens = (struct token *)realloc(*data->tokens, sizeof(struct token) * data->alloc_count);
	}

	fprintf(stdout, "holyc: new token -> %s\n", *data->construction);

	struct token *insertion = &((*data->tokens)[(*data->token_count)]);

	*insertion = (struct token) {
		.hash = hash_chars(*data->construction),
		.start_char_index = pinsor->right,
	};
	++(*data->token_count);
}

void
parser_mode_characters(char *chars, const char current_char, enum parser_type *last_type, struct parser_pinsor *pinsor, void *baton, void **next_call, struct parser_update_data *update_data)
{

	/* This is such a simple function for the characters mode that is it in fact really cool it works. */

	/* Char that made the mode switch, " or '. */
	char start_char = *((char *)baton);
	if (current_char == start_char) {
		/* Change back to the default function. */
		*next_call = parser_mode_default;
	};
}

void
parser_mode_comment(char *chars, const char current_char, enum parser_type *last_type, struct parser_pinsor *pinsor, void *baton, void **next_call, struct parser_update_data *update_data)
{
    /* The decider char tells whether it is a single line or an multiline comment likewise to this one right here. */
    char decider_char = *((char *)baton);
    switch (current_char) {
        /* Handle the case for single line comments. */
        case '\n': {
            /* End of single line comment. */
            if (decider_char == '/') {
                goto end_it;
            }
            return;
        }
        case '/': {
            /* End of the multiline comment. */
            if (chars[pinsor->right - 1] == '*') {
                /* Newline plus a single line comment. */
                goto end_it;
            }
            return;
        }
        default: {
            return;
        }
    }
    end_it: {
        pinsor->left = pinsor->right;
        ++pinsor->left;
        /* Obvious lie :). */
        *last_type = PARSE_TYPE_WHITESPACE;
        *next_call = parser_mode_default;
    }
}

void
parser_mode_default(char *chars, const char current_char, enum parser_type *last_type, struct parser_pinsor *pinsor, void *baton, void **next_call, struct parser_update_data *data)
{
	switch (current_char) {
		case '\t' ... ' ': {
			if (*last_type != PARSE_TYPE_WHITESPACE) {
				parser_add_token(chars, pinsor, data);
			}
			pinsor->left = pinsor->right;
			pinsor->left++;
			*last_type = PARSE_TYPE_WHITESPACE;
			break;
		}

		/* Use fallthrough for regular characters. */

		case '0' ... '9': 
			/* FALLTHROUGH */
		case 'A' ... 'Z':
			/* FALLTHROUGH */
		case 0x5f ... 'z': {
			
			/* If a regular char proceeds a special add it to the list of tokens. */
			if (*last_type == PARSE_TYPE_SPECIAL) {
				parser_add_token(chars, pinsor, data);
				pinsor->left = pinsor->right;
			}

			/* Set the last type to junk. */
			*last_type = PARSE_TYPE_JUNK;
			break;
		}
        case '/': {
            /* FALLTHROUGH */
        }
        case '*': {
            if (pinsor->right != 0 && chars[pinsor->right - 1] == '/') {
                *((char *)baton) = current_char;
                *next_call = parser_mode_comment;
                break;
            }
            goto special_default;
        }
        case '"': {
            /* FALLTHROUGH */
        }
        case '\'': {
            *((char *)baton) = current_char; 
            *next_call = parser_mode_characters;
            /* FALLTHROUGH */
        }

		/* Default is for special chars, easier to use fallthroughs for regular chars as they are more bunched up. */
		default: {
            special_default: {
			    if (*last_type != PARSE_TYPE_WHITESPACE) {
				    parser_add_token(chars, pinsor, data);
			    }
			    pinsor->left = pinsor->right;

			    *last_type = PARSE_TYPE_SPECIAL;
            }
        }
	}
}

int8_t 
parser_chars(char *chars, uint32_t char_count, struct token **tokens, uint32_t *token_count)
{
	/* Pinsor used like a claw for grabbing tokens. */
	struct parser_pinsor p = { 0 };

	/* Start in default mode. */
	parser_function callback = parser_mode_default;

	/* Always have a non incrimented buffer. */
	char *chars_copy = chars;
	/* Stores the current character being inspected. */
	char current_char;

	/* 
	 * Prepare the memory. 
	 * In this situation the program gueses by the char count to how many to tokens to pre allocate, saves constant reallocation.
	 */
	*tokens = (struct token *)calloc((char_count / 4) + 5, sizeof(*(*tokens)));

	/* Last type of char that was inspected. */
	enum parser_type last_type = PARSE_TYPE_WHITESPACE;

	/* Buffer to parse to the hasher where a token is stored. */
	char *under_construction = (char *)malloc(HOLYC_UNDER_CONSTRUCTION_SIZE);

	/* Extra data passed onto the next function call, call specific data. */
	void *baton = malloc(sizeof(*baton));

	struct parser_update_data data = {
		.tokens = tokens,
		.alloc_count = (char_count / 4) + 5,
		.token_count = token_count,
		.construction = &under_construction,
	};

	/* Loop througth the chars. */
	while ((current_char = *(chars_copy++))) {
		callback(chars, current_char, &last_type, &p, baton, (void **)&callback, &data);
		++p.right;
	}

	/* Reallocate the size of the token storage to what it actually needs to be. */
	*tokens = (struct token *)realloc(*tokens, *token_count * sizeof(*(*tokens)));

	free(under_construction);
	free(baton);
	return 0;
}
