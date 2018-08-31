#include <drivers/screen.h>
#include <kernel/idt.h>
#include <kernel/tss.h>
int main() {

    clear_screen();
    idt_install();

    // Install TSS
    void *gdt_tss = (void *) 0x7CBB; // Hardcoded address, change me!
    tss_write(gdt_tss);
    tss_flush();
    return 0;
}

