#include "io.h"

#include "libc/io/io.h"

#include "libc/libc.h"
#include "drivers/video/textmd/textmd.h"
#include "drivers/keybd/keybd.h"

#include <stdarg.h>

uint8_t current_color = CONSOLE_FOREGROUND_BLUE | CONSOLE_FOREGROUND_RED | CONSOLE_FOREGROUND_GREEN; // basically 0x07
uint16_t current_x = 0;
uint16_t current_y = 0;

struct stdout_cell_t stdout[80 * 24];

int print_str(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '\n') {
            current_y++;
            current_x = 0;
        }
        else if (str[i] == CHAR_DEVICE_CONTROL) { // device control 1
            if (str[i + 1] == DEVICE_CONTROL_COLOR) { // change color
                current_color = str[i + 2];

                i += 2;
            }
            else if (str[i + 1] == DEVICE_CONTROL_CLRSCR) {
                for (int j = 0; j < 80 * 24; j++) {
                    stdout[j].character = 0;
                    stdout[j].attr = 0;
                    stdout[j].color = str[i + 2];
                }
                current_color = str[i + 2];
                current_x = 0;
                current_y = 0;
                i += 2;
            }
        }
        else {
            stdout[current_y * 80 + current_x].character = str[i];
            stdout[current_y * 80 + current_x].color     = current_color;
            stdout[current_y * 80 + current_x].attr      = 0;

            current_x++;

            if (current_x > TEXTMD_WIDTH) {
                current_y++;
                current_x = 0;
            }
        }

        if (current_y == TEXTMD_HEIGHT - 1) {
            // scroll down
            for (int y = 0; y < TEXTMD_HEIGHT - 2; y++) {
                for (int x = 0; x < TEXTMD_WIDTH; x++) {
                    stdout[y * TEXTMD_WIDTH + x].character = stdout[y * TEXTMD_WIDTH + x + TEXTMD_WIDTH].character;
                }
            }

            for (int x = 0; x < TEXTMD_WIDTH; x++) {
                stdout[TEXTMD_WIDTH * (TEXTMD_HEIGHT - 2) + x].character = 0;
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

bool first_time = true;
int flush() {
    if (first_time == true) {
        first_time = false;
        print_str("\x11\x02\x07");  // clear screen
    }

    for (int i = 0; i < 80 * 24; i++) {
        *(char*)(TEXTMD_BUFFER_START + (i * 2)) = stdout[i].character;
        *(char*)(TEXTMD_BUFFER_START + (i * 2) + 1) = stdout[i].color;
    }

    return 0;
}

int cout(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char* tmp = " ";

    for (int i = 0; i < strlen(fmt); i++) {
        if (fmt[i] == '%') {
            if (fmt[i + 1] == 's') {
                print_str(va_arg(args, const char*));
                i++;
            }
            else if (fmt[i + 1] == 'c') {
                tmp[0] = va_arg(args, int);
                print_str(tmp);
                i++;
            }
            else if (fmt[i + 1] == 'x') {
                print_hex(va_arg(args, int));
                i++;
            }
            else if (fmt[i + 1] == 'd') {
                int a = va_arg(args, int);
                print_int(a, 0);
                
                i++;
            }
        }
        else {
            tmp[0] = fmt[i];
            print_str(tmp);
        }
    }

    va_end(args);

    flush();

    return 0;
}

char buffer[KEYBD_BUFFER_SIZE + 1];
const char* cin() {
    memset(buffer, 0, KEYBD_BUFFER_SIZE + 1);
    
    int i = 0;
    while (true) {
        char asc = keybd_getch();

        if (asc == '\n') { // enter
            cout("\n");

            return buffer;
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
            if (i < strlen(buffer)) {
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
                buffer[i] = asc;
                cout("%c", buffer[i]);
            }
        }

        if (i < KEYBD_BUFFER_SIZE) {
            i++;
        }
    }
}
