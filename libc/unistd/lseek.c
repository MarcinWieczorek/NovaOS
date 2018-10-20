#include <unistd.h>
#include <bits/syscall.h>

off_t lseek(int fd, off_t offset, int whence) {
    return syscall(SYS_lseek, fd, offset, whence);
}

