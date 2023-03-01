#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define ARGC 3
#define ARG_MIN 1


void print(int** arr, int k, int n); // to print 2D array

// asm function
int32_t myarr(int32_t** arr, int32_t rows, int32_t cols);

int main(int argc, char* argv[])
{
    // check if args are passed and valid
    if (argc != ARGC) {
        fprintf( stderr, "Usage: %s k n\n"
                        "where k is a number of rows, "
                        "n is a number of columns.\n"
                        "These numbers should be positive.\n", argv[0]);
        return EXIT_FAILURE;
    }

    int i;
    char* endptr;
    for (i = 1; i < ARGC; i++) {
        int32_t value = strtol(argv[i], &endptr, 10);
        if (*endptr != '\0' || value > INT32_MAX || value < ARG_MIN) {
            fprintf( stderr, "Error! Invalid input: %s", argv[i]);
            return EXIT_FAILURE;
        }
    }

    // size of matrix
    const int32_t k = strtol(argv[1], NULL, 10), n = strtol(argv[2], NULL, 10);

    // allocate memory
    int32_t** arr;
    if ( (arr = (int32_t**) malloc(sizeof(int32_t*[k]))) == NULL) {
        fprintf( stderr, "%s", "Error! Can't allocate memory.\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < k; i++) {
        if ( (arr[i] = (int32_t*) malloc(sizeof(int32_t[n]))) == NULL) {
            fprintf( stderr, "%s", "Error! Can't allocate memory.\n");
            return EXIT_FAILURE;
        }
        for (int32_t j = 0; j < n; j++)
            arr[i][j] = rand() % 50;
    }


    // print array
    print(arr, k, n);
    //printf("%p\n", arr);
//    printf("--\n%p\n%p\n%p\n--\n", arr[0], arr[1], arr[2]);

    // call asm function
    printf("\nResult: %d\n", myarr(arr, k, n));

    // free memory
    for (i=0;i<k;i++) {
        free(arr[i]);
        arr[i] = NULL;
    }
    free(arr);
    arr = NULL;

    return 0;
}

void print(int** arr, int32_t k, int32_t n)
{
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            //printf("%d\n", *(*arr+i*sizeof(int32_t*))+j);
            printf("%d\t", arr[i][j]);
        }
        printf("\n");
    }
}


