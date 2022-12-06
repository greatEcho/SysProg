#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

union U {
	// Area with title and coordinates
	struct {
		unsigned char title[20];
		float x;
		float y;
	} Area;
	// Array of bytes that shares memory space with Area
	unsigned char bytearr[28];
};

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



	FILE* read_ptr;
	const char* fname = argv[1];

	if (access(fname, F_OK) == 0) {
		read_ptr = fopen(fname, "rb");
	}
	/*
	else {
		fprintf(stderr, "%s", "\e[0;31m File doesn't exist.\n \e[0m");
		exit(EXIT_FAILURE);
	}

	fread(u2.bytearr, size, sizeof(char), read_ptr);
	if (feof(read_ptr))
        	printf("Error reading %s: unexpected end of file\n", fname);
	else if (ferror(read_ptr)) {
        	perror("Error reading file\n");
	}
	*/
	char c;
	printf("Here we go\n");

	// Reading file and Decryption
	int i = 0;
	while ( (c = fgetc(read_ptr)) != EOF) {
//		c = encrypt(c);
		u2.bytearr[i] = c;
		//printf("ok");
	//	printf("%c ", c);
		i++;
	}
	fclose(read_ptr);
	// fprint("%x\n", u2.bytearr);

	
	// printf("%s\t%f\t%f", u2.Area.title, u2.Area.x, u2.Area.y);

	process(u2.bytearr, u2.bytearr, size, pencrypt);
	printf("%s\t%f\t%f", u2.Area.title, u2.Area.x, u2.Area.y);

	return 0;
}
