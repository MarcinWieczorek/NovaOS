#include <string.h>

void *memset (void *pointer, int data, size_t no_bytes) {
    unsigned char *ptr = pointer;

    for(int i = 0; i < no_bytes; i++) {
        *(ptr + i) = data;
    }
}
