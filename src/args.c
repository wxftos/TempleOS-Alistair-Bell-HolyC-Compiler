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

#include "args.h"

#define ARGUMENTS_HELP_STRING "hvlc:"

void
arguments_help(void)
{
	fprintf(stdout, "holyc: usage [-%s] [-h help] [-v version] [-l license] [-c compile: file.hc].\n", ARGUMENTS_HELP_STRING);
}
void
arguments_version(void)
{
	struct utsname u;
	uname(&u);
	fprintf(stdout, "holyc: version %s, platform %s, arch %s.\n", HOLYC_BUILD_VERSION, u.sysname, u.machine);
}
void
arguments_license(void)
{
	fprintf(stdout, "holyc: license GPLv3, this program comes with ABSOLUTELY NO WARRANTY. This is free software, visit https://www.gnu.org/licenses/gpl-3.0.html for more info.\n");
}

int8_t
arguments_handle(const int32_t argc, const char **argv, struct arguments_data *data)
{
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
			case 'l': {
				arguments_license();
				return -1;
			}
			case 'c': {
				data->compiling = optarg; 
				return 1;
			}
			default: {
				break;
			}
		}
	}
	return -2;
}
