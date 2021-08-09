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

enum lexer_token_type {
	/* Only present on the first round of analysis. */
	LEXER_TOKEN_TYPE_FIRST,
	/* Terminates an expression, includes a semicolon or closing bracket. */
	LEXER_TOKEN_TYPE_TERMINATOR,
	/* Assings a symbol to another symbol, constant or expression. */
	LEXER_TOKEN_TYPE_ASSIGNMENT,
	/* Math operator / logical operation, + - * / ^ etc. */
	LEXER_TOKEN_TYPE_OPERATOR,
	/* Preoccurs an symbol, tells the compiler the type of the symbol. */
	LEXER_TOKEN_TYPE_TYPE,
	/* Symbol is a variable function or anything non predefined by the language that is affected by scope. */
	LEXER_TOKEN_TYPE_SYMBOL,
};

static inline void 
lexer_print_error(char *chars, const char *type, struct token *offender)
{
	char *err_buffer = (char *)malloc(offender->length);
	strncpy(err_buffer, chars + offender->start, offender->length);
	fprintf(stderr, "holyc: error, %s offender %s!\n", type, err_buffer);
	free(err_buffer);
}
static inline enum lexer_token_type
lexer_get_token_type(struct token *tk, uint32_t *current_scope, struct hash_table *types, struct hash_table *symbols)
{
	if (tk->length == 1) {
		/* Single length then check for a special type, operator, scope changer or terminator. */	
		switch (tk->hash) {
			/* + */
			case 355189: {
			}
			/* - */
			case 355191: {
			}
			/* * */
			case 355188: {
			}
			/* / */
			case 355193: {
			}
			/* = */
			case 355207: {
				return LEXER_TOKEN_TYPE_ASSIGNMENT;
			}
			default: {
			}
		}
	}
	/* If it is not an operator check within the symbol tables. */
	if (hash_table_find(&(*types), tk->hash)) {
		/* Set the last type to type. */
		return LEXER_TOKEN_TYPE_TYPE;
	}
	return LEXER_TOKEN_TYPE_SYMBOL;
}
static inline int8_t
lexer_syntax_valid(char *chars, enum lexer_token_type current, struct token *tk_current, enum lexer_token_type previous, struct token *tk_previous)
{
	switch (previous) {
		case LEXER_TOKEN_TYPE_FIRST: {
			if (current != LEXER_TOKEN_TYPE_TYPE || current != LEXER_TOKEN_TYPE_SYMBOL) {
				/* Syntax error! */
				lexer_print_error(chars, "syntax invalid, expected expression", tk_current);
				return -1;
			}
			break;
		}
		default: {
			return 0;
		}
	}
	return 0;
}

int8_t
lexer_loop(char *chars, struct token *tokens, const uint32_t token_count)
{
    if (token_count <= 0 || chars == NULL) {
        return 0;
    }
    
    uint32_t defined_types_count = 1, defined_symbols_count = 1, current_scope = 0;
    /* An array of the types defined within the program. */
    struct hash_table *defined_types = calloc(1, sizeof(*defined_types));
    /* Array of varaibles, functions and others within the program. */
    struct hash_table *defined_symbols = calloc(1, sizeof(*defined_symbols));

	/* Populate the scope 0 with system types. */
	lexer_populate_language_type_hashes(&(*defined_types));

	struct token *current_token = &(*tokens), *previous_token = NULL;
	enum lexer_token_type current_type, previous_type = LEXER_TOKEN_TYPE_FIRST;

	/* Stores the final result if syntax is correct. */
	int8_t valid = 0;

	/* For loop uses pointer incrimentation to allow for the exclusion of an interger counter. */
    for (; current_token != tokens + token_count; ++current_token) {
		current_type = lexer_get_token_type(current_token, &current_scope, defined_types, defined_symbols);

		/* Validate the syntax. */
		if (lexer_syntax_valid(chars, current_type, current_token, previous_type, previous_token) < 0) {
			valid = -1;
			goto cleanup;
		}
		/* Set the current values to the last round values for the next passthrough of the loop. */
		previous_token = current_token;
		previous_type  = current_type;
    }
   
	/* Cleanup the hashtables. */

	cleanup: {

		uint32_t i;
		for (i = 0; i < defined_types_count; ++i) {
			hash_table_destroy(&defined_types[i]);
		}
		for (i = 0; i < defined_symbols_count; ++i) {
			hash_table_destroy(&defined_symbols[i]);
		}

		/* Free the arrays that are heap allocated. */
		free(defined_types);
		free(defined_symbols);
	}
    return valid;
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
