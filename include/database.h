#ifndef DATABASE_H
#define DATABASE_H

#include "model.h"
#include <stdbool.h>

// Open the database connection, creating the database file if it doesn't exist
int open_database();

// Create the necessary tables in the database if they don't exist
int create_table();

// Add a new entry to the vault_entries table
// @param entry: The VaultEntry struct containing the data to be added
// @return: 1 on success, 0 on failure
int add_vault_entry(VaultEntry entry);

// Update an existing entry in the vault_entries table
// @param entry: The VaultEntry struct containing the updated data
// @return: 1 on success, 0 on failure
int update_vault_entry(VaultEntry entry);

// Delete an entry from the vault_entries table based on the service name
// @param service: The service name of the entry to be deleted
// @return: 1 on success, 0 on failure
int delete_vault_entry(char *service);

// Retrieve a single vault entry by its ID
// @param id: The ID of the vault entry to retrieve
// @return: VaultEntry struct containing the entry data, or an empty struct if not found
VaultEntry get_vault_entry_by_id(int id);

// Retrieve vault entries by service name (partial match)
// @param service: The service name to search for
// @param results: Array to store the matching VaultEntry results
// @param max_results: Maximum number of results to retrieve
// @return: Number of matching entries found
int find_vault_entries_by_service(const char *service, VaultEntry results[], int max_results);

// Retrieve all entries from vault_entries table safely
// @return: VaultEntryList containing all entries
VaultEntryList get_all_vault_entries();

// Close the database connection and free resources
int close_database();

// Save metadata into the database
// @param metadata: The AppMetadata struct containing the data to be saved
// @return: true on success, false on failure
bool db_save_metadata(AppMetadata metadata);

// Load metadata from the database
// @param metadata: Pointer to an AppMetadata struct to store the loaded data
// @return: true on success, false on failure
bool db_load_metadata(AppMetadata *metadata);

// Update metadata in the database
// @param metadata: The AppMetadata struct containing the updated data
// @return: true on success, false on failure
bool db_update_metadata(AppMetadata metadata);

#endif
