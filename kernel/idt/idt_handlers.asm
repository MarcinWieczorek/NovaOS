[GLOBAL idt_load]
[EXTERN idtp]
[EXTERN handle_isr]         ; Common handler

[GLOBAL handle_isr_0]        ; 0-32 exception handlers
[GLOBAL handle_isr_1]
[GLOBAL handle_isr_2]
[GLOBAL handle_isr_3]
[GLOBAL handle_isr_4]
[GLOBAL handle_isr_5]
[GLOBAL handle_isr_6]
[GLOBAL handle_isr_7]
[GLOBAL handle_isr_8]
[GLOBAL handle_isr_9]
[GLOBAL handle_isr_A]
[GLOBAL handle_isr_B]
[GLOBAL handle_isr_C]
[GLOBAL handle_isr_D]
[GLOBAL handle_isr_E]
[GLOBAL handle_isr_F]
[GLOBAL handle_isr_10]
[GLOBAL handle_isr_11]
[GLOBAL handle_isr_12]
[GLOBAL handle_isr_13]
[GLOBAL handle_isr_14]
[GLOBAL handle_isr_1E]
[GLOBAL handle_isr_1F]
[GLOBAL handle_isr_20]
[GLOBAL handle_isr_21]       ; Keyboard interrupt
[GLOBAL handle_isr_22]
[GLOBAL handle_isr_23]
[GLOBAL handle_isr_24]
[GLOBAL handle_isr_25]
[GLOBAL handle_isr_26]
[GLOBAL handle_isr_27]
[GLOBAL handle_isr_28]
[GLOBAL handle_isr_29]
[GLOBAL handle_isr_2A]
[GLOBAL handle_isr_2B]
[GLOBAL handle_isr_2C]
[GLOBAL handle_isr_2D]
[GLOBAL handle_isr_2E]
[GLOBAL handle_isr_2F]       ; IRQ15
[GLOBAL handle_isr_80]       ; 0x80 syscall

idt_load:                   ; Init IDT
    LIDT [idtp]
    STI
    RET

handle_isr_0:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0
    JMP handle_isr_common
handle_isr_1:
    CLI
    PUSH BYTE 0
    PUSH BYTE 1
    JMP handle_isr_common
handle_isr_2:
    CLI
    PUSH BYTE 0
    PUSH BYTE 2
    JMP handle_isr_common
handle_isr_3:
    CLI
    PUSH BYTE 0
    PUSH BYTE 3
    JMP handle_isr_common
handle_isr_4:
    CLI
    PUSH BYTE 0
    PUSH BYTE 4
    JMP handle_isr_common
handle_isr_5:
    CLI
    PUSH BYTE 0
    PUSH BYTE 5
    JMP handle_isr_common
handle_isr_6:
    CLI
    PUSH BYTE 0
    PUSH BYTE 6
    JMP handle_isr_common
handle_isr_7:
    CLI
    PUSH BYTE 0
    PUSH BYTE 7
    JMP handle_isr_common
handle_isr_8:
    CLI
    PUSH BYTE 8
    JMP handle_isr_common
handle_isr_9:
    CLI
    PUSH BYTE 0
    PUSH BYTE 9
    JMP handle_isr_common
handle_isr_A:
    CLI
    PUSH BYTE 0xA
    JMP handle_isr_common
handle_isr_B:
    CLI
    PUSH BYTE 0xB
    JMP handle_isr_common
handle_isr_C:
    CLI
    PUSH BYTE 0xC
    JMP handle_isr_common
handle_isr_D:
    CLI
    PUSH BYTE 0xD
    JMP handle_isr_common
handle_isr_E:
    CLI
    PUSH BYTE 0xE
    JMP handle_isr_common
handle_isr_10:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x10
    JMP handle_isr_common
handle_isr_11:
    CLI
    PUSH BYTE 0x11
    JMP handle_isr_common
handle_isr_12:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x12
    JMP handle_isr_common
handle_isr_13:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x13
    JMP handle_isr_common
handle_isr_14:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x14
    JMP handle_isr_common
handle_isr_1E:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x1E
    JMP handle_isr_common
handle_isr_20:
    CLI
    SUB ESP, 4
    PUSH BYTE 0
    PUSH BYTE 0x20
    JMP handle_isr_common
handle_isr_21:
    CLI
    SUB ESP, 4
    PUSH BYTE 0
    PUSH BYTE 0x21
    JMP handle_isr_common
handle_isr_22:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x22
    JMP handle_isr_common
handle_isr_23:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x23
    JMP handle_isr_common
handle_isr_24:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x24
    JMP handle_isr_common
handle_isr_25:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x25
    JMP handle_isr_common
handle_isr_26:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x26
    JMP handle_isr_common
handle_isr_27:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x27
    JMP handle_isr_common
handle_isr_28:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x28
    JMP handle_isr_common
handle_isr_29:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x29
    JMP handle_isr_common
handle_isr_2A:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x2A
    JMP handle_isr_common
handle_isr_2B:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x2B
    JMP handle_isr_common
handle_isr_2C:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x2C
    JMP handle_isr_common
handle_isr_2D:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x2D
    JMP handle_isr_common
handle_isr_2E:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x2E
    JMP handle_isr_common
handle_isr_2F:
    CLI
    PUSH BYTE 0
    PUSH BYTE 0x2F
    JMP handle_isr_common
handle_isr_80:
    CLI
    SUB ESP, 4
    PUSH BYTE 0
    PUSH BYTE 0
    MOV BYTE [ESP], 0x80
    JMP handle_isr_common
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
    MOV [ESP + 0x3C], EAX
    POP EAX
    POP GS
    POP FS
    POP ES
    POP DS
    POPA
    MOV DWORD EAX, [ESP + 8]
    ADD ESP, 4
handle_isr_common_end:
    ADD ESP, 8
    IRET                    ; Return from interrupt

