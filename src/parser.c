/* 
 * Copyright (c) 2021-2022 Alistair Bell <alistair@alistairbell.xyz>
 *
 * HolyC compiler is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https:www.gnu.org/licenses/>.
 */

#include "parser.h"
#include "parser_errors.h"
#include "defs.h"
#include "util.h"

struct state_machine {
	struct error error;
	unsigned int active_modifiers;
	unsigned int doubled_modifiers;
	unsigned int scope_param;
	unsigned int scope_global;
	unsigned int scope_index;
};
static const char *src_chars;

#define BIT_MASK(x) \
	(1 << ((unsigned int)x))

static int
throw_error(struct state_machine *mach)
{
	char cpy[256];
	register struct token const *t = mach->error.offender;
	strncpy(cpy, src_chars + t->offset, t->diff);
	cpy[t->diff] = (char)0;
	fprintf(stderr, "error: parser violation, %s, offender \'%s\', line %u!\n", error_str[(int)mach->error.value], cpy, t->line);
	return ~0;
}
int
parse_tokens(struct token *tokens, unsigned int count, char *chars)
{
	src_chars = chars;


	struct state_machine *mach = (struct state_machine *)calloc(1, sizeof(*mach));
	if (!mach) {
		register int snap = errno;
		fprintf(stderr, "error: failed to allocate parser state machine. %s\n", strerror(snap));
		return -1;
	}
	free(mach);
	return 0;
}


