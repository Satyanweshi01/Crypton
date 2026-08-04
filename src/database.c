#include "../include/model.h"
#include "../include/database.h"
#include "../include/sqlite3.h"
#include "../include/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <sys/stat.h>
#endif

#define MAX_QUERY_LENGTH 1024
#define DATABASE_PATH_SIZE 1024
#define MAX_LOG_LENGTH 2048

// Global Storage
sqlite3 *db;
char *errMSG = 0;
int rc; // return code

static void get_database_path(char *path, size_t path_size)
{
#ifdef _WIN32
    char exe_path[DATABASE_PATH_SIZE];
    char *last_slash;

    if (GetModuleFileNameA(NULL, exe_path, sizeof(exe_path)) == 0)
    {
        snprintf(path, path_size, ".\\data\\vault.db");
        return;
    }

    last_slash = strrchr(exe_path, '\\');
    if (!last_slash)
    {
        snprintf(path, path_size, ".\\data\\vault.db");
        return;
    }

    *last_slash = '\0';
    snprintf(path, path_size, "%s\\data", exe_path);
    _mkdir(path);
    snprintf(path, path_size, "%s\\data\\vault.db", exe_path);
#else
    mkdir("./data", 0755);
    snprintf(path, path_size, "./data/vault.db");
#endif
}

// Callback function for SQLite queries
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    (void)NotUsed;
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// Open the database connection
int open_database()
{
    char database_path[DATABASE_PATH_SIZE];
    char logMSG[MAX_LOG_LENGTH];

    get_database_path(database_path, sizeof(database_path));
    rc = sqlite3_open(database_path, &db);

    if (rc != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "ERROR:  Can't open database: %s", sqlite3_errmsg(db));
        log_msg(logMSG);
        return 0;
    }
    else
    {
        snprintf(logMSG, sizeof(logMSG), "INFO:   Opened database successfully");
        log_msg(logMSG);
    }

    return 1;
}

// Close the database connection
int close_database()
{
    char logMSG[MAX_LOG_LENGTH];

    if (sqlite3_close(db) != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "ERROR:  Can't close database: %s", sqlite3_errmsg(db));
        log_msg(logMSG);
        return 0;
    }
    else
    {
        snprintf(logMSG, sizeof(logMSG), "INFO:   Closed database successfully");
        log_msg(logMSG);
        return 1;
    }
}

// Create vault_entries and metadata tables
int create_table()
{
    char logMSG[MAX_LOG_LENGTH];

    const char *vault_query =
        "CREATE TABLE IF NOT EXISTS vault_entries ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "service TEXT NOT NULL,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "notes TEXT,"
        "updated_at TEXT NOT NULL,"
        "created_at TEXT NOT NULL);";

    const char *metadata_query =
        "CREATE TABLE IF NOT EXISTS metadata ("
        "id INTEGER PRIMARY KEY,"
        "master_password_hash TEXT NOT NULL,"
        "recovery_key_hash TEXT NOT NULL,"
        "updated_year INTEGER NOT NULL,"
        "updated_month INTEGER NOT NULL,"
        "updated_day INTEGER NOT NULL,"
        "updated_hour INTEGER NOT NULL,"
        "updated_minute INTEGER NOT NULL,"
        "updated_second INTEGER NOT NULL,"
        "created_year INTEGER NOT NULL,"
        "created_month INTEGER NOT NULL,"
        "created_day INTEGER NOT NULL,"
        "created_hour INTEGER NOT NULL,"
        "created_minute INTEGER NOT NULL,"
        "created_second INTEGER NOT NULL);";

    rc = sqlite3_exec(db, vault_query, callback, 0, &errMSG);

    if (rc != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "ERROR:  SQL error: %s", errMSG);
        log_msg(logMSG);
        sqlite3_free(errMSG);

        return 0;
    }

    rc = sqlite3_exec(db, metadata_query, callback, 0, &errMSG);

    if (rc != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "ERROR:  SQL error: %s", errMSG);
        log_msg(logMSG);
        sqlite3_free(errMSG);

        return 0;
    }

    snprintf(logMSG, sizeof(logMSG), "INFO:   Tables created successfully");
    log_msg(logMSG);

    return 1;
}

