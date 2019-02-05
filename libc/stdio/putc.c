#include <stdio.h>

int putc(int c, FILE *fh) {
    return fwrite(&c, 1, 1, fh);
}
