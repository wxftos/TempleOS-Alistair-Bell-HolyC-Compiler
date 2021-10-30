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

#define BUFF_SIZE 128

typedef int8_t (*lex_mode)(char *, char *, char *);

static uint32_t 
add_token(char *chars, char *rchar, char *wptr)
{
	register long diff = (rchar - wptr);
	char tmp[BUFF_SIZE] = { 0 };
	strncpy(tmp, chars + (wptr - chars), diff);
	fprintf(stdout, "%s\n", tmp);
	return 1;
}

static int8_t
is_special(char *rchar)
{
	switch (*rchar) {
		case ')':
		case '(':
		case '{':
		case '}':
		case '[':
		case ']':
		case ';':
		case '*':
		case '=':
			return 1;
	}
	return 0;
}

static int8_t
lex_whitespace(char *lchar, char *rchar, char *baton)
{
	if (*lchar > 32 && *rchar < 33) {
		return 1;
	} else if (*lchar < 33 && *rchar < 33) {
		return 2;
	}
	return 0;
}

static uint32_t
recursive_lex(char *chars, char *lchar, char *rchar, char *wptr)
{
	static char baton     = (char)0;
	static uint32_t added = 0;
	static lex_mode func  = lex_whitespace;
	
	switch (func(lchar, rchar, &baton)) {
		case 1: {
			if (wptr != rchar)
				added += add_token(chars, rchar, wptr);
		}
		case 2: {
			wptr = rchar + 1;
			break;
		}
		case 0: {
			if (is_special(rchar)) {
				if (rchar != wptr) {
					added += add_token(chars, rchar, wptr);
					wptr = rchar;
				} 
				added += add_token(chars, rchar + 1, wptr);
				++wptr;
			}
		}
	}
	/* Incriment the rightmost char claw. */
	lchar = rchar;
	++rchar;
	return (*rchar != 0) ? recursive_lex(chars, lchar, rchar, wptr) : added;
 }

int8_t
lex_chars(char *chars, struct token **out, uint32_t *count)
{
	if (*chars == '\0') {
		return 0;
	}
	
	char *wptr = (char *)chars;
	*count = recursive_lex(chars, chars, chars, wptr);
	*out = (struct token *)malloc((*count) * sizeof(*(*out)));
	fprintf(stdout, "tokens = %d\n", *count);
	return -1;
}
