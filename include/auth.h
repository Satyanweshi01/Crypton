#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

bool vault_exists(void);

void setup_vault(void);

bool login(void);

bool verify_master_password(const char *password);

bool verify_recovery_key(const char *key);

#endif
