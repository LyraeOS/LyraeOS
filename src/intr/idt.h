#ifndef IDT_H
#define IDT_H

#include <stddef.h>
#include <stdint.h>
#include "screen/tty.h"
#include "mem.h"
#include "intr/pic.h"
#include "intr/keyboard.h"
#include "isr_gen.h"

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
    // Segment registers (if you save them)
    uint64_t ds;
    
    // General purpose registers pushed by pusha equivalent (push order)
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    
    // Interrupt/exception number and error code
    uint64_t int_no, err_code;
    
    // Pushed by CPU automatically during interrupt
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed)) registers_t;


extern void idt_load();
extern void timer_irq(void);
extern void keyboard_irq(void);
void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags, uint8_t ist);
void init_pit(uint32_t frequency);
void idt_install();
extern volatile uint64_t timer_ticks;
void timer_interrupt(uint64_t irq_n);
void isr_handler(uint64_t exception_num);
#endif
