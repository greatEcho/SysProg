#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define ARGC 3
#define ARG_MIN 1
#define FILENAME_SIZE 32

void read_input_file(FILE** fp, char** buffer, long *file_size, const char* filename);
void create_file_chunks(char* buffer, long file_size, const size_t num_child);

int main(int argc, char* argv[])
{
    setlocale(0, "");
    //system("del chunk*");
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
    long file_size;
    read_input_file(&input_file, &buffer, &file_size, argv[1]);
    if (file_size <= 1) {
        fprintf(stderr, "Error: File size is too small.\n");
        fclose(input_file);
        return EXIT_FAILURE;
    }
    if (file_size < num_child) {
        num_child = file_size / 2;
        fprintf( stderr, "Warning: The number of child processes is too high. "
                         "The altered value will be %ld.\n", num_child);
    }

    // create file chunks
    create_file_chunks(buffer, file_size, num_child);

    // create child processes
    char* subproc = (char*) malloc(FILENAME_SIZE);
    if (!subproc) {
        perror("Can't allocate memory");
        exit(EXIT_FAILURE);
    }
    /* .hProcess - process descriptor 
    * .hTread - main thread descriptor
    * .dwProcessId - child pid
    * .dwThreadId - main thread id
    */
    PROCESS_INFORMATION *pi = (PROCESS_INFORMATION*) malloc(sizeof(PROCESS_INFORMATION) * num_child);
    if (!pi) {
        perror("Can't allocate memory");
        free(subproc);
        subproc = NULL;
        exit(EXIT_FAILURE);
    }

    /* STARTUPINFO - startup information for created process 
    *  stdinput, stdoutput, stderr, desktop handle, etc for the process
    */
    STARTUPINFO *si = (STARTUPINFO*) malloc(sizeof(STARTUPINFO) * num_child);
    if (!si) {
        perror("Can't allocate memory");
        free(subproc);
        free(pi);
        subproc = NULL;
        pi = NULL;
        exit(EXIT_FAILURE);
    }
    // only handles for wait function
    HANDLE* handles_pi = (HANDLE*) malloc(sizeof(HANDLE) * num_child);


    // initialize with zeros
    ZeroMemory(si, sizeof(STARTUPINFO) * num_child);
    ZeroMemory(pi, sizeof(PROCESS_INFORMATION) * num_child);
    for (size_t i = 0; i < num_child; i++) {


        // GetStartupInfo(&si[i]);
        sprintf(subproc, "./subproc chunk%d", i);
        if (!CreateProcess(NULL,            // No module name (use command line)
                        subproc,      // Command line
                        NULL,            // Process handle not inheritable
                        NULL,            // Thread handle not inheritable
                        FALSE,           // Set handle inheritance to FALSE
                        0,               // No creation flags
                        NULL,            // Use parent's environment block
                        NULL,            // Use parent's starting directory
                        &si[i],             // Pointer to STARTUPINFO structure
                        &pi[i])             // Pointer to PROCESS_INFORMATION structure
            ) /* if function returns false then error occured */
        {
                fprintf(stderr, "Error %d: Can't launch child process\n", GetLastError());
                exit(EXIT_FAILURE);
        }
        handles_pi[i] = pi[i].hProcess;

        // print child_pid
        printf("New process created with process ID %d\n", pi[i].dwProcessId);

        // Wait until child process exits
        // if (WaitForSingleObject(pi.hProcess, INFINITE) != WAIT_OBJECT_0) {
        //     printf("Error occured during child process (%d)\n", GetLastError());
        //     exit(EXIT_FAILURE);
        // }
        // CloseHandle(pi[i].hProcess);
        // CloseHandle(pi[i].hThread);
    }

    // Wait until child processes exit
    DWORD child_sig = WaitForMultipleObjects((DWORD) num_child, handles_pi, TRUE, INFINITE);
    if (child_sig == WAIT_FAILED) {
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
        exit(EXIT_FAILURE);
    }
    // Close process and thread handles
    for (int i = 0; i < num_child; i++) {
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }

    printf("Parent process continuing..\n");
    free(subproc);
    subproc = NULL;
    free(pi);
    pi = NULL;
    free(si);
    si = NULL;
    free(handles_pi);
    handles_pi = NULL;

  // read results from children
    char chunk_filename[FILENAME_SIZE];
    size_t result = 0;
    for (size_t i = 0; i < num_child; i++) {
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
}

void read_input_file(FILE** fp, char** buffer, long *file_size, const char* filename)
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
        sprintf(filename, "chunk%d", i);
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