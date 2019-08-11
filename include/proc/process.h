#pragma once

#include <kernel/thread/thread.h>

struct proc {
    int id;
    struct tty *tty;
    struct thread *thread;
};

struct proc *proc_create();

void proc_set_tty(struct proc *, struct tty *);

