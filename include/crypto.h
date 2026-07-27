#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdbool.h>

void hash_password(const char *password, char *hash);

bool verify_hash(const char *password,
                 const char *stored_hash);

void encrypt(char *text,
             const char *key);

void decrypt(char *text,
             const char *key);

void generate_random_password(char *password,
                              int length);

#endif