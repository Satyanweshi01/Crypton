#ifndef REPOSITORY_H
#define REPOSITORY_H

// Acts as the data access layer between the application and the database.
// Performs CRUD (Create, Read, Update, Delete) operations.
// Converts database records into application models and vice versa.
// Hides SQL implementation details from the business logic.

#include <stdbool.h>
#include "model.h"

// Saves a new vault entry to the database.
// @param entry The vault entry to save.
// @return true if the entry was saved successfully, false otherwise.
bool save_entry(VaultEntry entry);

// Updates an existing vault entry in the database.
// @param entry The vault entry to update.
// @return true if the entry was updated successfully, false otherwise.
bool update_entry(VaultEntry entry);

// Deletes a vault entry from the database by service name.
// @param service The service name of the vault entry to delete.
// @return true if the entry was deleted successfully, false otherwise.
bool delete_entry(char *service);

// Retrieves a vault entry by its ID from the database.
// @param id The ID of the vault entry to retrieve.
// @return The vault entry with the specified ID, or an empty entry if not found.
VaultEntry get_entry_by_id(int id);

// Finds vault entries by service name.
// @param service The service name to search for.
// @param results An array to store the found vault entries.
// @param max_results The maximum number of results to return.
// @return The number of entries found and stored in the results array.
int find_entries_by_service(const char *service, VaultEntry results[], int max_results);

// Retrieves all vault entries from the database.
// @return A VaultEntryList containing all vault entries.
VaultEntryList get_all_entries(void);

// Saves application metadata to the database.
// @param metadata The application metadata to save.
// @return true if the metadata was saved successfully, false otherwise.
bool save_metadata(AppMetadata metadata);

// Loads application metadata from the database.
// @param metadata A pointer to an AppMetadata structure to store the loaded metadata.
// @return true if the metadata was loaded successfully, false otherwise.
bool load_metadata(AppMetadata *metadata);

// Updates application metadata in the database.
// @param metadata The application metadata to update.
// @return true if the metadata was updated successfully, false otherwise.
bool update_metadata(AppMetadata metadata);

#endif
