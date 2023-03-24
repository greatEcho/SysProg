#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "global.h"
#include "Subproc_arg.h"

#define ARGC 3
#define ARG_MIN 1

HANDLE hMutex = NULL;
size_t result = 0;

void read_input_file(FILE** fp, char** buffer, size_t *file_size, const char* filename);
void run_threads(char* buffer, const size_t num_threads, const size_t file_size);
DWORD WINAPI subproc(LPVOID lpParam);

int main(int argc, char* argv[])
{
    // check if args are passed and valid
    if (argc != ARGC) {
        fprintf( stderr, "Usage: %s <input_file> <num_of_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }
    // check if third argument is valid
    char* endptr;
    size_t num_threads = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || num_threads > INT32_MAX || num_threads < ARG_MIN) {
        fprintf( stderr, "Error! Invalid input: %s", argv[2]);
        return EXIT_FAILURE;
    }
    // open and read the input file
    FILE* input_file;
    char* buffer;
    size_t file_size;
    read_input_file(&input_file, &buffer, &file_size, argv[1]);
    if (file_size <= 1) {
        fprintf(stderr, "Error: File size is too small.\n");
        fclose(input_file);
        return EXIT_FAILURE;
    }
    if (file_size < num_threads) {
        num_threads = file_size / 2;
        fprintf( stderr, "Warning: The number of child processes is too high. "
                         "The altered value will be %ld.\n", num_threads);
    }

    /* create and run threads */
    run_threads(buffer, num_threads, file_size);

    printf("-> Answer is %ld <-\n", result);

    return 0;
}

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

    // free memory 
    fclose(input_file);
    input_file = NULL;
}

void run_threads(char* buffer, const size_t num_threads, const size_t file_size)
{
    char* shift = buffer;
    size_t chunk_size;
    Subproc_arg* t_args = (Subproc_arg*) malloc(sizeof(Subproc_arg) * num_threads);

    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (!hMutex) {
        fprintf(stderr, "Error creating mutex\n");
        exit(EXIT_FAILURE);
    }
    // DWORD arg = 5;
    HANDLE* h_threads = (HANDLE*) malloc(sizeof(HANDLE) * num_threads);
    // Create and run threads
    for (size_t i = 0; i < num_threads; i++) {
        if (i+1 == num_threads)
            chunk_size = file_size - (num_threads - 1) * (file_size / num_threads);
        else
            chunk_size = file_size / num_threads;
        t_args[i].str = (char*) malloc(chunk_size);
        memcpy(t_args[i].str, shift, chunk_size);
        shift += chunk_size;
        t_args[i].size = chunk_size; 
        h_threads[i] = CreateThread(NULL, 0,
                    subproc, (LPVOID) &t_args[i],
                    0, NULL);

        if (!h_threads[i]) {
            perror("Error creating threads");
            free(h_threads);
            h_threads = NULL;
            exit(EXIT_FAILURE);
            }
    }
    //WaitForSingleObject(hThread, INFINITE);
    DWORD thread_sig = WaitForMultipleObjects((DWORD) num_threads, h_threads, TRUE, INFINITE);
    if (thread_sig == WAIT_FAILED) {
        fprintf(stderr, "Error occured during child process (%d)\n", GetLastError());
        DWORD errorCode = GetLastError();
        // Get the error message
        LPSTR errorMessage = NULL;
        DWORD result = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                                      NULL,
                                      errorCode,
                                      0,
                                      (LPSTR)&errorMessage,
                                      0,
                                      NULL);
        if (result != 0)
        {
            printf("Error code: %d\n", errorCode);
            printf("Error message: %s\n", errorMessage);
            LocalFree(errorMessage);
        }
    
    // close thread handles
    for (size_t i = 0; i < num_threads; i++) {
        CloseHandle(h_threads[i]);
        h_threads[i] = NULL;
    }
    free(h_threads);
    h_threads = NULL;
    CloseHandle(hMutex);
    }
}
