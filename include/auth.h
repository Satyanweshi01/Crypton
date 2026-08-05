#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

// Checks if a vault exists by attempting to load the application metadata.
// @return true if the vault exists, false otherwise.
bool vault_exists(void);

// Sets up a new vault by prompting the user to create a master password and a recovery key.
void setup_vault(void);

// Handles the login process, allowing the user up to 3 attempts to enter the correct master password.
// @return true if the login is successful, false otherwise.
bool login(void);

// Verifies the provided master password against the stored hash in the application metadata.
// @param password The master password to verify.
// @return true if the password is correct, false otherwise.
bool verify_master_password(const char *password);

// Verifies the provided recovery key against the stored hash in the application metadata.
// @param key The recovery key to verify.
// @return true if the key is correct, false otherwise.
bool verify_recovery_key(const char *key);

#endif
