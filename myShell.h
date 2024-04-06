#ifndef MYSHELL_H
#define MYSHELL_H

#include "myFunction.h"

/**
 * The main function serves as the entry point for the shell application, initiating
 * the shell's interactive loop and processing user commands.
 * 
 * Features and Flow:
 * 1. Display a welcome message to the user, introducing the shell.
 * 2. Enter an infinite loop to continuously prompt for and handle user commands.
 *    - The loop includes prompting the user with the current location (directory),
 *      reading user input, and processing the input to execute commands.
 * 3. Support for basic shell commands, including navigation (`cd`), file manipulation
 *    (`cp`, `delete`, `move`), text manipulation (`echo`), content display (`readI`),
 *    and informational (`wc`, `help`) commands.
 * 4. Implementation of piping between commands to allow for advanced command chaining.
 * 5. Dynamic memory management to ensure flexibility in handling user input and command
 *    processing, with appropriate cleanup to prevent memory leaks.
 * 6. Clean termination of the shell upon receiving the `exit` command, ensuring all
 *    allocated resources are freed and the shell exits gracefully.
 * 
 * The `main` function leverages functions defined in `myFunction.h` for executing
 * individual commands, showcasing the modular design of the shell. It represents a
 * comprehensive command-line interface for user interaction, combining built-in
 * command execution with basic shell functionalities.
 */

#endif // MYSHELL_H
