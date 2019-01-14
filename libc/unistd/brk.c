#include <unistd.h>
#include <bits/syscall.h>

int brk(void *ptr) {
    return syscall(SYS_brk, ptr);
}
