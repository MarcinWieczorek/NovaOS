#ifndef _FS_VFS_DEVICE_H
#define _FS_VFS_DEVICE_H

#define __NEED_ssize_t
#define __NEED_off_t
#include <stdint.h>
#include <stdlib.h>

enum device_type {
    BLOCK,
    CHAR,
};

typedef struct __file_operations {
} device_fops_t;

typedef struct __device_t {
    char *name;
    enum device_type type;
    uint32_t major, minor;
    void (*read)(struct __device_t *, uint8_t* buf, uint32_t offset, uint32_t len);
    void (*write)(struct __device_t *, uint8_t *buf, uint32_t offset, uint32_t len);
    device_fops_t *fops;
    void *priv;
} device_t;

device_t *device_init();

#endif
