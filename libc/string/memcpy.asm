global memcpy
; Memcpy
; Parameters
;  ESP + 0x08 = destination
;  ESP + 0x0C = source
;  ESP + 0x10 = n
;------------------------------------------------------------------------------
memcpy:
    PUSH EBP
    MOV  EBP, ESP

    CMP DWORD [ESP + 0x10], 0
    JE   memcpy_end

    XOR  ECX, ECX
    MOV  EAX, [ESP + 0x08]
    MOV  EBX, [ESP + 0x0C]
memcpy_loop:
    MOV  DL, [EBX]
    MOV  [EAX], DL
    INC  ECX
    INC  EAX
    INC  EBX
    CMP  ECX, [ESP + 0x10]
    JL   memcpy_loop

memcpy_end:
    MOV  ESP, EBP
    POP  EBP
    RET

