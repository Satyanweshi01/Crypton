#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/auth.h"
#include "../include/cli.h"
#include "../include/parser.h"
#include "../include/vault.h"
#define MAX_COMMAND_LENGTH 100
#define MAX_RESULTS 50

// Detect OS
#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS 1
#include <conio.h>
#else
#define PLATFORM_WINDOWS 0
#endif

static void read_field(const char *prompt, char *buffer, size_t size)
{
    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) == NULL)
    {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

void secured_read(const char *prompt, char *buffer, size_t size)
{
    printf("%s", prompt);
    fflush(stdout);

    // Enter Raw/No-Echo Mode if Unix
    if (!PLATFORM_WINDOWS)
    {
        system("stty -icanon -echo");
    }

    size_t i = 0;
    int ch;

    while (i < size - 1)
    {
        if (PLATFORM_WINDOWS)
        {
            ch = _getch(); // Windows specific raw char fetch
        }
        else
        {
            ch = getchar(); // Unix raw char fetch
        }

        // (Windows -> \r | Unix -> \n)
        if (ch == '\n' || ch == '\r' || ch == EOF)
        {
            break;
        }

        // Handle Backspace
        if (ch == 127 || ch == '\b')
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }

        buffer[i++] = (char)ch;
        printf("#");
        fflush(stdout);
    }

    buffer[i] = '\0';

    // Restore terminal mode
    if (!PLATFORM_WINDOWS)
    {
        system("stty sane");
    }

    printf("\n");
}

static int read_int_field(const char *prompt)
{
    char buffer[32];
    read_field(prompt, buffer, sizeof(buffer));
    return atoi(buffer);
}

static date current_date_time(void)
{
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    date result = {0};

    if (!local)
        return result;

    result.year = local->tm_year + 1900;
    result.month = local->tm_mon + 1;
    result.day = local->tm_mday;
    result.hour = local->tm_hour;
    result.minute = local->tm_min;
    result.second = local->tm_sec;

    return result;
}

static void print_entry(const VaultEntry *entry)
{
    printf("ID: %d\n", entry->id);
    printf("Service: %s\n", entry->service);
    printf("Username: %s\n", entry->username);
    printf("Password: %s\n", entry->password);
    printf("Notes: %s\n", entry->notes);
    printf("Updated: %02d-%02d-%04d %02d:%02d:%02d\n",
           entry->updated_at.day,
           entry->updated_at.month,
           entry->updated_at.year,
           entry->updated_at.hour,
           entry->updated_at.minute,
           entry->updated_at.second);
}

static bool copy_to_clipboard(const char *text)
{
#ifdef _WIN32
    FILE *clip = _popen("clip", "w");

    if (!clip)
        return false;

    fputs(text, clip);
    return _pclose(clip) == 0;
#else
    (void)text;
    return false;
#endif
}

void show_welcome(void)
{
    printf("+----------------------------------------+\n");
    printf("|                 CRYPTON                |\n");
    printf("+----------------------------------------+\n");
    printf("     Secure Password Management System\n\n");
}

void show_shell(void)
{
    printf("\nCrypton> ");
}

void read_command(char *buffer)
{
    if (fgets(buffer, MAX_COMMAND_LENGTH, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}
void start_cli(void)
{
    char command[MAX_COMMAND_LENGTH];

    show_welcome();

    if (!vault_exists())
        setup_vault();

    if (!login())
    {
        printf("Authentication failed. Exiting Crypton...\n");
        return;
    }

    while (1)
    {
        show_shell();
        read_command(command);

        if (command[0] == '\0')
            continue;

        parse_command(command);
    }
}

void cli_add(void)
{
    VaultEntry entry = {0};

    read_field("Service: ", entry.service, sizeof(entry.service));
    read_field("Username: ", entry.username, sizeof(entry.username));
    read_field("Password: ", entry.password, sizeof(entry.password));
    read_field("Notes: ", entry.notes, sizeof(entry.notes));

    entry.created_at = current_date_time();
    entry.updated_at = entry.created_at;

    if (vault_add_entry(entry))
        printf("Entry saved.\n");
    else
        printf("Could not save entry.\n");
}

void cli_list(void)
{
    VaultEntryList list = vault_get_all_entries();

    if (list.count == 0)
    {
        printf("No entries found.\n");
        free(list.items);
        return;
    }

    for (int i = 0; i < list.count; i++)
    {
        printf("\n");
        print_entry(&list.items[i]);
    }

    free(list.items);
}

void cli_get(void)
{
    int id = read_int_field("Entry ID: ");
    VaultEntry entry = {0};

    if (vault_get_entry(id, &entry))
    {
        print_entry(&entry);
        if (copy_to_clipboard(entry.password))
            printf("Password copied to clipboard.\n");
    }
    else
        printf("Entry not found.\n");
}

void cli_find(void)
{
    char service[100];
    VaultEntry results[MAX_RESULTS];
    int count;

    read_field("Service search: ", service, sizeof(service));
    count = vault_find_entries(service, results, MAX_RESULTS);

    if (count == 0)
    {
        printf("No matching entries found.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        printf("\n");
        print_entry(&results[i]);
    }
}

void cli_update(void)
{
    VaultEntry entry = {0};

    entry.id = read_int_field("Entry ID: ");
    read_field("Service: ", entry.service, sizeof(entry.service));
    read_field("Username: ", entry.username, sizeof(entry.username));
    read_field("Password: ", entry.password, sizeof(entry.password));
    read_field("Notes: ", entry.notes, sizeof(entry.notes));

    entry.created_at = current_date_time();
    entry.updated_at = entry.created_at;

    if (vault_update_entry(entry))
        printf("Entry updated.\n");
    else
        printf("Could not update entry.\n");
}

void cli_delete(void)
{
    char service[100];
    read_field("Service to delete: ", service, sizeof(service));

    if (vault_delete_entry(service))
        printf("Entry deleted.\n");
    else
        printf("Could not delete entry.\n");
}

void cli_generate(void)
{
    int length = read_int_field("Password length: ");
    char password[128];

    if (length <= 0 || length >= (int)sizeof(password))
    {
        printf("Invalid password length.\n");
        return;
    }

    vault_generate_password(password, length);
    printf("Generated password: %s\n", password);
}

void cli_clear(void)
{
    if (PLATFORM_WINDOWS)
    {
        system("cls");
    }
    else
    {
        system("clear");
    }
}
