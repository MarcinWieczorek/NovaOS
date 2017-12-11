#include <ctype.h>

int isxdigit(int c) {
    return isdigit(c) || (c >= 0x41 && c <= 0x46);
}
