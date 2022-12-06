#include <stdio.h> // printf scanf
#include <stdlib.h> // malloc free exit
#include <errno.h> // errno
#include <unistd.h>
#include "union.h"
#include "func.h"


int main(int argc, char* argv[])
{
	if (argc <= 1) {
		fprintf(stderr, "%s", "\e[0;31m You should specify file name as argument!\n \e[0m");
		exit(EXIT_FAILURE);
	}

	union U u2;
	int size = sizeof(u2);
	unsigned char (*pencrypt)(unsigned char) = &encrypt; // pointer to function
	unsigned char *res =malloc(size); // dynamic memory
		if (res == NULL) {
		fprintf(stderr, "%s", "Error allocating memory.\n");
		exit(EXIT_FAILURE);
	}

	printf("\e[0;35m Let's Decrypt:\n \e[0m"); // Hello using ANSI color codes

	// File handling
	FILE* read_ptr;
	const char* fname = argv[1];
	if (access(fname, F_OK) == 0) { // file exist
		read_ptr = fopen(fname, "rb");
	}
	else {
		fprintf(stderr, "%s", "\e[0;31m File doesn't exist.\n \e[0m");
		free(res);
		exit(EXIT_FAILURE);
	}

	// file exist but we can't read it
	if (read_ptr == NULL) {
		fprintf(stderr, "Value of errno: %d\nError opening file\n", errno);
		free(res);
		exit(EXIT_FAILURE);
	}


	// Reading the file
	char c;
	int i = 0;
	while ( (c = fgetc(read_ptr)) != EOF ) {
		if ( i >= 28) { 
		fprintf(stderr, "%s", "\e[0;31m Wrong file.\n \e[0m");
		free(res);
		fclose(read_ptr);
		exit(EXIT_FAILURE);
		}
		if (feof(read_ptr)) {
			fprintf(stderr, "Error reading %s: unexpected end of file\nValue of errno: %d\n", fname, errno);
			free(res);
			fclose(read_ptr);
			exit(EXIT_FAILURE);
		}
		else if (ferror(read_ptr)) {
			fprintf(stderr, "Error reading file\nValue of errno: %d\n", errno);
			free(res);
			fclose(read_ptr);
			exit(EXIT_FAILURE);
		}
		u2.bytearr[i] = c;
		i++;
	}

	
	// printf("%s\t%f\t%f", u2.Area.title, u2.Area.x, u2.Area.y);

	process(u2.bytearr, u2.bytearr, size, pencrypt);
	printf("%s\t%f\t%f\n", u2.Area.title, u2.Area.x, u2.Area.y);

	free(res);
	fclose(read_ptr);
	return 0;
}
