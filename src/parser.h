#ifndef HOLYC_PARSER_H
#define HOLYC_PARSER_H

#include "inc.h"

enum holyc_parse_type {
	/* Junk means non special to the parser, regular chars, used for starting tokens after specials. */
	HOLYC_PARSE_TYPE_JUNK,
	/* Includes spacees, newlines and tabs, also used for ending tokens. */
	HOLYC_PARSE_TYPE_WHITEPSACE,
	/* Special includes grammer of the language and will split up tokens if found within (depends on mode). */
	HOLYC_PARSE_TYPE_SPECIAL,
};

/* 
 * This is a function used by the parser to determine whether the char is special and requires it's own token.
 * If a token changes the interpreter mode (str or comment) then the callback is changed to account for this.
 * Params:
 *		1: current character being analyased,
 *      2: index along the buffer (right claw of the pinsor),
 *		3: the entire source (if previous lookup required),
 *      4: extra data the the mode may be interested in, (e.g. comment mode single line vs multi)
 *		5: pointer to the next function pointer to be used for analyses, updated if analysing requires it.
 * The function pointer pointer is generic but the implimentation must use the correct typing else trouble is inevitable. 
 */
typedef enum holyc_parse_type(*holyc_parse_mode_func_t)(char, uint64_t, char *, void *, void **);

/* 
 * For analysing the stream it each char is analysed for special chars or whitespace / newlines.
 * The right pinsor always advances until no more chars.
 * When a special char is found the substr of the buffer is retrieved using offsets calculated by the pinsor.
 * Then when the token is added the left is set to the right pinsor repeating until no more.
 */
struct holyc_parse_pinsor {
	uint64_t left;
	uint64_t right;
};

/* 
 * Token is a sudo representation of the string it derives from.
 * A token is used for parsing and creating expressions to then convert into a series of instructions.
 * Each token stores the starting index of the character within the buffer, knowledge of the next or the char count can get the origional string.
 */
struct holyc_token {
	holyc_hash_t hash;
	uint64_t start_char_index;
};

/* Turns the streams into tokens, populates the structures */
int8_t holyc_parse_stream(char *, uint32_t, struct holyc_token **, uint32_t *);


#endif
