#include "gfx.h"
// used this font from github
// https://github.com/azmr/blit-fonts
uint32_t glyphs[95] = {
0x00000000,0x08021084,0x0000294a,0x15f52bea,0x08fa38be,0x33a22e60,0x2e94d8a6,0x00001084,
0x10421088,0x04421082,0x00a23880,0x00471000,0x04420000,0x00070000,0x0c600000,0x02222200,
0x1d3ad72e,0x3e4214c4,0x3e22222e,0x1d18320f,0x210fc888,0x1d183c3f,0x1d17844c,0x0222221f,
0x1d18ba2e,0x210f463e,0x0c6018c0,0x04401000,0x10411100,0x00e03800,0x04441040,0x0802322e,
0x3c1ef62e,0x231fc544,0x1f18be2f,0x3c10862e,0x1f18c62f,0x3e10bc3f,0x0210bc3f,0x1d1c843e,
0x2318fe31,0x3e42109f,0x0c94211f,0x23149d31,0x3e108421,0x231ad6bb,0x239cd671,0x1d18c62e,
0x0217c62f,0x30eac62e,0x2297c62f,0x1d141a2e,0x0842109f,0x1d18c631,0x08454631,0x375ad631,
0x22a21151,0x08421151,0x3e22221f,0x1842108c,0x20820820,0x0c421086,0x00004544,0xbe000000,
0x00000082,0x1c97b000,0x0e949c21,0x1c10b800,0x1c94b908,0x3c1fc5c0,0x42211c4c,0x4e87252e,
0x12949c21,0x0c210040,0x8c421004,0x12519521,0x0c210842,0x235aac00,0x12949c00,0x0c949800,
0x4213a526,0x7087252e,0x02149800,0x0e837000,0x0c213c42,0x0e94a400,0x0464a400,0x155ac400,
0x36426c00,0x4e872529,0x1e223c00,0x1843188c,0x08421084,0x0c463086,0x0006d800 };


struct GfxCtx gfx_ctx = {0};

bool init_gfx(struct limine_framebuffer_request rq)
{
    if (rq.response == NULL ||
        rq.response->framebuffer_count < 1) {
        return false;
    }
    struct limine_framebuffer* fb = rq.response->framebuffers[0];
    gfx_ctx.bytePitch = fb->pitch / 4;
    gfx_ctx.height = fb->height;
    gfx_ctx.width = fb->width;
    gfx_ctx.fb_ptr = fb->address;
    gfx_ctx.framebuffer = fb;
    gfx_ctx.bg_color = 0x000000;
    gfx_ctx.fg_color = 0xFFFFFF;
    gfx_fill_slow(gfx_ctx.bg_color);
    init_tty(gfx_ctx);
    return true;
}

vec2 vec2_new(int x, int y) {
  vec2 new;
  new.x = x;
  new.y = y;
  return new;
}

void gfx_fill_slow(uint32_t c) {
  for (size_t x = 0; x < gfx_ctx.width; x++)
    for (size_t y = 0; y < gfx_ctx.height; y++)
      gfx_ctx.fb_ptr[x + y*gfx_ctx.bytePitch] = c;
}
void gfx_set_pixel(size_t x, size_t y, uint32_t c) {
  gfx_ctx.fb_ptr[x+y*gfx_ctx.bytePitch] = c;
}

void font_scale(uint8_t *buf, char c) {
  uint32_t bmp = glyphs[c-32];
  for (int y = 0; y < FONT_HEIGHT; y++) {
    for (int x = 0; x < FONT_WIDTH; x++) {
      uint32_t mask = 1 << (y*FONT_WIDTH+x);
      int v = (bmp & mask) ? 1 : 0;
      int scaled_x = x*SCALE_FACTOR;
      int scaled_y = y*SCALE_FACTOR;
      for (int sx = 0; sx < SCALE_FACTOR; sx++)
        for (int sy = 0; sy < SCALE_FACTOR; sy++)
          buf[(scaled_y + sy) * SCALED_WIDTH + (scaled_x + sx)] = v;
    }
  }
}

