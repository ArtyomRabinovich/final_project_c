#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFF_SIZE 256
#define blue() printf("\033[0;34m")
#define reset() printf("\033[0m")
#define green() printf("\033[0;32m")
#define bold() printf("\e[1m")
#define boldOff() printf("\e[m")

/**
 * Retrieves input from the user until a newline is entered. Dynamically allocates
 * memory for the input string, which must be freed by the caller to avoid memory leaks.
 * This function is useful for interactive command-line applications where input from
 * the user is required. It supports input of arbitrary length, ensuring that the program
 * can handle user input flexibly.
 *
 * Usage example:
 *   char *userInput = getInputFromUser();
 *   printf("You entered: %s\n", userInput);
 *   free(userInput); // Important to free the allocated memory
 *
 * @return A pointer to the dynamically allocated string containing the user's input.
 *         Returns NULL if there was an error allocating memory.
 */
char *getInputFromUser(void);

/**
 * Custom implementation of the string tokenization function that handles consecutive
 * delimiters and supports state persistence between calls. This function is designed
 * to be a more flexible alternative to the standard `strtok` function provided by C.
 * Unlike `strtok`, which uses a static buffer to maintain state between calls, `myStrtok`
 * manages its state in a way that makes it safe to use in concurrent or reentrant code.
 *
 * The function splits the input string `str` into tokens, which are sequences of characters
 * separated by characters found in `delim`. On the first call, `str` should point to the
 * string to be tokenized. Subsequent calls should pass NULL as `str` to continue tokenizing
 * the same string. `myStrtok` allows consecutive delimiters in the input string and treats
 * them as a single delimiter.
 *
 * Usage example:
 *   char str[] = "This,is,,a,test";
 *   const char delim[] = ",";
 *   char *token = myStrtok(str, delim);
 *   while (token != NULL) {
 *       printf("%s\n", token);
 *       token = myStrtok(NULL, delim);
 *   }
 *
 * @param str The string to tokenize on the first call; NULL to get the next token.
 * @param delim A null-terminated string containing the delimiters.
 * @return A pointer to the next token found in the string. Returns NULL when no more
 *         tokens are found. Each returned token pointer points to a null-terminated string.
 */
char *myStrtok(char *str, const char *delim);


/**
 * Splits a given string into an array of substrings based on spaces as delimiters,
 * leveraging a custom string tokenization function `myStrtok`. This function is
 * particularly useful for parsing command-line inputs into separate arguments where
 * arguments are separated by spaces. It dynamically allocates memory for the array
 * of argument strings and each argument string individually.
 *
 * The function iterates through the input string, tokenizing it by spaces. Each token
 * is added to a dynamically growing array of strings, which is resized as needed using
 * `realloc` to accommodate new tokens.
 *
 * Note: It's essential to free the memory allocated for the array and its contents
 *       when they are no longer needed to avoid memory leaks.
 *
 * Usage example:
 *   char input[] = "ls -l /home/user";
 *   char **args = splitArgument(input);
 *   for (int i = 0; args[i] != NULL; i++) {
 *       printf("%s\n", args[i]);
 *   }
 *   // Free allocated memory here
 *
 * @param str The input string to be split into arguments.
 * @return A pointer to a dynamically allocated array of strings, where each string
 *         is a space-delimited substring of the input. The array is null-terminated.
 *         Returns NULL if memory allocation fails at any point.
 */
char **splitArgument(char *str);


/**
 * Prints the current working directory along with the username and hostname in a
 * stylized format. This function is useful for shell environments to display a prompt
 * that includes contextual information about the user's current location within the
 * filesystem, as well as the system identity.
 *
 * The function retrieves the current working directory using `getcwd`, the current
 * user's username from the user's UID, and the system's hostname. If retrieving the
 * working directory or hostname fails, an error message is printed to standard error.
 *
 * The output is color-coded for visual distinction, with the username and hostname
 * displayed in cyan and the working directory in blue. ANSI escape codes are used
 * for coloring, making this function suitable for terminals that support such codes.
 *
 * Usage example:
 *   getLocation();
 *   // output: username@hostname:/path/to/directory$
 *
 * Note: This function does not return any value. It directly prints the formatted
 *       string to standard output.
 */
void getLocation();

/**
 * Prints a welcome message along with a stylized ASCII art logo to standard output.
 * This function is designed to be called at the start of a shell or application to greet
 * the user and provide some basic information or motivation. The message includes a
 * URL pointing to a GitHub repository, which could be replaced or removed as needed
 * based on the application's context.
 *
 * The ASCII art and the quotes are meant to add a touch of personality and humor to the
 * application, making the initial interaction with the user more engaging. This function
 * demonstrates how simple text output can be used effectively to enhance user experience.
 *
 * Usage example:
 *   welcome();
 *   // Outputs the ASCII art followed by "Welcome to myShell" and additional text.
 *
 * Note: This function does not take any parameters and does not return any value. It
 *       directly prints the welcome message and ASCII art to standard output.
 */
