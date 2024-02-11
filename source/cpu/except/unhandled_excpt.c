#include "drivers/video/textmd/textmd.h"

#include "cpu/int/interrupts.h"

// unhandled exception
// has to be unconnected to rest of kernel
__attribute__((noreturn))
void exception_handler(struct interrupt_frame *frame) {
    const char* str = "An unhandled exception has occured within the operating system ):";
    for (int i = 0; i < TEXTMD_WIDTH * TEXTMD_HEIGHT; i++) {
        *(char*)(0xb8000 + i * 2) = 0x00;
        *(char*)(0xb8000 + i * 2 + 1) = TEXTMD_BACKGROUND_RED | TEXTMD_FOREGROUND_WHITE;
    }

    for (int i = 0; str[i] != 0; i++) {
        *(char*)(0xb8000 + i * 2) = str[i];
    }

    __asm__ volatile ("cli; hlt"); // Completely hangs the computer

    while(1);
}
