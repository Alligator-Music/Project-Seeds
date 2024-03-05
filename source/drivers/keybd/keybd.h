#pragma once

#include "cpu/int/interrupts.h"

void __attribute__((interrupt)) keybd_irq(struct int_frame_t* frame);

char keybd_getch();
