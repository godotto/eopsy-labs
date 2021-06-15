#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define LEFT (philosopher_index + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS
#define RIGHT (philosopher_index + 1) % NUM_PHILOSOPHERS

// philosopher states
#define THINKING 0
#define HUNGRY 1
#define EATING 2

// philosophers states array and its mutexes (with forks mutex)
int state[NUM_PHILOSOPHERS] = {0};
pthread_mutex_t forks_mutex;
pthread_mutex_t philosopher_mutexes[NUM_PHILOSOPHERS];

void test(int philosopher_index)
{
    // if philosopher is hungry, let him eat
    if (state[philosopher_index] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[philosopher_index] = EATING;
        pthread_mutex_unlock(&philosopher_mutexes[philosopher_index]);
    }
}

void grab_forks(int philosopher_index)
{
    // lock mutex
    pthread_mutex_lock(&forks_mutex);

    // mark philosopher as hungry and let him eat
    state[philosopher_index] = HUNGRY;
    test(philosopher_index);

    // unlock mutex
    pthread_mutex_unlock(&forks_mutex);
    pthread_mutex_lock(&philosopher_mutexes[philosopher_index]);
}

void put_away_forks(int philosopher_index)
{
    // lock mutex
    pthread_mutex_lock(&forks_mutex);

    // indicate that philosopher is thinking again
    state[philosopher_index] = THINKING;

    // let another philosophers eat
    test(LEFT);
    test(RIGHT);

    // unlock mutex
    pthread_mutex_unlock(&forks_mutex);
}

void *philosopher(void *index)
{
    int philosopher = (int *)index - (int *)state;

    for (int i = 1; i <= 4; i++)
    {
        printf("philosopher[%d] wants to grab forks\n", philosopher);
        grab_forks(philosopher);

        printf("philosopher[%d] is eating\n", philosopher);
        sleep(2);

        printf("philosopher[%d] is putting forks away. Meals in total: %d\n", philosopher, i);
        put_away_forks(philosopher);
    }
}

int main()
{
    // initialize mutexes
    pthread_mutex_init(&forks_mutex, NULL);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_mutex_init(&philosopher_mutexes[i], NULL);
        pthread_mutex_lock(&philosopher_mutexes[i]);
    }

    // create threads
    pthread_t philosophers[NUM_PHILOSOPHERS];
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_create(&philosophers[i], NULL, philosopher, &state[i]);
        sleep(1);
    }

    // let the main thread wait for the subthreads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        pthread_join(philosophers[i], NULL);

    return EXIT_SUCCESS;
}
