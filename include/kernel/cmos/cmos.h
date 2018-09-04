#ifndef _KERNEL_CMOS_CMOS_H
#define _KERNEL_CMOS_CMOS_H
#include <stdint.h>

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

uint8_t CMOS_read(uint8_t);

void CMOS_write(uint8_t, uint8_t);

uint32_t CMOS_get_update();

uint32_t CMOS_get_unix_time();

#endif
