#pragma once

#define SEEDFS_FT_START_SECTS 17
#define SEEDFS_FT_SIZE_SECTS 32
#define SEEDFS_FT_START_ADDR 0x10000

#define SEEDFS_FILE_FULLNAME_SIZE 11
#define SEEDFS_FILE_NAME_SIZE 8
#define SEEDFS_FILE_EXT_SIZE 3

#define SEEDFS_FILE_ENTRY_SIZE 20

#define SEEDFS_MAX_FILE_ENTRYS 800

#define SEEDFS_FILE_TYPE_NULL    0b00000000
#define SEEDFS_FILE_TYPE_SYS     0b00000001
#define SEEDFS_FILE_TYPE_FILE    0b00000010

#define SEEDFS_FILE_LOAD_ADDR 0x100000

#include "libc.h"

typedef struct seedfs_file_info {
    char* file_name;
    unsigned int size_sects;
    unsigned int start_sect;
    unsigned char type;
};

int seedfs_init();

bool seedfs_exists(char* path);

uint8_t seedfs_filetype(char* path);

uint32_t seedfs_startsect(char* path);

uint32_t seedfs_sizesects(char* path);

void* seedfs_readfile(char* path);

int seedfs_createfile_entry(const char* path, uint32_t size_sects, uint32_t start_sect, uint8_t type);

int seedfs_createfile(const char* path, uint32_t size_sects, uint8_t type);

int seedfs_getfilecount();

int seedfs_deletefile(char* path);
