#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

FILE *fopen(const char *pathname, const char *mode) {
    int fd = open(pathname, O_RDWR);
    return fdopen(fd, mode);
}
