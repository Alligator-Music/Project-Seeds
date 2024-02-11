#include "pio.h"

void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
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