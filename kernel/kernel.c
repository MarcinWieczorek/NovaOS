#include <stdlib.h>
#include <stdio.h>

#include <kernel/idt/idt.h>
#include <kernel/thread/thread.h>
#include <kernel/tss/tss.h>

#include <fs/vfs/vfs.h>

#include <drivers/screen.h>
#include <drivers/pic/pic.h>

#define MSG_OK(fmt, ...) \
    do { \
        set_foreground_color(VGA_TEXTMODE_COLOR_WHITE); \
        puts("["); \
        set_foreground_color(VGA_TEXTMODE_COLOR_GREEN); \
        puts("OK"); \
        set_foreground_color(VGA_TEXTMODE_COLOR_WHITE); \
        puts("] "); \
        printf(fmt, ##__VA_ARGS__); \
        puts("\n"); \
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

    //Enable IRQ0
    PIC_clear_mask(0x0);

    // Install VFS
    vfs_init();
    MSG_OK("VFS initialized");
    return 0;
}

