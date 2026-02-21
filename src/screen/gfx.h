#ifndef GFX_H
#define GFX_H

#include "limine.h"
#include "util.h"
#include "tty.h"

#define FONT_WIDTH 8
#define FONT_HEIGHT 16
#define SCALE_FACTOR 1
#define SCALED_WIDTH (FONT_WIDTH*SCALE_FACTOR)
#define SCALED_HEIGHT (FONT_HEIGHT*SCALE_FACTOR)

struct GfxCtx {
  u64 width, height, bytePitch;
  volatile u32 *fb_ptr;
  struct limine_framebuffer *framebuffer;
  u32 bg_color, fg_color;
};

typedef struct {
  int x;
  int y;
} vec2;
vec2 vec2_new(int x, int y);
bool init_gfx(struct limine_framebuffer_request rq);
void gfx_fill_slow(u32 c);
void gfx_draw_character(char c, int x, int y, u32 fg, u32 bg);
void gfx_draw_character_transparent(char c, int start_x, int start_y, u32 fg);
void gfx_set_pixel(st x, st y, u32 c);
void font_scale(u8 *buf, char c);

void gfx_draw_rectangle(vec2 p1, vec2 p2, u32 c);
void gfx_draw_rectangle_filled(vec2 p1, vec2 p2, u32 c);
void gfx_draw_circle(vec2 center, u32 radius, u32 c);
void gfx_draw_line(vec2 p1, vec2 p2, u32 c);
void gfx_draw_triangle(vec2 p1, vec2 p2, vec2 p3, u32 c);
u32 rgb_to_hex(int r, int g, int b);
#endif
