#pragma once

void outb(unsigned short port, unsigned char val);

unsigned char inb(unsigned short port);

void outw(unsigned short port, unsigned short data);

unsigned short inw(unsigned short port);