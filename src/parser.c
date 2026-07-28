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
{{"add", cli_add},
{"list", cli_list},
{"get",  cli_get},
{"update", cli_update},
{"delete", cli_delete},
{"generate", cli_generate},
};

#define COMMAND_COUNT(sizeof(command_table) / sizeof(command_table[0]))

