#include <string.h>
#include <stdlib.h>

#include <dev/ramdev/ramdev.h>

device_t *ramdev_init(void *start) {
    device_t *dev = device_init("RAMDEV");
    dev_ramdev_t *ramdev = malloc(sizeof(dev_ramdev_t));

    ramdev->start = start;
    dev->priv = ramdev;
    dev->read = &ramdev_read;
    dev->write = &ramdev_write;

    return dev;
}

void ramdev_read(device_t *dev, uint8_t* buf, uint32_t offset, uint32_t len) {
    dev_ramdev_t *rd = (dev_ramdev_t *) dev->priv;
    memcpy(buf, ((char *) rd->start) + offset, len);
}

void ramdev_write(device_t *dev, uint8_t *buf, uint32_t offset, uint32_t len) {
    dev_ramdev_t *rd = (dev_ramdev_t *) dev->priv;
    memcpy(((char *) rd->start) + offset, buf, len);
}

