# Memory Map
         0 +-------------------------+
           |                         |
           |    e820 memory map      |
           |   each entry 24 bytes   |
           |                         |
           |                         |
     0xC00 | Entry count             |
     0xC04 +-------------------------+
           |                         |
           |          GDT            |
           | null entry              |
           | kernel code segment     |
           | kernel data segment     |
           | user code segment       |
           | user data segment       |
           | TSS                     |
     0xC2C | Descriptor              |
           +-------------------------+
           |/////////////////////////|
    0x1000 +-------------------------+
           |                         |
           | Kernel code             |
           |                         |
           +-------------------------+
           |/////////////////////////|
    0x7C00 +-------------------------+ 512 bytes
           |                         |
           | Bootloader              |
           |                         |
           |                         |
           |                         |
           |                         |
           +-------------------------+
           |                         |
           |                         |
           |                         |
           |                         |
           |                         |
           |                         |
           +-------------------------+