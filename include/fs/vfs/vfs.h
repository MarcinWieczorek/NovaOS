#ifndef _FS_VFS_VFS_H
#define _FS_VFS_VFS_H

#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_off_t
#include <bits/alltypes.h>

#include <fs/vfs/device.h>

#define VFS_MOUNTPOINTS_MAX 16
#define VFS_FD_MAX 16

#define vfs_fd_empty(__fd) (fdlist[__fd].fd == -1)

struct __vfs_mountpoint;

typedef struct {
    int fd;
    off_t seek;
    char *path;
    struct __vfs_mountpoint *mp;
    int mode;
} vfs_fdstruct;

typedef struct __vfs_fs {
    char *name;
    ssize_t (*write)(struct __vfs_fs *, vfs_fdstruct *, uint8_t *, size_t);
    void (*read)(struct __vfs_fs *, vfs_fdstruct *, uint8_t *, size_t);
    void *priv;
    device_t *dev;
} vfs_fs_t;

typedef struct __vfs_mountpoint {
    char *location;
    device_t *dev;
    vfs_fs_t *fs;
} vfs_mountpoint_t;

vfs_mountpoint_t *vfs_find_mountpoint(char *);

void vfs_read(vfs_fdstruct *, uint8_t *, size_t);

ssize_t vfs_write(vfs_fdstruct *, uint8_t *, size_t);

off_t vfs_seek(int, off_t, int);

void vfs_init();

void vfs_free();

int vfs_open(char *, int);

vfs_fdstruct *vfs_get_fdstruct(int);

vfs_mountpoint_t *vfs_mount(char *location, device_t *, vfs_fs_t *);

vfs_mountpoint_t **vfs_mountlist();

void vfs_umount(vfs_mountpoint_t *);

#endif
