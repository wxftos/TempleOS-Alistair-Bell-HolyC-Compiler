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
#include "lexer.h"

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

typedef int (*validate_callback)(char);

static int
isbdigit(char in)
{
	return (in == '0') + (in == '1');
}
static int
isfloat(char *in)
{
	char *inc = in;
	int ret = 0;
	while (*(++inc) != '\0') {
		ret += *inc == '.';
	}
	return ret;
}

hash_t
validate_numerical_constant(char *in, unsigned *type, unsigned int line)
{
	/* Check wheter it*/
	int res = isfloat(in);
	if (res > 1) {
		fprintf(stderr, "error floating point value \'%s\' has too many decimal points.\n", in);
		errno = -1;
		return 0;
	} else if (res) {
		*type = CONSTANT_DECIMAL;
		fprintf(stdout, "here\n");
		return (hash_t)atof(in);
	}
	*type = CONSTANT_INTEGER;
	char *eptr;
	hash_t ret;

#ifdef _DEFAULT_SOURCE
	ret = (hash_t)strtoll(in, &eptr, 0);
#else 
	ret = (hash_t)strtol(in, &eptr, 0);
#endif
	/* Whilst `strol(l)` does concatinate the garbage at the end of the string this will throw the error. */
	if (*eptr != '\0') {
		errno = -1;
		fprintf(stderr, "error: trailing junk in numerical constant \'%s\', \'%c\' onwards, line %u\n", in, *eptr, line);
		return errno;
	}
	if (errno != 0) {
		register int snap = errno;
		fprintf(stderr, "error: numerical constant conversion failed, %s, \'%s\', line %u\n", strerror(errno), in, line);
		return snap;
	}
	return ret;
}
