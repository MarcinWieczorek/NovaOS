#include <string.h>
#include <stdarg.h>
#include <drivers/screen.h>

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int stop = (int) format + strlen(format);

    while((int) format < stop) {
        char c = *(format);

        if(c == '%') {
            format++;
            char specifier = *format;

            switch(specifier) {
                case 's':
                    print(va_arg(args, char*));
                    break;
                case 'c':
                    print_char(va_arg(args, int));
                    break;
                case '%':
                    print_char('%');
                    break;
                case 'x':
                case 'X':
                    print_hex(va_arg(args, unsigned int), c=='X');
                    break;
                case 'u':
                    print_int(va_arg(args, unsigned int));
                    break;
                case 'd':
                case 'i':
                    print_int(va_arg(args, signed int));
                    break;
            }

            format++;
            continue;
        }

        format++;
        print_char(c);
    }

    va_end(args);
}
