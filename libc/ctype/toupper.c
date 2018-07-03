#include <ctype.h>

int toupper(int i) {
    if(islower(i)) {
        return i - 0x20;
    }

    return i;
}
