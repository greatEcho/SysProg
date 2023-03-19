#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>


int main(void)
{
    int status;
    pid_t wpid;
    pid_t pid = fork();
    if (pid == 0) execle("/bin/ls","-a", "exec.c", NULL);
    else {
        //wpid = waitpid(pid, &status, 0);
    }
    printf("wpid -> %d\n", wpid);
    printf("my child pid -> %d\n", pid);
    printf("my pid -> %d\n", getpid());

    return 0;
}