;
; Seeds OS loader
;

%define SECTS_TO_READ 32
%define OS_LOAD_ADDR 0xAE00
%define KERNEL_SECT_START 50

[org 0x7e00]
[bits 16]

; load kernel
mov si, 0                 ; counter for how many attempts at loading there have been

tryload:
    mov ah, 0x02
    mov al, SECTS_TO_READ ; sectors to read
    mov ch, 0             ; cylinder
    mov cl, KERNEL_SECT_START ; sector
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

; get physical memory map
memmap_entries equ 0x0800       ; Store number of memory map entries here
    get_memory_map:
        mov di, 0x0500              ; Memory map entries start here
        xor ebx, ebx                ; EBX = 0 to start, will contain continuation values
        xor bp, bp                  ; BP will store the number of entries
        mov edx, 'PAMS'             ; EDX = 'SMAP' but little endian
        mov eax, 0E820h             ; bios interrupt function number
        mov [ES:DI+20], dword 1     ; Force a valid ACPI 3.x entry
        mov ecx, 24                 ; Each entry can be up to 24 bytes
        int 15h                     ; Call the interrupt
        jc .error                   ; If carry is set, function not supported or errored

        cmp eax, 'PAMS'             ; EAX should equal 'SMAP' on successful call
        jne .error
        test ebx, ebx               ; Does EBX = 0? if so only 1 entry or no entries :(
        jz .error
        jmp .start                  ; EBX != 0, have a valid entry

    .next_entry:
        mov edx, 'PAMS'             ; Some BIOS may clobber edx, reset it here
        mov ecx, 24                 ; Reset ECX
        mov eax, 0E820h             ; Reset EAX
        int 15h

    .start:
        jcxz .skip_entry            ; Memory map entry is 0 bytes in length, skip
        mov ecx, [ES:DI+8]          ; Low 32 bits of length
        or ecx, [ES:DI+12]          ; Or with high 32 bits of length, or will also set the ZF
        jz .skip_entry              ; Length of memory region returned = 0, skip
        
    .good_entry:
        inc bp                      ; Increment number of entries
        add di, 24

    .skip_entry:
        test ebx, ebx               ; If EBX != 0, still have entries to read 
        jz .done
        jmp .next_entry

    .error:
        stc
        jmp error
    .done:
        mov [memmap_entries], bp    ; Store # of memory map entries when done
        clc

; enter protected mode
CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

cli                     ; load gdt
lgdt [GDT_descriptor]

mov eax, cr0            ; tell processor to enable protected mode
or eax, 1
mov cr0, eax

jmp CODE_SEG:clear_pipe ; clear 16 bit instruction pipeline
;; 32 bit section
[bits 32]
clear_pipe:

mov ax, DATA_SEG        ; setup 32 bit segments
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

mov ebp, 0x1000         ; setup 32 bit stack
mov esp, ebp

in al, 0x92             ; enable A20 line
or al, 0x02
out 0x92, al

push 0b00000000000000000000000000000000 ; kernel flags
                                        ;

jmp OS_LOAD_ADDR

[bits 16]
;; end of 32 bit section

GDT_start:      ; GDT (global descriptor table)
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:
GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start

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

msg_error: db 'Could not load kernel', 0

times 8192-($-$$) db 0
