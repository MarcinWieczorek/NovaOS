#ifndef _ORDERED_LIST_H
#define _ORDERED_LIST_H

#define __NEED_size_t
#include <stdint.h>

typedef struct {
    size_t size;
    size_t max_size;
    uint32_t (*compare) (void *, void *);
    void **array;
} ordered_array;

ordered_array *ordered_array_new(void *, size_t,
                                 uint32_t (*) (void *, void *));

void *ordered_array_get(ordered_array *, size_t);

uint8_t ordered_array_insert(ordered_array *, void *);

void ordered_array_free(ordered_array *);

uint32_t ordered_array_compare_default(void *, void *);

#endif
