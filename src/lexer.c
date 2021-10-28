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
decider_whitespace(char *rchar, char *lchar, char *baton)
{
	if (*rchar < 33 && *lchar > 32)
		return 1;
	else if (*rchar < 33 && *lchar < 33)
		return 2;
	return 0;
}
static int8_t
decider_string(char *rchar, char *lchar, char *baton)
{
	if (*rchar == *baton) {
		return 5;
	}
	return 3;
}
static int8_t
decider_comment(char *rchar, char *lchar, char *baton)
{
	/* Multiline comment. */
	switch (*baton) {
		case '/': {
			return 3 - (*rchar == '\n');
		}
		case '*': {
			if (*lchar == '*' && *rchar == '/') {
				/* Spoof the mode switcher. */
				*baton = '\0';
				return 2;
			}
		}
	}
	return 3;
}
static decider_func
switch_decider(char *rchar, char *lchar, char *baton)
{
	if (*baton == '\0') {
		switch (*rchar) {
			case '"':
			case '\'': *baton = *rchar; return decider_string;
			case '/':
			case '*': {
				if (*lchar == '/' || *lchar == '*') {
					*baton = *rchar;
					return decider_comment;
				}
			}
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


	char *rchar = chars, *lchar = chars, baton = '\0';
	struct pinsor p = { 0, 0 };
	decider_func f = decider_whitespace;

	for (; *rchar != '\0'; ++rchar) {
		switch (f(rchar, lchar, &baton)) {
			case 1: PRINT_TK(chars, p); break;
			case 2: p.left = p.right + 1; break;
			case 3: goto end_sequence;
		}
		f = switch_decider(rchar, lchar, &baton);
	end_sequence:
		lchar = rchar;
		++p.right;
	}
	return 0;
}
