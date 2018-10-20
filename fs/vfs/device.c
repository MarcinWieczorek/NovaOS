#include <fs/vfs/device.h>

device_t *device_init(char *name) {
    return malloc(sizeof(device_t));
}

