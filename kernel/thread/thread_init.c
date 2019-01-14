#include <stdlib.h>

#include <kernel/thread/thread.h>
#include <kernel/mm/layout.h>

void thread_init() {
    thread_pool = calloc(THREAD_COUNT_MAX, sizeof(thread_t *));
    current_thread_index = 0;
    stack_space = THREAD_STACK_START;

    stack_space += THREAD_STACK_SIZE;
    thread_t *t = malloc(sizeof(thread_t));
    t->esp0 = stack_space;
    t->name = "Kernel Thread #0";
    thread_count = 1;
    thread_pool[0] = t;
}

thread_t *thread_create(void *fun) {
    thread_t *t = malloc(sizeof(thread_t));

    int i = 0;
    for(; i < THREAD_COUNT_MAX; i++) {
        if(thread_pool[i] == NULL) {
            break;
        }
    }

    stack_space += THREAD_STACK_SIZE;
    thread_pool[i] = t;
    t->esp0 = stack_space;
    t->esp  = stack_space;
    t->name = "Thread";
    thread_count++;
    thread_write(fun, t);
    return t;
}

void thread_exit() {
    thread_remove(thread_get());
    thread_loop();
}

void thread_remove(thread_t *t) {
    uint32_t index = -1;
    for(int i = 0; i < THREAD_COUNT_MAX; i++) {
        if(thread_pool[i] == t) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        return;
    }

    thread_pool[index] = NULL;
    thread_count--;
    free(t);
}

thread_t *thread_get() {
    return thread_pool[current_thread_index];
}

void thread_loop() {
    thread_t *old, *new;

    old = thread_pool[current_thread_index];
    uint32_t new_index = current_thread_index;
    do {
        new_index = (new_index + 1) % thread_count;
    } while(thread_pool[new_index] == NULL);
    new = thread_pool[new_index];

    current_thread_index = new_index;
    thread_switch(old, new);
}