void welcome();

/**
 * Terminates the program with a message indicating the exit. This function is intended
 * to be called when the program should be exited cleanly. It displays a message to standard
 * output and then calls `exit` with `EXIT_SUCCESS` to end the program.
 *
 * The `trimmedInput` parameter, while not used within the function, signifies that this
 * function responds to a specific command, potentially after processing such as trimming.
 * However, the function's behavior does not change based on this input.
 *
 * Usage:
 *   logout(trimmedInput);
 *
 * @param trimmedInput A string that led to the invocation of this function.
 */
void logout(char* trimmedInput);


/**
 * Changes the current working directory of the process to the one specified in the
 * arguments. This function attempts to mimic the behavior of the shell command `cd`
 * by changing the process's working directory to the path given as the second argument
 * in the `args` array.
 *
 * If no path is provided (i.e., `args[1]` is NULL), or if the path processing fails,
 * an error message is printed to standard error indicating the correct usage or
 * reporting the failure to process the path.
 *
 * The path provided in `args[1]` is first normalized to remove any extraneous characters
 * or formatting issues. This normalized path is then used in a call to `chdir()` to
 * change the directory. If `chdir()` fails, an error message is printed using `perror`.
 *
 * Note: The function dynamically allocates memory for the normalized path, which is
 * subsequently freed before the function returns.
 *
 * @param args An array of strings where `args[0]` is assumed to be "cd" and `args[1]`
 *             is the target directory path. The array is expected to end with a NULL
 *             pointer.
 */
void cd(char **args);


/**
 * Copies the contents of one file to another. The function expects the source and 
 * destination file paths to be specified in the `args` array. It normalizes both paths 
 * before attempting the copy operation. If either path cannot be normalized, or if 
 * any of the required arguments are missing, an error message is displayed, and the 
 * function returns without performing the copy.
 *
 * The function opens the source file in binary read mode ('rb') and the destination 
 * file in binary write mode ('wb'), ensuring that the copy is performed in binary 
 * mode to preserve the exact content of the file, including any special characters 
 * or binary data. The contents are read from the source file into a buffer, then 
 * written from the buffer to the destination file, until the entire file has been 
 * copied. 
 *
 * After the operation is complete, the function frees the memory allocated for 
 * the normalized paths and closes both files. If the copy is successful, a 
 * confirmation message is printed to standard output.
 *
 * @param args An array of strings containing the command name, the source file path,
 *             and the destination file path.
 */
void cp(char **args);

/**
 * Deletes a file specified by the input string. The function first tokenizes the input
 * to extract the file path, then normalizes this path. If the path is successfully
 * normalized, it attempts to delete the file using the `remove` function. If the file
 * cannot be deleted, an error message is printed.
 *
 * Before attempting deletion, the function checks for valid input and prints a usage
 * message if the expected file path argument is not provided or if the input cannot
 * be tokenized properly. It also handles errors in path normalization by reporting
 * failure and exiting early.
 *
 * After attempting to delete the file, whether successful or not, the function frees
 * all dynamically allocated memory used for path normalization and argument tokenization.
 *
 * Note: This function is designed for use within applications that require file
 * management capabilities, allowing for the deletion of files through user commands
 * or programmatic input.
 *
 * @param input A string containing the 'delete' command followed by the file path to delete.
 */
void delete(char *input);

/**
 * Sets up a unidirectional pipe between two processes, allowing the standard output of
 * the first command to be directed to the standard input of the second command, mimicking
 * the behavior of a Unix shell pipe. This function creates a pipe, then forks two child
 * processes to execute the given external commands. It is intended to showcase inter-process
 * communication through pipes in scenarios where the shell itself might not have suitable
 * built-in commands for demonstration.
 *
 * The function performs the following steps:
 * 1. Creates a pipe using `pipe()`.
 * 2. Forks the first child process:
 *    - The child redirects its standard output to the write end of the pipe, then
 *      executes the first command using `execvp`.
 * 3. Forks the second child process:
 *    - This child connects its standard input to the read end of the pipe, then
 *      executes the second command using `execvp`.
 * 4. The parent process waits for both child processes to terminate before returning.
 *
 * If any step fails (creating a pipe, forking a process, or executing a command), an
 * error message is printed, and the program exits with `EXIT_FAILURE`.
 *
 * Note: This function assumes `argv1` and `argv2` are null-terminated arrays of strings
 * representing the command and its arguments to be executed by the child processes. The
 * use of external commands is necessary to demonstrate the functionality, as the custom
 * shell may not have suitable internal commands for such an illustration.
 *
 * @param argv1 A null-terminated array of strings for the first command and its arguments.
 * @param argv2 A null-terminated array of strings for the second command and its arguments.
 */
