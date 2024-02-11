#include "power.h"

#include "libc.h"
#include "libk/libk.h"

void halt() {
    asm("cli; hlt");
}

void reboot() {
    uint8_t good = 0x02;

    while (good & 0x02) {
        good = inb(0x64);
    }

    outb(0x64, 0xFE);

    halt();
}