#include <kernel/idt.h>
#include <kernel/pic.h>
#include <string.h>
#include <stdio.h>
#include <sys/io.h>

unsigned char* exception_messages[] = {
        "Division by 0",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Overflow",
        "Bound Range Exceeded",
        "Invalid Opcode",
        "Device Not Available",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Invalid TSS",
        "Segment Not Present",
        "Stack-Segment Fault",
        "General Protection Fault",
        "Page Fault",
        "", //Reserved 15
        "x87 Floating Point",
        "Alignment Check",
        "Machine Check",
        "SIMD Floating-Point",
        "Virtualization Exception",
        "", //Reserved 21
        "", //Reserved 22
        "", //Reserved 23
        "", //Reserved 24
        "", //Reserved 25
        "", //Reserved 26
        "", //Reserved 27
        "", //Reserved 28
        "", //Reserved 29
        "Security"
        "", //Reserved 31
};

void handle_isr(struct isr_regs *r) {
    if(r->int_no <= 32) {
        printf("\n*****************************************************\n");
        printf("Exception #%u\n%s\n", r->int_no, exception_messages[r->int_no]);

        if(r->err_code > 0) {
            printf("Error code: %u\n", r->err_code);
        }

        printf("EIP=%X\n\
GS= %X, FS =   %X, ES =    %X, DS= %X\n\
EDI=%X, ESI =  %X, EBP =   %X, ESP=%X\n\
EAX=%X, EBX =  %X, ECX =   %X, EDX=%X\n\
CS= %X, EFLAGS=%X, USERESP=%X, SS= %X\n\
*****************************************************\n",
            (r->eip - 0x1000),
            r->gs, r->fs, r->es, r->gs,
            r->edi, r->esi, r->ebp, r->esp,
            r->eax, r->ebx, r->ecx, r->edx,
            r->cs, r->eflags, r->useresp, r->ss);
    }
    else {
        printf("IRQ #%u\n", r->int_no);
        PIC_sendEOI(r->int_no);
    }
}

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel,
                  unsigned char flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_hi = (base & 0xFFFF0000) >> 16;
}

void idt_install() {
    int size = sizeof(struct idt_entry);
    idtp.limit = size * 256 - 1;
    idtp.base = (unsigned long) &idt;

    memset(&idt, 0x00, size * 256);
    idt_set_gate(0,  (unsigned) handle_isr0,  0x08, 0x8E);
    idt_set_gate(1,  (unsigned) handle_isr1,  0x08, 0x8E);
    idt_set_gate(2,  (unsigned) handle_isr2,  0x08, 0x8E);
    idt_set_gate(3,  (unsigned) handle_isr3,  0x08, 0x8E);
    idt_set_gate(4,  (unsigned) handle_isr4,  0x08, 0x8E);
    idt_set_gate(5,  (unsigned) handle_isr5,  0x08, 0x8E);
    idt_set_gate(6,  (unsigned) handle_isr6,  0x08, 0x8E);
    idt_set_gate(7,  (unsigned) handle_isr7,  0x08, 0x8E);
    idt_set_gate(8,  (unsigned) handle_isr8,  0x08, 0x8E);
    idt_set_gate(9,  (unsigned) handle_isr9,  0x08, 0x8E);
    idt_set_gate(10, (unsigned) handle_isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned) handle_isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned) handle_isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned) handle_isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned) handle_isr14, 0x08, 0x8E);
    idt_set_gate(16, (unsigned) handle_isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned) handle_isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned) handle_isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned) handle_isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned) handle_isr20, 0x08, 0x8E);
    idt_set_gate(30, (unsigned) handle_isr30, 0x08, 0x8E);

    // Mask interrupts
    PIC_remap(0x20, 0x28);

    idt_set_gate(32, (unsigned) handle_isr32, 0x08, 0x8E);
    idt_set_gate(33, (unsigned) handle_isr33, 0x08, 0x8E);
    idt_set_gate(34, (unsigned) handle_isr34, 0x08, 0x8E);
    idt_set_gate(35, (unsigned) handle_isr35, 0x08, 0x8E);
    idt_set_gate(36, (unsigned) handle_isr36, 0x08, 0x8E);

    idt_load();
    printf("IDT initialized\n");

}

unsigned char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
    '9', '0', '-', '=', '\b',    /* Backspace */
    '\t',            /* Tab */
    'q', 'w', 'e', 'r',    /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',    /* Enter key */
    0,            /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',    /* 39 */
    '\'', '`',   0,        /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
    'm', ',', '.', '/',   0,                /* Right shift */
    '*',
    0,    /* Alt */
    ' ',    /* Space bar */
    0,    /* Caps lock */
    0,    /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,    /* < ... F10 */
    0,    /* 69 - Num lock*/
    0,    /* Scroll Lock */
    0,    /* Home key */
    0,    /* Up Arrow */
    0,    /* Page Up */
    '-',
    0,    /* Left Arrow */
    0,
    0,    /* Right Arrow */
    '+',
    0,    /* 79 - End key*/
    0,    /* Down Arrow */
    0,    /* Page Down */
    0,    /* Insert Key */
    0,    /* Delete Key */
    0,   0,   0,
    0,    /* F11 Key */
    0,    /* F12 Key */
    0,    /* All other keys are undefined */
};

