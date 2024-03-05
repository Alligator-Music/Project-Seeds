#pragma once

#define DRIVE_NUM_FLOPPY_0 0x00
#define DRIVE_NUM_FLOPPY_1 0x01

#define DRIVE_NUM_HD_0     0x80
#define DRIVE_NUM_HD_1     0x81

#define DRIVE_NUM_CD_DVD   0xE0

char* getdriveid_name(unsigned int drive_num);
