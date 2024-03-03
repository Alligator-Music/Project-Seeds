#include "keybd.h"

#include "libc.h"
#include "libk/libk.h"

extern struct input_stream_t main_input_stream;

char keybd_scantable[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b',   /* Backspace */
    '\t',           /* Tab */
    'q', 'w', 'e', 'r', /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',       /* Enter key */
    0,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   /* 39 */
    '\'', '`',   0,     /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',         /* 49 */
    'm', ',', '.', '/',   0,                    /* Right shift */
    '*',
    0,  /* Alt */
    ' ',    /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    '\b',  /* Left Arrow */
    0,
    0x18,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

bool pressed = false;
bool shift = false;

char keybd_getch() {
    char sc;

    while (true) {
        outb(0x20, 0x20);

        sc = inb(0x60);
        if (sc == 0x2a) {
            shift = true;
            sc = 0x80;
        }
        //else {
        //    shift = false;
        //}

        if (sc & 0x80) {
            // released
            pressed = false;
        } else {
            // pressed
            if (!pressed) {
                pressed = true;
                if (shift) {
                    shift = false;
                    return keybd_scantable[sc] - 0x20;
                }
                else {
                    return keybd_scantable[sc];
                }
            }
        }
    }
}
