#include <stdlib.h>
#include <string.h>
#include <math.h>

int atoi(const char *str) {
    signed int i = 0;

    int n = strlen(str);
    char negative = 0;
    do {
        if(*str >= '0' && *str <= '9') {
            i += (int)(*str - 0x30) * pow(10, n);
        }
        else if(*str == '-') {
            negative = 1;
        }
        else {
            return 0;
        }

        n--;
    }
    while(*++str != '\0');

    i /= 10;

    return negative ? -i : i;
}
