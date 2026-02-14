#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "util.h"
#include "gfx.h"

struct TTYCtx {
    uint64_t width, height, bytePitch;
    volatile uint32_t *fb_ptr;
    size_t col, row;
};
struct GfxCtx;
bool init_tty(struct GfxCtx gfx_ctx);
void kputchar(char c);
int kprintf(const char* restrict format, ...);
#endif
