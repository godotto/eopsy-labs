#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

#define NUM_CHILD 8

int main()
{
    pid_t child_processes[NUM_CHILD]; // array which holds PIDs

#ifdef WITH_SIGNALS
    struct sigaction old_actions[_NSIG]; // array with old actions which after exexuction will be restored
    
    // ignore all singals coming to this process
    for (int i = 0; i < _NSIG; i++)
    {
        struct sigaction sa;
        sa.sa_handler = SIG_IGN;
        sigaction(i, &sa, &old_actions[i]);
    }

    sigaction(SIGCHLD, &old_actions[SIGCHLD], NULL); // restore default SIGCHILD handler

#endif

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

            for (int j = 0; j < i; j++)
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

    int status;                 // exit code of child process
    int terminatedChildren = 0; // number of children with process

    while (wait(&status) > 0) // wait until all children has been exit code 0
    {
        if (status == EXIT_SUCCESS)
            terminatedChildren++;
    }

    printf("parent[%d]: all children have been terminated\n", getpid());
    printf("parent[%d]: %d children have been terminted with 0 exit code\n", getpid(), terminatedChildren);

    return 0;
}
