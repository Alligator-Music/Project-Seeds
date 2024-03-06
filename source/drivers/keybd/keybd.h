#pragma once

#include "cpu/int/interrupts.h"

#define PS2_DATA_PORT 0x60

void __attribute__((interrupt)) keybd_irq(struct int_frame_t* frame);

char keybd_getch();
