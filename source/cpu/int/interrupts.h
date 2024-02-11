#pragma once

#define TRAP_GATE_FLAGS     0x8F    // P = 1, DPL = 00, S = 0, Type = 1111 (32bit trap gate)
#define INT_GATE_FLAGS      0x8E    // P = 1, DPL = 00, S = 0, Type = 1110 (32bit interrupt gate)
#define INT_GATE_USER_FLAGS 0xEE    // P = 1, DPL = 11, S = 0, Type = 1110 (32bit interrupt gate, called from PL 3)

void idt_set_descriptor(unsigned char vector, void* isr, unsigned char flags);

int idt_init(void);
