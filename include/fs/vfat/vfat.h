#ifndef _VFAT_H
#define _VFAT_H

#include <stdint.h>

#include <fs/vfs/vfs.h>

#define VFAT_ENTRY_FREE 0x0
#define VFAT_ENTRY_EOF 0xFFFFFFFF
#define VFAT_ENTRY_BAD 0xFFFFFFF7

#define VFAT_DIR_ATTR_READ_ONLY 0x01
#define VFAT_DIR_ATTR_HIDDEN    0x02
#define VFAT_DIR_ATTR_SYSTEM    0x04
#define VFAT_DIR_ATTR_VOLUME_ID 0x08
#define VFAT_DIR_ATTR_DIRECTORY 0x10
#define VFAT_DIR_ATTR_ARCHIVE   0x20
#define FAT_DIR_ATTR_LONGNAME (VFAT_DIR_ATTR_READ_ONLY \
        | VFAT_DIR_ATTR_HIDDEN | VFAT_DIR_ATTR_SYSTEM | VFAT_DIR_ATTR_VOLUME_ID)

typedef struct {
    uint8_t  BS_jmpBoot[3];
    uint8_t  BS_OEMName[8];
    uint16_t BPB_BytsPerSec;
    uint8_t  BPB_SecPerClus;
    uint16_t BPB_RsvdSecCnt;
    uint8_t  BPB_NumFATs;
    uint16_t BPB_RootEntCnt;
    uint16_t BPB_TotSec16;
    uint8_t  BPB_Media;
    uint16_t BPB_FATSz16;
    uint16_t BPB_SecPerTrk;
    uint16_t BPB_NumHeads;
    uint32_t BPB_HiddSec;
    uint32_t BPB_TotSec32;

    uint32_t BPB_FATSz32;
    uint16_t BPB_ExtFlags;
    uint16_t BPB_FSVer;
    uint32_t BPB_RootClus;
    uint16_t BPB_FSInfo;
    uint16_t BPB_BkBootSec;
    uint8_t  BPB_Reserved[12];
    uint8_t  BS_DrvNum;
    uint8_t  BS_Reserved1;
    uint8_t  BS_BootSig;
    uint32_t BS_VolID;
    uint8_t  BS_VolLab[11];
    uint64_t BS_FilSysType;
} __attribute__ ((packed)) VFAT_header;

typedef struct {
    uint8_t  name[11];
    uint8_t  attr;
    uint8_t  ntres;
    uint8_t  crt_time_tenth;
    uint16_t crt_time;
    uint16_t crt_date;
    uint16_t lst_acc_date;
    uint16_t fst_clus_hi;
    uint16_t wrt_time;
    uint16_t wrt_date;
    uint16_t fst_clus_lo;
    uint32_t file_size;
} VFAT_directory_entry;

typedef struct __fs_vfat {
    VFAT_header *header;
    uint32_t *fat;
    uint32_t fatsz;
    uint32_t first_root_dir_entry;
    uint8_t type;
} fs_vfat_t;

vfs_fs_t *VFAT_init(device_t *dev);

void VFAT_free(fs_vfat_t *);

uint32_t VFAT_fat_entry(vfs_fs_t *fs, uint32_t cluster);

uint32_t VFAT_size_clusters(vfs_fs_t *fs, uint32_t cluster);

uint32_t VFAT_read_fat(vfs_fs_t *fs, uint32_t entry);

void VFAT_read_cluster(vfs_fs_t *fs, uint32_t cluster, uint8_t *);

void VFAT_read_chain(vfs_fs_t *fs, uint32_t cluster, uint8_t *);

ssize_t VFAT_write_chain(vfs_fs_t *fs, uint32_t cluster, off_t offset,
                      uint8_t *buf, size_t n);

VFAT_directory_entry *VFAT_read_dir_root(vfs_fs_t *);

VFAT_directory_entry *VFAT_read_dir_clus(vfs_fs_t *, uint32_t);

VFAT_directory_entry *VFAT_read_dir(vfs_fs_t *, VFAT_directory_entry *);

uint8_t VFAT_fat_hasnext(vfs_fs_t *fs, uint32_t entry);

ssize_t VFAT_read(vfs_fs_t *, vfs_fdstruct *, uint8_t *, size_t);

ssize_t VFAT_write(vfs_fs_t *, vfs_fdstruct *, uint8_t *, size_t);

#endif
