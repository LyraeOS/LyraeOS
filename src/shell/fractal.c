#include "fractal.h"
vec2 midpoint(vec2 a, vec2 b) {
    return vec2_new((a.x + b.x) / 2, (a.y + b.y) / 2);
}
void sierpinski(vec2 p1, vec2 p2, vec2 p3, int depth) {
    if (depth == 0) {
        gfx_draw_triangle(p1, p2, p3, rgb_to_hex(p1.x+p3.y/((u32)timer_ticks*40), p2.x+p2.y/((u32)timer_ticks*30), p3.x+p1.y*((u32)timer_ticks*30)));
        return;
    }
    vec2 mid12 = midpoint(p1, p2);
    vec2 mid23 = midpoint(p2, p3);
    vec2 mid31 = midpoint(p3, p1);
    
    sierpinski(p1, mid12, mid31, depth - 1);
    sierpinski(mid12, p2, mid23, depth - 1);
    sierpinski(mid31, mid23, p3, depth - 1);
}
