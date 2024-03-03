#include "krnl/cmd/cli.h"

#include "libc.h"

void start_cli() {
    //cout("Seeds System CLI\n");
    //cout("\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\n");

    cout("\nSeeds System CLI\nType 'help' for help.\n");

    while (true) {
        cout("> ");
        char* input = cin();
        
        int ret = cmd(input);
        if (ret == ERR_CMD_UNKNOWN_CMD) {
            cout("Unknown Command '%s', type 'help' for help\n", input);
        }
    }
}