// Add a new entry to vault_entries (Auto-increment ID, full date/time format)
int add_vault_entry(VaultEntry entry)
{
    char query[MAX_QUERY_LENGTH];
    char logMSG[MAX_LOG_LENGTH];

    const char *fmt =
        "INSERT INTO vault_entries "
        "(service, username, password, notes, updated_at, created_at) "
        "VALUES ('%s', '%s', '%s', '%s', '%02d-%02d-%04d %02d:%02d:%02d', '%02d-%02d-%04d %02d:%02d:%02d');";

    snprintf(query, sizeof(query), fmt,
             entry.service,
             entry.username,
             entry.password,
             entry.notes,
             entry.updated_at.day,
             entry.updated_at.month,
             entry.updated_at.year,
             entry.updated_at.hour,
             entry.updated_at.minute,
             entry.updated_at.second,
             entry.created_at.day,
             entry.created_at.month,
             entry.created_at.year,
             entry.created_at.hour,
             entry.created_at.minute,
             entry.created_at.second);

    rc = sqlite3_exec(db, query, callback, 0, &errMSG);

    if (rc != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "ERROR:  SQL error: %s", errMSG);
        log_msg(logMSG);
        sqlite3_free(errMSG);

        return 0;
    }
    else
    {
        snprintf(logMSG, sizeof(logMSG), "INFO:   Entry added successfully");
        log_msg(logMSG);

        return 1;
    }
}

// Update existing entry matching service name
int update_vault_entry(VaultEntry entry)
{
    char query[MAX_QUERY_LENGTH];
    char logMSG[MAX_LOG_LENGTH];

    const char *fmt = "UPDATE vault_entries "
                      "SET service = '%s', username = '%s', password = '%s', notes = '%s', "
                      "updated_at = '%02d-%02d-%04d %02d:%02d:%02d', "
                      "created_at = '%02d-%02d-%04d %02d:%02d:%02d' "
                      "WHERE id = %d;";

    snprintf(query, sizeof(query), fmt,
             entry.service,
             entry.username,
             entry.password,
             entry.notes,
             entry.updated_at.day,
             entry.updated_at.month,
             entry.updated_at.year,
             entry.updated_at.hour,
             entry.updated_at.minute,
             entry.updated_at.second,
             entry.created_at.day,
             entry.created_at.month,
             entry.created_at.year,
             entry.created_at.hour,
             entry.created_at.minute,
             entry.created_at.second,
             entry.id);

    rc = sqlite3_exec(db, query, callback, 0, &errMSG);

    if (rc != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "ERROR:  SQL error: %s", errMSG);
        log_msg(logMSG);
        sqlite3_free(errMSG);

        return 0;
    }
    else
    {
        snprintf(logMSG, sizeof(logMSG), "INFO:   Entry updated successfully");
        log_msg(logMSG);

        return 1;
    }
}

// Delete entry by service name
int delete_vault_entry(char *service)
{
    char query[MAX_QUERY_LENGTH];
    char logMSG[MAX_LOG_LENGTH];

    const char *fmt = "DELETE FROM vault_entries "
                      "WHERE service = '%s';";

    snprintf(query, sizeof(query), fmt, service);

    rc = sqlite3_exec(db, query, callback, 0, &errMSG);

    if (rc != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "ERROR:  SQL error: %s", errMSG);
        log_msg(logMSG);
        sqlite3_free(errMSG);

        return 0;
    }
    else
    {
        snprintf(logMSG, sizeof(logMSG), "INFO:   Entry deleted successfully");
        log_msg(logMSG);

        return 1;
    }
}

// Safe string copy helper for SQLite column results
static void safe_column_text(sqlite3_stmt *stmt, int col, char *dest, size_t dest_size)
{
    const unsigned char *txt = sqlite3_column_text(stmt, col);
    if (txt)
    {
        snprintf(dest, dest_size, "%s", (const char *)txt);
    }
    else
    {
        dest[0] = '\0';
    }
}

// Retrieve all entries from vault_entries table safely
VaultEntryList get_all_vault_entries()
{
    VaultEntryList entryList;
    char logMSG[MAX_LOG_LENGTH];

    entryList.items = NULL;
    entryList.count = 0;

    const char *query = "SELECT id, service, username, password, notes, updated_at, created_at FROM vault_entries;";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "ERROR:  Failed to fetch data: %s", sqlite3_errmsg(db));
        log_msg(logMSG);

        return entryList;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // Safe memory reallocation with temporary pointer
        VaultEntry *temp = realloc(entryList.items, (entryList.count + 1) * sizeof(VaultEntry));
        if (!temp)
        {
            snprintf(logMSG, sizeof(logMSG), "ERROR:  Memory allocation error during vault entry fetch");
            log_msg(logMSG);

            break;
        }
        entryList.items = temp;
        entryList.count++;

        VaultEntry *entry = &entryList.items[entryList.count - 1];
        entry->id = sqlite3_column_int(stmt, 0);

        safe_column_text(stmt, 1, entry->service, sizeof(entry->service));
        safe_column_text(stmt, 2, entry->username, sizeof(entry->username));
        safe_column_text(stmt, 3, entry->password, sizeof(entry->password));
        safe_column_text(stmt, 4, entry->notes, sizeof(entry->notes));

        const unsigned char *updated_str = sqlite3_column_text(stmt, 5);
        if (updated_str)
        {
            sscanf((const char *)updated_str, "%02d-%02d-%04d %02d:%02d:%02d",
                   &entry->updated_at.day, &entry->updated_at.month, &entry->updated_at.year,
                   &entry->updated_at.hour, &entry->updated_at.minute, &entry->updated_at.second);
        }

        const unsigned char *created_str = sqlite3_column_text(stmt, 6);
        if (created_str)
        {
            sscanf((const char *)created_str, "%02d-%02d-%04d %02d:%02d:%02d",
                   &entry->created_at.day, &entry->created_at.month, &entry->created_at.year,
                   &entry->created_at.hour, &entry->created_at.minute, &entry->created_at.second);
        }
    }

    sqlite3_finalize(stmt);
    return entryList;
}

