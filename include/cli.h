#ifndef CLI_H
#define CLI_H

#include <stddef.h>

void start_cli(void);
void show_welcome(void);
void show_shell(void);
void read_command(char *buffer);
void secured_read(const char *prompt, char *buffer, size_t size);
/* Command Handlers */
void cli_add(void);
void cli_list(void);
void cli_get(void);
void cli_find(void);
void cli_update(void);
void cli_delete(void);
void cli_generate(void);
void cli_clear(void);

#endif
