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

#include "lexer.h"

#define BUFF_SIZE 256
#define PREALLOC_COUNT 50

/* Forware declare the token structure. */
struct machine;

typedef void (lex_func)(char *, char *, struct machine *);
static void  lex_default(char *, char *, struct machine *);
static void  lex_string(char *, char *, struct machine *);
static void  lex_comment(char *, char *, struct machine *);

struct machine {
	unsigned char new_token;
	char baton;
	char *last_write;
	char *start[2];
	unsigned int diff[2];
	lex_func *func;
};

static int 
new_token(char *chars, char *start, const unsigned long diff, unsigned int line_num, struct token *out)
{
	static char char_buffer[BUFF_SIZE]; 
	strncpy(char_buffer, chars + (start - chars), diff);
	char_buffer[diff] = '\0';
	*out = (struct token) {
		.offset = start - chars,
		.diff   = diff,
		.line   = line_num,
	};
	return lex_decipher(out, char_buffer, start, diff);
}
static int 
delim(char *in)
{
	switch (*in) {
		case '}': case '{': case ']': case '[': case ')': case '(':
		case ';': case '=': case '+': case '-': case '/': case '*':
		case '%': case '<': case '>': case '^': case '|': case '!':
		case '&': case '~':
			return 1;
		case '\'':
		case '"':
			return 2;
	}
	return 0;
}
static void
lex_default(char *rchar, char *lchar, struct machine *machine)
{
	if (isspace(*rchar) != 0) {
		if (isspace(*lchar) != 0 || (rchar - machine->last_write) < 1) {
			++machine->last_write;
			return;
		} 
		machine->new_token = 1;
		machine->start[0] = machine->last_write;
		machine->diff[0] = rchar - machine->last_write;
		return;
	}

	int index = 0, res;
	if ((res = delim(rchar)) < 1) {
		return;
	}
	
	if (*rchar == '/') {
		register char f = *(rchar + 1);
		if (f == '/' || f == '*') {
		machine->func = lex_comment;
		machine->baton = f;
		res = -1;
		}
	}
	/* Check to add the previous segment. */
	if ((rchar - machine->last_write) > 0) {
		machine->start[index] = machine->last_write;
		machine->diff[index] = rchar - machine->last_write;
		++index;
	}

	switch (res) {
		case 1: {
		/* Adds the delim as a token. */
		machine->start[index] = rchar;
		machine->diff[index] = 1;
		++index;
		break;
		}
		case 2: {
		/* Changes the mode to string mode. */
		machine->func = lex_string;
		machine->baton = *rchar;
		break;
		}
	}
	machine->new_token = index;
}
static void
lex_string(char *rchar, char *lchar, struct machine *machine)
{
	machine->func = (*rchar == machine->baton) ? lex_default : lex_string;
	return;
}
static void
lex_comment(char *rchar, char *lchar, struct machine *machine)
{
	if (machine->baton == '/') {
		/* Single line comment mode. */
		if (*rchar == '\n') {
			goto change_mode;
		}
		return;
	} else {
		if (*lchar == '*' && *rchar == '/')
			goto change_mode;
		return;
	}

change_mode:
	machine->func = lex_default;
	machine->last_write = rchar + 1;
	return;
}
int
lex_chars(char *in, struct token **out, unsigned int *count)
{
	*count = 0;
	char *lchar, *rchar;
	unsigned int line_num, alloc_count;
	signed int interrupt;
	struct token *tptr;
	struct machine machine;

	/* Start at the first line, despite computers indexing from 0. */
	line_num = 1;
	/* Accumilative total for the the size of the buffer allocated to store generated tokens.*/
	alloc_count = PREALLOC_COUNT;
	/* Interrupt allows called functions to terminate the mainloop. */
	interrupt = 0;

	lchar = in;
	rchar = in;
	
	/* Allocate the first buffer. */
	*out = (struct token *)malloc(alloc_count * sizeof(struct token));
	if (!(*out)) {
		register int snap = errno;
		fprintf(stderr, "error: failed to allocate token storage buffer. %s.\n", strerror(errno));
		return -1;
	}

	/* Write pointer to the start of the allocated buffer. */
	tptr = &(*out)[0];

	/* Populate the machine with data. */
	machine = (struct machine) {
		.last_write = &in[0],
	};

	/* Handle some lexer edge cases. This prevents some malformed stuff. */
	if (delim(in) == 2) {
		machine.func = lex_string;
		machine.baton = *in;
	} else {
		machine.func = lex_default;
	}
	if (isspace(*rchar)) {
		++machine.last_write;
	}

	do {
		++rchar;
		line_num += (*rchar == '\n');
		machine.func(rchar, lchar, &machine);

		if (machine.new_token > 0) {
			if ((*count - machine.new_token) >= alloc_count) {
				alloc_count += PREALLOC_COUNT;
				*out = (struct token *)realloc(*out, alloc_count * sizeof(*(*out)));
			}
			unsigned int index = 0;
			while (machine.new_token > 0) {
				if ((interrupt = new_token(in, machine.start[index], machine.diff[index], line_num, tptr)) < 0)
					return -1;
				++(*count);
				++tptr;
				++index;
				--machine.new_token;
			}
			machine.new_token = 0;
			machine.last_write = rchar + 1;
		}
		lchar = rchar;
	} while (*rchar != (char)0);

	return interrupt;
}
