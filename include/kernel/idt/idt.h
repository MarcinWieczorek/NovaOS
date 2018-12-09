#ifndef _H_KERNEL_IDT
#define _H_KERNEL_IDT

#include <stdlib.h>

struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct isr_regs {
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

unsigned char keyboard_map[128];
char* exception_messages[32];

extern void idt_load(void);
extern void handle_isr_0(void);
extern void handle_isr_1(void);
extern void handle_isr_2(void);
extern void handle_isr_3(void);
extern void handle_isr_4(void);
extern void handle_isr_5(void);
extern void handle_isr_6(void);
extern void handle_isr_7(void);
extern void handle_isr_8(void);
extern void handle_isr_9(void);
extern void handle_isr_A(void);
extern void handle_isr_B(void);
extern void handle_isr_C(void);
extern void handle_isr_D(void);
extern void handle_isr_E(void);
extern void handle_isr_F(void);
extern void handle_isr_10(void);
extern void handle_isr_11(void);
extern void handle_isr_12(void);
extern void handle_isr_13(void);
extern void handle_isr_14(void);
extern void handle_isr_1E(void);
extern void handle_isr_1F(void);
extern void handle_isr_20(void);
extern void handle_isr_21(void);
extern void handle_isr_22(void);
extern void handle_isr_23(void);
extern void handle_isr_24(void);
extern void handle_isr_25(void);
extern void handle_isr_26(void);
extern void handle_isr_27(void);
extern void handle_isr_28(void);
extern void handle_isr_29(void);
extern void handle_isr_2A(void);
extern void handle_isr_2B(void);
extern void handle_isr_2C(void);
extern void handle_isr_2D(void);
extern void handle_isr_2E(void);
extern void handle_isr_2F(void);
extern void handle_isr_80(void);

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

void idt_install(void);

void handle_exception(int exception, int code_size);

long handle_isr(struct isr_regs *r);

#endif
