#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0; // Number of items in the buffer

void sleep_wakeup_producer();
void sleep_wakeup_consumer();
void producer();
void consumer();

pthread_t prod_thread, cons_thread;

int main() {
    // Creating producer and consumer threads
    pthread_create(&prod_thread, NULL, (void *)sleep_wakeup_producer, NULL);
    pthread_create(&cons_thread, NULL, (void *)sleep_wakeup_consumer, NULL);

    // Waiting for threads to finish
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    return 0;
}

void sleep_wakeup_producer() {
    while (1) {
        producer();
        usleep(500000); // sleep to simulate some time delay
    }
}

void sleep_wakeup_consumer() {
    while (1) {
        consumer();
        usleep(500000); // sleep to simulate some time delay
    }
}

void producer() {
    if (count == BUFFER_SIZE) {
        printf("Buffer is full. Producer is sleeping.\n");
    } else {
        buffer[count] = rand() % 100; // Produce an item
        printf("Produced: %d\n", buffer[count]);
        count++;
        printf("Producer wakes up consumer.\n");
    }
}

void consumer() {
    if (count == 0) {
        printf("Buffer is empty. Consumer is sleeping.\n");
    } else {
        printf("Consumed: %d\n", buffer[count - 1]);
        count--;
        printf("Consumer wakes up producer.\n");
    }
}
