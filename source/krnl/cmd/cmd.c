#include "krnl/cmd/cmd.h"

#include "libc/libc.h"

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
    else if (strcmp(str, "")) {
        return 0;
    }
    else {
        return ERR_CMD_UNKNOWN_CMD;
    }
}