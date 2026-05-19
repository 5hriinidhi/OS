#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define N 5

// Each fork is represented by a semaphore
sem_t forks[N];

// Philosopher Function
void* philosopher(void* arg)
{
    int id = *(int*)arg;

    int left  = id;
    int right = (id + 1) % N;

    while (true)
    {
        // Thinking
        cout << "Philosopher "
             << id
             << " is THINKING"
             << endl;

        sleep(1);

        // Deadlock avoidance:
        // Last philosopher picks right fork first
        if (id == N - 1)
        {
            sem_wait(&forks[right]);
            sem_wait(&forks[left]);
        }
        else
        {
            sem_wait(&forks[left]);
            sem_wait(&forks[right]);
        }

        // Eating
        cout << "Philosopher "
             << id
             << " is EATING"
             << endl;

        sleep(1);

        // Put down forks
        sem_post(&forks[left]);
        sem_post(&forks[right]);

        cout << "Philosopher "
             << id
             << " put down forks"
             << endl;
    }

    return NULL;
}

int main()
{
    pthread_t philosophers[N];
    int ids[N];

    // Initialize semaphores for forks
    for (int i = 0; i < N; i++)
        sem_init(&forks[i], 0, 1);

    // Create philosopher threads
    for (int i = 0; i < N; i++)
    {
        ids[i] = i;

        pthread_create(
            &philosophers[i],
            NULL,
            philosopher,
            &ids[i]
        );
    }

    // Wait for philosopher threads
    for (int i = 0; i < N; i++)
        pthread_join(philosophers[i], NULL);

    // Destroy semaphores
    for (int i = 0; i < N; i++)
        sem_destroy(&forks[i]);

    return 0;
}