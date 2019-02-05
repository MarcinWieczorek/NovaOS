#include <stdio.h>

int fgetc(FILE *stream) {
    unsigned char c;
    fread(&c, 1, 1, stream);
    return (int) c;
}
