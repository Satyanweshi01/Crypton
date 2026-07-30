#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
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

 // XOR Encryption
void encrypt(char *text, const char *key)
{
    int i;
    int keyLength = strlen(key);

    for (i = 0; text[i] != '\0'; i++)
    {
        text[i] = text[i] ^ key[i % keyLength];
    }
}
 // XOR Decryption
void decrypt(char *text, const char *key)
{
    int i;
    int keyLength = strlen(key);

    for (i = 0; text[i] != '\0'; i++)
    {
        text[i] = text[i] ^ key[i % keyLength];
    }
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