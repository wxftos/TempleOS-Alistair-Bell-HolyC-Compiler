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

#include "util.h"

#define HASH_TABLE_STARTING_ALLOC 50
#define HASH_TABLE_NEW_BATCH_FORMULAR(ac, mem) \
    ac * ((ac / HASH_TABLE_STARTING_ALLOC) + 1) * sizeof(mem)

hash_t
hash_chars(char *chars)
{

	/* Running of the hash first initialsed for bitshifts to work on the subsequent passes. */
	hash_t total = 5381;

	char c;
	while ((c = *chars++)) {
		total += ((total << 6) + total) + c;
	}
	return total;
}

static void
hash_table_resize(struct hash_table *table)
{
    /* Reallocate the memory. */
    table->members = realloc(table->members, HASH_TABLE_NEW_BATCH_FORMULAR(table->allocation_count, *table->members));
    exit(-1);
}
void 
hash_table_new(struct hash_table *table, uint8_t scope)
{
    *table = (struct hash_table) {
       .elements = 0,
       .members = calloc(HASH_TABLE_STARTING_ALLOC, sizeof(*table->members)),
       .scope = scope,
       .allocation_count = HASH_TABLE_STARTING_ALLOC,
    };
    /* Zero out the members. */
    memset(table->members, 0, HASH_TABLE_STARTING_ALLOC* sizeof(*table->members));
}
void
hash_table_insert(struct hash_table *table, hash_t hash)
{
   if (table->elements == (table->allocation_count - 1)) {
   }
}
void
hash_table_insert_batch(struct hash_table *table, hash_t *hashes, uint32_t count)
{
    /* Check for dynamic reallocation process. */ 
    if (table->elements == (table->allocation_count - 1)) {
        hash_table_resize(table);
    }
    
    /* Loop through finding a home for the new hashes. */
    uint32_t pos;
    hash_t *ptr = &hashes[0];
    for (; ptr != &hashes[count - 1]; ++ptr) {
        pos = ((uint32_t) *ptr % table->allocation_count);
        table->members[pos] = *ptr;
        ++table->elements;
    }
}
uint32_t
hash_table_find(struct hash_table *table, hash_t hash)
{
	uint32_t index = ((uint32_t) hash % table->allocation_count);
	return (table->members[index] != 0);	
}
void 
hash_table_destroy(struct hash_table *table)
{
    free(table->members);
}


