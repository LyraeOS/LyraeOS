#include <shell/programs/minesweeper.h>
#include <drivers/input/mouse.h>
#include <kernel/command.h>
#include <drivers/display/tty.h>


#define GRIDX 30
#define GRIDY 19
#define RECTX 40
#define RECTY 40

static int grid[GRIDX][GRIDY];
const int neighbors[8][8] = { //Array for what indexes to check
    {-1,-1},
    { 0,-1},
    { 1,-1},
    {-1, 0},

    { 1, 0},
    {-1, 1},
    { 0, 1},
    { 1, 1}
};

int amountClicked = 0;
int amountNeeded = (GRIDX * GRIDY) - ((GRIDX * GRIDY * 3)/10);

void drawFlag(int x, int y) {
    gfx_draw_rectangle_filled(vec2_new(x * RECTX, y * RECTY), vec2_new(x * RECTX + RECTX, y * RECTY + RECTY), 0x995511);
}

int flatten2D(int x, int y) {
    return x + y * GRIDY;
}

void drawSquare(int x, int y) {
    if (grid[x][y] >= 10) {
        drawFlag(x,y);
        return;
    }
    bool color = true;
    if ((x + y) % 2) {
        color = false;
    }
    int num = grid[x][y];
    if (num >= 0 && num <= 9) {
        if (color) {
            gfx_draw_rectangle_filled(vec2_new(x * RECTX, y * RECTY), vec2_new(x * RECTX + RECTX, y * RECTY + RECTY), 0x11AA11);
        } else {
            gfx_draw_rectangle_filled(vec2_new(x * RECTX, y * RECTY), vec2_new(x * RECTX + RECTX, y * RECTY + RECTY), 0x119911);
        }
        return;
    } else {
        if (color) {
            gfx_draw_rectangle_filled(vec2_new(x * RECTX, y * RECTY), vec2_new(x * RECTX + RECTX, y * RECTY + RECTY), 0xFFFFFF);
        } else {
            gfx_draw_rectangle_filled(vec2_new(x * RECTX, y * RECTY), vec2_new(x * RECTX + RECTX, y * RECTY + RECTY), 0xDDDDDD);
        }
        if (grid[x][y] != -10) {
            gfx_draw_character_transparent('0' + grid[x][y] * -1, x * RECTX + RECTX/2 - 8, y * RECTY + RECTY/2 - 8, 0x000000);
        }
    }
}

void drawGrid(void) {
    for (size_t x = 0; x < GRIDX; x++) {
        for (size_t y = 0; y < GRIDY; y++) {
            drawSquare(x,y);
        }
    }
}

int click(int x, int y) {
    if (grid[x][y] == 9) {
        return 2;
    } else if (grid[x][y] == -10) {
        return 1;
    } else if (grid[x][y] > 9) {
        return 1; //not sure what you want the return code to be?
    } else {
        amountClicked++;
        if (grid[x][y] > 0) {
            grid[x][y] *= -1;
        } else if (grid[x][y] == 0) {
            grid[x][y] = -10;
            drawSquare(x,y);
            for (int i = 0; i < 8; i++) {//Clicking neighbors
                int xi = neighbors[i][0], yi = neighbors[i][1];
                if (x + xi >= GRIDX || x + xi < 0 || y + yi < 0 || y + yi >= GRIDY || grid[x + xi][y + yi] >= 10 || grid[x+xi][y+yi] == -10) {
                    continue;
                }
                wait_ms(10);
                click(x+xi,y+yi);
                drawSquare(x + xi, y + yi);
            }
        }
    }
    return 1;
}

void flag(int x, int y) {
    if (grid[x][y] < 0) {
        return;
    }
    if (grid[x][y] < 10) {
        grid[x][y] *= 10;
    } else {
        grid[x][y] /= 10;
    }
    return;
}

