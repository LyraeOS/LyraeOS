#include "tty.h"

struct TTYCtx tty_ctx = {0};
static size_t max_x_chars;
static size_t max_y_chars;
static uint32_t custom_color;
bool init_tty(struct GfxCtx gfx_ctx) {
    tty_ctx.bytePitch = gfx_ctx.bytePitch;
    tty_ctx.fb_ptr = gfx_ctx.fb_ptr;
    tty_ctx.height = gfx_ctx.height;
    tty_ctx.width = gfx_ctx.width;
    tty_ctx.row = 0;
    tty_ctx.col = 0;
    max_x_chars = tty_ctx.width / SCALED_WIDTH;
    max_y_chars = tty_ctx.height / SCALED_HEIGHT;
    tty_ctx.bg = 0x000000;
    tty_ctx.fg = 0xffffff;
    custom_color = tty_ctx.fg;
    gfx_fill_slow(tty_ctx.bg);
    return true;
}
void tty_scroll(size_t line) {
    uint32_t* dest = (uint32_t*)tty_ctx.fb_ptr;
    uint32_t* src = (uint32_t*)tty_ctx.fb_ptr + ((line*SCALED_HEIGHT) * tty_ctx.width);
    uint32_t pixelSize = (tty_ctx.height - (line*SCALED_HEIGHT)) * tty_ctx.width;
    uint32_t byteSize = pixelSize * sizeof(uint32_t);
    memmove(dest, src, byteSize);
    tty_ctx.row--;
}

void kputchar(char c) {
    //int line;
    if (c == '\n') {
        tty_ctx.col = 0;
        tty_ctx.row++;
        if (tty_ctx.row >= max_y_chars) {
            // SCROLL AT SOME POINT
            tty_scroll(1);
        }
        return;
    }
    gfx_draw_character(c, tty_ctx.col*SCALED_WIDTH, tty_ctx.row*SCALED_HEIGHT, custom_color, tty_ctx.bg);
    tty_ctx.col++;
    if (tty_ctx.col >= max_x_chars) {
        tty_ctx.col = 0;
        tty_ctx.row++;
        if (tty_ctx.row >= max_y_chars) {
            tty_scroll(1);
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
            if (cur == 'd') {
                int num = va_arg(params, int);
                char buf[12];
                int s = num;
                int i = 0;
                if (num < 0) {
                    num = -num;
                }
                if (num == 0) {
                    buf[i++] = '0';
                }
                while (num > 0) {
                    buf[i++] = num % 10 + '0';
                    num /= 10;
                }
                if (s < 0) {
                    buf[i++] = '-';
                }
                for (int j = 0, k = i - 1; j < k; j++, k--) {
                    char temp = buf[j];
                    buf[j] = buf[k];
                    buf[k] = temp;
                }
                buf[i] = '\0';
                kprint((const char*)buf, kstrlen(buf));
            }
            if (cur == 'x') {
                int num = va_arg(params, int);
                char* nums = "0123456789ABCDEF";
                char buf[12];
                int i = 0;
                if (num == 0) {
                    buf[i++] = '0';
                }
                while (num > 0) {
                    buf[i++] = nums[num % 16];
                    num /= 16;
                }
                for (int j = 0, k = i - 1; j < k; j++, k--) {
                    char temp = buf[j];
                    buf[j] = buf[k];
                    buf[k] = temp;
                }
                buf[i] = '\0';
                kprint((const char*)buf, kstrlen(buf));
            }
            // temporary code to reset color
            if (cur == 'r') {
                custom_color = tty_ctx.fg;
            }
            // temporary code to set color
            if (cur == 'o') {
                custom_color = va_arg(params, uint32_t);
            }
            format++;
        }
    }
    return 1;
}
void tty_clear() {
    gfx_fill_slow(tty_ctx.bg);
    tty_ctx.row = 0;
    tty_ctx.col = 0;
}
void tty_backspace() {
    if (tty_ctx.col < 1)
        return;
    tty_ctx.col--;
    gfx_draw_character(' ', tty_ctx.col*SCALED_WIDTH, tty_ctx.row*SCALED_HEIGHT, custom_color, tty_ctx.bg);
}
ScreenScale tty_get_screen_size() {
    ScreenScale sc;
    sc.x = tty_ctx.width;
    sc.y = tty_ctx.height;
    return sc;
}