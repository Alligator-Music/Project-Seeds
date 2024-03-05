
#include "drivers/disk/disk.h"
#include "libc/libc.h"

char out[4] = "   ";
char* getdriveid_name(unsigned int drive_num) {
    unsigned char drive_num8 = (unsigned char)drive_num;

    // calculate display name for drive id
    if (drive_num8 >= DRIVE_NUM_HD_0 && drive_num8 != DRIVE_NUM_CD_DVD) {
        out[0] = 'h'; out[1] = 'd';
        if (drive_num8 - 0x80 > 8) {
            out[2] = 'x';
        }
        else {
            out[2] = drive_num8 - 0x80 + 0x30;
        }
    }
    else if (drive_num8 >= DRIVE_NUM_FLOPPY_0 && drive_num8 < DRIVE_NUM_HD_0) {
        out[0] = 'f'; out[1] = 'l';
        if (drive_num8 > 8) {
            out[2] = 'x';
        }
        else {
            out[2] = drive_num8 + 0x30;
        }
    }

    return out;
}
