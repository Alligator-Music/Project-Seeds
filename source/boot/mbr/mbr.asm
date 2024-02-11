;
; Seeds Master Boot Record
;

%define SECTS_TO_READ 16
%define OS_LOAD_ADDR 0x7e00

[org 0x7c00]
[bits 16]

; setup segments
mov ax, 0
mov es, ax
mov ds, ax
mov gs, ax

; setup stack
mov bp, 0x1000
mov sp, 0x1000

; load os
mov si, 0                 ; counter for how many attempts at loading there have been

tryload:
    mov ah, 0x02
    mov al, SECTS_TO_READ ; sectors to read
    mov ch, 0             ; cylinder
    mov cl, 2             ; sector
    mov dh, 0             ; head
    mov bx, OS_LOAD_ADDR  ; where to load

    clc                   ; clear carry bit so no fake errors are caught
    int 0x13

    jc errorload          ; jump to errorload if there was an error
    jmp noerrload

errorload:
    inc si                ; increment counter for load attempts
    cmp si, 5
    je error
    jmp tryload

noerrload:                ; there was no error loading (or we got less then 5 errors in a row)

jmp OS_LOAD_ADDR

error:
    mov si, msg_error
    call print
    cli
    hlt

; subroutine for printing text
; si = pointer to txt
print:
    pusha
    mov ah, 0x0e
.loop:
    lodsb
    cmp al, 0
    je .end
    int 0x10
    jmp .loop
.end:
    popa
    ret

msg_error: db 'MBR Error', 0

times 510-($-$$) db 0
dw 0xaa55
