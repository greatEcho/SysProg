#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void* myturn(void* arg)
{
    for (size_t i = 0; i < 8; i++) {
        sleep(1);
        printf("My Turn! %ld\n", i);
    }
    return NULL;
}


void yourturn()
{
    for (size_t i = 0; i < 3; i++) {
        sleep(2);
        printf("Your Turn! %ld\n", i);
    }
}

int main(void)
{ pthread_t newthread;

    pthread_create(&newthread, NULL, myturn, NULL);
    // myturn();
    yourturn();
    pthread_join(newthread, NULL);

    return 0;
}

