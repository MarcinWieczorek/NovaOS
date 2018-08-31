global tss_flush

tss_flush:
    CLI
    MOV AX, 0x2B
    LTR AX
    STI
    RET
