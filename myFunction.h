#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFF_SIZE 256
#define blue() printf("\033[0;34m")
#define reset() printf("\033[0m")
#define green() printf("\033[0m")
#define bold() printf("\e[1m")
#define boldOff() printf("\e[m")



/**
 * Reads a string from the standard input (stdin) until a newline character is encountered.
 *
 * This function dynamically allocates memory to store the input string, including
 * space for the null terminator. The caller is responsible for freeing this memory
 * when it is no longer needed.
 *
 * @return A pointer to the dynamically allocated string containing the user input,
 *         excluding the newline character. Returns NULL if memory allocation fails.
 *
 * @warning The caller is responsible for freeing the memory allocated for the
 *          returned string using free().
 */
char* getInputFromUser(void);

/**
 * Splits a string into individual tokens based on whitespace.
 * 
 * This function takes a string as input and tokenizes it based on whitespace
 * characters (space, tab, newline). It dynamically allocates memory for an
 * array of strings to store the tokens. The caller is responsible for freeing
 * the memory allocated for the array and its elements when it's no longer needed.
 * 
 * @param input A pointer to the string to be split.
 * 
 * @return A dynamically allocated array of strings (char**) containing the tokens.
 *         The last element of the array is NULL. Returns NULL if input is NULL
 *         or if memory allocation fails.
 * 
 * @note The input string will be modified (tokenized) by this function.
 *       Make sure to use a copy of the original string if preservation
 *       of the original string is required.
 * 
 * @warning The caller is responsible for freeing the memory allocated for
 *          the returned array and its elements using free().
 */
char** splitArgument(char*);


/**
 * Retrieves and prints the current working directory to standard output.
 *
 * This function attempts to obtain the current working directory path and prints
 * it to the standard output. If successful, the directory path is followed by a
 * dollar sign ('$'). If an error occurs, an error message is printed instead.
 *
 * @warning This function does not return the directory path but prints it directly
 *          to the standard output.
 */
void getLocation(void);
