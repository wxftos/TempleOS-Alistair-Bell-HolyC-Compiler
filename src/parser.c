#include "parser.h"

/* Foward declarations! */
static enum holyc_parse_type holyc_parse_mode_default(char, uint64_t, char *, void *, void **);
static enum holyc_parse_type holyc_parse_mode_string(char, uint64_t, char *, void *, void **);
static enum holyc_parse_type holyc_parse_mode_comment(char, uint64_t, char *, void *, void **);


/* 
 * Default mode used when string, comment or char mode are inactive.
 * Reports all special characters and grammer within the language.
 * Will recognise when to change modes and how to handle the new tokens.
 */
static enum holyc_parse_type 
holyc_parse_mode_default(char current_char, uint64_t location, char *chars, void *extra, void **next_func)
{
	switch (current_char) {
	}
	return HOLYC_PARSE_TYPE_JUNK;
}

/* 
 * String mode, activated when a new '"' is found in the buffer.
 * Rules:
 *      whitespace is ignored and not cared for,
 *      special chars (operators, colons and grammer) are ignored,
 */
static enum holyc_parse_type 
holyc_parse_mode_string(char current_char, uint64_t location, char *chars, void *extra, void **next_func)
{
	switch (current_char) {
	}
	return HOLYC_PARSE_TYPE_JUNK;
}

/* 
 * Comment mode, activated when sequencial
 *
 */
static enum holyc_parse_type
holyc_parse_mode_comment(char current_char, uint64_t location, char *chars, void *extras, void **next_func)
{
	return HOLYC_PARSE_TYPE_JUNK;
}


int8_t 
holyc_parse_stream(char *buffer, uint32_t char_count, struct holyc_token **tokens, uint32_t *token_count)
{

	/* Pinsor used for analysis. */
	struct holyc_parse_pinsor p = { 0 };

	/* Reference to the current character being inspected. */
	char current;
				
	void *extra = (void *)malloc(sizeof(*extra));

	/* Start with the default callback defined. */
	holyc_parse_mode_func_t function = holyc_parse_mode_default;

	while ((current = *buffer++)) {

		/* Executes the function handling the result. */
		switch (function(current, p.right, buffer, extra, (void *)&function)) {
		}

		/* Incriment the right pinsor at the end of the loop. */
		++p.right;
	}
	free(extra);

	return 0;
}
