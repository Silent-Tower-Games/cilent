#include "Maps.h"

uint64_t Cilent_Hash_String(const char* str)
{
    uint64_t hash = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        // This is how Java hashes strings
        hash += 31 * hash + str[i];
    }

    return hash;
}
