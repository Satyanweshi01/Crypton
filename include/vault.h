#ifndef VAULT_H
#define VAULT_H

#include <stdbool.h>
#include "model.h"

// Adds a new entry to the vault.
// @param entry The VaultEntry to be added.
// @return true if the entry was added successfully, false otherwise.
bool vault_add_entry(VaultEntry entry);

// Updates an existing entry in the vault.
// @param entry The VaultEntry with updated information.
// @return true if the entry was updated successfully, false otherwise.
bool vault_update_entry(VaultEntry entry);

// Deletes an entry from the vault based on the service name.
// @param service The name of the service associated with the entry to be deleted.
// @return true if the entry was deleted successfully, false otherwise.
bool vault_delete_entry(char *service);

// Retrieves an entry from the vault based on its ID.
// @param id The ID of the entry to be retrieved.
// @param entry A pointer to a VaultEntry structure where the retrieved entry will be stored.
// @return true if the entry was retrieved successfully, false otherwise.
bool vault_get_entry(int id, VaultEntry *entry);

// Finds entries in the vault based on the service name.
// @param service The name of the service to search for.
// @param results An array of VaultEntry structures where the found entries will be stored.
// @param max_results The maximum number of results to return.
// @return The number of entries found.
int vault_find_entries(const char *service,
                       VaultEntry results[],
                       int max_results);

// Retrieves all entries from the vault.
// @return A VaultEntryList containing all entries in the vault.
VaultEntryList vault_get_all_entries(void);

// Generates a random password of the specified length.
// @param password A pointer to a character array where the generated password will be stored.
// @param length The desired length of the generated password.
void vault_generate_password(char *password,
                             int length);

#endif
