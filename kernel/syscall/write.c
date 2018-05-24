#include <kernel/syscall/syscall.h>
#include <drivers/screen.h>
#include <stdlib.h>

ssize_t do_write(int fildes, const void *buf, size_t nbyte) {
    size_t i = 0;
    for(; i < nbyte; i++) {
        print_char(*(char *)(buf + i));
    }

    return i + 1;
}
