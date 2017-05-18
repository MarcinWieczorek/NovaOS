[GLOBAL idt_load]
[EXTERN idtp]
[EXTERN handle_isr]         ; Common handler

[GLOBAL handle_isr0]        ; 0-32 exception handlers
[GLOBAL handle_isr1]
[GLOBAL handle_isr2]
[GLOBAL handle_isr3]
[GLOBAL handle_isr4]
[GLOBAL handle_isr5]
[GLOBAL handle_isr6]
[GLOBAL handle_isr7]
[GLOBAL handle_isr8]
[GLOBAL handle_isr9]
[GLOBAL handle_isr10]
[GLOBAL handle_isr11]
[GLOBAL handle_isr12]
[GLOBAL handle_isr13]
[GLOBAL handle_isr14]
[GLOBAL handle_isr15]
[GLOBAL handle_isr16]
[GLOBAL handle_isr17]
[GLOBAL handle_isr18]
[GLOBAL handle_isr19]
[GLOBAL handle_isr20]
[GLOBAL handle_isr30]
[GLOBAL handle_isr31]
[GLOBAL handle_isr32]
[GLOBAL handle_isr33]       ; Keyboard interrupt
[GLOBAL handle_isr34]
[GLOBAL handle_isr35]
[GLOBAL handle_isr36]
[GLOBAL handle_isr37]
[GLOBAL handle_isr38]
[GLOBAL handle_isr39]
[GLOBAL handle_isr40]
[GLOBAL handle_isr41]
[GLOBAL handle_isr42]
[GLOBAL handle_isr43]
[GLOBAL handle_isr44]
[GLOBAL handle_isr45]
[GLOBAL handle_isr46]
[GLOBAL handle_isr47]       ; IRQ15

idt_load:                   ; Init IDT
    LIDT [idtp]
    STI
    RET

handle_isr0:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0
    JMP handle_isr_common
handle_isr1:
    CLI
    PUSH BYTE 0
    PUSH BYTE 1
    JMP handle_isr_common
handle_isr2:
    CLI
    PUSH BYTE 0
    PUSH BYTE 2
    JMP handle_isr_common
handle_isr3:
    CLI
    PUSH BYTE 0
    PUSH BYTE 3
    JMP handle_isr_common
handle_isr4:
    CLI
    PUSH BYTE 0
    PUSH BYTE 4
    JMP handle_isr_common
handle_isr5:
    CLI
    PUSH BYTE 0
    PUSH BYTE 5
    JMP handle_isr_common
handle_isr6:
    CLI
    PUSH BYTE 0
    PUSH BYTE 6
    JMP handle_isr_common
handle_isr7:
    CLI
    PUSH BYTE 0
    PUSH BYTE 7
    JMP handle_isr_common
handle_isr8:
    CLI
    PUSH BYTE 8
    JMP handle_isr_common
handle_isr9:
    CLI
    PUSH BYTE 0
    PUSH BYTE 9
    JMP handle_isr_common
handle_isr10:
    CLI
    PUSH BYTE 10
    JMP handle_isr_common
handle_isr11:
    CLI
    PUSH BYTE 11
    JMP handle_isr_common
handle_isr12:
    CLI
    PUSH BYTE 12
    JMP handle_isr_common
handle_isr13:
    CLI
    PUSH BYTE 13
    JMP handle_isr_common
handle_isr14:
    CLI
    PUSH BYTE 14
    JMP handle_isr_common
handle_isr16:
    CLI
    PUSH BYTE 0
    PUSH BYTE 16
    JMP handle_isr_common
handle_isr17:
    CLI
    PUSH BYTE 17
    JMP handle_isr_common
handle_isr18:
    CLI
    PUSH BYTE 0
    PUSH BYTE 18
    JMP handle_isr_common
handle_isr19:
    CLI
    PUSH BYTE 0
    PUSH BYTE 19
    JMP handle_isr_common
handle_isr20:
    CLI
    PUSH BYTE 0
    PUSH BYTE 20
    JMP handle_isr_common
handle_isr30:
    CLI
    PUSH BYTE 0
    PUSH BYTE 30
    JMP handle_isr_common
handle_isr32:
    CLI
    PUSH BYTE 0
    PUSH BYTE 32
    JMP handle_isr_common
handle_isr33:
    CLI
    PUSH BYTE 0
    PUSH BYTE 33
    JMP handle_isr_common
handle_isr34:
    CLI
    PUSH BYTE 0
    PUSH BYTE 34
    JMP handle_isr_common
handle_isr35:
    CLI
    PUSH BYTE 0
    PUSH BYTE 35
    JMP handle_isr_common
handle_isr36:
    CLI
    PUSH BYTE 0
    PUSH BYTE 36
handle_isr37:
    CLI
    PUSH BYTE 0
    PUSH BYTE 37
handle_isr38:
    CLI
    PUSH BYTE 0
    PUSH BYTE 38
handle_isr39:
    CLI
    PUSH BYTE 0
    PUSH BYTE 39
handle_isr40:
    CLI
    PUSH BYTE 0
    PUSH BYTE 40
handle_isr41:
    CLI
    PUSH BYTE 0
    PUSH BYTE 41
handle_isr42:
    CLI
    PUSH BYTE 0
    PUSH BYTE 42
handle_isr43:
    CLI
    PUSH BYTE 0
    PUSH BYTE 43
handle_isr44:
    CLI
    PUSH BYTE 0
    PUSH BYTE 44
handle_isr45:
    CLI
    PUSH BYTE 0
    PUSH BYTE 45
handle_isr46:
    CLI
    PUSH BYTE 0
    PUSH BYTE 46
handle_isr47:
    CLI
    PUSH BYTE 0
    PUSH BYTE 47
handle_isr_common:          ; Common handler caller
    PUSHA
    PUSH DS
    PUSH ES
    PUSH FS
    PUSH GS
    MOV AX, 0x10
    MOV DS, AX
    MOV ES, AX
    MOV FS, AX
    MOV GS, AX
    MOV EAX, ESP
    PUSH EAX
    MOV EAX, handle_isr
    CALL EAX                ; Call C handler
    POP EAX
    POP GS
    POP FS
    POP ES
    POP DS
    POPA
    ADD ESP, 8
    IRET                    ; Return from interrupt

