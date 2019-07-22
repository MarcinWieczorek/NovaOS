#include <fs/ext2/ext2.h>

vfs_fs_t *ext2_init(device_t *dev) {
    fs_ext2_t *ext2 = malloc(sizeof(fs_ext2_t));
    vfs_fs_t *fs = malloc(sizeof(vfs_fs_t));
    fs->dev = dev;
    fs->name = "ext2";
    fs->read = &ext2_read;
    /* fs->write = &ext2_write; */
    fs->priv = ext2;
    ext2->fs = fs;

    //Read the superblock
    struct ext2_superblock *sb = malloc(1024);
    ext2->sb = sb;
    /* struct ext2_superblock *sb = &ext2->sb; */
    dev->read(dev, (char *) sb, 1024, 1024);
    /* printf("Last mounted %s\n", sb->s_last_mounted); */

    uint32_t bs = 1024 << sb->s_log_block_size;
    ext2->bs = bs;
    uint32_t inode_bitmap_blocks = (sb->s_inode_size * sb->s_inodes_per_group) / bs;
    /* printf("Inode bitmap blocks: %d (0x%X)\n", inode_bitmap_blocks, inode_bitmap_blocks); */

    ext2->inode_bitmap = malloc(bs * inode_bitmap_blocks);
    dev->read(dev, ext2->inode_bitmap, bs * 3, inode_bitmap_blocks);
    ext2->inode_table = bs * (3 + inode_bitmap_blocks);
    /* printf("ipg = 0x%X\n", sb->s_inodes_per_group); */
    /* ext2->data_start = ext2->inode_table + (sb->s_inode_size * sb->s_inodes_per_group); */

    /* printf("First inode: %d (0x%X)\n", sb->s_first_ino, sb->s_first_ino); */
    /* printf("First inode offset: 0x%X\n", */
    /*        ext2->inode_table + sb->s_inode_size * (sb->s_first_ino - 1)); */

#if 0
    for(int i = 12; i < 13; i++) {
        struct ext2_inode *inode = ext2_inode_info(ext2, i);

        if((inode->i_mode & 0x8000) == 0) {
            continue;
        }

        printf("[%d] mode=0x%X\n", i, inode->i_mode);
        ext2_inode_print(inode);
        char buf[1024];
        ext2_inode_read(ext2, inode, buf);
        printf("Content: '%s'\n", buf);
    }
    struct ext2_inode *first_ino = ext2_inode_info(ext2, EXT2_ROOT_INO);
    /* struct ext2_inode *first_ino = ext2_inode_info(ext2, 2); */
    ext2_inode_print(first_ino);
    /* free(first_ino); */
    struct ext2_dir_entry **root_dir = ext2_dir_get(ext2, first_ino);
    ext2_dir_print(root_dir);

    /* printf("Data start: 0x%X\n", ext2->data_start); */
    printf("Find test: 0x%X\n", ext2_file_find(ext2, "/XYZ.txt/"));
    /* uint8_t *root_buf = malloc(1024); */
#endif
    /* ext2_inode_read(ext2, &first_ino, root_buf); */
    /* printf("%X %X %X\n", root_buf[0], root_buf[1], root_buf[2]); */

#if 0
    for(int i = 0; i < 128; i++) {
        printf("%c", ext2_inode_exists(ext2, i) ? 'T' : 'F');

        if(i % 8 == 7) {
            puts("");
        }
    }
    printf("Bitmap done.\n");
#endif

    return fs;
}

ssize_t ext2_read(vfs_fs_t *fs, vfs_fdstruct *fd, uint8_t *buf, size_t n) {
    fs_ext2_t *ext2 = fd->mp->fs->priv;
    uint32_t inode_index = ext2_file_find(ext2, fd->path);
    struct ext2_inode *inode = ext2_inode_info(ext2, inode_index);
    ext2_inode_read(ext2, inode, buf, fd->seek, n);
}

ssize_t ext2_write(vfs_fs_t *fs, vfs_fdstruct *fd, uint8_t *buf, size_t n) {

}

char ext2_inode_exists(fs_ext2_t *ext2, uint32_t index) {
    return !((ext2->inode_bitmap[index / 8] >> (7 - index % 8)) & 1);
}

struct ext2_inode *ext2_inode_info(fs_ext2_t *ext2, uint32_t index) {
    /* printf("[inode_info] 0x%X\n", index); */
    index--;
    struct ext2_inode *inode = malloc(sizeof(struct ext2_inode));
    ext2->fs->dev->read(
            ext2->fs->dev,
            inode,
            ext2->inode_table + ext2->sb->s_inode_size * index,
            ext2->sb->s_inode_size
    );

