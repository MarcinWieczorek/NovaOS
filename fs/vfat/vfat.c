#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fs/vfat/vfat.h>

vfs_fs_t *VFAT_init(device_t *dev) {
    /* dev->read(dev, buf, 0, 3); */
    vfs_fs_t *fs = malloc(sizeof(vfs_fs_t));
    fs->dev = dev;
    fs_vfat_t *vfatfs = malloc(sizeof(fs_vfat_t));
    fs->priv = vfatfs;
    fs->name = "VFAT";
    fs->read = &VFAT_read;
    VFAT_header *h = malloc(sizeof(VFAT_header));
    vfatfs->header = h;
    dev->read(dev, (uint8_t *) h, 0, sizeof(VFAT_header));

    uint32_t root_dir_sectors =
        (h->BPB_RootEntCnt * 32 + h->BPB_BytsPerSec - 1) / h->BPB_BytsPerSec;

    uint32_t fatsz;

    if(h->BPB_FATSz16 != 0) {
        fatsz = h->BPB_FATSz16;
    }
    else {
        fatsz = h->BPB_FATSz32;
    }

    vfatfs->fatsz = fatsz;
    uint32_t tot_sec;

    if(h->BPB_TotSec16 != 0) {
        tot_sec = h->BPB_TotSec16;
    }
    else {
        tot_sec = h->BPB_TotSec32;
    }

    uint32_t data_sec = tot_sec - (h->BPB_RsvdSecCnt + (h->BPB_NumFATs * fatsz) + root_dir_sectors);
    uint32_t count_of_clusters = data_sec / h->BPB_SecPerClus;

    if(count_of_clusters < 4085) {
        vfatfs->type = 12;
    }
    else if(count_of_clusters < 65525) {
        vfatfs->type = 16;
    }
    else {
        vfatfs->type = 32;
    }

    int first_root_dir_entry = h->BPB_RsvdSecCnt * h->BPB_BytsPerSec
        + (h->BPB_NumFATs * fatsz * h->BPB_BytsPerSec);

    vfatfs->first_root_dir_entry = first_root_dir_entry;

    uint32_t static_fat[128];
    uint32_t *fat = &static_fat[0];
    vfatfs->fat = fat;
    dev->read(dev, (uint8_t *) fat, h->BPB_RsvdSecCnt * h->BPB_BytsPerSec, h->BPB_BytsPerSec);

    return fs;
}

uint32_t VFAT_fat_entry(vfs_fs_t *fs, uint32_t cluster) {
    fs_vfat_t *vfat = fs->priv;
    VFAT_header *h = vfat->header;
    uint32_t fat_offset;

    if(vfat->type == 12) {
        fat_offset = cluster + (cluster / 2);
    }
    else if(vfat->type == 32) {
        fat_offset = cluster * 4;
    }
    uint32_t fat_sector = h->BPB_RsvdSecCnt + (fat_offset / h->BPB_BytsPerSec);
    uint32_t fat_entry = fat_offset % h->BPB_BytsPerSec;
    fat_entry /= 4;
    return VFAT_read_fat(fs, fat_entry);
}

uint32_t VFAT_size_clusters(vfs_fs_t *fs, uint32_t cluster) {
    fs_vfat_t *vfat = fs->priv;
    uint32_t entry;
    uint32_t size = 0;

    do {
        size++;
        entry = VFAT_fat_entry(fs, cluster);
        cluster = entry;
    } while(VFAT_fat_hasnext(fs, entry));

    return size;
}

uint32_t VFAT_size_bytes(vfs_fs_t *fs, uint32_t cluster) {
    fs_vfat_t *vfat = fs->priv;
    return VFAT_size_clusters(fs, cluster)
        * vfat->header->BPB_BytsPerSec * vfat->header->BPB_SecPerClus;
}

uint32_t VFAT_read_fat(vfs_fs_t *fs, uint32_t entry) {
    fs_vfat_t *vfat = fs->priv;
    return vfat->fat[entry];
}

void VFAT_read_cluster(vfs_fs_t *fs, uint32_t cluster, uint8_t *buf) {
    /* printf("READING cluster 0x%X\n", cluster); */
    cluster -= 2;
    fs_vfat_t *vfat = fs->priv;
    fs->dev->read(fs->dev, buf,
            (vfat->header->BPB_RsvdSecCnt
            + cluster * vfat->header->BPB_SecPerClus
            + (vfat->header->BPB_NumFATs * vfat->fatsz)
            + (vfat->header->BPB_RootEntCnt * 32
                + vfat->header->BPB_BytsPerSec - 1)
            / vfat->header->BPB_BytsPerSec) * vfat->header->BPB_BytsPerSec,
                  vfat->header->BPB_BytsPerSec);
                  vfat->header->BPB_BytsPerSec);
}

