#include <stdio.h>
#include <unistd.h>
#include <string.h>

int puts(const char *str) {
    write(0, str, strlen(str));
}