// Retrieve single entry by ID
VaultEntry get_vault_entry_by_id(int id)
{
    VaultEntry result = {0};
    char logMSG[MAX_LOG_LENGTH];

    sqlite3_stmt *stmt;
    const char *query = "SELECT id, service, username, password, notes, updated_at, created_at "
                        "FROM vault_entries WHERE id = ?;";

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "ERROR:  Error during fetching vault entry");
        log_msg(logMSG);

        return result;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        VaultEntry *entry = &result;
        entry->id = sqlite3_column_int(stmt, 0);

        safe_column_text(stmt, 1, entry->service, sizeof(entry->service));
        safe_column_text(stmt, 2, entry->username, sizeof(entry->username));
        safe_column_text(stmt, 3, entry->password, sizeof(entry->password));
        safe_column_text(stmt, 4, entry->notes, sizeof(entry->notes));

        const unsigned char *updated_str = sqlite3_column_text(stmt, 5);
        if (updated_str)
        {
            sscanf((const char *)updated_str, "%02d-%02d-%04d %02d:%02d:%02d",
                   &entry->updated_at.day, &entry->updated_at.month, &entry->updated_at.year,
                   &entry->updated_at.hour, &entry->updated_at.minute, &entry->updated_at.second);
        }

        const unsigned char *created_str = sqlite3_column_text(stmt, 6);
        if (created_str)
        {
            sscanf((const char *)created_str, "%02d-%02d-%04d %02d:%02d:%02d",
                   &entry->created_at.day, &entry->created_at.month, &entry->created_at.year,
                   &entry->created_at.hour, &entry->created_at.minute, &entry->created_at.second);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

// Search entries matching service substring into caller-allocated array
int find_vault_entries_by_service(const char *service, VaultEntry results[], int max_results)
{
    char logMSG[MAX_LOG_LENGTH];

    if (!service || !results || max_results <= 0)
        return 0;

    sqlite3_stmt *stmt;
    const char *query = "SELECT id, service, username, password, notes, updated_at, created_at "
                        "FROM vault_entries WHERE service LIKE ?;";

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "ERROR:  Error during fetching vault entry");
        log_msg(logMSG);

        return 0;
    }

    char search_pattern[MAX_QUERY_LENGTH];
    snprintf(search_pattern, sizeof(search_pattern), "%%%s%%", service);
    sqlite3_bind_text(stmt, 1, search_pattern, -1, SQLITE_TRANSIENT);

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max_results)
    {
        VaultEntry *entry = &results[count];
        entry->id = sqlite3_column_int(stmt, 0);

        safe_column_text(stmt, 1, entry->service, sizeof(entry->service));
        safe_column_text(stmt, 2, entry->username, sizeof(entry->username));
        safe_column_text(stmt, 3, entry->password, sizeof(entry->password));
        safe_column_text(stmt, 4, entry->notes, sizeof(entry->notes));

        const unsigned char *updated_str = sqlite3_column_text(stmt, 5);
        if (updated_str)
        {
            sscanf((const char *)updated_str, "%02d-%02d-%04d %02d:%02d:%02d",
                   &entry->updated_at.day, &entry->updated_at.month, &entry->updated_at.year,
                   &entry->updated_at.hour, &entry->updated_at.minute, &entry->updated_at.second);
        }

        const unsigned char *created_str = sqlite3_column_text(stmt, 6);
        if (created_str)
        {
            sscanf((const char *)created_str, "%02d-%02d-%04d %02d:%02d:%02d",
                   &entry->created_at.day, &entry->created_at.month, &entry->created_at.year,
                   &entry->created_at.hour, &entry->created_at.minute, &entry->created_at.second);
        }

        count++;
    }

    sqlite3_finalize(stmt);
    return count;
}

