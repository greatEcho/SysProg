#include <stdio.h>

int main()
{
    int arr[4][3] = {{1,2,3}, {2,3,4}, {3,4,5}, {4,5,6}};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            printf("arr[%d][%d] = %i\n", i, j, arr[i][j]);
        }
    }
    return 0;
}

