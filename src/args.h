#ifndef HOLYC_ARGS_H
#define HOLYC_ARGS_H

#include "inc.h"

/* 
 * Header and source file responsible for handling the command line arguments for the program.
 * Arguments are parsed and some which result in no compiling terminates the program after writing to stdout (-h, -v, etc).
 * The arguments follow the bsd style of flags.
 */


struct arguments_data {
	const char *compiling;
};

/* Displays a list of the possible arguments. */
void arguments_help(void);
/* Displays the build version along with the current os release and architecture. */
void arguments_version(void);
/* Displays the license of the progam. */
void arguments_license(void);
/* Runs the argument parser, returns < 0 on no continue, returns the usefull stuff in the struct buffer passed in. */
int8_t arguments_handle(int32_t, const char **, struct arguments_data *);

#endif
