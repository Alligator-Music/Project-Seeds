[extern syscall_func0]

global syscall_dispatcher

syscall_dispatcher:
    pusha       ; save register
    push .string
    call syscall_func0
    popa
    add esp, 0x4
    iret
.string: db 'DUMMY SYSCALL', 0
