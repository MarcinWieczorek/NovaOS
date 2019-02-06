#include <stdio.h>
#include <../internal/stdio_impl.h>

off_t ftello(FILE *stream) {
    return stream->off;
}
