#include <string.h>
#include "vault.h"
#include "repository.h"
#include "crypto.h"

#define VAULT_KEY "crypton"

bool vault_add_entry(VaultEntry entry)
{
    if(strlen(entry.service) == 0)
    return false;
    
    if(strlen(entry.username) == 0)
    return false;

    encrypt(entry.password, VAULT_KEY);
    return save_entry(entry);
}

bool vault_update_entry(VaultEntry entry)
{
    if(entry.id <= 0)
    return false;

    encrypt(entry.password, VAULT_KEY);
    return update_entry(entry);
}

bool vault_delete_entry(char *service)
{
    if(service == NULL)
    return false;

    return delete_entry(service);

}

bool vault_get_entry(int id, VaultEntry *entry)
{
    if(entry == NULL)
        return false;

    *entry = get_entry_by_id(id, entry);

    if(entry->id <= 0)
        return false;

    decrypt(entry->password, VAULT_KEY);

    return true;
}


int vault_find_entries(const char *service,
                       VaultEntry results[],
                       int max_results)
{
                           int count = find_entries_by_service(service,
                                        results,
                                        max_results);

    for(int i = 0; i < count; i++)
        decrypt(results[i].password,
                VAULT_KEY);

    return count;
}

VaultEntryList vault_get_all_entries(void)

{
    VaultEntryList list = get_all_entries();

    for(int i = 0; i < list.count; i++)
        decrypt(list.items[i].password,
                VAULT_KEY);

    return list;
}

void vault_generate_password(char *password,
                             int length)

{
    generate_random_password(password,
                             length);
}
