
; sample program

[org 0x100000]
[bits 32]

mov byte [0xb8000], '9'

ret

times 512-($-$$) db 0
