BITS 64
default rel
%macro pushaq 0
    push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro
%macro popaq 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro


global timer_irq
extern timer_interrupt

timer_irq:
	pushaq
    cld
    mov rdi, 32
    
	call timer_interrupt

    popaq
    iretq

global keyboard_irq
extern keyboard_interrupt_handler

keyboard_irq:
	pushaq
    cld
	cli
	call keyboard_interrupt_handler
	sti
    popaq
    iretq

global idt_load
extern idtp

idt_load:
    lidt [idtp]   ; load IDT pointer
    ret
