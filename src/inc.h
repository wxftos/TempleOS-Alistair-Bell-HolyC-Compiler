#ifndef HOLYC_INC_H
#define HOLYC_INC_H

/*
 * File handles the includes for all the project and defines useful macros.
 * The build system really should precompile the header for decreased build times.
 * Header should only include very useful stuff else put it in another file.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utsname.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

#define ARRAY_LEN(arr)  \
	sizeof(arr) / sizeof((arr)[0])


/* The hash type of the strings, easy upgradability. */
typedef uint32_t holyc_hash_t;

/* Max value for the hash, needs to be updated on type change. */
#define HOLYC_HASH_T_MAX UINT32_MAX


#endif
