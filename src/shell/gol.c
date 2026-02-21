
#include "gol.h"
#include <stdbool.h>
#include "mem.h"
#include "screen/gfx.h"
#include "util.h"
#include "intr/idt.h"

static inline st cell_index(int x, int y, int grid_y)
{
    return (st)x * (st)grid_y + (st)y;
}

static inline bool bit_get(u8 *arr, st i)
{
    return (arr[i >> 3] >> (i & 7)) & 1;
}

static inline void bit_set(u8 *arr, st i, bool v)
{
    if (v)
        arr[i >> 3] |= (1u << (i & 7));
    else
        arr[i >> 3] &= ~(1u << (i & 7));
}
void ConwaysMain(int cell_size)
{
    ScreenScale screen_scale = tty_get_screen_size();
    int grid_x = screen_scale.x / cell_size;
    int grid_y = screen_scale.y / cell_size;
    u8 grid[(grid_x * grid_y + 7) / 8];
    for (int x = 0; x < grid_x; x++)
    {
        for (int y = 0; y < grid_y; y++)
        {
            u64 v = (u64)x * 0x27d4eb2d;
            v ^= (u64)y * 0x165667b19e3779f9ULL;
            v ^= timer_ticks;
            v ^= v >> 33;
            v *= 0xff51afd7ed558ccdULL;
            v ^= v >> 33;

            bool alive = ((v & 3) == 0);
            st i = cell_index(x, y, grid_y);
            bit_set(grid, i, alive);

            u32 color = alive ? 0xFF00FF : 0x000000;
            gfx_draw_rectangle_filled(
                vec2_new(x * cell_size, y * cell_size),
                vec2_new(x * cell_size + cell_size, y * cell_size + cell_size),
                color);
        }
    }
    u8 temp[(grid_x * grid_y + 7) / 8];
    while (true)
    {
        if (!keyboard_empty(&keypress_queue))
        {
            if (keyboard_pop(&keypress_queue) == 'q')
                return;
        }
        // size_t start = timer_ticks;
        for (int x = 0; x < grid_x; x++)
        {
            for (int y = 0; y < grid_y; y++)
            {
                int amount = 0;

                for (int xi = -1; xi <= 1; xi++)
                {
                    for (int yi = -1; yi <= 1; yi++)
                    {
                        if (xi == 0 && yi == 0)
                            continue; // don’t count self
                        int nx = x + xi;
                        int ny = y + yi;
                        if (nx >= 0 && ny >= 0 && nx < grid_x && ny < grid_y)
                        {
                            st ni = cell_index(nx, ny, grid_y);
                            if (bit_get(grid, ni))
                                amount++;
                        }
                    }
                }

                st i = cell_index(x, y, grid_y);
                bool alive = bit_get(grid, i);

                bool next =
                    alive ? (amount == 2 || amount == 3)
                          : (amount == 3);

                bit_set(temp, i, next);

                if (alive != next)
                {
                    u32 color = next ? 0xFFFFFF : (u32) timer_ticks * 20;
                    gfx_draw_rectangle_filled(
                        vec2_new(x * cell_size, y * cell_size),
                        vec2_new(x * cell_size + cell_size, y * cell_size + cell_size),
                        color);
                }
            }
        }
        //wait_ms(10);
        //  kprintf("{d}\n",timer_ticks-start);
        memcpy(grid, temp, (grid_x * grid_y + 7) / 8);
    }
    return;
}