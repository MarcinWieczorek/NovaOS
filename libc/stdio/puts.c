#include <stdio.h>
#include <unistd.h>
#include <string.h>

int puts(const char *str) {
    return write(STDOUT_FILENO, str, strlen(str));
}
