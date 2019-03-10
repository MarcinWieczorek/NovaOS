#include <stdlib.h>
#include <stdio.h>

#include <kernel/idt/idt.h>
#include <kernel/mm/layout.h>
#include <kernel/paging/paging.h>
#include <kernel/thread/thread.h>
#include <kernel/tss/tss.h>

#include <fs/vfs/vfs.h>
#include <fs/devfs/devfs.h>

#include <drivers/screen.h>
#include <drivers/pic/pic.h>

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
    return 0;
}

