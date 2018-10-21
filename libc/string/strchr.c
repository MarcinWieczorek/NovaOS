#include <string.h>

char *strchr(const char *str, int c) {
    for(; *str && *str != c; str++);
    return (char *) (*str ? str : NULL);
}

