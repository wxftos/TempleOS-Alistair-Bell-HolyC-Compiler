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


enum machine_flags {
	FLAG_ADD_TOKEN = 0x1,
	FLAG_NEW_PROC  = 0x2,
};

struct state_machine {
	unsigned int       flags;
	unsigned int       flags_data;	
	char               *wptr;
};

typedef int (*state_proc)(char *, char *, struct state_machine *);

static int
determine_special(char *rchar)
{
	switch (*rchar) {
		case '\'':
		case '"':
			return 2;
		case ';':
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case '+':
			return 1;
		default:
			return 0;
	}
}

static int
state_whitespace(char *lchar, char *rchar, struct state_machine *machine)
{
	if (*rchar < 33) {
		if (*lchar < 33) {
			++machine->wptr;
		} else {
			/* Tell the machine that we want a new token. */
			machine->flags += FLAG_ADD_TOKEN;
		}
	} else {
		/* The rchar was not whitespace, in this case check if it is special. */	
		switch (determine_special(rchar)) {
			case 1: {
				break;
			}
			case 2: {
				machine->flags      += FLAG_ADD_TOKEN;
				machine->flags      += (determine_special(lchar) < 1 * FLAG_NEW_PROC);
				/* Prepare the packet. */
				machine->flags_data = 0x01;
				machine->flags_data += (*rchar << 16);
				break;
			}
		}
	}
	return 0;
}
static int
state_string(char *lchar, char *rchar, struct state_machine *machine)
{
	register char packed_char = (char)(machine->flags_data >> 16) & 0xff;
	if (*rchar == packed_char) {
		/* Call for a new token. */
		machine->flags      = FLAG_NEW_PROC | FLAG_ADD_TOKEN;
		/* This tells the machine to switch to whitespace mode and spoof the size of the strncpy call. */
		machine->flags_data = 0;
		machine->flags_data += (((rchar - machine->wptr) + 1) << 8);
	}
	return 0;
}
static int
state_comment(char *lchar, char *rchar, struct state_machine *machine)
{
	return 0;
}

/* Declare our table of state machines, this is what the `state_machine->flag_data` can be set to on mode changes. */
static const state_proc machine_table[] = { state_whitespace, state_string, state_comment };

int
lex_chars(char *chars, struct token **out, unsigned int *count)
{
	char *lchar = chars;
	char *rchar = chars;
	state_proc proc = state_whitespace;
	struct state_machine machine = { .wptr = chars };

	/* This prevents a segfault of memory read as the rchar is always set to the location -> (chars + 1). */
	if (*(chars + 1) == '\0') {
		return -1;
	}
	do {
		++rchar;
		proc(lchar, rchar, &machine);
		/* Handle the state machine. */
		if (machine.flags & FLAG_ADD_TOKEN) {
			char tmp[64] = { 0 };
			/* The 2nd byte of the flags data tells us about the length of the new token. */
			register unsigned int copy_size = (machine.flags_data >> 8) & 255;
			strncpy(tmp, chars + (machine.wptr - chars), copy_size == 0 ? (rchar - machine.wptr) : copy_size);
			fprintf(stdout, "[%s]\n", tmp);

			machine.wptr = rchar + 1;
			/* Remove the `add token` flag and reset the flags data. */
			machine.flags &= ~(FLAG_ADD_TOKEN);
			machine.flags_data = 0; 

		} else if (machine.flags & FLAG_NEW_PROC) {
			/* Update the proc function pointer to the lower 8 bits of the data, this is the convention. */
			proc = machine_table[machine.flags_data & 0xff];
			/* Remove the flag. */
			machine.flags &= ~(FLAG_NEW_PROC);
		}
		lchar = rchar;
	/* Loop until a null terminator is reached.  */
	} while (*rchar != (char)0);

	return -1;
}
