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

int8_t
lexer_loop(char *chars, struct token *tokens, const uint32_t token_count)
{
    if (token_count <= 0 || chars == NULL) {
        return 0;
    }

    /* Create symbol hash tables of the tokens values. */
    struct hash_table language_types;
    lexer_populate_langauge_type_hashes(&language_types);
    fprintf(stdout, "holyc: initialised lexer language types, %d present\n", language_types.elements);
    hash_table_destroy(&language_types);
    

    struct token *current;
    for (current = &(*tokens); current != tokens + (token_count - 1); ++current) {
        /* Firstly understand the type of token we are dealing with. */       
    }

    return 0;
}
void 
lexer_populate_langauge_type_hashes(struct hash_table *table)
{
    /* Pregenerated hashes for the types using the algorithm. */
    const hash_t types[] = {
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

    /* Create a new table with the scope of 0 with a set number of elements preallocated as this won't grow. */
    hash_table_new(table, 0, 9);

    /* Batch add the types. */
    hash_table_insert_batch(table, types, ARRAY_LEN(types));
}
void 
lexer_populate_language_keyword_hashes(struct hash_table *table)
{

}
