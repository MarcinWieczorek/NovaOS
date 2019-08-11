#include <proc/process.h>

struct proc *proc_create() {
    return malloc(sizeof(struct proc));
}