void mypipe(char **argv1, char **argv2);


/**
 * Moves or renames a file from a source path to a destination path. This function attempts
 * to mimic the behavior of the Unix 'mv' command. It first checks if the source and
 * destination paths are provided. If not, it outputs a usage message. Both paths are
 * normalized to handle any inconsistencies in their format.
 *
 * Before attempting to move the file, it checks if the source file exists using `access`.
 * If the source file does not exist, an error message is displayed, and the function
 * exits.
 *
 * The function then attempts to move the file using `rename`. If moving the file fails
 * because the destination is a directory (`errno` is `EISDIR`), it constructs a new
 * destination path by appending the source file name to the destination directory path.
 * It then tries to move the file again. If the move is successful, a confirmation message
 * is printed. Otherwise, an error message is displayed.
 *
 * Memory allocated for normalized paths is freed before the function exits. Additional
 * memory allocated for constructing the new destination path in case of a directory
 * destination is also freed after use.
 *
 * @param args An array of strings where `args[1]` is the source file path and `args[2]`
 *             is the destination path or directory. The array should end with a NULL
 *             pointer.
 */
void move(char **args);


/**
 * Appends the provided text to the specified file. This function takes an array of
 * strings where the text to append is specified up to the ">>" marker, and the file path
 * follows the marker. It opens the specified file in append mode and writes the text
 * to it, creating the file if it does not exist.
 *
 * This functionality is similar to the shell's append redirect operation. It's useful
 * for logging or adding information to a file without overwriting its current contents.
 *
 * Usage example:
 *   args = {"echoppend", "This text will be appended.", ">>", "log.txt", NULL};
 *   echoppend(args);
 *
 * The function checks for the existence of the ">>" marker to separate the text from
 * the file path. If the file cannot be opened or written to, an error message is printed
 * to standard error.
 *
 * @param args An array of strings containing the text to append, the ">>" marker,
 *             and the path to the file. The array is expected to end with a NULL pointer.
 */
void echoppend(char **args);


/**
 * Writes (overwrites) the provided text to a file specified in the arguments. This function
 * parses an array of strings where the text to be written is specified up to the ">" marker,
 * and the file path is indicated after the marker. It opens the specified file in write mode,
 * which creates the file if it does not exist or truncates it to zero length if it does, and
 * then writes the text to the file.
 *
 * This functionality mimics the shell's overwrite redirect operation, allowing for simple
 * text output to files within a program. It's useful for generating logs, reports, or any
 * form of output that requires saving to a file.
 *
 * Usage example:
 *   args = {"echorite", "This text will overwrite the file.", ">", "output.txt", NULL};
 *   echorite(args);
 *
 * The function identifies the ">" marker to separate the text from the file path. In case
 * the file cannot be opened or written to, an error message is printed to standard error.
 *
 * @param args An array of strings containing the text to write, the ">" marker, and the
 *             path to the file. The array is expected to end with a NULL pointer.
 */
void echorite(char **args);


/**
 * Normalizes a file path by removing redundant slashes, trimming leading and trailing
 * whitespace, and eliminating surrounding quotes if present. This function is useful
 * in command-line applications or scripts where file paths may be input by the user
 * and need to be sanitized before use. It dynamically allocates memory for the
 * normalized path, which must be freed by the caller.
 *
 * The normalization process includes:
 * - Trimming leading and trailing whitespace.
 * - Removing surrounding quotes (single or double).
 * - Replacing consecutive slashes with a single slash.
 *
 * Usage example:
 *   char rawPath[] = "  \"/some///path/\"  ";
 *   char *normPath = normalizePath(rawPath);
 *   printf("Normalized path: %s\n", normPath);
 *   free(normPath); // Free the dynamically allocated memory
 *
 * @param path The file path string to normalize.
 * @return A dynamically allocated string containing the normalized path. The caller
 *         is responsible for freeing this memory. Returns NULL if memory allocation fails.
 */
char *normalizePath(char *path);

