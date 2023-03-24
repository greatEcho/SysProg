#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#define NUM_THREADS 2

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
size_t globalVar = 5;
void *f(void* arg)
{
    pthread_mutex_lock(&mutex);
    printf("%ld\n", globalVar);
    globalVar = (globalVar * 2) + (globalVar % 3);
    printf("%ld\n", globalVar);
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}
int main()
{

    // pthread_t newthread;
    // pthread_create(&newthread, NULL, f, NULL);
    // pthread_join(newthread, NULL);

    

    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * NUM_THREADS);
    size_t* t_args = (size_t*) malloc(sizeof(size_t) * NUM_THREADS);

    // create and start threads
    for (size_t i = 0; i < NUM_THREADS; i++) {
        t_args[i] = i;
        size_t err_n = pthread_create(&threads[i], NULL, f, (void*)&t_args[i]); 
        if (err_n) {
            perror("Error: ");
            exit(EXIT_FAILURE);
        }
    // pthread_t newthread;
    // pthread_create(&newthread, NULL, f, NULL);
    // pthread_join(newthread, NULL);
    }

    // Wait all threads
    for (size_t i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads are completed\n");

    free(threads);
    free(t_args);
    return 0;
}