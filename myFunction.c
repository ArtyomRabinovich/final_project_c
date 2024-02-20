#include <stdio.h>
#include "myFunction.h"


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
    static char *lastToken = NULL;
    if (str == NULL) {
        str = lastToken; 
    }
    if (str == NULL) { 
        return NULL;
    }

    
    str += strspn(str, delim);
    if (*str == '\0') {
        return NULL;
    }


    char *end = str + strcspn(str, delim);
    if (*end == '\0') {
        lastToken = NULL;
    } else {
        *end = '\0';
        lastToken = end + 1;
    }

    return str;
}

// עליכם לממש את הפונקציה strtok כלומר שהפונקציה הנ"ל תבצע בדיוק אותו הדבר רק בלי השימוש בפונקציה strtok
char **splitArgument(char *str)
{
    char *subStr;
    int size = 1;
    int index = 0;
    subStr = myStrtok(str, " "); 
    char **arguments = (char **)malloc((size + 1) * sizeof(char *));
    if (!arguments) {
        perror("malloc failed");
        return NULL;
    }

    while (subStr != NULL) {
        *(arguments + index) = subStr;
        index++;
        size++;
        arguments = (char **)realloc(arguments, (size + 1) * sizeof(char *)); // +1 for NULL terminator
        if (!arguments) {
            perror("realloc failed");
            return NULL;
        }
        subStr = myStrtok(NULL, " ");
    }
    *(arguments + index) = NULL;

    return arguments;
}

void getLocation() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("dir: %s$", cwd);
    } else {
        perror("getcwd() error");
    }
}