#include "krnl/cmd/cmd.h"

#include "libc/libc.h"

#include "drivers/disk/ata/ata.h"

int cmd(const char* str) {
    if (strcmp("help", str)) {
        cout("ver - displays the version of seeds\n");
        cout("help - displays this message\n");
        cout("reboot - reboots the machine\n");
        cout("fstest - tests filesystem\n");
        cout("excpt - trigger a div_by_0 exception in the system\n");
        return 0;
    }
    else if (strcmp("ver", str)) {
        cout("Seeds v1.0 x86_32\n");
        return 0;
    }
    else if (strcmp("reboot", str)) {
        reboot();
        return 0;
    }
    else if (strcmp("fstest", str)) {
        cout("Unimplemented file system.\n");
        return 0;
    }
    else if (strcmp("excpt", str)) {
        __asm__ volatile ("int $0x00");
    }
    else if (strcmp(str, "")) {
        return 0;
    }
    else {
        return ERR_CMD_UNKNOWN_CMD;
    }
}
