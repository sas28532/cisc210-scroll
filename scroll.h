#ifndef SCROLL_H
#define SCROLL_H

#include <stdint.h>

void open_display(void);
void close_display(void);
void clear_display(void);
void display_letter(char letter, int xOffset, int yOffset);

void open_input(void);
void close_input(void);
void check_input(void (*handler)(unsigned int), int delay_ms);

void set_display_rotation(int r);
void set_display_color(uint16_t c);

#endif

