#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <lib/util.h>
#include <mm/mem.h>
#include <drivers/display/gfx.h>
#include <cpu/idt.h>

typedef struct {
    uint32_t fg, bg, cursor;
    uint32_t error, warning, success, info;
    uint32_t accent;
} TTYTheme;

static const TTYTheme LYRAE_LIGHT = {
    .fg      = 0x2b2d42, 
    .bg      = 0xf7f9fc, 
    .cursor  = 0x5b8def, 

    .error   = 0xd64550, 
    .warning = 0xf4a261, 
    .success  = 0x2a9d8f, 
    .info    = 0x457b9d, 

    .accent  = 0x7c5cff, 
};
static const TTYTheme LYRAE_DARK = {
    .fg      = 0xe6edf3,
    .bg      = 0x0f172a,
    .cursor  = 0x7dd3fc,

    .error   = 0xf87171,
    .warning = 0xfbbf24,
    .success  = 0x4ade80,
    .info    = 0x60a5fa,
    .accent  = 0xa78bfa,
};

static const TTYTheme LYRAE_PANIC = {
    .fg      = 0xffdede,
    .bg      = 0x190609,
    .cursor  = 0xff6666,

    .error   = 0xff6666,
    .warning = 0xffb347,
    .success  = 0x90ee90,
    .info    = 0x87cefa,

    .accent  = 0xff4444,
};

struct TTYCtx {
    uint64_t width, height, bytePitch;
    volatile uint32_t *fb_ptr;
    size_t col, row;
    TTYTheme theme;
};
typedef struct {
    uint64_t x, y;
} ScreenSize;
struct GfxCtx;
bool init_tty(struct GfxCtx gfx_ctx);
void kputchar(char c);
void tty_scroll(size_t line);

int kprintf(const char* restrict format, ...);
int vkprintf(const char* restrict format, va_list args);

void tty_clear();
void tty_backspace();
ScreenSize tty_get_screen_size();
void tty_set_cursor_pos(size_t x, size_t y);
void tty_draw_cursor();
void tty_erase_cursor(size_t lastx, size_t lasty);
void tty_update_cursor();
void tty_set_cursor_enabled(bool enabled);
void tty_change_theme(TTYTheme theme);
TTYTheme* tty_cur_theme();
void tty_read_line(char* input_buf);

#endif
