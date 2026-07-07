#ifndef IDT_H
#define IDT_H

#include <stddef.h>
#include <stdint.h>
#include "screen/tty.h"
#include "../memory/mem.h"
#include "intr/pic.h"
#include "intr/keyboard.h"
#include "isr_gen.h"
#include "mouse.h"

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
typedef struct {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t int_no;
    uint64_t err;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed)) registers_t;


extern void idt_load();
extern void timer_irq(void);
extern void keyboard_irq(void);
extern void mouse_irq(void);
void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags, uint8_t ist);
void init_pit(uint32_t frequency);
void idt_install();
extern volatile uint64_t timer_ticks;
void timer_interrupt(uint64_t irq_n);
void isr_handler(registers_t *regs);
#endif
