#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) { /* this is child process */
        printf(
            "This is the child process, my PID is %ld and my parent PID is %ld\n",
            (long)getpid(), (long)getppid());
    } else if (pid > 0) { /* this is the parent process */
        printf(
            "This is the parent process, my PID is %ld and the child PID is %ld\n",
            (long)getpid(), (long)pid);

        printf("Wait for the child process to terminate\n");
        wait(&status);           /* wait for the child process to terminate */
        if (WIFEXITED(status)) { /* child process terminated normally */
            printf("Child process exited with status = %d\n",
                   WEXITSTATUS(status));
        } else { /* child process did not terminate normally */
            printf("ERROR: Child process did not terminate normally!\n");
            /* look at the man page for wait (man 2 wait) to
               determine how the child process was terminated */
        }
    } else { /* we have an error in process creation */
        perror("fork");
        exit(EXIT_FAILURE);
    }

    printf("[%ld]: Exiting program .....\n", (long)getpid());
    return 0;
}