void minesweeperMain(void) {
    tty_clear();
    amountClicked = 0;
    int numMines = (GRIDX * GRIDY * 3)/10;
    while (numMines>0){
        for (int x = 0; x < GRIDX; x++)
        {
            for (int y = 0; y < GRIDY; y++)
            {
                if (numMines <= 0) {
                    break;
                }
                uint64_t v = (uint64_t)x * 0x27d4eb2d;
                v ^= (uint64_t)y * 0x165667b19e3779f9ULL;
                v ^= timer_ticks;
                v ^= v >> 33;
                v *= 0xff51afd7ed558ccdULL;
                v ^= v >> 33;

                if (v%11 == 0 && grid[x][y] != 9) {
                    numMines--;
                    grid[x][y] = 9;
                }
            }
        }
    }
    for (int x = 0; x < GRIDX; x++) { //Defines squares as numbers for how many mines are around them
        for (int y = 0; y < GRIDY; y++) {
            if (grid[x][y] != 9) {
                int amount = 0;
                for (int i = 0; i < 8; i++) {//Checking neighbors
                    int xi = neighbors[i][0], yi = neighbors[i][1];
                    if (grid[x+xi][y+yi] == 9) {
                        amount++;
                    }
                }
                grid[x][y] = amount;
            }
        }
    }

    drawGrid();
    bool clicked = false;
    int xc = 0, yc = 0;
    while (!clicked) {
        xc += 1;
        if (xc >= GRIDX) {
            xc = 0;
            yc++;
        }
        if (yc >= GRIDY) {
            yc = 0;
        }
        uint64_t v = (uint64_t)xc * 0x27d4eb2d;
        v ^= (uint64_t)yc * 0x165667b19e3779f9ULL;
        v ^= timer_ticks;
        v ^= v >> 33;
        v *= 0xff51afd7ed558ccdULL;
        v ^= v >> 33;

        if (v % 3 == 0 && grid[xc][yc] == 0) {
            click(xc,yc);
            xc = xc, yc = yc;
            clicked = true;
        }
    }

    bool prev = mouse_get_state()->enabled;
    mouse_set_enable(true);
    int temp = 1;
    while (temp == 1 && amountClicked < amountNeeded) {
        mouse_state_t* state = {0};
        state = mouse_get_state(); 
        while (!state->left_click && !state->right_click && keyboard_empty()) {
            xc = state->x / RECTX;
            yc = state->y / RECTY;
            state = mouse_get_state();
        }
        char pressed = '\0';
        if (!keyboard_empty())
            pressed = keyboard_pop();
        if (state->left_click) {
            temp = click(xc, yc);
        } else if (state->right_click) {
            flag(xc, yc);
        } else if (pressed == ';') {
            for (int x = 0; x < GRIDX; x++) { //Defines squares as numbers for how many mines are around them
                for (int y = 0; y < GRIDY; y++) {
                    if (grid[x][y] == 9) {
                        flag(x,y);
                    }
                    drawSquare(x,y);
                }
            }
        } else if (pressed == ':') {
            for (int x = 0; x < GRIDX; x++) { //Defines squares as numbers for how many mines are around them
                for (int y = 0; y < GRIDY; y++) {
                    if (grid[x][y] == 9) {
                        flag(x,y);
                    } else {
                        click(x,y);
                        wait_ms(5);
                    }
                    drawSquare(x,y);
                }
            }
        }
        drawSquare(xc, yc);
        wait_ms(100);
    }
    
    for (int x = 0; x < GRIDX; x++) {
        for (int y = 0; y < GRIDY; y++) {
            grid[x][y] = 0;
        }
    }

    if (amountClicked >= amountNeeded) {
        mouse_set_enable(prev);
        ConwaysMain(2);
        tty_clear();
    } else {
        minesweeperMain();
    }
}



COMMAND(mine, "runs minesweeper") {
    tty_set_cursor_enabled(false);
    minesweeperMain();
    tty_set_cursor_enabled(true);
    return 0;
}
