#include "tty.h"

struct TTYCtx tty_ctx = {0};
size_t tty_max_chars_x;
size_t tty_max_chars_y;
static uint32_t custom_color;
bool cursor_enabled = true;
bool init_tty(struct GfxCtx gfx_ctx)
{
    tty_ctx.bytePitch = gfx_ctx.bytePitch;
    tty_ctx.fb_ptr = gfx_ctx.fb_ptr;
    tty_ctx.height = gfx_ctx.height;
    tty_ctx.width = gfx_ctx.width;
    tty_ctx.row = 0;
    tty_ctx.col = 0;
    tty_max_chars_x = tty_ctx.width / SCALED_WIDTH;
    tty_max_chars_y = tty_ctx.height / SCALED_HEIGHT;
    tty_ctx.bg = 0x000000;
    tty_ctx.fg = 0xffffff;
    custom_color = tty_ctx.fg;
    gfx_fill_slow(tty_ctx.bg);
    return true;
}
void tty_scroll(size_t line)
{
    if (line == 0 || (line * SCALED_HEIGHT) >= tty_ctx.height)
        return;

    tty_erase_cursor(tty_ctx.col, tty_ctx.row);
    uint32_t *dest = (uint32_t *)tty_ctx.fb_ptr;
    uint32_t *src = (uint32_t *)tty_ctx.fb_ptr + ((line * SCALED_HEIGHT) * tty_ctx.width);
    uint32_t pixelSize = (tty_ctx.height - (line * SCALED_HEIGHT)) * tty_ctx.width;
    uint32_t byteSize = pixelSize * sizeof(uint32_t);
    memmove(dest, src, byteSize);

    size_t y1 = (tty_max_chars_y - 1) * SCALED_HEIGHT;
    size_t y2 = y1 + SCALED_HEIGHT + SCALE_FACTOR;
    if (y2 > tty_ctx.height)
        y2 = tty_ctx.height;

    gfx_draw_rectangle_filled(
        vec2_new(0, y1),
        vec2_new(tty_ctx.width, y2),
        tty_ctx.bg);

    if (tty_ctx.row >= line)
        tty_set_cursor_pos(0, tty_ctx.row - line);
    else
        tty_set_cursor_pos(0, 0);

    tty_draw_cursor();
}
void kputchar(char c)
{
    // int line;
    if (c == '\n')
    {
        tty_erase_cursor(tty_ctx.col, tty_ctx.row);
        tty_ctx.col = 0;
        if (tty_ctx.row >= (tty_max_chars_y))
        {
            tty_scroll(1);
        }
        else
        {
            tty_set_cursor_pos(0, tty_ctx.row + 1);
        }
        return;
    }
    gfx_draw_character(c, tty_ctx.col * SCALED_WIDTH, tty_ctx.row * SCALED_HEIGHT, custom_color, tty_ctx.bg);
    tty_set_cursor_pos(tty_ctx.col + 1, tty_ctx.row);
    if (tty_ctx.col >= tty_max_chars_x - 1)
    {
        tty_ctx.col = 0;
        if (tty_ctx.row > tty_max_chars_y)
        {
            tty_scroll(1);
        }
        else
        {
            tty_set_cursor_pos(0, tty_ctx.row + 1);
        }
    }
}
void kprint(const char *data, size_t length)
{
    const unsigned char *bytes = (const unsigned char *)data;
    for (size_t i = 0; i < length; i++)
        kputchar(bytes[i]);
}
typedef enum
{
    NOTHING,
    FORMATTING,
} FormatMode;
int kprintf(const char *restrict format, ...)
{
    va_list params;
    va_start(params, format);
    FormatMode current = NOTHING;
    while (*format != '\0')
    {
        char cur = format[0];
        if (cur == '{')
        {
            format++;
            current = FORMATTING;
            continue;
        }
        else if (cur == '}')
        {
            format++;
            current = NOTHING;
            continue;
        }
        if (current == NOTHING)
        {
            // print the character normally
            kprint(&cur, sizeof(char));
            format++;
        }
        else
        {
            if (cur == 's')
            {
                const char *str = va_arg(params, const char *);
                size_t len = kstrlen(str);
                kprint(str, len);
            }
            if (cur == 'c')
            {
                char c = (char)va_arg(params, int);
                kprint(&c, sizeof(c));
            }
            if (cur == 'd')
            {
                int num = va_arg(params, int);
                char buf[12];
                int s = num;
                int i = 0;
                if (num < 0)
                {
                    num = -num;
                }
                if (num == 0)
                {
                    buf[i++] = '0';
                }
                while (num > 0)
                {
                    buf[i++] = num % 10 + '0';
                    num /= 10;
                }
                if (s < 0)
                {
                    buf[i++] = '-';
                }
                for (int j = 0, k = i - 1; j < k; j++, k--)
                {
                    char temp = buf[j];
                    buf[j] = buf[k];
                    buf[k] = temp;
                }
                buf[i] = '\0';
                kprint((const char *)buf, kstrlen(buf));
            }
            if (cur == 'x')
            {
                int num = va_arg(params, int);
                char *nums = "0123456789ABCDEF";
                char buf[12];
                int i = 0;
                if (num == 0)
                {
                    buf[i++] = '0';
                }
                while (num > 0)
                {
                    buf[i++] = nums[num % 16];
                    num /= 16;
                }
                for (int j = 0, k = i - 1; j < k; j++, k--)
                {
                    char temp = buf[j];
                    buf[j] = buf[k];
                    buf[k] = temp;
                }
                buf[i] = '\0';
                kprint((const char *)buf, kstrlen(buf));
            }
            if (cur == 'f')
            {
                double num = va_arg(params, double);
                char buf[32];
                int i = 0;
                if (num < 0)
                {
                    buf[i++] = '-';
                    num = -num;
                }
                int int_part = (int)num;
                double frac_part = num - int_part;
                char int_buf[16];
                int int_i = 0;
                if (int_part == 0)
                {
                    int_buf[int_i++] = '0';
                }
                else
                {
                    while (int_part > 0)
                    {
                        int_buf[int_i++] = int_part % 10 + '0';
                        int_part /= 10;
                    }
                }
                for (int j = int_i - 1; j >= 0; j--)
                {
                    buf[i++] = int_buf[j];
                }
                buf[i++] = '.';
                for (int j = 0; j < 6; j++)
                {
                    frac_part *= 10;
                    int digit = (int)frac_part;
                    buf[i++] = digit + '0';
                    frac_part -= digit;
                }
                buf[i] = '\0';
                kprint((const char *)buf, kstrlen(buf));
            }
            // temporary code to reset color
            if (cur == 'r')
            {
                custom_color = tty_ctx.fg;
            }
            // temporary code to set color
            if (cur == 'o')
            {
                custom_color = va_arg(params, uint32_t);
            }
            format++;
        }
    }
    return 1;
}
void tty_clear()
{
    gfx_fill_slow(tty_ctx.bg);
    tty_set_cursor_pos(0, 0);
}
void tty_backspace()
{
    if (tty_ctx.col < 1)
        return;
    tty_set_cursor_pos(tty_ctx.col - 1, tty_ctx.row);
    gfx_draw_character(' ', tty_ctx.col * SCALED_WIDTH, tty_ctx.row * SCALED_HEIGHT, custom_color, tty_ctx.bg);
}
ScreenScale tty_get_screen_size()
{
    ScreenScale sc;
    sc.x = tty_ctx.width;
    sc.y = tty_ctx.height;
    return sc;
}
void tty_set_cursor_pos(size_t x, size_t y)
{
    size_t lastx = tty_ctx.col, lasty = tty_ctx.row;
    if (x >= tty_max_chars_x)
        x = tty_max_chars_x - 1;
    if (y >= tty_max_chars_y)
        y = tty_max_chars_y - 1;
    if (lastx >= tty_max_chars_x)
        lastx = tty_max_chars_x - 1;
    if (lasty >= tty_max_chars_y)
        lasty = tty_max_chars_y - 1;
    tty_ctx.col = x;
    tty_ctx.row = y;
    tty_erase_cursor(lastx, lasty);
    tty_draw_cursor();
}
void tty_erase_cursor(size_t lastx, size_t lasty)
{
    size_t y1 = lasty * SCALED_HEIGHT + SCALED_HEIGHT;
    size_t y2 = y1 + SCALE_FACTOR;
    if (y1 >= tty_ctx.height)
        return;
    if (y2 > tty_ctx.height)
        y2 = tty_ctx.height;
    gfx_draw_rectangle_filled(
        vec2_new(lastx * SCALED_WIDTH, y1),
        vec2_new(lastx * SCALED_WIDTH + SCALED_WIDTH, y2),
        tty_ctx.bg);
}
void tty_draw_cursor()
{
    size_t y1 = tty_ctx.row * SCALED_HEIGHT + SCALED_HEIGHT;
    size_t y2 = y1 + SCALE_FACTOR;
    if (y1 >= tty_ctx.height)
        return;
    if (y2 > tty_ctx.height)
        y2 = tty_ctx.height;
    if (cursor_enabled)
        gfx_draw_rectangle_filled(
            vec2_new(tty_ctx.col * SCALED_WIDTH, y1),
            vec2_new(tty_ctx.col * SCALED_WIDTH + SCALED_WIDTH, y2),
            custom_color);
}
static uint64_t last_blink = 0;
static bool cursor_visible = false;
void tty_update_cursor()
{
    const uint64_t BLINK_INTERVAL = 500;

    if (timer_ticks - last_blink < BLINK_INTERVAL)
        return;

    last_blink = timer_ticks;

    if (cursor_visible)
    {
        tty_erase_cursor(tty_ctx.col, tty_ctx.row);
        cursor_visible = false;
    }
    else
    {
        tty_draw_cursor();
        cursor_visible = true;
    }
}