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
#define BLACK          0x0
#define BLUE           0x1
#define GREEN          0x2
#define CYAN           0x3
#define RED            0x4
#define MAGENTA        0x5
#define BROWN          0x6
#define GRAY           0x7
#define DARK_GRAY      0x8
#define BRIGHT_BLUE    0x9
#define BRIGHT_GREEN   0xA
#define BRIGHT_CYAN    0xB
#define BRIGHT_RED     0xC
#define BRIGHT_MAGENTA 0xD
#define YELLOW         0xE
#define WHITE          0xF

void print_char_at_attr(char c, int row, int col, char attribute_byte);

void print_char_at(char c, int row, int col);

void print_char(char c);

void print_at_attr(char* string, int row, int col, char attribute_byte);

void print_at(char* string, int row, int col);

void print(char* c);

void print_ln(char* c);

void print_int_at_attr(size_t i, int row, int col, char attribute_byte);

void print_int_at(size_t i, int row, int col);

void print_int(size_t i);

void print_hex_at_attr(int i, int row, int col, char attribute_byte,
                       char uppercase);

void print_hex_at(int i, int row, int col, char uppercase);

void print_hex(int i, char uppercase);

int get_screen_offset(int row, int col);

int get_cursor(void);

void set_cursor_offset(int offset);

void set_cursor(int row, int col);

void clear_screen(void);

void scroll_up(int lines);

void scroll_down(int lines);

char get_char_at(int row, int col);

void set_foreground_color(char color);

void set_background_color(char color);

#endif
