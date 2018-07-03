#include <stdio.h>
#include <unistd.h>

int putchar(int c) {
    return write(0, &c, 1);
}
