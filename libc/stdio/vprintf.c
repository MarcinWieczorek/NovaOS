#include <stdio.h>
#include <string.h>
#include <unistd.h>

int vprintf(const char *format, __isoc_va_list args) {
    char str[256]; //TODO
    memset(&str, 0, 256);
    int r = vsprintf(str, format, args);
    write(STDOUT_FILENO, str, r);
    return r;
}
