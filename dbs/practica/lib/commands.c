#include "commands.h"


// Gets a pointer to a command, to allow implementing custom behavior
// returns null if not found.
//
// NOTE: Command arrays must end in a command with no name
const struct command*
command_by_name(const struct command* commands,
                const char* const name) {
    size_t i = 0;

    if ( ! commands )
        return NULL;

    while ( 1 ) {
        // The list of commands is over and we didn't find
        // anything
        if ( ! commands[i].name )
            return NULL;

        if ( strcmp(commands[i].name, name) == 0 )
            return commands + i;

        ++i;
    }

    return NULL;
}

// Default help command if implementation doesn't provide one
int command_default_help(const char* const program_name,
                         const struct command* commands,
                         int argc,
                         char** argv) {
    size_t i = 0;
    const struct command* command;

    if ( ! commands )
        return 1;

    if ( argc == 0 ) {
        printf("Usage: %s <command> [args...]\n", program_name);
        printf("Available commands:\n");

        while ( 1 ) {
            if ( ! commands[i].name )
                break;

            printf("\t%s\t%s\n", commands[i].name, commands[i].description);
            ++i;
        }

        printf("To see extended info on each command run:\n");
        printf("\t%s help <command>\n", program_name);

        return 1;
    } else if ( argc > 1 ) {
        printf("%s: unrecognized argument: \"%s\"\n", program_name, argv[1]);
        return 1;
    }

    command = command_by_name(commands, argv[0]);

    if ( ! command ) {
        printf("%s: command \"%s\" not found\n", program_name, argv[0]);
        return 1;
    }

    printf("%s: %s\n", command->name, command->description);
    if ( command->longdesc )
        printf("\n%s\n", command->longdesc);

    return 1;
}


// Tries to execute a command in the command list. The command name must be
// in argv[1]. Example usage:
//
// int main(int argc, char** argv) {
//     return exec_comand(my_commands, argc, argv);
//  }
int command_exec(const struct command* commands, int argc, char** argv) {
    const struct command* command;

    if ( argc == 1 ) {
        command = command_by_name(commands, "help");

        if ( command )
            return command->executor(argc - 1, argv + 1);

        return command_default_help(argv[0], commands, argc - 1, argv + 1);
    }

    command = command_by_name(commands, argv[1]);

    if ( command )
        return command->executor(argc - 2, argv + 2);

    if ( strcmp(argv[1], "help") == 0 )
        return command_default_help(argv[0], commands, argc - 2, argv + 2);

    printf("%s: command \"%s\" not found\n", argv[0], argv[1]);
    printf("\trun `%s help` for a list of options\n", argv[0]);
    return 1;
}
