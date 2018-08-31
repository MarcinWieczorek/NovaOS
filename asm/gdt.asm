gdt_start:

gdt_null:                ; 0.
    DD 0x0
    DD 0x0

gdt_code:                ; 1. 0xB
    DW 0xFFFF
    DW 0x0
    DB 0x0
    DB 10011010b
    DB 11001111b
    DB 0x0

gdt_data:                ; 2. 0x13
    DW 0xFFFF
    DW 0x0
    DB 0x0
    DB 10010010b
    DB 11001111b
    DB 0x0

gdt_user_code:           ; 3. 0x1B
    DW 0xFFFF
    DW 0
    DB 0
    DB 11111010b
    DB 11001111b
    DB 0

gdt_user_data:           ; 4. 0x23
    DW 0xFFFF
    DW 0
    DB 0
    DB 11110010b
    DB 11001111b
    DB 0

gdt_tss:                 ; 5. 0x2B
    DW 0x0068
    DW 0x0
    DB 0x0
    DB 11101001b
    DB 00000000b
    DB 0x0

gdt_end:

gdt_descriptor:
    DW gdt_end - gdt_start - 1
    DD gdt_start

CODE_SEG EQU gdt_code - gdt_start
DATA_SEG EQU gdt_data - gdt_start
TSS_SEG  EQU gdt_tss - gdt_start

