#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <drivers/screen.h>
#include <ctype.h>

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int r = vprintf(format, args);
    va_end(args);
    return r;
}
