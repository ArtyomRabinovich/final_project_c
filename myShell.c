#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "myFunction.h" 

int main(void) {
    char *input;
    char **args;
    char *command;

    welcome(); 

    while (1) {
        getLocation();              
        printf("> ");
        input = getInputFromUser(); 

        if (input == NULL || strlen(input) == 0) {
            free(input);
            continue;
        }
    char *inputCopy = strdup(input); 
        if (!inputCopy) {
        perror("Failed to copy input");
        free(input);
        continue;
        }
    char* trimmedInput = trim(input);
    if (strncmp(trimmedInput, "exit", 4) == 0) {
        logout(trimmedInput); 
    }
        char **splitCommands = splitOnPipe(input);
        if (splitCommands) {
            char **argv1 = splitArgument(splitCommands[0]);
            char **argv2 = splitArgument(splitCommands[1]);

            mypipe(argv1, argv2); 

            free(argv1);
            free(argv2);
            free(splitCommands[0]);
            free(splitCommands[1]);
            free(splitCommands);
        } else {
            args = splitArgument(input);

            if (args == NULL) {
                free(input);
                continue;
            }
            int i = 0;


            command = args[0];

            if (strcmp(command, "help") == 0) {
                help();
            } else if (strcmp(command, "cd") == 0) {
                cd(args);
            } else if (strcmp(command, "cp") == 0) {
                cp(args);
            } else if (strcmp(command, "delete") == 0) {
                delete(inputCopy);
            } else if (strcmp(command, "move") == 0) {
                move(args);
            }  else if (strcmp(command, "echo") == 0)
            {
                int redirectIndex = 1; 
                for (; args[redirectIndex] != NULL; redirectIndex++)
                {
                    if (strcmp(args[redirectIndex], ">>") == 0 || strcmp(args[redirectIndex], ">") == 0)
                    {
                        break; 
                    }
                }

                if (args[redirectIndex] == NULL)
                {
                   
                    for (int i = 1; args[i] != NULL; i++)
                    {
                        printf("%s ", args[i]);
                    }
                    printf("\n");
                }
                else if (args[redirectIndex + 1] == NULL)
                {
                    fprintf(stderr, "Error: Redirection operator '%s' found but no file path specified.\n", args[redirectIndex]);
                }
                else
                {
                    if (strcmp(args[redirectIndex], ">>") == 0)
                    {
                        echoppend(args);
                    }
                    else
                    {
                        echorite(args);
                    }
                }
            } else if (strcmp(command, "read") == 0) {
                readI(args);
            } else if (strcmp(command, "wc") == 0) {
                wordCount(args);
            } else {
                printf("Command not found. Type 'help' for a list of commands.\n");
            }

            free(args);
        }

        free(input);
    }

    return 0;
}
