#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


void square(int**, int, int);
//int64_t maxofthree(int64_t, int64_t, int64_t);
//int addition(int);
int** sarr(int**);

int main(void)
{
    int k = 3, n = 5;
    int** arr = (int**)malloc(sizeof(int[k]));
    for (int i=0; i<k; i++) {
        arr[i] = (int*)malloc(sizeof(int[n]));
    }
    int count = 0;
    for (int i=0; i<k; i++) {
        for (int j=0;j<n;j++) {
            arr[i][j] = ++count;
        }
    }

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
    //square(arr, k, n);
    //printf("%ld\n", maxofthree(1, -2, 22)); 
    //printf("%d\n", addition(10));
    printf("%d\n", *arr);
    printf("%d\n",sarr(arr));

    // free memory
    for (int i=0;i<k;i++) {
        free(arr[i]);
    }
    free(arr);

    return 0;
}

void square(int** arr, int k, int n)
{
    for (int i=0;i<k;i++) {
        for (int j=0;j<n;j++) {
            printf("%d\n", *(*arr+i*sizeof(int*))+j);
        }
    }
}
    
