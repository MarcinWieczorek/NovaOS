#include <string.h>

char *strstr(const char *haystack, const char *needle) {
    char *nw = (char*) needle;
    char c;
    char *first = (char*) haystack;

    while((c = *(haystack++)) != '\0') {
        if(c != *nw) {
            if(*nw == '\0') {
                return first;
            }

            nw = (char*) needle;
            first = (char*) haystack;
        }
        else {
            nw++;
        }
    }

    return NULL;
}
