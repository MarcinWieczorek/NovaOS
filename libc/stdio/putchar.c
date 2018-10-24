#include <stdio.h>
#include <unistd.h>

int putchar(int c) {
    return write(STDOUT_FILENO, &c, 1);
}
