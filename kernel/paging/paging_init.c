#include <kernel/paging/paging.h>
#include <kernel/mm/layout.h>

void paging_init() {
    paging_pd *page_directory = (paging_pd *) PAGING_DIRECTORY;
    paging_pt *fpt = (paging_pt *) PAGING_FIRST_TABLE;

    for(int i = 0; i < 1024; i++) {
        page_directory[i] = 1;
    }

    uint32_t *first_page_table = (uint32_t *) fpt;

    for(int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | 3;
    }

    uint32_t *pd = (uint32_t *) &page_directory[0];
    pd[0] = ((uint32_t) first_page_table) | 3;

    paging_identity(paging_get_pt_addr((paging_pd *) &page_directory[0]), 0, 0xFFFFFFFF);
    paging_enable((paging_pd *) &page_directory[0]);
}
