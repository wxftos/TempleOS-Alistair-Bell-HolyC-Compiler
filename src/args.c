/* 
 * Copyright (c) 2021-2022 Alistair Bell <alistair@alistairbell.xyz>
 *
 * HolyC compiler is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//#include <getopt.h>

#include "args.h"
#define HOLYC_BUILD_VERSION "0.0.17"
#define ARGUMENTS_HELP_STRING "c:hv"
#define ARGUMENTS_HELP_PRETTY "chv"
char*__progname;
void
arguments_help(void)
{
	fprintf(stderr, "usage: %s [-%s] [-c to_compile] [-h help] [-v version].\n", __progname, ARGUMENTS_HELP_PRETTY);
}
void
arguments_version(void)
{
	#if 0
	struct utsname u;
	uname(&u);
	fprintf(stdout, "version: %s, platform %s, runtime arch %s.\n", HOLYC_BUILD_VERSION, u.sysname, u.machine);
	#endif
	fprintf(stdout, "version: %s, platform windows, runtime arch x86.\n", HOLYC_BUILD_VERSION);
}
int
arguments_handle(const int argc, const char **argv, struct arguments_data *data)
{
	/* Disable error message, use custom instead. */
	int opterr = 0;
#if 0
	int opt;
	while ((opt = getopt(argc, (char *const *)argv, ARGUMENTS_HELP_STRING)) != -1) {
		switch ((char)opt) {
			case 'h': {
				/* Display help. */	
				arguments_help();
				return -1;
			}
			case 'v': {
				/* Display the build version. */
				arguments_version();
				return -1;
			}
			case 'c': {
				data->compiling = optarg; 
				return 1;
			}
			default: {
				fprintf(stderr, "error: unrecognised or invalid usage of flag -%c, run whith -h for a list of options.\n", (char)optopt);
				return -1;
			}
		}
	}
#endif
	return -2;
}
