#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdbool.h>

// Hash a password using a simple hashing algorithm (djb2).
// @param password The password to hash.
// @param hash The output buffer to store the resulting hash.
void hash_password(const char *password, char *hash);

// Verify if the provided password matches the stored hash.
// @param password The password to verify.
// @param stored_hash The stored hash to compare against.
bool verify_hash(const char *password, const char *stored_hash);

// Encrypt a text using XOR encryption with a given key.
// @param text The text to encrypt (in-place).
// @param key The key to use for encryption.
void encrypt(char *text, const char *key);

// Decrypt a text that was encrypted using XOR encryption with a given key.
// @param text The text to decrypt (in-place).
// @param key The key to use for decryption.
void decrypt(char *text, const char *key);

// Generate a random password of a specified length.
// @param password The output buffer to store the generated password.
// @param length The desired length of the password.
void generate_random_password(char *password, int length);

#endif
