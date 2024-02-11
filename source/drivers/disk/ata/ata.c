#include "drivers/disk/ata/ata.h"
#include "libk/libk.h"

void ata_rw_sectors(const unsigned int size_in_sectors, unsigned int starting_sector, unsigned int address, unsigned char command) {
    outb(0x1F6, (0xE0 | ((starting_sector >> 24) & 0x0F))); // Head/drive # port - send head/drive #
    outb(0x1F2, size_in_sectors);                           // Sector count port - # of sectors to read/write
    outb(0x1F3, starting_sector & 0xFF);                    // Sector number port / LBA low bits 0-7
    outb(0x1F4, ((starting_sector >> 8)  & 0xFF));          // Cylinder low port / LBA mid bits 8-15
    outb(0x1F5, ((starting_sector >> 16) & 0xFF));          // Cylinder high port / LBA high bits 16-23
    outb(0x1F7, command);                                   // Command port - send read/write command
                                                            
    unsigned short* address_ptr = (unsigned short*)address;                                                           

    if (command == ATA_READ_WITH_RETRY) {
        for (unsigned char i = size_in_sectors; i > 0; i--) {
            // Poll status port after reading 1 sector
            while (inb(0x1F7) & (1 << 7)) // Wait until BSY bit is clear 
                ;

            // Read 256 words from data port into memory
            for (int j = 0; j < 256; j++)
                *address_ptr++ = inw(0x1F0);

            // 400ns delay - Read alternate status register
            for (char k = 0; k < 4; k++)
                inb(0x3F6);
        }

    } else if (command == ATA_WRITE_WITH_RETRY) {
        for (char i = size_in_sectors; i > 0; i--) {
            // Poll status port after reading 1 sector
            while (inb(0x1F7) & (1 << 7)) // Wait until BSY bit is clear 
                ;

            // Write 256 words from memory to data port
            for (int j = 0; j < 256; j++)
                outw(0x1F0, *address_ptr++);

            // 400ns delay - Read alternate status register
            for (char k = 0; k < 4; k++)
                inb(0x3F6);
        }

        // Send cache flush command after write command is finished
        outb(0x1F7, 0xE7);

        // Wait until BSY bit is clear after cache flush
        while (inb(0x1F7) & (1 << 7));
    }

    // TODO: Handle disk write error for file table here...
    //   Check error ata pio register here...
}

void ata_set_sectors(const unsigned int size_in_sectors, unsigned int starting_sector, unsigned char val) {
    outb(0x1F6, (0xE0 | ((starting_sector >> 24) & 0x0F))); // Head/drive # port - send head/drive #
    outb(0x1F2, size_in_sectors);                           // Sector count port - # of sectors to read/write
    outb(0x1F3, starting_sector & 0xFF);                    // Sector number port / LBA low bits 0-7
    outb(0x1F4, ((starting_sector >> 8)  & 0xFF));          // Cylinder low port / LBA mid bits 8-15
    outb(0x1F5, ((starting_sector >> 16) & 0xFF));          // Cylinder high port / LBA high bits 16-23
    outb(0x1F7, ATA_WRITE_WITH_RETRY);                                   // Command port - send read/write command

    for (char i = size_in_sectors; i > 0; i--) {
        // Poll status port after reading 1 sector
        while (inb(0x1F7) & (1 << 7)) // Wait until BSY bit is clear 
            ;

        // Write 256 words from memory to data port
        for (int j = 0; j < 256; j++)
            outw(0x1F0, val);

        // 400ns delay - Read alternate status register
        for (char k = 0; k < 4; k++)
            inb(0x3F6);
    }

    // Send cache flush command after write command is finished
    outb(0x1F7, 0xE7);

    // Wait until BSY bit is clear after cache flush
    while (inb(0x1F7) & (1 << 7));

    // TODO: Handle disk write error for file table here...
    //   Check error ata pio register here...
}