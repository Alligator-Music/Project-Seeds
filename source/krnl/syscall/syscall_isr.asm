[extern syscall_func]

global syscall_dispatcher

syscall_dispatcher:
    pusha       ; save registers
    call syscall_func
    popa
    ;add esp, 0x4
    iret
