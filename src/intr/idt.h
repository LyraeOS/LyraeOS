#ifndef IDT_H
#define IDT_H

#include "screen/tty.h"
#include "mem.h"
#include "intr/pic.h"
#include "intr/keyboard.h"
#include "isr_gen.h"
#include "liblyr.h"
struct __attribute__((packed)) idt_entry {
    u16 offset_low;
    u16 selector;
    u8  ist;        // bits 0-2 = IST index, rest zero
    u8  type_attr;  // flags
    u16 offset_mid;
    u32 offset_high;
    u32 zero;
};

struct __attribute__((packed, aligned(16))) idt_ptr {
    u16 limit;
    u64 base;
};
typedef struct {
    // Segment registers (if you save them)
    u64 ds;
    
    // General purpose registers pushed by pusha equivalent (push order)
    u64 r15, r14, r13, r12, r11, r10, r9, r8;
    u64 rbp, rdi, rsi, rdx, rcx, rbx, rax;
    
    // Interrupt/exception number and error code
    u64 int_no, err_code;
    
    // Pushed by CPU automatically during interrupt
    u64 rip;
    u64 cs;
    u64 rflags;
    u64 rsp;
    u64 ss;
} __attribute__((packed)) registers_t;


extern void idt_load();
extern void timer_irq(void);
extern void keyboard_irq(void);
void idt_set_gate(u8 num, u64 base, u16 sel, u8 flags, u8 ist);
void init_pit(u32 frequency);
void idt_install();
extern volatile u64 timer_ticks;
void timer_interrupt(u64 irq_n);
void isr_handler(u64 exception_num);
#endif
