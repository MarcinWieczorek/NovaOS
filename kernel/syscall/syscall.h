#ifndef _KERNEL_SYSCALL_SYSCALL_H
#define _KERNEL_SYSCALL_SYSCALL_H
#define __NEED_size_t
#define __NEED_ssize_t
#include <bits/alltypes.h>

ssize_t do_write(int fildes, const void *buf, size_t nbyte);

#endif
