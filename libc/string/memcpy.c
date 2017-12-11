#include <string.h>

void *memcpy(void *dst, const void *src, size_t no_bytes) {
    char *dest = (void*) dst;
    char *source = src;

    for(int i = 0; i < no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}
