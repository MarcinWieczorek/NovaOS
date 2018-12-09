#include <string.h>

char *strcpy(char *dest, const char *src) {
    char *ptr = dest;

    do {
        *(ptr++) = *(src++);
    }
    while(*src != '\0');

    return dest;
}

