#include "krnl/cmd/cmd.h"

#include "libc/libc.h"

#include "drivers/disk/ata/ata.h"

int cmd(const char* str) {
    if (strcmp("ver", str)) {
        cout("Seeds v1.0 x86_32\n");
        return 0;
    }
    else if (strcmp("help", str)) {
        cout("ver - displays the version of seeds\n");
        cout("help - displays this message\n");
        cout("reboot - reboots the machine\n");
        cout("count - get file count\n");
        cout("fstest - tests filesystem\n");
        cout("prog - run sample program\n");
        cout("excpt - trigger a div_by_0 exception in the system\n");
        return 0;
    }
    else if (strcmp("reboot", str)) {
        reboot();
        return 0;
    }
    else if (strcmp("count", str)) {
        cout("%d\n", file_count());
        return 0;
    }
    else if (strcmp("fstest", str)) {
        if (seedfs_exists("sdkrnl  bin") && seedfs_exists("load    bin")) {
            cout("sdkrnl.bin and load.bin exist!\n");
        }
        if (file_create("TESTFILE$$$") == 0) {
            cout("TESTFILE$$$ created!\n");
        }
        cout("file count (should be 2 + files added rn): %d\n", file_count());
        
        if (seedfs_deletefile("TESTFILE$$$") == 0) {
            cout("TESTFILE$$$ was deleted!\n");
        }
        cout("file count (should be 2): %d\n", file_count());
        return 0;
    }
    else if (strcmp("prog", str)) {
        typedef void func(void);
        void* addr = file_read("prog    bin");
        
        if (addr == null) {
            cout("Error: prog.bin does not exist!\n");
        }
        else {
            cout("Running at: 0x%x\n", (int)addr);
            cout("Contents: ");
            for (int i = 0; i < 2048; i++) {
                cout("%c", *(char*)(addr + i));
            }
            
            func* f = (func*)addr;
            //f();
        }
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
