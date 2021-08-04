#include "args.h"


void
arguments_help(void)
{
	fprintf(stdout, "holyc: usage [-hv] [-h help] [-v version] file.hc\n");
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
	while ((opt = getopt(argc, (char *const *)argv, "hv")) != -1) {
		switch ((char)opt) {
			case 'h': {
				/* Display help. */	
				arguments_help();
				break;
			}
			case 'v': {
				/* Display the build version. */
				arguments_version();
				break;
			}
			default: {
				data->compiling = argv[argc - 1];
				return 1;
			}
		}
	}
	return -1;
}
