#include <unistd.h>
#include <bits/syscall.h>

pid_t fork(void) {
    return syscall(SYS_fork);
}
