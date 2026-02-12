#ifndef GFX_H
#define GFX_H

#include "limine.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


struct GfxCtx {
  uint64_t width, height, bytePitch;
  volatile uint32_t *fb_ptr;
  struct limine_framebuffer *framebuffer;
  uint32_t bg_color, fg_color;
  uint32_t char_x, char_y; // Current position of where we are drawing characters.
};
bool init_gfx(struct limine_framebuffer_request rq);
void gfx_fill_slow(uint32_t c);
void gfx_draw_character(char c, int x, int y);
void gfx_set_pixel(size_t x, size_t y, uint32_t c);
void gfx_draw_string(char* s, int x, int y);
void font_scale(uint8_t *buf, char c);
#endif
