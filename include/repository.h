
//Acts as the data access layer between the application and the database.
//Performs CRUD (Create, Read, Update, Delete) operations.
//Converts database records into application models and vice versa.
//Hides SQL implementation details from the business logic.


bool save_entry(VaultEntry *entry);

bool update_entry(VaultEntry *entry);

bool delete_entry(int id);

VaultEntry get_entry_by_id(int id);

int find_entries_by_site(const char *site, VaultEntry results[]);

int get_all_entries(VaultEntry results[]);