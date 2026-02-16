#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "util.h"
#include "mem.h"
#include "gfx.h"

struct TTYCtx {
    uint64_t width, height, bytePitch;
    volatile uint32_t *fb_ptr;
    size_t col, row;
    uint32_t fg, bg;
};
typedef struct {
    uint64_t x, y;
} ScreenScale;
struct GfxCtx;
bool init_tty(struct GfxCtx gfx_ctx);
void kputchar(char c);
void tty_scroll(size_t line);
int kprintf(const char* restrict format, ...);
void tty_clear();
void tty_backspace();
ScreenScale tty_get_screen_size();
#endif
