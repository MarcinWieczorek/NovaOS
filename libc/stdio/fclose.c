#include <stdio.h>
#include <unistd.h>

int fclose(FILE *stream) {
    return close(fileno(stream));
}

