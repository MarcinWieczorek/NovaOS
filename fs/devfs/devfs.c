#include <string.h>

#include <fs/devfs/devfs.h>
#include <fs/vfs/vfs.h>

static devfs_t *devfs_get() {
    return vfs_find_mountpoint("/dev")->fs->priv;
}

int devfs_init() {
    devfs_t *devfs = malloc(sizeof(devfs_t));
    devfs->majors = calloc(DEVFS_MAJORS_SIZE, sizeof(devfs_major_t));
    devfs->devices = calloc(16, sizeof(device_t));

    if(devfs->majors == NULL) {
        return 0;
    }

    for(int i = 0; i < DEVFS_MAJORS_SIZE; i++) {
        devfs->majors[i].name = NULL;
        devfs->majors[i].fops = NULL;
    }

    vfs_fs_t *fs = malloc(sizeof(vfs_fs_t));
    fs->priv = devfs;
    if(vfs_mount("/dev", NULL, fs) == NULL) {
        return 0;
    }

    return 1;
}

int devfs_major_register(uint32_t major, char *name, device_fops_t *fops) {
    if(devfs_major_exists(major)) {
        return 0;
    }

    devfs_t *devfs = devfs_get();
    devfs->majors[major].name = name;
    devfs->majors[major].fops = fops;
    return 1;
}

int devfs_major_unregister(uint32_t major) {
    devfs_t *devfs = devfs_get();
    devfs->majors[major].name = NULL;
    devfs->majors[major].fops = NULL;
    return 1;
}

int devfs_major_exists(uint32_t major) {
    devfs_t *devfs = devfs_get();
    return devfs->majors[major].name != NULL
        && devfs->majors[major].fops != NULL;
}

devfs_major_t *devfs_major_get(uint32_t major) {
    devfs_t *devfs = devfs_get();
    return devfs->majors + major;
}

int devfs_node_add(uint8_t *path, device_t *dev) {
    if(dev->major == -1) {
        return 0;
    }

    devfs_major_t *major = devfs_major_get(dev->major);
    dev->minor = major->nextminor++;
    dev->fops = malloc(sizeof(device_fops_t));

    if(dev->fops == NULL) {
        return 0;
    }

    memcpy(dev->fops, major->fops, sizeof(device_fops_t));

    return 1;
}

