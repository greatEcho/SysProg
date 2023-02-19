#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


void print(int**, int, int);
//int64_t maxofthree(int64_t, int64_t, int64_t);
//int addition(int);
//int** sarr(int**);
int32_t myarr(int32_t** arr, int32_t rows, int32_t cols);

int main(void)
{
    int32_t k = 3, n = 7;
    int32_t** arr = (int32_t**)malloc(sizeof(int32_t[k]));
    for (int32_t i=0; i<k; i++) {
        arr[i] = (int32_t*)malloc(sizeof(int32_t[n]));
    }
    int32_t count = 0;
    for (int32_t i=0; i<k; i++) {
        for (int32_t j=0;j<n;j++) {
            arr[i][j] = count++;
        }
    }
    printf("HEYY %d\n", arr[2][10]);

    // print 2D-array
    /*for (int i=0;i<k;i++) {
        for (int j=0;j<n;j++) {
            printf("%d\t", arr[i][j]);
            printf("%d\t", (*arr)+i*sizeof(int*));
            printf("%d\t", *(*arr+i*sizeof(int*))+j);
            printf("%d\n", arr[i]);
        }
        printf("\n");
    }*/
    print(arr, k, n);
    //printf("%ld\n", maxofthree(1, -2, 22)); 
    //printf("%d\n", addition(10));
    printf("%lx\n", *arr);
    printf("new\n");
    printf("%lx\n", myarr(arr, k, n));
    //printf("%d\n",sarr(arr));

    // free memory
    for (int32_t i=0;i<k;i++) {
        free(arr[i]);
    }
    free(arr);

    return 0;
}

void print(int** arr, int32_t k, int32_t n)
{
    for (int32_t i=0;i<k;i++) {
        for (int32_t j=0;j<n;j++) {
            printf("%d\n", *(*arr+i*sizeof(int32_t*))+j);
            printf("%d\n", arr[i][j]);
        }
    }
}
    