/**
 * Splits a command string into two separate strings based on the first occurrence
 * of a pipe symbol ('|'). This function is particularly useful for parsing command
 * lines that involve piping between two commands in a shell-like interface. The
 * function dynamically allocates memory for an array of two strings, representing
 * the command before and after the pipe symbol. It's the caller's responsibility
 * to free the allocated memory for both the array and the strings within it to
 * prevent memory leaks.
 *
 * If the input command does not contain a pipe symbol, the returned array will
 * contain the original command as its first element and NULL as its second element.
 *
 * Usage example:
 *   char command[] = "ls -l | grep 'Jun'";
 *   char **splitCommands = splitOnPipe(command);
 *   if (splitCommands[0] && splitCommands[1]) {
 *       printf("Command 1: %s\n", splitCommands[0]);
 *       printf("Command 2: %s\n", splitCommands[1]);
 *   }
 *   // Free allocated memory
 *   free(splitCommands[0]);
 *   free(splitCommands[1]);
 *   free(splitCommands);
 *
 * @param command The command string to be split on the first pipe symbol.
 * @return An array of two dynamically allocated strings split on the first pipe symbol.
 *         The caller must free the allocated memory. If there's no pipe in the command,
 *         the second string in the array will be NULL.
 */
char** splitOnPipe(const char* command);

/**
 * Removes leading and trailing whitespace from a string. This function modifies the input
 * string in place and returns a pointer to the modified string. Whitespace is defined by
 * the standard `isspace` function, which includes spaces, tabs, newlines, and other common
 * whitespace characters.
 *
 * The function iterates over the string from the beginning to find the first non-whitespace
 * character and then from the end to find the last non-whitespace character. It then
 * terminates the string at the last non-whitespace character, effectively removing trailing
 * whitespace. If the string is entirely whitespace, the function returns a pointer to the
 * string's null terminator, resulting in an empty string.
 *
 * Usage example:
 *   char example[] = "  hello world  ";
 *   printf("'%s'\n", trim(example));
 *   // Output: 'hello world'
 *
 * Note: Since this function modifies the input string in place, it does not allocate new
 *       memory and thus does not require any memory to be freed. However, this also means
 *       it should not be used with string literals or any read-only strings, as attempting
 *       to modify such strings results in undefined behavior.
 *
 * @param str The string to be trimmed. Must be a modifiable string, not a string literal.
 * @return A pointer to the trimmed string, which is the same as the input string modified
 *         in place.
 */
char* trim(char* str);


/**
 * Reads and displays the content of a specified file. This function expects an array of
 * strings as its argument, where the first element after the command itself should be the
 * file path to read. It first normalizes the provided file path to handle any irregularities
 * like redundant slashes or surrounding quotes. If the file path is not provided or the
 * normalization fails, it prints an error message to standard error.
 *
 * Upon successfully opening the file, `readI` reads the file content line by line into a
 * buffer and prints it to standard output until reaching the end of the file. It then frees
 * the dynamically allocated memory for the normalized path and closes the file.
 *
 * Usage example:
 *   char *args[] = {"readI", "example.txt", NULL};
 *   readI(args);
 *   // This will print the content of 'example.txt' to standard output.
 *
 * Note: This function is designed to be used in a shell or command-line utility where
 *       reading file content directly to the terminal is a common operation. It provides
 *       basic error handling for missing arguments and file access issues.
 *
 * @param args An array of strings containing the function name and the path to the file
 *             to be read. The array is expected to end with a NULL pointer to mark the end
 *             of arguments.
 */
void readI(char **args);



/**
 * Counts the number of words or lines in the specified file, depending on the option
 * provided. This function is designed to mimic the basic functionality of the Unix
 * `wc` command, but tailored to work within the constraints of this application. It
 * accepts an array of strings where the first element (after the function name) should
 * be the option ("-w" for words, "-l" for lines) and the second element should be the
 * path to the file to be analyzed.
 *
 * Usage example:
 *   char *args[] = {"wordCount", "-w", "example.txt", NULL};
 *   wordCount(args); // Counts words in example.txt
 *
 *   char *args[] = {"wordCount", "-l", "example.txt", NULL};
 *   wordCount(args); // Counts lines in example.txt
 *
 * The function does not return a value but prints the count directly to standard output.
 * If the file cannot be opened or read, an error message is printed instead.
 *
 * @param args An array of strings containing the function name, the option ("-w" or "-l"),
 *             and the path to the file. The array is expected to end with a NULL pointer.
 */
void wordCount(char **args);


/**
 * Displays a list of available commands and their descriptions.
 *
 * This function prints a help message to the standard output, listing all
 * the available commands supported by the shell along with a brief description
 * of what each command does. It's intended to assist users in understanding the
 * functionality of the shell and how to use its commands effectively.
 *
 * Usage:
 *   help
 *
 * Example:
 *   > help
 *   Available commands:
 *     cd <directory> - Change the current directory to <directory>
 *     cp <source> <destination> - Copy <source> to <destination>
 *     delete <file> - Delete <file>
 *     ... and so on for each command ...
 *
 * Note:
 *   This command does not require any arguments. Typing 'help' will display
 *   the list of commands.
 */
void help(void);

#endif // MYFUNCTION_H
