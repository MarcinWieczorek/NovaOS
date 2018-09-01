#ifndef _H_DRIVERS_SCREEN
#define _H_DRIVERS_SCREEN

#include <stdio.h>

#define  VIDEO_ADDRESS 0xB8000
#define  MAX_ROWS  25
#define  MAX_COLS  80
#define  WHITE_ON_BLACK 0x0F
#define  REG_SCREEN_CTRL 0x3D4
#define  REG_SCREEN_DATA 0x3D5

// Colors
#define VGA_TEXTMODE_COLOR_BLACK          0x0
#define VGA_TEXTMODE_COLOR_BLUE           0x1
#define VGA_TEXTMODE_COLOR_GREEN          0x2
#define VGA_TEXTMODE_COLOR_CYAN           0x3
#define VGA_TEXTMODE_COLOR_RED            0x4
#define VGA_TEXTMODE_COLOR_MAGENTA        0x5
#define VGA_TEXTMODE_COLOR_BROWN          0x6
#define VGA_TEXTMODE_COLOR_GRAY           0x7
#define VGA_TEXTMODE_COLOR_DARK_GRAY      0x8
#define VGA_TEXTMODE_COLOR_BRIGHT_BLUE    0x9
#define VGA_TEXTMODE_COLOR_BRIGHT_GREEN   0xA
#define VGA_TEXTMODE_COLOR_BRIGHT_CYAN    0xB
#define VGA_TEXTMODE_COLOR_BRIGHT_RED     0xC
#define VGA_TEXTMODE_COLOR_BRIGHT_MAGENTA 0xD
#define VGA_TEXTMODE_COLOR_YELLOW         0xE
#define VGA_TEXTMODE_COLOR_WHITE          0xF

void print_char_at_attr(char c, int row, int col, char attribute_byte);

void print_char_at(char c, int row, int col);

void print_char(char c);

void print_at_attr(char* string, int row, int col, char attribute_byte);

void print_at(char* string, int row, int col);

void print(char* c);

void print_ln(char* c);

int get_screen_offset(int row, int col);

int get_cursor(void);

void set_cursor_offset(int offset);

void set_cursor(int row, int col);

void shift_cursor(int row, int col);

void clear_screen(void);

void scroll_up(int lines);

void scroll_down(int lines);

char get_char_at(int row, int col);

void set_foreground_color(char color);

void set_background_color(char color);

void set_blinking(int b);

#endif
