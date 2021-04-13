#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

#define NUM_CHILD 8

int main()
{
    pid_t child_processes[NUM_CHILD];   // array which holds PIDs 

    for (int i = 0; i < NUM_CHILD; i++) // create NUM_CHILD child processes in the loop
    {
        // fork process and save child's PID to the array
        pid_t child_pid = fork();
        child_processes[i] = child_pid;

        // check which process is currently being processed
        switch (child_pid)
        {
        // child creating was unsuccessful
        case -1:
            // print message about error and send SIGTERM to already created processes
            perror("parent[%d]: child process creation failure");

            for (int j = 0; j < NUM_CHILD; j++)
                kill(child_processes[j], SIGTERM);

            // terminate with failure code
            exit(EXIT_FAILURE);

        // child has been created and current process is child
        case 0:
            // print message, wait for 10 seconds and print next message
            printf("child[%d]: parent's PID=%d\n", getpid(), getppid());
            sleep(10);
            printf("child[%d]: execution completed\n", getpid());

            _exit(EXIT_SUCCESS); // terminate with succes code

        // child has been created and current process is parent
        default:
            break;
        }
        
        sleep(1); // make one second of delay between each fork call
    }

    printf("parent[%d]: all children have been created\n", getpid());

    while (wait(NULL) > 0) // wait until all children has been terminated
    {
    }

    printf("parent[%d]: all children have been terminated\n", getpid());

    return 0;
}
