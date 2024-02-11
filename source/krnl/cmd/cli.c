#include "krnl/cmd/cli.h"

#include "libc.h"

void start_cli() {
    // print task bar
    cout("%sSeeds Kernel Mode CLI%s", "\x11\x02\x02\x11\x03\x70", "\x11\x02\x01");

    while (true) {
        cout("seeds> ");
        char* input = cin().buffer;
        int ret = cmd(input);
        if (ret == ERR_CMD_UNKNOWN_CMD) {
            cout("Unknown Command '%s'\n", input);
        }
    }
}
