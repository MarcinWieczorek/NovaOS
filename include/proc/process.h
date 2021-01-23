#pragma once

#include <kernel/thread/thread.h>
#include <fs/vfs/vfs.h>

#define PROC_FD_MAX 16
struct __vfs_fdstruct;
struct proc {
    int id;
    struct tty *tty;
    struct thread *thread;
    struct __vfs_fdstruct fd_list[PROC_FD_MAX];
};

struct proc *proc_create();

void proc_set_tty(struct proc *, struct tty *);

struct proc *proc_get();

