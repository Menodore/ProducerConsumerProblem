#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0; // Pointers for the buffer

sem_t empty; // Semaphore to count empty slots
sem_t full;  // Semaphore to count full slots
pthread_mutex_t mutex; // Mutex for critical section

void *producer(void *arg);
void *consumer(void *arg);

int main() {
    pthread_t prod_thread, cons_thread;

    // Initialize the semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE); // Initially all slots are empty
    sem_init(&full, 0, 0);            // No slots are full initially
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Wait for threads to complete
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Clean up
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void *producer(void *arg) {
    while (1) {
        int item = rand() % 100; // Produce an item

        sem_wait(&empty); // Decrement empty count
        pthread_mutex_lock(&mutex); // Enter critical section

        buffer[in] = item; // Add the item to the buffer
        printf("Produced: %d\n", item);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Leave critical section
        sem_post(&full); // Increment full count

        usleep(500000); // Sleep to simulate work
    }
}

void *consumer(void *arg) {
    while (1) {
        sem_wait(&full); // Decrement full count
        pthread_mutex_lock(&mutex); // Enter critical section

        int item = buffer[out]; // Remove item from buffer
        printf("Consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Leave critical section
        sem_post(&empty); // Increment empty count

        usleep(500000); // Sleep to simulate work
    }
}