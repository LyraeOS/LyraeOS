#ifndef GFX_H
#define GFX_H

#include "limine.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "util.h"
#include "tty.h"

#define FONT_WIDTH 8
#define FONT_HEIGHT 16

typedef struct {
  int factor;
  int width, height;
} ScalingInfo;

struct GfxCtx {
  uint64_t width, height, bytePitch;
  volatile uint32_t *fb_ptr;
  struct limine_framebuffer *framebuffer;
  uint32_t bg_color, fg_color;
  ScalingInfo scaling_info;
};

typedef struct {
  int x;
  int y;
} vec2;

vec2 vec2_new(int x, int y);

bool init_gfx(struct limine_framebuffer_request rq);
void gfx_fill_slow(uint32_t c);
void gfx_draw_character(char c, int x, int y, uint32_t fg, uint32_t bg);
void gfx_draw_character_transparent(char c, int start_x, int start_y, uint32_t fg);
void gfx_set_pixel(size_t x, size_t y, uint32_t c);
void font_scale(uint8_t *buf, char c);

void gfx_draw_rectangle(vec2 p1, vec2 p2, uint32_t c);
void gfx_draw_rectangle_filled(vec2 p1, vec2 p2, uint32_t c);
void gfx_draw_circle(vec2 center, uint32_t radius, uint32_t c);
void gfx_draw_line(vec2 p1, vec2 p2, uint32_t c);
void gfx_draw_triangle(vec2 p1, vec2 p2, vec2 p3, uint32_t c);

void gfx_update_scale(int scale_factor);
ScalingInfo gfx_get_scaling_info();

#define RGB(R,G,B) ((((R) & 0xff) << 16) | (((G) & 0xff) << 8) | ((B) & 0xff))
#endif
