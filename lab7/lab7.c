#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    char *line = NULL;
    size_t maxlen = 0;
    ssize_t n;

    FILE *file = fopen("input.txt", O_RDONLY);
    if (file == NULL) {
        printf("Error opening input file");
        exit(-1);
    }

    while ((n = getline(&line, &maxlen, file)) > 0) {
        printf("%s\n", strtok(line, " "));
    }
    free(line);

    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0) { /* this is child process */
        printf("This is the child process, my PID is %ld and my parent PID is %ld\n", (long)getpid(), (long)getppid());
    }
    else if (pid > 0) { /* this is the parent process */
        printf("This is the parent process, my PID is %ld and the child PID is %ld\n", (long)getpid(), (long)pid);

        printf("Wait for the child process to terminate\n");
        wait(&status);

        if (WIFEXITED(status)) { /* child process terminated normally */
            printf("Child process exited with status = %d\n",
                   WEXITSTATUS(status));
        }
        else { /* child process did not terminate normally */
            printf("ERROR: Child process did not terminate normally!\n");
        }
    }
    else { /* we have an error in process creation */
        perror("fork");
        exit(EXIT_FAILURE);
    }

    printf("[%ld]: Exiting program .....\n", (long)getpid());
    return 0;
}