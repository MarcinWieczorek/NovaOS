#include <drivers/screen.h>
#include <kernel/low_level.h>
#include <stdarg.h>
#include <string.h>

char attribute = WHITE_ON_BLACK;

void print_char_at_attr(char c, int row, int col, char attribute_byte) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;

    if(!attribute_byte) {
        attribute_byte = attribute;
    }

    if(row == -1 && col == -1) {
        int cursor = get_cursor() / 2;
        row = cursor / MAX_COLS;
        col = cursor % MAX_COLS;
    }

    if(row >= MAX_ROWS) {
        scroll_up(1);
        row--;
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
    print_char_at_attr(c, row, col, attribute);
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
    print_at_attr(string, row, col, attribute);
}

void print(char* string) {
    print_at(string, -1, -1);
}

void print_ln(char* string) {
    print(string);
    print("\n");
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
    print_int_at_attr(i, row, col, attribute);
}

void print_int(int i) {
    print_int_at(i, -1, -1);
}

void print_hex_at_attr(int i, int row, int col, char attribute_byte) {
    char string[] = "0x0000";

    int cindex = sizeof(string) - 2;
    while(i != 0) {
        int remainder = i % 16;
        i /= 16;
        char c;

        if(remainder >= 10) {
            c = (char) (remainder + 55);
        }
        else {
            c = (char) (remainder + 48);
        }

        string[cindex--] = c;
    }

    print_at_attr(string, row, col, attribute_byte);
}

void print_hex_at(int i, int row, int col) {
    print_hex_at_attr(i, row, col, attribute);
}

void print_hex(int i) {
    print_hex_at(i, -1, -1);
}

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int stop = (int) format + strlen(format);

    while(format < stop) {
        char c = *(format);

        if(c == '%') {
            format++;
            char specifier = *format;

            switch(specifier) {
                case 's':
                    print(va_arg(args, char*));
                    break;
                case '%':
                    print_char('%');
                    break;
                case 'x':
                case 'X':
                    print_hex(va_arg(args, unsigned int));
                    break;
                case 'u':
                    print_int(va_arg(args, unsigned int));
                    break;
                case 'd':
                case 'i':
                    print_int(va_arg(args, signed int));
                    break;
            }

            format++;
            continue;
        }

        format++;
        print_char(c);
    }
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

void scroll_up(int lines) {
    for(int y = lines; y < MAX_ROWS; y++) {
        memcpy((char*) get_screen_offset(y, 0) + VIDEO_ADDRESS,
                    (char*) get_screen_offset(y - lines, 0) + VIDEO_ADDRESS,
                    MAX_COLS * 2);
    }

    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;

    int offset = get_screen_offset(MAX_ROWS - lines, 0);
    while(lines > 0) {
        for(int x = 0; x < MAX_COLS; x++) {
            vidmem[offset] = ' ';
            vidmem[offset + 1] = WHITE_ON_BLACK;
            offset += 2;
        }

        lines--;
    }
}

char get_char_at(int row, int col) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    return vidmem[get_screen_offset(row, col)];
}

void set_foreground_color(char color) {
    attribute = color | (attribute & 0xF0);
}

void set_background_color(char color) {
    attribute = (color << 4) | (attribute & 0x0F);
}