    return inode;
}

uint32_t ext2_inode_size(fs_ext2_t *ext2, struct ext2_inode *inode) {
    return inode->i_size;
    /* return (inode->i_blocks / (2 << ext2->sb->s_log_block_size)) * ext2->bs; */
}

void ext2_inode_read(fs_ext2_t *ext2, struct ext2_inode *inode, uint8_t *buf,
                     off_t seek, size_t size) {
    off_t offset = ext2->bs * inode->i_block[0] + seek;
    /* printf("[inode_read] inode=0x%X offset=0x%X block=0x%X\n", inode->i_uid, offset, inode->i_block[0]); */
    ext2->fs->dev->read(
            ext2->fs->dev,
            buf,
            offset,
            /* ext2_inode_size(ext2, inode) */
            size
    );
}

void ext2_inode_print(struct ext2_inode *inode) {
    printf("--- Inode Details ---\nMode: 0x%X\nUID: 0x%X\nSize: 0x%X\n"
           "Blocks: %X %X %X %X %X\n",
           inode->i_mode, inode->i_uid, inode->i_size,
           inode->i_block[0],
           inode->i_block[1],
           inode->i_block[2],
           inode->i_block[3],
           inode->i_block[4]
    );
}

struct ext2_dir_entry **ext2_dir_get(fs_ext2_t *ext2, struct ext2_inode *inode) {
    /* printf("[dir_get] %X\n", inode); */
    uint32_t inode_size = ext2_inode_size(ext2, inode);
    /* printf("IS: 0x%X\n", inode_size); */
    uint8_t *buf = malloc(inode_size);
    ext2_inode_read(ext2, inode, buf, 0, inode_size);
    int entries = 6;

    /* struct ext2_dir_entry *array = calloc(entries + 1, sizeof(struct ext2_dir_entry)); */
    struct ext2_dir_entry **array = calloc(entries + 1, sizeof(struct ext2_dir_entry *));

    size_t offset = 0;
    for(int i = 0; i < entries; i++) {
        if(offset >= inode_size || *(uint32_t *) (buf + offset) == 0) {
            array[i] = NULL; // terminate the array
            break;
        }

        array[i] = malloc(sizeof(struct ext2_dir_entry));
        memcpy(array[i], buf + offset, 8);
        array[i]->name = malloc(array[i]->name_len + 1);
        array[i]->name[array[i]->name_len] = 0;
        memcpy(array[i]->name, buf + offset + 8, array[i]->name_len);
        offset += array[i]->rec_len;
    }

    /* array[entries] = NULL; // terminate with null */
    free(buf);
    return array;
}

void ext2_dir_free(struct ext2_dir_entry **dir) {
    for(int i = 0; dir[i]; i++) {
        free(dir[i]);
    }

    free(dir);
}

/* dir - NULL terminated array */
void ext2_dir_print(struct ext2_dir_entry **dir) {
    printf("IN _RL_ FT Name\n");
    for(int i = 0; dir[i]; i++) {
        printf("0%X 0%X 0%X '%s'\n", dir[i]->inode, dir[i]->rec_len, dir[i]->file_type, dir[i]->name);
    }
    puts("----------");
}

uint32_t ext2_file_find_re(fs_ext2_t *ext2, char *filename, uint32_t inode_index) {
    uint32_t result = 0;
    struct ext2_inode *inode = ext2_inode_info(ext2, inode_index);
    struct ext2_dir_entry **dir = ext2_dir_get(ext2, inode);

    char *slash = strchr(filename, '/');

    if(slash == NULL) {
        /* printf("Reached the end.\n"); */
        return inode_index;
    }

    char *filename_copy = malloc(strlen(filename));
    strncpy(filename_copy, filename, slash - filename);

    /* printf("Filename: '%s'\n", filename_copy); */
    /* ext2_dir_print(dir); */

    for(int i = 0; dir[i]; i++) {
        /* printf("CMP: '%s' == '%s'\n", filename_copy, dir[i]->name); */
        if(strcmp(filename_copy, dir[i]->name) == 0) {
            /* printf("FOUND\n"); */
            result = ext2_file_find_re(ext2, slash + 1, dir[i]->inode);
            break;
        }
    }

    free(filename_copy);
    ext2_dir_free(dir);
    free(inode);
    return result;
}

uint32_t ext2_file_find(fs_ext2_t *ext2, char *filename) {
    /* printf("Trying to find: '%s'\n", filename); */
    // get rid of the slash at the beginning
    if(filename[0] == '/') {
        filename++;
    }

    return ext2_file_find_re(ext2, filename, EXT2_ROOT_INO);
}

