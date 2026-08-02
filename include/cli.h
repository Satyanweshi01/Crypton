#ifndef CLI_H
#define CLI_H

void start_cli(void);
void show_welcome(void);
void show_shell(void);
void read_command(char *buffer);
/* Command Handlers */
void cli_add(void);
void cli_list(void);
void cli_get(void);
void cli_find(void);
void cli_update(void);
void cli_delete(void);
void cli_generate(void);

#endif