void gfx_draw_character(char c, int start_x, int start_y) {
  uint8_t buf[SCALED_HEIGHT*SCALED_WIDTH];
  font_scale(buf, c);
  for (int x = start_x; x < start_x + SCALED_WIDTH; x++) {
    for (int y = start_y; y < start_y + SCALED_HEIGHT; y++) {
      int x_iter = (x - start_x);
      int y_iter = (y - start_y);
      gfx_set_pixel(x, y, buf[x_iter + y_iter*SCALED_WIDTH] ? gfx_ctx.fg_color : gfx_ctx.bg_color);
    }
  }
}

void gfx_draw_rectangle_filled(vec2 p1, vec2 p2, uint32_t c) {
  for (int x = p1.x; x < p2.x; x++) {
    for (int y = p1.y; y < p2.y; y++) {
      gfx_ctx.fb_ptr[x + y*gfx_ctx.bytePitch] = c;
    }
  }
}

// uses the midpoint circle drawing algorithm
void gfx_draw_circle(vec2 center, uint32_t radius, uint32_t c) {
  int x = radius, y = 0;
  int p = 1 - radius;

  gfx_set_pixel(x + center.x, y + center.y, c);

  if (radius > 0) {
      gfx_set_pixel(x + center.x, -y + center.y, c);
      gfx_set_pixel(y + center.x, x + center.y, c);
      gfx_set_pixel(-y + center.x, x + center.y, c);
  }
  
  while (x > y) {
    y++;
    // Mid-point is inside or on the perimeter
    if (p <= 0) {
      p = p + 2*y + 1;
    } else {
      // mid-point is outside the perimeter
      x--;
      p = p + 2*y - 2*x + 1;
    }

    if (x < y)
      break;
      
    gfx_set_pixel(x + center.x, y + center.y, c);
    gfx_set_pixel(-x + center.x, y + center.y, c);
    gfx_set_pixel(x + center.x, -y + center.y, c);
    gfx_set_pixel(-x + center.x, -y + center.y, c);

    if (x != y) {
      gfx_set_pixel(y + center.x, x + center.y, c);
      gfx_set_pixel(-y + center.x, x + center.y, c);
      gfx_set_pixel(y + center.x, -x + center.y, c);
      gfx_set_pixel(-y + center.x, -x + center.y, c);
    }
  }
}
// Bresenham approach
void gfx_draw_line(vec2 p1, vec2 p2, uint32_t c) {
  int dx = abs(p2.x - p1.x);
  int dy = abs(p2.y - p1.y);
  int sx = p1.x < p2.x ? 1 : -1;
  int sy = p1.y < p2.y ? 1 : -1;
  int err = dx - dy;
  int x1 = p1.x, x2 = p2.x, y1 = p1.y, y2=p2.y; 
  while (1) {

    gfx_set_pixel(x1, y1, c);

    if (x1 == x2 && y1 == y2) break;

    int e2 = 2 * err;

    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }

    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}
void gfx_draw_rectangle(vec2 p1, vec2 p2, uint32_t c) {
  gfx_draw_line(vec2_new(p2.x, p1.y), vec2_new(p2.x, p2.y), c);
  gfx_draw_line(vec2_new(p1.x, p1.y), vec2_new(p2.x, p1.y), c);
  gfx_draw_line(vec2_new(p2.x, p2.y), vec2_new(p1.x, p2.y), c);
  gfx_draw_line(vec2_new(p1.x, p1.y), vec2_new(p1.x, p2.y), c);
}
void gfx_draw_triangle(vec2 p1, vec2 p2, vec2 p3, uint32_t c) {
  gfx_draw_line(p1, p2, c);
  gfx_draw_line(p2, p3, c);
  gfx_draw_line(p3, p1, c);
}
