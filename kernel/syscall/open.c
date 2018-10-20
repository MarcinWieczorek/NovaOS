#include <fs/vfs/vfs.h>

int do_open(const char *path, int mode, ...) {
    return vfs_open((char *) path, mode);
}
