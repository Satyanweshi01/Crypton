#include "../include/model.h"
#include "../include/database.h"
#include "../include/sqlite3.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUERY_LENGTH 1024

// Global Storage

sqlite3 *db;
char *errMSG = 0;
int rc; // return code

// Definitions

// Callback function for SQLite queries
// This function is called for each row returned by a query. It prints the column names and values to the console.
// Returns 0 to indicate success.
// Note: This function is used as a callback for sqlite3_exec() and is called for each row returned by the query.
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// Open the database connection
// Returns 1 on success, 0 on failure
int open_database()
{
    printf("SQLite version: %s\n", sqlite3_libversion());

    rc = sqlite3_open("./data/vault.db", &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    return 1;
}

// Close the database connection
// Returns 1 on success, 0 on failure
int close_database()
{
    if (sqlite3_close(db) != SQLITE_OK)
    {
        fprintf(stderr, "Can't close database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        fprintf(stderr, "Closed database successfully\n");
        return 1;
    }
}

// Create the vault_entries table if it doesn't exist
// Returns 1 on success, 0 on failure
int create_table()
{
    const char *query = "CREATE TABLE IF NOT EXISTS vault_entries ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "service TEXT NOT NULL,"
                        "username TEXT NOT NULL,"
                        "password TEXT NOT NULL,"
                        "notes TEXT,"
                        "updated_at TEXT NOT NULL,"
                        "created_at TEXT NOT NULL);";

    rc = sqlite3_exec(db, query, callback, 0, &errMSG);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMSG);
        sqlite3_free(errMSG);
        return 0;
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
        return 1;
    }
}

// Add a new entry to the vault_entries table
// Parameters:
// - entry: A VaultEntry struct containing the data to be inserted into the table.
// Returns 1 on success, 0 on failure
int add_vault_entry(VaultEntry entry)
{
    // Format the query string with the values from the VaultEntry struct
    char query[MAX_QUERY_LENGTH];

    const char *fmt =
        "INSERT INTO vault_entries "
        "(id, service, username, password, notes, updated_at, created_at) "
        "VALUES (%d, '%s', '%s', '%s', '%s', '%02d-%02d-%04d', '%02d-%02d-%04d');";

    snprintf(query, sizeof(query), fmt,
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

    rc = sqlite3_exec(db, query, callback, 0, &errMSG);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMSG);
        sqlite3_free(errMSG);
        return 0;
    }
    else
    {
        fprintf(stdout, "Entry added successfully\n");
        return 1;
    }
}

int update_vault_entry(VaultEntry entry)
{
    char query[MAX_QUERY_LENGTH];

    const char *fmt = "UPDATE vault_entries"
                      "SET service = '%s', username = '%s', password = '%s', notes = '%s', updated_at = '%02d-%02d-%04d', created_at = '%02d-%02d-%04d'"
                      "WHERE service = '%s';";

    snprintf(query, sizeof(query), fmt,
             entry.service,
             entry.username,
             entry.password,
             entry.notes,
             entry.updated_at.day,
             entry.updated_at.month,
             entry.updated_at.year,
             entry.created_at.day,
             entry.created_at.month,
             entry.created_at.year,
             entry.service);

    rc = sqlite3_exec(db, query, callback, 0, &errMSG);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMSG);
        sqlite3_free(errMSG);
        return 0;
    }
    else
    {
        fprintf(stdout, "Entry updated successfully\n");
        return 1;
    }
}

int delete_vault_entry(char *service)
{
    char query[MAX_QUERY_LENGTH];

    const char *fmt = "DELETE FROM vault_entries"
                      "WHERE service = '%s';";

    snprintf(query, sizeof(query), fmt, service);

    rc = sqlite3_exec(db, query, callback, 0, &errMSG);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMSG);
        sqlite3_free(errMSG);
        return 0;
    }
    else
    {
        fprintf(stdout, "Entry deleted successfully\n");
        return 1;
    }
}

// Retrieve all entries from the vault_entries table
// Returns a VaultEntryList struct containing an array of VaultEntry structs and the count of entries retrieved.
// Returns NULL if table is empty
VaultEntryList get_all_vault_entries()
{
    VaultEntryList entryList;
    entryList.items = NULL;
    entryList.count = 0;

    const char *query = "SELECT * FROM vault_entries;";

    // Prepare the SQL statement
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return entryList;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        entryList.count++;

        // Reallocate memory for the new entry
        entryList.items = realloc(entryList.items, entryList.count * sizeof(VaultEntry));

        // Get a pointer to the newly added entry
        VaultEntry *entry = &entryList.items[entryList.count - 1];
        entry->id = sqlite3_column_int(stmt, 0);

        // Copy the string values from the database to the VaultEntry
        snprintf(entry->service, sizeof(entry->service), "%s", sqlite3_column_text(stmt, 1));
        snprintf(entry->username, sizeof(entry->username), "%s", sqlite3_column_text(stmt, 2));
        snprintf(entry->password, sizeof(entry->password), "%s", sqlite3_column_text(stmt, 3));
        snprintf(entry->notes, sizeof(entry->notes), "%s", sqlite3_column_text(stmt, 4));

        sscanf((const char *)sqlite3_column_text(stmt, 5), "%02d-%02d-%04d", &entry->updated_at.day, &entry->updated_at.month, &entry->updated_at.year);
        sscanf((const char *)sqlite3_column_text(stmt, 6), "%02d-%02d-%04d", &entry->created_at.day, &entry->created_at.month, &entry->created_at.year);
    }

    sqlite3_finalize(stmt);

    return entryList;
}
