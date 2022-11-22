#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

union U {
	struct {
		unsigned char title[20];
		float x;
		float y;
	} Area;
	unsigned char bytearr[28];
};

unsigned char encrypt(unsigned char x)
{
	unsigned char y = x ^ 0xF0; // 0xF0 bitmask
	return y;
}

void process(char* x_array, char* y_array, int size, unsigned char(f)(unsigned char))
{
	for (int i = 0; i <= size; i++) {
		y_array[i] = f(x_array[i]);
	}
}


int main(int argc, char argv[])
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
	const char* fname = argv[2];
	if (access(fname, F_OK) == 0) {
		read_ptr = fopen(fname, "rb");
	}
	else {
		fprintf(stderr, "%s", "\e[0;31m File doesn't exist.\n \e[0m");
	}


	fread(u2.bytearr, size, 1, read_ptr);
	fclose(read_ptr);


	process(u2.bytearr, res, size, pencrypt);

	return 0;
}
