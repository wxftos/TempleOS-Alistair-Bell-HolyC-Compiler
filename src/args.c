#include "args.h"


void
arguments_help(void)
{
	fprintf(stdout, "holyc: usage [-hvc:] [-h help] [-v version] [-c compile: file.hc]\n");
}
void
arguments_version(void)
{
	struct utsname u;
	uname(&u);
	fprintf(stdout, "holyc: version %s, platform %s, arch %s\n", HOLYC_BUILD_VERSION, u.sysname, u.machine);
}

int8_t
arguments_handle(const int32_t argc, const char **argv, struct arguments_data *data)
{
	int opt;
	while ((opt = getopt(argc, (char *const *)argv, "hvc:")) != -1) {
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
			}
			default: {
				break;
			}
		}
	}
	return 0;
}
