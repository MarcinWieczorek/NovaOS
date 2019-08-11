#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <proc/elf/elf32.h>
#include <kernel/thread/thread.h>

struct elf32 *elf32_load(void *data) {
    struct elf32 *elf = malloc(sizeof(struct elf32));
    elf->header = data;
    // Section headers
    struct elf32_section *sh = data + elf->header->e_shoff;
    for(int i = 0; i < elf->header->e_shnum; i++) {
        if(sh->sh_addr > 0) {
            memcpy((void *) sh->sh_addr, data + sh->sh_offset, sh->sh_size);
        }
        sh++;
    }

    return elf;
}

int elf32_execute(struct elf32 *elf) {
    thread_create(thread_get()->proc, (void *) elf->header->e_entry);
    return 0;
}

void elf32_free(struct elf32 *elf) {

}

