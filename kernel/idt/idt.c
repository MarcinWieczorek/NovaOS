#define __NEED_off_t

#include <bits/alltypes.h>
#include <bits/syscall.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/io.h>

#include <kernel/idt/idt.h>
#include <kernel/syscall/syscall.h>
#include <kernel/thread/thread.h>
#include <kernel/tty.h>

#include <drivers/pic/pic.h>

char* exception_messages[] = {
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

uint8_t lctrl, lalt;

long handle_isr(struct isr_regs *r) {
    long ret = 0;

    if(r->int_no < 32) {
        printf("\n*****************************************************\n");
        printf("Exception #%u\n%s\n", r->int_no, exception_messages[r->int_no]);

        if(r->err_code > 0) {
            printf("Error code: %u\n", r->err_code);
        }

        printf("EIP=%X\n"
               "GS= %X, FS =   %X, ES =    %X, DS= %X\n"
               "EDI=%X, ESI =  %X, EBP =   %X, ESP=%X\n"
               "EAX=%X, EBX =  %X, ECX =   %X, EDX=%X\n"
               "CS= %X, EFLAGS=%X, USERESP=%X, SS= %X\n"
               "*****************************************************\n",
            (r->eip - 0x1000),
            r->gs, r->fs, r->es, r->gs,
            r->edi, r->esi, r->ebp, r->esp,
            r->eax, r->ebx, r->ecx, r->edx,
            r->cs, r->eflags, r->useresp, r->ss);
        while(1);
    }
    else if(r->int_no == 0x80) {
        // Magic to get syscall's variables
        char *vars = (char *) *((uint32_t *) ((char *) r + 0x18)) + 12;
        short syscall_no = *(vars - 4);

        switch(syscall_no) {
            case SYS_read:
                ret = do_read(*(int *) vars,
                    (void *) *(int *) (vars + sizeof(int)),
                    *(size_t *) (vars + sizeof(int) + sizeof(char *)));
                break;
            case SYS_write:
                ret = do_write(*(int *) vars,
                    (const void *) *(int *) (vars + sizeof(int)),
                    *(size_t *) (vars + sizeof(int) + sizeof(const char *)));
                break;
            case SYS_open:
                ret = do_open(*(const char **) vars,
                    *(int *) (vars + sizeof(const char)));
                break;
            case SYS_time:
                ret = do_time((time_t *) *(int *) vars);
                break;
            case SYS_brk:
                ret = do_brk((void *) *(int *) vars);
                break;
            case SYS_lseek:
                ret = do_lseek(*(unsigned int *) vars,
                    *(off_t *) (vars + sizeof(int)),
                    *(unsigned int *) (vars + sizeof(int) + sizeof(off_t *)));
                break;
        }
    }
    else {
        if(r->int_no == 0x20) { //Timer IRQ
            thread_loop();
        }
        else if(r->int_no == 0x21) {
            unsigned char scancode;
            scancode = inb(0x60);

            switch(scancode) {
                case 0x1D: // Left CTRL
                case 0x9D:
                    lctrl = !(scancode & 0x80);
                    break;
                case 0x38: // Left ALT
                case 0xB8:
                    lalt = !(scancode & 0x80);
                    break;
                case 0x4B: // Left arrow
                    if(lctrl && lalt) {
                        struct tty *tty = tty_get_current();
                        struct tty *prev = tty_get_prev(tty);

                        if(prev != NULL && prev != tty) {
                            tty_switch(prev);
                        }
                    }
                    break;
                case 0x4D: // Right arrow
                    if(lctrl && lalt) {
                        struct tty *tty = tty_get_current();
                        struct tty *next = tty_get_next(tty);

                        if(next != NULL && next != tty) {
                            tty_switch(next);
                        }
                    }
                    break;
                default:
            }
        }
        else {
            printf("\nIRQ #%i", r->int_no);
        }
    }

    PIC_send_EOI(r->int_no);
    return ret;
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
    idt_set_gate(0x00, (unsigned) handle_isr_0,  0x08, 0x8E);
    idt_set_gate(0x01, (unsigned) handle_isr_1,  0x08, 0x8E);
    idt_set_gate(0x02, (unsigned) handle_isr_2,  0x08, 0x8E);
    idt_set_gate(0x03, (unsigned) handle_isr_3,  0x08, 0x8E);
    idt_set_gate(0x04, (unsigned) handle_isr_4,  0x08, 0x8E);
    idt_set_gate(0x05, (unsigned) handle_isr_5,  0x08, 0x8E);
    idt_set_gate(0x06, (unsigned) handle_isr_6,  0x08, 0x8E);
    idt_set_gate(0x07, (unsigned) handle_isr_7,  0x08, 0x8E);
    idt_set_gate(0x08, (unsigned) handle_isr_8,  0x08, 0x8E);
    idt_set_gate(0x09, (unsigned) handle_isr_9,  0x08, 0x8E);
    idt_set_gate(0x0A, (unsigned) handle_isr_A,  0x08, 0x8E);
    idt_set_gate(0x0B, (unsigned) handle_isr_B,  0x08, 0x8E);
    idt_set_gate(0x0C, (unsigned) handle_isr_C,  0x08, 0x8E);
    idt_set_gate(0x0D, (unsigned) handle_isr_D,  0x08, 0x8E);
    idt_set_gate(0x0E, (unsigned) handle_isr_E,  0x08, 0x8E);
    idt_set_gate(0x10, (unsigned) handle_isr_10, 0x08, 0x8E);
    idt_set_gate(0x11, (unsigned) handle_isr_11, 0x08, 0x8E);
    idt_set_gate(0x12, (unsigned) handle_isr_12, 0x08, 0x8E);
    idt_set_gate(0x13, (unsigned) handle_isr_13, 0x08, 0x8E);
    idt_set_gate(0x14, (unsigned) handle_isr_14, 0x08, 0x8E);
    idt_set_gate(0x1E, (unsigned) handle_isr_1E, 0x08, 0x8E);

    // Mask interrupts
    PIC_remap(0x20, 0x28);

    idt_set_gate(0x20, (unsigned) handle_isr_20, 0x08, 0x8E);
    idt_set_gate(0x21, (unsigned) handle_isr_21, 0x08, 0x8E);
    idt_set_gate(0x22, (unsigned) handle_isr_22, 0x08, 0x8E);
    idt_set_gate(0x23, (unsigned) handle_isr_23, 0x08, 0x8E);
    idt_set_gate(0x24, (unsigned) handle_isr_24, 0x08, 0x8E);
    idt_set_gate(0x25, (unsigned) handle_isr_25, 0x08, 0x8E);
    idt_set_gate(0x26, (unsigned) handle_isr_26, 0x08, 0x8E);
    idt_set_gate(0x27, (unsigned) handle_isr_27, 0x08, 0x8E);
    idt_set_gate(0x28, (unsigned) handle_isr_28, 0x08, 0x8E);
    idt_set_gate(0x29, (unsigned) handle_isr_29, 0x08, 0x8E);
    idt_set_gate(0x2A, (unsigned) handle_isr_2A, 0x08, 0x8E);
    idt_set_gate(0x2B, (unsigned) handle_isr_2B, 0x08, 0x8E);
    idt_set_gate(0x2C, (unsigned) handle_isr_2C, 0x08, 0x8E);
    idt_set_gate(0x2D, (unsigned) handle_isr_2D, 0x08, 0x8E);
    idt_set_gate(0x2E, (unsigned) handle_isr_2E, 0x08, 0x8E);
    idt_set_gate(0x2F, (unsigned) handle_isr_2F, 0x08, 0x8E);

    // Syscall
    idt_set_gate(0x80, (unsigned) handle_isr_80, 0x08, 0xEE);

    idt_load();
}

unsigned char keyboard_map[128] = {
    0,
    27,
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',    /* 9 */
    '9',
    '0',
    '-',
    '=',
    '\b',   /* Backspace */
    '\t',   /* Tab */
    'q',
    'w' ,
    'e',
    'r',    /* 19 */
    't',
    'y',
    'u',
    'i',
    'o',
    'p',
    '[',
    ']',
    '\n',   /* Enter key */
    0,      /* 29   - Control */
    'a',
    's',
    'd',
    'f',
    'g',
    'h',
    'j',
    'k',
    'l',
    ';',    /* 39 */
    '\'',
    '`',
    0,      /* Left shift */
    '\\',
    'z',
    'x',
    'c',
    'v',
    'b',
    'n',    /* 49 */
    'm',
    ',',
    '.',
    '/',
    0,      /* Right shift */
    '*',
    0,      /* Alt */
    ' ',    /* Space bar */
    0,      /* Caps lock */
    0,      /* 59 - F1 key ... > */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,      /* < ... F10 */
    0,      /* 69 - Num lock*/
    0,      /* Scroll Lock */
    0,      /* Home key */
    0,      /* Up Arrow */
    0,      /* Page Up */
    '-',
    0,      /* Left Arrow */
    0,
    0,      /* Right Arrow */
    '+',
    0,      /* 79 - End key*/
    0,      /* Down Arrow */
    0,      /* Page Down */
    0,      /* Insert Key */
    0,      /* Delete Key */
    0,
    0,
    0,
    0,      /* F11 Key */
    0,      /* F12 Key */
    0,      /* All other keys are undefined */
};

