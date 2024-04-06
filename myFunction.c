#include <stdio.h>
#include "myFunction.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h> 

#define BUFFER_SIZE 4096

char *getInputFromUser()
{
    char ch;
    int size = 1;
    int index = 0;
    char *str = (char *)malloc(size * sizeof(char));
    while ((ch = getchar()) != '\n')
    {
        *(str + index) = ch;
        size++;
        index++;
        str = (char *)realloc(str, size * sizeof(char));
    }
    *(str + index) = '\0';

    return str;
}

char *myStrtok(char *str, const char *delim) {
    static char *nextToken = NULL;
    if (str) nextToken = str;  
    if (!nextToken) return NULL;

    char *startToken = nextToken;
    char *endToken;
    bool inQuote = false;


    for (endToken = startToken; *endToken; ++endToken) {
        if (*endToken == '\"') {
            inQuote = !inQuote;
            memmove(endToken, endToken + 1, strlen(endToken));
            endToken--;
        } else if (!inQuote && strchr(delim, *endToken) != NULL) {
            break;
        }
    }

    if (!*endToken) {
        nextToken = NULL;
    } else {
        *endToken = '\0';
        nextToken = endToken + 1; 
    }

    if (startToken == nextToken) return NULL;

    return startToken;
}

char** splitOnPipe(const char* command) {
    char** result = malloc(2 * sizeof(char*));
    if (result == NULL) {
        perror("Failed to allocate memory for result");
        return NULL;
    }

    result[0] = NULL;
    result[1] = NULL;

    const char* pipePos = strchr(command, '|');
    if (pipePos == NULL) {
        free(result);
        return NULL;
    }

    size_t beforeLength = pipePos - command;
    size_t afterLength = strlen(command) - beforeLength - 1;

    result[0] = malloc(beforeLength + 1);
    result[1] = malloc(afterLength + 1);

    if (result[0] == NULL || result[1] == NULL) {
        perror("Failed to allocate memory for split parts");
        free(result[0]);
        free(result[1]);
        free(result);
        return NULL;
    }

    strncpy(result[0], command, beforeLength);
    result[0][beforeLength] = '\0';
    strncpy(result[1], pipePos + 1, afterLength);
    result[1][afterLength] = '\0';

    for (int i = 0; i < 2; i++) {
        char *start = result[i], *end;

        while (isspace((unsigned char)*start)) start++;

        end = start + strlen(start) - 1;

        while (end > start && isspace((unsigned char)*end)) end--;
        
        *(end + 1) = '\0';

        if (start > result[i]) {
            memmove(result[i], start, end - start + 2);
        }
    }

    return result;
}
char *normalizePath(char *path) {
    if (path == NULL) {
        return NULL;
    }

    int start = 0, end = strlen(path) - 1;
    
    while ((path[start] == ' ' || path[start] == '"') && start <= end) {
        start++;
    }

    while ((path[end] == ' ' || path[end] == '"') && end >= start) {
        end--;
    }

    int length = end - start + 1;
    if (length <= 0) {
        char *emptyPath = malloc(1);
        if (emptyPath) {
            emptyPath[0] = '\0';
        }
        return emptyPath;
    }

    char *normalizedPath = malloc(length + 1);
    if (normalizedPath == NULL) {
        perror("Failed to allocate memory for normalized path");
        return NULL;
    }

    int j = 0;
    bool lastWasSlash = false; 

    for (int i = start; i <= end; i++) {
        if (path[i] == '/') {
            if (!lastWasSlash) {
                normalizedPath[j++] = path[i];
            }
            lastWasSlash = true;
        } else if (path[i] == ' ') {
            if (!lastWasSlash && ((i + 1 <= end && path[i + 1] != '/') && (i - 1 >= start && path[i - 1] != '/'))) {
                normalizedPath[j++] = path[i];
            }
        } else {
            normalizedPath[j++] = path[i];
            lastWasSlash = false;
        }
    }

    normalizedPath[j] = '\0';

    return normalizedPath;
}


