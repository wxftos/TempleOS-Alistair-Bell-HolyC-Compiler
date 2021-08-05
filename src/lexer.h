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

#ifndef HOLYC_LEXER_H
#define HOLYC_LEXER_H

#include "inc.h"
#include "parser.h"

/* Turns tokens into arch independant instructions for the binary generator. */
int8_t lexer_loop(char *, struct token *, const uint32_t);
/* Populates type hash table scope 0 with the standard types for the language. */
void lexer_populate_langauge_type_hashes(struct hash_table *);
/* Populates the keywords for the language, this is static and cannot be added to during compilation. */
void lexer_populate_language_keyword_hashes(struct hash_table *);

#endif