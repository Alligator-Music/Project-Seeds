#include "keybd.h"

#include "libc.h"
#include "libk/libk.h"

#include "cpu/int/interrupts.h"
#include "cpu/pic/pic.h"

typedef struct key_info_t {
    uint8_t key;
    bool shift;
    bool ctrl;
};

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

const uint8_t *num_row_shifts = ")!@#$%^&*(";

void __attribute__((interrupt)) keybd_irq(struct int_frame_t* frame) {
    
    enum {
        LSHIFT_MAKE  = 0x2A,
        LSHIFT_BREAK = 0xAA,
        RSHIFT_MAKE  = 0x36,
        RSHIFT_BREAK = 0xB6,
        LCTRL_MAKE   = 0x1D,
        LCTRL_BREAK  = 0x9D,
    };

    uint8_t key;
    static bool e0 = false;

    struct key_info_t *key_info = 0x0600;
    key_info->ctrl = false;
    key_info->key = 0;
    key_info->shift = false;

    key = inb(PS2_DATA_PORT);   // Read in new key

    if (key) {
        if      (key == LSHIFT_MAKE  || key == RSHIFT_MAKE) key_info->shift = true; 
        else if (key == LSHIFT_BREAK || key == RSHIFT_BREAK) key_info->shift = false; 
        else if (key == LCTRL_MAKE)  key_info->ctrl = true;
        else if (key == LCTRL_BREAK) key_info->ctrl = false;
        else if (key == 0xE0) e0 = true;
        else {
            if (!(key & 0x80)) {
                // Don't translate escaped scancodes, only return them
                if (!e0) {
                    key = keybd_scantable[key]; 

                    // If pressed shift, translate key to shifted key
                    if (key_info->shift) {
                        if      (key >= 'a' && key <= 'z') key -= 0x20;  // Convert lowercase into uppercase
                        else if (key >= '0' && key <= '9') key = num_row_shifts[key-0x30];  // Get int value of character, offset into shifted nums
                        else {
                            if      (key == '=')  key = '+';
                            else if (key == '\\') key = '|'; 
                            else if (key == '`')  key = '~';
                            else if (key == '[')  key = '{';
                            else if (key == ']')  key = '}';
                            else if (key == '\'') key = '\"';
                            else if (key == ';')  key = ':';
                            else if (key == ',')  key = '<';
                            else if (key == '.')  key = '>';
                            else if (key == '/')  key = '?';
                            else if (key == '-')  key = '_';
                            // TODO: Add more shifted keys here...
                        }
                    }
                }
                key_info->key = key;    // Set ascii key value in struct
                *(uint8_t*)0x0600 = key;
            }
            if (e0) e0 = false;
        }
    }

    pic_sendeoi(IRQ_PS2_KEYBD);
}

char keybd_getch() {
    struct key_info_t* key_info = 0x0600;
    while (!key_info->key) {
        __asm__ volatile ("hlt");
    }

    uint8_t ret = key_info->key;
    key_info->key = 0;

    return ret;
}
