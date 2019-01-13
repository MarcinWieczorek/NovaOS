#include <stdio.h>
#include <unistd.h>
#include <string.h>

int puts(const char *str) {
    int r = write(STDOUT_FILENO, str, strlen(str));
    r += putchar('\n');
    return r;
}
