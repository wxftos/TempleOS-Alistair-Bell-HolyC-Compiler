#include "args.h"


void
arguments_help(void)
{
<<<<<<< HEAD
	fprintf(stdout, "holyc: usage [-hvc:] [-h help] [-v version] [-c compile: file.hc]\n");
=======
	fprintf(stdout, "holyc: usage [-hv] [-h help] [-v version] file.hc\n");
>>>>>>> 9c08ed93174eaa496552e5195ee94aea10939317
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
<<<<<<< HEAD
	while ((opt = getopt(argc, (char *const *)argv, "hvc:")) != -1) {
=======
	while ((opt = getopt(argc, (char *const *)argv, "hv")) != -1) {
>>>>>>> 9c08ed93174eaa496552e5195ee94aea10939317
		switch ((char)opt) {
			case 'h': {
				/* Display help. */	
				arguments_help();
<<<<<<< HEAD
				return -1;
=======
				break;
>>>>>>> 9c08ed93174eaa496552e5195ee94aea10939317
			}
			case 'v': {
				/* Display the build version. */
				arguments_version();
<<<<<<< HEAD
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
=======
				break;
			}
			default: {
				data->compiling = argv[argc - 1];
				return 1;
			}
		}
	}
	return -1;
>>>>>>> 9c08ed93174eaa496552e5195ee94aea10939317
}
