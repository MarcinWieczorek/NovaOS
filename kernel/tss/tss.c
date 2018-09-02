#include <string.h>

#include <kernel/tss/tss.h>

tss_entry_t tss_entry;

void tss_write(gdt_entry_bits *g) {
   uint32_t base = (uint32_t) &tss_entry;
   uint32_t limit = sizeof(tss_entry);

   g->limit_low=limit&0xFFFF;
   g->base_low=base&0xFFFFFF; //isolate bottom 24 bits
   g->accessed=1; //This indicates it's a TSS and not a LDT. This is a changed meaning
   g->read_write=0; //This indicates if the TSS is busy or not. 0 for not busy
   g->conforming_expand_down=0; //always 0 for TSS
   g->code=1; //For TSS this is 1 for 32bit usage, or 0 for 16bit.
   g->always_1=0; //indicate it is a TSS
   g->DPL=3; //same meaning
   g->present=1; //same meaning
   g->limit_high=(limit&0xF0000)>>16; //isolate top nibble
   g->available=0;
   g->always_0=0; //same thing
   g->big=0; //should leave zero according to manuals. No effect
   g->gran=0; //so that our computed GDT limit is in bytes, not pages
   g->base_high=(base&0xFF000000)>>24; //isolate top byte.

   memset(&tss_entry, 0, sizeof(tss_entry));

   tss_entry.esp0 = 0xA0000; // Set the kernel stack pointer.
   tss_entry.ss0  = 0x10;   // Set the kernel stack segment.
}

void tss_install() {
    tss_write(TSS_ADDRESS);
    tss_flush();
}
