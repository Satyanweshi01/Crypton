#ifndef DATABASE_H
#define DATABASE_H

#include "model.h"

int open_database();
int create_table();
int add_entry(VaultEntry entry);
VaultEntryList get_all_entries();
int close_database();

#endif
