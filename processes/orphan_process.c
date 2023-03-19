#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void)
{
    pid_t child_pid = fork();
    if (child_pid == 0) {
        printf("Child process: pid=%d, ppid=%d\n", getpid(), getppid());
        sleep(40); // Sleep for 10 seconds to give parent process time to exit
        while(1);
        printf("Child process exiting\n");
    }  else {
        // Parent process
        sleep(10);
        printf("Parent process: pid=%d\n", getpid());
        printf("Parent process exiting\n");
        exit(EXIT_SUCCESS);
    }
    
    
    return 0;
}