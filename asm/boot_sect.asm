[ORG 0x7c00]
MOV [BOOT_DRIVE], DL        ; Store boot drive from DL
MOV BP, 0x9000              ; Set the stack pointer
MOV SP, BP                  ; Set the stack pointer

MOV BX, MSG_REAL_MODE
CALL println                ; Print the message
CALL load_kernel            ; Load the kernel
CALL gdt_copy
CALL switch_to_pm           ; Switch to Protected Mode

%include "print.asm"        ; Includes
%include "disk.asm"
%include "gdt.asm"          ; Global Descriptor Table
%include "print_pm.asm"     ; Protected mode
%include "switch_to_pm.asm"

[BITS 16]
load_kernel:
    MOV BX, 0
    MOV CL, 2               ; Start reading from 2nd sector (after boot)
    MOV DH, 52
    MOV CH, 0x00            ; Select cylinder 0
    PUSH 0x1000
    POP ES
    MOV DL, [BOOT_DRIVE]
    CALL disk_load

    RET

[BITS 32]
BEGIN_PM:
    MOV EBX, MSG_PROT_MODE  ; Display a message about
    CALL print_string_pm    ; the protected mode
    CALL KERNEL_OFFSET      ; Call kernel's main()
    JMP $                   ; Hang.

MSG_REAL_MODE: DB "RM.",0
MSG_PROT_MODE: DB "PM.",0
MSG_DISK_ERROR: DB "drERR",0
BOOT_DRIVE: DB 0            ; Boot drive
KERNEL_OFFSET EQU 0x10000    ; Kernel entry offset

TIMES 510-($-$$) DB 0       ; End of the
DW 0xAA55                   ; boot sector

