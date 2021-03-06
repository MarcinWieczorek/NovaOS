#ifndef _KERNEL_MM_LAYOUT_H
#define _KERNEL_MM_LAYOUT_H

#define E820_START      0x0000
#define GDT_START       0x0C04
#define GDT_SEG_KERNEL_CODE 0x0000
#define GDT_ADDR_TSS ((void *) 0x0C2C)
#define GDT_DESCRIPTOR  0x0C2C

#define PAGING_DIRECTORY   0x1000
#define PAGING_FIRST_TABLE 0x2000

#define BOOT_START 0x7C00

#define KERNEL_CODE 0x10000

#define THREAD_STACK_START 0x100000
#define THREAD_STACK_SIZE  0x500

#define HEAP_START 0x105000
#define HEAP_END   0xF05000

#endif
