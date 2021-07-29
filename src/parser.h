#ifndef HOLYC_PARSER_H
#define HOLYC_PARSER_H

#include "inc.h"

struct holyc_token {
	uint32_t array_index;
	holyc_hash_t hash;
};

/* Turns the streams into tokens, populates the structures */
int8_t holyc_parse_stream(char *buffer, uint32_t char_count, struct holyc_token **tokens, uint32_t *token_count);


#endif
