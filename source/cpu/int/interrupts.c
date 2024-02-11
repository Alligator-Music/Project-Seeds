#include "cpu/int/interrupts.h"

typedef struct {
	unsigned short isr_low;      // The lower 16 bits of the ISR's address
	unsigned short kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	unsigned char reserved;     // Set to zero
	unsigned char attributes;   // Type and attributes; see the IDT page
	unsigned short isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

typedef struct {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

void idt_set_descriptor(unsigned char vector, void* isr, unsigned char flags);
void idt_set_descriptor(unsigned char vector, void* isr, unsigned char flags) {
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low        = (unsigned int)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (unsigned int)isr >> 16;
    descriptor->reserved       = 0;
}

extern void* isr_stub_table[];

int idt_init(void);
int idt_init() {
    idtr.base = (void*)&idt[0];
    idtr.limit = (unsigned short)sizeof(idt_entry_t) * 256 - 1;
 
    for (unsigned char vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], INT_GATE_FLAGS);
        //vectors[vector] = 1;
    }
 
    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    //__asm__ volatile ("sti"); // set the interrupt flag

    return 0;
}
