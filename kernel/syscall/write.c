#define _NEED_ssize_t
#include <bits/alltypes.h>
#include <kernel/syscall/syscall.h>
#include <drivers/screen.h>
#include <fs/vfs/vfs.h>
#include <kernel/tty.h>
#include <proc/process.h>

ssize_t do_write(int fildes, const void *buf, size_t nbyte) {
    struct proc *proc = proc_get();
    if(fildes == 1) { //TODO implement stdout device
        size_t i = 0;
        struct tty *tty = proc->tty;

        for(; i < nbyte; i++) {
            tty_print_char(tty, *(char *)(buf + i));
        }

        tty_draw(tty);

        return i + 1;
    }

    return vfs_write(vfs_get_fdstruct(proc, fildes), (void *) buf, nbyte);
}
