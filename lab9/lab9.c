#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t pid;

static void sig_hand(int signo)
{
    switch (signo) {
        case SIGINT:
            printf(" [%d] Child interrupted, please kill\n", pid);
            break;
        case SIGTSTP:
            printf(" [%d] Child suspended, please continue or kill\n", pid);
            break;
    }
}

int main(int argc, char **argv)
{
    int status;

    signal(SIGINT, sig_hand);
    signal(SIGTSTP, sig_hand);

    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        exit(-1);
    }

    pid = fork();
    if (pid == 0) { /* this is child process */

        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(-1);
    }
    else if (pid > 0) { /* this is the parent process */
        printf("Wait for the child process to terminate\n");
        wait(&status);           /* wait for the child process to terminate */
        if (WIFEXITED(status)) { /* child process terminated normally */
            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
        }
        else { /* child process did not terminate normally */
            for (; ;) {
                pause();
            }
        }
    }
    else {              /* we have an error */
        perror("fork"); /* use perror to print the system error message */
        exit(EXIT_FAILURE);
    }

    return 0;
}
