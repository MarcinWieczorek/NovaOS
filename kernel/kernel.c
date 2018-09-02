#include <stdlib.h>

#include <kernel/idt/idt.h>
#include <kernel/tss/tss.h>

#include <drivers/screen.h>

#define MSG_OK(fmt, ...) \
    do { \
        puts("["); \
        set_foreground_color(VGA_TEXTMODE_COLOR_GREEN); \
        puts("OK"); \
        set_foreground_color(VGA_TEXTMODE_COLOR_WHITE); \
        puts("] "); \
        printf(fmt, ##__VA_ARGS__); \
        puts("\n"); \
    } while(0)

int main() {

    clear_screen();
    // Install IDT
    idt_install();
    MSG_OK("Interrupts initialized");

    // Install TSS
    tss_install();
    MSG_OK("TSS initialized");
    return 0;
}

