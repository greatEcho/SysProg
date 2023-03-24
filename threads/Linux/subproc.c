#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Subproc_arg.h"

#define ARGC 2
#define FILENAME_SIZE 32

void write_result(size_t result, char* filename);
size_t count_symbols(char* str, const size_t str_size);

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* subproc(void* arg)
{
    pthread_t tid = pthread_self();
    printf("Thread ID: %lu\n", (unsigned long)tid);
    /* convert arguments */
    Subproc_arg* subproc_arg = (Subproc_arg*) arg;
    size_t str_size = subproc_arg->size;
    char* str = (char*) malloc(str_size);
    str = subproc_arg->str;
    // printf("passed arg: size = %ld\n", str_size);
        // for (size_t j = 0; j < str_size; j++) {
        //     printf("%02X", str[j]);
        // }
        // printf("\n");
    pthread_mutex_lock(&mutex);
    size_t result = count_symbols(str, str_size);
    free(str);
    str = NULL;
    pthread_mutex_unlock(&mutex);
    /* return value from thread function */
    size_t* presult = (size_t*) malloc(sizeof(size_t));
    *presult = result;
    pthread_exit((void*) presult);
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

void write_result(size_t result, char* filename)
{
    char* result_filename = malloc(FILENAME_SIZE);
    if (!result_filename) {
        fprintf(stderr, "Error: can't allocate memory\n");
        exit(EXIT_FAILURE);
    }
    sprintf(result_filename, "%s_result", filename);
    FILE* output_file = fopen(result_filename, "w");
    if (!output_file) {
        perror("File opening failed");
        free(result_filename);
        result_filename = NULL;
        exit(EXIT_FAILURE);
    }
    fprintf(output_file, "%ld", result);
    // free allocated memory
    fclose(output_file);
    output_file = NULL;
    free(result_filename);
    result_filename = NULL;
}