uint8_t VFAT_fat_hasnext(vfs_fs_t *fs, uint32_t entry) {
    if(entry < 2) {
        return 0;
    }

    switch(((fs_vfat_t *) fs->priv)->type) {
        case 12:
            return entry < 0xFF8;
        case 16:
            return entry < 0xFFF8;
        case 32:
            return (entry >> 1) < 0xFFFFFF8;
    }
}

void VFAT_read_chain(vfs_fs_t *fs, uint32_t cluster, uint8_t *buf) {
    fs_vfat_t *vfat = fs->priv;
    uint32_t entry;

    do {
        VFAT_read_cluster(fs, cluster, buf);
        buf += vfat->header->BPB_BytsPerSec;
        entry = VFAT_fat_entry(fs, cluster);
        cluster = entry;
    } while(VFAT_fat_hasnext(fs, entry));
}

VFAT_directory_entry *VFAT_read_dir_root(vfs_fs_t *fs) {
    fs_vfat_t *vfat = fs->priv;
    uint32_t cluster;

    if(vfat->type == 32) {
        cluster = vfat->header->BPB_RootClus + 1;
    }
    else {
        cluster = 0;
    }

    return VFAT_read_dir(fs, cluster);
}

VFAT_directory_entry *VFAT_read_dir(vfs_fs_t *fs, uint32_t cluster) {
    fs_vfat_t *vfat = fs->priv;
    VFAT_header *h = vfat->header;
    uint8_t *buf = calloc(h->BPB_BytsPerSec * h->BPB_SecPerClus,
                          VFAT_size_clusters(fs, cluster));
    VFAT_read_chain(fs, cluster, buf);
    return (VFAT_directory_entry *) buf;
}

void VFAT_free(fs_vfat_t *vfat) {
    free(vfat->header);
    free(vfat->fat);
    free(vfat);
}

/* VFAT_directory_entry *VFAT_find_in_dir() */

VFAT_directory_entry *VFAT_find_dir_entry(vfs_fs_t *fs, char *filename) {
    VFAT_directory_entry *root_dir = VFAT_read_dir_root(fs);
    VFAT_directory_entry *dir = NULL;
    uint8_t name[13];

    for(int di = 0; di < 512 / sizeof(VFAT_directory_entry); di++) {
        VFAT_directory_entry *d = root_dir + di;

        uint8_t n0 = d->name[0];

        if(n0 == 'A' || d->attr == FAT_DIR_ATTR_LONGNAME) {
            continue;
        }

        int name_pos = 0;
        for(int ni = 0; ni < 11; ni++) {
            if(d->name[ni] == ' ') {
                continue;
            }

            if(ni == 8) {
                name[name_pos++] = '.';
            }

            name[name_pos++] = d->name[ni];
        }

        name[name_pos] = '\0';

        if(strncmp(filename, name, 11) == 0) {
            dir = d;
            break;
        }
    }

    if(dir == NULL) {
        free(root_dir);
        return NULL;
    }

    VFAT_directory_entry *dir_copy = malloc(sizeof(VFAT_directory_entry));
    memcpy(dir_copy, dir, sizeof(VFAT_directory_entry));
    free(root_dir);
    return dir_copy;
}

uint32_t VFAT_file_size(vfs_fs_t *fs, char *filename) {
    return VFAT_find_dir_entry(fs, filename)->file_size;
}

void VFAT_read(vfs_fs_t *fs, vfs_fdstruct *fds, uint8_t *buf, size_t n) {
    char *filename = fds->path + strlen(fds->mp->location);
    VFAT_directory_entry *dir = VFAT_find_dir_entry(fs, filename);

    if(dir == NULL) {
        return;
    }

    uint32_t clus = dir->fst_clus_lo | (dir->fst_clus_hi << 8);
    uint8_t *file_buf = malloc(dir->file_size);
    VFAT_read_chain(fs, clus, file_buf);
    memcpy(buf, file_buf + fds->seek, n);
    free(file_buf);
    free(dir);
}