#include <stdbool.h>
#include <stdio.h>
#include "../include/repository.h"


bool save_entry(VaultEntry *entry)
{
    open_database();
    char *query = "INSERT INTO vault_entries ("
                        "service, username, password,"
                        "notes, updated_at, created_at)"
                        "VALUES('entry->service','entry->username'," 
                        "'entry->password', 'entry->notes'," 
                        "'entry->updated_at', 'entry->created_at');"
                        ;
    sqlite3_stmt *obj = sqlite3_prepare_v2(db,query,)

}
