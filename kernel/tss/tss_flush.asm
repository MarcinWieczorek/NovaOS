global tss_flush

tss_flush:
    CLI
    MOV AX, 0x28
    LTR AX
    STI
    RET
