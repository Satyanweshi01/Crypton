
// Acts as the data access layer between the application and the database.
// Performs CRUD (Create, Read, Update, Delete) operations.
// Converts database records into application models and vice versa.
// Hides SQL implementation details from the business logic.

#include <stdbool.h>
#include "model.h"

bool save_entry(VaultEntry entry);

bool update_entry(VaultEntry entry);

bool delete_entry(char *service);

VaultEntry get_entry_by_id(int id , VaultEntry *entry);

int find_entries_by_service(const char *service, VaultEntry results[], int max_results);

VaultEntryList get_all_entries(void);