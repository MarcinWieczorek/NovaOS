#include <stdio.h>
#include <string.h>

int vprintf(const char *format, __isoc_va_list args) {
    char str[256]; //TODO
    memset(&str, 0, 256);
    int r = vsprintf(str, format, args);
    puts(str);
    return r;
}
