#include <stdio.h> // printf scanf
#include <stdlib.h> // malloc free exit
#include <errno.h> // errno
#include "union.h"
#include "func.h"
	       	

int main(int argc, char* argv[])
{
	if (argc <= 1) {
		fprintf(stderr, "%s", "\e[0;31m You should specify file name as argument!\n \e[0m");
		exit(EXIT_FAILURE);
	}

	union U u1;
	int size = sizeof(u1);
	unsigned char (*pencrypt)(unsigned char) = &encrypt; // pointer to function
	unsigned char *res = malloc(size); // dynamic memory
	if (res == NULL) {
		fprintf(stderr, "%s", "Error allocating memory.\n");
		exit(EXIT_FAILURE);
	}

	printf("\e[0;35m Let's Encrypt:\n \e[0m"); // Hello using ANSI color codes

	// User input
	scanf("%s", u1.Area.title);
	scanf("%f %f", &u1.Area.x, &u1.Area.y);

	
	
	/* Test Presentation (Save plaintext) */
	FILE *test_ptr; 
	test_ptr = fopen("plaintext", "wb");
	fwrite(u1.bytearr, size, sizeof(char), test_ptr);

	// Encryption
	process(u1.bytearr, res, size, pencrypt); 

	// File handling
	FILE *write_ptr;
	const char* fname = argv[1];
	write_ptr = fopen(fname, "wb");
	if (write_ptr == NULL) {
		fprintf(stderr, "Value of errno: %d\nError opening file\n", errno);
		free(res);
		exit(EXIT_FAILURE);
	}

	// Legit write file stream
	fwrite(res, size, sizeof(char), write_ptr); // Write into file

	// printf("%s\t%f\t%f\n", u1.Area.title, u1.Area.x, u1.Area.y);

	// print plaintext as bytes
	for (int i = 0; i < size; i++) {
		printf("%x ", u1.bytearr[i]);
	}
	free(res);
	fclose(write_ptr);
	return 0;
}
