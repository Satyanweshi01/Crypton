#include <stdio.h>
#include <string.h>
#include "cli.h"
void show_welcome(void)
{
    printf("******************************************\n");
    printf("      PASSWORD MANAGER (CLI VERSION)\n");
    printf("******************************************\n");
    printf("Secure Password Management System\n\n");
}

void show_shell(void)
{
    printf("pm> ");//PasswordManager>
}

void read_command(char *buffer)
{
    fgets(buffer, 100, stdin);
    // Removal of newline if present 
    int i = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            break;
        }
        i++;
    }
}
void start_cli(void)
{
    char command[100];

    show_welcome();

    while (1)
    {
        show_shell();
        read_command(command);

        /* Command processing will be handled elsewhere */
        printf("You entered: %s\n", command);

        if (strcmp(command, "exit") == 0)
            break;
    }

    printf("Goodbye!\n");
}
