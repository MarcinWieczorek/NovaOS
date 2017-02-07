gdt_start:

gdt_null:
    DD 0x0
    DD 0x0

gdt_code:
    DW 0xFFFF
    DW 0x0
    DB 0x0
    DB 10011010b
    DB 11001111b
    DB 0x0

gdt_data:
    DW 0xFFFF
    DW 0x0
    DB 0x0
    DB 10010010b
    DB 11001111b
    DB 0x0

gdt_end:

gdt_descriptor:
    DW gdt_end - gdt_start - 1
    DD gdt_start

CODE_SEG EQU gdt_code - gdt_start
DATA_SEG EQU gdt_data - gdt_start

