global gdt_load
extern gdtr

section .text
bits 64
default rel
gdt_load:
    lgdt [gdtr]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    push 0x08
    lea rax, [rel .flush]
    push rax
    retfq

.flush:
    ret
