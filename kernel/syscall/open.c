#include <fs/vfs/vfs.h>
#include <proc/process.h>

int do_open(const char *path, int mode, ...) {
    return vfs_open(proc_get(), (char *) path, mode);
}
