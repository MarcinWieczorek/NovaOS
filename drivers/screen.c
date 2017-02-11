#include <drivers/screen.h>
#include <kernel/low_level.h>

void print_char_at_attr(char c, int row, int col, char attribute_byte) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;

    if(!attribute_byte) {
        attribute_byte = WHITE_ON_BLACK;
    }

    if(row == -1 && col == -1) {
        int cursor = get_cursor() / 2;
        row = cursor / MAX_COLS;
        col = cursor % MAX_COLS;
    }

    if(c == '\n') {
        set_cursor(row + 1, 0);
        return;
    }

    int offset = get_screen_offset(row, col);
    vidmem[offset] = c;
    vidmem[offset + 1] = attribute_byte;
    set_cursor_offset(offset + 2);
}

void print_char_at(char c, int row, int col) {
    print_char_at_attr(c, row, col, WHITE_ON_BLACK);
}

void print_char(char c) {
    print_char_at(c, -1, -1);
}

void print_at_attr(char* string, int row, int col, char attribute_byte) {
    int i = 0;
    while(string[i] != 0) {
        print_char_at_attr(string[i++], row, col, attribute_byte);

        if(col != -1) {
            col++;
        }
    }
}

void print_at(char* string, int row, int col) {
    print_at_attr(string, row, col, WHITE_ON_BLACK);
}

void print(char* string) {
    print_at(string, -1, -1);
}

void print_int_at_attr(int i, int row, int col, char attribute_byte) {
    char b[128];
    char const digit[] = "0123456789";
    char* p = b;

    if(i < 0) {
        *p++ = '-';
        i *= -1;
    }

    int shifter = i;

    do {
        ++p;
        shifter = shifter / 10;
    }
    while(shifter);

    *p = '\0';
    do {
        *--p = digit[i % 10];
        i = i / 10;
    }
    while(i);

    print_at_attr(b, row, col, attribute_byte);
}

void print_int_at(int i, int row, int col) {
    print_int_at_attr(i, row, col, WHITE_ON_BLACK);
}

void print_int(int i) {
    print_int_at(i, -1, -1);
}

int get_screen_offset(int row, int col) {
    return (row * MAX_COLS + col) * 2;
}

int get_cursor() {
    out_b(REG_SCREEN_CTRL, 0x0E);
    int offset = in_b(REG_SCREEN_DATA) << 8;
    out_b(REG_SCREEN_CTRL, 0x0F);
    offset += in_b(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int offset) {
    offset /= 2;
    out_b(REG_SCREEN_CTRL, 0x0E);
    out_b(REG_SCREEN_DATA, offset >> 8);
    out_b(REG_SCREEN_CTRL, 0x0F);
    out_b(REG_SCREEN_DATA, offset);
}

void set_cursor(int row, int col) {
    set_cursor_offset(get_screen_offset(row, col));
}

void clear_screen() {
    for(int r = 0; r < MAX_ROWS; r = r+1) {
        for(int c = 0; c < MAX_COLS; c = c+1) {
            print_char_at(' ', r, c);
        }
    }

    set_cursor(0, 0);
}

