#include <kernel/pic.h>
#include <sys/io.h>
#include <stdint.h>

void PIC_sendEOI(unsigned char irq) {
    if(irq >= 8) {
        outb(PIC_EOI, PIC2);
    }

    outb(PIC_EOI, PIC1);
}

void PIC_remap(int offset1, int offset2) {
    outb(ICW1_INIT + ICW1_ICW4, PIC1_COMMAND);
    outb(offset1,               PIC1_DATA);
    outb(0x04,                  PIC1_DATA);
    outb(ICW4_8086,             PIC1_DATA);

    outb(ICW1_INIT + ICW1_ICW4, PIC2_COMMAND);
    outb(offset2,               PIC2_DATA);
    outb(0x02,                  PIC2_DATA);
    outb(ICW4_8086,             PIC2_DATA);
}

void IRQ_set_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(value, port);
}

void IRQ_clear_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(value, port);
}
