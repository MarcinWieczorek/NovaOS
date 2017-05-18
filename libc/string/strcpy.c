#include <string.h>

char *strcpy(char *dest, const char *src) {
    do {
        *(dest++) = *(src++);
    }
    while (src != '\0');
}

