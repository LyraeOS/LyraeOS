#ifndef IDT_H
#define IDT_H

#include <stddef.h>
#include <stdint.h>
#include "screen/tty.h"
#include "mem.h"
#include "intr/pic.h"


struct __attribute__((packed)) idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;        // bits 0-2 = IST index, rest zero
    uint8_t  type_attr;  // flags
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
};

struct __attribute__((packed, aligned(16))) idt_ptr {
    uint16_t limit;
    uint64_t base;
};

static struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();
extern void isr32(void);
static void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags, uint8_t ist) {
    idt[num].offset_low  = base & 0xFFFF;
    idt[num].selector    = sel;
    idt[num].ist         = ist & 0x7;
    idt[num].type_attr   = flags;
    idt[num].offset_mid  = (base >> 16) & 0xFFFF;
    idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
    idt[num].zero        = 0;
}
void init_pit(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;  // PIT base frequency
    
    outb(0x43, 0x36);  // Command: channel 0, lohi, rate generator
    outb(0x40, divisor & 0xFF);         // Low byte
    outb(0x40, (divisor >> 8) & 0xFF);  // High byte
}
void idt_install()
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint64_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256);
    
    idt_set_gate(32, (uint64_t)isr32, 0x08, 0x8E, 0);

    idt_load();
    remap_pic(0x20, 0x28);
    init_pit(1000);
    __asm__ volatile ("sti");
}
volatile uint64_t timer_ticks = 0;
void timer_interrupt(uint64_t irq_n) {
    timer_ticks++;
    
    pic_send_eoi(irq_n);
}
#endif