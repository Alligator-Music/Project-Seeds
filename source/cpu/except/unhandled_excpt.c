
#include "cpu/int/interrupts.h"

// unhandled exception handler
// has to be unconnected to rest of kernel
__attribute__((noreturn))
void exception_handler(struct int_frame_t* frame) {
    
    // print out error message directly to video memory
    const char* str = "An unhandled exception has occured within the operating system ):\nPress any key to restart...";
    for (int i = 0; i < 80 * 25; i++) {
        *(char*)(0xb8000 + i * 2) = 0x00;
        *(char*)(0xb8001 + i * 2) = 0x40 | 0x07;
    }

    int o = 0;
    for (int i = 0; str[i] != 0; i++) {
        if (str[i] == '\n') {
            o += 79 - (o % 80);
        }
        else {
            *(char*)(0xb8000 + o * 2) = str[i];
        }
        o++;
    }

    // set cursor position
    outb(0x3D4, 0x0F);
	outb(0x3D5, (char) (o & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (char) ((o >> 8) & 0xFF));

    // wait until key is pressed to reboot
    unsigned char alr_pressed = 1;
    while (1) {
        outb(0x20, 0x20);

        if (!(inb(0x60) & 0x80)) {
            if (!alr_pressed) {
                reboot();
            }
        }
        else {
            alr_pressed = 0;
        }
    }

    __asm__ volatile ("cli; hlt");

    while(1);
}
