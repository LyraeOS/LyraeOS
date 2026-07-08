#ifndef GDT_H
#define GDT_H
#include <stdint.h>

struct __attribute__((packed)) GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
};

struct __attribute__((packed)) GDTR {
    uint16_t limit;
    uint64_t base;
};

static struct GDTEntry gdt[3];
struct GDTR gdtr;

static struct GDTEntry gdt_entry(uint8_t access, uint8_t flags) {
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
    gdtr.base = (uint64_t)&gdt;
}

extern void gdt_load(void);

void gdt_install(void) {
    gdt_init();
    gdt_load();
}

#endif