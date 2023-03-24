#include <stdio.h>
#include <windows.h>

// Shared data
int counter = 0;
HANDLE hMutex;

// Thread function
DWORD WINAPI increment_counter(LPVOID lpParam) {
    int num_iterations = *(int*)lpParam;

    for (int i = 0; i < num_iterations; ++i) {
        // Lock the mutex
        WaitForSingleObject(hMutex, INFINITE);

        // Increment the counter
        counter++;

        // Unlock the mutex
        ReleaseMutex(hMutex);
    }
    return 0;
}

int main() {
    const int num_iterations = 100000;

    // Create the mutex
    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        printf("Error creating mutex.\n");
        return 1;
    }

    // Create threads
    HANDLE thread1 = CreateThread(NULL, 0, increment_counter, &num_iterations, 0, NULL);
    HANDLE thread2 = CreateThread(NULL, 0, increment_counter, &num_iterations, 0, NULL);

    if (thread1 == NULL || thread2 == NULL) {
        printf("Error creating threads.\n");
        return 1;
    }

    // Wait for both threads to finish
    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

    // Close thread handles
    CloseHandle(thread1);
    CloseHandle(thread2);

    // Close the mutex handle
    CloseHandle(hMutex);

    printf("Counter value: %d\n", counter);
    return 0;
}