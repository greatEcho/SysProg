#include <stdio.h>
#include <unistd.h>

int main(void)
{
    fork();
    printf("Hello, World! %d\n", getpid());
    return 0;
}