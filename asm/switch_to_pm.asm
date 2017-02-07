[BITS 16]
switch_to_pm:

    MOV AH, 0x00
    MOV AL, 0x03
    INT 0x10

    CLI
    LGDT [gdt_descriptor]
    MOV EAX, CR0
    OR EAX, 0x1
    MOV CR0, EAX
    JMP CODE_SEG:start_protected_mode

[BITS 32]

start_protected_mode:
    MOV AX, DATA_SEG        ; set old segments
    MOV DS, AX              ; point regsiters to data selector in gdt
    MOV SS, AX
    MOV ES, AX
    MOV FS, AX
    MOV GS, AX

    MOV EBP, 0x90000        ; Stack pointer
    MOV ESP, EBP

    CALL BEGIN_PM

