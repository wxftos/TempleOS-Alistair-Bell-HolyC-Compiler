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

typedef int (*lex_mode)(char *, char *, char *, void **);

static unsigned long line_number = 1;

#define ADD_TOKEN_CHECK(count, chars, rchar, wptr) \
	if (wptr != rchar) { \
		*count += add_token(chars, rchar, wptr); \
		wptr = rchar; \
	}

static unsigned int 
add_token(char *chars, char *rchar, char *wptr)
{
	register size_t diff = (rchar - wptr);
	char char_buff[BUFF_SIZE] = { 0 };
	strncpy(char_buff, chars + (wptr - chars), diff);
	fprintf(stdout, "line %3ld -> %s\n", line_number, char_buff);
	return 1;
}

static int
is_special(char *rchar)
{
	switch (*rchar) {
		case '+':
		case '-':
		case ')':
		case '(':
		case '{':
		case '}':
		case '[':
		case ']':
		case ';':
		case '=':
			return 1;
		case '\'':
		case '"':
			return 2;
		case '*':
		case '/':
			return 3;
	}
	return 0;
}

static int
lex_whitespace(char *lchar, char *rchar, char *baton, void **func)
{
	if (*lchar > 32 && *rchar < 33) {
		return 1;
	} else if (*lchar < 33 && *rchar < 33) {
		return 2;
	}
	return 0;
}
static int
lex_string(char *lchar, char *rchar, char *baton, void **func)
{
	if (*rchar == *baton) {
		*func = (void **)lex_whitespace;
	}
	return INT8_MAX;
}
static int
lex_comment(char *lchar, char *rchar, char *baton, void **func)
{
	switch (*baton) {
		case '/': {
			if (*rchar == '\n') {
				*func = (void *)lex_whitespace;
			}
			break;
		}
		case '*': {
			if (*lchar == '*' && *rchar == '/') {
				*func = (void *)lex_whitespace;
			}
		}
	}
	return 2;
}
int
lex_chars(char *chars, struct token **out, unsigned int *count)
{
	if (*chars == '\0') {
		return (int8_t)*chars;
	}
	
	char *lchar = chars, *rchar = chars, *wptr = chars;
	char baton = (char)0;
	lex_mode func = lex_whitespace;
	
	while (*rchar != '\0') {
		switch (func(lchar, rchar, &baton, (void **)&func)) {
			case 0: {
				/* SPECIAL SWITCH. */
				switch (is_special(rchar)) {
					case 1: {
						ADD_TOKEN_CHECK(count, chars, rchar, wptr);
						*count += add_token(chars, rchar + 1, wptr);
						++wptr;
						break;
					}
					case 2: {
						ADD_TOKEN_CHECK(count, chars, rchar, wptr);
						baton = *rchar;
						func = lex_string;
						break;
					}
					case 3: {
						if (*lchar == '/') {
							if (*rchar == '/' || *rchar == '*') {
								baton = *rchar;
								func = lex_comment;
							} else {
								ADD_TOKEN_CHECK(count, chars, rchar, wptr);
							}
						}
						break;
					}
				}
				break;
			}
			case 1: {
				ADD_TOKEN_CHECK(count, chars, rchar, wptr);
			}
			case 2: {
				wptr = rchar + 1;
				break;
			}
		}
		/* Update the newline counter if applicable, set the lchar to rchar then incriment rchar. */
		line_number += (*rchar == '\n');
		lchar = rchar;
		++rchar;
	}

	/* Check that the mode is whitespace, if not than an error has occured. */
	if (func == lex_string) {
		fprintf(stdout, "error: %s not terminated, expected terminator [%c] at end of file!\n", (baton == '\'') ? "character sequence" : "string", baton);
		return -1;
	}
	fprintf(stdout, "token count %d\n", *count);
	*out = (struct token *)malloc(10);
	return 0;
 }
