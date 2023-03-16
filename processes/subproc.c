#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    printf("%d", argc);
    if (argc != 3) {
        printf("Error in usage of child process.\n");
        return EXIT_FAILURE;
    }
    int x = atoi(argv[1]);
    printf("Subproc.. -> %s\n", x);
    return 0;
}