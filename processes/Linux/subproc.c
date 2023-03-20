#include <stdio.h>
#include <stdlib.h>

#define ARGC 2
#define FILENAME_SIZE 32

size_t read_chunk_file(char* filename);
void write_result(size_t result, char* filename);

int main(int argc, char* argv[])
{
    /* check the number of passed arguments */
    if (argc != ARGC) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char* filename = argv[1];
    size_t counter = read_chunk_file(filename);

    write_result(counter, filename);

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
        printf("I/O error when reading\n");
        fclose(chunk_file);
        chunk_file = NULL;
        exit(EXIT_FAILURE);
    }
    else if (feof(chunk_file)) {
        printf("End of file is reached successfully\n");
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