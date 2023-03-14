#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // syscall read write
#include <fcntl.h> //syscall open close
#include <errno.h>
#include <string.h>
#include <sys/stat.h> // file permissions

#define BUFFER_SIZE 22

int main(int argc, char* argv[])
{
    // check if args are passed
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // open input file
    int input_fd = open(argv[1], O_WRONLY);
    if (input_fd < 0) {
        // perror("Error: ");
        char* errMsg = strerror(errno);
        // printf("%d\n", strlen(errMsg));
        write(STDERR_FILENO, "Error (input): ", 16);
        write(STDERR_FILENO, errMsg, strlen(errMsg));
        return EXIT_FAILURE;
    }
    // open output file
    int output_fd = open(argv[2], O_WRONLY); // S_IRWXU = 00700
    if (output_fd < 0) {
        // perror("Error: output");
        char* errMsg = strerror(errno);
        write(STDERR_FILENO, "Error (output): ", 17); // 17 - msg length
        write(STDERR_FILENO, errMsg, strlen(errMsg));
        close(input_fd); // free fd
        return EXIT_FAILURE;
    }
    
    // allocate memory
    char* buffer = (char*)malloc(BUFFER_SIZE);
    // if memory were not allocated
    if (!buffer) {
        char* errMsg = strerror(errno);
        write(STDERR_FILENO, "Error: ", 8); // 17 - msg length
        write(STDERR_FILENO, errMsg, strlen(errMsg));
        close(input_fd); // free fd
        close(output_fd);
        return EXIT_FAILURE;
    }

    // count bytes
    int bytes_read = 0;
    int file_length = 0; 
    // read a file
    while ( (bytes_read = read(input_fd, buffer, BUFFER_SIZE)) ) {
        file_length += bytes_read;
        //write(STDOUT_FILENO, buffer, bytes_read); // print data
    }

    // convert int to str
    char str[33]; // up to 1 Gb
    int len = sprintf(str, "%d", file_length);

    // write in a file
    int bytes_written;
    if ((bytes_written = write(output_fd, str, len)) < len) {
//        perror("write");    
        if (bytes_written < 0) {
            char* errMsg = strerror(errno);
            write(STDERR_FILENO, "Error: ", 8); // 17 - msg length
            write(STDERR_FILENO, errMsg, strlen(errMsg));
        }
        else {
            char* msg = "Error: Can't write data in the file.\n";
            write(STDERR_FILENO, msg, sizeof(msg));
        }
        close(input_fd);
        close(output_fd);
        free(buffer);
        return EXIT_FAILURE;
    }  
    
    /* sizeof - with '\0', strlen - w/o '\0' */
    // int bytes_written = write(input_fd, buffer, sizeof(buffer));
    // printf("%d\n", bytes_written);

    // free memory
    close(input_fd);
    close(output_fd);
    free(buffer);
    buffer = NULL;

    return 0;
}