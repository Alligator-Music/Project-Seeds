#include "krnl/syscall/syscall.h"

#include "cpu/int/interrupts.h"

#include "libc.h"

int syscall_install() {
    idt_set_descriptor(0x80, syscall_dispatcher, INT_GATE_USER_FLAGS);
    return 0;
}

int syscall_func0(char* str) {
    cout("DUMMY SYSCALL");
    
    return 0;
}
