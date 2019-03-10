#include <kernel/paging/paging.h>

void paging_identity(paging_pt *pt, uint32_t from, size_t size) {
    from &= 0xFFFFF000;

    for(; size > 0; from += 4096, size -= 4096, pt++){
        *pt |= PAGING_MASK_PT_P;
    }
}
