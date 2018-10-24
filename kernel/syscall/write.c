#define _NEED_ssize_t
#include <bits/alltypes.h>
#include <kernel/syscall/syscall.h>
#include <drivers/screen.h>
#include <fs/vfs/vfs.h>

ssize_t do_write(int fildes, const void *buf, size_t nbyte) {
    if(fildes == 1) { //TODO implement stdout device
        size_t i = 0;
        for(; i < nbyte; i++) {
            print_char(*(char *)(buf + i));
        }

        return i + 1;
    }

    return vfs_write(vfs_get_fdstruct(fildes), (void *) buf, nbyte);
}
