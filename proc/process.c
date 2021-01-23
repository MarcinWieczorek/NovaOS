#include <proc/process.h>
#include <kernel/tty.h>

struct proc *proc_create() {
    struct proc *p = malloc(sizeof(struct proc));
    for(int i = 3; i < PROC_FD_MAX; i++) {
        p->fd_list[i].fd = -1;
    }
    return p;
}

void proc_set_tty(struct proc *p, struct tty *t) {
    p->tty = t;
    t->proc_fg = p;
}

struct proc *proc_get() {
    thread_t *t = thread_get();

    // NULL thread is kernel
    if(t == NULL) {
        return NULL;
    }

    return t->proc;
}

