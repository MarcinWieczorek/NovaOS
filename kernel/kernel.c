#include <drivers/screen.h>
#include <kernel/idt.h>
int main() {

    clear_screen();
    idt_install();
    return 0;
}

