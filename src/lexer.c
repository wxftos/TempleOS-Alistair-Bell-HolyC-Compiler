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

#define PRINT_TK(c, p) \
	{ \
		char t[BUFF_SIZE] = { 0 }; \
		strncpy(t, c + p.left, p.right - p.left); \
		fprintf(stdout, "[%s]\n", t); \
		p.left = p.right + 1; \
	}

typedef int8_t (*decider_func)(char *, char *, char *);

static int8_t
decider_whitespace(char *cchar, char *lchar, char *baton)
{
	if (*cchar < 33 && *lchar > 32)
		return 1;
	else if (*cchar < 33 && *lchar < 33)
		return 2;
	return 0;
}
static int8_t
decider_string(char *cchar, char *lchar, char *baton)
{
	if (*cchar == *baton) {
		return 5;
	}
	return 3;
}
static decider_func
switch_decider(char *cchar, char *lchar, char *baton)
{
	if (*baton == '\0') {
		switch (*cchar) {
			case '"':
			case '\'': *baton = *cchar; return decider_string;
		}
	}
	*baton = '\0';
	return decider_whitespace;
}

int8_t
lex_chars(char *chars, struct token **out, uint32_t *count)
{
	if (*chars == '\0') {
		return *chars;
	}


	char *cchar = chars, *lchar = chars, baton = '\0';
	struct pinsor p = { 0, 0 };
	decider_func f = decider_whitespace;

	for (; *cchar != '\0'; ++cchar) {
		switch (f(cchar, lchar, &baton)) {
			case 1: PRINT_TK(chars, p); break;
			case 2: p.left = p.right + 1; break;
			case 3: goto end_sequence;
		}
		f = switch_decider(cchar, lchar, &baton);
	end_sequence:
		lchar = cchar;
		++p.right;
	}
}
