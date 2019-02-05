#define __NEED_off_t
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <../internal/stdio_impl.h>

FILE *fdopen(int fd, const char *mode) {
    FILE *stream = malloc(sizeof(FILE));
    stream->off = lseek(fd, 0, SEEK_CUR);
    stream->fd = fd;
    return stream;
}
