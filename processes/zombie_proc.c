#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(void)
{
    size_t i = 0;
    while (i++ < 10) {
        if (fork() == 0) {
            printf("child started..and done! (%d)\n", getpid());
            return 0;
        } else {
            printf("parent just won't die..\n");
            sleep(5);
        }
    }
}