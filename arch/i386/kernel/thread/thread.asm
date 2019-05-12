[GLOBAL thread_switch]
[GLOBAL thread_write]
[EXTERN thread_exit]

TSS_ADDRESS EQU 0xC2C

STRUC TCB
    .PID:  RESD 1
    .PPID: RESD 1
    .ESP0: RESD 1
    .ESP:  RESD 1
    .CR3:  RESD 1
    .NAME  RESD 1
ENDSTRUC

; Context switching
; Parameters:
;  ESP + 5*4 (0x14) - old
;  ESP + 6*4 (0x18) - new
thread_switch:
    CLI

    PUSH EBX
    PUSH ESI
    PUSH EDI
    PUSH EBP

    MOV  EDI, [ESP + 0x14]        ; old
    MOV  ESI, [ESP + 0x18]        ; new

    MOV  [EDI+TCB.ESP], ESP       ; Save old stack
    MOV  ESP, [ESI+TCB.ESP]       ; Restore new stack

    POP  EBP
    POP  EDI
    POP  ESI
    POP  EBX

    STI
    RET

; Creating thread
; Parameters:
;  ESP + 0x8 - function
;  ESP + 0xC - thread struct
thread_write:
    PUSH EBP
    MOV  EBP, ESP

    MOV DWORD EDI, [ESP + 0x0C] ; Thread struct

    MOV DWORD EBX, [EDI + TCB.ESP]
    MOV DWORD EAX, [ESP + 0x8]
    MOV DWORD [EBX + 0x14], thread_exit
    MOV DWORD [EBX + 0x10], EAX
    MOV DWORD [EBX + 0x0C], 0
    MOV DWORD [EBX + 0x08], 0
    MOV DWORD [EBX + 0x04], 0
    MOV DWORD [EBX + 0x00], 0

    MOV  ESP, EBP
    POP  EBP
    RET