char **splitArgument(char *str) {
    int size = 1, index = 0;
    char **arguments = (char **)malloc((size + 1) * sizeof(char *));
    if (!arguments) {
        perror("malloc failed");
        return NULL;
    }

    char *subStr = myStrtok(str, " ");
    while (subStr != NULL) {
        arguments[index++] = subStr;
        size++;
        arguments = (char **)realloc(arguments, (size + 1) * sizeof(char *));
        if (!arguments) {
            perror("realloc failed");
            return NULL;
        }
        subStr = myStrtok(NULL, " ");
    }
    arguments[index] = NULL;

    return arguments;
}

char* trim(char* str) {
    while (isspace((unsigned char)*str)) str++; 
    if (*str == 0) return str; 

    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0'; 

    return str;
}

void getLocation()
{
    char cwd[1024];
    char hostname[1024];

    struct passwd *pw = getpwuid(geteuid());
    char *username = pw ? pw->pw_name : "unknown";

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        return;
    }

    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        perror("gethostname() error");
        return;
    }

    printf("\033[1;36m%s@%s\033[0m:\033[1;34m%s\033[0m$ ", username, hostname, cwd);
}

void welcome()
{
    printf("\n"
           "/***\n"
           " *              _      \n"
           " *            /\\ \\     \n"
           " *           /  \\ \\    \n"
           " *          / /\\ \\ \\   \n"
           " *         / / /\\ \\ \\  \n"
           " *        / / /  \\ \\_\\ \n"
           " *       / / /    \\/_/ \n"
           " *      / / /          \n"
           " *     / / /________   \n"
           " *    / / /_________\\  \n"
           " *    \\/____________/  \n"
           " *                     \n"
           " */\n\n"
           "Welcome to myShell\n"
           "https://github.com/ArtyomRabinovich/final_project_c\n\n"
           "\"Programming in C is like cooking with only a sharp knife and a block of ice - you have to be clever with what you've got!\" - Brian Kernighan\n\n");
}

void logout(char* trimmedInput) {
    printf("Exiting program.\n");
    exit(EXIT_SUCCESS);
}


void cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: cd <path>\n");
    }
    else
    {
        char *normalizedPath = normalizePath(args[1]);

        if (normalizedPath != NULL)
        {
            if (chdir(normalizedPath) != 0)
            {
                perror("cd failed");
            }
            free(normalizedPath);
        }
        else
        {
            fprintf(stderr, "Error: Failed to process the path. It might be empty or invalid.\n");
        }
    }
}

void cp(char **args)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "Usage: cp <source> <destination>\n");
        return;
    }

    char *sourcePathNormalized = normalizePath(args[1]);
    char *destinationPathNormalized = normalizePath(args[2]);

    if (sourcePathNormalized == NULL || destinationPathNormalized == NULL)
    {
        fprintf(stderr, "Error normalizing paths.\n");
        if (sourcePathNormalized != NULL)
            free(sourcePathNormalized);
        if (destinationPathNormalized != NULL)
            free(destinationPathNormalized);
        return;
    }


    FILE *sourceFile = fopen(sourcePathNormalized, "rb");
    if (sourceFile == NULL)
    {
        perror("Failed to open source file");
        free(sourcePathNormalized);
        free(destinationPathNormalized);
        return;
    }

  
    FILE *destinationFile = fopen(destinationPathNormalized, "wb");
    if (destinationFile == NULL)
    {
        perror("Failed to open destination file");
        fclose(sourceFile);
        free(sourcePathNormalized);
        free(destinationPathNormalized);
        return;
    }

    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0)
    {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);
    free(sourcePathNormalized);
    free(destinationPathNormalized);
    printf("File copied successfully.\n");
}

