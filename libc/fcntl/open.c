#include <fcntl.h>
#include <unistd.h>
#include <bits/syscall.h>

int open(const char *path, int mode, ...) {
    return syscall(SYS_open, path, mode);
}
