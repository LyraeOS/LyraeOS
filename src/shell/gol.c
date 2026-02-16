
#include "gol.h"
#include <stdbool.h>
#include "mem.h"
#include "screen/gfx.h"
#include "util.h"
#include "intr/idt.h"

#define CELLSIZE 5
#define GRIDX (1280 / CELLSIZE)
#define GRIDY (800 / CELLSIZE)

static bool grid[GRIDX][GRIDY]; // It's techincally slightly faster to do a 1d array

void ConwaysMain(void)
{
    for (int x = 0; x < GRIDX; x++)
    {
        for (int y = 0; y < GRIDY; y++)
        {
            // kprintf("{d}\n", x + y + timer_ticks);
            // wait_ms(x+y);
            uint64_t v = (uint64_t)x * 0x27d4eb2d;
            v ^= (uint64_t)y * 0x165667b19e3779f9ULL;
            v ^= timer_ticks;

            v ^= v >> 33;
            v *= 0xff51afd7ed558ccdULL;
            v ^= v >> 33;
            if ((v & 3) == 0)
            {
                grid[x][y] = true;
                gfx_draw_rectangle_filled(vec2_new(x * CELLSIZE, y * CELLSIZE), vec2_new(x * CELLSIZE + CELLSIZE, y * CELLSIZE + CELLSIZE), 0xFF00FF);
            }
            else
            {
                grid[x][y] = false;
                gfx_draw_rectangle_filled(vec2_new(x * CELLSIZE, y * CELLSIZE), vec2_new(x * CELLSIZE + CELLSIZE, y * CELLSIZE + CELLSIZE), 0x000000);
            }
        }
    }
    while (true)
    {
        if (!keyboard_empty(&keypress_queue)) {
            if (keyboard_pop(&keypress_queue) == 'q')
                return;
        }
        // size_t start = timer_ticks;
        static bool temp[GRIDX][GRIDY];
        for (int x = 0; x < GRIDX; x++)
        {
            for (int y = 0; y < GRIDY; y++)
            {
                int amount = 0;
                for (int xi = -1; xi <= 1; xi++)
                { // I probably should've done a array for this
                    for (int yi = -1; yi <= 1; yi++)
                    {
                        if (x + xi >= 0 && y + yi >= 0 && x + xi < GRIDX && y + yi < GRIDY)
                        {
                            if (grid[x + xi][y + yi])
                            {
                                amount++;
                            }
                        }
                    }
                }
                if (grid[x][y])
                {
                    // kprintf("{d}{d}\n",grid[x][y], amount);
                    if (amount == 3 || amount == 4)
                    {
                        temp[x][y] = true;
                    }
                    else
                    {
                        temp[x][y] = false;
                        gfx_draw_rectangle_filled(vec2_new(x * CELLSIZE, y * CELLSIZE), vec2_new(x * CELLSIZE + CELLSIZE, y * CELLSIZE + CELLSIZE), (uint32_t)timer_ticks*100);
                    }
                }
                else if (amount == 3)
                {
                    temp[x][y] = true;

                    uint8_t r = (uint32_t)timer_ticks % 256;
                    uint8_t g = ((uint32_t)timer_ticks/ 20) % 256;
                    uint8_t b = ((x + y + timer_ticks / 10) * 5) % 256;

                    uint32_t color = (r << 16) | (g << 8) | b;

                    gfx_draw_rectangle_filled(
                        vec2_new(x * CELLSIZE, y * CELLSIZE),
                        vec2_new(x * CELLSIZE + CELLSIZE, y * CELLSIZE + CELLSIZE),
                        color);
                }
                else
                {
                    temp[x][y] = false;
                }
                //wait_ms(10);
            }
        }
        // define grid as temp but don't do a pointer
        // How would I use memcpy for this
        for (int x = 0; x < GRIDX; x++)
        {
            for (int y = 0; y < GRIDY; y++)
            {
                grid[x][y] = temp[x][y];
            }
        }
        //wait_ms(100);
        // kprintf("{d}\n",timer_ticks-start);
    }
    return;
}