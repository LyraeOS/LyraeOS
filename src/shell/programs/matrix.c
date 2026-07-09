#include <shell/programs/matrix.h>
#include <kernel/command.h>

void matrix() {
    TTYTheme* cur = tty_cur_theme();
    while (true) {
        if (!keyboard_empty())
        {
            if (keyboard_pop() == 'q')
                return;
        }
        ScreenSize sc = tty_get_screen_size();
        ScalingInfo si = gfx_get_scaling_info();
        size_t x_chars = sc.x / si.width;
        size_t y_chars = sc.y / si.height;
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
                    gfx_draw_character(characters[v % 2], x*si.width, y*si.height, cur->accent, cur->bg);
                } else {
                    gfx_draw_character(characters[v % 2], x*si.width, y*si.height, cur->bg, cur->bg);
                }
                
            }
        }
        wait_ms(100);
    }
}

COMMAND(matrix, "does a matrix kinda thing") {
    tty_set_cursor_enabled(false);
    matrix();
    tty_set_cursor_enabled(true);
    tty_clear();
    return 0;
}
