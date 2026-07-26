#include <stdio.h>
#include <string.h>
#include "../include/cli.h"
#include "../include/parser.h"
#define  MAX_COMMAND_LENGTH 100
void show_welcome(void)
{
    printf("******************************************\n");
    printf("               CRYPTON\n");
    printf("******************************************\n");
    printf("Secure Password Management System\n\n");
}

void show_shell(void)
{
    printf("Crypton> ");
}

void read_command(char *buffer)
{
   if(fgets(buffer , MAX_COMMAND_LENGTH, stdin) != NULL)
   {
    buffer[strcspn(buffer , "\n")] = '\0';
   }
}
void start_cli(void)
{
    char command[MAX_COMMAND_LENGTH];

    show_welcome();

    while (1)
    {
        show_shell();
        read_command(command);

        if(command[0] == '\0')
          continue;
        
        parse_command(command);
    }
}
