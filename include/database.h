#ifndef DATABASE_H
#define DATABASE_H

#include "model.h"
#include <stdbool.h>

int open_database();

int create_table();

int add_vault_entry(VaultEntry entry);

int update_vault_entry(VaultEntry entry);

int delete_vault_entry(char *service);

VaultEntry get_vault_entry_by_id(int id);

int find_vault_entries_by_service(const char *service, VaultEntry results[], int max_results);

VaultEntryList get_all_vault_entries();

int close_database();

bool db_save_metadata(AppMetadata metadata);

bool db_load_metadata(AppMetadata *metadata);

bool db_update_metadata(AppMetadata metadata);

#endif
