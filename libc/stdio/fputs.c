#include <stdio.h>
#include <string.h>

int fputs(const char *ptr, FILE *stream) {
    return fwrite(ptr, strlen(ptr), 1, stream);
}
