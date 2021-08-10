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

#include "lexer.h"

enum lexer_type {
	LEXER_TYPE_TYPE,
};

struct lexer_scopes {
	uint16_t data;

	union {
		struct {
			uint16_t param;
			uint16_t brace; 
		};
	};
};

static void
lexer_throw_error(char *chars, struct token *offender, const char *msg) {

	/* Store the string of the char before the chars get incrimented and broken up. */
	char *err_buff = (char *)malloc(offender->length + 1);
	memset(err_buff, 0, offender->length);

	strncpy(err_buff, chars + offender->start, offender->length);
	err_buff[offender->length] = '\0';

	/* Get the line that the error is on, this is calculated on the fly to reduce memory that the tokens have to store. */	
	uint32_t ln = 1, i = 0; 
	for (; i < offender->start; ++i) {
		ln += (chars[i] == '\n');
	}
	
	fprintf(stderr, "holyc: error syntax violation. %s, offender \'%s\', line %d.\n", msg, err_buff, ln);
	free(err_buff);
}

int8_t
lexer_validate_expression(char *chars, struct token *tokens, struct pinsor *p, struct expression *expr)
{

	/* 
     * Single length expressions are allowed in holyc but only if the single token is a call to a function which takes 0 required arguments.
	 * Whilst C allows use of single line references it is forbidden for now as it is dead code that the programmer should iron out.
	 */
	if (p->right - p->left == 2) {
		fprintf(stderr, "holyc: error single token expression forbidden!\n");
		return -1;
	}

	/* This loop does no syntax checking, it finds what type the tokens are and will go through all of them. The next stage checks for errors. */
	struct token *ptr = &(*(tokens + p->left));
	fprintf(stdout, "holyc: new token type run\n");
	for (; ptr != &tokens[p->right]; ++ptr) {
	}
	return 1;
}

int8_t
lexer_loop(char *chars, struct token *tokens, const uint32_t token_count)
{
    if (token_count <= 0 || chars == NULL) {
        return 0;
    }
    
	/* Everybody's favourite returning struct, p the pinsor. */
	struct pinsor p = { 0 };
	struct lexer_scopes scope = { 0 };

	struct token *tptr = &(*tokens);
    for (; p.right < token_count; ++p.right) {
		switch (tptr->hash) {
			/* Semicolon. */
			case 355205: {
				break;
			}
			/* Opening param. */
			case 355186: {
				++scope.param;
				break;
			}
			/* Closing param. */
			case 355187: {
				if (scope.param == 0) {
					/* Too many scope dereferences, syntax error. */
					lexer_throw_error(chars, tptr, "Too many scope dereferenes");
					return -1;
				}
				else if (scope.param == 1) {
				}
				--scope.param;
				break;
			}
			case 355269: {
				++scope.brace;
				break;
			}
			case 355271: {
				/* Too many scope dereferences, similiar to the closing param above. */
				if (scope.brace == 0) {
					lexer_throw_error(chars, tptr, "Too many scope dereferences");
					return -1;
				}
				--scope.brace;
			}
			break;
		}

		++tptr;
    }
	return 0;
}
void 
lexer_populate_language_type_hashes(struct hash_table *table)
{
    /* Pregenerated hashes for the types using the algorithm. */
    hash_t types[] = {
		23444510,   /* I8 */
		23445302,   /* U8 */
		1547337252, /* I16 */
		1547389524, /* U16 */
		1547337380, /* I32 */
		1547389652, /* U32 */
		1547337580, /* I64 */
		1547389852, /* U64 */
		23445294,   /* U0 */
    };

    /* Create a new table with the scope of 0. */
    hash_table_new(table, 0);

    /* Batch add the types. */
    hash_table_insert_batch(table, types, ARRAY_LEN(types));
}
void 
lexer_populate_language_keyword_hashes(struct hash_table *table)
{

}
