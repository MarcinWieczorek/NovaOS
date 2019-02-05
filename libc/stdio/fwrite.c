#include <stdio.h>
#include <unistd.h>
#include <../internal/stdio_impl.h>

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    stream->off = write(fileno(stream), ptr, size * nmemb);
    return stream->off;
}
