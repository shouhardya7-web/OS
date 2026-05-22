#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define ITEMS_PER_PRODUCER 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

int producers_done = 0;
int N, M;

void *producer(void *arg) {

    int id = *(int *)arg;

    for(int i = 0; i < ITEMS_PER_PRODUCER; i++) {

        pthread_mutex_lock(&lock);

        while(count == BUFFER_SIZE)
            pthread_cond_wait(&not_full, &lock);

        buffer[in] = id * 100 + i;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("Producer %d produced %d\n", id, buffer[in]);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);
    }

    pthread_mutex_lock(&lock);
    producers_done++;
    pthread_cond_broadcast(&not_empty);
    pthread_mutex_unlock(&lock);

    return NULL;
}

void *consumer(void *arg) {

    int id = *(int *)arg;

    while(1) {

        pthread_mutex_lock(&lock);

        while(count == 0 && producers_done < N)
            pthread_cond_wait(&not_empty, &lock);

        if(count == 0 && producers_done == N) {
            pthread_mutex_unlock(&lock);
            break;
        }

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("Consumer %d consumed %d\n", id, item);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("Usage: ./prod_cons N M\n");
        return 1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);

    pthread_t producers[N], consumers[M];
    int pid[N], cid[M];

    for(int i = 0; i < N; i++) {
        pid[i] = i;
        pthread_create(&producers[i], NULL, producer, &pid[i]);
    }

    for(int i = 0; i < M; i++) {
        cid[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &cid[i]);
    }

    for(int i = 0; i < N; i++)
        pthread_join(producers[i], NULL);

    for(int i = 0; i < M; i++)
        pthread_join(consumers[i], NULL);

    return 0;
}
