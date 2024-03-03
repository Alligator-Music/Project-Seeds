#include "fs/seedfs/seedfs.h"

#include "drivers/disk/ata/ata.h"
#include "libc/libc.h"

int seedfs_init() {
    // read ft into memory at 0x10000
    ata_rw_sectors(SEEDFS_FT_SIZE_SECTS, SEEDFS_FT_START_SECTS, SEEDFS_FT_START_ADDR, ATA_READ_WITH_RETRY);
    return 0;
}

bool seedfs_exists(char* path) {
    for (int i = 0; i < SEEDFS_MAX_FILE_ENTRYS; i++) {
        if (memcmp(
            (char*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE)),
            path,
            SEEDFS_FILE_FULLNAME_SIZE )) {
                return true;
        }
    }

    return false;
}

uint8_t seedfs_filetype(char* path) {
    if (seedfs_exists(path)) {
        for (int i = 0; i < SEEDFS_MAX_FILE_ENTRYS; i++) {
            if (memcmp(
            (char*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE) - 8),
            path,
            SEEDFS_FILE_FULLNAME_SIZE )) {
                return *(uint8_t*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE) + 11 + 4 + 4);
            }
        }
    }
    else {
        return SEEDFS_FILE_TYPE_NULL;
    }

    return SEEDFS_FILE_TYPE_NULL;
}

uint32_t seedfs_startsect(char* path) {
    if (seedfs_exists(path)) {
        for (int i = 0; i < SEEDFS_MAX_FILE_ENTRYS; i++) {
            if (memcmp(
            (char*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE) - 8),
            path,
            SEEDFS_FILE_FULLNAME_SIZE )) {
                return *(uint32_t*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE) + 11);
            }
        }
    }
    else {
        return 0;
    }

    return 0;
}

uint32_t seedfs_sizesects(char* path) {
    if (seedfs_exists(path)) {
        for (int i = 0; i < SEEDFS_MAX_FILE_ENTRYS; i++) {
            if (memcmp(
            (char*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE) - 8),
            path,
            SEEDFS_FILE_FULLNAME_SIZE )) {
                return *(uint32_t*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE) + 11 + 4);
            }
        }
    }
    else {
        return 0;
    }

    return 0;
}

void* seedfs_readfile(char* path) {
    if (seedfs_exists(path)) {
        ata_rw_sectors(seedfs_sizesects(path), seedfs_startsect(path), SEEDFS_FILE_LOAD_ADDR, ATA_READ_WITH_RETRY);
        return SEEDFS_FILE_LOAD_ADDR;
    }
    else {
        return null;
    }
}

void seedfs_update_ft() {
    ata_rw_sectors(SEEDFS_MAX_FILE_ENTRYS / 512, SEEDFS_FT_START_SECTS, SEEDFS_FT_START_ADDR, ATA_WRITE_WITH_RETRY);
}

int seedfs_createfile_entry(const char* path, uint32_t size_sects, uint32_t start_sect, uint8_t type) {
    if (strlen(path) != SEEDFS_FILE_FULLNAME_SIZE) {
        return 1;
    }

    for (int i = 0; i < SEEDFS_MAX_FILE_ENTRYS; i++) {
        if (*(char*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE)) == 0) {
            memcpy(path, (char*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE)), SEEDFS_FILE_FULLNAME_SIZE);
            *(uint32_t*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE) + 11 + 4) = size_sects;
            *(uint32_t*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE) + 11) = start_sect;
            *(uint8_t*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE) + 11 + 4 + 4) = type;
            break;
        }
    }

    seedfs_update_ft();

    return 0;
}

int seedfs_createfile(const char* path, uint32_t size_sects, uint8_t type) {
    int highest = 0;
    for (int i = 0; i < SEEDFS_MAX_FILE_ENTRYS; i++) {
        if (*(uint32_t*)((SEEDFS_FT_START_ADDR + (i * SEEDFS_FILE_ENTRY_SIZE)) + 11) > highest) {
            highest = *(uint32_t*)((SEEDFS_FT_START_ADDR + (i * SEEDFS_FILE_ENTRY_SIZE)) + 11);
        }
    }

    return seedfs_createfile_entry(path, size_sects, highest, type);
}

int seedfs_getfilecount() {
    for (int i = 0; i < SEEDFS_MAX_FILE_ENTRYS; i++) {
        if (*(char*)((SEEDFS_FT_START_ADDR + (i * SEEDFS_FILE_ENTRY_SIZE))) == 0) {
            return i;
        }
    }
}

// [file][file][file][file][file]
//               ^
//               |
// [file][file]  <-- [file][file]

int seedfs_deletefile(char* path) {
    if (seedfs_exists(path)) {
        for (int i = 0; i < SEEDFS_MAX_FILE_ENTRYS; i++) {
            if (memcmp(
            (char*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE)),
            path,
            SEEDFS_FILE_FULLNAME_SIZE )) {
                cout("start sect: 0x%x\n", seedfs_startsect(path));
                cout("size sects: 0x%x\n", seedfs_sizesects(path));
                ata_set_sectors(seedfs_sizesects(path), seedfs_startsect(path), 0);
                
                memcpy((char*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE) + SEEDFS_FILE_ENTRY_SIZE),
                       (char*)((SEEDFS_FT_START_ADDR + i * SEEDFS_FILE_ENTRY_SIZE)),
                       (SEEDFS_MAX_FILE_ENTRYS * SEEDFS_FILE_ENTRY_SIZE) - SEEDFS_FILE_ENTRY_SIZE * i
                );

                seedfs_update_ft();
            }
        }
    }
    else {
        return 1;
    }
}
