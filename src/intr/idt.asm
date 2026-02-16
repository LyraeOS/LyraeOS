BITS 64
default rel
%macro pushaq 0
    push rax		;save current rax
	push rbx		;save current rbx
	push rcx		;save current rcx
	push rdx		;save current rdx
	push rbp		;save current rbp
	push rdi		;save current rdi
	push rsi		;save current rsi
	push r8			;save current r8
	push r9			;save current r9
	push r10		;save current r10
	push r11		;save current r11
	push r12		;save current r12
	push r13		;save current r13
	push r14		;save current r14
	push r15		;save current r15
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


global isr32
extern timer_interrupt

isr32:
	pushaq
    cld
    mov rdi, 32
    
	call timer_interrupt

    popaq
    iretq

global idt_load
extern idtp

idt_load:
    lidt [idtp]   ; load IDT pointer
    ret
