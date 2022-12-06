#ifndef FUNC_H
#define FUNC_H

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

#endif