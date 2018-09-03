global syscall

syscall:
    PUSH EBP
    MOV EBP, ESP

    INT 0x80

    MOV ESP, EBP
    POP EBP
    RET
