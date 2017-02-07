; Displays the character in AL
print_char:
    MOV AH, 0x0E
    INT 0x10
    RET

; Displays the string in BX
print_string:
    MOV AL, [BX]
    CALL print_char
    ADD BX, 1
    CMP BYTE [BX], 0x00
    JNE print_string
    print_string_return:
    RET

print_hex:
    pusha                   ; save the register values to the stack for later

    mov cx,4                ; Start the counter: we want to print 4 characters
                            ; 4 bits per char, so we're printing a total of 16 bits

char_loop:
    dec cx                  ; Decrement the counter

    mov ax, dx              ; copy bx into ax so we can mask it for the last chars
    shr dx, 4               ; shift bx 4 bits to the right
    and ax, 0xf             ; mask ah to get the last 4 bits

    mov bx, HEX_OUT         ; set bx to the memory address of our string
    add bx, 2               ; skip the '0x'
    add bx, cx              ; add the current counter to the address

    cmp ax,0xa              ; Check to see if it's a letter or number
    jl set_letter           ; If it's a number, go straight to setting the value
    add byte [bx], 7        ; If it's a letter, add 7
                            ; Why this magic number? ASCII letters start 17
                            ; characters after decimal numbers. We need to cover that
                            ; distance. If our value is a 'letter' it's already
                            ; over 10, so we need to add 7 more.
    jl set_letter

set_letter:
    add byte [bx], al       ; Add the value of the byte to the char at bx

    cmp cx,0                ; check the counter, compare with 0
    je print_hex_done       ; if the counter is 0, finish
    jmp char_loop           ; otherwise, loop again

print_hex_done:
    mov bx, HEX_OUT         ; print the string pointed to by bx
    call print_string
    popa                    ; pop the initial register values back from the stack
    ret                     ; return the function

println:
    CALL print_string
print_newline:
    PUSHA
    MOV AL, 10
    CALL print_char
    MOV AH, 0x03
    MOV BH, 0x00
    INT 0x10
    MOV DL, 0x00
    MOV AH, 0x02
    INT 0x10
    POPA
    RET

