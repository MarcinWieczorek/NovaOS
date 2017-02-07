unsigned char in_b(unsigned short port) {
    unsigned char result;
    __asm__("IN %%DX, %%AL" : "=a" (result) : "d" (port));
    return result;
}

void out_b(unsigned short port, unsigned char data) {
    __asm__("OUT %%AL, %%DX" : :"a" (data), "d" (port));
}

