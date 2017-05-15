#include <drivers/screen.h>
#include <kernel/idt.h>

void main() {
    clear_screen();
    idt_install();
}

