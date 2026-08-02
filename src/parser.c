#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/cli.h"

typedef void (*CommandHandler)(void);

typedef struct
{
const char *command;
CommandHandler handler;
}Command;

static const Command command_table[] =
{
{"add",         cli_add},
{"list",        cli_list},
{"get",         cli_get},
{"find",        cli_find},
{"update",      cli_update},
{"delete",      cli_delete},
{"generate",    cli_generate},
};

#define COMMAND_COUNT (sizeof (command_table) / sizeof(command_table[0]))

void parse_command(const char*input)

{
    if(input == NULL)
    return;

    if(strcmp(input , "exit") == 0)
    {
        printf("Exiting Crypton...\n");
        exit(EXIT_SUCCESS);
    }

    if(strcmp(input , "help") == 0)
    {
        printf("Commands: add, list, get, find, update, delete, generate, help, exit\n");
        return;
    }

    for(size_t i = 0 ; i < COMMAND_COUNT; i++)
    {
        if (strcmp(input , command_table[i].command) == 0)
        {
            command_table[i].handler();
            return;
        }
    }

    printf("Unknown command : %s\n" , input);
}

