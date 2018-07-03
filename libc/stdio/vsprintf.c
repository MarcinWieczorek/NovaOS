#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

const char digits_dec[] = "0123456789";
const char digits_hex_upper[] = "0123456789ABCDEF";
const char digits_hex_lower[] = "0123456789abcdef";

static void vsprintf_int(char **str, int i, int base, const char *digits);

int vsprintf(char *str, const char *format, __isoc_va_list args) {
    int stop = (int) format + strlen(format);
    char length = 64;

    while((int) format < stop) {
        char c = *(format);

        if(c == '%' || isdigit(c)) {
            format++;
            char specifier = *format;

            if(specifier == 's') {
                char *s = va_arg(args, char*);
                strcpy(str, s);
                str += strlen(s);
            }
            else if(specifier == '%') {
                *(str++) = '%';
            }
            else if(specifier == 'd' || specifier == 'i') {
                vsprintf_int(&str, va_arg(args, signed int),
                             10, digits_dec);
            }
            else if(specifier == 'x') {
                vsprintf_int(&str, va_arg(args, signed int),
                             16, digits_hex_lower);
            }
            else if(specifier == 'X') {
                vsprintf_int(&str, va_arg(args, signed int),
                             16, digits_hex_upper);
            }
            else if(specifier == 'f') {
                float f = va_arg(args, double);
                vsprintf_int(&str, (int) f, 10, digits_dec);
                *(str++) = '.';
                vsprintf_int(&str, abs((f - (unsigned int)f) * 10000000.f),
                             10, digits_dec);
            }

            format++;
            continue;
        }

        format++;
        *(str++) = c;
        length = 64;
    }

    va_end(args);
}

static void vsprintf_int(char **str, int i, int base, const char *digits) {
    if(i < 0) {
        *(*(str)++) = '-';
    }

    unsigned char len = 0;
    signed int ii = i;
    do {
        len++;
        ii /= base;
    } while(ii);

    unsigned char llen = len;
    unsigned char zero = 1;

    do {
        char digit = digits[i % base];

        if(zero) {
            if(digit == '0') {
                len--;
                continue;
            }
            else {
                zero = 0;
            }
        }

        *(*str + len - 1) = digit;
        i /= base;
        len--;
    } while(i);

    *str += llen;
}
