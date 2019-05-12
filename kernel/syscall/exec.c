#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>

#include <proc/elf/elf32.h>

int do_execve(const char *filename, char *const argv[], char *const envp[]) {
    uint8_t *buf = malloc(1500);
    int fd = open(filename, O_RDONLY);
    read(fd, buf, 512 * 3);
    struct elf32 *elf = elf32_load(buf);
    int ret = elf32_execute(elf);
    elf32_free(elf);
    return ret;
}
