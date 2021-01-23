#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <kernel/idt/idt.h>
#include <kernel/memory_manager.h>
#include <kernel/mm/layout.h>
#include <kernel/paging/paging.h>
#include <kernel/thread/thread.h>
#include <kernel/tss/tss.h>
#include <kernel/tty.h>

#include <fs/vfs/vfs.h>
#include <fs/devfs/devfs.h>
#include <fs/vfat/vfat.h>
#include <kernel/syscall/syscall.h>

#include <drivers/screen.h>
#include <drivers/pic/pic.h>
#include <dev/ramdev/ramdev.h>

#define MSG_OK(fmt, ...) \
    do { \
        set_foreground_color(VGA_TEXTMODE_COLOR_WHITE); \
        printf("["); \
        set_foreground_color(VGA_TEXTMODE_COLOR_GREEN); \
        printf("OK"); \
        set_foreground_color(VGA_TEXTMODE_COLOR_WHITE); \
        printf("] "); \
        printf(fmt, ##__VA_ARGS__); \
        puts(""); \
    } while(0)

#define MSG_ERR(fmt, ...) \
    do { \
        set_foreground_color(VGA_TEXTMODE_COLOR_WHITE); \
        printf("["); \
        set_foreground_color(VGA_TEXTMODE_COLOR_RED); \
        printf("ERR"); \
        set_foreground_color(VGA_TEXTMODE_COLOR_WHITE); \
        printf("] "); \
        printf(fmt, ##__VA_ARGS__); \
        puts(""); \
    } while(0)

int main(void) {
    clear_screen();

    // Install IDT
    idt_install();
    MSG_OK("IDT initialized");

    // Install TSS
    thread_init();
    tss_install();
    MSG_OK("TSS initialized");

    //Enable Paging
    paging_init();
    MSG_OK("Paging enabled");

    //Enable IRQs
    PIC_clear_mask(0x0);
    PIC_clear_mask(0x6);

    //Create TTY
    struct tty *tty0 = tty_create();
    tty_switch(tty0);

    // Install VFS
    vfs_init();
    MSG_OK("VFS initialized");

    // Init DEVFS
    if(devfs_init()) {
        MSG_OK("DEVFS initialized");
    }
    else {
        MSG_ERR("DEVFS initialization failed");
    }

    // Create ram device
    device_t *ramdev = ramdev_init((void *) 0x20000);
    MSG_OK("RAMDEV initialized");

    //Create VFAT
    vfs_fs_t *vfat = VFAT_init(ramdev);

    if(vfat == NULL) {
        MSG_ERR("VFAT initalization failed");
        return 1;
    }

    MSG_OK("VFAT initialized");
    vfs_mount("/", ramdev, vfat);

    // Create TTY
    struct tty *tty0 = tty_create();
    device_t *tty_dev = tty_dev_init(tty0);
    devfs_node_add("/dev/tty", tty_dev);
    tty_switch(tty0);
    PIC_clear_mask(0x0);
    tty_draw(tty0);

    // Create process
    struct proc *p_init = proc_create();
    proc_set_tty(p_init, tty0);

    // static base because no paging
    uint8_t *proc_base = (void *) 0x08048000;
    uint8_t *buf = proc_base;
    int fd = open("/HWB.O", O_RDONLY);

    if(fd == -1) {
        puts("PANIC: Failed to open init file");
        return 0;
    }

    read(fd, buf, 512);
    thread_create(p_init, (void *) 0x80480B2);
    tty_draw(tty0);
    return 0;
}

