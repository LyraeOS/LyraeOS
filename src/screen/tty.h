#ifndef TTY_H
#define TTY_H

#include "util.h"
#include "mem.h"
#include "gfx.h"
#include "intr/idt.h"
#include "liblyr.h"

struct TTYCtx {
    u64 width, height, bytePitch;
    volatile u32 *fb_ptr;
    st col, row;
    u32 fg, bg;
};
typedef struct {
    u64 x, y;
} ScreenScale;
struct GfxCtx;
bool init_tty(struct GfxCtx gfx_ctx);
void kputchar(char c);
void tty_scroll(st line);
int kprintf(const char* restrict format, ...);
void tty_clear();
void tty_backspace();
ScreenScale tty_get_screen_size();
void tty_set_cursor_pos(st x, st y);
void tty_draw_cursor();
void tty_erase_cursor(st lastx, st lasty);
void tty_update_cursor();
void tty_set_cursor_enabled(bool enabled);
#endif
