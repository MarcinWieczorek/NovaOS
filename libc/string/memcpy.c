#include <string.h>

void *memcpy (void *src, const void *dst, size_t no_bytes) {
    unsigned char *dest = dst;
    unsigned char *source = src;

    for(int i = 0; i < no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}
