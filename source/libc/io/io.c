#include "io.h"

#include "libc/io/io.h"

#include "libc/libc.h"
#include "drivers/video/textmd/textmd.h"
#include "drivers/keybd/keybd.h"

#include "fs/seedfs/seedfs.h"

#include <stdarg.h>

uint8_t current_color = TEXTMD_CONSOLE_COLOR;
uint16_t current_x = 0;
uint16_t current_y = 0;
uint16_t current_tb_x = 0;
uint8_t current_tb_color = 0x70;
uint8_t current_out = OUT_CONSOLE;

int print_str(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '\n') {
            if (current_out == OUT_CONSOLE) {
                current_y++;
                current_x = 0;
            }
        }
        else if (str[i] == CHAR_DEVICE_CONTROL) { // device control 1
            if (str[i + 1] == DEVICE_CONTROL_COLOR) { // change color
                if (current_out == OUT_CONSOLE) {
                    current_color = str[i + 2];
                }
                else if (current_out == OUT_TASKBAR) {
                    current_tb_color = str[i + 2];
                }
                i += 2;
            }
            else if (str[i + 1] == DEVICE_CONTROL_SETOUT) { // set output stream
                if (str[i + 2] == OUT_CONSOLE) {
                    current_out = OUT_CONSOLE;
                }
                else if (str[i + 2] == OUT_TASKBAR) {
                    current_out = OUT_TASKBAR;
                }
                i += 2;
            }
            else if (str[i + 1] == DEVICE_CONTROL_CLRSCR) {
                if (current_out == OUT_CONSOLE) {
                    for (int j = 0; j < TEXTMD_WIDTH * (TEXTMD_HEIGHT - 1) * 2; j += 2) {
                        *(char*)(TEXTMD_BUFFER_START + j) = 0;
                        *(char*)(TEXTMD_BUFFER_START + j + 1) = str[i + 2];
                    }
                    current_color = str[i + 2];
                    current_x = 0;
                    current_y = 0;
                }
                else if (current_out == OUT_TASKBAR) {
                    for (int j = 0; j < TEXTMD_WIDTH * 2; j += 2) {
                        *(char*)((TEXTMD_BUFFER_START + (TEXTMD_WIDTH * 2 * (TEXTMD_HEIGHT - 1))) + j) = 0;
                        *(char*)((TEXTMD_BUFFER_START + (TEXTMD_WIDTH * 2 * (TEXTMD_HEIGHT - 1))) + j + 1) = str[i + 2];
                    }
                    current_tb_color = str[i + 2];
                    current_tb_x = 0;
                }
                i += 2;
            }
        }
        else {
            if (current_out == OUT_CONSOLE) {
                struct cell_t cell; 
                cell.character = str[i]; 
                cell.color = current_color;

                txtmd_place_cell(cell, current_x, current_y);

                current_x++;

                if (current_x > TEXTMD_WIDTH) {
                    current_y++;
                    current_x = 0;
                }
            }
            else if (current_out == OUT_TASKBAR) {
                struct cell_t cell; 
                cell.character = str[i]; 
                cell.color = current_tb_color;

                txtmd_place_cell(cell, current_tb_x, TEXTMD_HEIGHT - 1);

                current_tb_x++;

                if (current_tb_x == TEXTMD_WIDTH) {
                    current_tb_x = 0;
                }
            }
        }

        if (current_y == TEXTMD_HEIGHT - 1) {
            // scroll down
            for (int i = 0; i < TEXTMD_HEIGHT - 1; i++) {
                memcpy(
                    (char*)(TEXTMD_BUFFER_START + (TEXTMD_WIDTH * 2 * i) + TEXTMD_WIDTH * 2), 
                    (char*)(TEXTMD_BUFFER_START + (TEXTMD_WIDTH * 2 * i)), 
                    TEXTMD_WIDTH * 2); 
            }

            for (int j = 0; j < TEXTMD_WIDTH * 2; j += 2) {
                *(char*)(TEXTMD_BUFFER_START + (TEXTMD_WIDTH * 2 * (TEXTMD_HEIGHT - 2)) + j) = 0;
                *(char*)(TEXTMD_BUFFER_START + (TEXTMD_WIDTH * 2 * (TEXTMD_HEIGHT - 2)) + j + 1) = current_color;
            }

            current_y--;
        }
    }

    txtmd_cur_move(current_x, current_y);

    return 0;
}

