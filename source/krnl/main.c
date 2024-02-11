// drivers
#include "cpu/int/interrupts.h"
#include "krnl/syscall/syscall.h"
#include "fs/seedfs/seedfs.h"
#include "drivers/video/textmd/textmd.h"
#include "drivers/keybd/keybd.h"
#include "krnl/mm/memman.h"

// libc
#include "libc.h"
#include "libk/libk.h"

// other
#include "krnl/cmd/cli.h"

void main() {
    ////////////////////////
    // initialize drivers //
    ////////////////////////
    txtmd_init();

    if (idt_init() != 0) {
        cout("Error: could not setup IDT");
        halt();
    }

    if (syscall_install() != 0) {
        cout("Error: could not install syscall");
        halt();
    }

    if (seedfs_init() != 0) {
        cout("Error: could not initialize seedfs");
        halt();
    }

    // wow this code looks kinda ugly but thats what box drawing characters are ):
    cout("%sWelcome to Seeds System\n", "\x11\x03\x07");
    cout("\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\n");
    cout("(1) Boot to Seeds CLI\n");

    // make task bar
    cout("%sSeeds Boot Menu%s", "\x11\x02\x02\x11\x11\x03\x70", "\x11\x02\x01");

    while (true) {
        char sc = keybd_getch();
        if (sc == '1') {
            start_cli();
            break;
        }
    }
    
    halt();
}