void delete(char *input)
{
    char **args = splitArgument(input);
    if (args == NULL || args[1] == NULL)
    {
        fprintf(stderr, "delete: expected a file path\n");
        if (args != NULL)
        {
            free(args);
        }
        return;
    }

    char *normalizedPath = normalizePath(args[1]);
    if (normalizedPath == NULL)
    {
        fprintf(stderr, "Error normalizing path.\n");
        free(args);
        return;
    }

    if (remove(normalizedPath) != 0)
    {
        perror("Error deleting file");
    }
    else
    {
        printf("File deleted successfully.\n");
    }

    free(normalizedPath);
    free(args);
}

void mypipe(char **argv1, char **argv2) {
    int pipefd[2];
    pid_t cpid1, cpid2;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid1 = fork();
    if (cpid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid1 == 0) { 
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); 

        execvp(argv1[0], argv1);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        cpid2 = fork();
        if (cpid2 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (cpid2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);

            execvp(argv2[0], argv2);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {
            close(pipefd[0]);
            close(pipefd[1]);
            wait(NULL);
            wait(NULL);
        }
    }
}


void move(char **args)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "Usage: mv <source> <destination>\n");
        return;
    }

    char *sourcePathNormalized = normalizePath(args[1]);
    char *destinationPathNormalized = normalizePath(args[2]);

    if (access(sourcePathNormalized, F_OK) != 0)
    {
        perror("Source file does not exist");
        free(sourcePathNormalized);
        free(destinationPathNormalized);
        return;
    }

    if (rename(sourcePathNormalized, destinationPathNormalized) != 0)
    {
        if (errno == EISDIR)
        {
            char *fileName = strrchr(sourcePathNormalized, '/');
            if (fileName == NULL)
            {
                fileName = sourcePathNormalized;
            }
            else
            {
                fileName += 1;
            }

            size_t newDestPathLen = strlen(destinationPathNormalized) + strlen(fileName) + 2; // +2 for '/' and '\0'
            char *newDestPath = (char *)malloc(newDestPathLen);
            if (newDestPath == NULL)
            {
                perror("Failed to allocate memory");
                free(sourcePathNormalized);
                free(destinationPathNormalized);
                return;
            }

            snprintf(newDestPath, newDestPathLen, "%s/%s", destinationPathNormalized, fileName);

            if (rename(sourcePathNormalized, newDestPath) != 0)
            {
                perror("Failed to move file");
            }
            else
            {
                printf("File moved successfully.\n");
            }

            free(newDestPath);
        }
        else
        {
            perror("Failed to move file");
        }
    }
    else
    {
        printf("File moved successfully.\n");
    }

    free(sourcePathNormalized);
    free(destinationPathNormalized);
}

void echoppend(char **args)
{
    int i = 1;
    for (; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">>") == 0)
        {
            break;
        }
    }

    char *textToAppend = NULL;
    int length = 0;
    for (int j = 1; j < i; j++)
    {
        length += strlen(args[j]) + 1;
    }

    textToAppend = (char *)malloc(length);
    if (!textToAppend)
    {
        perror("Allocation failure");
        return;
    }

    textToAppend[0] = '\0';
    for (int j = 1; j < i; j++)
    {
        strcat(textToAppend, args[j]);
        if (j < i - 1)
        {
            strcat(textToAppend, " ");
        }
    }

    if (args[i] != NULL && args[i + 1] != NULL)
    {
        char *normalizedPath = normalizePath(args[i + 1]);
        if (!normalizedPath)
        {
            fprintf(stderr, "Path normalization error.\n");
            free(textToAppend);
            return;
        }

        FILE *file = fopen(normalizedPath, "a");
        if (!file)
        {
            perror("File opening failure");
            free(textToAppend);
            free(normalizedPath);
            return;
        }
        fprintf(file, "%s\n", textToAppend);
        fclose(file);
        free(normalizedPath);
    }
    else
    {
        fprintf(stderr, "Usage error: Missing file path for redirection.\n");
    }

    free(textToAppend);
}

