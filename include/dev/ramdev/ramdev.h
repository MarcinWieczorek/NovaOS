#ifndef _DEV_RAMDEV_RAMDEV_H
#define _DEV_RAMDEV_RAMDEV_H

#include <fs/vfs/vfs.h>

typedef struct {
    void *start;
} dev_ramdev_t;

device_t *ramdev_init(void *);

void ramdev_read(device_t *, uint8_t* buf, uint32_t offset, uint32_t len);
void ramdev_write(device_t *, uint8_t *buf, uint32_t offset, uint32_t len);

#endif
