#ifndef _FS_VFS_DEVICE_H
#define _FS_VFS_DEVICE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct __device_t {
    char *name;
    void (*read)(struct __device_t *, uint8_t* buf, uint32_t offset, uint32_t len);
    void (*write)(struct __device_t *, uint8_t *buf, uint32_t offset, uint32_t len);
    void *priv;
} device_t;

device_t *device_init();

#endif
