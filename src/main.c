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
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "inc.h"
#include "util.h"
#include "args.h"
#include "lexer.h"
#include "parser.h"

/* Opens the files, validates its present and not a dir, reads the chars into the buffer. */
static int 
handle_file(const char *file, char **chars, unsigned long *char_count)
{
	struct stat sbuff = { 0 };
	if (stat(file, &sbuff) < 0) {
		fprintf(stderr, "error: unable to find file \'%s\', does it exist?\n", file);
		return -1;
	}
	if (S_ISDIR(sbuff.st_mode)) {
		fprintf(stderr, "error: \'%s\' is directory, cannot compile a directory!\n", file);
		return -1;
	}

	FILE *f = fopen(file, "r");
	if (f == NULL) {
		register int snapshot = errno;
		fprintf(stderr, "error: fopen failed on \'%s\', %s.\n", file, strerror(snapshot));	
		return -1;
	}
	
	(void)fseek(f, 0, SEEK_END);
	*char_count = ftell(f);
	*chars = (char *)malloc((*char_count + 1) * sizeof(char));
	/* Rewind the file pointer. */
	rewind(f);
	/* Read our data into the char buffer. */
	(void)fread((void *)(*chars), (*char_count + 1) * sizeof(char), 1, f);
	/* Close the file handle. */
	fclose(f);
	return 1;
}


int
main(int argc, const char **argv)
{
	/* Check that we actually got an argument. */
	if (argc < 2) {
		arguments_help();	
		return 1;
	}

	struct arguments_data data = { 0 };
	/* Misc flags return -1 for no continue. */
	int result;
	if ((result = arguments_handle(argc, argv, &data)) < 0) {
		if (result == -2) {
			arguments_help();
		}
		return 1;
	}

	/* 
	 * Read the source file.
	 * Fetch the chars that we can parse.
	 */
	const char *target = data.compiling;

	char *chars = NULL;
	unsigned long char_count = 0;
	if (handle_file(target, &chars , &char_count) < 0) {
		return 1;
	}

	/*
	 * Call the parse function.
	 * This turns the chars into a series of identified tokens.
	 * The parser is smart and will not split strings and can identify strings against numerical constants.
	 */
	struct token *tokens; 
	unsigned int token_count = 0;
	if (lex_chars(chars, &tokens, &token_count) < 0) {
		return 1;
	}

	parse_tokens(tokens, token_count, chars);


	free(tokens);
	free(chars);
	return 0;
}
