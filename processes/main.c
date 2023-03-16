#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

#define INPUT_FILE text
#define NUM_CHILD 2

int main(void)
{
    int i = 1;
    char* str;
    printf("Res: %s\n", str);
    for (size_t i = 0; i < NUM_CHILD; i++) {
        pid_t pid = fork(); /* create child process */
        sprintf(str, "%d", i);
        execl("./subproc", str, NULL); /* child process runs new program */ 

    }

    __pid_t res = fork();
    switch(res) {
        case 0: {
            execve("./subproc", NULL, NULL);
        }
        case -1: {
            printf("Error %d\n", errno);
            return 1;
        }
        default:
            __pid_t res = fork();
            execve("./subproc", NULL, NULL);
            return 0;
    }
    

    return 0;
}