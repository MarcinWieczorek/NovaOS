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
unsigned char* exception_messages[32];

extern void idt_load(void);
extern void handle_isr0(void);
extern void handle_isr1(void);
extern void handle_isr2(void);
extern void handle_isr3(void);
extern void handle_isr4(void);
extern void handle_isr5(void);
extern void handle_isr6(void);
extern void handle_isr7(void);
extern void handle_isr8(void);
extern void handle_isr9(void);
extern void handle_isr10(void);
extern void handle_isr11(void);
extern void handle_isr12(void);
extern void handle_isr13(void);
extern void handle_isr14(void);
extern void handle_isr16(void);
extern void handle_isr17(void);
extern void handle_isr18(void);
extern void handle_isr19(void);
extern void handle_isr20(void);
extern void handle_isr30(void);
extern void handle_isr32(void);
extern void handle_isr33(void);
extern void handle_isr34(void);
extern void handle_isr35(void);
extern void handle_isr36(void);
extern void handle_isr37(void);
extern void handle_isr38(void);
extern void handle_isr39(void);
extern void handle_isr40(void);
extern void handle_isr41(void);
extern void handle_isr42(void);
extern void handle_isr43(void);
extern void handle_isr44(void);
extern void handle_isr45(void);
extern void handle_isr46(void);
extern void handle_isr47(void);
extern void handle_isr128(void);

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

void idt_install(void);

void handle_exception(int exception, int code_size);

long handle_isr(struct isr_regs *r);

#endif
