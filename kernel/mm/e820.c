#include <kernel/mm/e820.h>

e820_entry* e820_get() {
    return (e820_entry *) e820_ADDRESS;
}

uint32_t e820_count() {
    return *((uint32_t *) 0xC00);
}
