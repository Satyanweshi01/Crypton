#include "../include/database.h"
#include <stdio.h>

int main()
{
    if (!open_database())
        return 1;
    if (!create_table())
        return 1;

    VaultEntry test = {1, "GitHub", "user", "pass", "notes", {2026, 7, 27, 0, 0, 0}, {2026, 7, 27, 0, 0, 0}};
    add_vault_entry(test);
    VaultEntry test1 = {2, "Instagram", "user", "suor", "another notes", {2026, 7, 20, 0, 0, 0}, {2026, 7, 20, 0, 0, 0}};
    add_vault_entry(test1);

    VaultEntryList list = get_all_vault_entries();

    for (int i = 0; i < list.count; i++)
    {
        VaultEntry entry = list.items[i];
        printf("ID: %d, Service: %s, Username: %s, Password: %s, Notes: %s, Updated At: %02d-%02d-%04d, Created At: %02d-%02d-%04d\n",
               entry.id,
               entry.service,
               entry.username,
               entry.password,
               entry.notes,
               entry.updated_at.day,
               entry.updated_at.month,
               entry.updated_at.year,
               entry.created_at.day,
               entry.created_at.month,
               entry.created_at.year);
    }

    printf("Retrieved %d entries\n", list.count);

    close_database();

    return 0;
}
