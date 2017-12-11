#include <string.h>
#include <stdlib.h>

char *strtok(char *start, const char *delim) {
    char *c = start;

    while(*c != '\0') {
        if(*(c+1) == '\0') {
            char *result = malloc(strlen(start) + 1);
            strcpy(result, start);
            *start = '\0';
            return result;
        }

        if(strncmp(c, delim, strlen(delim)) == 0) {
            char *result = malloc(c - start + 1);
            memset(result, 0, c - start + 1);
            strncpy(result, start, c - start);

            int strlen_start = strlen(start);
            strcpy(start, c + 1);
            *(start + strlen_start - strlen(result) - 1) = '\0';
            return result;
        }

        c++;
    }

    return NULL;
}
