#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

sem_t rw_mutex;      // Controls access for writers
sem_t mutex_rc;      // Protects read_count

int read_count = 0;  // Number of active readers
int shared_data = 0; // Shared resource

// Reader Function
void* reader(void* arg)
{
    int id = *(int*)arg;

    sem_wait(&mutex_rc);

    read_count++;

    // First reader blocks writers
    if (read_count == 1)
        sem_wait(&rw_mutex);

    sem_post(&mutex_rc);

    // Reading Section
    cout << "Reader " << id
         << " reads: " << shared_data << endl;

    sleep(1);

    sem_wait(&mutex_rc);

    read_count--;

    // Last reader allows writers
    if (read_count == 0)
        sem_post(&rw_mutex);

    sem_post(&mutex_rc);

    return NULL;
}

// Writer Function
void* writer(void* arg)
{
    int id = *(int*)arg;

    // Writer gets exclusive access
    sem_wait(&rw_mutex);

    // Writing Section
    shared_data++;

    cout << "Writer " << id
         << " writes: " << shared_data << endl;

    sleep(1);

    sem_post(&rw_mutex);

    return NULL;
}

int main()
{
    pthread_t readers[3], writers[2];

    int ids[5] = {1, 2, 3, 4, 5};

    // Initialize semaphores
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex_rc, 0, 1);

    // Create reader threads
    for (int i = 0; i < 3; i++)
        pthread_create(&readers[i], NULL, reader, &ids[i]);

    // Create writer threads
    for (int i = 0; i < 2; i++)
        pthread_create(&writers[i], NULL, writer, &ids[i]);

    // Wait for readers
    for (int i = 0; i < 3; i++)
        pthread_join(readers[i], NULL);

    // Wait for writers
    for (int i = 0; i < 2; i++)
        pthread_join(writers[i], NULL);

    // Destroy semaphores
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex_rc);

    return 0;
}
