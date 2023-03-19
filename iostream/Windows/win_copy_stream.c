#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "2000.txt"
#define OUTPUT_FILE "output"
#define BUFFER_SIZE 22

void win_print(LPCSTR message);
void error_handle(HANDLE handle,LPCSTR init_msg, void* ptr, HANDLE opened_files[], size_t num_files);
void cleanup(void* ptr, HANDLE opened_files[], size_t num_files);

int wmain(void)
{
    // TO DO: ARGV ARGC
    // set locale based on user's env for all categories
    setlocale(0, "");
    HANDLE input_fh, output_fh;
    DWORD bytes_written, bytes_read;
    DWORD file_size = 0;
    //char buffer[BUFFER_SIZE];
    //char* buffer = (char*) malloc(BUFFER_SIZE);
    char* buffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BUFFER_SIZE);
    HANDLE* opened_files = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(HANDLE) * 2);
    if (!buffer) {
        printf("Error: Can't allocate memory.\n");
        return EXIT_FAILURE;
    }

    // open the input file
    input_fh = CreateFile(INPUT_FILE, /*file name*/
                          GENERIC_READ, /*desired access to the file*/
                          0, /*share mode (disabled)*/
                          NULL, /*handle can't be inherited by child process*/
                          OPEN_EXISTING, /*how to open the file (raise an error if file does'nt exist)*/
                          FILE_ATTRIBUTE_NORMAL, /*file attributes*/
                          NULL /*template file that supplies its attributes*/
                          );
    // handle errors
    error_handle(input_fh, "Error opening input file: ", buffer, NULL, 0);
    opened_files[0] = input_fh; 

    // set pointer to the beginning of the file
    SetFilePointer(input_fh, 0, NULL, FILE_BEGIN);
    // count the input file size
    do {
        if (!ReadFile(input_fh, buffer, BUFFER_SIZE, &bytes_read, NULL)) {
            printf("Error reading from file\n");
            CloseHandle(input_fh);
            free(buffer);
            buffer = NULL;
            return EXIT_FAILURE;
        }
        file_size += bytes_read;
    } while (bytes_read);

    // open the output file
    output_fh = CreateFile(OUTPUT_FILE,
                           GENERIC_WRITE,
                           0, 
                           NULL, 
                           OPEN_EXISTING, /*raise an error if the file doesn't exist*/
                           FILE_ATTRIBUTE_NORMAL, 
                           NULL
                           );
    // handle errors
   // HANDLE opened_files[] = {input_fh};
    error_handle(output_fh, "Error opening output file: ", buffer, opened_files, 1);
    opened_files[1] = output_fh;

    // convert int to str
    char str[33]; // up to 1 Gb
    int len = sprintf(str, "%d", file_size);

    SetFilePointer(output_fh, 0, NULL, FILE_BEGIN);
    // Write data to the output file
    if (!WriteFile(output_fh, /*filename*/
                   str, /*pointer to the buffer*/
                   len, 
                   &bytes_written, 
                   NULL) //LAST NULL - not overlapped (async)
                   || bytes_written != len) { 
        printf("Error writing to the file\n");
        //opened_files = {input_fh, output_fh};
        //cleanup(buffer, {input_fh, output_fh}, 2);
        return EXIT_FAILURE;
    }
    printf("%d\n", file_size);
    win_print((LPCSTR) str);

    cleanup(buffer, opened_files, 2);

    return EXIT_SUCCESS;
}

void win_print(LPCSTR message)
{
    DWORD dw_bytes_written = 0;
    HANDLE console_h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console_h == INVALID_HANDLE_VALUE)
        exit(EXIT_FAILURE);
    
    if (!WriteConsole(console_h, message, strlen(message), &dw_bytes_written, NULL))
        exit(EXIT_FAILURE);
}

void error_handle(HANDLE handle,LPCSTR init_msg, void* ptr, HANDLE opened_files[], size_t num_files)
{
    if (handle == INVALID_HANDLE_VALUE) {
        LPSTR err_msg;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL,
                      GetLastError(),
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      (LPSTR) &err_msg,
                      0,
                      NULL
                      );
        win_print(init_msg);
        printf("%s\n", err_msg);
        //printf("Error opening input file: %s\n", err_msg);
        LocalFree(err_msg);
        cleanup(ptr, opened_files, num_files);
        exit(EXIT_FAILURE);
    }
}

void cleanup(void* ptr, HANDLE opened_files[], size_t num_files)
{
    if (ptr != NULL) {
        //free(ptr);
        if (!HeapFree(GetProcessHeap(), 0, ptr)) {
            printf("Can't free memory\n");
            exit(EXIT_FAILURE);
        }
        ptr = NULL;
    }
    for (size_t i = 0; i < num_files; i++) {
        if (opened_files[i] != NULL) {
            CloseHandle(opened_files[i]);
            opened_files[i] = NULL;
        }
    }
    if (!HeapFree(GetProcessHeap(), 0, opened_files)) {
        printf("Can't free memory\n");
        exit(EXIT_FAILURE);
    }
    opened_files = NULL;
}