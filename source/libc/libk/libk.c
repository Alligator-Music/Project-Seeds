#include  "libc/libk/libk.h"

#include "libc/libc.h"

void reboot() {
    uint8_t good = 0x02;

    while (good & 0x02) {
        good = inb(0x64);
    }

    outb(0x64, 0xFE);

    halt();
}

// pio operations
void outb(unsigned short port, unsigned char val) { 
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port) : "memory"); 
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

void outw(unsigned short port, unsigned short data) { 
    asm volatile ("outw %w0, %w1" : : "a" (data), "Nd" (port)); 
}

unsigned short inw(unsigned short port) {
    unsigned short ret;
    __asm__ volatile("inw %w1, %w0" : "=a" (ret) : "Nd" (port));
    return ret;
}

void io_wait() { 
    outb(0x80, 0);
}

// inline assembly functions
void sti() { __asm__ volatile ("sti"); /* set the interrupt flag */ }

void cli() { __asm__ volatile ("cli"); /* clear the interrupt flag */ }

void halt() { __asm__ volatile ("cli; hlt"); /* halt the computer and clear interrupts so no interrupt unhalts it */ }


