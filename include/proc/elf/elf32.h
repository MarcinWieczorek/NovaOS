#ifndef _PROC_ELF_ELF_H
#define _PROC_ELF_ELF_H

#define EI_NIDENT 16

typedef short Elf32_Half;
typedef int Elf32_Word;
typedef int Elf32_Off;
typedef int Elf32_Sword;
typedef int Elf32_Addr;

struct elf32_header {
    unsigned char   e_ident[EI_NIDENT];
    Elf32_Half      e_type;        // 16
    Elf32_Half      e_machine;     // 18
    Elf32_Word      e_version;     // 20
    Elf32_Addr      e_entry;       // 24
    Elf32_Off       e_phoff;       // 28
    Elf32_Off       e_shoff;       // 32
    Elf32_Word      e_flags;       // 36
    Elf32_Half      e_ehsize;      // 40
    Elf32_Half      e_phentsize;   // 42
    Elf32_Half      e_phnum;       // 44
    Elf32_Half      e_shentsize;   // 46
    Elf32_Half      e_shnum;       // 48
    Elf32_Half      e_shtrndx;     // 50
};

struct elf32_section {
    Elf32_Word      sh_name;
    Elf32_Word      sh_type;
    Elf32_Word      sh_flags;
    Elf32_Addr      sh_addr;
    Elf32_Off       sh_offset;
    Elf32_Word      sh_size;
    Elf32_Word      sh_link;
    Elf32_Word      sh_info;
    Elf32_Word      sh_addralign;
    Elf32_Word      sh_entsize;
};

struct elf32_phdr {
    Elf32_Word      p_type;
    Elf32_Off       p_offset;
    Elf32_Addr      p_vaddr;
    Elf32_Addr      p_paddr;
    Elf32_Word      p_filesz;
    Elf32_Word      p_memsz;
    Elf32_Word      p_flags;
    Elf32_Word      p_align;
};

#define ELF32_SHF_WRITE     0x1
#define ELF32_SHF_ALLOC     0x2
#define ELF32_SHF_EXECINSTR 0x4
#define ELF32_SHF_MASKPROC  0xF0000000

#define ELF32_SHT_NULL      0x0
#define ELF32_SHT_PROGBITS  0x1
#define ELF32_SHT_SYMTAB    0x2
#define ELF32_SHT_STRTAB    0x3
#define ELF32_SHT_RELA      0x4
#define ELF32_SHT_HASH      0x5
#define ELF32_SHT_DYNAMIC   0x6
#define ELF32_SHT_NOTE      0x7
#define ELF32_SHT_NOBITS    0x8
#define ELF32_SHT_REL       0x9
#define ELF32_SHT_SHLIB     0xA
#define ELF32_SHT_DYNSYM    0xB
#define ELF32_SHT_LOPROC    0x70000000
#define ELF32_SHT_HIPROC    0x7FFFFFFF
#define ELF32_SHT_LOUSER    0x80000000
#define ELF32_SHT_HIUSER    0xFFFFFFFF

struct elf32 {
    struct elf32_header *header;
};

struct elf32 *elf32_load(void *data);

void elf32_free(struct elf32 *);

int elf32_execute(struct elf32 *);

#endif
