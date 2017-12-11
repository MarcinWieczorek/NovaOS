#include <stdlib.h>
#include <string.h>
#include <math.h>

double atof(const char *str) {
    const char *start = str;
    int i = 0;
    double d = 0;
    char buf[32] = {0};
    const char *coma = start;

    while(*str != '\0') {
        char c = *str;

        if(c == '.') {
            strncpy(buf, start, str - start);
            i = atoi(buf);
            coma = str;
        }

        str++;
    }

    if(coma == start) {
        return atoi(start);
    }

    memset(buf, 0, 32);
    strncpy(buf, coma + 1, str - coma - 1);
    d = atoi(buf) / pow(10, str - coma);

    return i + d;
}
