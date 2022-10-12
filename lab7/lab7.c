#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BUF_SIZE 256

int main(int argc, char **argv)
{
    FILE *file = fopen("input.txt", "r");

    if (file == NULL) {
        printf("Error opening input file");
        exit(-1);
    }

    char *line;
    char line_dup[BUF_SIZE];
    char *total_args[BUF_SIZE];
    char token[BUF_SIZE];
    char command[BUF_SIZE];
    char buf[BUF_SIZE];

    size_t maxlen = 0;
    ssize_t n;
    int count, pid;
    int status;

    while ((n = getline(&line, &maxlen, file)) > 0) {
        count = 0;
        strcpy(line_dup, line);
        strcpy(token, strtok(line, " "));
        strcpy(command, token);
        while (token != NULL) {
            if (strstr(token, "\n") != NULL)
                strcpy(token, strstr(token, "\n"));
            strcpy(total_args[count++], token);
            strcpy(token, strtok(NULL, " "));
        }
        total_args[count] = NULL;

        buf[strcspn(buf, "\n")] = 0;
        pid = fork();
        if (pid == 0) {
            printf("%s %s\n", command, total_args[0]);
            execvp(command, total_args);
            perror("Exec failed");
            break;
        }
        else if (pid > 0) {
            wait(&status);
        }
        else {
            printf("Error creating fork\n");
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
    free(line);
    // free(token);

    // pid = fork();
    // if (pid == 0) { /* this is child process */
    //     printf("This is the child process, my PID is %ld and my parent PID is %ld\n", (long)getpid(), (long)getppid());
    // }
    // else if (pid > 0) { /* this is the parent process */
    //     printf("This is the parent process, my PID is %ld and the child PID is %ld\n", (long)getpid(), (long)pid);

    //     printf("Wait for the child process to terminate\n");
    //     wait(&status);

    //     if (WIFEXITED(status)) { /* child process terminated normally */
    //         printf("Child process exited with status = %d\n",
    //                WEXITSTATUS(status));
    //     }
    //     else { /* child process did not terminate normally */
    //         printf("ERROR: Child process did not terminate normally!\n");
    //     }
    // }
    // else { /* we have an error in process creation */
    //     perror("fork");
    //     exit(EXIT_FAILURE);
    // }

    // printf("[%ld]: Exiting program .....\n", (long)getpid());
    return 0;
}