#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "user32.lib") /* for AnsitoOem */

#define BUFFER_SIZE 10

void win_print(LPSTR message);
void error_handle(HANDLE handle,LPSTR init_msg, void* ptr, HANDLE opened_files[], size_t num_files);
void cleanup(void** pptr, HANDLE opened_files[], size_t num_files);

int main(int argc, char* argv[])
{
    // check if args are passed
    if (argc != 3) {
        char* msg = (char*) malloc(32);
        sprintf(msg, "Usage: %s <input_file> <output_file>\n", argv[0]);
        win_print((LPSTR)msg);
        return EXIT_FAILURE;
    }

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
    input_fh = CreateFile(argv[1], /*file name*/
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
            win_print("Error reading from file\n");
            printf("Error reading from file\n");
            cleanup(&buffer, opened_files, 1);
            return EXIT_FAILURE;
        }
        file_size += bytes_read;
    } while (bytes_read);

    // open the output file
    output_fh = CreateFile(argv[2],
                           GENERIC_WRITE,
                           0, 
                           NULL, 
                           OPEN_EXISTING, /*raise an error if the file doesn't exist*/
                           FILE_ATTRIBUTE_NORMAL, 
                           NULL
                           );
    // handle errors
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
        error_handle(INVALID_HANDLE_VALUE, "Error writing to the file", buffer, opened_files, 2);
        return EXIT_FAILURE;
    }

    // print result
    printf("%d\n", file_size);
    win_print((LPSTR) str);
    cleanup(&buffer, opened_files, 2);

    return EXIT_SUCCESS;
}

void win_print(LPSTR message)
{
    DWORD dw_bytes_written = 0; /* unsigned 32-bit integer */
    HANDLE console_h = GetStdHandle(STD_OUTPUT_HANDLE); /* get a pointer to Console */ /* STD_ERROR_HANDLE*/
    if (console_h == INVALID_HANDLE_VALUE)
        exit(EXIT_FAILURE);
    char msg[256];
    lstrcpy(msg, message);
    AnsiToOem(msg, msg); 
    if (!WriteConsole(console_h, (LPCSTR) msg, strlen(message), &dw_bytes_written, NULL))
        exit(EXIT_FAILURE);
}

void error_handle(HANDLE handle,LPSTR init_msg, void* ptr, HANDLE opened_files[], size_t num_files)
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
        win_print(err_msg);
        //printf("%s\n", err_msg);
        //printf("Error opening input file: %s\n", err_msg);
        LocalFree(err_msg);
        cleanup(&ptr, opened_files, num_files);
        exit(EXIT_FAILURE);
    }
}

void cleanup(void** pptr, HANDLE opened_files[], size_t num_files)
{
    if (*pptr != NULL) {
        //free(ptr);
        if (!HeapFree(GetProcessHeap(), 0, pptr)) {
            printf("Can't free memory\n");
            exit(EXIT_FAILURE);
        }
        pptr = NULL;
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