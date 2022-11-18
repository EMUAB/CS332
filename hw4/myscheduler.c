#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "queue.h"

#define CMD_BUF_SIZE 0x800

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct cmd_info
{
    int jobid;
    char *cmd;
};

void *runCmd(void *arg)
{
    struct cmd_info *info = (struct cmd_info *)arg;
    int jobid = info->jobid;
    char *cmd = info->cmd;

    char *total_args[20];
    size_t maxlen = 0;
    ssize_t n;
    int count, pid, status;
    int fdout, fderr;
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Command must be in the format: ./myscheduler <# of cores>\n");
        exit(-1);
    }
    int threads = atoi(argv[1]);
    char cmd[CMD_BUF_SIZE], cmd_dup[CMD_BUF_SIZE];
    char *token;
    while (1)
    {
        // Prompt for command to be entered
        printf("Enter command> ");
        fgets(cmd, CMD_BUF_SIZE, stdin);
        cmd[strcspn(cmd, "\n")] = 0;

        strcpy(cmd_dup, cmd);
        strtok(cmd_dup, " ");
        if (!strcmp(cmd_dup, "submit"))
        {

            printf("%s\n", cmd);
        }
        else if (!strcmp(cmd_dup, "showjobs"))
        {
            printf("%s\n", cmd);
        }
        else
        {
            printf("\"%s\" not OK\t", cmd_dup);
            printf("Available commands: \"submit\", \"showjobs\"\n");
        }

        fflush(stdin);
        fflush(stdout);
    }

    return 0;
}
