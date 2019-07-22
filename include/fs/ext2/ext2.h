#include <stdio.h>
#include <string.h>

#include <fs/vfs/vfs.h>

#define EXT2_ROOT_INO 2

struct ext2_superblock {
    uint32_t s_inodes_count;
    uint32_t s_blocks_count;
    uint32_t s_r_blocks_count;
    uint32_t s_free_blocks_count;
    uint32_t s_free_inodes_count;
    uint32_t s_first_data_block;
    uint32_t s_log_block_size;
    uint32_t s_log_frag_size;
    uint32_t s_blocks_per_group;
    uint32_t s_frags_per_group;
    uint32_t s_inodes_per_group;
    uint32_t s_mtime;
    uint32_t s_wtime;
    uint16_t s_mnt_count;
    uint16_t s_max_mnt_count;
    uint16_t s_magic;
    uint16_t s_state;
    uint16_t s_errors;
    uint16_t s_minor_rev_level;
    uint32_t s_lastcheck;
    uint32_t s_checkinterval;
    uint32_t s_creator_os;
    uint32_t s_rev_level;
    uint16_t s_def_resuid;
    uint16_t s_def_resgid;
    /* -- EXT2_DYNAMIC_REV Specific -- */
    uint32_t s_first_ino;
    uint16_t s_inode_size;
    uint16_t s_block_group_nr;
    uint32_t s_feature_compat;
    uint32_t s_feature_incompat;
    uint32_t s_feature_ro_compat;
    char     s_uuid[16];
    char     s_volume_name[16];
    char     s_last_mounted[64];
    uint32_t s_algo_bitmap;
    /* -- Performance Hints -- */
    uint8_t  s_prealloc_blocks;
    uint8_t  s_prealloc_dir_blocks;
    uint16_t alignment;
    /* -- Journaling Support -- */
    /* 208 16 s_journal_uuid */
    uint32_t s_journal_inum8;
    /* Disk OrganizationOffset (bytes)Size (bytes)Description */
    uint32_t s_journal_dev;
    uint32_t s_last_orphan;
    /* -- Directory Indexing Support -- */
    /* 236 4 x 4s_hash_seed */
    uint8_t  s_def_hash_version;
    /* 253 3 padding - reserved for future expansion */
    /* -- Other options -- */
    uint32_t s_default_mount_options;
    uint32_t s_first_meta_bg;
    /* 264 760 Unused - reserved for future revisions */
};

struct ext2_inode {
    uint16_t i_mode;
    uint16_t i_uid;
    uint32_t i_size;
    uint32_t i_atime;
    uint32_t i_ctime;
    uint32_t i_mtime;
    uint32_t i_dtime;
    uint16_t i_gid;
    uint16_t i_links_count;
    uint32_t i_blocks;
    uint32_t i_flags;
    uint32_t i_osd1;
    uint32_t i_block[15];
    uint32_t i_generation;
    uint32_t i_file_acl;
    uint32_t i_dir_acl;
    uint32_t i_faddr;
    uint32_t i_osd2[3];
};

struct ext2_dir_entry {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
    char *name;
};

typedef struct __fs_ext2 {
    vfs_fs_t *fs;
    struct ext2_superblock *sb;
    uint8_t *inode_bitmap;
    uint32_t inode_table;
    // uint32_t data_start;
    uint32_t bs;
} fs_ext2_t;

vfs_fs_t *ext2_init(device_t *);

ssize_t ext2_read(vfs_fs_t *, vfs_fdstruct *, uint8_t *, size_t);

ssize_t ext2_write(vfs_fs_t *, vfs_fdstruct *, uint8_t *, size_t);

char ext2_inode_exists(fs_ext2_t *, uint32_t);

struct ext2_inode *ext2_inode_info(fs_ext2_t *, uint32_t index);

void ext2_inode_print(struct ext2_inode *);

void ext2_inode_read(fs_ext2_t *, struct ext2_inode *, uint8_t *, off_t seek, size_t size);

struct ext2_dir_entry **ext2_dir_get(fs_ext2_t *, struct ext2_inode *);

void ext2_dir_print(struct ext2_dir_entry **);

uint32_t ext2_file_find(fs_ext2_t *, char *filename);

