#ifndef GDT_H
#define GDT_H
#include "liblyr.h"

struct __attribute__((packed)) GDTEntry {
    u16 limit_low;
    u16 base_low;
    u8  base_mid;
    u8  access;
    u8  granularity;
    u8  base_high;
};

struct __attribute__((packed)) GDTR {
    u16 limit;
    u64 base;
};

static struct GDTEntry gdt[3];
struct GDTR gdtr;

static struct GDTEntry gdt_entry(u8 access, u8 flags) {
    struct GDTEntry e = {0};
    e.access = access;
    e.granularity = flags;
    return e;
}

void gdt_init(void) {
    gdt[0] = gdt_entry(0, 0);
    gdt[1] = gdt_entry(0x9A, 0xA0);  // code
    gdt[2] = gdt_entry(0x92, 0xA0);  // data

    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (u64)&gdt;
}

extern void gdt_load(void);

void gdt_install(void) {
    gdt_init();
    gdt_load();
}

#endif