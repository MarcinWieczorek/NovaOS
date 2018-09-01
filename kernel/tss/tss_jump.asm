global tss_jump

tss_jump:
    CLI
    MOV        AX,  0x23        ; User Data segment
    MOV        DS,  AX
    MOV        ES,  AX
    MOV        FS,  AX
    MOV        GS,  AX

    MOV        EAX, ESP
    PUSH DWORD 0x23             ; User data segment
    PUSH       EAX
    PUSHF
    PUSH DWORD 0x1B             ; User code segment
    PUSH DWORD [ESP + 0x14]
    STI
    IRET
    ADD        ESP, 4
    RET

