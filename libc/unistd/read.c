#include <unistd.h>
#include <bits/syscall.h>

ssize_t read(int fd, void *data, size_t n) {
    return syscall(SYS_read, fd, data, n);
}

