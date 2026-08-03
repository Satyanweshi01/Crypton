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
} Command;

static const Command command_table[] =
    {
        {"add", cli_add},
        {"list", cli_list},
        {"get", cli_get},
        {"find", cli_find},
        {"update", cli_update},
        {"delete", cli_delete},
        {"generate", cli_generate},
        {"clear", cli_clear}};

#define COMMAND_COUNT (sizeof(command_table) / sizeof(command_table[0]))

void parse_command(const char *input)

{
    if (input == NULL)
        return;

    if (strcmp(input, "exit") == 0)
    {
        printf("Exiting Crypton...\n");
        cli_clear();
        exit(EXIT_SUCCESS);
    }

    if (strcmp(input, "help") == 0)
    {
        printf("Commands:\n");
        printf("  add       Create a new password entry\n");
        printf("  list      Show all stored account names & their password\n");
        printf("  get       Retrieve and copy a specific password to clipboard\n");
        printf("  find      Search for a service matching a keyword\n");
        printf("  update    Change the informations for an existing account\n");
        printf("  delete    Permanently remove a specific service\n");
        printf("  generate  Create a strong, secure random password\n");
        printf("  help      Display this command menu\n");
        printf("  clear     Clear the console screen\n");
        printf("  exit      Securely close the application\n\n");
        return;
    }

    for (size_t i = 0; i < COMMAND_COUNT; i++)
    {
        if (strcmp(input, command_table[i].command) == 0)
        {
            command_table[i].handler();
            return;
        }
    }

    printf("Unknown command : %s\n", input);
}
