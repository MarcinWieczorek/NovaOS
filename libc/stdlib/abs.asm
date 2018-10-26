global abs

$abs:
    PUSH EBP
    MOV EBP, ESP

    MOV EAX, [EBP + 0x8]
    MOV EBX, EAX
    SAR EBX, 31
    XOR EAX, EBX
    SUB EAX, EBX

    MOV ESP, EBP
    POP EBP
    RET
