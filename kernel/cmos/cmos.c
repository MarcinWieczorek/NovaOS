#include <kernel/cmos/cmos.h>
#include <sys/io.h>

uint8_t CMOS_read(uint8_t addr) {
    outb(addr, CMOS_ADDR);
    return inb(CMOS_DATA);
}

uint32_t CMOS_get_unix_time() {
    int update = 1;
    while(update) {
        unsigned char c = CMOS_read(0xA);
        if(!(c & 0x80)) {
            update = 0;
        }
    }

    uint8_t sec    = CMOS_read(0x00);
    uint8_t min    = CMOS_read(0x02);
    uint8_t hour   = CMOS_read(0x04);
    uint8_t day    = CMOS_read(0x07);
    uint8_t month  = CMOS_read(0x08);
    uint8_t year   = CMOS_read(0x09);
    uint8_t format = CMOS_read(0x0B);

    // convert all fields from BDC to binary if bit[2] is clear
    if(!(format & 0x4)) {

        sec = (sec & 0xf) + (sec>>4)*10;
        min = (min & 0xf) + (min>>4)*10;
        hour = (hour & 0xf) + ((hour&0x70)>>4)*10 + (hour&0x80); // keep bit-7
        day = (day & 0xf) + (day>>4)*10;
        month = (month & 0xf) + (month>>4)*10;
        year = (year & 0xf) + (year>>4)*10;

    }

    // convert hours from 12 to 24 format if bit[1] is clear
    if(!(format & 0x2)) {
        int ampm = hour & 0x80;
        hour = hour & 0x7f;
        if(hour == 12) {
            hour = 0;
        }

        if(ampm) {
            hour += 12;
        }
    }

    uint32_t fullyeardays = year * 365 + ((year-1)/4) + 1;
    static uint32_t daysbeforemonth[12] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
    };
    uint32_t thisyeardays = daysbeforemonth[month-1] + day
        - (((year%4)==0 && month > 2) ? 0 : 1);
    uint32_t date = fullyeardays + thisyeardays + 10957;
    return sec + 60 * (min + 60 * (hour + 24 * date));
}