void echorite(char **args)
{
    int i = 1; 
    for (; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            break;
        }
    }

    char *textToWrite = NULL;
    int length = 0;
    for (int j = 1; j < i; j++)
    {
        length += strlen(args[j]) + 1;
    }

    textToWrite = (char *)malloc(length);
    if (!textToWrite)
    {
        perror("Allocation failure");
        return;
    }

    textToWrite[0] = '\0';
    for (int j = 1; j < i; j++)
    {
        strcat(textToWrite, args[j]);
        if (j < i - 1)
        {
            strcat(textToWrite, " ");
        }
    }

    if (args[i] != NULL && args[i + 1] != NULL)
    {
        char *normalizedPath = normalizePath(args[i + 1]);
        if (!normalizedPath)
        {
            fprintf(stderr, "Path normalization error.\n");
            free(textToWrite);
            return;
        }

        FILE *file = fopen(normalizedPath, "w");
        if (!file)
        {
            perror("File opening failure");
            free(textToWrite);
            free(normalizedPath);
            return;
        }
        fprintf(file, "%s\n", textToWrite);
        fclose(file);
        free(normalizedPath);
    }
    else
    {
        fprintf(stderr, "Usage error: Missing file path for redirection.\n");
    }

    free(textToWrite);
}

void readI(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: read <filePath>\n");
        return;
    }

    char *normalizedPath = normalizePath(args[1]);
    if (normalizedPath == NULL)
    {
        fprintf(stderr, "Error normalizing path.\n");
        return;
    }

    FILE *file = fopen(normalizedPath, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: File '%s' not found.\n", normalizedPath);
        free(normalizedPath);
        return;
    }


    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
    }
    printf("\n");
    fclose(file);
    free(normalizedPath);
}

void wordCount(char **args)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "Usage: wordCount <-l|-w> <filePath>\n");
        return;
    }

    char *option = args[1];
    char *filePath = args[2];

    char *normalizedPath = normalizePath(filePath);
    if (normalizedPath == NULL)
    {
        fprintf(stderr, "Error normalizing path.\n");
        return;
    }

    FILE *file = fopen(normalizedPath, "r");
    if (!file)
    {
        perror("Failed to open file");
        free(normalizedPath);
        return;
    }

    int count = 0;
    char buffer[1024];

  if (strcmp(option, "-l") == 0)
{
    int lines = 0;
    bool isLastCharNewline = false;

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        lines++;
        size_t len = strlen(buffer);
        isLastCharNewline = (buffer[len - 1] == '\n');
    }


    if (!isLastCharNewline && lines > 0) {

    }

    printf("Line count: %d\n", lines);
}
    else if (strcmp(option, "-w") == 0)
    {
        char *token;
        while (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            token = strtok(buffer, " \t\n");
            while (token != NULL)
            {
                count++;
                token = strtok(NULL, " \t\n");
            }
        }
        printf("Word count: %d\n", count);
    }
    else
    {
        fprintf(stderr, "Invalid option: %s\n", option);
    }

    fclose(file);
    free(normalizedPath);
}

void help(void)
{
    printf("Available commands:\n");
    printf("  cd <directory> - Change the current directory to <directory>.\n");
    printf("  cp <source> <destination> - Copy <source> file to <destination>.\n");
    printf("  delete <file> - Delete the specified <file>.\n");
    printf("  move <source> <destination> - Move <source> to <destination>.\n");
    printf("  echo >> <text> <file> - Append <text> to <file>.\n");
    printf("  echo > <text> <file> - Overwrite <file> with <text>.\n");
    printf("  readI <file> - Display the content of <file>.\n");
    printf("  wc -l <file> - Count the number of lines in <file>.\n");
    printf("  wc -w <file> - Count the number of words in <file>.\n");
    printf("  exit - Exit the shell.\n");
    printf("  help - Display this help message.\n");
}

