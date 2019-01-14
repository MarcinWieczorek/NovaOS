#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define MM_FLAG_FREE 0b00
#define MM_FLAG_HEAD 0b01
#define MM_FLAG_TAIL 0b10

uint8_t *bitmap;
void *head;
size_t max_memory;
void *mem_start, *mem_end;

static void set_flag(void *ptr, uint8_t flag) {
    uint8_t index = (ptr - mem_start) / 4;
    uint8_t pos = (ptr - mem_start) % 4;
    bitmap[index] |= flag << (6 - pos * 2);
}

static char get_flag(void *ptr) {
    uint8_t index = (ptr - mem_start) / 4;
    uint8_t pos = (ptr - mem_start) % 4;
    return bitmap[index] >> (6 - pos * 2) & 0x3;
}

static size_t mm_getsize(void *ptr) {
    size_t count = 0;

    while(get_flag(++ptr) == MM_FLAG_TAIL) {
        count++;
    }

    return count;
}

// LIBC

void *malloc(size_t size) {
    if(mem_start == NULL) {
        mem_start = (void *) brk(NULL);
        mem_end   = (void *) 0xF000000;
        head = mem_start + 0x1000;
    }

    if(head + size - 1 > mem_end) {
        return NULL;
    }

    set_flag(head, MM_FLAG_HEAD);

    size_t n = 1;
    while(++n <= size) {
        head++;
        set_flag(head, MM_FLAG_TAIL);
    }

    void *ptr = ++head - size;
    memset(ptr, 0, size);
    return ptr;
}

void free(void *ptr) {

}

void *realloc(void *ptr, size_t size) {
    void *newptr = malloc(size);
    memcpy(newptr, ptr, mm_getsize(ptr));
    free(ptr);
    return newptr;
}

