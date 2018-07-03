#include <stdio.h>
#include <stdarg.h>

int sprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    return vsprintf(str, format, args);
}
