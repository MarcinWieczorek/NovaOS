#include <kernel/pic.h>
#include <kernel/low_level.h>

void PIC_sendEOI(unsigned char irq) {
    if(irq >= 8) {
        out_b(PIC2, PIC_EOI);
        io_wait();
    }

    out_b(PIC1, PIC_EOI);
    io_wait();
}

void PIC_remap(int offset1, int offset2) {
    out_b(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);
    io_wait();
    out_b(PIC1_DATA, offset1);
    io_wait();
    out_b(PIC1_DATA, 0x04);
    io_wait();
    out_b(PIC1_DATA, ICW4_8086);
    io_wait();

    out_b(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);
    io_wait();
    out_b(PIC2_DATA, offset2);
    io_wait();
    out_b(PIC2_DATA, 0x02);
    io_wait();
    out_b(PIC2_DATA, ICW4_8086);
    io_wait();
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
    value = in_b(port) | (1 << IRQline);
    out_b(port, value);
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
    value = in_b(port) & ~(1 << IRQline);
    out_b(port, value);
}
