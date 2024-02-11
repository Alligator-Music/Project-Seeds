#include "drivers/video/textmd/textmd.h"
#include "libk/libk.h"

void txtmd_cur_enable(struct cursor_t cur_info) {
    outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cur_info.start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cur_info.end);
}

void txtmd_cur_disable() {
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void txtmd_cur_move(short x, short y) {
    short pos = x + y * 80;
    
	if (pos > TEXTMD_WIDTH * TEXTMD_HEIGHT) {
        pos = pos - TEXTMD_WIDTH * TEXTMD_HEIGHT;
	}
	
	outb(0x3D4, 0x0F);
	outb(0x3D5, (char) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (char) ((pos >> 8) & 0xFF));
}

void txtmd_place_cell(struct cell_t cell, short x, short y) {
    short pos = x + (y * 80);

    if (pos > TEXTMD_WIDTH * TEXTMD_HEIGHT) {
        pos = pos - TEXTMD_WIDTH * TEXTMD_HEIGHT;
	}

    *(char*)(TEXTMD_BUFFER_START + ((x + (y * 80)) * 2)) = cell.character;
    *(char*)(TEXTMD_BUFFER_START + ((x + (y * 80)) * 2) + 1) = cell.color;
}

void txtmd_clear(unsigned char color) {
    struct cell_t cell; cell.character = 0; cell.color = color;
    for (int y  = 0; y < TEXTMD_HEIGHT; y++) {
        for (int x = 0; x < TEXTMD_WIDTH; x++) {
            txtmd_place_cell(cell, x, y);
        }
    }
}

void txtmd_init() {
    struct cursor_t cur_info; cur_info.start = TEXTMD_CUR_UNDERSCORE_START; cur_info.end = TEXTMD_CUR_UNDERSCORE_END;
    txtmd_cur_enable(cur_info);
    txtmd_cur_move(0, 0);
    txtmd_clear(TEXTMD_CONSOLE_COLOR);
}
