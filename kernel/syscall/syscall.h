#ifndef _KERNEL_SYSCALL_SYSCALL_H
#define _KERNEL_SYSCALL_SYSCALL_H
#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_off_t
#include <bits/alltypes.h>
#include <unistd.h>


ssize_t do_read(int, void *, size_t);

ssize_t do_write(int fildes, const void *buf, size_t nbyte);

int do_open(const char *, int, ...);

int do_execve(const char *, char *const [], char *const []);

off_t do_lseek(int, off_t, int);

#include <time.h>

time_t do_time(time_t *);

int do_brk(void *);

#endif
