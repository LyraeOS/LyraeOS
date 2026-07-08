bits 64
global fpu_init

fpu_init:
    mov rax, cr0
    and ax, 0xFFF3
    or ax, 0x2
    mov cr0, rax

    mov rax, cr4
    or ax, 0x600
    mov cr4, rax

    fninit
    ret