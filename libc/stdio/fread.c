#include <stdio.h>
#include <unistd.h>
#include <../internal/stdio_impl.h>

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    stream->off = read(fileno(stream), ptr, size * nmemb);
    return stream->off;
}
