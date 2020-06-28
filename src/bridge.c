#include "bridge.h"

/* Create a hash from range 0-HASH_MAX for a string */
unsigned long var_hash(const char *key)
{
    int sum = 0;
    for(char c = *key++; c != '\0'; c = *key++)
    {
        sum += (int)c * HASH_MAX;
    }

    return sum % HASH_MAX;
}