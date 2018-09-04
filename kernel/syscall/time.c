#include <kernel/syscall/syscall.h>
#include <kernel/cmos/cmos.h>

time_t do_time(time_t *tloc) {
    time_t t = CMOS_get_unix_time();

    if(tloc != NULL) {
        *tloc = t;
    }

    return t;
}
