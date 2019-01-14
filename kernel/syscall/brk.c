#include <unistd.h>
#include <kernel/mm/layout.h>

int do_brk(void *ptr) {
    return HEAP_START;
}
