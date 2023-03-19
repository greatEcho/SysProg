#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: %s <integer> <string>\n", argv[0]);
        return 1;
    }

    int arg1 = atoi(argv[1]);
    char *arg2 = argv[2];

    printf("arg1: %d\n", arg1);
    printf("arg2: %s\n", arg2);

    return 0;
}