#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define MAX_COMMANDS 100

typedef struct {
    char *command;
    int id;
} command_t;

command_t commands[MAX_COMMANDS];
pthread_t thread[MAX_COMMANDS];
int numCommands;

// thread routine for executing commands
void *threadRoutine(void *arg){
    command_t *command = (command_t*) arg;
    printf("Executing command %d: %s\n", command->id, command->command);
    system(command->command);
}

int main(){
    char userInput[256];
    numCommands = 0;
    printf("Enter commands to execute: \n");
    while(fgets(userInput, 256, stdin) != NULL){
        // create command structure with info
        command_t command;
        command.command = strdup(userInput);
        command.id = numCommands;

        // create thread to execute command
        pthread_create(&thread[numCommands], NULL, threadRoutine, (void*) &command);
        numCommands++;
    }
    // wait for all the threads to finish
    for (int i = 0; i < numCommands; i++) {
        pthread_join(thread[i], NULL);
    }

    return 0;
}
