#define __NEED_off_t
#include <stdio.h>
#include <unistd.h>
#include <../internal/stdio_impl.h>

int fseek(FILE *stream, long offset, int whence) {
    stream->off = lseek(fileno(stream), offset, whence);
    return stream->off;
}
