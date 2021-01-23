#include <kernel/syscall/syscall.h>
#include <fs/vfs/vfs.h>

ssize_t do_read(int fd, void *buf, size_t n) {
    return vfs_read(vfs_get_fdstruct(proc_get(), fd), buf, n);
}
