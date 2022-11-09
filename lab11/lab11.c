#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define BUF_SIZE 256

int main(int argc, char **argv)
{
    int dflag = 0;
    // Format checks
    if (argc < 2) {
        printf("Command should be of the format ./lab11 <input_file> (-d)\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 3 && !strcmp(argv[2], "-d")) {
        dflag = 1;
    }
    // Open the given file & make output.log
    FILE *file_in = fopen(argv[1], "r");
    if (file_in == NULL) {
        printf("Error opening input file\n");
        exit(EXIT_FAILURE);
    }
    FILE *file_out = fopen("output.log", "w");

    char *line, *token, *time_str, *spidout, *spiderr;
    char *total_args[20];
    size_t maxlen = 0;
    ssize_t n;
    int count, pid, status;
    time_t current_time;
    int fdout, fderr;

    spidout = malloc(sizeof(char) * 20);
    spiderr = malloc(sizeof(char) * 20);
    if (dflag && mkdir("console_out", 0775) && errno != EEXIST) {
        printf("Error creating output directory\n");
        exit(-1);
    }

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
        if (pid == 0) {  // Child process exec
            dflag ? sprintf(spidout, "console_out/%d.out", getpid()) : sprintf(spidout, "%d.out", getpid()) ;
            if ((fdout = open(spidout,
                              O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1) {
                printf("Error opening <pid>.out\n");
                exit(-1);
            }
            dflag ? sprintf(spiderr, "console_out/%d.err", getpid()) : sprintf(spiderr, "%d.err", getpid());
            if ((fderr = open(spiderr,
                              O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1) {
                printf("Error opening <pid>.err\n");
                exit(-1);
            }
            dup2(fdout, 1);
            dup2(fderr, 2);
            execvp(total_args[0], total_args);
            perror("Exec failed");
            exit(-1);
        }
        else if (pid > 0) {  // Parent process
            wait(&status);
            if (WEXITSTATUS(status) == 0) {
                time(&current_time);
                fputs(ctime(&current_time), file_out);
            }
            else {
                printf("Child process \"%s\" terminated with status %d\n", total_args[0], status);
                exit(-1);
            }
        }
        else {  // ICE
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    }
    // Final cleanup
    free(line);
    free(token);
    free(spidout);
    free(spiderr);
    fclose(file_in);
    fclose(file_out);

    return 0;
}