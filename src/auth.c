#include "../include/auth.h"
#include "../include/crypto.h"
#include "../include/model.h"
#include "../include/repository.h"
#include "../include/cli.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define AUTH_INPUT_SIZE 128

static void read_auth_input(const char *prompt, char *buffer, size_t size)
{
    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) == NULL)
    {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
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

bool vault_exists(void)
{
    AppMetadata metadata = {0};
    return load_metadata(&metadata);
}

void setup_vault(void)
{
    char password[AUTH_INPUT_SIZE];
    char confirm_password[AUTH_INPUT_SIZE];
    char recovery_key[AUTH_INPUT_SIZE];
    AppMetadata metadata = {0};

    printf("No vault setup found. Create your master password.\n");

    while (1)
    {
        read_auth_input("New master password: ", password, sizeof(password));
        secured_read("Confirm master password: ", confirm_password, sizeof(confirm_password));

        if (password[0] == '\0')
        {
            printf("Master password cannot be empty.\n");
            continue;
        }

        if (strcmp(password, confirm_password) != 0)
        {
            printf("Passwords do not match. Try again.\n");
            continue;
        }

        break;
    }

    while (1)
    {
        read_auth_input("Recovery key: ", recovery_key, sizeof(recovery_key));

        if (recovery_key[0] != '\0')
            break;

        printf("Recovery key cannot be empty.\n");
    }

    hash_password(password, metadata.master_password_hash);
    hash_password(recovery_key, metadata.recovery_key_hash);
    metadata.created_at = current_date_time();
    metadata.updated_at = metadata.created_at;

    cli_clear();

    if (save_metadata(metadata))
        printf("Vault setup complete.\n");
    else
        printf("Vault setup failed.\n");
}

bool login(void)
{
    char password[AUTH_INPUT_SIZE];

    for (int attempt = 1; attempt <= 3; attempt++)
    {
        secured_read("Master password: ", password, sizeof(password));

        cli_clear();

        if (verify_master_password(password))
        {
            printf("Login successful.\n");
            return true;
        }

        printf("Invalid master password. Attempts left: %d\n", 3 - attempt);
    }

    return false;
}

bool verify_master_password(const char *password)
{
    AppMetadata metadata = {0};

    if (!password || !load_metadata(&metadata))
        return false;

    return verify_hash(password, metadata.master_password_hash);
}

bool verify_recovery_key(const char *key)
{
    AppMetadata metadata = {0};

    if (!key || !load_metadata(&metadata))
        return false;

    return verify_hash(key, metadata.recovery_key_hash);
}
