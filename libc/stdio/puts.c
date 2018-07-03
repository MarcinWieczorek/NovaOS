#include <stdio.h>
#include <unistd.h>
#include <string.h>

int puts(const char *str) {
    return write(0, str, strlen(str));
}