// Insert metadata with discrete integer date/time fields
bool db_save_metadata(AppMetadata metadata)
{
    sqlite3_stmt *stmt;
    char logMSG[MAX_LOG_LENGTH];

    const char *query =
        "INSERT OR REPLACE INTO metadata "
        "(id, master_password_hash, recovery_key_hash, "
        "updated_year, updated_month, updated_day, updated_hour, updated_minute, updated_second, "
        "created_year, created_month, created_day, created_hour, created_minute, created_second) "
        "VALUES (1, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        snprintf(logMSG, sizeof(logMSG), "INFO:   Saved metadata successfully");
        log_msg(logMSG);

        return false;
    }

    sqlite3_bind_text(stmt, 1, metadata.master_password_hash, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, metadata.recovery_key_hash, -1, SQLITE_STATIC);

    sqlite3_bind_int(stmt, 3, metadata.updated_at.year);
    sqlite3_bind_int(stmt, 4, metadata.updated_at.month);
    sqlite3_bind_int(stmt, 5, metadata.updated_at.day);
    sqlite3_bind_int(stmt, 6, metadata.updated_at.hour);
    sqlite3_bind_int(stmt, 7, metadata.updated_at.minute);
    sqlite3_bind_int(stmt, 8, metadata.updated_at.second);

    sqlite3_bind_int(stmt, 9, metadata.created_at.year);
    sqlite3_bind_int(stmt, 10, metadata.created_at.month);
    sqlite3_bind_int(stmt, 11, metadata.created_at.day);
    sqlite3_bind_int(stmt, 12, metadata.created_at.hour);
    sqlite3_bind_int(stmt, 13, metadata.created_at.minute);
    sqlite3_bind_int(stmt, 14, metadata.created_at.second);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

// Load metadata into caller-provided struct
bool db_load_metadata(AppMetadata *metadata)
{
    if (!metadata)
        return false;

    sqlite3_stmt *stmt;

    const char *query =
        "SELECT master_password_hash, recovery_key_hash, "
        "updated_year, updated_month, updated_day, updated_hour, updated_minute, updated_second, "
        "created_year, created_month, created_day, created_hour, created_minute, created_second "
        "FROM metadata WHERE id = 1;";

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
        return false;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        safe_column_text(stmt, 0, metadata->master_password_hash, sizeof(metadata->master_password_hash));
        safe_column_text(stmt, 1, metadata->recovery_key_hash, sizeof(metadata->recovery_key_hash));

        metadata->updated_at.year = sqlite3_column_int(stmt, 2);
        metadata->updated_at.month = sqlite3_column_int(stmt, 3);
        metadata->updated_at.day = sqlite3_column_int(stmt, 4);
        metadata->updated_at.hour = sqlite3_column_int(stmt, 5);
        metadata->updated_at.minute = sqlite3_column_int(stmt, 6);
        metadata->updated_at.second = sqlite3_column_int(stmt, 7);

        metadata->created_at.year = sqlite3_column_int(stmt, 8);
        metadata->created_at.month = sqlite3_column_int(stmt, 9);
        metadata->created_at.day = sqlite3_column_int(stmt, 10);
        metadata->created_at.hour = sqlite3_column_int(stmt, 11);
        metadata->created_at.minute = sqlite3_column_int(stmt, 12);
        metadata->created_at.second = sqlite3_column_int(stmt, 13);

        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
}

// Update existing metadata row
bool db_update_metadata(AppMetadata metadata)
{
    sqlite3_stmt *stmt;

    const char *query =
        "UPDATE metadata SET "
        "master_password_hash = ?, "
        "recovery_key_hash = ?, "
        "updated_year = ?, "
        "updated_month = ?, "
        "updated_day = ?, "
        "updated_hour = ?, "
        "updated_minute = ?, "
        "updated_second = ?, "
        "created_year = ?, "
        "created_month = ?, "
        "created_day = ?, "
        "created_hour = ?, "
        "created_minute = ?, "
        "created_second = ? "
        "WHERE id = 1;";

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, metadata.master_password_hash, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, metadata.recovery_key_hash, -1, SQLITE_STATIC);

    sqlite3_bind_int(stmt, 3, metadata.updated_at.year);
    sqlite3_bind_int(stmt, 4, metadata.updated_at.month);
    sqlite3_bind_int(stmt, 5, metadata.updated_at.day);
    sqlite3_bind_int(stmt, 6, metadata.updated_at.hour);
    sqlite3_bind_int(stmt, 7, metadata.updated_at.minute);
    sqlite3_bind_int(stmt, 8, metadata.updated_at.second);

    sqlite3_bind_int(stmt, 9, metadata.created_at.year);
    sqlite3_bind_int(stmt, 10, metadata.created_at.month);
    sqlite3_bind_int(stmt, 11, metadata.created_at.day);
    sqlite3_bind_int(stmt, 12, metadata.created_at.hour);
    sqlite3_bind_int(stmt, 13, metadata.created_at.minute);
    sqlite3_bind_int(stmt, 14, metadata.created_at.second);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}
