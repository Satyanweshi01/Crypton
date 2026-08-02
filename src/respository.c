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

    if (!add_vault_entry(entry))
    {
        close_database();
        return false;
    }

    if (!close_database())
        return false;

    return true;
}

bool update_entry(VaultEntry entry)
{
    if (!open_database())
        return false;

    if (!update_vault_entry(entry))
    {
        close_database();
        return false;
    }

    if (!close_database())
        return false;

    return true;
}

bool delete_entry(char *service)
{
    if (!open_database())
        return false;

    if (!delete_vault_entry(service))
    {
        close_database();
        return false;
    }

    if (!close_database())
        return false;

    return true;
}

VaultEntry get_entry_by_id(int id)
{
    VaultEntry entry = {0};

    if (!open_database())
        return entry;

    entry = get_vault_entry_by_id(id);
    close_database();

    return entry;
}

int find_entries_by_service(const char *service, VaultEntry results[], int max_results)
{
    int count = 0;

    if (!open_database())
        return 0;

    count = find_vault_entries_by_service(service, results, max_results);
    close_database();

    return count;
}

VaultEntryList get_all_entries()
{
    VaultEntryList list = {0};

    if (!open_database())
        return list;

    list = get_all_vault_entries();
    close_database();

    return list;
}

bool save_metadata(AppMetadata metadata)
{
    bool saved;

    if (!open_database())
        return false;

    saved = db_save_metadata(metadata);
    close_database();

    return saved;
}

bool load_metadata(AppMetadata *metadata)
{
    bool loaded;

    if (!open_database())
        return false;

    loaded = db_load_metadata(metadata);
    close_database();

    return loaded;
}

bool update_metadata(AppMetadata metadata)
{
    bool updated;

    if (!open_database())
        return false;

    updated = db_update_metadata(metadata);
    close_database();

    return updated;
}
