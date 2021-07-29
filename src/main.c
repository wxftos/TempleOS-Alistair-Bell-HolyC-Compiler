#include "inc.h"

static int8_t 
holyc_handle_file(const char *file, const char *mode, char **char_buffer, int32_t *char_count)
{
	/* Stat the file to check that it exists. */
	struct stat st;
	if (stat(file, &st) != 0) {
		fprintf(stderr, "holyc: error unable to find file %s, does it exist?\n", file);	
		return ENOENT;
	}
	/* Check that it actually has blocks. */
	if (st.st_blocks <= 0) {
		fprintf(stdout, "holyc: warning file %s lacks bytes, pointless to compile an empty file!\n", file);	
	}

	/* Validate that it is not a directory. */
	if (S_ISDIR(st.st_mode)) {
		fprintf(stderr, "holyc: error %s is a directory, please specify an actual source file!\n", file);
		return ENOENT;
	}

	FILE *f = fopen(file, mode);

	fseek(f, 0, SEEK_END);
	*char_count = ftell(f);
	rewind(f);

	/* 
	 * Allocate the buffer's memory.
	 * Zero the new memory.
	 * fread into the buffer returning success
	*/
	size_t buffer_size = sizeof(*char_buffer) * (*char_count);
	*char_buffer = (char *)malloc(buffer_size);
	memset(*char_buffer, 0, buffer_size);

	fread((void *)(*char_buffer), buffer_size, sizeof(*char_buffer), f);
	return 0; 
}

int
main(int argc, const char **args)
{
	/* Check that we actually got an argument */
	if (argc < 2) {
		fprintf(stderr, "holyc: error no input files!\n");
		return 1;
	}

	/* Read the source file.
	 * Fetch the bytes that we can parse.
	 */

	char *bytes = NULL;
	int32_t chars = 0;
	if (holyc_handle_file(args[1], "r", &bytes, &chars) != 0) {
		return 1;
	}
	fprintf(stdout, "Bytes:\n%s", bytes);

	free(bytes);
	return 0;
}
