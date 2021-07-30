#include "parser.h"

#define CRW(x)                                 \
	case (x): return HOLYC_PARSE_TYPE_WHITESPACE
#define CRS(x)                                 \
	case (x): return HOLYC_PARSE_TYPE_SPECIAL



static uint8_t
holyc_parse_is_whitespace(char is)
{
	switch (is) {
		case ' ': {
			return 1;
		}
		case '\0': {
			return 1;
		}
		case '\n': {
			return 1;
		}
		default: {
			return 0;
		}
	}
}

enum holyc_parse_type 
holyc_parse_mode_default(char current_char, uint64_t location, char *chars, void *extra, void **next_func)
{
	switch (current_char) {
		/* Whitepsace chars. */
		CRW(' ');
		CRW('\n');

		/* Special chars which have a single function. */
		CRS('+');
		CRS('-');
		CRS('/');
		CRS('&');


		/* Special situations */
		case '*': {
			/* Start of a multiline comment. */
			if (location != 0 && chars[location - 1] == '/') {
				/* Update to comment mode. */
				*next_func = holyc_parse_mode_comment;		
				return HOLYC_PARSE_TYPE_JUNK;
			}
			return HOLYC_PARSE_TYPE_JUNK;
		}
	
		default: {
			return HOLYC_PARSE_TYPE_JUNK;
		}
	}
}

enum holyc_parse_type 
holyc_parse_mode_string(char current_char, uint64_t location, char *chars, void *extra, void **next_func)
{
	/* 
	 * The comment mode extra is a single integer.
	 * 0 = single line.
	 * 1 = multiline.
	 */

	uint8_t mode = *((uint8_t *)extra);
	switch (mode) {
		case 0: {
			if (current_char == '\n') {
				*next_func = holyc_parse_mode_default;
			}
		}
	}

	return HOLYC_PARSE_TYPE_JUNK;
}

enum holyc_parse_type
holyc_parse_mode_comment(char current_char, uint64_t location, char *chars, void *extras, void **next_func)
{
	fprintf(stdout, "holyc: inside comment mode %c\n", current_char);
	/* When this one is called assume that it is at the start of a comment. */
	switch (current_char) {

		default: {
			return HOLYC_PARSE_TYPE_JUNK;
		}
	}
}

int8_t 
holyc_parse_stream(char *buffer, uint32_t char_count, struct holyc_token **tokens, uint32_t *token_count)
{

	/* Pinsor used for analysis. */
	struct holyc_parse_pinsor p = { 0 };

	/* Reference to the previous character which was inspected. */
	char current, previous = '\0';
	
	/* Buffer to hold under construction tokens, 64 chars . */
	char *under_construction = (char *)malloc(sizeof(*under_construction) * HOLYC_UNDER_CONSTRUCTION_COUNT);
	memset(under_construction, 0, sizeof(*under_construction) * HOLYC_UNDER_CONSTRUCTION_COUNT);

				 
	/* Daisy chain data that can be passed onto the next function call to give special data about the chars. */
	void *extra = (void *)malloc(sizeof(*extra));

	/* Start with the default callback defined. */
	holyc_parse_mode_func_t function = holyc_parse_mode_default;

	char *buffer_cpy = buffer;
	while ((current = *buffer_cpy++)) {

		++p.right;
		
		/* Executes the function handling the result. */
		switch (function(current, p.right, buffer, extra, (void *)&function)) {
			case HOLYC_PARSE_TYPE_WHITESPACE: {
				/* Ignore consequtive whitespace. */
				if (holyc_parse_is_whitespace(previous)) {
					p.left = p.right;
					break;	
				}

				strncpy(under_construction, buffer + p.left, p.right - p.left - 1);
				fprintf(stdout, "holyc: %s\n", under_construction);
				memset(under_construction, 0, sizeof(*under_construction) * HOLYC_UNDER_CONSTRUCTION_COUNT);
				/* Add the token to the list. */
				p.left = p.right;
				break;
			}

			default:{ 
				break;
			}
		}
		previous = current;
	}
	free(extra);
	free(under_construction);

	return 0;
}
