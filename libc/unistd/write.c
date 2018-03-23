#include <unistd.h>
#include <bits/syscall.h>

ssize_t write(int fd, const void *data, size_t n) {
    return syscall(SYS_write, fd, data, n);
}
