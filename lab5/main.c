#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

#define NUM_PHILOSOPHERS 5

int forks; // ID of semaphores array

void grab_forks(int left_fork_id)
{
    // create sembuf structure for semop function
    struct sembuf sb[2];

    // assign semaphores IDs
    sb[0].sem_num = left_fork_id;
    sb[1].sem_num = (left_fork_id + 1) % 5;

    // by decrementation mark that forks are being used
    sb[0].sem_op = -1;
    sb[1].sem_op = -1;

    sb[0].sem_flg = 0;
    sb[1].sem_flg = 0;

    // perform grabbing operation and check whether there were any errors
    if (semop(forks, sb, 2) == -1)
        printf("Grabbing went wrong for philosopher %d\n", left_fork_id);
}

void put_away_forks(int left_fork_id)
{
    // create sembuf structure for semop function
    struct sembuf sb[2];

    // assign semaphores IDs
    sb[0].sem_num = left_fork_id;
    sb[1].sem_num = (left_fork_id + 1) % 5;

    // by incrementation mark that forks are being used
    sb[0].sem_op = 1;
    sb[1].sem_op = 1;

    sb[0].sem_flg = 0;
    sb[1].sem_flg = 0;

    // perform putting away operation and check whether there were any errors
    if (semop(forks, sb, 2) == -1)
        printf("Putting away went wrong for philosopher %d\n", left_fork_id);
}

void philosopher(int philosopher_index)
{
    for (int i = 1; i <= 4; i++)
    {
        // thinking phase
        printf("philosopher[%d] is thinking\n", philosopher_index);
        sleep(3);

        // grabbing forks, eating and putting them down
        printf("philosopher[%d] wants to grab forks\n", philosopher_index);
        grab_forks(philosopher_index);

        printf("philosopher[%d] is eating\n", philosopher_index);
        sleep(2);

        printf("philosopher[%d] is putting forks away. Meals in total: %d\n", philosopher_index, i);
        put_away_forks(philosopher_index);
    }
}

int main()
{
    // philosophers PIDs array
    pid_t philosophers[NUM_PHILOSOPHERS];

    // create NUM_PHILOSOPHERS semaphores
    forks = semget(IPC_PRIVATE, NUM_PHILOSOPHERS, IPC_CREAT | 0600);

    // set up all semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        semctl(forks, i, SETVAL, 1);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        // fork process for each philosopher and save its PID to the array
        pid_t philosopher_pid = fork();
        philosophers[i] = philosopher_pid;

        // check which process is currently being processed
        switch (philosopher_pid)
        {
        // child creating was unsuccessful
        case -1:
            // print message about error and send SIGTERM to already created processes
            printf("parent[%d]: child process creation failure\n", getpid());
            for (int j = 0; j < i; j++)
                kill(philosophers[j], SIGTERM);

            // terminate with failure code
            exit(EXIT_FAILURE);

        // child has been created and current process is child
        case 0:
            // call philosopher function
            philosopher(i);
            return EXIT_SUCCESS;

        // child has been created and current process is parent
        default:
            break;
        }
    }

    // wait until all children has been exit code 0
    int status;
    while (wait(&status) > 0)
    {
    }

    return EXIT_SUCCESS;
}