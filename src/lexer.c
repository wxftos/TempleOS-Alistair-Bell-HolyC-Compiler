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
 * along with this program.  If not, see <https:www.gnu.org/licenses/>.
*/

#include "lexer.h"

#define BUFF_SIZE 256
#define PREALLOC_SIZE 50

/* Forware declare the token structure. */
struct machine;

typedef void (lex_func)(char *, char *, struct machine *);
static void  lex_default(char *, char *, struct machine *);
static void  lex_string(char *, char *, struct machine *);
static void  lex_comment(char *, char *, struct machine *);

struct machine {
	char new_token;
	char baton;
	char *last_write;
	char *start[2];
	unsigned long diff[2];
	lex_func *func;
};

static int 
new_token(char *chars, char *start, const unsigned long diff, struct token *out)
{
	char tmp[BUFF_SIZE] = { 0 };
	strncpy(tmp, chars + (start - chars), diff);
	return lex_decipher(out, tmp, start, diff);
}
	
static int 
delim(char *in)
{
	switch (*in) {
		case '}':
		case '{':
		case ']':
		case '[':
		case ')':
		case '(':
		case ';':
		case '=':
		case '+':
		case '-':
		case '/':
		case '*':
		case '%':
		case '<':
		case '>':
		case '^':
		case '|':
		case '!':
		case '&':
		case '~':
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
	if (isspace(*rchar)) {
		if (isspace(*lchar) || (rchar - machine->last_write) < 1) {
			++machine->last_write;
		} else {
			machine->new_token = 1;
			machine->start[0] = machine->last_write;
			machine->diff[0] = rchar - machine->last_write;
		}
	} else {
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
	/* Keeps track of the allocation count of the returned buffer. */
	unsigned int alloc_count = PREALLOC_SIZE;
	/* Allows the insertion of tokens to fail and trigger the failure of the mainloop. */
	signed int interupt = 0;

	/* Like pinsors to extract strings and substrings. */
	char *rchar = in;
	char *lchar = in;
	struct machine machine = { .last_write = &(*in) };
	*out = (struct token *)malloc(PREALLOC_SIZE * sizeof(*(*out)));
	/* Output of where token data is stored. */
	struct token *tptr = *out;

	if (delim(in) == 2) {
		machine.func = lex_string;
		machine.baton = *in;
	} else {
		machine.func = lex_default;
	}
	*count ^= *count;

	do {
		++rchar;
		machine.func(rchar, lchar, &machine);

		if (machine.new_token > 0) {
			if ((*count - machine.new_token) >= alloc_count) {
				alloc_count += PREALLOC_SIZE;
				*out = (struct token *)realloc(*out, alloc_count * sizeof(*(*out)));
			}
			/* Add the first one. */
			interupt = new_token(in, machine.start[0], machine.diff[0], tptr);
			++(*count);
			if (interupt < 0)
				return -1;

			if (machine.new_token > 1) {
				interupt = new_token(in, machine.start[1], machine.diff[1], tptr);
				if (interupt < 0)
					return -1;
				++(*count);
			}
			machine.new_token = 0;
			machine.last_write = rchar + 1;
		}
		lchar = rchar;
	} while (*rchar != (char)0);
	return interupt;
}
