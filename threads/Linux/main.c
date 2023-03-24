#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>
#include <string.h>
#include <sys/types.h>
#include <inttypes.h>
#include <pthread.h>
#include "Subproc_arg.h"
#include "global.h"

#define ARGC 3
#define ARG_MIN 1

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
size_t result = 0;

void read_input_file(FILE** fp, char** buffer, size_t* file_size, const char* filename);
void* subproc(void* arg);
void *f(void* arg);
size_t run_threads(char* buffer, const size_t num_threads, const size_t file_size);


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
        fprintf( stderr, "Warning: The number of threads is too high. "
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
    
    fclose(input_file);
    input_file = NULL;
}

size_t run_threads(char* buffer, const size_t num_threads, const size_t file_size)
{
    size_t chunk_result = 0, chunk_size;
    char* shift = buffer;
    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * num_threads);
    Subproc_arg* t_args = (Subproc_arg*) malloc(sizeof(Subproc_arg) * num_threads);
    void** t_results = (void**) malloc(sizeof(void*) * num_threads);

    // create and run threads
    for (size_t i = 0; i < num_threads; i++) {
        if (i+1 == num_threads)
            chunk_size = file_size - (num_threads - 1) * (file_size / num_threads);
        else
            chunk_size = file_size / num_threads;
        t_args[i].str = (char*) malloc(chunk_size);
        memcpy(t_args[i].str, shift, chunk_size);
        for (size_t j = 0; j < chunk_size; j++) {
            printf("%02X", t_args[i].str[j]);
        }
        printf("\n");
        shift += chunk_size;
        t_args[i].size = chunk_size; 
        size_t err_n = pthread_create(&(threads[i]), NULL, subproc, (void*)&t_args[i]); 
        if (err_n) {
            perror("Error: ");
            exit(EXIT_FAILURE);
        }
    }

    // Wait all threads
    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        // pthread_join(threads[i], &(t_results[i]));
        // printf("result: %ld\n", *((size_t*) t_results[i]));
        // get results from threads
        // result += *((size_t*) t_results[i]);
    }

    printf("All threads are completed\n");


    // free memory
    free(threads);
    threads = NULL;
    free(t_args);
    t_args = NULL;
    free(t_results);
    t_results = NULL;

    return result;
}
