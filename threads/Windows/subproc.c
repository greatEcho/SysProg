#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Subproc_arg.h"
#include "global.h"

#define ARGC 2
#define FILENAME_SIZE 32

size_t read_chunk_file(char* filename);
void write_result(size_t result, char* filename);
size_t count_symbols(char* str, const size_t str_size);

DWORD WINAPI subproc(LPVOID lpParam)
{
    // convert args
    Subproc_arg* subproc_arg = (Subproc_arg*) lpParam; 
    size_t str_size = subproc_arg->size;
    char* str = (char*) malloc(str_size);
    str = subproc_arg->str;
    for (size_t j = 0; j < str_size; j++) {
        printf("%02X", str[j]);
    }
    printf("\n");
    size_t chunk_result = count_symbols(str, str_size);
    WaitForSingleObject(hMutex, INFINITE);
    // threadsafe
    result += chunk_result;
    ReleaseMutex(hMutex);
    printf("Hello from thread!\n");
    ExitThread(0);

    return 0;
}

size_t read_chunk_file(char* filename)
{
    size_t counter = 0;
    FILE* chunk_file;
    chunk_file = fopen(filename, "r");
    if (!chunk_file) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }
    int c;
    // count how many ascii 0x00-0x1F in chunk file
    while ((c = fgetc(chunk_file)) != EOF) {
        if ((c >= 0 && c <= 0x1F) || (c == 0x7F)) {
            ++counter;
        }
    }
    if (ferror(chunk_file)) {
        fprintf(stderr, "I/O error when reading\n");
        fclose(chunk_file);
        chunk_file = NULL;
        exit(EXIT_FAILURE);
    }
    else if (feof(chunk_file)) {
        //printf("End of file is reached successfully\n");
    }

    fclose(chunk_file);
    chunk_file = NULL;
    return counter;
}

void write_result(size_t result, char* filename)
{
    char* result_filename = malloc(FILENAME_SIZE);
    sprintf(result_filename, "%s_result", filename);
    FILE* output_file = fopen(result_filename, "w");
    if (!output_file) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }
    fprintf(output_file, "%d", result);
    // free allocated memory
    fclose(output_file);
    output_file = NULL;
}

size_t count_symbols(char* str, const size_t str_size)
{
    size_t counter = 0;
    for (size_t i = 0; i < str_size; i++) {
        char c = str[i]; 
        if ((c >= 0 && c <= 0x1F) || (c == 0x7F)) {
            ++counter;
        }
    }
    return counter;
}