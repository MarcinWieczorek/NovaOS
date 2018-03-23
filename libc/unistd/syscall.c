#include <unistd.h>

long syscall(long call, ...) {
    __asm__ volatile ("INT $0x80");
    return 0;
}
