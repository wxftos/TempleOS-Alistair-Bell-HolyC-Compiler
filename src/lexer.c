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

int8_t
lexer_validate_expression(char *chars, const struct token *tokens, struct pinsor *p, struct expression *expr)
{

	/* 
     * Single length expressions are allowed in holyc but only if the single token is a call to a function which takes 0 required arguments.
	 * Whilst C allows use of single line references it is forbidden for now as it is dead code that the programmer should iron out.
	 */
	if (p->right - p->left == 2) {
		fprintf(stderr, "holyc: error single token expression forbidden!\n");
		return -1;
	}
	fprintf(stderr, "holyc: expression range [%ld %ld], %ld.\n", p->left, p->right, p->right - p->left);

	/* This loop does no syntax checking, it finds what type the tokens are and will go through all of them. The next stage checks for errors. */
	uint32_t i = 0;
	for (; i < (p->right - p->left); ++i) {
	}
	return 1;
}

int8_t
lexer_loop(char *chars, struct token *tokens, const uint32_t token_count)
{
    if (token_count <= 0 || chars == NULL) {
        return 0;
    }
    
	struct token *current_token = &(*tokens);
	/* Everybody's favourite returning struct, p the pinsor. */
	struct pinsor p = { 0 };
	struct expression e = { 0 };

    for (; current_token != tokens + token_count; ++current_token) {

		/* 
		 * Loop through the tokens until an special hash is found which symbolises the end of an expression.
         * This would include ; { 
		 */
		switch (current_token->hash) {
			case 355205:
				/* FALLTHROUGH */
			case 355269: {
				/* 
			     * Once a expression terminator is found, using the previous tokens an expression is to be created.
				 * The expression checks using rules that the order of types of tokens is correct.
				 * It checks for any syntax errors, malformed chars etc.
				 */
				if (lexer_validate_expression(chars, tokens, &p, &e) < 0) {
					return -1;
				}
				p.left = p.right;
			}
			default: {
				/* Nothing todo on default. */
			}
		}
		/* Update the right claw. */
		++p.right;
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
