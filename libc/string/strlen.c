#include <string.h>

size_t strlen(const char *str) {
    const char *s;
    for(s = str; *s; ++s);
    return s - str;
}

