#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

static const char digits_dec[10] = "0123456789";
static const char digits_hex_upper[16] = "0123456789ABCDEF";
static const char digits_hex_lower[16] = "0123456789abcdef";

int vsprintf_int(char **, int, int, const char *, char);

int vsprintf(char *str, const char *format, __isoc_va_list args) {
    int stop = (int) format + strlen(format);
    char length = 64;

    while((int) format < stop) {
        char c = *(format);

        if(c == '%') {
            format++;
            char specifier = *format;

            if(specifier == 's') {
                char *s = va_arg(args, char *);
                strcpy(str, s);
                str += strlen(s);
            }
            else if(specifier == '%') {
                *(str++) = '%';
            }
            else if(specifier == 'd' || specifier == 'i') {
                vsprintf_int(&str, va_arg(args, signed int),
                             10, digits_dec, 1);
            }
            else if(specifier == 'u') {
                vsprintf_int(&str, va_arg(args, unsigned int),
                             10, digits_dec, 0);
            }
            else if(specifier == 'x') {
                vsprintf_int(&str, va_arg(args, signed int),
                             16, digits_hex_lower, 0);
            }
            else if(specifier == 'X') {
                vsprintf_int(&str, va_arg(args, signed int),
                             16, digits_hex_upper, 0);
            }
            else if(specifier == 'f') {
                float f = va_arg(args, double);
                vsprintf_int(&str, (int) f, 10, digits_dec, 1);

                if((int) f != f) {
                    *(str++) = '.';
                    int fl = vsprintf_int(&str,
                                          abs((int) (f * 1000000) % 1000000),
                                          10, digits_dec, 0);

                    for(char *fstr = str - 1; str - fstr < fl; fstr--) {
                        if(*fstr == '0') {
                            *fstr = '\0';
                            str--;
                            continue;
                        }

                        break;
                    }
                }
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

int vsprintf_int(char **str, int i, int base, const char *digits, char sig) {
    unsigned int ui = i;
    if(i < 0) {
        if(sig) {
            *((*str)++) = '-';
            ui = -i;
        }
    }

    unsigned char len = 0;
    unsigned int i_len = ui;
    do {
        len++;
        i_len /= base;
    } while(i_len);

    unsigned char llen = len;

    do {
        char digit = digits[ui % base];
        *(*str + len - 1) = digit;
        ui /= base;
        len--;
    } while(ui);

    *str += llen;
    return llen;
}
