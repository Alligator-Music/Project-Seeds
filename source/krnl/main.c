//
// Project Seeds Kernel
// void main() is the first thing run by load.bin
//

#include "cpu/int/interrupts.h"
#include "cpu/pic/pic.h"
#include "krnl/syscall/syscall.h"
#include "drivers/video/textmd/textmd.h"
#include "drivers/keybd/keybd.h"
#include "drivers/disk/disk.h"
#include "krnl/mm/memman.h"

#include "libc.h"
#include "libk/libk.h"

#include "krnl/cmd/cli.h"

void main(unsigned int flags, unsigned int drive_num) {
    txtmd_init();                                                      // setup text mode

    flush();                                                           // flush stdout for the first time
    cout("%s", "\x11\x02\x07");                                        // and clear the screen

    cout("Booted from drive %s (0x%x)\n", getdriveid_name(drive_num), drive_num);
    
    cout("Entered 32 bit protected mode\n");                           // print out stuff load.exe did
    cout("Loaded into kernel, addr: 0x%x\n", (unsigned int)main);

    idt_init();                                                        // setup the interrupt descriptor table, so we can have system calls,
    cout("Installed IDT, IDTR addr: 0x%x\n", (unsigned int)&idtr);     // exceptions, timers, keyboard handlers, etc.

    idt_set_descriptor(0x80, syscall_dispatcher, INT_GATE_USER_FLAGS); // set interrupt 0x80 to the system call dispatcher
    cout("Installed syscall at int 0x80, Handler addr: 0x%x\n", (unsigned int)syscall_dispatcher);

    pic_remap(PIC_REMAP_MASTER, PIC_REMAP_SLAVE);                      // remap the pic, so it doesn't conflict with exceptions.
    cout("Remapped PIC by offset 0x20\n");                             // this is done because exceptions (int 0-31) take up the space the pic
                                                                       // does, so we have to move the irq's to an upper space (in this case right after)

    for (int i = 0; i < 15; i++) { irq_set_mask(i); }                  // set IRQ masks for all irq's, so the system doesn't crash for an unhandled one
    cout("Masked IRQ's 0-15\n");                                       // (like the system timer) right when we enable interrupts

    idt_set_descriptor(IRQ_REMAP + IRQ_PS2_KEYBD, keybd_irq, INT_GATE_FLAGS); // set the irq 0x21 (keyboard irq) to our keyboard irq handler
    irq_clear_mask(IRQ_PS2_KEYBD);                                     // clear the irq mask for the keyboard irq so it triggers
    cout("Keyboard IRQ 0x01 setup\n");

    while (inb(0x64) & 1) { inb(PS2_DATA_PORT); }                      // clear ps2 keyboard buffer
    
    sti();                                                             // enable interrupts
    cout("Enabled interrupts\n");

    /*
    // what is this idk its like memory management but i did it wrong and idk how mmap stuff works )):
    uint32_t full_mem_size;
    for (int i = 0; i < *(char*)0x0800; i++) {
        cout("mmap entry length: %d mmap entry type: %d\n", *(uint64_t*)(0x0600 + (i * 24) + 8), *(uint32_t*)(0x0600 + (i * 24) + 16));
        if (*(uint32_t*)(0x0600 + (i * 24) + 16) == 0) {
            full_mem_size += *(uint64_t*)(0x0600 + (i * 24) + 8);
        }
    }
    cout("full memory size: 0x%x or %d", full_mem_size, full_mem_size);
    */

    start_cli();                                                       // start the seeds CLI (Command Line Interface)
    
    halt();                                                            // halt the system incase the CLI exits
    cout("System halted\n");
}
