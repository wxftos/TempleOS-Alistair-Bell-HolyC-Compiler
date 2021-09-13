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
 * along with this program.  If not, see <https:www.gnu.org/licenses/>.
*/

#include "parser.h"

#define BUFF_SIZE 128

/* 
 * Whilst static variables are not good practice it does solve the callback's params being flooded with other calls required params. 
 * Only varaibles which make sense to be static are, along with any function defines for referencing.
 */

static char baton;
static char *static_chars;
static struct pinsor pinsor = { 0 };
static void (*callback)(const char, const char, enum token_type *);

static void parse_mode_chars(const char, const char, enum token_type *);
static void parse_mode_comment(const char, const char, enum token_type *);
static void parse_mode_default(const char, const char, enum token_type *);

static void
parser_add_token(void)
{
	char tmp[64] = { 0 };
	strncpy(tmp, static_chars + pinsor.left, pinsor.right - pinsor.left);
	//tmp[pinsor.right - pinsor.left] = '\0';
	fprintf(stdout, "holyc: [%s]\n", tmp);
}

void
parse_mode_chars(const char cchar, const char lchar, enum token_type *ltype)
{
	/* Char that made the mode switch, " or '. */
	if (cchar == baton) {
		/* Change back to the default function. */
		callback = parse_mode_default;
	};
}

void
parse_mode_comment(const char cchar, const char lchar, enum token_type *ltype)
{
    switch (cchar) {
        /* Handle the case for single line comments. */
        case '\n': {
            /* End of single line comment. */
            if (baton == '/') {
                goto end_it;
            }
            return;
        }
        case '/': {
            /* End of the multiline comment. */
            if (lchar == '*') {
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
        pinsor.left = pinsor.right;
        ++pinsor.left;
        /* Obvious lie :). */
        *ltype = TOKEN_TYPE_NULL;
        callback = parse_mode_default;
    }
}

void
parse_mode_default(const char cchar, const char lchar, enum token_type *ltype)
{
	switch (cchar) {
		case '\t' ... ' ': {
			if (*ltype != TOKEN_TYPE_NULL) {
				 parser_add_token();
			}
			pinsor.left = pinsor.right;
			++pinsor.left;
			*ltype = TOKEN_TYPE_NULL;
			break;
		}

		/* Use fallthrough for regular characters. */

		case '0' ... '9': 
			/* FALLTHROUGH */
		case 'A' ... 'Z':
			/* FALLTHROUGH */
		case 0x5f ... 'z': {
			/* If a regular char proceeds a special add it to the list of tokens. */
			if (*ltype == TOKEN_TYPE_OPER) {
				 parser_add_token();
				pinsor.left = pinsor.right;
			}

			/* Set the last type to junk. */
			*ltype = TOKEN_TYPE_REG;
			break;
		}
        case '/': {
            /* FALLTHROUGH */
        }
        case '*': {
            if (pinsor.right != 0 && lchar == '/') {
                baton = cchar;
                callback = parse_mode_comment;
                break;
            }
            goto special_default;
        }
        case '"': {
            /* FALLTHROUGH */
        }
        case '\'': {
            baton = cchar; 
            callback = parse_mode_chars;
            /* FALLTHROUGH */
        }

		/* Default is for special chars, easier to use fallthroughs for regular chars as they are more bunched up. */
		default: {
            special_default: {
			    if (*ltype != TOKEN_TYPE_NULL) {
				    parser_add_token();
			    }
			    pinsor.left = pinsor.right;
			    *ltype = TOKEN_TYPE_OPER;
            }
        }
	}
}

int8_t 
parse_chars(char *chars, uint32_t char_count, struct token **tokens, uint32_t *token_count)
{
	callback = parse_mode_default;

	/* Always have a non incrimented buffer. */
	static_chars = chars;
	/* Stores the current character being inspected. */
	char *cchar = &(*chars), lchar = ' ';
	enum token_type ltype = TOKEN_TYPE_NULL;

	*tokens = (struct token *)malloc((char_count / 5) + 5 * sizeof(struct token));

	/* Loop througth the chars. */
	do {
		callback(*cchar, lchar, &ltype);
		lchar = *cchar;
		++pinsor.right;
		++cchar;
	} while (cchar < chars + char_count);

	/* Reallocate the size of the token storage to what it actually needs to be. */
	*tokens = (struct token *)realloc(*tokens, *token_count * sizeof(*(*tokens)));

	return 0;
}
