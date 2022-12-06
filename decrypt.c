#include <stdio.h> // printf scanf
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "union.h"


unsigned char encrypt(unsigned char x)
{
	// Reverse high set of bits 1110 0101 -> 0001 0101
	unsigned char y = x ^ 0xF0; // 0xF0 bitmask
	return y;
}

// Get initial array ob bytes and target array, size and target function
void process(char* x_array, char* y_array, int size, unsigned char(f)(unsigned char))
{
	// use given function to each element of array
	for (int i = 0; i <= size; i++) {
		y_array[i] = f(x_array[i]);
	}
}


int main(int argc, char* argv[])
{
	if (argc <= 1) {
		fprintf(stderr, "%s", "\e[0;31m You should specify file name as argument!\n \e[0m");
		return 1;
	}

	union U u2;
	int size = sizeof(u2);
	unsigned char (*pencrypt)(unsigned char) = &encrypt; // pointer to function
	unsigned char *res =malloc(size); // dynamic memory

	printf("\e[0;35m Let's Decrypt:\n \e[0m"); // Hello

	// File handling
	FILE* read_ptr;
	const char* fname = argv[1];
	if (access(fname, F_OK) == 0) {
		read_ptr = fopen(fname, "rb");
	}
	else {
		fprintf(stderr, "%s", "\e[0;31m File doesn't exist.\n \e[0m");
		free(res);
		exit(EXIT_FAILURE);
	}

	// fread(u2.bytearr, size, sizeof(char), read_ptr); // Read from file
	// if (feof(read_ptr)) {
	// 	fprintf(stderr, "Error reading %s: unexpected end of file\nValue of errno: %d\n", fname, errno);
	// 	free(res);
	// 	fclose(read_ptr);
	// 	exit(EXIT_FAILURE);
	// }
	// else if (ferror(read_ptr)) {
	// 	fprintf(stderr, "Error reading file\nValue of errno: %d\n", errno);
	// 	free(res);
	// 	fclose(read_ptr);
	// 	exit(EXIT_FAILURE);
	// }

	// Reading file and Decryption
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
	// fprint("%x\n", u2.bytearr);

	
	// printf("%s\t%f\t%f", u2.Area.title, u2.Area.x, u2.Area.y);

	process(u2.bytearr, u2.bytearr, size, pencrypt);
	printf("%s\t%f\t%f\n", u2.Area.title, u2.Area.x, u2.Area.y);

	free(res);
	fclose(read_ptr);
	return 0;
}