void print_hex(uint32_t num) {
    uint8_t write_buffer[32] = { 0 };
    uint32_t len = 0;
    const uint8_t digits[] = "0123456789ABCDEF";
    uint8_t buf[16] = {0};
    int32_t i = 0;
    uint32_t n = num;
    bool pad = (n < 0x10);

    do {
        buf[i++] = digits[n % 16];
    } while ((n /= 16) > 0);

    if (pad) {
        write_buffer[len++] = '0';  // Add padding 0
    }

    while (--i >= 0)  {
        write_buffer[len++] = buf[i];
    }

    print_str(write_buffer);
}

void print_int(int32_t num, bool sign) {
    uint8_t write_buffer[32] = { 0 };
    uint32_t len = 0;
    const uint8_t digits[] = "0123456789ABCDEF";
    bool negative = false;
    uint8_t buf[16] = {0};
    int32_t i = 0;
    int32_t n = num;

    if (sign == true && num < 0) {
        negative = true;
        n = -num;
    }

    do {
        buf[i++] = digits[n % 10];
    } while ((n /= 10) > 0);

    if (negative) buf[i++] = '-';

    while (--i >= 0) {
        write_buffer[len++] = buf[i];
    }

    print_str(write_buffer);
}

int cout(const char* fstr, ...) {
    va_list args;
    va_start(args, fstr);

    char* tmp = " ";

    for (int i = 0; i < strlen(fstr); i++) {
        if (fstr[i] == '%') {
            if (fstr[i + 1] == 's') {
                print_str(va_arg(args, const char*));
                i++;
            }
            else if (fstr[i + 1] == 'c') {
                tmp[0] = va_arg(args, int);
                print_str(tmp);
                i++;
            }
            else if (fstr[i + 1] == 'x') {
                print_hex(va_arg(args, int));
                i++;
            }
            else if (fstr[i + 1] == 'd') {
                int a = va_arg(args, int);
                if (a < 0) {
                    print_int(a, 0);
                }
                else {
                    print_int(a, 1);
                }
                
                i++;
            }
        }
        else {
            tmp[0] = fstr[i];
            print_str(tmp);
        }
    }

    va_end(args);

    return 0;
}

struct cin_ret_t cin() {
    struct cin_ret_t ret;
    memset(ret.buffer, 0, KEYBD_BUFFER_SIZE);
    
    int i = 0;
    while (true) {
        char asc = keybd_getch();
        ret.size = i;

        if (asc == '\n') { // enter
            cout("\n");

            return ret;
        }
        else if (asc == CHAR_BACKSPACE) { // backspace / left arrow
            if (i != 0) {
                if (current_x == 0) {
                    if (current_y != 0) {
                        txtmd_cur_move(current_x, current_y - 1);
                        current_y--;
                    }
                }
                else {
                    txtmd_cur_move(current_x - 1, current_y);
                    current_x--;
                }
            
                if (i != 0) {
                    i -= 2;
                }
            } else {
                i--;
            }
        }
        else if (asc == CHAR_RIGHT_ARROW) { // right arrow
            if (i < strlen(ret.buffer)) {
                if (current_x == TEXTMD_WIDTH) {
                    if (current_y != TEXTMD_HEIGHT) {
                        txtmd_cur_move(current_x, current_y + 1);
                        current_y++;
                    }
                }
                else {
                    txtmd_cur_move(current_x + 1, current_y);
                    current_x++;
                }
            }
            else {
                i--;
            }
        }
        else { // regular key
            if (i < KEYBD_BUFFER_SIZE) {
                ret.buffer[i] = asc;
                char* str = "";
                str[0] = ret.buffer[i];
                str[1] = 0;
                cout(str);
            }
        }

        if (i < KEYBD_BUFFER_SIZE) {
            i++;
        }
    }
}

const char* file_read(const char* path) {
    const char* ret = seedfs_readfile(path);

    return ret;
}

int file_create(const char* path) {
    return seedfs_createfile(path, 1, SEEDFS_FILE_TYPE_FILE);
}

int file_count() {
    return seedfs_getfilecount();
}

