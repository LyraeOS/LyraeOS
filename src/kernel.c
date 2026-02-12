#include "gfx.h"
#include "mem.h"
#include "util.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests"))) static volatile uint64_t
    limine_base_revision[] = LIMINE_BASE_REVISION(4);
__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = 0};

__attribute__((used,
               section(".limine_requests_start"))) static volatile uint64_t
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

void kmain(void) {
  if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
    hlt_loop();
  }
  if (!init_gfx(framebuffer_request)) {
    hlt_loop();
  }

  gfx_draw_string("SHAPES !!!!!!", 50, 50);
  gfx_draw_circle(vec2_new(300, 300), 100, 0xFF0000);
  gfx_draw_circle(vec2_new(300, 400), 100, 0x00FF00);
  gfx_draw_circle(vec2_new(300, 501), 100, 0x0000FF);
  gfx_draw_rectangle(vec2_new(500, 100), vec2_new(700, 200), 0xFFFF00);
  gfx_draw_line(vec2_new(600, 600), vec2_new(500, 500), 0x00FFFF);
  gfx_draw_rectangle_filled(vec2_new(500, 300), vec2_new(700, 400), 0xFF00FF);
  gfx_draw_triangle(vec2_new(800, 400), vec2_new(1000, 400), vec2_new(900, 200), 0xFFFFFF);
  hlt_loop();
}
