#pragma once

// user definitions
#define KEYBD_BUFFER_SIZE 32

#define CONSOLE_FOREGROUND_BLUE   0b00000001
#define CONSOLE_FOREGROUND_GREEN  0b00000010
#define CONSOLE_FOREGROUND_RED    0b00000100
#define CONSOLE_FOREGROUND_BRIGHT 0b00001000

#define CONSOLE_BACKGROUND_BLUE   0b00010000
#define CONSOLE_BACKGROUND_GREEN  0b00100000
#define CONSOLE_BACKGROUND_RED    0b01000000
#define CONSOLE_BACKGROUND_BRIGHT 0b10000000

#define CHAR_DEVICE_CONTROL 0x11
#define CHAR_NEWLINE        0x0a
#define CHAR_BACKSPACE      0x08
#define CHAR_LEFT_ARROW     0x08
#define CHAR_RIGHT_ARROW    0x18

#define DEVICE_CONTROL_COLOR 0x01
#define DEVICE_CONTROL_CLRSCR 0x02

#include "libc/libc.h"

typedef struct __attribute__((packed)) stdout_cell_t {
    unsigned char character;   // character
    unsigned char color;       // color
    unsigned short attr;       // attributes
};

int flush();

int cout(const char* fmt, ...);

const char* cin();

void* file_read(const char* path);

int file_create(const char* path);

int file_count();
