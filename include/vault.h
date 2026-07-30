#ifndef VAULT_H
#define VAULT_H

#include <stdbool.h>
#include "model.h"

bool vault_add_entry(VaultEntry entry);

bool vault_update_entry(VaultEntry entry);

bool vault_delete_entry(char *service);

bool vault_get_entry(int id, VaultEntry *entry);

int vault_find_entries(const char *service,
                       VaultEntry results[],
                       int max_results);

VaultEntryList vault_get_all_entries(void);

void vault_generate_password(char *password,
                             int length);

#endif

