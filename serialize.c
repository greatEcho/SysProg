#include <stdio.h>
#include <stdlib.h>

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
	union U u1;
	int size = sizeof(u1);
	unsigned char (*pencrypt)(unsigned char) = &encrypt; // pointer to function
	unsigned char *res = malloc(size); // dynamic memory

	printf("\e[0;35m Let's Encrypt:\n \e[0m"); // Hello

	// User input
	scanf("%s", u1.Area.title);
	scanf("%f %f", &u1.Area.x, &u1.Area.y);

	process(u1.bytearr, res, size, pencrypt);


	FILE *write_ptr;
	write_ptr = fopen("cipher", "wb");
	fwrite(res, size, 1, write_ptr);

	printf("%s\t%f\t%f", u1.Area.title, u1.Area.x, u1.Area.y);


	free(res);
	return 0;
}
