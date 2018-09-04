#include <time.h>
#include <unistd.h>

#include <bits/syscall.h>

time_t time(time_t *tloc) {
    return syscall(SYS_time, tloc);
}
