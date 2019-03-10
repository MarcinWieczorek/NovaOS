global paging_enable

; Enable Paging
; Parameters:
; EBP + 0x08 - Paging directory pointer
paging_enable:
    PUSH EBP
    MOV EBP, ESP

    ; load page directory
    MOV EAX, [EBP + 0x08]
    MOV CR3, EAX

    ; enable paging
    MOV EAX, CR0
    OR  EAX, 0x80000000
    MOV CR0, EAX

    MOV ESP, EBP
    POP EBP
    RET
