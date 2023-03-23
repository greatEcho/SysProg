#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>
#include <string.h>
#include <sys/types.h>

#define INPUT_FILE "text"
#define NUM_CHILD 4
#define ARGC 1
#define FILENAME_SIZE 32

void read_input_file(FILE** fp, char** buffer, long* file_size);
void create_file_chunks(char** buffer, long file_size);

int main(int argc, char* argv[])
{
    // check if args are passed and valid
    if (argc != ARGC) {
        fprintf( stderr, "Usage: %s <input_file> <num_of_processes>\n", argv[0]);
        return EXIT_FAILURE;
    }
    // open and read the input file
    FILE* input_file;
    char* buffer;
    long file_size;
    read_input_file(&input_file, &buffer, &file_size);

    // create file chunks
    create_file_chunks(&buffer, file_size);


    // creat child processes
    char chunk_filename[FILENAME_SIZE];
    pid_t child_pid, wpid;
    int status;
    for (int i = 0; i < NUM_CHILD; i++) {
        sprintf(chunk_filename, "chunk%d", i);
        // create child process copy
        child_pid = fork();
        switch(child_pid) {
        case -1:
            perror("Fork failed");
            exit(EXIT_FAILURE);
        case 0:
            // Child process 
            printf("Now we start child process..\n");
            if ( execl("./subproc", "subproc", chunk_filename, NULL) == -1 ) {
                perror("Starting child process failed.");
                exit(EXIT_FAILURE);
            }
            // these code will be never reached
            printf("Child process finished.\n");
            exit(0);
        default:
            // Parent process
            printf("Parent process waiting for children...\n");
            do {
                wpid = waitpid(child_pid, &status, WUNTRACED);
                if (wpid == -1) {
                    perror("Waitpid Error");
                    exit(1);
                }
                if (WEXITSTATUS(status)) { perror("Child failed"); exit(EXIT_FAILURE); }
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            printf("Parent process continuing...\n");
        }

    }
    printf("parent pid -> %d\n", getpid());

    // read results from children
    size_t result = 0;
    for (size_t i = 0; i < NUM_CHILD; i++) {
        sprintf(chunk_filename, "chunk%d_result", i);
        FILE* chunk_file = fopen(chunk_filename, "r");
        if (!chunk_file) {
            perror("Error reading file");
            exit(EXIT_FAILURE);
        } 
        // get the file size
        fseek(chunk_file, 0, SEEK_END);
        size_t file_size = ftell(chunk_file);
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

    printf("-> Answer is %d <-\n", result);

    return 0;
} // end of the program


void read_input_file(FILE** fp, char** buffer, long *file_size)
{
    FILE* input_file = *fp;
    input_file = fopen(INPUT_FILE, "r");
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

void create_file_chunks(char** buffer, long file_size)
{
    size_t chunk_size;
    for (size_t i = 0; i < NUM_CHILD; i++) {
        if (i+1 == NUM_CHILD)
            chunk_size = file_size - (NUM_CHILD - 1) * (file_size / NUM_CHILD);
        else
            chunk_size = file_size / NUM_CHILD;
        char* filename = malloc(10);
        sprintf(filename, "chunk%d", i);
        FILE* output_file = fopen(filename, "w");
        if (!output_file) {
            perror("File opening failed");
            exit(EXIT_FAILURE);
        }
        char* chunk_buf = (char*) malloc(chunk_size + 1);
        strncpy(chunk_buf, *buffer + i*chunk_size, chunk_size);
        chunk_buf[chunk_size] = '\0';
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
    free(*buffer);
    *buffer = NULL;
}