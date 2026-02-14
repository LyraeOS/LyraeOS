#include "tty.h"

struct TTYCtx tty_ctx = {0};
static size_t max_x_chars;
static size_t max_y_chars;

bool init_tty(struct GfxCtx gfx_ctx) {
    tty_ctx.bytePitch = gfx_ctx.bytePitch;
    tty_ctx.fb_ptr = gfx_ctx.fb_ptr;
    tty_ctx.height = gfx_ctx.height;
    tty_ctx.width = gfx_ctx.width;
    tty_ctx.row = 0;
    tty_ctx.col = 0;
    max_x_chars = tty_ctx.width / SCALED_WIDTH;
    max_y_chars = tty_ctx.height / SCALED_HEIGHT;
    return true;
}

void kputchar(char c) {
    //int line;
    if (c == '\n') {
        tty_ctx.col = 0;
        if (tty_ctx.row++ == max_y_chars) {
            // SCROLL AT SOME POINT

        }
        return;
    }
    gfx_draw_character(c, tty_ctx.col*SCALED_WIDTH, tty_ctx.row*SCALED_HEIGHT);
    if (tty_ctx.col++ == max_x_chars) {
        tty_ctx.col = 0;
        if (tty_ctx.row++ == max_y_chars) {
            // SCROLL AT SOME POINT

        }
    }
}
void kprint(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++)
        kputchar(bytes[i]);
}
typedef enum {
    NOTHING,
    FORMATTING,
} FormatMode;
int kprintf(const char* restrict format, ...) {
    va_list params;
    va_start(params, format);
    FormatMode current = NOTHING; 
    while (*format != '\0') {
        char cur = format[0];
        if (cur == '{') {
           format++;
           current = FORMATTING; 
           continue;
        } else if (cur == '}') {
            format++;
            current = NOTHING;
            continue;
        }
        if (current == NOTHING) {
            // print the character normally
            kprint(&cur, sizeof(char));
            format++;
        } else {
            if (cur == 's') {
                const char* str = va_arg(params, const char *);
                size_t len = kstrlen(str);
                kprint(str, len);
            }
            if (cur == 'c') {
                char c = (char) va_arg(params, int);
                kprint(&c, sizeof(c));
            }
            format++;
        }
    }
    return 1;
}
