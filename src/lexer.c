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
	uint32_t data;
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
	return 1;
}

int8_t
lexer_loop(char *chars, struct token *tokens, const uint32_t token_count)
{
    if (token_count <= 0 || chars == NULL) {
        return 0;
    }
    
	/* Populate the language types. */
	struct hash_table l_types;
	lexer_populate_language_type_hashes(&l_types);
	
	struct token *tptr = &(*tokens);
	for (; tptr < tokens + token_count; ++tptr) {
 	}


	/* Cleanup the hashtables. */
	hash_table_destroy(&l_types);
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
    hash_table_create(table);

    /* Batch add the types. */
    hash_table_insert_batch(table, types, ARRAY_LEN(types));
}
void 
lexer_populate_language_keyword_hashes(struct hash_table *table)
{

}
