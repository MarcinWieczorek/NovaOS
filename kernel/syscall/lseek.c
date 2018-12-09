#include <unistd.h>
#include <fs/vfs/vfs.h>

off_t do_lseek(int fd, off_t offset, int whence) {
    return vfs_seek(fd, offset, whence);
}
