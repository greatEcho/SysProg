#include <stdio.h>
#include <errno.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    int file = open("hello", O_RDONLY);
    if (file < 0) {
        printf("Error %d\n", errno);
        
        return 1;
    }
}