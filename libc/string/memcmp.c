#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n) {
    while(--n && *(char *) s1 == *(char *) s2) {
        ++s1;
        ++s2;
    }

    return *(char *) s1  - *(char *) s2;
}
