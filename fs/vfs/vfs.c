#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <fs/vfs/vfs.h>
#include <kernel/alg/ordered_array.h>

ordered_array *vfs_mountpoints;
vfs_fdstruct *fdlist;

static uint32_t vfs_mount_compare(void *va, void *vb) {
    char *a = (*(vfs_mountpoint_t **) va)->location;
    char *b = (*(vfs_mountpoint_t **) vb)->location;
    int ai = 0, bi = 0;

    for(int i = 0; a[i]; i++) {
        if(a[i] == '/') {
            ai++;
        }
    }

    for(int i = 0; b[i]; i++) {
        if(b[i] == '/') {
            bi++;
        }
    }
    return bi > ai;
}

vfs_mountpoint_t *vfs_find_mountpoint(char *path) {
    for(int i = 0; i < vfs_mountpoints->size; i++) {
        vfs_mountpoint_t *mp = ordered_array_get(vfs_mountpoints, i);

        if(strlen(mp->location) > strlen(path)) {
            continue;
        }

        if(strncmp(mp->location, path, strlen(mp->location)) == 0) {
            return mp;
        }
    }

    return NULL;
}

void vfs_init() {
    void *arr = calloc(VFS_MOUNTPOINTS_MAX, sizeof(vfs_mountpoint_t *));
    vfs_mountpoints = ordered_array_new(arr, VFS_MOUNTPOINTS_MAX, &vfs_mount_compare);
    fdlist = calloc(VFS_FD_MAX, sizeof(vfs_fdstruct));

    for(int i = 0; i < VFS_FD_MAX; i++) {
        fdlist[i].fd = -1;
    }

    fdlist[0].fd = 0;
    fdlist[1].fd = 1;
    fdlist[2].fd = 2;
}

vfs_mountpoint_t *vfs_mount(char *location, device_t *dev, vfs_fs_t *fs) {
    vfs_mountpoint_t *mp = malloc(sizeof(vfs_mountpoint_t));
    mp->location = location;
    mp->dev = dev;
    mp->fs = fs;
    ordered_array_insert(vfs_mountpoints, mp);
    return mp;
}

void vfs_umount(vfs_mountpoint_t *mp) {
    free(mp);
}

void vfs_free() {
    for(int i = 0; i < vfs_mountpoints->size; i++) {
        free(vfs_mountpoints->array[i]);
    }

    free(vfs_mountpoints);
    vfs_mountpoints = NULL;
}

ssize_t vfs_read(vfs_fdstruct *fds, uint8_t *buffer, size_t size) {
    if(fds == NULL || fds->fd == -1) {
        return 0;
    }

    ssize_t nread = fds->mp->fs->read(fds->mp->fs, fds, buffer, size);
    fds->seek += nread;
    return nread;
}

ssize_t vfs_write(vfs_fdstruct *fds, uint8_t *buf, size_t n) {
    if(fds == NULL || fds->fd == -1) {
        return 0;
    }

    ssize_t nwrote = fds->mp->fs->write(fds->mp->fs, fds, buf, n);
    fds->seek += nwrote;
    return nwrote;
}

off_t vfs_seek(int fd, off_t offset, int whence) {
    struct proc *proc = proc_get();
    vfs_fdstruct *fds = vfs_get_fdstruct(proc, fd);
    off_t newseek;

    if(whence == SEEK_CUR) {
        newseek = fds->seek + offset;
    }
    else if(whence == SEEK_SET){
        newseek = offset;
    }

    fds->seek = newseek;

    return fds->seek;
}

vfs_fdstruct *vfs_get_fdstruct(struct proc *p, int fd) {
    return &p->fd_list[fd];
}

int vfs_open(char *path, int mode) {
    vfs_fdstruct *fds;
    int fd = -1;
    struct proc *proc = proc_get();

    for(int fdl = 0; fdl < VFS_FD_MAX; fdl++) {
        if(vfs_fd_empty(proc, fdl)) {
            fd = fdl;
            break;
        }
    }

    vfs_mountpoint_t *mp = vfs_find_mountpoint(path);

    if(fd == -1 || mp == NULL) {
        return -1;
    }

    fds = proc->fd_list + fd;
    fds->fd = fd;
    fds->path = path;
    fds->mode = mode;
    fds->seek = 0;
    fds->mp = mp;

    return fd;
}

