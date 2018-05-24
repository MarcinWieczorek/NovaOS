#include <stdio.h>
#include <unistd.h>

int putchar(int c) {
    write(0, &c, 1);
}
