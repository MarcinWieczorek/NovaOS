#ifndef _THREAD_THREAD_H
#define _THREAD_THREAD_H
#include <stdint.h>

#define THREAD_COUNT_MAX 16

typedef struct {
    uint32_t esp0;
    uint32_t esp;
    uint32_t cr3;
    char *name;
} thread_t;

thread_t **thread_pool;
uint32_t current_thread_index = 0;
uint32_t thread_count = 0;
uint32_t stack_space;

void thread_init();

void thread_switch(thread_t *, thread_t *);

thread_t *thread_create(void *fun);

void thread_remove(thread_t *);

void thread_write(void *fun, thread_t *);

thread_t *thread_get();

void thread_loop();

#endif

