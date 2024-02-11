#pragma once

#define TEXTMD_BUFFER_START 0xb8000
#define TEXTMD_HEIGHT 25
#define TEXTMD_WIDTH 80

#define TEXTMD_FOREGROUND_BLACK         0x00
#define TEXTMD_FOREGROUND_BLUE          0x01
#define TEXTMD_FOREGROUND_GREEN         0x02
#define TEXTMD_FOREGROUND_CYAN          0x03
#define TEXTMD_FOREGROUND_RED           0x04
#define TEXTMD_FOREGROUND_MAGENTA       0x05
#define TEXTMD_FOREGROUND_BROWN         0x06
#define TEXTMD_FOREGROUND_GRAY          0x07
#define TEXTMD_FOREGROUND_DARK_GRAY     0x08
#define TEXTMD_FOREGROUND_LIGHT_BLUE    0x09
#define TEXTMD_FOREGROUND_LIGHT_GREEN   0x0a
#define TEXTMD_FOREGROUND_LIGHT_CYAN    0x0b
#define TEXTMD_FOREGROUND_LIGHT_RED     0x0c
#define TEXTMD_FOREGROUND_LIGHT_MAGENTA 0x0d
#define TEXTMD_FOREGROUND_YELLOW        0x0e
#define TEXTMD_FOREGROUND_WHITE         0x0f

#define TEXTMD_BACKGROUND_BLACK         0x00
#define TEXTMD_BACKGROUND_BLUE          0x10
#define TEXTMD_BACKGROUND_GREEN         0x20
#define TEXTMD_BACKGROUND_CYAN          0x30
#define TEXTMD_BACKGROUND_RED           0x40
#define TEXTMD_BACKGROUND_MAGENTA       0x50
#define TEXTMD_BACKGROUND_BROWN         0x60
#define TEXTMD_BACKGROUND_GRAY          0x70
#define TEXTMD_BACKGROUND_DARK_GRAY     0x80
#define TEXTMD_BACKGROUND_LIGHT_BLUE    0x90
#define TEXTMD_BACKGROUND_LIGHT_GREEN   0xa0
#define TEXTMD_BACKGROUND_LIGHT_CYAN    0xb0
#define TEXTMD_BACKGROUND_LIGHT_RED     0xc0
#define TEXTMD_BACKGROUND_LIGHT_MAGENTA 0xd0
#define TEXTMD_BACKGROUND_YELLOW        0xe0
#define TEXTMD_BACKGROUND_WHITE         0xf0

#define TEXTMD_CUR_UNDERSCORE_START 14
#define TEXTMD_CUR_UNDERSCORE_END   15

#define TEXTMD_CONSOLE_COLOR TEXTMD_BACKGROUND_BLACK | TEXTMD_FOREGROUND_GRAY

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
