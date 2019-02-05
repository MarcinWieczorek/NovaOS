#define __NEED_off_t
#include <stdio.h>
#include <bits/alltypes.h>
#include <../internal/stdio_impl.h>

int fileno(FILE *fh) {
    return fh->fd;
}
