#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>
#include <string.h>
#include <sys/types.h>
#include <inttypes.h>

#define ARGC 3
#define ARG_MIN 1
#define FILENAME_SIZE 32

void read_input_file(FILE** fp, char** buffer, size_t* file_size, const char* filename);
void create_file_chunks(char* buffer, long file_size, const size_t num_child);

int main(int argc, char* argv[])
{
    // system("./del_chunk.sh");
    // check if args are passed and valid
    if (argc != ARGC) {
        fprintf( stderr, "Usage: %s <input_file> <num_of_processes>\n", argv[0]);
        return EXIT_FAILURE;
    }
    // check if third argument is valid
    char* endptr;
    size_t num_child = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || num_child > INT32_MAX || num_child < ARG_MIN) {
        fprintf( stderr, "Error! Invalid input: %s", argv[2]);
        return EXIT_FAILURE;
    }

    // open and read the input file
    FILE* input_file;
    char* buffer;
    size_t file_size;
    read_input_file(&input_file, &buffer, &file_size, argv[1]);
    if (file_size < num_child) {
        num_child = file_size / 2;
        fprintf( stderr, "Warning: The number of child processes is too high. "
                         "The altered value will be %ld.\n", num_child);
    }
    // create file chunks
    create_file_chunks(buffer, file_size, num_child);


    // create child processes
    char chunk_filename[FILENAME_SIZE];
    pid_t child_pid, wpid;
    int status;
    for (size_t i = 0; i < num_child; i++) {
        sprintf(chunk_filename, "chunk%ld", i);
        // create child process copy
        child_pid = fork();
        switch(child_pid) {
        case -1:
            perror("Fork failed");
            exit(EXIT_FAILURE);
        case 0:
            // Child process 
            // printf("Now we start child process..\n");
            printf("Child pid -> %d\n", getpid());
            if ( execl("./subproc", "subproc", chunk_filename, NULL) == -1 ) {
                perror("Starting child process failed");
                exit(EXIT_FAILURE);
            }
            // these code will be never reached
            printf("Child process finished.\n");
            exit(0);
        // default:
            // Parent process
            //printf("Parent process waiting for children...\n");
            // do {
            //     wpid = waitpid(child_pid, &status, WUNTRACED);
            //     if (wpid == -1) {
            //         perror("Waitpid Error");
            //         exit(1);
            //     }
            //     //size_t check = WEXITSTATUS(status);
            //     //printf("%ld\n", check);
            //     if (WIFEXITED(status) && WEXITSTATUS(status)) { fprintf(stderr, "Child failed.\n"); exit(EXIT_FAILURE); }

            // } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

    }
    for (size_t i = 0; i < num_child; i++) {
            do {
                wpid = waitpid(-1, &status, WUNTRACED);
                if (wpid == -1) {
                    perror("Waitpid Error");
                    exit(1);
                }
                //size_t check = WEXITSTATUS(status);
                //printf("%ld\n", check);
                if (WIFEXITED(status) && WEXITSTATUS(status)) { fprintf(stderr, "Child failed.\n"); exit(EXIT_FAILURE); }

            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    printf("Parent process continuing...\n");
    printf("Parent pid -> %d\n", getpid());

    // read results from children
    size_t result = 0;
    for (size_t i = 0; i < num_child; i++) {
        sprintf(chunk_filename, "chunk%ld_result", i);
        FILE* chunk_file = fopen(chunk_filename, "r");
        if (!chunk_file) {
            perror("Error reading file");
            exit(EXIT_FAILURE);
        } 
        // get the file size
        fseek(chunk_file, 0, SEEK_END);
        file_size = ftell(chunk_file);
        fseek(chunk_file, 0, SEEK_SET);
        // alocate memory for buffer
        char* buffer = (char*) malloc(file_size + 1);
        if (!buffer) {
            perror("Can't allocate memory");
            fclose(chunk_file);
            exit(EXIT_FAILURE);
        }
        fread(buffer, 1, file_size, chunk_file);
        buffer[file_size] = '\0';

        // convert string to integer and add to result
        result += atoi(buffer);
    
        // free memory
        fclose(chunk_file);
        chunk_file = NULL;
        free(buffer);
        buffer = NULL;

    }

    printf("-> Answer is %ld <-\n", result);

    return 0;
} // end of the program


void read_input_file(FILE** fp, char** buffer, size_t *file_size, const char* filename)
{
    FILE* input_file = *fp;
    input_file = fopen(filename, "r");
    if (!input_file) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }
    // get the file size
    fseek(input_file, 0, SEEK_END);
    *file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    // alocate memory for buffer
    *buffer = (char*) malloc(*file_size + 1);
    if (!*(buffer)) {
        perror("Can't allocate memory");
        fclose(input_file);
        exit(EXIT_FAILURE);
    }
    fread(*buffer, 1, *file_size, input_file);
    (*buffer)[*file_size] = '\0';
    
    fclose(input_file);
    input_file = NULL;
}

void create_file_chunks(char* buffer, long file_size, const size_t num_child)
{
    size_t chunk_size;
    char* shift = buffer;
    for (size_t i = 0; i < num_child; i++) {
        if (i+1 == num_child)
            chunk_size = file_size - (num_child - 1) * (file_size / num_child);
        else
            chunk_size = file_size / num_child;
        char* filename = malloc(10);
        sprintf(filename, "chunk%ld", i);
        FILE* output_file = fopen(filename, "w");
        if (!output_file) {
            perror("File opening failed");
            exit(EXIT_FAILURE);
        }
        char* chunk_buf = (char*) malloc(chunk_size);
        memcpy(chunk_buf, shift, chunk_size);
        for (size_t j = 0; j < chunk_size; j++) {
            printf("%02X", chunk_buf[j]);
        }
        printf("\n");
        shift += chunk_size;
        fwrite(chunk_buf, 1, chunk_size, output_file); /* write chunk of buffer in new file */
        // free allocated memory
        fclose(output_file);
        output_file = NULL;
        free(filename);
        filename = NULL;
        free(chunk_buf);
        chunk_buf = NULL;
    }
    // free allocated memory
    free(buffer);
    buffer = NULL;
}