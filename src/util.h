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

#ifndef HOLYC_UTIL_H
#define HOLYC_UTIL_H

#include "inc.h"

/* 
 * Hash table structure.
 * Used in the lexer to store the defines of new tokens that are within the program.
 * Each table represents an heirarchy of scope. 0 = global.
 * The scope gets more local the greater the value.
 * All programs have scope 0 regardless and defines language keywords and types.
 */
struct hash_table {
    uint32_t elements;
    /* The table uses batch allocations for the member count, this reduces reallocation calls. */
    uint32_t allocation_count;
    hash_t *members;
    uint8_t scope;
};

/* Hashes a character for better finding of tokens. */
hash_t hash_chars(char *);

/* Creates a new hash table preallocating a batch of spots. */
void hash_table_new(struct hash_table *, const uint8_t, const uint32_t);
/* Inserts a single member to the hash table. */
void hash_table_insert(struct hash_table *, const hash_t);
/* Adds a batch of new members at once, useful for adding the base language types. */
void hash_table_insert_batch(struct hash_table *, const hash_t *, const uint32_t);
/* Finds a hash stored within the table. Returns UINT32_MAX on failure. */
uint32_t hash_table_find(struct hash_table *, const hash_t);
/* Destroys a table freeing the memory that it has allocated. */
void hash_table_destroy(struct hash_table *);




#endif
