#include <ctype.h>

int tolower(int i) {
    if(isupper(i)) {
        return i + 0x20;
    }

    return i;
}
