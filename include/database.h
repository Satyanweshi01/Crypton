#ifndef DATABASE_H
#define DATABASE_H

#include "model.h"

int open_database();

int create_table();

int add_vault_entry(VaultEntry entry);

int update_vault_entry(VaultEntry entry);

int delete_vault_entry(char *service);

VaultEntryList get_all_vault_entries();

int close_database();

#endif
