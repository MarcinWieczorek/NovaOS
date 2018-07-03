#include <stdlib.h>
#include <string.h>
#include <kernel/memory_manager.h>

void *realloc(void *ptr, size_t size) {
    void *newptr = malloc(size);
    memcpy(newptr, ptr, mm_getsize(ptr));
    free(ptr);
    return newptr;
}

