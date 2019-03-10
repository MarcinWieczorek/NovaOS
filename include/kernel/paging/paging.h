#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#define __NEED_size_t
#include <stdint.h>

#define PAGING_MASK_PT_C 0x10
#define PAGING_MASK_PT_W 0x08
#define PAGING_MASK_PT_U 0x04
#define PAGING_MASK_PT_R 0x02
#define PAGING_MASK_PT_P 0x01

#define PAGING_UINT32_TYPES 1

#ifdef PAGING_UINT32_TYPES
typedef uint32_t paging_pd;
typedef uint32_t paging_pt;
#else
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
#endif

void paging_init();

void paging_enable(paging_pd *);

void paging_identity(paging_pt *pt, uint32_t from, size_t size);

paging_pt *paging_get_pt_addr(paging_pd *);

void paging_map(void *paddr, void *vaddr, uint32_t flags);

#endif
