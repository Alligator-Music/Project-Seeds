#pragma once

#define TRAP_GATE_FLAGS     0x8F    // P = 1, DPL = 00, S = 0, Type = 1111 (32bit trap gate)
#define INT_GATE_FLAGS      0x8E    // P = 1, DPL = 00, S = 0, Type = 1110 (32bit interrupt gate)
#define INT_GATE_USER_FLAGS 0xEE    // P = 1, DPL = 11, S = 0, Type = 1110 (32bit interrupt gate, called from PL 3)

typedef struct {
	unsigned short isr_low;      // The lower 16 bits of the ISR's address
	unsigned short kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	unsigned char reserved;     // Set to zero
	unsigned char attributes;   // Type and attributes; see the IDT page
	unsigned short isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

typedef struct {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

void idt_set_descriptor(unsigned char vector, void* isr, unsigned char flags);

int idt_init(void);
