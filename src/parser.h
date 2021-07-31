#ifndef HOLYC_PARSER_H
#define HOLYC_PARSER_H

#include "inc.h"

enum holyc_parse_type {
	/* Junk means non special to the parser, regular chars, used for starting tokens after specials. */
	HOLYC_PARSE_TYPE_JUNK,
	/* Includes spacees, newlines and tabs, also used for ending tokens. */
	HOLYC_PARSE_TYPE_WHITESPACE,
	/* Special includes grammer of the language and will split up tokens if found within (depends on mode). */
	HOLYC_PARSE_TYPE_SPECIAL,
};

/* Tempory chars buffer amount of char count. */
#define HOLYC_UNDER_CONSTRUCTION_COUNT 64


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
