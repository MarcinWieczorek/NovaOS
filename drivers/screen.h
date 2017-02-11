#define  VIDEO_ADDRESS 0xB8000
#define  MAX_ROWS  25
#define  MAX_COLS  80
#define  WHITE_ON_BLACK 0x0F
#define  REG_SCREEN_CTRL 0x3D4
#define  REG_SCREEN_DATA 0x3D5

void print_char_at_attr(char c, int row, int col, char attribute_byte);

void print_char_at(char c, int row, int col);

void print_char(char c);

void print_at_attr(char* string, int row, int col, char attribute_byte);

void print_at(char* string, int row, int col);

void print(char* c);

void print_int_at_attr(int i, int row, int col, char attribute_byte);

void print_int_at(int i, int row, int col);

void print_int(int i);

int get_screen_offset(int row, int col);

int get_cursor();

void set_cursor_offset(int offset);

void set_cursor(int row, int col);

void clear_screen();

