#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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
	       	

int main(int argc, char* argv[])
{
	if (argc <= 1) {
		fprintf(stderr, "%s", "\e[0;31m You should specify file name as argument!\n \e[0m");
		return 1;
	}

	union U u1;
	int size = sizeof(u1);
	unsigned char (*pencrypt)(unsigned char) = &encrypt; // pointer to function
	unsigned char *res = malloc(size); // dynamic memory

	printf("\e[0;35m Let's Encrypt:\n \e[0m"); // Hello

	// User input
	scanf("%s", u1.Area.title);
	scanf("%f %f", &u1.Area.x, &u1.Area.y);

	FILE *write_ptr;
	write_ptr = fopen("plaintext", "wb");
	fwrite(u1.bytearr, size, sizeof(char), write_ptr);
	process(u1.bytearr, res, size, pencrypt);


	//FILE *write_ptr;
	const char* fname = argv[1];
	write_ptr = fopen(fname, "wb");

	if (access(fname, W_OK) == 0) {
		write_ptr = fopen(fname, "wb");
	}
	else {
		fprintf(stderr, "%s", "\e[0;31m File doesn't exist.\n \e[0m");
		exit(EXIT_FAILURE);
	}

	fwrite(res, size, sizeof(char), write_ptr);
	if (feof(write_ptr)) {
		fprintf(stderr, "Error reading %s: unexpected end of file\nValue of errno: %d\n", fname, errno);
		fclose(write_ptr);
		exit(EXIT_FAILURE);
	}
	else if (ferror(write_ptr)) {
		fprintf(stderr, "Error reading file\nValue of errno: %d\n", errno);
		fclose(write_ptr);
		exit(EXIT_FAILURE);
	}

	printf("%s\t%f\t%f\n", u1.Area.title, u1.Area.x, u1.Area.y);

	for (int i = 0; i < size; i++) {
		printf("%x ", u1.bytearr[i]);
	}
	free(res);
	return 0;
}
