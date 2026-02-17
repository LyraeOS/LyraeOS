#include "matrix.h"

void matrix() {
    while (true) {
        if (!keyboard_empty(&keypress_queue))
        {
            if (keyboard_pop(&keypress_queue) == 'q')
                return;
        }
        ScreenScale sc = tty_get_screen_size();
        size_t x_chars = sc.x / SCALED_WIDTH;
        size_t y_chars = sc.y / SCALED_HEIGHT;
        char characters[] = {'0', '1'};
        for (size_t x = 0; x < x_chars; x++) {
            for (size_t y = 0; y < y_chars; y++) {
                uint64_t v = (uint64_t)x * 0x27d4eb2d;
                v ^= (uint64_t)y * 0x165667b19e3779f9ULL;
                v ^= timer_ticks;
                v ^= v >> 33;
                v *= 0xff51afd7ed558ccdULL;
                v ^= v >> 33;
                if (v % 5 == 0) {
                    gfx_draw_character(characters[v % 2], x*SCALED_WIDTH, y*SCALED_HEIGHT, 0x00FF00, 0x000000);
                } else {
                    gfx_draw_character(characters[v % 2], x*SCALED_WIDTH, y*SCALED_HEIGHT, 0x000000, 0x000000);
                }
                
            }
        }
        wait_ms(100);
    }
}