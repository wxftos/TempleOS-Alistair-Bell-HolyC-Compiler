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

hash_t
hash_chars(char *chars)
{
	hash_t total = 5381;
	char c;
	while ((c = *chars++)) {
		total += ((total << 5) + total) + c;
	}
	return total;
}
int
validate_numerical_constant(char *in, const unsigned int diff)
{
	/* Validates whether constant numbers, or values are in the correct format. */
	if (isdigit(in[1]) != 1) {
		switch (in[1]) {
			case 'x': {
			}
			case 'b': {
			}
			default: {
				fprintf(stderr, "error: numerical constant %s has invalid base escape sequence '%c'.\n", in, in[1]);
				return -1;
			}
		}
	}
}
