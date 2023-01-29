#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int k = 3, n = 4;
    int* arr = malloc( (k * n) * sizeof(int));

    for (int i = 0; i < k * n; i++) {
        arr[i] = i + 1;
        printf("%d\n", arr[i]);
    }
        
    
    free(arr);
    return 0;
}
