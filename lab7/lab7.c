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
    // Open the given file & make output.log
    FILE *file_in = fopen(argv[1], "r");
    if (file_in == NULL) {
        printf("Error opening input file");
        exit(EXIT_FAILURE);
    }
    FILE *file_out = fopen("output.log", "w");

    char *line;
    char *total_args[20];
    char *token;
    char *time_str;
    size_t maxlen = 0;
    ssize_t n;
    int count, pid;
    int status;
    time_t current_time;

    // Repeats for every command in given file
    while ((n = getline(&line, &maxlen, file_in)) > 0) {
        // Chop newline off of line to clean up output.log
        line[strcspn(line, "\n")] = 0;
        fputs(line, file_out);
        fputs("\t", file_out);

        // Make total args have an entry for each part of command
        count = 0;
        token = strtok(line, " ");
        while (token != NULL) {
            while (strstr(token, "\n") != NULL)
                token[strcspn(token, "\n")] = 0;
            total_args[count++] = token;
            token = strtok(NULL, " ");
        }
        total_args[count] = 0;

        // Prints the time
        time(&current_time);
        time_str = ctime(&current_time);
        time_str[strcspn(time_str, "\n")] = 0;
        fputs(time_str, file_out);
        fputs("\t", file_out);

        pid = fork();
        if (pid == 0) { // Child process exec
            execvp(total_args[0], total_args);
            perror("Exec failed");
            break;
        }
        else if (pid > 0) { // Parent process
            wait(&status);
            time(&current_time);
            fputs(ctime(&current_time), file_out);
        }
        else { // ICE
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    }
    // Final cleanup
    free(line);
    free(token);
    fclose(file_in);
    fclose(file_out);

    return 0;
}