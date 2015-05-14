/**
 * Simple command/subcommand library
 *
 *
 * @license MIT
 *
 *
 */
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <stdio.h>
#include <string.h>


typedef int (*command_callback_t)(int, char**);

// A command is described by:
//  - name: The name which should be passed as a second parameter
//  - executor: The function that will be executed if the command
//      matches. Receives the argc of the program minus two, and the rest
//      of argv
//  - description: Brief description, to show in help messages.
//  - longdesc: Long description to show when you run `help <command>
struct command {
    const char* const name;
    int (*executor)(int, char**);
    const char* const description;
    const char* const longdesc;
};

// Gets a pointer to a command, to allow implementing custom behavior
// returns null if not found.
//
// NOTE: Command arrays must end in a command with no name
const struct command*
command_by_name(const struct command* commands,
                const char* const name);

// Default help command if implementation doesn't provide one
int command_default_help(const char* const program_name,
                         const struct command* commands,
                         int argc,
                         char** argv);

// Tries to execute a command in the command list. The command name must be
// in argv[1]. Example usage:
//
// int main(int argc, char** argv) {
//     return exec_comand(my_commands, argc, argv);
//  }
int command_exec(const struct command* commands, int argc, char** argv);

#endif
