/* 
 * Copyright (c) 2021 Alistair Bell <alistair@alistairbell.xyz>
 *
 * HolyC compiler is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
int arguments_handle(int, const char **, struct arguments_data *);

#endif
