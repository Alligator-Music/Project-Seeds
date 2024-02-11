#pragma once

#define KEYBD_BUFFER_SIZE 32

#define CHAR_DEVICE_CONTROL 0x11
#define CHAR_NEWLINE        0x0a
#define CHAR_BACKSPACE      0x08
#define CHAR_LEFT_ARROW     0x08
#define CHAR_RIGHT_ARROW    0x18

#define DEVICE_CONTROL_COLOR 0x01
#define DEVICE_CONTROL_SETOUT 0x02
#define DEVICE_CONTROL_CLRSCR 0x03

#define OUT_CONSOLE 0x01
#define OUT_TASKBAR 0x02

int cout(const char* fstr, ...);

typedef struct cin_ret_t {
    char buffer[256];
    int size;
};

struct cin_ret_t cin();

const char* file_read(const char* path);

int file_create(const char* path);

int file_count();
