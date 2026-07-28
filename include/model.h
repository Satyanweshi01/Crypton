#ifndef MODEL_H
#define MODEL_H
typedef struct date_data
{
    int year;
    int month;
    int day;

    int hour;
    int minute;
    int second;
} date;

typedef struct
{
    int id;
    char service[100];
    char username[100];
    char password[256];
    char notes[500];
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
    char master_password_hash[128];
    char recovery_key_hash[128];
    date updated_at;
    date created_at;
} AppMetadata;

#endif
