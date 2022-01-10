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
