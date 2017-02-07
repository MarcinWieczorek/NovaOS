[BITS 32]

VIDEO_MEMORY EQU 0xB8000
WHITE_ON_BLACK EQU 0x0F

print_string_pm:
    PUSHA
    MOV EDX, VIDEO_MEMORY
print_string_pm_loop:
    MOV AL, [EBX]
    MOV AH, WHITE_ON_BLACK

    CMP AL, 0
    JE print_string_pm_done

    MOV [EDX], AX
    INC EBX
    ADD EDX, 2
    JMP print_string_pm_loop
print_string_pm_done:
    POPA
    RET
