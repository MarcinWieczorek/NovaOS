#include <stdlib.h>

#include <kernel/thread/thread.h>

void thread_init() {
    thread_pool = calloc(THREAD_COUNT_MAX, sizeof(thread_t));
    current_thread_index = 0;

    thread_t *t = &thread_pool[0];
    t->esp0 = 0xA0000;
    t->name = "Kernel Thread #0";
    thread_count = 1;
}

thread_t *thread_create(void *fun) {
    thread_t *t = &thread_pool[current_thread_index++];
    t->esp0 = thread_pool[0].esp0;
    t->esp  = 0xA000 + 20;
    t->name = "Thread";
    thread_count++;
    thread_write(fun, t);
}
