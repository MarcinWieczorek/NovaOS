; Displays the character in AL
print_char:
    MOV AH, 0x0E
    INT 0x10
    RET

; Displays the string in BX
print_string:
    MOV AL, [BX]
    CALL print_char
    ADD BX, 1
    CMP BYTE [BX], 0x00
    JNE print_string
    print_string_return:
    RET

println:
    CALL print_string
print_newline:
    PUSHA
    MOV AL, 10
    CALL print_char
    MOV AH, 0x03
    MOV BH, 0x00
    INT 0x10
    MOV DL, 0x00
    MOV AH, 0x02
    INT 0x10
    POPA
    RET

