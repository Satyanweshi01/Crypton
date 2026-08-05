#ifndef CLI_H
#define CLI_H

#include <stddef.h>

// Starts the command-line interface for the application.
// This function initializes the CLI, checks for vault existence, handles user login,
// and enters a loop to read and process user commands.
void start_cli(void);

// Displays the welcome message for the CLI.
void show_welcome(void);

// Displays the command prompt for the CLI.
void show_shell(void);

// Reads a command from the user input.
// @param buffer The buffer to store the command.
void read_command(char *buffer);

// Reads user input securely without echoing the characters to the console.
// @param prompt The message to display to the user.
// @param buffer The buffer to store the user input.
// @param size The size of the buffer.
void secured_read(const char *prompt, char *buffer, size_t size);

/* Command Handlers */

// Adds a new entry to the vault.
void cli_add(void);

// Lists all entries in the vault.
void cli_list(void);

// Retrieves a specific entry from the vault based on its ID.
void cli_get(void);

// Searches for entries in the vault based on a service name.
void cli_find(void);

// Updates an existing entry in the vault.
void cli_update(void);

// Deletes an entry from the vault based on the service name.
void cli_delete(void);

// Generates a random password of a specified length.
void cli_generate(void);

// Clears the console screen based on the OS.
void cli_clear(void);

#endif
