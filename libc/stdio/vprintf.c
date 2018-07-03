#include <stdio.h>

int vprintf(const char *format, __isoc_va_list args) {
    char str[100]; //TODO
    int r = vsprintf(str, format, args);
    puts(str);
    return r;
}
