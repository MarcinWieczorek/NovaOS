#ifndef _FS_DEVFS_DEVFS_H
#define _FS_DEVFS_DEVFS_H

#include <fs/vfs/device.h>

#define DEVFS_MAJORS_SIZE 16

typedef struct __device_major {
    char *name;
    device_fops_t *fops;
    uint32_t nextminor;
} devfs_major_t;

typedef struct __devfs {
    devfs_major_t *majors;
    device_t *devices;
} devfs_t;

int devfs_init();

int devfs_major_register(uint32_t, char *, device_fops_t *);

int devfs_major_unregister(uint32_t);

int devfs_major_exists(uint32_t);

devfs_major_t *devfs_major_get(uint32_t);

int devfs_node_add(uint8_t *, device_t *);

#endif
