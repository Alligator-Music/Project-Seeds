#pragma once

#define TEXTMD_BUFFER_START 0xb8000
#define TEXTMD_HEIGHT 25
#define TEXTMD_WIDTH 80

#define TEXTMD_CUR_UNDERSCORE_START 14
#define TEXTMD_CUR_UNDERSCORE_END   15

typedef struct cell_t {
    unsigned char character;
    unsigned char color;
};

typedef struct cursor_t {
    unsigned char start;
    unsigned char end;
};

void txtmd_cur_enable(struct cursor_t cur_info);

void txtmd_cur_disable();

void txtmd_cur_move(short x, short y);

void txtmd_place_cell(struct cell_t cell, short x, short y);

void txtmd_clear(unsigned char color);

void txtmd_init();
