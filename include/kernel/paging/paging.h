#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#define __NEED_size_t
#include <stdint.h>

typedef struct {
    uint32_t address : 20;
    uint32_t available : 3;
    uint32_t G : 1;
    uint32_t S : 1;
    uint32_t zero : 1;
    uint32_t A : 1;
    uint32_t D : 1;
    uint32_t W : 1;
    uint32_t U : 1;
    uint32_t R : 1;
    uint32_t P : 1;
} paging_pd;

typedef struct {
    uint32_t address : 20;
    uint32_t available : 3;
    uint32_t G : 1;
    uint32_t zero : 1;
    uint32_t D : 1;
    uint32_t A : 1;
    uint32_t C : 1;
    uint32_t W : 1;
    uint32_t U : 1;
    uint32_t R : 1;
    uint32_t P : 1;
} paging_pt;

void paging_enable(paging_pd *);

void paging_identity(paging_pt *pt, uint32_t from, size_t size);

paging_pt *paging_get_pt_addr(paging_pd *);

void paging_map(void *paddr, void *vaddr, uint32_t flags);

#endif
