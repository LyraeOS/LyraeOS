#include <drivers/input/mouse.h>
#include <drivers/display/gfx.h>
#include <drivers/display/tty.h>
#include <cpu/idt.h>
#include <kernel/command.h>

mouse_state_t mouse_state = {0};
uint32_t mouse_prev_pixels[CURSOR_WIDTH][CURSOR_HEIGHT] = {0};

void mouse_init() {
    mouse_state.enabled = false;
    mouse_wait(PS2_WAIT_WRITE);
    outb(PS2_SC_REG, 0xA8); // enable mouse

    // request config byte
    mouse_wait(PS2_WAIT_WRITE);
    outb(PS2_SC_REG, 0x20);
    // enable mouse interrupts
    mouse_wait(PS2_WAIT_READ);
    uint8_t config = inb(PS2_DATA);

    config |= 0x02; // enable mouse interrupts
    config &= ~0x20; // enable mouse clock line
    config |= 0x01; // keep keyboard interrupts
    
    // define byte config
    mouse_wait(PS2_WAIT_WRITE);
    outb(PS2_SC_REG, 0x60);
    mouse_wait(PS2_WAIT_WRITE);
    outb(PS2_DATA, config);

    // send command to mouse
    mouse_wait(PS2_WAIT_WRITE);
    outb(PS2_SC_REG, 0xD4);
    //enable streaming
    mouse_wait(PS2_WAIT_WRITE);
    outb(0x60, 0xF4);

    mouse_wait(PS2_WAIT_READ);
    inb(PS2_DATA);
};

void mouse_save_position() {
    for (int x = 0; x < CURSOR_WIDTH; x++) {
        for (int y = 0; y < CURSOR_HEIGHT; y++) {
            mouse_prev_pixels[x][y] = gfx_get_pixel(mouse_state.x + x, mouse_state.y + y);
        }
    }
}

void mouse_clear_trail(int px, int py) {
    for (int x = 0; x < CURSOR_WIDTH; x++) {
        for (int y = 0; y < CURSOR_HEIGHT; y++) {
            gfx_set_pixel(px + x, py + y, mouse_prev_pixels[x][y]);
        }
    }
}

void mouse_handler() {
    mouse_state.data = inb(0x60);

    if (mouse_state.cycle == 0 && !(mouse_state.data & 0x08)) {
        pic_send_eoi(44);
        return;
    };

    mouse_state.packet[mouse_state.cycle++] = mouse_state.data;

    if (mouse_state.cycle == 3) {
        mouse_state.cycle = 0;

        mouse_state.left_click = (mouse_state.packet[0] & 0x1);
        mouse_state.right_click = (mouse_state.packet[0] & 0x2);
        mouse_state.middle_click = (mouse_state.packet[0] & 0x4);

        int dx = (int)(char)mouse_state.packet[1];
        int dy = (int)(char)mouse_state.packet[2];

        int px = mouse_state.x;
        int py = mouse_state.y;
        if (mouse_state.enabled) mouse_clear_trail(px, py); 


        mouse_state.x += dx;
        mouse_state.y -= dy;

        ScreenSize ss = tty_get_screen_size();

        // clamp within screen
        // yes, i know its spaghetti, ill fix it later
        mouse_state.x = (mouse_state.x < 0) ? 0 : ((mouse_state.x > (int)ss.x - CURSOR_WIDTH - 1) ? (int)ss.x - CURSOR_WIDTH - 1 : mouse_state.x);
        mouse_state.y = (mouse_state.y < 0) ? 0 : ((mouse_state.y > (int)ss.y - CURSOR_HEIGHT - 1) ? (int)ss.y - CURSOR_HEIGHT - 1 : mouse_state.y);

        if (mouse_state.enabled) {
            mouse_save_position();
            mouse_draw(0xffffff);
        }
    }
    pic_send_eoi(44);
}

// 9x9 square until fancier cursor
void mouse_draw(uint32_t color) {
    uint32_t main_color = color;
    uint32_t secondary = 0x000000;
    if (mouse_state.left_click) {
        main_color = secondary;
        secondary = color;
    }
    gfx_draw_rectangle_filled(vec2_new(mouse_state.x, mouse_state.y),
                              vec2_new(mouse_state.x + CURSOR_WIDTH, mouse_state.y + CURSOR_HEIGHT),
                              secondary);
    gfx_draw_rectangle_filled(vec2_new(mouse_state.x + 2, mouse_state.y + 2),
                              vec2_new(mouse_state.x + CURSOR_WIDTH - 2, mouse_state.y + CURSOR_HEIGHT - 2),
                              main_color);
}

void mouse_set_enable(bool en) {
    mouse_state.enabled = en;
};

mouse_state_t* mouse_get_state() {
    return &mouse_state;
};

COMMAND(cur, "enables the mouse cursor") {
    mouse_state_t* state = mouse_get_state();
    mouse_set_enable(!state->enabled);
    tty_clear();
    return 0;
}
