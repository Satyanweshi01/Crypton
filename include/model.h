#ifndef MODEL_H
#define MODEL_H
typedef struct date_data
{
    int day;
    int month;
    int year;
} date;

typedef struct
{
    int id;
    char service[50];
    char username[50];
    char password[50];
    char notes[200];
    date updated_at;
    date created_at;
} VaultEntry;

typedef struct
{
    VaultEntry *items;
    int count;
} VaultEntryList;

typedef struct
{
    char master_password_hash[50];
    date updated_at;
    date created_at;
} AppMetadata;

#endif
