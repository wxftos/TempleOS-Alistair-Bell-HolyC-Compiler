#ifndef HOLYC_UTIL_H
#define HOLYC_UTIL_H

#include "inc.h"

/*
 * Hashing algorithm currently uses djb2, upgrade for dictionary conflicts may be neccessary!!
 */


/* Hashes a character for better finding of tokens. */
holyc_hash_t holyc_hash_chars(char *buff);

#endif
