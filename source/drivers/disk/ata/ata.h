#pragma once

#define ATA_READ_WITH_RETRY 0x20
#define ATA_WRITE_WITH_RETRY 0x30

void ata_rw_sectors(const unsigned int size_in_sectors, unsigned int starting_sector, unsigned int address, unsigned char command);

void ata_set_sectors(const unsigned int size_in_sectors, unsigned int starting_sector, unsigned char val);
