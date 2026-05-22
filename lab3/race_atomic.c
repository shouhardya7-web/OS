#include <stdio.h>
#include <pthread.h>

#define ITERATIONS 1000000

int counter = 0;

void *increment(void *arg) {

    for(int i = 0; i < ITERATIONS; i++) {

        __atomic_fetch_add(&counter, 1, __ATOMIC_SEQ_CST);
    }

    return NULL;
}

int main(void) {

    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);

    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);

    pthread_join(t2, NULL);

    printf("Final counter = %d\n", counter);

    return 0;
}

