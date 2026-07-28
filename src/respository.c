#include <stdbool.h>
#include <stdio.h>
#include "../include/sqlite3.h"
#include "../include/repository.h"
#include "../include/model.h"
#include "../include/database.h"

bool save_entry(VaultEntry entry)
{
    if (!open_database())
        return false;

    if (!add_entry(entry))
        return false;

    if (!close_database())
        return false;

    return true;
}

bool update_entry(VaultEntry entry)
{
    if (!open_database())
        return false;

    if (!update_entry(entry))
        return false;

    if (!close_database())
        return false;

    return true;
}

bool delete_entry(char *service)
{
    if (!open_database())
        return false;

    if (!delete_vault_entry(service))
        return false;

    if (!close_database())
        return false;

    return true;
}

VaultEntryList get_all_entries()
{
    return get_all_vault_entries();
}
