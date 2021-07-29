#include "util.h"

holyc_hash_t
holyc_hash_chars(char *chars)
{

	/* Running of the hash first initialsed for bitshifts to work on the subsequent passes. */
	holyc_hash_t total = 5381;

	char c;
	while ((c = *chars++)) {
		total += ((total << 6) + total) + c;
	}
	return total;
}
