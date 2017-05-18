#include <string.h>

char* strcat(char *restrict to, const char *restrict from) {
    strcpy(to + strlen (to), from);
    return to;
}

