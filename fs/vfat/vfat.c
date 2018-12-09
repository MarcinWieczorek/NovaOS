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
    fs->write = &VFAT_write;
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

uint32_t VFAT_get_offset_cluster(vfs_fs_t *fs, uint32_t cluster) {
    fs_vfat_t *vfat = fs->priv;
    return (vfat->header->BPB_RsvdSecCnt
            + (cluster - 2) * vfat->header->BPB_SecPerClus
            + (vfat->header->BPB_NumFATs * vfat->fatsz)
            + (vfat->header->BPB_RootEntCnt * 32
                + vfat->header->BPB_BytsPerSec - 1)
            / vfat->header->BPB_BytsPerSec) * vfat->header->BPB_BytsPerSec;
}

void VFAT_read_cluster(vfs_fs_t *fs, uint32_t cluster, uint8_t *buf) {
    fs_vfat_t *vfat = fs->priv;
    fs->dev->read(fs->dev, buf, VFAT_get_offset_cluster(fs, cluster),
                  vfat->header->BPB_BytsPerSec);
}

void VFAT_write_cluster(vfs_fs_t *fs, uint32_t cluster, off_t offset,
                        uint8_t *buf, size_t n) {
    fs->dev->write(fs->dev, buf,
                   VFAT_get_offset_cluster(fs, cluster) + offset, n);
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

ssize_t VFAT_write_chain(vfs_fs_t *fs, uint32_t cluster, off_t offset,
                         uint8_t *buf, size_t n) {
    size_t n_start = n;
    fs_vfat_t *vfat = fs->priv;
    uint32_t entry;
    uint32_t bpc = vfat->header->BPB_BytsPerSec * vfat->header->BPB_SecPerClus;

    do {
        if(n <= 0) {
            break;
        }

        if(offset == 0) {
            VFAT_write_cluster(fs, cluster, 0, buf, n);
            buf += bpc;
            n -= bpc;
        }
        else if(offset < bpc) {
            uint32_t ncluster = n > bpc ? bpc - offset : n;
            VFAT_write_cluster(fs, cluster, offset, buf, ncluster);
            buf += bpc - offset;
            n -= ncluster;
            offset = 0;
        }
        else {
            offset -= bpc;
        }

        entry = VFAT_fat_entry(fs, cluster);
        cluster = entry;
    } while(VFAT_fat_hasnext(fs, entry));

    return n_start - n;
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

    return VFAT_read_dir_clus(fs, cluster - 2);
}

VFAT_directory_entry *VFAT_read_dir_clus(vfs_fs_t *fs, uint32_t cluster) {
    fs_vfat_t *vfat = fs->priv;
    VFAT_header *h = vfat->header;
    uint8_t *buf = calloc(h->BPB_BytsPerSec * h->BPB_SecPerClus,
                          VFAT_size_clusters(fs, cluster));
    VFAT_read_chain(fs, cluster, buf);
    return (VFAT_directory_entry *) buf;
}

VFAT_directory_entry *VFAT_read_dir(vfs_fs_t *fs, VFAT_directory_entry *d) {
    return VFAT_read_dir_clus(fs, (d->fst_clus_hi << 16) | d->fst_clus_lo);
}

void VFAT_free(fs_vfat_t *vfat) {
    free(vfat->header);
    free(vfat->fat);
    free(vfat);
}

uint8_t *VFAT_get_name_fat(uint8_t *str) {
    uint8_t *name = malloc(12);
    uint8_t *dotpos = strchr(str, '.');
    memset(name, ' ', 11);
    name[11] = '\0';
    if(dotpos != NULL) {
        strncpy(name, str, dotpos - str);
        strncpy(name + 8, dotpos + 1, 3);
    }
    else {
        strncpy(name, str, strlen(str));
    }
    return name;
}

VFAT_directory_entry *VFAT_find_in_dir_re(vfs_fs_t *fs,
                                          VFAT_directory_entry *search_dir,
                                          uint8_t *filename) {
    uint8_t *rname = strchr(filename, '/');
    int dname_len = rname - filename;
    uint8_t *dname = malloc(dname_len + 1);
    strncpy(dname, filename, dname_len);
    dname[dname_len] = '\0';
    uint8_t *fatdname = VFAT_get_name_fat(dname);
    uint8_t *fatrname = VFAT_get_name_fat(rname + 1);

    VFAT_directory_entry *next_dir = NULL;

    for(int di = 0; di < 512 / sizeof(VFAT_directory_entry); di++) {
        VFAT_directory_entry *d = search_dir + di;
        if(d->attr == 0) {
            break;
        }

        if(*dname) {
            if(d->attr != VFAT_DIR_ATTR_DIRECTORY) {
                continue;
            }

            if(strncmp(fatdname, d->name, 11) == 0) {
                next_dir = d;
                break;
            }
        }
        else {

            if(strncmp(fatrname, d->name, 11) == 0) {
                free(fatdname);
                free(fatrname);
                free(dname);
                return d;
            }
        }
    }

    free(fatrname);
    free(fatdname);
    if(next_dir != NULL) {
        VFAT_directory_entry *next_dir_content = VFAT_read_dir(fs, next_dir);
        return VFAT_find_in_dir_re(fs, next_dir_content, rname);
    }

    free(dname);
    return NULL;
}

uint32_t VFAT_file_size(vfs_fs_t *fs, uint8_t *filename) {
    VFAT_directory_entry *root_dir = VFAT_read_dir_root(fs);
    VFAT_directory_entry *dir = VFAT_find_in_dir_re(fs, root_dir, filename);
    free(root_dir);
    return dir->file_size;
}

ssize_t VFAT_read(vfs_fs_t *fs, vfs_fdstruct *fds, uint8_t *buf, size_t n) {
    uint8_t *filename = fds->path + strlen(fds->mp->location);
    VFAT_directory_entry *root_dir = VFAT_read_dir_root(fs);
    VFAT_directory_entry *dir = VFAT_find_in_dir_re(fs, root_dir, filename);
    free(root_dir);

    if(dir == NULL) {
        return 0;
    }

    uint32_t clus = dir->fst_clus_lo | (dir->fst_clus_hi << 16);
    uint8_t *file_buf = malloc(dir->file_size);
    VFAT_read_chain(fs, clus, file_buf);
    memcpy(buf, file_buf + fds->seek, n);
    free(file_buf);
    free(dir);
    return n;
}

ssize_t VFAT_write(vfs_fs_t *fs, vfs_fdstruct *fds, uint8_t *buf, size_t n) {
    uint8_t *filename = fds->path + strlen(fds->mp->location);
    VFAT_directory_entry *root_dir = VFAT_read_dir_root(fs);
    VFAT_directory_entry *dir = VFAT_find_in_dir_re(fs, root_dir, filename);
    free(root_dir);

    if(dir == NULL) {
        return 0;
    }

    uint32_t clus = dir->fst_clus_lo | (dir->fst_clus_hi << 16);
    free(dir);
    return VFAT_write_chain(fs, clus, fds->seek, buf, n);
}

