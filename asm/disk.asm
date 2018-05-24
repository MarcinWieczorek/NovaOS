
disk_load:
    PUSH DX                 ; Save DX for future use
    MOV AH, 0x02            ; BIOS read sector function
    MOV AL, DH              ; Read DH sectors
    MOV CH, 0x00            ; Select cylinder 0
    MOV DH, 0x00            ; Select head 0
    MOV CL, 0x02            ; Start reading from 2nd sector (after boot)
    INT 0x13                ; Interrupt
    JC disk_error           ; Disk error
    POP DX                  ; Restore DX register
    CMP DH, AL              ; sectors read count must equal sectors expected
    JNE disk_error          ; Disk error
    RET

disk_error:
    MOV BX, MSG_DISK_ERROR  ; Print error message
    CALL println
    RET

