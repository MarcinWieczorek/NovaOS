#ifndef _KERNEL_MM_E820_H
#define _KERNEL_MM_E820_H

#include <stdint.h>

#define e820_ADDRESS 0x0

typedef struct {
    uint32_t BaseL;   // base address uint64_t
    uint32_t BaseH;
    uint32_t LengthL; // length uint64_t
    uint32_t LengthH;
    uint32_t Type;    // entry Type
    uint32_t ACPI;    // extended

}__attribute__((packed)) e820_entry;

e820_entry* e820_get();

uint32_t e820_count();

#endif
