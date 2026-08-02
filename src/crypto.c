#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define ENCRYPTION_PREFIX "ENC:"

static char to_hex(unsigned char value)
{
    return (char)(value < 10 ? '0' + value : 'A' + (value - 10));
}

static int from_hex(char value)
{
    if (value >= '0' && value <= '9')
        return value - '0';
    if (value >= 'A' && value <= 'F')
        return value - 'A' + 10;
    if (value >= 'a' && value <= 'f')
        return value - 'a' + 10;
    return -1;
}
//  Hash Password
void hash_password(const char *password, char *hash)
{
    unsigned long value = 5381;

    while (*password)
    {
        value = ((value << 5) + value) + (*password);
        password++;
    }

    sprintf(hash, "%lu", value);
}
// verify password
bool verify_hash(const char *password, const char *stored_hash)
{
    char new_hash[100];

    hash_password(password, new_hash);

    if (strcmp(new_hash, stored_hash) == 0)
        return true;

    return false;
}

 // XOR encryption stored as printable hex text.
void encrypt(char *text, const char *key)
{
    char encrypted[256];
    size_t text_length;
    size_t key_length;

    if (!text || !key || key[0] == '\0')
        return;

    if (strncmp(text, ENCRYPTION_PREFIX, strlen(ENCRYPTION_PREFIX)) == 0)
        return;

    text_length = strlen(text);
    key_length = strlen(key);

    if ((text_length * 2) + strlen(ENCRYPTION_PREFIX) >= sizeof(encrypted))
        return;

    strcpy(encrypted, ENCRYPTION_PREFIX);

    for (size_t i = 0; i < text_length; i++)
    {
        unsigned char byte = (unsigned char)(text[i] ^ key[i % key_length]);
        encrypted[strlen(ENCRYPTION_PREFIX) + (i * 2)] = to_hex((byte >> 4) & 0x0F);
        encrypted[strlen(ENCRYPTION_PREFIX) + (i * 2) + 1] = to_hex(byte & 0x0F);
    }

    encrypted[strlen(ENCRYPTION_PREFIX) + (text_length * 2)] = '\0';
    strcpy(text, encrypted);
}
 // XOR decryption from printable hex text.
void decrypt(char *text, const char *key)
{
    char decrypted[256];
    size_t prefix_length = strlen(ENCRYPTION_PREFIX);
    size_t encoded_length;
    size_t key_length;

    if (!text || !key || key[0] == '\0')
        return;

    if (strncmp(text, ENCRYPTION_PREFIX, prefix_length) != 0)
        return;

    encoded_length = strlen(text + prefix_length);
    key_length = strlen(key);

    if (encoded_length % 2 != 0 || encoded_length / 2 >= sizeof(decrypted))
        return;

    for (size_t i = 0; i < encoded_length / 2; i++)
    {
        int high = from_hex(text[prefix_length + (i * 2)]);
        int low = from_hex(text[prefix_length + (i * 2) + 1]);

        if (high < 0 || low < 0)
            return;

        decrypted[i] = (char)(((high << 4) | low) ^ key[i % key_length]);
    }

    decrypted[encoded_length / 2] = '\0';
    strcpy(text, decrypted);
}
  //Generate Random Password
void generate_random_password(char *password, int length)
{
    const char characters[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#$%^&*";

    int total = strlen(characters);

    srand(time(NULL));

    for (int i = 0; i < length; i++)
    {
        password[i] = characters[rand() % total];
    }

    password[length] = '\0';
}
