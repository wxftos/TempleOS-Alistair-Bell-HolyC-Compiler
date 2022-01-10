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

#ifndef HOLYC_PARSER_ERRORS_H
#define HOLYC_PARSER_ERRORS_H

#include "inc.h"
#include "lexer.h"

enum error_type {
	ERROR_UNDEFINED_REFERENCE,
	ERROR_SCOPE_VIOLATION,
	ERROR_EXPECTED_EXPRESSION,
	ERROR_REPEATED_MODIFIER,
	ERROR_CONFLICTING_MODIFIER,
};
static const char *error_str[] = {
	[ERROR_UNDEFINED_REFERENCE]  = "undefined reference",
	[ERROR_SCOPE_VIOLATION]      = "violation to program scope",
	[ERROR_EXPECTED_EXPRESSION]  = "expected expression",
	[ERROR_REPEATED_MODIFIER]    = "repeated expression modifier",
	[ERROR_CONFLICTING_MODIFIER] = "conflicting modifiers applied",
};
struct error {
	struct token *offender;
	enum error_type value;
};



#endif
