#ifndef IDT_H
#define IDT_H

#include <stddef.h>
#include <stdint.h>
#include "screen/tty.h"
#include "mem.h"
#include "intr/pic.h"
#include "intr/keyboard.h"


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


extern void idt_load();
extern void timer_irq(void);
extern void keyboard_irq(void);
void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags, uint8_t ist);
void init_pit(uint32_t frequency);
void idt_install();
extern volatile uint64_t timer_ticks;
void timer_interrupt(uint64_t irq_n);

#endif
