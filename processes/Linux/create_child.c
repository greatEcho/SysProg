#include <stdio.h>
#include <unistd.h>

int main(void)
{
    if (fork() == 0) {
        printf("I'm a child. %d\n", getpid());
    }
    else {
        printf("I'm a parent process. %d\n", getpid());
    }

    return 0;
}