#include <unistd.h>
#include <bits/syscall.h>

int close(int fd) {
    return syscall(SYS_close, fd);
}
