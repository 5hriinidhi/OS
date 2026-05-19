#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];

int in = 0;
int out = 0;

sem_t empty;             // Empty slots in buffer
sem_t full;              // Filled slots in buffer

pthread_mutex_t mutex;   // Protects buffer access

// Producer Function
void* producer(void* arg)
{
    for (int i = 1; i <= 5; i++)
    {
        int item = i;

        sleep(1);

        // Wait if buffer is full
        sem_wait(&empty);

        pthread_mutex_lock(&mutex);

        buffer[in] = item;

        cout << "Produced: "
             << item
             << " at index "
             << in << endl;

        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);

        // Signal that one more item is available
        sem_post(&full);
    }

    return NULL;
}

// Consumer Function
void* consumer(void* arg)
{
    for (int i = 1; i <= 5; i++)
    {
        sleep(2);

        // Wait if buffer is empty
        sem_wait(&full);

        pthread_mutex_lock(&mutex);

        int item = buffer[out];

        cout << "Consumed: "
             << item
             << " from index "
             << out << endl;

        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);

        // Signal that one more slot is free
        sem_post(&empty);
    }

    return NULL;
}

int main()
{
    pthread_t prod, cons;

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for threads
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Destroy semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);

    pthread_mutex_destroy(&mutex);

    return 0;
}
