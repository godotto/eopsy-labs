#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <stdbool.h>

#define NUM_CHILD 8

#ifdef WITH_SIGNALS
    bool interrupt_occured = false; // if true, interrupt signal has been sent

    // new interrupt handler which prints information and sets the mark
    void newInterruptHandler()
    {
        interrupt_occured = true; // set mark to true
        printf("parent[%d]: keyboard interrupt received\n", getpid());
    }

    // new terminate handler for children
    void newTerminateHandler()
    {
        printf("child[%d]: process terminated\n", getpid());
    }
#endif

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

    // set new interrupt handler
    struct sigaction sa;
    sa.sa_handler = &newInterruptHandler;
    sigaction(SIGINT, &sa, NULL);

#endif

    for (int i = 0; i < NUM_CHILD; i++) // create NUM_CHILD child processes in the loop
    {
        #ifdef WITH_SIGNALS
            // if interrupt occured, abort creation process and send SIGTERM to created children
            if (interrupt_occured)
            {
                for (int j = 0; j < i; j++)
                    kill(child_processes[j], SIGTERM);
                
                printf("parent[%d]: child process creation interrupt\n", getpid());
                break;
            }
        #endif
        
        // fork process and save child's PID to the array
        pid_t child_pid = fork();
        child_processes[i] = child_pid;

        // check which process is currently being processed
        switch (child_pid)
        {
        // child creating was unsuccessful
        case -1:
            // print message about error and send SIGTERM to already created processes
            printf("parent[%d]: child process creation failure\n", getpid());

            for (int j = 0; j < i; j++)
                kill(child_processes[j], SIGTERM);

            // terminate with failure code
            exit(EXIT_FAILURE);

        // child has been created and current process is child
        case 0:
            #ifdef WITH_SIGNALS
                // ignore interrupt signal
                sa.sa_handler = SIG_IGN;
                sigaction(SIGINT, &sa, NULL);

                // set new terminate handler
                sa.sa_handler = &newTerminateHandler;
                sigaction(SIGTERM, &sa, NULL);
            #endif
            
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

    #ifdef WITH_SIGNALS
        if (!interrupt_occured) // if interrupt occured don't print the message
    #endif
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
